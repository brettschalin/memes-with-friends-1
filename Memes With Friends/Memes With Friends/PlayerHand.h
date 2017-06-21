#pragma once

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <array>
#include <memory>

#include "Card.h"
#include "CardFactory.h"
#include "GameDisplay.h"

class PlayerHand
{
protected:
	std::shared_ptr<ALLEGRO_FONT> font;
	ALLEGRO_COLOR color;
	int x;
	int y;
	std::vector<std::shared_ptr<Card>> cards;
public:
	PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR init_color, int x, int y, CardFactory &card_factory);
	// This class can't be copied
	PlayerHand(const PlayerHand &) = delete;
	PlayerHand &operator=(const PlayerHand &) = delete;
	void draw();
	Card* get_card(int index);
	void remove_card(int index);
};
