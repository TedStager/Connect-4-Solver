# Connect-4-Solver
Uses a Negamax algorithm to provide the mathematically optimal move for Connect 4.

This project was heavily inspired by Pascal Pons' implementation of the same algorithm (see connect4.gamesolver.org) as well as various algorithms described on the Chess Programming Wiki (see chessprogramming.org).

Note that this is a work in progress, and the negamax algorithm occasionally gets stuck. The current version works most of the time reasonably well, but is prone to failure if the algorithm is run to early in the game (and there are too many game states to search).
