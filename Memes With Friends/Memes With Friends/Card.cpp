#include "Card.h"
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_physfs.h>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
#include <iostream>

Card::Card(std::string meme, std::vector<int> numbers)
{
	this->meme_image = al_load_bitmap(meme.c_str());

	std::vector<int>::iterator it;
	int i = 0;
	for (it = numbers.begin(); it < numbers.end(); it++, i++) {
		switch (i) {
		case 0:
			this->up = *it;
			break;
		case 1:
			this->down = *it;
			break;
		case 2:
			this->left = *it;
			break;
		case 3:
			this->right = *it;
			break;
		}
	}
}

Card::~Card()
{
	al_destroy_bitmap(this->meme_image);
}

void Card::draw()
{
	if (!this->meme_image) return;

	// Draw the meme image first at the appropriate location
	al_draw_scaled_bitmap(this->meme_image, 0, 0, al_get_bitmap_width(this->meme_image), al_get_bitmap_height(this->meme_image), this->x1, this->y1, Card::CARD_W, Card::CARD_H, 0);

	// Draw the border around the meme image to make it a card
	al_draw_rectangle(this->x1, this->y1, this->x2, this->y2, this->CARD_COLOR, Card::CARD_BORDER_WIDTH);

	// Draw left number
	al_draw_filled_rectangle(this->x1 + (this->CARD_BORDER_WIDTH * 2) - 5, this->y1 + (this->CARD_H / 2) - 5, this->x1 + (this->CARD_BORDER_WIDTH * 2) + 15, this->y1 + (this->CARD_H / 2) + 15, this->gamedisplay->get_background_color());
	al_draw_text(font, al_map_rgb(0, 0, 0), this->x1 + (this->CARD_BORDER_WIDTH * 2), this->y1 + (this->CARD_H / 2), ALLEGRO_ALIGN_LEFT, std::to_string(left).c_str());

	// Draw right number
	al_draw_filled_rectangle(this->x1 + this->CARD_W - (this->CARD_BORDER_WIDTH * 2) - 15, this->y1 + (this->CARD_H / 2) - 5, this->x1 + this->CARD_W - (this->CARD_BORDER_WIDTH * 2) + 5, this->y1 + (this->CARD_H / 2) + 15, this->gamedisplay->get_background_color());
	al_draw_text(font, al_map_rgb(0, 0, 0), this->x1 + this->CARD_W - (this->CARD_BORDER_WIDTH * 2) - 10, this->y1 + (this->CARD_H / 2), ALLEGRO_ALIGN_LEFT, std::to_string(right).c_str());

	// Draw up number
	al_draw_filled_rectangle(this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2) - 5, this->y1 + (this->CARD_BORDER_WIDTH * 2) - 5, this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2) + 15, this->y1 + (this->CARD_BORDER_WIDTH * 2) + 15, this->gamedisplay->get_background_color());
	al_draw_text(font, al_map_rgb(0, 0, 0), this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2), this->y1 + (this->CARD_BORDER_WIDTH * 2), ALLEGRO_ALIGN_LEFT, std::to_string(up).c_str());

	// Draw down number
	al_draw_filled_rectangle(this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2) - 5, this->y1 + this->CARD_H - (this->CARD_BORDER_WIDTH * 2) - 15, this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2) + 15, this->y1 + this->CARD_H - (this->CARD_BORDER_WIDTH * 2) + 5, this->gamedisplay->get_background_color());
	al_draw_text(font, al_map_rgb(0, 0, 0), this->x1 + (this->CARD_W / 2) - (this->CARD_BORDER_WIDTH * 2), this->y1 + this->CARD_H - (this->CARD_BORDER_WIDTH * 2) - 10, ALLEGRO_ALIGN_LEFT, std::to_string(down).c_str());
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
	this->CARD_COLOR = color;
}

void Card::set_font(ALLEGRO_FONT *font)
{
	this->font = font;
}

void Card::set_gamedisplay(GameDisplay *gamedisplay)
{
	this->gamedisplay = gamedisplay;
}

int Card::get_up()
{
	return this->up;
}

int Card::get_down()
{
	return this->down;
}

int Card::get_left()
{
	return this->left;
}

int Card::get_right()
{
	return this->right;
}

bool Card::compare_to_right(Card *othercard)
{
	return this->right > othercard->get_left();
}

bool Card::compare_to_left(Card *othercard)
{
	return this->left > othercard->get_right();
}

bool Card::compare_to_up(Card *othercard)
{
	return this->up > othercard->get_down();
}

bool Card::compare_to_down(Card *othercard)
{
	return this->down > othercard->get_up();
}
