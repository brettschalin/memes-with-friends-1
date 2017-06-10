#include "PlayerHand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <memory>
#include <utility>
#include <vector>
#include "Card.h"
#include "GameDisplay.h"
#include "CardFactory.h"
#include <iostream>
PlayerHand::PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR color, int x, int y, CardFactory *card_factory)
{
	this->font = font;
	this->color = color;
	this->x = x;
	this->y = y;
	int i;
	int real_x;
	int card_width = (Card::CARD_BORDER_WIDTH * 2) + Card::CARD_W;
	for (i = 0; i < 5; i++) {
		real_x = this->x + (i * (card_width + 10));
		std::unique_ptr<Card> card{card_factory->create_card()};
		card->set_font(font);
		card->set_gamedisplay(gamedisplay);
		card->set_color(this->color);
		card->set_pos(real_x, this->y);
		this->cards.push_back(std::move(card));
	}
}


PlayerHand::~PlayerHand()
{
}

void PlayerHand::draw()
{
	for (auto &c : this->cards) {
		c->draw();
	}
}
