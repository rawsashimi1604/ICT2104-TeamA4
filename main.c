/* DriverLib Includes */
#include "driverlib.h"

// for MAP_ prefix functions
#include "rom_map.h"

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Ultrasonic module
#include "UltrasonicModule/ultrasonic.h"
#include "Utility/time_utility.h"

extern uint32_t sensor1InterruptCount;

// Motor module
#include "MotorModule/motor.h"

// Infrared module
#include <InfraredModule/barcode.h>
#include <InfraredModule/encoder.h>

extern volatile int g_time_ms;
extern volatile int g_b_notch_counting;
extern volatile Encoder *p_left_encoder;
extern volatile Encoder *p_right_encoder;

// Communications module
#include "CommunicationsModule/comm.h"

extern char g_userInput[5];
extern int g_inputCounter;
extern int g_inputX;
extern int g_inputY;

// mapping data structures
#include "MappingModule/queue.h"
#include "MappingModule/stack.h"
#include "MappingModule/graph.h"
#include "MappingModule/barcode_linkedlist.h"
#include "MappingModule/hump_linkedlist.h"

#include "helper.h"

// /* ====================================================================== */

// Mapping module interface
void Mapping_writeBarcodeData(char data);
void Mapping_writeHumpData(void);

/* =================================== */

// add our function declarations here in the future
void updateCarDirection(char turn);
void initializeCarPosition(void);
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoLeft, bool canGoRight, Graph *graph);
void mapMaze(Vertex *start, Graph *graph);
bool driveCarUsingPath(int *listOfCoords, int numberOfCoords);
bool bfs(Vertex *sourceV, Vertex *endV, Graph *graph);

/* =================================== */
HumpList *humpData = NULL;
BarcodeList *barCodeData = NULL;
char buffer[100] = { 0 };

// char carDirection = 'N', 'E', 'S', 'W'
// dfs use bfs to backtrack to the vertex
// bfs will drive the car if there is a possible path to the destination and return true
// else it will return false
// assume bfs will not mark vertices as visited on the global graph data structure

char carDirection = 'N';
Vertex *carCurrentPosition; // points to the vertex where the car is currently at

// Kaho
void Mapping_writeBarcodeData(char data)
{
    BarcodeList_addNode(carCurrentPosition->x, carCurrentPosition->y, data, barCodeData);
}

// Kaho
void Mapping_writeHumpData(void)
{
    HumpList_addNode(carCurrentPosition->x, carCurrentPosition->y, humpData);
}

// owner            : Kevin
// description      : takes in the car's current position and
//                    updates the ajd list of the vertex based on ultra sensors and car orientation.
//
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoLeft,
               bool canGoRight, Graph *graph)
{
    int x = currentPos->x;
    int y = currentPos->y;

    // if the car is facing north
    if (carDirection == 'N')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
    }

    // if the car is facing south
    else if (carDirection == 'S')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
    }

    // if the car is facing east
    else if (carDirection == 'E')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
    }

    // if the car is facing west
    else if (carDirection == 'W')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
    }
    else
    {
        // debug
//        printf("ERROR: updateMap failed - Unknown car direction.\n");
        return;
    }

    // debug
//    printf("updateMap completed sucessfully.\n");
}

// owner            : Kaho
// description      : drives the car based on the map using dfs
void mapMaze(Vertex *start, Graph *graph)
{
    // Map initialization for starting conditions
    UtilityTime_delay(60);
    bool canGoFront = Ultrasonic_checkFront();
    UtilityTime_delay(60);
    bool canGoBack = Ultrasonic_checkBack();
    UtilityTime_delay(60);
    bool canGoLeft = Ultrasonic_checkLeft();
    UtilityTime_delay(60);
    bool canGoRight = Ultrasonic_checkRight();
    UtilityTime_delay(60);

    carCurrentPosition = Graph_addVertex(0, 0, graph);
    updateMap(carCurrentPosition, canGoFront, canGoLeft, canGoRight, graph);

    // modified dfs starting condition to suit maze
    Stack *s = Stack_makeStack();
    start = carCurrentPosition;
    start->visited = true;
    graph->numberOfNodesVisited++;

    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        if (start->adjacencyList[i] == NULL)
            break;
        Stack_push(start->adjacencyList[i], s);
    }

    int *pathCoordinate = malloc(2 * 2 * sizeof(int));

    bool isAdj = false;

    // dfs
    while (s->size != 0)
    {
        isAdj = false;

        Vertex *v = Stack_pop(s);
        if (v->visited == true)
            continue;

        // debugging
//        snprintf(buffer, 100, "Car is currently at (%d, %d)", carCurrentPosition->x, carCurrentPosition->y);
//        printf("%s\n", buffer);
//        Vertex_writeStrToBuff(buffer, v);
//        printf("visiting: %s\n", buffer);

        // pathCoordinate[0 * 2 + 0] means pathCoordinate[0][0]
        // pathCoordinate[0 * 2 + 1] means pathCoordinate[0][1]
        // pathCoordinate[1 * 2 + 0] means pathCoordinate[1][0]
        // pathCoordinate[1 * 2 + 1] means pathCoordinate[1][1]
        pathCoordinate[0 * 2 + 0] = carCurrentPosition->x;
        pathCoordinate[0 * 2 + 1] = carCurrentPosition->y;
        pathCoordinate[1 * 2 + 0] = v->x;
        pathCoordinate[1 * 2 + 1] = v->y;

        for (i = 0; i < 4; i++)
        {
            if (carCurrentPosition->adjacencyList[i] == NULL)
                break;
            if (carCurrentPosition->adjacencyList[i] == v)
            {
                driveCarUsingPath(pathCoordinate, 2);
                isAdj = true;
                break;
            }
        }

        if (!isAdj)
        {
            // driveCarUsingPath will drive me to the vertex I want to visit
            // returns true if that is possible
            // returns false if that is not possible
            // BFS will drive me there if I am not adjacent to the vertex
            bfs(carCurrentPosition, v, graph);
        }

        v->visited = true;
        graph->numberOfNodesVisited++;
        carCurrentPosition = v;

        // check surroundings and update the map accordingly
        canGoFront = Ultrasonic_checkFront();
        UtilityTime_delay(60);
        canGoBack = Ultrasonic_checkBack();
        UtilityTime_delay(60);
        canGoLeft = Ultrasonic_checkLeft();
        UtilityTime_delay(60);
        canGoRight = Ultrasonic_checkRight();
        UtilityTime_delay(60);

        updateMap(carCurrentPosition, canGoFront, canGoLeft, canGoRight, graph);

        for (i = 0; i < 4; i++)
        {
            if (v->adjacencyList[i] == NULL)
                break;
            if (!v->adjacencyList[i]->visited)
            {
                Stack_push(v->adjacencyList[i], s);

                // debugging
//                printf("DFS current Vertex at: ");
//                Vertex_writeStrToBuff(buffer, v);
//                printf("%s\n", buffer);
//                printf("DFS pushing V->adj[%d]: ", i);
//                Vertex_writeStrToBuff(buffer, v->adjacencyList[i]);
//                printf("%s\n", buffer);
            }
        }
    }

    free(pathCoordinate);
    Stack_destroy(s);
}

// owner: kevin
// desc:  turn the car to the direction that is given
bool adjustCarDirection(char directionToTurnTo)
{
    if (carDirection == 'N')
    {
        if (directionToTurnTo == 'S')
        {
            Motor_turnLeft();
            Motor_turnLeft();
            carDirection = 'S';
        }
        else if (directionToTurnTo == 'E')
        {
            Motor_turnRight();
            carDirection = 'E';
        }
        else if (directionToTurnTo == 'W')
        {
            Motor_turnLeft();
            carDirection = 'W';
        }
    }
    else if (carDirection == 'E')
    {
        if (directionToTurnTo == 'N')
        {
            Motor_turnLeft();
            carDirection = 'N';
        }
        else if (directionToTurnTo == 'S')
        {
            Motor_turnRight();
            carDirection = 'S';
        }
        else if (directionToTurnTo == 'W')
        {
            Motor_turnLeft();
            Motor_turnLeft();
            carDirection = 'W';
        }
    }
    else if (carDirection == 'S')
    {
        if (directionToTurnTo == 'N')
        {
            Motor_turnLeft();
            Motor_turnLeft();
            carDirection = 'N';
        }
        else if (directionToTurnTo == 'E')
        {
            Motor_turnLeft();
            carDirection = 'E';
        }
        else if (directionToTurnTo == 'W')
        {
            Motor_turnRight();
            carDirection = 'W';
        }
    }
    else if (carDirection == 'W')
    {
        if (directionToTurnTo == 'N')
        {
            Motor_turnRight();
            carDirection = 'N';
        }
        else if (directionToTurnTo == 'S')
        {
            Motor_turnLeft();
            carDirection = 'S';
        }
        else if (directionToTurnTo == 'E')
        {
            Motor_turnLeft();
            Motor_turnLeft();
            carDirection = 'E';
        }
    }
    return true;
}

// owner            : Kevin
// description      : drive car following the path
// input            : it should take in a list of x y coordinates
bool driveCarUsingPath(int *listOfCoords, int numberOfCoords)
{
    // declaring the require elements
    int numberOfDirections = numberOfCoords - 1; // number of directions = number of coords -1

    // prevent malloc of < 0
    if (numberOfDirections < 0)
        numberOfDirections = 0;

    // store the directions the car will need to move
    // if number of directions is positive, malloc numberOfDirections
    // else malloc just 1 byte to prevent any errors
    char *directionsArrs = malloc(sizeof(char) * (numberOfDirections > 0 ? numberOfDirections : 1));

    uint8_t i = 0;
    // go through the coords and create the char array to store the directions
    for (i = 0; i < numberOfDirections; i++)
    {
        // minus the x cord of 2 coords. if not 0 (false), car moves in x axis
        if ((listOfCoords[(i + 1) * 2 + 0] - listOfCoords[i * 2 + 0]) && !(listOfCoords[(i + 1) * 2 + 1] - listOfCoords[i * 2 + 1]))
        {
            // if x is positive, move W, if neg, move E
            switch (listOfCoords[(i + 1) * 2 + 0] - listOfCoords[i * 2 + 0])
            {
            case -1:
                directionsArrs[i] = 'W';
                break;
            case 1:
                directionsArrs[i] = 'E';
                break;
            default:

                // debug
//                printf("ERROR: driveCarUsingPath have a coords that skip a step. \n");
                free(directionsArrs);
                return false;
            }
        }
        // minus the y cord of 2 coords. if not 0 (false), car moves in y axis
        else if ((listOfCoords[(i + 1) * 2 + 1] - listOfCoords[i * 2 + 1]) && !(listOfCoords[(i + 1) * 2 + 0] - listOfCoords[i * 2 + 0]))
        {
            // if x is positive, move W, if neg, move E
            switch (listOfCoords[(i + 1) * 2 + 1] - listOfCoords[i * 2 + 1])
            {
            case -1:
                directionsArrs[i] = 'S';
                break;
            case 1:
                directionsArrs[i] = 'N';
                break;
            default:

                // debug
//                printf("ERROR: driveCarUsingPath have a coords that skip a step. \n");
                free(directionsArrs);
                return false;
            }
        }
        else
        {
            // coord somehow wants to move diagonally
            // debug
//            printf("ERROR: driveCarUsingPath wants to move diagonally. \n");
            free(directionsArrs);
            return false;
        }
    }

    // For debugging purposes, prints the directions the car will need to move.
    // printf("numberOfDirections = %d\n", numberOfDirections);
//    printf("driveCarUsingPath will move car in: ");
//    for (i = 0; i < numberOfDirections; i++)
//    {
//        printf("%c ", directionsArrs[i]);
//    }
//    printf("\n");

    // given the list of directions, move the car and adjust the orientation accordingly
    for (i = 0; i < numberOfDirections; i++)
    {
        // car is facing the right direction. move up
        if (carDirection == directionsArrs[i])
        {
            Motor_driveForward();
        }
        else
        {
            adjustCarDirection(directionsArrs[i]);
            Motor_driveForward();
        }
    }
    free(directionsArrs);
    return true; // end of driveCarUsingPath
}

// owner            : Kevin
// description      : takes in the sources vertex, and target vertex. If the car can move there,
//                    drives the car there and returns true. Otherwise, returns false.
bool bfs(Vertex *sourceV, Vertex *targetV, Graph *graph)
{

    // Declare required structures and var
    // Custom list that tracks vertices that are visited by BFS, and their parent vertices.
    struct VisitedNode
    {
        Vertex *vertexPointer;            // holds the pointer to the vertex
        struct VisitedNode *parentVertex; // holds the pointer to the parent of the vertex
        struct VisitedNode *next;
    };
    typedef struct VisitedNode *node;
    node visitedListHead = NULL;  // pointer used as the head of the list
    node tempVisitedNode1 = NULL; // pointer used for various operations
    node tempVisitedNode2 = NULL; // pointer used for various operations
    Vertex *tempV = NULL;         // pointer used for various operations

    Queue *queue1 = Queue_makeQueue(); // main queue used for BFS
    if (queue1 == NULL)
    {
        // debug
//        printf("BFS queue allocation failed. \n");
        return false;
    }
    Stack *stack1 = Stack_makeStack(); // used to recontruct path later on.
    if (stack1 == NULL)
    {
        // debug
//        printf("BFS stack allocation failed. \n");
        return false;
    }

    // push the source node onto the queue, then mark as visited by adding to the list
    Queue_enqueue(sourceV, queue1);
    tempVisitedNode1 = (node) malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
    if (tempVisitedNode1 == NULL)
    {
        // debug
//        printf("BFS tempVisitedNode1 allocation failed. \n");
        // garbage collection
        while (visitedListHead != NULL)
        {
            tempVisitedNode1 = visitedListHead;
            visitedListHead = visitedListHead->next;
            free(tempVisitedNode1);
        }
        Queue_destroy(queue1);
        Stack_destroy(stack1);
        return false;
    }
    tempVisitedNode1->vertexPointer = sourceV; // set the pointer (sourceV) that this node represents
    tempVisitedNode1->parentVertex = NULL;     // sourceV does not have a parent
    tempVisitedNode1->next = NULL;
    visitedListHead = tempVisitedNode1; // setting the head of the list
    tempVisitedNode1 = NULL;

    uint8_t i = 0;
    // Loop till queue is empty
    // check is the queue is empty (size = 0)
    while (queue1->size != 0)
    {
        // dequeue and check if the the pointer is the targetV they are looking for
        tempV = Queue_dequeue(queue1);
        if (tempV == targetV)
        {
            // targetV has been found. push v pointers to the stack based on vistedList parent

            tempVisitedNode1 = visitedListHead; // setting a pointer to head
            // look for tempV in the visitedList, set as tempVisitedNode1
            while (tempVisitedNode1->vertexPointer != tempV)
            {
                tempVisitedNode1 = tempVisitedNode1->next; // move down the list

                // checking for error case
                if ((tempVisitedNode1->next == NULL)
                        && (tempVisitedNode1->vertexPointer != tempV))
                {
                    // debug
//                    printf("ERROR: tempV cannot be found in VistedNode linked list.\n");
                    // garbage collection
                    while (visitedListHead != NULL)
                    {
                        tempVisitedNode1 = visitedListHead;
                        visitedListHead = visitedListHead->next;
                        free(tempVisitedNode1);
                    }
                    Queue_destroy(queue1);
                    Stack_destroy(stack1);
                    return false;
                }
            }
            // reconstructing shortest path
            // moving back up to source using parent pointer
            while (tempVisitedNode1 != NULL)
            {
                // pushing vertices onto stack on by one
                if (!Stack_push(tempVisitedNode1->vertexPointer, stack1))
                {
                    // debug
//                    printf("ERROR: BFS Stack allocation failed.");

                    // garbage collection
                    while (visitedListHead != NULL)
                    {
                        tempVisitedNode1 = visitedListHead;
                        visitedListHead = visitedListHead->next;
                        free(tempVisitedNode1);
                    }
                    Queue_destroy(queue1);
                    Stack_destroy(stack1);
                    return false;
                }
                tempVisitedNode1 = tempVisitedNode1->parentVertex;
            }

            // int listOfCoords[stack1->size][2];
            // memset(listOfCoords, 0, stack1->size * 2 * sizeof(int));

            // initialize a coord array based on the size of the stack
            int *listOfCoords = malloc(stack1->size * 2 * sizeof(int));
            int stackSize = stack1->size;
            // moving the car based on path constructed. craft a coord array and send to drive car using path
            for (i = 0; stack1->size != 0; i++)
            {
                // debug
//                printf("Size of stack: %d | ", stack1->size); // for debuggin purposes
                Stack_peak(stack1);                     // for debuggin purposes
                tempV = Stack_pop(stack1);                    // pop the stack
                listOfCoords[i * 2 + 0] = tempV->x; // add the x coord of the vertex from the stack onto the listOfCoords
                listOfCoords[i * 2 + 1] = tempV->y; // add the y coord of the vertex from the stack onto the listOfCoords
            }
            // passing the pointer of the listOfCoords to the drive function to drive
            // printf("Number of Coords being passed in: %d \n", (sizeof(listOfCoords)/sizeof(listOfCoords[0])));
            if (!driveCarUsingPath(listOfCoords, stackSize))
            {
                // debug
//                printf("ERROR: BFS listOfCoords has wrong coords.\n");
                free(listOfCoords);
                return false;
            }
            // update the car's position
            carCurrentPosition = tempV;

            // garbage collection
            while (visitedListHead != NULL)
            {
                tempVisitedNode1 = visitedListHead;
                visitedListHead = visitedListHead->next;
                free(tempVisitedNode1);
            }
            Queue_destroy(queue1);
            Stack_destroy(stack1);

            // debug
//            printf("BFS completed successfully.\n");
            free(listOfCoords);
            return true; // success in BFS n drive
        }

        // targetV has not been found. iterate throu adjList of node and enqueue if visited by car and not by BFS
        for (i = 0; i < 4; i++)
        {
            if (tempV->adjacencyList[i] == NULL) // breaks if no more ajd vertices
                break;

            // queue if the vertex is visited by car and not visited by BFS yet
            // check if the adj vertex has been visited by the car or checker for the target node
            if (tempV->adjacencyList[i]->visited  || (tempV->adjacencyList[i] == targetV))
            {
                tempVisitedNode1 = visitedListHead; // set a pointer to the head of visited node list
                // interate thru visited list
                while (tempVisitedNode1 != NULL)
                {
                    if (tempV->adjacencyList[i] == tempVisitedNode1->vertexPointer) // check if the adj vertex has been visted by BFS
                        break; // adjacent vertex has already been visited has already been visited. skip it

                    if (tempVisitedNode1->next == NULL)
                    {         // check if it is the last item on the Visitedlist
                        Queue_enqueue(tempV->adjacencyList[i], queue1); // enqueue as has not been visited by BFS
                        // set as visited by adding to visited list (repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = NULL; //(repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = (node) malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
                        if (tempVisitedNode1 == NULL)
                        {
                            printf("BFS tempVisitedNode1 allocation failed. \n");
                            // garbage collection
                            while (visitedListHead != NULL)
                            {
                                tempVisitedNode1 = visitedListHead;
                                visitedListHead = visitedListHead->next;
                                free(tempVisitedNode1);
                            }
                            Queue_destroy(queue1);
                            Stack_destroy(stack1);
                        }
                        tempVisitedNode1->vertexPointer = tempV->adjacencyList[i]; // set the pointer (sourceV) that this node represents
                        // set parent visitNode, which is the ptr of VisitedNode that contains tempV
                        tempVisitedNode2 = visitedListHead; // setting the head of the list
                        while (tempVisitedNode2->vertexPointer != tempV)
                        { // move pointer till VisitedNode
                            tempVisitedNode2 = tempVisitedNode2->next;
                        }
                        tempVisitedNode1->parentVertex = tempVisitedNode2;
                        tempVisitedNode1->next = NULL;

                        tempVisitedNode2 = visitedListHead; // setting the head of the list
                        // moving tempVisitedNode2 down to last item on the list
                        while (tempVisitedNode2->next != NULL)
                        {
                            tempVisitedNode2 = tempVisitedNode2->next; // moving down list
                        }
                        tempVisitedNode2->next = tempVisitedNode1; // adding tempVisitedNode1 to back of the list

                        tempVisitedNode1 = NULL; // reset pointer
                        tempVisitedNode2 = NULL; // reset pointer

                        break;
                    }
                    tempVisitedNode1 = tempVisitedNode1->next; // go to next visitednNode
                };
            }
        }
    }
    // garbage collection
    while (visitedListHead != NULL)
    {
        tempVisitedNode1 = visitedListHead;
        visitedListHead = visitedListHead->next;
        free(tempVisitedNode1);
    }
    Queue_destroy(queue1);
    Stack_destroy(stack1);

    // debugging purposes
//    printf("ERROR: Shortest Path could not be constructed.\n");
//    printf("sourceV: ");
//    Vertex_writeStrToBuff(buffer, sourceV);
//    printf("%s\n", buffer);
//    printf("targetV: ");
//    Vertex_writeStrToBuff(buffer, targetV);
//    printf("%s\n", buffer);
//    printf("Graph: \n");
//    Graph_display(graph);
//    printf("=====  BFS DEBUG ===== \n");

    return false;
}

// owner            : Kaho
// description      : sends the graphical representation of the graph in adj representation
//                       for the Communications module to print the map
void displayMapOnM5(Graph *graph)
{
    Node *trav = graph->list->head;
    char temp[10] = { 0 };

    uint8_t i = 0;
    snprintf(buffer, 100, "Car is currently at (%d, %d)", carCurrentPosition->x, carCurrentPosition->y);

    // debug
    // printf("%s\n", buffer);
    Communication_sendMap(buffer);
    while (trav != NULL)
    {
        Vertex *v = trav->data;
        snprintf(buffer, 100, "(%d, %d): ", v->x, v->y);
        for (i = 0; i < 4; i++)
        {
            if (v->adjacencyList[i] == NULL)
                continue;
            snprintf(temp, 10, "(%d, %d) ", v->adjacencyList[i]->x, v->adjacencyList[i]->y);
            strcat(buffer, temp);
        }
        strcat(buffer, "\n");
        Communication_sendMap(buffer);

        // debug
//        printf("%s\n", buffer);
        trav = trav->next;
    }
}

void initializeAllPortsToLow(void) {
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P4, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P7, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P8, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P9, PIN_ALL16);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P10, PIN_ALL16);

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P7, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P8, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, PIN_ALL16);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P10, PIN_ALL16);
}

void TA2_0_IRQHandler(void)
{
    // Increase global interrupt count
    sensor1InterruptCount++;

    g_time_ms += 1;

    if (g_time_ms % 1000 == 0)
    {
        updateRPM(p_left_encoder);
        updateRPM(p_right_encoder);
    }

    if (g_time_ms % 500 == 0) {
        Infrared_getCarSpeed();
    }

    /* Clear interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(ULTRASONIC_TIMER_MODULE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

// DEMO
int main(void)
{
    WDT_A_holdTimer();

    LED2_init(GREEN + RED);
    LED2_off(GREEN);
    LED2_off(RED);

    Interrupt_disableMaster();

    initializeAllPortsToLow();

    Motor_init();

    Ultrasonic_init();
    UtilityTime_initDelayTimer();

    Encoder_init();

//    Barcode_init();
//    Communication_init();

    // Enable interrupts globally (set GIE bit)
    Interrupt_enableMaster();

    // allocates memory for an empty graph
    // using a linked list to store our graph
    Graph *graph = Graph_makeGraph();
    mapMaze(carCurrentPosition, graph);

    if (bfs(carCurrentPosition, Graph_addVertex(0, 0, graph), graph))
    {
        // toggle LED2 green
//        printf("bfs ok\n");
        LED2_on(GREEN);
    }
    else
    {
        // toggle LED2 red
//        printf("bfs failed\n");
        LED2_on(RED);
    }

    // debug
//    printf("======\n");

//    Graph_display(graph);
//    printf("Expected: %d == %d\n", graph->numberOfNodesVisited, graph->size);
//    printf("Explored: %s\n", Graph_isExplored(graph) ? "true" : "false");

//    printf("======\n");
    displayMapOnM5(graph);

    // frees memory related to graphz
    Graph_destroy(graph);

    PCM_gotoLPM3();
}

//// graph test
//int main(void)
//{
//    printf("graph test\n");
//
//    // allocates memory for an empty graph
//    // using a linked list to store our graph
//    Graph *graph = Graph_makeGraph();
//
//    // to add vertices to a graph with no edges
//
//    // row 1
//    Vertex *v1 = Graph_addVertex(-2, 3, graph);
//    Vertex *v2 = Graph_addVertex(-1, 3, graph);
//    Vertex *v3 = Graph_addVertex(0, 3, graph);
//    Vertex *v4 = Graph_addVertex(1, 3, graph);
//
//    // row2
//    Vertex *v5 = Graph_addVertex(-2, 2, graph);
//    Vertex *v6 = Graph_addVertex(-1, 2, graph);
//    Vertex *v7 = Graph_addVertex(0, 2, graph);
//    Vertex *v8 = Graph_addVertex(1, 2, graph);
//
//    // row 3
//    Vertex *v9 = Graph_addVertex(-2, 1, graph);
//    Vertex *v10 = Graph_addVertex(-1, 1, graph);
//    Vertex *v11 = Graph_addVertex(0, 1, graph);
//    Vertex *v12 = Graph_addVertex(1, 1, graph);
//
//    // row 4
//    Vertex *v13 = Graph_addVertex(-2, 0, graph);
//    Vertex *v14 = Graph_addVertex(-1, 0, graph);
//    carCurrentPosition = Graph_addVertex(0, 0, graph);
//    Vertex *v16 = Graph_addVertex(1, 0, graph);
//
//    // row 5
//    Vertex *v17 = Graph_addVertex(-2, -1, graph);
//    Vertex *v18 = Graph_addVertex(-1, -1, graph);
//    Vertex *v19 = Graph_addVertex(0, -1, graph);
//    Vertex *v20 = Graph_addVertex(1, -1, graph);
//
//    // note: adding an existing vertex will not do anything
//    // although the validation check here is lowkey expensive
//    Graph_addVertex(0, 0, graph);
//
//    // to add edge between 2 existing vertices in a graph
//    // row 1
//    Graph_addEdge(v1, v2);
//    Graph_addEdge(v2, v3);
//    Graph_addEdge(v3, v4);
//    Graph_addEdge(v1, v5);
//    Graph_addEdge(v3, v7);
//    Graph_addEdge(v4, v8);
//
//    // row2
//    Graph_addEdge(v9, v5);
//    Graph_addEdge(v6, v10);
//    Graph_addEdge(v7, v11);
//    Graph_addEdge(v8, v12);
//
//    // row 3
//    Graph_addEdge(v9, v13);
//    Graph_addEdge(v9, v10);
//    Graph_addEdge(v11, carCurrentPosition);
//    Graph_addEdge(v16, v12);
//
//    // row 4
//    Graph_addEdge(v13, v14);
//    Graph_addEdge(v14, carCurrentPosition);
//    Graph_addEdge(v16, v20);
//    Graph_addEdge(v17, v13);
//
//    // row 5
//    Graph_addEdge(v17, v18);
//    Graph_addEdge(v18, v19);
//    Graph_addEdge(v20, v19);
//
//     // note: adding an existing edge between two vertices will not do anything
//     // although the validation check here is lowkey expensive
//    Graph_addEdge(v20, v19);
//
////    mapMaze(carCurrentPosition, graph);
////
////    if (bfs(carCurrentPosition, Graph_addVertex(1, -1, graph), graph))
////    {
////        printf("bfs ok\n");
////    }
////    else
////    {
////        printf("bfs failed\n");
////    }
//    printf("======\n");
//
//    Graph_display(graph);
//    printf("Expected: %d == %d\n", graph->numberOfNodesVisited,
//           graph->size);
//    printf("Explored: %s\n", Graph_isExplored(graph) ? "true" : "false");
//
//    printf("======\n");
//    Communication_sendData(1, 0x414243);
////    displayMapOnM5(graph);
//
//    // frees memory related to graphz
//    Graph_destroy(graph);
//
//    PCM_gotoLPM3();
//}

// hump test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     HumpList *test = HumpList_makeList();
//     printf("hump linkedlist test\n");
//
//     // for add()/delete(), function needs the x, y coordinate as well as the list.
//
//     // deleting an item that does not exist does nothing
//     HumpList_addNode(0, 0, test);
//     HumpList_addNode(0, 1, test);
//     HumpList_addNode(0, 2, test);
//     HumpList_addNode(0, 3, test);
//     HumpList_display(test);
//
//     // frees memory related to list
//     HumpList_destroy(test);
// }
//
// barcode test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     BarcodeList *test = BarcodeList_makeList();
//     printf("barcode linkedlist test\n");
//
//     // for add()/delete(), function needs the x, y coordinate as well as the list.
//
//     // deleting an item that does not exist does nothing
//     BarcodeList_addNode(0, 0, 'A', test);
//     BarcodeList_addNode(0, 1, 'B', test);
//     BarcodeList_addNode(0, 2, 'C', test);
//     BarcodeList_addNode(0, 3, 'D', test);
//     BarcodeList_display(test);
//
//     // frees memory related to list
//     BarcodeList_destroy(test);
// }

// linked list test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     graph = List_makeList();
//
//     // for add()/delete(), function needs the x, y coordinate as well as the list.
//
//     // deleting an item that does not exist does nothing
//     List_delete(0, 0, graph);
//     List_addVertex(0, 0, graph);
//     List_addVertex(0, 1, graph);
//     List_addVertex(0, 2, graph);
//     List_addVertex(0, 3, graph);
//     List_delete(0, 1, graph);
//     List_delete(20, 20, graph);
//     List_display(graph);
//
//     // frees memory related to list
//     List_destroy(graph);
// }

// Stack test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     Stack *s = Stack_makeStack();

//     // deleting an item that does not exist does nothing
//     Stack_pop(s);

//     Vertex *v1 = Vertex_createVertex(0, 0);
//     Vertex *v2 = Vertex_createVertex(0, 1);
//     Vertex *v3 = Vertex_createVertex(0, 2);

//     Stack_push(v1, s);
//     Stack_push(v2, s);
//     Stack_push(v3, s);
//     Vertex *e = Stack_pop(s);

//     Stack_peak(s);
//     Stack_display(s);

//     Vertex_freeVertex(e);
//     // frees memory related to list
//     Stack_destroy2(s);
// }

// queue test
// int main(void)
// {
//     // allocates memory for an empty listS
//     // using a linked list to store our graph
//     Queue *q = Queue_makeQueue();

//     Vertex *v1 = Vertex_createVertex(0, 0);
//     Vertex *v2 = Vertex_createVertex(0, 1);
//     Vertex *v3 = Vertex_createVertex(0, 2);

//     // deleting an item that does not exist does nothing
//     Queue_dequeue(q);
//     Queue_enqueue(v1, q);
//     Queue_enqueue(v2, q);
//     Queue_enqueue(v3, q);
//     Vertex *temp = Queue_dequeue(q);
//     Vertex_freeVertex(temp);

//     Queue_display(q);
//     // =========================

//     // frees memory related to list
//     Queue_destroy2(q);
// }
