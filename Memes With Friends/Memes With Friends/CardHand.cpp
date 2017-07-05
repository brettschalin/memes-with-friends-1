#include <allegro5/allegro_font.h>
#include <memory>

#include "CardHand.h"

#include "Card.h"
#include "CardFactory.h"
#include "GameDisplay.h"

CardHand::CardHand(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay, ALLEGRO_COLOR init_color, int x, int y, CardFactory &card_factory)
	:font{font}, x{x}, y{y}
{
	color = init_color;
	cards.resize(5);
	int i = 0, card_width = Card::CARD_BORDER_WIDTH * 2 + Card::CARD_W;
	for (auto &c : cards) {
		c = std::shared_ptr<Card>{card_factory.create_card()};
		c->set_font(font);
		c->set_gamedisplay(gamedisplay);
		c->set_color(color);
		c->set_pos(x + i++ * (card_width + 10), y);
	}
}

std::shared_ptr<Card> CardHand::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    for (auto card : cards) {
        if (card->process(ev, gamedisplay)) return card;
    }
    return NULL;
}

void CardHand::draw()
{
	for (auto &c : cards) {
		if (&c == NULL) continue;
		c->draw();
	}
}

std::shared_ptr<Card> CardHand::get_card(size_t index)
{
	if (index < 0 || index >= cards.size()) return NULL;

	return cards[index];
}

void CardHand::remove_card(std::shared_ptr<Card> card)
{
    cards.erase(std::remove(cards.begin(), cards.end(), card), cards.end());
}

unsigned int CardHand::hand_size()
{
	return cards.size();
}

