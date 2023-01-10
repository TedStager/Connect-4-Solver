#pragma once
#include <cstdint>

// stores a specific position of a connect 4 board
// uses a bitmap of player stones and of the total occupied spaces on the board

/*
	note our board will be arranged with the bits as follows (starting from 0), using 
	this format to store the player tokens
	
row:     6 | 35 36 37 38 39 40 41
		 5 | 28 29 30 31 32 33 34
		 4 | 21 22 23 24 25 26 27
		 3 | 14 15 16 17 18 19 20
		 2 |  7  8  9 10 11 12 13
		 1 |  0  1  2  3  4  5  6
		   -----------------------
col:          1  2  3  4  5  6  7

	then the data storing all the tokens will be arranged as follows: (not yet, but potentially)

	   44 47 50 53 56 59 62
	   43 46 49 52 55 58 61
	   42 45 48 51 54 57 60
	   --------------------
col:    1  2  3  4  5  6  7

	including a bit is the same as adding 2^n to the total board sum
*/

class Position
{
	public:

		// define the bitwise masks, as well as our board
		// using uint64_t from cstd int so that the integer size is always the same
		static const uint64_t columnMask[7];
		static const uint64_t rowMask[6];
		uint64_t player, board; 

		// variable for the number of moves since beginning of game
		int numMoves;

		// base constructor function, makes one with all 0s
		Position ();

		// copy constructor function
		Position (const Position &P);

		// return best possible score here
		int maxScore () const;

		// returns if a column is playable
		bool canPlay (int column) const;

		// determines if a move loses so we don't search there
		bool isLosingMove (int column) const;

		// updates the object if a position is played
		void playCol (int column);

		// indicate if any move will connect 4 tokens
		bool willWin (int column) const;

		// determines if game is draw (ie all spots are occupied with no win)
		bool isDraw () const;
};

// takes in a position and displays board to console
void displayBoard (const Position &P);

// raises 2 to the power of a number, for easy binary def'n
unsigned long long pow2 (int n);



















