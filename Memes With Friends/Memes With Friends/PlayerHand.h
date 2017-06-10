#pragma once

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <memory>
#include "Card.h"
#include "GameDisplay.h"
#include "CardFactory.h"

class PlayerHand
{
protected:
	int x;
	int y;
	ALLEGRO_COLOR color;
	std::shared_ptr<ALLEGRO_FONT> font;
	std::vector<std::unique_ptr<Card>> cards;
public:
	PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR color, int x, int y, CardFactory *card_factory);
	// This class can't be copied
	PlayerHand(const PlayerHand &) = delete;
	PlayerHand &operator=(const PlayerHand &) = delete;
	~PlayerHand();
	void draw();
};

