#include "Player2Hand.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameDisplay.h"
#include "CardFactory.h"

Player2Hand::Player2Hand(ALLEGRO_FONT *font, GameDisplay *gamedisplay, CardFactory *card_factory) : PlayerHand::PlayerHand(font, gamedisplay, al_map_rgb(0, 0, 255), 50, GameDisplay::SCREEN_H - ((Card::CARD_BORDER_WIDTH * 2) + Card::CARD_H) - 50, card_factory)
{
}


Player2Hand::~Player2Hand()
{
}

void Player2Hand::draw()
{
	PlayerHand::draw();

	al_draw_text(this->font, al_map_rgb(0, 0, 255), this->x, GameDisplay::SCREEN_H - ((Card::CARD_BORDER_WIDTH * 2) + Card::CARD_H) - 80, ALLEGRO_ALIGN_LEFT, "Player 2");
}
