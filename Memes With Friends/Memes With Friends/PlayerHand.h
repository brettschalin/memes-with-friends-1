#pragma once

#include <allegro5/allegro_font.h>

#include "CardFactory.h"
#include "GameDisplay.h"
#include "CardHand.h"

class PlayerHand : public CardHand
{
public:
	PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory);
	void draw();
};

