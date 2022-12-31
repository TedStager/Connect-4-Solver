#include "transposition.hpp"
#include <iostream>
using namespace std;

uint64_t TranspositionTable::genKey (const Position &P)
{
	// add in the player tokens
	uint64_t key = P.player;

	// add board tokens on top
	for (int col = 0; col < 7; col++)
	{
		// find the number of tokens in the column
		int tokenCount = 0;
		bool done = false;

		for (int row = 0; row < 6 && !done; row++)
		{
			if (P.board & P.rowMask[row] & P.columnMask[col])
				tokenCount++;
			else
				done = true;
		}

		// add the bits onto the key
		key += (tokenCount << (42 + 3 * col));
	}

	return key;
}

unsigned int TranspositionTable::genIndex (uint64_t key)
{
	return key % table.size(); // if the size is a prime number should be as unique as possible
							   // collisions occur about 1 in every sqrt(length) entries (see birthday paradox)
}

TranspositionTable::TranspositionTable(int tableSize)
{
	table.resize(tableSize);
}

void TranspositionTable::setScore (const Position &P, int score)
{
	uint64_t key = genKey(P);
	unsigned int index = genIndex(key);

	table[index].key = key; // if there is a collisison, we always overwrite
	table[index].val = score;

	//cout << "Set score: " << score << " at index " << index << endl;
}

int TranspositionTable::getScore (const Position &P)
{
	uint64_t key = genKey(P);
	unsigned int index = genIndex(key);

	if (table[index].key == key) // make sure we didn't overwrite it
	{
		//cout << "got score: " << table[index].val << endl;
		return table[index].val;
	}
	else
	{
		return -30;
	}
}













