#include "PlayerHand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include "Card.h"
#include "GameDisplay.h"

PlayerHand::PlayerHand(ALLEGRO_FONT *font, GameDisplay *gamedisplay, ALLEGRO_COLOR color, int x, int y)
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
		Card *card = new Card();
		card->set_font(font);
		card->set_gamedisplay(gamedisplay);
		card->set_color(this->color);
		card->set_pos(real_x, this->y);
		this->cards.push_back(card);
	}
}


PlayerHand::~PlayerHand()
{
}

void PlayerHand::draw()
{
	std::vector<Card *>::iterator it;
	for (it = this->cards.begin(); it < this->cards.end(); it++) {
		Card *card = *it;
		card->draw();
	}
}
