#pragma once
#include <stdio.h>
#include "Card.h"

#define BOARDSIZE 3
#define HANDSIZE 5

struct gameData
{
	int currentPlayer; //zero based
	ALLEGRO_COLOR* colors[2];
	Card* playerCards[2][HANDSIZE];
	Card* board[BOARDSIZE][BOARDSIZE];

};

class GameManager
{
private:
	gameData* data;

public:
	GameManager();
	~GameManager();
	int getCurrentPlayer();
	void setCurrentPlayer(int player);
	void playCard(int player, int x, int y);
	Card* getCard(int x, int y); //Get the card at the speficied board position
	bool in_bounds(int x, int y);

};
