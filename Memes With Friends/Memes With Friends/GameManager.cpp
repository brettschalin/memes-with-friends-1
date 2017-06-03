#include "GameManager.h"

GameManager::GameManager()
{
	//Assign who's first to play
	int currPlayer = (rand() % 2);
	setCurrentPlayer(currPlayer);

	//Initialize everyone's hand...
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < HANDSIZE; j++)
		{
			data->playerCards[i][j] = new Card();
		}
	}

	//...and the board
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			data->board[i][j] = NULL;
		}
	}

};

GameManager::~GameManager()
{
	for (int i = 0; i < HANDSIZE; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (data->playerCards[j][i] != NULL)
				delete data->playerCards[j][i];
		}
	}

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (data->board[j][i])
				delete data->board[j][i];
		}
	}

	delete data->playerCards;
	delete data->board;
	delete data;
}

int GameManager::getCurrentPlayer()
{
	return data->currentPlayer;
}

void GameManager::setCurrentPlayer(int player)
{
	data->currentPlayer = player;
}

void GameManager::playCard(int player, int x, int y)
{
	//complicated
	//and in progress

	/*
	The general strategy here is to do the calculations
	and let the main game loop handle animations.
	We will assume that the move is legal
	>first, remove the Card() from the active player's hand
	 and put it on the board
	>then determine which Card()s flip colors
	
	
	*/

}


Card* GameManager::getCard(int x, int y)
{

	return data->board[x][y];

}
