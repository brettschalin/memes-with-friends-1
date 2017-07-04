#pragma once
#include <stdio.h>
#include <algorithm>
#include "Card.h"
#include "CardFactory.h"
#include "ComputerHand.h"
#include "PlayerHand.h"

#define BOARDSIZE 3
#define HANDSIZE 5
#define NUMBER_OF_PLAYERS 2

struct gameData
{
	int current_player; //zero based
	std::vector<ALLEGRO_COLOR> colors;
    std::shared_ptr<ComputerHand> computerCards;
    std::shared_ptr<PlayerHand> playerCards;
	std::shared_ptr<Card> board[BOARDSIZE][BOARDSIZE];

	int scores[BOARDSIZE*BOARDSIZE];
};

class GameManager
{
private:
	gameData data;
    std::shared_ptr<CardFactory> card_factory;

public:
	GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay);
	~GameManager();
	int get_current_player();
	void set_current_player(int player);
	void play_card(std::shared_ptr<Card> card, int x, int y);
	std::shared_ptr<Card> get_card(int x, int y);
	std::shared_ptr<Card> draw_card_from_hand(int index);
	bool in_bounds(int x, int y);
	int get_score(int player);
    void draw();
};
