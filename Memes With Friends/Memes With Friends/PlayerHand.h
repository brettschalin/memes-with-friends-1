#pragma once

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include "Card.h"
#include "GameDisplay.h"

class PlayerHand
{
protected:
	int x;
	int y;
	ALLEGRO_COLOR color;
	ALLEGRO_FONT *font;
	std::vector<Card *> cards;
public:
	PlayerHand(ALLEGRO_FONT *font, GameDisplay *gamedisplay, ALLEGRO_COLOR color, int x, int y);
	~PlayerHand();
	void draw();
};

