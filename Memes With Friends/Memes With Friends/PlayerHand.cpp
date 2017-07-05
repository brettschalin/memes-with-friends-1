#include <allegro5/allegro_font.h>

#include "PlayerHand.h"

#include "CardFactory.h"
#include "GameDisplay.h"
#include "PlayerHand.h"

PlayerHand::PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory)
	:CardHand::CardHand(font, gamedisplay, al_map_rgb(0, 0, 255), 50, GameDisplay::SCREEN_H - (Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H) - 50, card_factory)
{
}

void PlayerHand::draw()
{
	CardHand::draw();
	al_draw_text(font.get(), al_map_rgb(0, 0, 255),
		     x, GameDisplay::SCREEN_H - (Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H) - 105,
		     ALLEGRO_ALIGN_LEFT, "Player");
}
