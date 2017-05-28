#include "Card.h"
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

Card::Card()
{
	srand((int)time(0));
	int r = (rand() % 2) + 1;

	switch (r) {
	case 1:
		this->meme_image = al_load_bitmap("picard-facepalm.jpg");
		break;
	case 2:
		this->meme_image = al_load_bitmap("pepe.jpg");
		break;
	}
}


Card::~Card()
{
	al_destroy_bitmap(this->meme_image);
}

const void Card::draw()
{
	// Draw the meme image first at the appropriate location
	al_draw_scaled_bitmap(this->meme_image, 0, 0, al_get_bitmap_width(this->meme_image), al_get_bitmap_height(this->meme_image), this->x1, this->y1, Card::CARD_W, Card::CARD_H, 0);

	// Draw the border around the meme image to make it a card
	al_draw_rectangle(this->x1, this->y1, this->x2, this->y2, this->CARD_COLOR, Card::CARD_BORDER_WIDTH);

	// TODO: Draw the four directional numbers indicating attack power of this card
}

const void Card::set_pos(int x1, int y1)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x1 + Card::CARD_W;
	this->y2 = y1 + Card::CARD_H;
}

const void Card::set_color(ALLEGRO_COLOR color)
{
	this->CARD_COLOR = color;
}
