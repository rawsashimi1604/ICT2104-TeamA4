// // Yo Kevin/Irfaan, you can uncomment out the below when doing the testing

// /* DriverLib Includes */
// #include "driverlib.h"

// // for MAP_ prefix functions
// #include "rom_map.h"

// /* Standard Includes */
// #include <stdio.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>

// // Ultrasonic module

// // Motor module

// // mapping data structures
// #include "MappingModule/queue.h"
// #include "MappingModule/stack.h"
// #include "MappingModule/graph.h"
// #include "MappingModule/barcode_linkedlist.h"
// #include "MappingModule/hump_linkedlist.h"

// /* test stubs from other modules */
// #include "MappingModule/init.h"
// #include "MappingModule/ultrasonic.h"

// /* ====================================================================== */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Ultrasonic module

// Motor module

// mapping data structures
#include "queue.h"
#include "stack.h"
#include "graph.h"
#include "barcode_linkedlist.h"
#include "hump_linkedlist.h"

/* test stubs from other modules */
#include "init.h"
#include "ultrasonic.h"

void Motor_driveForward(int units)
{
}

void Motor_turnLeft(void)
{
}
void Motor_turnRight(void)
{
}

/* =================================== */

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
char buffer[100] = {0};

// char carDirection = 'N', 'E', 'S', 'W'
// dfs use bfs to backtrack to the vertex
// bfs will drive the car if there is a possible path to the destination and return true
// else it will return false
// assume bfs will not mark vertices as visited on the global graph data structure

char carDirection = 'N';
Vertex *carCurrentPosition; // points to the vertex where the car is currently at

// Kaho
// Might not expose this to other modules
// Vertex *Mapping_getCarCurrentPosition(void)
// {
//     return carCurrentPosition;
// }

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

// owner            : Irfaan
// description      : drives the robot car until it reaches a corner
// start condition  : anywhere on the maze
// end condition    : one of the following conditions must hold:
//                      Ultrasonic_checkFront() and Ultrasonic_checkLeft() must return false
//                      Ultrasonic_checkFront() and Ultrasonic_checkRight() must return false
//                      Ultrasonic_checkBack() and Ultrasonic_checkLeft() must return false
//                      Ultrasonic_checkBack() and UUltrasonic_checkRight() must return false
void initializeCarPosition(void)
{
    // - car need to check ultra sensors. it will need to have the condition where the car is in a corner
    // - at least 2 of the ultra sensor needs to be blocked. the 2 sensor that are blocked must NOT be opp of one another.
    // - if above not true, car turn till at least one of the side ultra sensor is blocked. then move forward till front is blocked.
    // - once done, turn arround so that the front is clear. set car as facing north and starting current position as 00,
    // - kaho’s mapMaze can commence

    bool objFront = Ultrasonic_checkFront();
    bool objBehind = Ultrasonic_checkBack();
    bool objLeft = Ultrasonic_checkLeft();
    bool objRight = Ultrasonic_checkRight();

    if ((objFront == false && objLeft == false && objBehind == true && objRight == true) ||
        (objFront == false && objLeft == true && objBehind == true && objRight == false))
    {
        printf("Car is Ready!");
    }
    else if (objFront == true && objLeft == true && objBehind == false && objRight == false)
    {
        Motor_turnRight();
        printf("Car turned right");
    }
    else if (objFront == true && objLeft == false && objBehind == false && objRight == true)
    {
        Motor_turnLeft();
        printf("Car turned left");
    }
    else if (objFront == true && objLeft == false && objBehind == true)
    {
        Motor_turnLeft();
        printf("Car turned left");
        while (objFront == false)
        {
            Motor_driveForward(1);
            printf("Car moving forward");
        }
    }
    else if (objFront == false && objLeft == false && objBehind == true && objRight == false)
    {
        Motor_turnLeft();
        printf("Car turned left");
        while (objFront == false)
        {
            Motor_driveForward(1);
            printf("Car moving forward");
        }
    }
}

// owner            : Kevin
// description      : takes in the car's current position and
//                    updates the ajd list of the vertex based on ultra sensors and car orientation.
//
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoLeft, bool canGoRight, Graph *graph)
{
    int x = currentPos->x;
    int y = currentPos->y;

    // if the car is facing north
    if (carDirection == 'N')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
    }

    // if the car is facing south
    else if (carDirection == 'S')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
    }

    // if the car is facing east
    else if (carDirection == 'E')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x + 1, y, graph));
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
    }

    // if the car is facing west
    else if (carDirection == 'W')
    {
        // create a new vertex and update the adj list for both vertices (addEdge)
        if (canGoFront)
            Graph_addEdge(currentPos, Graph_addVertex(x - 1, y, graph));
        if (canGoLeft)
            Graph_addEdge(currentPos, Graph_addVertex(x, y - 1, graph));
        if (canGoRight)
            Graph_addEdge(currentPos, Graph_addVertex(x, y + 1, graph));
    }
    else
    {
        printf("ERROR: updateMap failed - Unknown car direction.\n");
        return;
    }
    printf("updateMap completed sucessfully.\n");
}

// owner            : Kaho
// description      : drives the car based on the map using dfs
void mapMaze(Vertex *start, Graph *graph)
{
    // Map initialization for starting conditions
    bool canGoFront = Ultrasonic_checkFront();
    bool canGoBack = Ultrasonic_checkBack();
    bool canGoLeft = Ultrasonic_checkLeft();
    bool canGoRight = Ultrasonic_checkRight();
    carCurrentPosition = Graph_addVertex(0, 0, graph);

    // comment out for now for testing purposes
    // because updateMap() is impleted and will create vertices with edges
    // this function will create the neccsary vertices adjact to current position
    // updateMap(carCurrentPosition, canGoFront, canGoLeft, canGoRight, graph);

    // modified dfs starting condition to suit maze
    Stack *s = Stack_makeStack();
    start->visited = true;
    graph->numberOfNodesVisited++;

    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        if (start->adjacencyList[i] == NULL)
            break;
        Stack_push(start->adjacencyList[i], s);
    }

    // might want to change to pointer on heap instead, see how @Kevin
    // just to match the new function signature
    // if want to change then remember to free it
    int pathCoordinate[2][2] = {{0, 0}, {0, 0}};

    // dfs
    while (s->size != 0)
    {
        Vertex *v = Stack_pop(s);
        if (v->visited == true)
            continue;

        pathCoordinate[0][0] = carCurrentPosition->x;
        pathCoordinate[0][1] = carCurrentPosition->y;
        pathCoordinate[1][0] = v->x;
        pathCoordinate[1][1] = v->y;

        // driveCarUsingPath will drive me to the adjacent vertex I want to visit
        // returns true if that is possible
        // returns false if that is not possible
        // BFS will drive me there if I am not adjacent to the vertex
        if (!driveCarUsingPath(pathCoordinate, 2))
            bfs(carCurrentPosition, v, graph);

        v->visited = true;
        graph->numberOfNodesVisited++;
        carCurrentPosition = v;

        Vertex_writeStrToBuff(buffer, v);
        printf("visited: %s\n", buffer);

        // check surroundings and update the map accordingly
        canGoFront = Ultrasonic_checkFront();
        canGoBack = Ultrasonic_checkBack();
        canGoLeft = Ultrasonic_checkLeft();
        canGoRight = Ultrasonic_checkRight();

        // comment out for now for testing purposes
        // because updateMap() is impleted and will create vertices with edges
        // updateMap(carCurrentPosition, canGoFront, canGoLeft, canGoRight, graph);

        for (i = 0; i < 4; i++)
        {
            if (v->adjacencyList[i] == NULL)
                break;
            if (!v->adjacencyList[i]->visited)
                Stack_push(v->adjacencyList[i], s);
        }
    }

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
        }
        else if (directionToTurnTo == 'E')
            Motor_turnRight();
        else if (directionToTurnTo == 'W')
            Motor_turnLeft();
    }
    else if (carDirection == 'E')
    {
        if (directionToTurnTo == 'N')
            Motor_turnLeft();
        else if (directionToTurnTo == 'S')
            Motor_turnRight();
        else if (directionToTurnTo == 'W')
        {
            Motor_turnLeft();
            Motor_turnLeft();
        }
    }
    else if (carDirection == 'S')
    {
        if (directionToTurnTo == 'N')
        {
            Motor_turnLeft();
            Motor_turnLeft();
        }
        else if (directionToTurnTo == 'E')
            Motor_turnLeft();
        else if (directionToTurnTo == 'W')
            Motor_turnRight();
    }
    else if (carDirection == 'W')
    {
        if (directionToTurnTo == 'N')
            Motor_turnRight();
        else if (directionToTurnTo == 'S')
            Motor_turnLeft();
        else if (directionToTurnTo == 'E')
        {
            Motor_turnLeft();
            Motor_turnLeft();
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

    // char directionsArrs[numberOfDirections];
    // memset(directionsArrs, 0, numberOfDirections);

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
        if (listOfCoords[(i + 1) * 2 + 0] - listOfCoords[i * 2 + 0])
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
                printf("ERROR: driveCarUsingPath have a coords that skip a step. \n");
                free(directionsArrs);
                return false;
            }
        }
        // minus the y cord of 2 coords. if not 0 (false), car moves in y axis
        else if (listOfCoords[(i + 1) * 2 + 1] - listOfCoords[i * 2 + 1])
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
                printf("ERROR: driveCarUsingPath have a coords that skip a step. \n");
                free(directionsArrs);
                return false;
            }
        }
        else
        {
            // coord somehow wants to move diaganally
            printf("ERROR: driveCarUsingPath wants to move diaganally. \n");
            free(directionsArrs);
            return false;
        }
    }

    // For debugging purposes, prints the directions the car will need to move.
    //  printf("numberOfDirections = %d\n", numberOfDirections);
    printf("driveCarUsingPath will move car in: ");
    for (i = 0; i < numberOfDirections; i++)
    {
        printf("%c ", directionsArrs[i]);
    }
    printf("\n");

    // given the list of directions, move the car and adjust the orientation accordingly
    for (i = 0; i < numberOfDirections; i++)
    {
        // car is facing the right direction. move up
        if (carDirection == directionsArrs[i])
        {
            Motor_driveForward(1);
        }
        else
        {
            adjustCarDirection(directionsArrs[i]);
            Motor_driveForward(1);
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
        printf("BFS queue allocation failed. \n");
        return false;
    }
    Stack *stack1 = Stack_makeStack(); // used to recontruct path later on.
    if (stack1 == NULL)
    {
        printf("BFS stack allocation failed. \n");
        return false;
    }

    // push the source node onto the queue, then mark as visited by adding to the list
    Queue_enqueue(sourceV, queue1);
    tempVisitedNode1 = (node)malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
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
                if ((tempVisitedNode1->next == NULL) && (tempVisitedNode1->vertexPointer != tempV))
                {
                    printf("ERROR: tempV cannot be found in VistedNode linked list.\n");
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
            // reconstrcting shortest path
            // moving back up to source using parent pointer
            while (tempVisitedNode1 != NULL)
            {
                // pushing vertices onto stack on by one
                if (!Stack_push(tempVisitedNode1->vertexPointer, stack1))
                {
                    printf("ERROR: BFS Stack allocation failed.");

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
                printf("Size of stack: %d | ", stack1->size); // for debuggin purposes
                Stack_peak(stack1);                           // for debuggin purposes
                tempV = Stack_pop(stack1);                    // pop the stack
                listOfCoords[i * 2 + 0] = tempV->x;           // add the x coord of the vertex from the stack onto the listOfCoords
                listOfCoords[i * 2 + 1] = tempV->y;           // add the y coord of the vertex from the stack onto the listOfCoords
            }
            // passing the pointer of the listOfCoords to the drive function to drive
            // printf("Number of Coords being passed in: %d \n", (sizeof(listOfCoords)/sizeof(listOfCoords[0])));
            if (!driveCarUsingPath(listOfCoords, stackSize))
            {
                printf("ERROR: BFS listOfCoords has wrong coords.\n");
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
            printf("BFS completed sucessfully.\n");
            free(listOfCoords);
            return true; // success in BFS n drive
        }

        // targetV has not been found. iterate throu adjList of node and enqueue if visited by car and not by BFS
        for (i = 0; i < 4; i++)
        {
            if (tempV->adjacencyList[i] == NULL) // breaks if no more ajd vertices
                break;

            // queue if the vertex is visited by car and not visited by BFS yet
            // check if the adj vertex has been visited by the car
            if (tempV->adjacencyList[i]->visited)
            {
                tempVisitedNode1 = visitedListHead; // set a pointer to the head of visited node list
                // interate thru visited list
                while (tempVisitedNode1 != NULL)
                {
                    if (tempV->adjacencyList[i] == tempVisitedNode1->vertexPointer) // check if the adj vertex has been visted by BFS
                        break;                                                      // adjacent vertex has already been visited has already been visited. skip it

                    if (tempVisitedNode1->next == NULL)
                    {                                                   // check if it is the last item on the Visitedlist
                        Queue_enqueue(tempV->adjacencyList[i], queue1); // enqueue as has not been visited by BFS
                        // set as visited by adding to visited list (repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = NULL;                                     //(repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = (node)malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
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
    printf("ERROR: Shortest Path could not be constructed.\n");
    return false;
}

// TODO: nice to have graphical map?
// owner            : Kaho
// description      : sends the graphical representation of the graph in adj representation
//                       for the Communications module to print the map
void displayMapOnM5(Graph *graph)
{
    Node *trav = graph->list->head;
    char temp[10] = {0};

    uint8_t i = 0;
    snprintf(buffer, 100, "Car is currently at (%d, %d)", carCurrentPosition->x, carCurrentPosition->y);
    printf("%s\n", buffer);
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
        // strcat(buffer, "\n");
        // Communications_send(buffer)
        printf("%s\n", buffer);
        trav = trav->next;
    }
}

// graph test
int main(void)
{

    // Yo Kev/Irfaan, you can uncomment these out for faster printing :D
    /* Setting our MCLK to 48MHz for faster programming */
    // MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    // FlashCtl_setWaitState(FLASH_BANK0, 2);
    // FlashCtl_setWaitState(FLASH_BANK1, 2);
    // MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

    printf("graph test\n");
    // allocates memory for an empty graph
    // using a linked list to store our graph
    Graph *graph = Graph_makeGraph();

    // to add verices to a graph with no edges

    // row 1
    Vertex *v1 = Graph_addVertex(-2, 3, graph);
    Vertex *v2 = Graph_addVertex(-1, 3, graph);
    Vertex *v3 = Graph_addVertex(0, 3, graph);
    Vertex *v4 = Graph_addVertex(1, 3, graph);

    // row2
    Vertex *v5 = Graph_addVertex(-2, 2, graph);
    Vertex *v6 = Graph_addVertex(-1, 2, graph);
    Vertex *v7 = Graph_addVertex(0, 2, graph);
    Vertex *v8 = Graph_addVertex(1, 2, graph);

    // row 3
    Vertex *v9 = Graph_addVertex(-2, 1, graph);
    Vertex *v10 = Graph_addVertex(-1, 1, graph);
    Vertex *v11 = Graph_addVertex(0, 1, graph);
    Vertex *v12 = Graph_addVertex(1, 1, graph);

    // row 4
    Vertex *v13 = Graph_addVertex(-2, 0, graph);
    Vertex *v14 = Graph_addVertex(-1, 0, graph);
    carCurrentPosition = Graph_addVertex(0, 0, graph);
    Vertex *v16 = Graph_addVertex(1, 0, graph);

    // row 5
    Vertex *v17 = Graph_addVertex(-2, -1, graph);
    Vertex *v18 = Graph_addVertex(-1, -1, graph);
    Vertex *v19 = Graph_addVertex(0, -1, graph);
    Vertex *v20 = Graph_addVertex(1, -1, graph);

    // note: adding an existing vertex will not do anything
    // although the validation check here is lowkey expensive
    Graph_addVertex(0, 0, graph);

    // to add edge between 2 existing vertices in a graph
    // row 1
    Graph_addEdge(v1, v2);
    Graph_addEdge(v2, v3);
    Graph_addEdge(v3, v4);
    Graph_addEdge(v1, v5);
    Graph_addEdge(v3, v7);
    Graph_addEdge(v4, v8);

    // row2
    Graph_addEdge(v9, v5);
    Graph_addEdge(v6, v10);
    Graph_addEdge(v7, v11);
    Graph_addEdge(v8, v12);

    // row 3
    Graph_addEdge(v9, v13);
    Graph_addEdge(v9, v10);
    Graph_addEdge(v11, carCurrentPosition);
    Graph_addEdge(v16, v12);

    // row 4
    Graph_addEdge(v13, v14);
    Graph_addEdge(v14, carCurrentPosition);
    Graph_addEdge(v16, v20);
    Graph_addEdge(v17, v13);

    // row 5
    Graph_addEdge(v17, v18);
    Graph_addEdge(v18, v19);
    Graph_addEdge(v20, v19);

    // // note: adding an existing edge between two vertices will not do anything
    // // although the validation check here is lowkey expensive
    Graph_addEdge(v20, v19);

    // how to iterate through the adjacent vertices of a graph
    // Graph_adj(x, y, graph) takes in 3 args: the x and y coordinate of the graph, and the graph itself
    // returns an adj array of 4 pointers to vertices. If not initialized, then it is (-100, -100) for a vertex
    // Vertex **adj = Graph_adj(0, 0, graph);
    // uint8_t i = 0;
    // for (i = 0; i < 4; i++)
    // {
    //     printf("(%d, %d)\n", adj[i]->x, adj[i]->y);
    // }

    // alternatively, you may access the vertex adj list directly
    // such as vertex->adj, but make sure not to go beyond size 4

    // Vertex_writeStrToBuff(buffer, adj[0]);
    // printf("%s\n", buffer);
    mapMaze(carCurrentPosition, graph);

    if (bfs(carCurrentPosition, Graph_addVertex(1, -1, graph), graph))
        printf("bfs ok\n");
    else
        printf("bfs failed\n");
    printf("======\n");

    Graph_display(graph);
    printf("Expected: %d == %d\n", graph->numberOfNodesVisited, graph->size);
    printf("Explored: %s\n", Graph_isExplored(graph) ? "true" : "false");

    printf("======\n");
    displayMapOnM5(graph);

    // frees memory related to graphz
    Graph_destroy(graph);

    // Yo Kev/Irfaan, you can uncomment these out if you testing it on the MSP432
    // printf("Success!\n");
    // printf("Going to LPM3\n");
    // while (1)
    // {
    //     PCM_gotoLPM3();
    // }
}

// hump test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     HumpList *test = HumpList_makeList();
//     printf("hump linkedlist test\n");

//     // for add()/delete(), function needs the x, y coordinate as well as the list.

//     // deleting an item that does not exist does nothing
//     HumpList_addNode(0, 0, test);
//     HumpList_addNode(0, 1, test);
//     HumpList_addNode(0, 2, test);
//     HumpList_addNode(0, 3, test);
//     HumpList_display(test);

//     // frees memory related to list
//     HumpList_destroy(test);
// }

// barcode test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     BarcodeList *test = BarcodeList_makeList();
//     printf("barcode linkedlist test\n");

//     // for add()/delete(), function needs the x, y coordinate as well as the list.

//     // deleting an item that does not exist does nothing
//     BarcodeList_addNode(0, 0, 'A', test);
//     BarcodeList_addNode(0, 1, 'B', test);
//     BarcodeList_addNode(0, 2, 'C', test);
//     BarcodeList_addNode(0, 3, 'D', test);
//     BarcodeList_display(test);

//     // frees memory related to list
//     BarcodeList_destroy(test);
// }

// linked list test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     graph = List_makeList();

//     // for add()/delete(), function needs the x, y coordinate as well as the list.

//     // deleting an item that does not exist does nothing
//     List_delete(0, 0, graph);
//     List_addVertex(0, 0, graph);
//     List_addVertex(0, 1, graph);
//     List_addVertex(0, 2, graph);
//     List_addVertex(0, 3, graph);
//     List_delete(0, 1, graph);
//     List_delete(20, 20, graph);
//     List_display(graph);

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
