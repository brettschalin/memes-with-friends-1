#pragma once
#include <stdio.h>
#include <algorithm>
#include "Card.h"
#include "CardFactory.h"
#include "ComputerHand.h"
#include "PlayerHand.h"
#include "Point.h"
#include "MenuButton.h"

#define BOARDSIZE 3
#define HANDSIZE 5
#define NUMBER_OF_PLAYERS 2

//board boundaries. Might need changing
#define BOARD_LEFT 1015
#define BOARD_RIGHT 1573
#define BOARD_TOP 96
#define BOARD_BOTTOM 984
#define BOARD_W (BOARD_RIGHT - BOARD_LEFT)
#define BOARD_H (BOARD_BOTTOM - BOARD_TOP)
#define BOARD_SLOTSIZE_W (BOARD_W / BOARDSIZE)
#define BOARD_SLOTSIZE_H (BOARD_H / BOARDSIZE)

struct gameData
{
	PLAYER current_player;
    std::shared_ptr<ComputerHand> computerCards;
    std::shared_ptr<PlayerHand> playerCards;
    std::array<std::array<std::shared_ptr<Card>, BOARDSIZE>, BOARDSIZE> board;
    int playerscore;
    int computerscore;
};

struct gridHoverData {
    int gridx;
    int gridy;
    bool shouldhighlight;
};

class GameManager
{
private:
    ALLEGRO_COLOR computercolor;
    ALLEGRO_COLOR playercolor;
	gameData data;
    std::shared_ptr<CardFactory> card_factory;
    void draw_card_from_hand(std::shared_ptr<Card> card);
    void flip_color(std::shared_ptr<Card> card);
    std::shared_ptr<Card> selected_card;
    gridHoverData hoverdata;
    std::shared_ptr<ALLEGRO_FONT> font;
    std::shared_ptr<ALLEGRO_FONT> buttonfont;
    std::shared_ptr<ALLEGRO_FONT> gameoverfont;
    GameDisplay *gamedisplay;
    bool gameover;
    std::shared_ptr<MenuButton> newgame;
    DIFFICULTY difficulty;
    std::string debugoutput;

public:
	GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay);
	PLAYER get_current_player();
	void set_current_player(PLAYER player);
	STATUS play_card(std::shared_ptr<Card> card, int x, int y);
	std::shared_ptr<Card> get_card(int x, int y);
	bool in_bounds(int x, int y);
	int get_score(PLAYER player);
    void draw();
    gameData get_data(); // TEMPORARY
    void process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay);
    bool grid_occupied(int x, int y);
	void draw_horizontal_line(float y, ALLEGRO_COLOR color);
	void draw_vertical_line(float x, ALLEGRO_COLOR color);
    std::tuple<int, int> gridslotat(int x, int y);
    bool gridcontains(int x, int y, Point topleft, Point bottomright);
    void aiturn();
    bool get_gameover();
    void set_difficulty(DIFFICULTY difficulty);
    std::string get_debugoutput();
};
