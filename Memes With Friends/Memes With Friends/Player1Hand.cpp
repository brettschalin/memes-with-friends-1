#include "Player1Hand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameDisplay.h"
#include "CardFactory.h"

Player1Hand::Player1Hand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory *card_factory) : PlayerHand::PlayerHand(font, gamedisplay, al_map_rgb(255, 0, 0), 50, 50, card_factory)
{
}


Player1Hand::~Player1Hand()
{
}

void Player1Hand::draw()
{
	PlayerHand::draw();

	al_draw_text(this->font.get(), al_map_rgb(255, 0, 0), this->x, this->y + ((Card::CARD_BORDER_WIDTH * 2) + Card::CARD_H) + 10, ALLEGRO_ALIGN_LEFT, "Player 1");
}
