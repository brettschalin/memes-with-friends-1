#pragma once

#include "PlayerHand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameDisplay.h"
#include "CardFactory.h"

class Player2Hand :
	public PlayerHand
{
public:
	Player2Hand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory *card_factory);
	~Player2Hand();
	void draw();
};

