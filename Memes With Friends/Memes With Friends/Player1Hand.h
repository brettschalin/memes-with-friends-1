#pragma once

#include "PlayerHand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameDisplay.h"
#include "CardFactory.h"

class Player1Hand :
	public PlayerHand
{
public:
	Player1Hand(ALLEGRO_FONT *font, GameDisplay *gamedisplay, CardFactory *card_factory);
	~Player1Hand();
	void draw();
};

