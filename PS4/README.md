# PS4 - The Kevin Bacon Game
## Graphs and Maps in Java - Graphlib.java

This program uses breadth first search to find the closest "path" between actors. It trains from a large
set of actors (vertices) and the movies they share (directed lines) then, according to user input, creates
a bfs graph with the actor chosen as the root. The program can then compute the closest path between another
user input actor.

### How to make the algorithm faster
The program takes some time to run a new 'center of the universe' root actor because of the algorithm
for average separation. It was part of the assignment and deals with creating a BFS tree for every
connection which has a non-zero path to the root and saving the path size to calculate
an average later. Follow the instructions on line 138 of GraphLib to eliminate this process.