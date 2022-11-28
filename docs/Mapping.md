# Mapping Module

[Back to Home](../README.md)

## Mapping
Our mapping module incorporates the motor, ultrasonic and communications module to move, detect obstacles and communicate details to users respectively. The mapping module makes use of Depth First Search algorithm to keep track of the visited coordinates in the map and also to drive the car using the Shortest Path created.

## Functions
isCorrectStartingPosition()
Check and return a bool if the car is in the optimal starting position, i.e.
1) Back is blocked
2) Left or Right is blocked

initializeCarPosition()
This function is used to check if the car is in the optimal position to start mapping the maze. In order to fulfill that condition the car has to be in a corner and the front is not blocked. First the isCorrectStartingPosition() function will be called to determine if the car is in the corner. Then data from all the 4 ultrasonic sensors will be used to move the car until it reaches a corner. Lastly, the car will turn until the front is not blocked and back/right or back/left sensors are blocked. The main objective of this function is to make the mapping as efficient as possible.

updateMap()
This is to create a new vertex depending on the direction the car is facing and update the adjacency list for both the vertices.

mapMaze()
This function is the core of the mapping module as it manoeuvres the car through the maze using modified DFS which uses BFS to do the backtracking. 

adjustCarDirection()
This is mainly a helper function for the driveCarUsingPath() function to change the direction of the car when it hits an endpoint.

driveCarUsingPath()
This function is used to identify the set of directions the car had to move from its starting coordinate to target coordinate. It will use the help of adjustCarDirection() helper function to turn the car when it is facing the wall to continue moving.

bfs()
This function just takes in the source vertex and target vertx to construct the shortest path and moves the car to the target vertex.

displayMapOnM5()
This function is used by the Communications module to graphically print the map.
