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

#include "cvector.h"
#include "cvector_utils.h"

/* =================================== */

// might or might not implement this
// might just have 2 more structs, one for each (hump / barCode)
// reason: lazy D;
// struct coordinate
// {
//     int x;
//     int y;
// };

// humps = [(0, 1), (2, 3), (-1, -1)]
// barCodeData = [(0, 1, 'A'), (1, 2, 'B')]
cvector_vector_type(bool) visited = NULL;
List *graph;
Vertex *carCurrentPosition;    // points to the vertex where the car is currently at
Vertex *carCurrentOrientation; // points to the vertex that the car is facing, if facing a wall, then it is NULL
unsigned char numberOfNodesVisited;
unsigned char sizeOfGraph;

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

    int x = 0;
    int y = 0;
    Node *temp = 0;
    if (canGoFront)
    {
        Vertex *temp = List_addVertex(x, y, graph);
    }

    // need to discuss this with the tean
    // have to specify which index is for front, which index is for back etc
    // currentPos->adjacencyList[0] = temp;
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
    carCurrentPosition = List_addVertex(0, 0, graph);
    updateMap(carCurrentPosition, canGoFront, canGoBack, canGoLeft, canGoRight);
}

// TODO
// owner            : Kaho
// description      : drives the car based on the map using dfs
void dfs(Vertex *start, Node *graph, bool visited[])
{
}

// owner            : Kevin
// description      : check if car is facing the vertex that its about to visit
//                      return True if Yes else False
bool isCorrectOrientation(Vertex *aboutToVistVertex, Vertex *carOrientation)
{
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
bool canVisitVertexFromCurrent(Vertex *currentCarPos, Vertex *aboutToVisitVertex)
{
}

// owner            : Kevin
// description      : retrace back to the vertex that is adjacent to the vertex that I am about to visit
//                      this function drives the car
void retraceBackToVertexAdjacentToAboutToVisitVertex(Vertex *currentCarPos, Node *path)
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
// description      :
void bfs()
{
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

// graph test
int main(void)
{
    printf("hello world\n");
    // allocates memory for an empty graph
    // using a linked list to store our graph
    Graph *g = Graph_makeGraph();

    // for addVertex() function needs the x, y coordinate as well as the graph.

    // to add verices to a graph with no edges
    Graph_addVertex(0, 0, g);
    Graph_addVertex(0, 1, g);
    Graph_addVertex(0, 2, g);
    Graph_addVertex(0, 3, g);
    Graph_addVertex(0, 4, g);

    // note: adding an existing vertex will not do anything
    // although the validation check here is very expensive
    Graph_addVertex(0, 0, g);

    // to add edge between 2 existing vertices in a graph
    Graph_addEdge(0, 0, 0, 1, g);
    Graph_addEdge(0, 0, 0, 2, g);
    Graph_addEdge(0, 0, 0, 3, g);
    Graph_addEdge(0, 0, 0, 4, g);
    Graph_addEdge(0, 0, 0, 4, g);

    // note: adding an existing edge between two vertices will not do anything
    // although the validation check here is very expensive
    Graph_addEdge(0, 0, 0, 1, g);

    // how to iterate through the adjacent vertices of a graph
    Vertex **adj = Graph_adj(0, 0, g);
    for (size_t i = 0; i < 4; i++)
    {
        printf("(%d, %d)\n", adj[i]->x, adj[i]->y);
    }

    Graph_display(g);

    // frees memory related to graph
    Graph_destroy(g);
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
