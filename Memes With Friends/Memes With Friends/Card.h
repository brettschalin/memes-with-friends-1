#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Card
{
private:
	int x1;
	int y1;
	int x2;
	int y2;
	ALLEGRO_COLOR CARD_COLOR;
	ALLEGRO_COLOR border_color;
	ALLEGRO_BITMAP *meme_image;
public:
	const static int CARD_W = 166;
	const static int CARD_H = 266;
	const static int CARD_BORDER_WIDTH = 5;
	Card();
	~Card();
	const void draw();
	const void set_pos(int x1, int y1);
	const void set_color(ALLEGRO_COLOR color);
};

