# Project 2 — Maze Solver

Your name: Scott Tennison

## Algorithm Description

This program solves mazes using Breadth-First Search (BFS) with a queue. Starting from the S position, the algorithm explores all neighboring cells level by level, adding valid moves (non-wall, unvisited cells) to the queue. Each cell tracks where it came from using a parent array. When E is reached, the program traces backwards through the parent array from E to S, marking each open space along the path with -. If the queue empties without finding E, the maze has no solution.

## Test Cases Description

A maze with no solution (E is completely walled off)
A maze with no S (missing start)
A maze with wrong row count (header says 4 rows but only 3 exist)
A maze with an invalid character (like a ? or number)
A maze with a valid but tricky long path

## Optional: Things to Know
