#include "position.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

// trivial 2^n function to make defining things easier
unsigned long long pow2(int n)
{
	return pow(2, n);
}

// need to define our mask constants (so that we can look at a specific row or column)
const uint64_t Position::rowMask[6] = 
			{
				(pow2(0) + pow2(1) + pow2(2) + pow2(3) + pow2(4) + pow2(5) + pow2(6)),
				(pow2(7) + pow2(8) + pow2(9) + pow2(10) + pow2(11) + pow2(12) + pow2(13)),
				(pow2(14) + pow2(15) + pow2(16) + pow2(17) + pow2(18) + pow2(19) + pow2(20)),
				(pow2(21) + pow2(22) + pow2(23) + pow2(24) + pow2(25) + pow2(26) + pow2(27)),
				(pow2(28) + pow2(29) + pow2(30) + pow2(31) + pow2(32) + pow2(33) + pow2(34)),
				(pow2(35) + pow2(36) + pow2(37) + pow2(38) + pow2(39) + pow2(40) + pow2(41))
		   };
const uint64_t Position::columnMask[7] = 
			{
				(pow2(0) + pow2(7) + pow2(14) + pow2(21) + pow2(28) + pow2(35)),
				(pow2(1) + pow2(8) + pow2(15) + pow2(22) + pow2(29) + pow2(36)),
				(pow2(2) + pow2(9) + pow2(16) + pow2(23) + pow2(30) + pow2(37)),
				(pow2(3) + pow2(10) + pow2(17) + pow2(24) + pow2(31) + pow2(38)),
				(pow2(4) + pow2(11) + pow2(18) + pow2(25) + pow2(32) + pow2(39)),
				(pow2(5) + pow2(12) + pow2(19) + pow2(26) + pow2(33) + pow2(40)),
				(pow2(6) + pow2(13) + pow2(20) + pow2(27) + pow2(34) + pow2(41))
			};

// constructor for an empty board
Position::Position ()
{
	player = 0;
	board = 0;
	numMoves = 0;
}

// "copy" constructor function, makes a new Position identical to the last
Position::Position (const Position &P)
{
	player = P.player;
	board = P.board;
	numMoves = P.numMoves;
}

int Position::maxScore () const
{
	return 21 - (*this).numMoves / 2;
}

/* says if there's room in a given column in the position
	note that column is 0-indexed */
bool Position::canPlay (int col) const
{
	if (col > 6 || col < 0)
	{
		cout << "Error: column out of bounds" << endl;
		return false;
	}

	if ((board & columnMask[col]) == columnMask[col]) // then the column must be full
	{
		return false;
	}

	return true;
}

void Position::playCol (int col)
{
	for (int row = 0; row <= 5; row++)
	{
		if (board & (rowMask[row] & columnMask[col]))
		{
			//cout << "we contuin";
			continue;
		}
		else
		{
			player += (columnMask[col] & rowMask[row]);
			board += (columnMask[col] & rowMask[row]);
			break;
		}
	}


	numMoves++;

	// switch player and opponent
	player ^= board;
}

// determines if a copy of the player's tokens with the new column played has 4 in a row
bool Position::willWin (int col) const
{
	uint64_t newState = player;
	bool win = false;

	// if we can't play, why bother
	if (!canPlay(col))
	{
		return false;
	}

	// play the token in newState
	for (int row = 0; row <= 5; row++)
	{
		if (board & (rowMask[row] & columnMask[col]))
		{
			//cout << "skipping row " << row << endl;
			continue;
		}
		else
		{
			newState += (columnMask[col] & rowMask[row]);
			//cout << "playing row " << row << " and column " << col << endl;
			break;
		}
	}


	// horizontal wins
	for (int row = 0; row <= 5 && !win; row++)
	{
		for (int column = 0; column <= 3 && !win; column++) // note column here is the starting column
		{
			if ((rowMask[row] & columnMask[column] & newState) &&
				 (rowMask[row] & columnMask[column + 1] & newState) &&
				 (rowMask[row] & columnMask[column + 2] & newState) &&
				 (rowMask[row] & columnMask[column + 3] & newState))
			{
				win = true;
			}
		}
	}

	// vertical wins
	for (int column = 0; column <= 6 && !win; column++)
	{
		for (int row = 0; row <= 2 && !win; row++)
		{
			if ((rowMask[row] & columnMask[column] & newState) &&
				 (rowMask[row + 1] & columnMask[column] & newState) &&
				 (rowMask[row + 2] & columnMask[column] & newState) &&
				 (rowMask[row + 3] & columnMask[column] & newState))
			{
				win = true;
			}
		}
	}

	// diagonal wins going up (positive slope?)
	for (int column = 0; column <= 3 && !win; column++)
	{
		for (int row = 0; row <= 2 && !win; row++)
		{
			if ((rowMask[row] & columnMask[column] & newState) &&
				 (rowMask[row + 1] & columnMask[column + 1] & newState) &&
				 (rowMask[row + 2] & columnMask[column + 2] & newState) &&
				 (rowMask[row + 3] & columnMask[column + 3] & newState))
			{
				win = true;
			}
		}
	}

	// negative slope wins
	for (int column = 0; column <= 3 && !win; column++)
	{
		for (int row = 5; row >= 3 && !win; row--)
		{
			if ((rowMask[row] & columnMask[column] & newState) &&
				 (rowMask[row - 1] & columnMask[column + 1] & newState) &&
				 (rowMask[row - 2] & columnMask[column + 2] & newState) &&
				 (rowMask[row - 3] & columnMask[column + 3] & newState))
			{
				win = true;
			}
		}
	}

	// return if we won
	return win;
}

bool Position::isLosingMove (int col) const
{
	Position NewPos(*this);
	NewPos.playCol(col);

	for (int x = 0; x < 7; x++)
	{
		if (NewPos.willWin(x))
			return true;
	}

	return false;
}

// draw function
bool Position::isDraw () const
{
	if (numMoves == 42)
		return true;
	return false;
}


void displayBoard (const Position &P)
{
	for (int j = 5; j >= 0; j--)
	{
		for (int i = 0; i <= 6; i++)
		{
			cout << setw(3);
			if ((P.rowMask[j] & P.columnMask[i]) & (P.player & P.board))
				cout << 'X';
			else if ((P.rowMask[j] & P.columnMask[i]) & (P.player ^ P.board))
				cout << 'O';
			else
				cout << '.';
		}

		cout << endl;
	}

	cout << endl;
}

