#include <allegro5/allegro_font.h>

#include "ComputerHand.h"

#include "CardFactory.h"
#include "GameDisplay.h"
#include "PlayerHand.h"
#include "GameManager.h"

ComputerHand::ComputerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, CardFactory &card_factory)
	:CardHand::CardHand(font, gamedisplay, al_map_rgb(255, 0, 0), 50, 50, card_factory)
{
}

void ComputerHand::draw()
{
	CardHand::draw();
	al_draw_text(font.get(), al_map_rgb(255, 0, 0),
		     x, y + Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H + 30,
		     ALLEGRO_ALIGN_LEFT, "Computer");

    al_draw_line(x, y + Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H + 30 + 18, BOARD_LEFT - 20, y + Card::CARD_BORDER_WIDTH * 2 + Card::CARD_H + 30 + 18, al_map_rgb(255, 0, 0), 3);
}
