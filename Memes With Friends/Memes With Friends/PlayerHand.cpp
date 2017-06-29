#include <allegro5/allegro_font.h>
#include <memory>

#include "PlayerHand.h"

#include "Card.h"
#include "CardFactory.h"
#include "GameDisplay.h"

PlayerHand::PlayerHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR init_color, int x, int y, CardFactory &card_factory)
	:font{font}, x{x}, y{y}
{
	color = init_color;
	cards.resize(5);
	int i = 0, card_width = Card::CARD_BORDER_WIDTH * 2 + Card::CARD_W;
	for (auto &c : cards) {
		c = std::unique_ptr<Card>{card_factory.create_card()};
		c->set_font(font);
		c->set_gamedisplay(gamedisplay);
		c->set_color(color);
		c->set_pos(x + i++ * (card_width + 10), y);
	}
}

void PlayerHand::draw()
{
	for (auto &c : cards) {
		if (&c == NULL) continue;
		c->draw();
	}
}

std::shared_ptr<Card> PlayerHand::get_card(size_t index)
{
	if (index < 0 || index >= cards.size()) return NULL;

	return cards[index];
}

void PlayerHand::remove_card(size_t index)
{
	if (index < 0 || index >= cards.size()) return;

	cards.erase(cards.begin()+index);

}

size_t PlayerHand::hand_size()
{
	return cards.size();
}

