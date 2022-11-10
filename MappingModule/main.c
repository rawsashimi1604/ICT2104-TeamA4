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
Graph *Mapping_getMap(void);
void Mapping_writeBarcodeData(char data);
void Mapping_writeHumpData(void);

/* =================================== */

HumpList *humpData = NULL;
BarcodeList *barCodeData = NULL;
cvector_vector_type(bool) visited = NULL;
Graph *graph;
char buffer[100] = {0};

// char carDirection = 'N', 'E', 'S', 'W'
// dfs use bfs to backtrack to the vertex
// bfs will drive the car if there is a possible path to the destination and return true
// else it will return false
// assume bfs will not mark vertices as visited on the global graph data structure

char carDirection = 'N';
Vertex *carCurrentPosition;    // points to the vertex where the car is currently at
Vertex *carCurrentOrientation; // points to the vertex that the car is facing, if facing a wall, then it is NULL
unsigned char numberOfNodesVisited;
unsigned char sizeOfGraph;

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
Graph *Mapping_getMap(void)
{
    return graph;
}

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
void updateMap(Vertex *currentPos, bool canGoFront, bool canGoBehind, bool canGoLeft, bool canGoRight)
{

    // int x = 0;
    // int y = 0;
    // Node *temp = 0;
    // if (canGoFront)
    // {
    //     Vertex *temp = List_addVertex(x, y, graph);
    // }
}

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
    bool canGoFront = Ultrasonic_checkFront();
    bool canGoBack = Ultrasonic_checkBack();
    bool canGoLeft = Ultrasonic_checkLeft();
    bool canGoRight = Ultrasonic_checkRight();
    carCurrentPosition = Graph_addVertex(0, 0, graph);
    updateMap(carCurrentPosition, canGoFront, canGoBack, canGoLeft, canGoRight);
}

bool isVertexAdjacentToCurrent(Vertex *currentCarPos, Vertex *aboutToVisitVertex);
bool isCorrectOrientation(Vertex *v, Vertex *carCurrentOrientation);

// For Kevin:
// bfs needs to calculate if I can go there in the first place

// TODO
// owner            : Kaho
// description      : drives the car based on the map using dfs
void dfs(Vertex *start, Graph *graph)
{
    Stack *s = Stack_makeStack();
    start->visited = true;
    graph->numberOfNodesVisited++;

    Vertex **adj = Graph_adj(start->x, start->y, graph);
    for (size_t i = 0; i < 4; i++)
    {
        if (adj[i] == NULL)
            break;
        Stack_push(adj[i]->x, adj[i]->y, s);
    }

    while (s->size != 0)
    {
        Node *n = Stack_pop(s);
        Vertex *v = Graph_getVertex(n->data->x, n->data->y, graph);
        if (v->visited == true)
        {
            Node_freeNode(n);
            continue;
        }

        // this will ensure that I am within 1 unit of the vertex that I am about to visit
        // bfs will drive me there
        if (!isVertexAdjacentToCurrent(carCurrentPosition, v))
            retraceBackToVertexAdjacentToAboutToVisitVertex(carCurrentPosition, v, graph);

        // this will ensure that I am pointing to the vertex that I am about to visit
        if (!isCorrectOrientation(v, carCurrentOrientation))
            adjustOrientation(carCurrentPosition, carDirection);

        // Motor_driveForward(1);
        v->visited = true;
        graph->numberOfNodesVisited++;

        Vertex_writeStrToBuff(buffer, v);
        printf("visited: %s\n", buffer);

        bool canGoFront = Ultrasonic_checkFront();
        bool canGoBack = Ultrasonic_checkBack();
        bool canGoLeft = Ultrasonic_checkLeft();
        bool canGoRight = Ultrasonic_checkRight();

        updateMap(graph, canGoFront, canGoBack, canGoLeft, canGoRight);

        adj = Graph_adj(v->x, v->y, graph);
        for (size_t i = 0; i < 4; i++)
        {
            if (adj[i] == NULL)
                break;
            if (!adj[i]->visited)
                Stack_push(adj[i]->x, adj[i]->y, s);
        }
        Node_freeNode(n);
    }

    Stack_destroy(s);
}

// owner            : Kevin
// description      : check if car is facing the vertex that its about to visit
//                      return True if Yes else False
bool isCorrectOrientation(Vertex *aboutToVistVertex, Vertex *carOrientation)
{
    return true;
}

// owner            : Kevin
// description      : while car is not correctOrientation
//                      turn car at 90 degrees to correct the orientation
void adjustOrientation(Vertex *currentCarPos, Vertex *carOrientation)
{
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

// owner            : Kevin
// description      : retrace back to the vertex that is adjacent to the vertex that I am about to visit
//                      this function drives the car
// void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Node *path)
// {
// }

void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Vertex *target, Graph *g)
{
}

// TODO
// owner            : Kaho
// description      : returns a path to the end coordinate if available, else null
// see https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-from-a-function-in-c
int **reconstructPath(int startX, int startY, int endX, int endY, int edgeTo[][2])
{
}

// owner            : Irfaan
// description      : drive car following the path
// input            : it is a list of coordinates to the end goal
void driveCarUsingPath(int path[][2])
{
}

// owner            : Kevin
// description      : takes in the sources vertex, and destination vertex. If the car can move there,
//                    drives the car there and returns true. Otherwise, returns false.
bool bfs(Vertex *sourceV, Vertex *endV, Graph *graph)
{
    // //start from the sorucesv, move 1 level through the AJ looking for the endV. if found, end and drive.
    // //if not found, returns false.

    // //Declare required structures and var
    // Vertex
    // Queue *queue1 = Queue_makeQueue(); //main queue used for BFS
    // Vertex *vistedList[] = NULL; //List of visited vertex pointers. used to recontruct path
    // Stack * stack1 = Stack_makeStack(); //used to recontruct path later on.

    // //push the souce node onto the queue
    // Queue_enqueue(int sourceV->x, int sourceV->y, Queue *queue1);

    // //Loop till queue is empty
    // while()//check is the queue is empty (reutrn)

    // return false;
}

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

// hump test
// int main(void)
// {
//     // allocates memory for an empty list
//     // using a linked list to store our graph
//     HumpList *test = HumpList_makeList();
//     printf("hello world\n");

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
//     printf("hello world\n");

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

// graph test
int main(void)
{
    printf("hello world\n");
    // allocates memory for an empty graph
    // using a linked list to store our graph
    Graph *graph = Graph_makeGraph();

    // for addVertex() function needs the x, y coordinate as well as the graph.

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

    dfs(carCurrentPosition, graph);

    printf("======\n");

    Graph_display(graph);
    printf("Expected: %d == %d\n", graph->numberOfNodesVisited, graph->size);
    printf("Explored: %s\n", Graph_isExplored(graph) ? "true" : "false");

    // frees memory related to graph
    Graph_destroy(graph);
}

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

//     // for add()/delete(), function needs the x, y coordinate as well as the list.

//     // deleting an item that does not exist does nothing
//     Stack_pop(s);
//     Stack_push(0, 0, s);
//     Stack_push(0, 1, s);
//     Stack_push(0, 2, s);
//     Node *e = Stack_pop(s);
//     Node_freeNode(Stack_pop(s));
//     Node_freeNode(Stack_pop(s));
//     Stack_pop(s);
//     Node_freeNode(e);

//     Stack_peak(s);
//     Stack_display(s);

//     // frees memory related to list
//     Stack_destroy(s);
// }

// queue test
// int main(void)
// {
//     // allocates memory for an empty listS
//     // using a linked list to store our graph
//     Queue *q = Queue_makeQueue();

//     // for add()/delete(), function needs the x, y coordinate as well as the list.

//     // deleting an item that does not exist does nothing
//     Queue_dequeue(q);
//     Queue_enqueue(0, 0, q);
//     Queue_enqueue(0, 1, q);
//     Queue_enqueue(0, 2, q);
//     Node *temp = Queue_dequeue(q);
//     Node_freeNode(temp);

//     Queue_display(q);
//     // =========================

//     // frees memory related to list
//     Queue_destroy(q);
// }
