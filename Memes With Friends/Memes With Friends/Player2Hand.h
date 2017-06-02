#pragma once

#include "PlayerHand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameDisplay.h"

class Player2Hand :
	public PlayerHand
{
public:
	Player2Hand(ALLEGRO_FONT *font, GameDisplay *gamedisplay);
	~Player2Hand();
	void draw();
};

