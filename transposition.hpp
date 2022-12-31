#pragma once
#include "position.hpp"
#include <vector>
using namespace std;

class TranspositionTable 
{

private:

	// hashes the position into a single number
	uint64_t genKey (const Position &P);

	// assigns each key a semi-unique index so the table doesn't need to be searched
	unsigned int genIndex (uint64_t key);

	// stores each entry as a key-value pair
	struct KeyVal
	{
		uint64_t key;
		int8_t val;
	};

	// use a vector to be able to resize the table
	vector<KeyVal> table;

public:

	// constructs a table with a given size
	// can be changed to accomodate different memory availability
	TranspositionTable (int tableSize);

	int getScore (const Position &P);

	void setScore (const Position &P, int score);

};