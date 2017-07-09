#pragma once

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <array>
#include <memory>

#include "Card.h"
#include "CardFactory.h"
#include "GameDisplay.h"

class CardHand
{
protected:
	std::shared_ptr<ALLEGRO_FONT> font;
	ALLEGRO_COLOR color;
	int x;
	int y;
	std::vector<std::shared_ptr<Card>> cards;
public:
	CardHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR init_color, int x, int y, CardFactory &card_factory);
	// This class can't be copied
	CardHand(const CardHand &) = delete;
	CardHand &operator=(const CardHand &) = delete;
	void draw();
	std::shared_ptr<Card> get_card(unsigned int index);
    std::vector<std::shared_ptr<Card>> get_cards();
	void remove_card(std::shared_ptr<Card> card);
	unsigned int hand_size();
    std::shared_ptr<Card> process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay);
};
