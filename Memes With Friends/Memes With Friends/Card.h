#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <vector>
#include <memory>
#include "GameDisplay.h"

class Card
{
private:
	int x1;
	int y1;
	int x2;
	int y2;
	int up;
	int down;
	int left;
	int right;
	ALLEGRO_COLOR CARD_COLOR;
	ALLEGRO_COLOR border_color;
	std::shared_ptr<ALLEGRO_BITMAP> meme_image;
	std::shared_ptr<ALLEGRO_FONT> font;
	GameDisplay *gamedisplay;
public:
	const static int CARD_W = 166;
	const static int CARD_H = 266;
	const static int CARD_BORDER_WIDTH = 5;
	Card(std::string meme, std::vector<int> numbers);
	void draw();
	void set_pos(int x1, int y1);
	void set_color(ALLEGRO_COLOR color);
	void set_font(std::shared_ptr<ALLEGRO_FONT> font);
	void set_gamedisplay(GameDisplay *gamedisplay);
	int get_up();
	int get_down();
	int get_left();
	int get_right();
	bool compare_to_right(Card &othercard); // compares this card's right value to othercard's left value
	bool compare_to_left(Card &othercard); // compares this card's left value to othercard's right value
	bool compare_to_up(Card &othercard); // compares this card's up value to othercard's down value
	bool compare_to_down(Card &othercard); // compares this card's down value to othercard's up value
};
