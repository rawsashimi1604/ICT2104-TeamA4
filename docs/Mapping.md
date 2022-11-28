# Mapping Module

[Back to Home](../README.md)

## Mapping
Our mapping module incorporates the motor and ultrasonic modules to move, detect obstacles and map a maze. The mapping module makes use of Depth First Search algorithm to keep track of the visited coordinates in the map and also to drive the car using the Shortest Path created. It uses Breadth First Search to avoid the worst case for DFS when it backtracks. When it backtracks, it uses BFS to find the shortest path.

## Functions
updateMap()
This is to create a new vertex depending on the direction the car is facing and update the adjacency list for both the vertices.

mapMaze()
This function is the core of the mapping module as it moes the car through the maze using modified DFS which uses BFS to do the backtracking for optimization. 

adjustCarDirection()
This is mainly a helper function for the driveCarUsingPath() function to change the direction of the car when it hits an endpoint.

driveCarUsingPath()
This function is used to identify the set of directions the car had to move from its starting coordinate to target coordinate. It will use the help of adjustCarDirection() helper function to turn the car when it is facing the wall to continue moving.

bfs()
This function takes in the source vertex and target vertx to construct the shortest path and moves the car to the target vertex.

displayMapOnM5()
This function is used by the Communications module to graphically print the map.

## Memory allocation

We used optimal structure padding to minimize the memory space taken when a struct (such as a Vertex) is malloc-ed. This is done by adjusting the structure definition such that it is always front the smallest data type to the biggest type. An example is shown below:

![image](https://user-images.githubusercontent.com/71180280/204321787-d9f8b595-c570-4e33-a94e-4bde97cdf341.png)

## Data structures used

Many auxiliary data structures has also been added to support the mapping module, such as a graph, linked lists, queues and stacks using linked list implementation. Most implementations are linked lists for optimal memory usage, unlike an associative array hash table implementation where there could be empty buckets. The team took this into account due to the limited memory that the MSP432 has. 

## Memory taken on heap space for a 5x4 map

![image](https://user-images.githubusercontent.com/71180280/204321543-5aee568c-83c9-4890-852f-a6f14289788f.png)

## Mapping algorithm bounds

For our mapping algorithm, there is no bounds on the map. It can keep going so long as the car has memory and has not overflowed the int8_t data type. If you refer to the Vertex structure shown earlier, you can see that we use x, y coordinates to store a representation of the map. What this means is our map can go as far as 256 x 256 map, and as long as the car has enough memory. Based on our estimation, if a 5x4 map around 3KB, and there's 64KB SRAM on the MSP432, then it should have no issues storing a 100 x 100 map.

We purposefully chose a generic algorithm for this reason.
