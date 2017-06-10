#pragma once

#include <allegro5/allegro_font.h>

#include "CardFactory.h"
#include "GameDisplay.h"
#include "PlayerHand.h"

class Player1Hand : public PlayerHand
{
public:
	Player1Hand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory);
	void draw();
};

