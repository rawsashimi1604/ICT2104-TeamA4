#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "queue.h"
#include "stack.h"
#include "linkedlist.h"

#include "cvector.h"
#include "cvector_utils.h"

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

    // updateMap()
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
    currentPos->adjacencyList[0] = temp;
}

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

// owner            : Kaho
// description      : returns a string representation for the Communications module to print the map
char *getMap(void)
{
}

// owner            : Kaho
// description      : iterates through the graph and check if the maze is explored
bool isMapExplored(void)
{
    // return n_of_visted nodes == n_of_all_nodes
}

// linked list test
int main(void)
{
    // allocates memory for an empty list
    // using a linked list to store our graph
    List *list = List_makeList();

    // for add()/delete(), function needs the x, y coordinate as well as the list.

    // deleting an item that does not exist does nothing
    List_delete(0, 0, list);
    List_addVertex(0, 0, list);
    List_addVertex(0, 1, list);
    List_addVertex(0, 2, list);
    List_addVertex(0, 3, list);
    List_delete(0, 1, list);
    List_delete(20, 20, list);
    List_display(list);
    // =========================
    List_reverse(list);
    printf("Reversed: using three pointers. \n");
    List_display(list);
    List_reverse_using_two_pointers(list);
    printf("Reversed: using two pointers. \n");
    List_display(list);
    // =========================

    // frees memory related to list
    List_destroy(list);
}

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
