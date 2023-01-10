#include "algorithm.hpp"
#include <iostream>
#include <cmath>
using namespace std;

// declare our pointer here so that everything can see it without having to pass
TranspositionTable *transTable;

int negamax (const Position &P, int alpha, int beta)
{

	cout << "(" << alpha << ", " << beta << ")" << endl;
	cout << "depth: " << P.numMoves << endl;

	// static eval of pos
	if (P.isDraw()) // run draw first since it's quicker
	{
		cout << "static eval of score: draw / 0" << endl;
		return 0;
	}

	for (int col = 0; col < 7; col++)
	{
		if (P.willWin(col))
		{
			cout << "static eval of score: win / " << P.maxScore()  << " in col " << col << endl;
			return P.maxScore(); // winning this turn is equivalent to max score
		}
	}

	int tableScore = transTable->getScore(P);
	if (tableScore != -30)
	{
		cout << "used table to get score: " << tableScore << endl;
		return tableScore;
	}

	// check if opp will win in col
	for (int col = 0; col < 7; col++)
	{
		if (P.isLosingMove(col))
		{
			cout << "opp wins in col " << col << endl;
			Position P2(P);
			P2.playCol(col);
			return -P2.maxScore();
		}
	}


	// make sure upper bound is still possible
	if (beta > P.maxScore())
	{
		cout << "reset beta to " << P.maxScore() << endl;
		beta = P.maxScore();
	}

	// prune if the window is empty
	if (alpha >= beta)
	{
		cout << "found an empty window, cut off" << endl;
		return beta;
	}

	// boolean for if we fully search or run a null window
	// init to true to find PV
	bool fullSearch = true;

	int score = START_ALPHA; // impossibly low val to start with

	for (int col = 0; col < 7; col++)
	{
		if (P.canPlay(colOrder[col]))
		{
			Position P2(P);
			P2.playCol(colOrder[col]);

			if (fullSearch)
				score = -negamax(P2, -beta, -alpha);

			else 
			{
				// if it passes null window (will be better) run full search
				if (nullWindow(P2, alpha))
					score = -negamax(P2, -beta, -alpha);
				else
					score = alpha;
			}

			if (score >= beta)
			{
				cout << "fails hard: " << score << " >= " << beta << endl;
				return beta; // fail hard, score can't be bigger than beta
			}
			else
				cout << "score in range" << endl;

			// if the score is better than lower bound, set new lower bound
			if (score > alpha)
			{
				cout << "found a better score: alpha/score = " << score << endl;
				alpha = score;
			}

			// reset so we only search better nodes on next goaround
			fullSearch = false; 
		}
		else
		{
			cout << "won't consider column " << colOrder[col] << endl;
			//displayBoard(P);
		}
	}

	// log the score we found
	transTable->setScore(P, alpha);

	cout << "returning back a node" << endl;

	// highest possible guaranteed score
	return alpha;
}

bool nullWindow (const Position &P, int alpha)
{
	cout << "null window with alpha = " << alpha << endl;

	int beta = alpha + 1;
	
	cout << "running negamax (from null window): alpha = " << -beta << " beta = " << -alpha << endl;
	int score = -negamax(P, -beta, -alpha);

	if (score > alpha)
	{
		cout << "ran a null window with alpha = " << alpha << endl << "result: true" << endl;
		return true;
	}

	cout << "ran a null window with alpha: " << alpha << endl << "result: false" << endl;
	return false; 
}

// use negamax on each possible node to find the best one
int optimalMove (const Position &P, TranspositionTable &tTable)
{
	// initialize with impossible/bad values
	int bestColumn = -1;
	int score = 0, maxScore = -21;
	int colScores[7] = {-30};

	// tell our pointer where the table is
	transTable = &tTable;

	for (int col = 0; col <= 6; col++)
		if (P.willWin(colOrder[col]))
		{
			cout << "you will win in column (from optmove) " << colOrder[col] << endl;
			return colOrder[col];
		}

	for (int col = 0; col <= 6; col++)
	{
		if (P.canPlay(colOrder[col]))
		{
			// otherwise, start negamaxing
			Position P2(P);
			P2.playCol(colOrder[col]);

			score = -negamax(P2, -21, 21); // initial call of negamax

			cout << "=============== OVERALL SCORE OF COL " << colOrder[col] << " IS " << score << "===================" << endl;

			colScores[colOrder[col]] = score;

			if (score > maxScore)
			{
				maxScore = score;
				bestColumn = colOrder[col];
			}
		}
		else
		{
			cout << "can't play col (from optmove) " << colOrder[col] << endl;
		}
	}

	cout << "Scores of columns: " << endl;

	for (int x = 0; x < 7; x++)
		cout << colScores[x] << " ";
	cout << endl;

	return bestColumn;
}




