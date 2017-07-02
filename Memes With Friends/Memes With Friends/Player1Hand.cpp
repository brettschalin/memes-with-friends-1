#include <allegro5/allegro_font.h>

#include "Player1Hand.h"

#include "CardFactory.h"
#include "GameDisplay.h"
#include "PlayerHand.h"

Player1Hand::Player1Hand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory)
	:PlayerHand::PlayerHand(font, gamedisplay, al_map_rgb(255, 0, 0), 50, 50, card_factory)
{
}

void Player1Hand::draw()
{
	PlayerHand::draw();
	al_draw_text(font.get(), al_map_rgb(255, 0, 0),
		     x, y + Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H + 10,
		     ALLEGRO_ALIGN_LEFT, "Computer");
}
