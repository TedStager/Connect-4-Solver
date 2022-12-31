#pragma once
#include "position.hpp"
#include "transposition.hpp"

// negamax: returns the best possible score (assuming perfect play) of a column using negamax algorithm
/*
moves are evaluated on score, where the score is how many stones you have left before the winning move
a score of 0 denotes a draw is your best possible outcome in that path

negamax works on the principle that the score of the opponent is -1 * your score
if the opponent can win with 12 stones left, then the score WRT the player of that path is -12

base function works as a recursive call, we go through each possible iteration and build
the stack until a win is reached (or a draw) at which point we compute the score and go 
from there

we always keep updating the score with the best score (most positive)

can further increase efficiency using alpha beta pruning
algorithm will only explore down a node that could possible supply a better score
if a node lies outside the interval (alpha, beta), the function will not recurse down

alpha is the lowest score for us that we will search, if a node is below alpha we stop searching
once a score higher than alpha is found, alpha is then set to that score

we will implement a Principal Variation search algorithm
first we explore all the way down a path
then we only look to see if the next node to search can beat that path
	narrowing the alpha beta window to [alpha, alpha + 1]
	since we're using a failhard framework, a score of alpha ties (or is worse than) that pos, alpha + 1 is better
if the score is better we explore that route and set a new alpha

*/
int negamax (const Position &P, int alpha, int beta);

// determines if a move has a better or same/worse outcome than a given score (alpha)
bool nullWindow (const Position &P, int alpha);

// returns what column to play in, calling negamax
int optimalMove (const Position &P, TranspositionTable &tTable);

// initial values for alpha and beta, must be impossibly high/low
const int START_ALPHA = -21;
const int START_BETA = 21;
// alpha is too low and we work our way up
// beta is too high and we work our way down

// array to define in what order we explore the columns
const int colOrder[7] = {3, 4, 2, 5, 1, 6, 0};
// center columns then work our way out
// center columns are generally better, and alpha-beta works best when best moves are first
// this idea brought to you courtesy of pascal pons