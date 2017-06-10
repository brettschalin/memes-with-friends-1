#include <allegro5/allegro_font.h>

#include "Player2Hand.h"

#include "CardFactory.h"
#include "GameDisplay.h"
#include "PlayerHand.h"

Player2Hand::Player2Hand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory)
	:PlayerHand::PlayerHand(font, gamedisplay, al_map_rgb(0, 0, 255), 50, GameDisplay::SCREEN_H - (Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H) - 50, card_factory)
{
}

void Player2Hand::draw()
{
	PlayerHand::draw();
	al_draw_text(font.get(), al_map_rgb(0, 0, 255),
		     x, GameDisplay::SCREEN_H - (Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H) - 80,
		     ALLEGRO_ALIGN_LEFT, "Player 2");
}
