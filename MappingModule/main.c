#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Ultrasonic module
#include "init.h"
#include "ultrasonic.h"

// Motor module

// mapping data structures
#include "queue.h"
#include "stack.h"
#include "graph.h"
#include "barcode_linkedlist.h"
#include "hump_linkedlist.h"

#include "cvector.h"
#include "cvector_utils.h"

/* =================================== */

// Mapping module interface
void Mapping_writeBarcodeData(char data);
void Mapping_writeHumpData(void);

/* =================================== */

// add our function declarations here in the future
void updateCarDirection(char turn);
void initializeCarPosition(void);
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoBehind, bool canGoLeft, bool canGoRight);
void mapMaze(Vertex *start, Graph *graph);
bool isCorrectOrientation(Vertex *aboutToVistVertex, Vertex *carOrientation);
void adjustOrientation(Vertex *currentCarPos, char carOrientation);
bool isVertexAdjacentToCurrent(Vertex *currentCarPos, Vertex *aboutToVisitVertex);
void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Vertex *target, Graph *g);
int **reconstructPath(int startX, int startY, int endX, int endY, int edgeTo[][2]);
void driveCarUsingPath(Vertex *targetVertex);
bool bfs(Vertex *sourceV, Vertex *endV, Graph *graph);
bool isMapExplored(Graph *graph);

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

// might not need this anymore
Vertex *carCurrentOrientation; // points to the vertex that the car is facing, if facing a wall, then it is NULL

//  N
// W E
//  S
// expects a argument of either 'L', or 'R'
void updateCarDirection(char turn)
{
    if (carDirection == 'N' && turn == 'L')
        carDirection = 'W';
    else if (carDirection == 'N' && turn == 'R')
        carDirection = 'E';
    else if (carDirection == 'E' && turn == 'L')
        carDirection = 'N';
    else if (carDirection == 'E' && turn == 'R')
        carDirection = 'S';
    else if (carDirection == 'S' && turn == 'L')
        carDirection = 'E';
    else if (carDirection == 'S' && turn == 'R')
        carDirection = 'W';
    else if (carDirection == 'W' && turn == 'L')
        carDirection = 'S';
    else if (carDirection == 'W' && turn == 'R')
        carDirection = 'N';
}

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
//                      Ultrasonic_checkBehind() and Ultrasonic_checkLeft() must return false
//                      Ultrasonic_checkBehind() and UUltrasonic_checkRight() must return false
void initializeCarPosition(void)
{
}

// owner            : Kevin
// description      : updates the map based on the car's current position
//                      and adds it to the map
//                      and updates the current vertex's adjacency list
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoBehind, bool canGoLeft, bool canGoRight){
    //this function shold take in the cars current position as a vertex, (already created)
    // int x = 0;
    // int y = 0;
    // Node *temp = 0;
    // if (canGoFront)
    // {
    //     Vertex *temp = List_addVertex(x, y, graph);
    // }
}

// will delete this, just keeping it here for notes at the moment
// owner            : Kaho
// description      : checks the car's front, left, back, right using Ultrasonic sensors and initializes the map
// start condition  : correctCar() is called and car is in a corner
// end condition    : graph initialized with initial conditions
// example          : c is the car, | is a wall, - is an empty grid
// map = []
// -|-|-
// -|- -
// c -|-
// mapInit()
// map = [(0, 0, true), (1, 0, false), (0, 1, false)]
// where each vertex is represent as a struct (int x_coordinate, int y_coordinate, bool isVisited)
void mapInit(void)
{
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

    // this function will create the neccsary vertices adjact to current position
    updateMap(carCurrentPosition, canGoFront, canGoBack, canGoLeft, canGoRight);

    Stack *s = Stack_makeStack();
    start->visited = true;
    graph->numberOfNodesVisited++;

    for (size_t i = 0; i < 4; i++)
    {
        if (start->adjacencyList[i] == NULL)
            break;
        Stack_push(start->adjacencyList[i], s);
    }

    while (s->size != 0)
    {
        Vertex *v = Stack_pop(s);
        if (v->visited == true)
            continue;

        // this will ensure that I am within 1 unit of the vertex that I am about to visit
        // bfs will drive me there
        if (!isVertexAdjacentToCurrent(carCurrentPosition, v))
            bfs(carCurrentPosition, v, graph);

        // this will ensure that I am pointing to the vertex that I am about to visit
        if (!isCorrectOrientation(v, carCurrentOrientation))
            adjustOrientation(carCurrentPosition, carDirection);

        if (carCurrentPosition != v)
        {
            // Motor_driveForward(1);
        }

        v->visited = true;
        graph->numberOfNodesVisited++;
        carCurrentPosition = v;

        Vertex_writeStrToBuff(buffer, v);
        printf("visited: %s\n", buffer);

        canGoFront = Ultrasonic_checkFront();
        canGoBack = Ultrasonic_checkBack();
        canGoLeft = Ultrasonic_checkLeft();
        canGoRight = Ultrasonic_checkRight();

        updateMap(carCurrentPosition, canGoFront, canGoBack, canGoLeft, canGoRight);

        for (size_t i = 0; i < 4; i++)
        {
            if (v->adjacencyList[i] == NULL)
                break;
            if (!v->adjacencyList[i]->visited)
                Stack_push(v->adjacencyList[i], s);
        }
    }

    Stack_destroy(s);
}

// owner            : Kevin
// description      : check if car is facing the vertex that its about to visit
//                      return True if Yes else False
bool isCorrectOrientation(Vertex *aboutToVistVertex, Vertex *carOrientation)
{
    //is this still needed? - kev
    return true;
}

// owner            : Kevin
// description      : while car is not correctOrientation
//                      turn car at 90 degrees to correct the orientation
void adjustOrientation(Vertex *currentCarPos, char carOrientation)
{
    //is this still needed? - kev
}

// owner            : Irfaan
// description      : checks if car can visit the vertex v that its is about to visit
//                      from its current position by driving straight
// isVertexAd
// bool canVisitVertexFromCurrent(Vertex *currentCarPos, Vertex *aboutToVisitVertex)
bool isVertexAdjacentToCurrent(Vertex *currentCarPos, Vertex *aboutToVisitVertex)
{
    return true;
}

// Consult Kevin, probably don't need anymore
// owner            : Kevin
// description      : retrace back to the vertex that is adjacent to the vertex that I am about to visit
//                      this function drives the car
// void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Node *path)
// {
// }
void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Vertex *target, Graph *g)
{
}

// KIV, MIGHT NOT NEED TO DO ANYMORE
// Consult Kevin, probably don't need anymore
// owner            : Kaho
// description      : returns a path to the end coordinate if available, else null
// see https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-from-a-function-in-c
int **reconstructPath(int startX, int startY, int endX, int endY, int edgeTo[][2])
{
    return 0;
}

// owner            : Irfaan
// description      : drive car following the path
// input            : it is a list of coordinates to the end goal
void driveCarUsingPath(Vertex *targetVertex)
{
}


// owner            : Kevin
// description      : takes in the sources vertex, and target vertex. If the car can move there,
//                    drives the car there and returns true. Otherwise, returns false.
bool bfs(Vertex *sourceV, Vertex *targetV, Graph *graph){

    //Declare required structures and var
    //Custom linkedlist that tracks vertices that are visited by BFS, and their parent vertex.
    struct VisitedNode{
        Vertex* vertexPointer; //holds the pointer to the vertex
        struct VisitedNode* parentVertex;  //holds the pointer to the parent of the vertex
        struct VisitedNode* next; //the normal next pointer that points to the next item on the list. 
    };
    typedef struct VisitedNode* node;
    node visitedListHead = NULL; //pointer used as the head of the list
    node tempVisitedNode1 = NULL; //pointer used for various operations
    node tempVisitedNode2 = NULL; //pointer used for various operations

    Vertex *tempV = NULL; //use to temp hold Vertex pointers 
    Queue *queue1 = Queue_makeQueue(); //main queue used for BFS
    if (queue1 == NULL){
        printf("BFS queue allocation failed. \n");
        return false;
    }
    Stack * stack1 = Stack_makeStack(); //used to recontruct path later on.
    if (stack1 == NULL){
        printf("BFS stack allocation failed. \n");
        return false;
    }


    //push the source node onto the queue, then mark as visited by adding to the list
    Queue_enqueue(sourceV, queue1);

    tempVisitedNode1 = (node)malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
    if (tempVisitedNode1 = NULL){ //check that the malloc succeeded

        //garbage collection
        while(visitedListHead != NULL){
            tempVisitedNode1 = visitedListHead;
            visitedListHead = visitedListHead -> next;
            free(tempVisitedNode1);
        }
        Queue_destroy(queue1);
        Stack_destroy(stack1);

        printf("BFS tempVisitedNode1 allocation failed. \n");
        return false;
    }
    tempVisitedNode1-> vertexPointer = sourceV; //set the pointer (sourceV) that this visitednode represents 
    tempVisitedNode1-> parentVertex = NULL; // sourceV does not have a parent, as is the root of the BFS graph
    tempVisitedNode1->next = NULL;
    visitedListHead = tempVisitedNode1; //setting the head of the list to point to the node just created.
    tempVisitedNode1 = NULL;

    //Loop till queue is empty
    while(queue1->size != 0){//check is the queue is empty (size = 0)
        //dequeue and check if the the pointer is the targetV they are looking for
        tempV = Queue_dequeue(queue1); 
        if (tempV == targetV){
            //targetV has been found. push v pointers to the stack based on vistedList parent    

            tempVisitedNode1 = visitedListHead; //setting a pointer to head
            //look for tempV in the visitedList, set as tempVisitedNode1
            while(tempVisitedNode1 -> vertexPointer != tempV){
                tempVisitedNode1 = tempVisitedNode1 -> next; //move down the list

                if( (tempVisitedNode1 -> next == NULL) && (tempVisitedNode1 -> vertexPointer != tempV) ){ //checking for error case
                    printf("ERROR: tempV cannot be found in VistedNode linked list.\n");
                    //garbage collection
                    while(visitedListHead != NULL){
                        tempVisitedNode1 = visitedListHead;
                        visitedListHead = visitedListHead -> next;
                        free(tempVisitedNode1);
                    }
                    Queue_destroy(queue1);
                    Stack_destroy(stack1);
                    return false;
                }
            }
            //reconstrcting shortest path
            while(tempVisitedNode1 != NULL){ //moving back up to source using parent pointer
                if(!Stack_push(tempVisitedNode1 -> vertexPointer, stack1)){ //pushing vertices onto stack on by one 
                    printf("ERROR: BFS Stack allocation failed.");

                    //garbage collection
                    while(visitedListHead != NULL){
                        tempVisitedNode1 = visitedListHead;
                        visitedListHead = visitedListHead -> next;
                        free(tempVisitedNode1);
                    }
                    Queue_destroy(queue1);
                    Stack_destroy(stack1);
                    return false; 
                }
                tempVisitedNode1 = tempVisitedNode1 -> parentVertex;
            }

            //moving the car based on path constructed

            while( stack1 -> size != 0){
                driveCarUsingPath(Stack_pop(stack1)); //calling function to drive car. 
            }

            //garbage collection
            while(visitedListHead != NULL){
                tempVisitedNode1 = visitedListHead;
                visitedListHead = visitedListHead -> next;
                free(tempVisitedNode1);
            }
            Queue_destroy(queue1);
            Stack_destroy(stack1);
            return true; //success in BFS n drive
        }

        //targetV has not been found. iterate throu adjList of node and enqueue if visited by car and not by BFS
        for (size_t i = 0; i < 4; i++){ 
            if (tempV -> adjacencyList[i] == NULL) //breaks if no more ajd vertices
                break; 

            //queue if the vertex is visited by car and not visited by BFS yet
            //check if the adj vertex has been visited by the car
            if(tempV -> adjacencyList[i] -> visited){ 
                tempVisitedNode1 = visitedListHead; //set a pointer to the head of visited node list
                //interate thru visited list
                while (tempVisitedNode1 != NULL){
                    if (tempV -> adjacencyList[i] == tempVisitedNode1 -> vertexPointer) //check if the adj vertex has been visted by BFS                        
                        break; //adjacent vertex has already been visited has already been visited. skip it
                    
                    if (tempVisitedNode1 -> next == NULL){ //check if it is the last item on the Visitedlist
                        Queue_enqueue(tempV -> adjacencyList[i], queue1); //enqueue as has not been visited by BFS
                        //set as visited by adding to visited list (repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = NULL; //(repurpose tempVisitedNode1 as is no longer needed)
                        tempVisitedNode1 = (node)malloc(sizeof(struct VisitedNode)); // allocate memory using malloc()
                        if (tempVisitedNode1 == NULL){
                            printf("BFS tempVisitedNode1 alloction failed. \n");
                            //garbage collection
                            while(visitedListHead != NULL){
                                tempVisitedNode1 = visitedListHead;
                                visitedListHead = visitedListHead -> next;
                                free(tempVisitedNode1);
                            }
                            Queue_destroy(queue1);
                            Stack_destroy(stack1);
                        }
                        tempVisitedNode1-> vertexPointer = tempV -> adjacencyList[i]; //set the pointer (sourceV) that this node represents 
                        // set parent visitNode, which is the ptr of VisitedNode that contains tempV
                        tempVisitedNode2 = visitedListHead; //setting the head of the list
                        while(tempVisitedNode2 -> vertexPointer != tempV){ //move pointer till VisitedNode
                            tempVisitedNode2 = tempVisitedNode2 -> next;
                        }
                        tempVisitedNode1-> parentVertex = tempVisitedNode2; 
                        tempVisitedNode1->next = NULL;
                        
                        tempVisitedNode2 = visitedListHead; //setting the head of the list
                        //moving tempVisitedNode2 down to last item on the list
                        while(tempVisitedNode2 -> next != NULL){ 
                            tempVisitedNode2 = tempVisitedNode2 -> next; //moving down list
                        }
                        tempVisitedNode2 -> next = tempVisitedNode1; //adding tempVisitedNode1 to back of the list

                        tempVisitedNode1 = NULL; //reset pointer
                        tempVisitedNode2 = NULL; //reset pointer
                        
                        break;
                    }
                    tempVisitedNode1 = tempVisitedNode1->next; //go to next visitednNode
                };
            }
        }
    }
    //garbage collection
    while(visitedListHead != NULL){
        tempVisitedNode1 = visitedListHead;
        visitedListHead = visitedListHead -> next;
        free(tempVisitedNode1);
    }
    Queue_destroy(queue1);
    Stack_destroy(stack1);
    printf("ERROR: BNF failed.\n");
    return false;
}//end of BFS function

// TODO
// owner            : Kaho
// description      : returns a string representation for the Communications module to print the map
// char *getMap(Graph *graph)
// {
//     return Graph_toString(graph);
// }

// owner            : Kaho
// description      : iterates through the graph and check if the maze is explored
bool isMapExplored(Graph *graph)
{
    return Graph_isExplored(graph);
}

// graph test
int main(void)
{
    printf("graph test\n");
    // allocates memory for an empty graph
    // using a linked list to store our graph
    Graph *graph = Graph_makeGraph();

    // to add verices to a graph with no edges
    carCurrentPosition = Graph_addVertex(0, 0, graph);

    Graph_addVertex(-2, 3, graph);
    Graph_addVertex(-1, 3, graph);
    Graph_addVertex(0, 3, graph);
    Graph_addVertex(1, 3, graph);

    Graph_addVertex(-2, 2, graph);
    Graph_addVertex(-1, 2, graph);
    Graph_addVertex(0, 2, graph);
    Graph_addVertex(1, 2, graph);

    Graph_addVertex(-2, 1, graph);
    Graph_addVertex(-1, 1, graph);
    Graph_addVertex(0, 1, graph);
    Graph_addVertex(1, 1, graph);

    Graph_addVertex(-2, 0, graph);
    Graph_addVertex(-1, 0, graph);
    Graph_addVertex(1, 0, graph);

    Graph_addVertex(-2, -1, graph);
    Graph_addVertex(-1, -1, graph);
    Graph_addVertex(0, -1, graph);
    Graph_addVertex(1, -1, graph);

    // note: adding an existing vertex will not do anything
    // although the validation check here is very expensive
    Graph_addVertex(0, 0, graph);

    // to add edge between 2 existing vertices in a graph
    Graph_addEdge(-2, 3, -1, 3, graph);
    Graph_addEdge(-2, 3, -2, 2, graph);
    Graph_addEdge(-1, 3, 0, 3, graph);
    Graph_addEdge(0, 3, 1, 3, graph);

    Graph_addEdge(1, 3, 1, 2, graph);
    Graph_addEdge(0, 3, 0, 2, graph);
    Graph_addEdge(-1, 2, -1, 1, graph);
    Graph_addEdge(-2, 2, -2, 1, graph);

    Graph_addEdge(-2, 1, -1, 1, graph);
    Graph_addEdge(0, 2, 0, 1, graph);
    Graph_addEdge(1, 2, 1, 1, graph);

    Graph_addEdge(-2, 0, -2, 1, graph);
    Graph_addEdge(-2, 0, -1, 0, graph);
    Graph_addEdge(0, 0, -1, 0, graph);
    Graph_addEdge(0, 0, 0, 1, graph);
    Graph_addEdge(1, 0, 1, 1, graph);
    Graph_addEdge(-2, -1, -2, 0, graph);
    Graph_addEdge(-2, -1, -1, -1, graph);
    Graph_addEdge(-1, -1, 0, -1, graph);
    Graph_addEdge(0, -1, 1, -1, graph);
    Graph_addEdge(1, -1, 1, 0, graph);

    // note: adding an existing edge between two vertices will not do anything
    // although the validation check here is very expensive
    Graph_addEdge(0, 0, 0, 1, graph);

    // how to iterate through the adjacent vertices of a graph
    // Graph_adj(x, y, graph) takes in 3 args: the x and y coordinate of the graph, and the graph itself
    // returns an adj array of 4 pointers to vertices. If not initialized, then it is (-100, -100) for a vertex
    // Vertex **adj = Graph_adj(0, 0, graph);
    // for (size_t i = 0; i < 4; i++)
    // {
    //     printf("(%d, %d)\n", adj[i]->x, adj[i]->y);
    // }

    // Vertex_writeStrToBuff(buffer, adj[0]);
    // printf("%s\n", buffer);

    mapMaze(carCurrentPosition, graph);

    printf("======\n");

    Graph_display(graph);
    printf("Expected: %d == %d\n", graph->numberOfNodesVisited, graph->size);
    printf("Explored: %s\n", Graph_isExplored(graph) ? "true" : "false");

    // frees memory related to graph
    Graph_destroy(graph);
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
