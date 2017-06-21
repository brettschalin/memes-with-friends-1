#pragma once
#include <stdio.h>
#include <algorithm>
#include "Card.h"
#include "CardFactory.h"
#include "Player1Hand.h"
#include "Player2Hand.h"

#define BOARDSIZE 3
#define HANDSIZE 5
#define NUMBER_OF_PLAYERS 2

struct gameData
{
	int current_player; //zero based
	std::vector<ALLEGRO_COLOR> colors;
	//Card* player_cards[NUMBER_OF_PLAYERS][HANDSIZE];
	Player1Hand* player1Cards;
	Player2Hand* player2Cards;

	Card* board[BOARDSIZE][BOARDSIZE];

};

class GameManager
{
private:
	gameData data;
	CardFactory card_factory = CardFactory();

public:
	GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay);
	~GameManager();
	int get_current_player();
	void set_current_player(int player);
	void play_card(Card* card, int x, int y);
	Card* get_card(int x, int y);
	Card* draw_card_from_hand(int index);
	bool in_bounds(int x, int y);
	//int get_score(int player);


};
