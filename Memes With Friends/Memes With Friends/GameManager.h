#pragma once
#include <stdio.h>
#include "Card.h"

#define BOARDSIZE 3
#define HANDSIZE 5

struct gameData
{
	int currentPlayer; //zero based
	std::vector<ALLEGRO_COLOR> colors;
	Card* playerCards[2][HANDSIZE];
	Card* board[BOARDSIZE][BOARDSIZE];
	int* scores;

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
	void playCard(Card* card, int x, int y);
	Card* getCard(int x, int y);
	Card* drawCardFromHand(int index);
	bool in_bounds(int x, int y);
	int getScore(int player);


};
