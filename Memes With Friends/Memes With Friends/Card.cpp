#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include <vector>
#include <string>
#include <tuple>

#include "Card.h"

#include "GameDisplay.h"

Card::Card(std::string meme, std::vector<int> numbers)
	:meme_image{al_load_bitmap(meme.c_str()), &al_destroy_bitmap}
{
	int i{0};
	for (auto num : numbers) {
		switch (i++) {
		case 0:
			up = num;
			break;
		case 1:
			down = num;
			break;
		case 2:
			left = num;
			break;
		case 3:
			right = num;
			break;
		}
	}
}

std::tuple<int, int> Card::get_pos() {
    return std::tuple<int, int>(x1, y1);
}

bool Card::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    int mouse_x = 0, mouse_y = 0;
    int sx = 0, sy = 0;

    mouse_x = ev.mouse.x;
    mouse_y = ev.mouse.y;
    std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);

    // left button clicked
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && ev.mouse.button == 1) {
        // check if click was in my bounds
        if (contains(sx, sy)) return true;
    }

    return false;
}

bool Card::contains(int x, int y) {
    // if mouseX is >= topleftX and mouseX <= bottomrightX
    // AND
    // mouseY is >= topleftY and mouseY <= bottomrightY
    return (
               (x >= x1) &&
               (x <= x2)
           )
           &&
           (
               (y >= y1) &&
               (y <= y2)
           );
}

void Card::draw()
{
	if (meme_image == nullptr) return;

	// Draw the meme image first at the appropriate location
	al_draw_scaled_bitmap(meme_image.get(),
			      0, 0,
			      al_get_bitmap_width(meme_image.get()),
			      al_get_bitmap_height(meme_image.get()),
			      x1, y1,
			      Card::CARD_W, Card::CARD_H, 0);

	// Draw the border around the meme image to make it a card
	al_draw_rectangle(x1,
			  y1,
			  x2,
			  y2,
			  CARD_COLOR, Card::CARD_BORDER_WIDTH);

	// Draw left number
	al_draw_filled_rectangle(x1 + (CARD_BORDER_WIDTH * 2) - 5,
				 y1 + (CARD_H / 2) - 5,
				 x1 + (CARD_BORDER_WIDTH * 2) + 15,
				 y1 + (CARD_H / 2) + 15,
				 gamedisplay->get_background_color());
	al_draw_text(font.get(), al_map_rgb(0, 0, 0),
		     x1 + (CARD_BORDER_WIDTH * 2),
		     y1 + (CARD_H / 2),
		     ALLEGRO_ALIGN_LEFT,
		     std::to_string(left).c_str());

	// Draw right number
	al_draw_filled_rectangle(x1 + CARD_W - (CARD_BORDER_WIDTH * 2) - 15,
				 y1 + (CARD_H / 2) - 5,
				 x1 + CARD_W - (CARD_BORDER_WIDTH * 2) + 5,
				 y1 + (CARD_H / 2) + 15,
				 gamedisplay->get_background_color());
	al_draw_text(font.get(), al_map_rgb(0, 0, 0),
		     x1 + CARD_W - (CARD_BORDER_WIDTH * 2) - 10,
		     y1 + (CARD_H / 2),
		     ALLEGRO_ALIGN_LEFT,
		     std::to_string(right).c_str());

	// Draw up number
	al_draw_filled_rectangle(x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2) - 5,
				 y1 + (CARD_BORDER_WIDTH * 2) - 5,
				 x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2) + 15,
				 y1 + (CARD_BORDER_WIDTH * 2) + 15,
				 gamedisplay->get_background_color());
	al_draw_text(font.get(), al_map_rgb(0, 0, 0),
		     x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2),
		     y1 + (CARD_BORDER_WIDTH * 2),
		     ALLEGRO_ALIGN_LEFT,
		     std::to_string(up).c_str());

	// Draw down number
	al_draw_filled_rectangle(x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2) - 5,
				 y1 + CARD_H - (CARD_BORDER_WIDTH * 2) - 15,
				 x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2) + 15,
				 y1 + CARD_H - (CARD_BORDER_WIDTH * 2) + 5,
				 gamedisplay->get_background_color());
	al_draw_text(font.get(), al_map_rgb(0, 0, 0),
		     x1 + (CARD_W / 2) - (CARD_BORDER_WIDTH * 2),
		     y1 + CARD_H - (CARD_BORDER_WIDTH * 2) - 10,
		     ALLEGRO_ALIGN_LEFT,
		     std::to_string(down).c_str());
}

void Card::set_pos(int x1, int y1)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x1 + Card::CARD_W;
	this->y2 = y1 + Card::CARD_H;
}

void Card::set_color(ALLEGRO_COLOR color)
{
	CARD_COLOR = color;
}

void Card::set_font(std::shared_ptr<ALLEGRO_FONT> font)
{
	this->font = font;
}

void Card::set_gamedisplay(GameDisplay *gamedisplay)
{
	this->gamedisplay = gamedisplay;
}

int Card::get_up()
{
	return up;
}

int Card::get_down()
{
	return down;
}

int Card::get_left()
{
	return left;
}

int Card::get_right()
{
	return right;
}

bool Card::compare_to_right(Card &othercard)
{
	return right > othercard.get_left();
}

bool Card::compare_to_left(Card &othercard)
{
	return left > othercard.get_right();
}

bool Card::compare_to_up(Card &othercard)
{
	return up > othercard.get_down();
}

bool Card::compare_to_down(Card &othercard)
{
	return down > othercard.get_up();
}
