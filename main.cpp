#include <iostream>
#include "position.hpp"
#include "algorithm.hpp"
using namespace std;

// wrote the game function outside of the main for easier debugging
void playGame();

int main()
{
	playGame();

	return EXIT_SUCCESS;
}

void playGame()
{
	Position game;
	int playerIn = -1;
	bool gameEnd = false;

	TranspositionTable table(7111123); // prime number corresponding to approx. 64 MB (each entry is 9 bytes)
	// a prime number size results in less collisions

	cout << "During player 1's turn, input -3 to activate negamax" << endl;

	while (!gameEnd)
	{
		// first player
		cout << "Player 1's turn: " << endl;
		displayBoard(game);
		cin >> playerIn;

		if (playerIn == -3)
		{
			int best = optimalMove(game, table);
			cout << "best move is " << best << endl;
			cout << "enter move: " << endl;
			cin >> playerIn;
		}
		
		if (game.canPlay(playerIn))
		{
			if (game.willWin(playerIn))
			{
				game.playCol(playerIn);
				displayBoard(game);
				cout << "Player 1 wins!" << endl;
				gameEnd = true;
			}			

			game.playCol(playerIn);
		}
		else
		{
			cout << "bad input error" << endl;
			gameEnd = true;	// bad input ends game
		}

		// second player
		cout << "Player 2's turn: " << endl;
		displayBoard(game);
		cin >> playerIn;
		
		if (game.canPlay(playerIn) && !gameEnd)
		{
			if (game.willWin(playerIn))
			{
				game.playCol(playerIn);
				displayBoard(game);
				cout << "Player 2 wins!" << endl;
				gameEnd = true;
			}			

			game.playCol(playerIn);
		}
		else
		{
			cout << "bad input error" << endl;
			gameEnd = true;
		}


		// draw only happens after player 2's turn
		if (game.isDraw())
		{
			cout << "You drew!" << endl;
			gameEnd = true;
		}
	}
}