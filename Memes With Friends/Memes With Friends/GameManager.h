#pragma once
#include <stdio.h>
#include "Card.h"
#include "CardFactory.h"

#define BOARDSIZE 3
#define HANDSIZE 5

struct gameData
{
	int current_player; //zero based
	std::vector<ALLEGRO_COLOR> colors;
	Card* player_cards[2][HANDSIZE];
	Card* board[BOARDSIZE][BOARDSIZE];
	int scores[2];

};

class GameManager
{
private:
	gameData data;
	CardFactory card_factory = CardFactory();

public:
	GameManager();
	~GameManager();
	int get_current_player();
	void set_current_player(int player);
	void play_card(Card* card, int x, int y);
	Card* get_card(int x, int y);
	Card* draw_card_from_hand(int index);
	bool in_bounds(int x, int y);
	int get_score(int player);


};
