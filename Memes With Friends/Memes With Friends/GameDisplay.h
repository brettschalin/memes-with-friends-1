#pragma once

#include <allegro5/allegro.h>
#include <tuple>

class GameDisplay
{
private:
	int fontsize = 12;
	ALLEGRO_COLOR BACKGROUND_COLOR;
	ALLEGRO_DISPLAY_MODE disp_data;
	ALLEGRO_DISPLAY *display = NULL;
public:
	GameDisplay();
	// This class can't be copied
	GameDisplay(const GameDisplay &) = delete;
	GameDisplay &operator=(const GameDisplay &) = delete;
	~GameDisplay();
	bool valid_display();
	int get_font_size();
	ALLEGRO_DISPLAY *get_display();
	void set_background_color(ALLEGRO_COLOR color);
	ALLEGRO_COLOR get_background_color();
	void clear_display();
	std::tuple<int, int> convert_coordinates(int x, int y);
	const static int SCREEN_W = 1920;
	const static int SCREEN_H = 1080;
};

