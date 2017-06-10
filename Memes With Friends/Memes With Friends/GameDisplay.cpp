#include "GameDisplay.h"
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <tuple>

GameDisplay::GameDisplay()
{
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	display = al_create_display(disp_data.width, disp_data.height);

	int windowWidth = al_get_display_width(display);
	int windowHeight = al_get_display_height(display);
	int screenWidth = SCREEN_W;
	int screenHeight = SCREEN_H;

	float sx = windowWidth / (float)screenWidth;
	float sy = windowHeight / (float)screenHeight;

	if ((sx < (float)0.0) || (sy < (float)0.0)) {
		fontsize = 16;
	} else {
		fontsize = 12;
	}

	ALLEGRO_TRANSFORM trans;
	al_identity_transform(&trans);
	al_scale_transform(&trans, sx, sy);
	al_use_transform(&trans);
}

GameDisplay::~GameDisplay()
{
	al_destroy_display(display);
}

bool GameDisplay::valid_display()
{
	if (this->display) return true;
	return false;
}

int GameDisplay::get_font_size()
{
	return this->fontsize;
}

ALLEGRO_DISPLAY * GameDisplay::get_display()
{
	return this->display;
}

void GameDisplay::set_background_color(ALLEGRO_COLOR color)
{
	this->BACKGROUND_COLOR = color;
}

ALLEGRO_COLOR GameDisplay::get_background_color()
{
	return this->BACKGROUND_COLOR;
}

void GameDisplay::clear_display()
{
	al_clear_to_color(this->BACKGROUND_COLOR);
}

std::tuple<int, int> GameDisplay::convert_coordinates(int x, int y)
{
	int windowWidth = al_get_display_width(this->display);
	int windowHeight = al_get_display_height(this->display);
	int screenWidth = this->SCREEN_W;
	int screenHeight = this->SCREEN_H;

	float sx = windowWidth / (float)screenWidth;
	float sy = windowHeight / (float)screenHeight;

	// HIGHER RESOLUTION
	// sx = 3840(S) / 1920(W) = 2
	// x = 3840(S) / 2(sx) = 1920
	// x = 1920(S) / 2(sx) = 960

	// LOWER RESOLUTION
	// sx = 1024(S) / 1920(W) = 0.533333333333333333
	// x = 1024(S) / 0.533333333333333333(sx) = 1920
	// x = 512(S) / 0.533333333333333333(sx) = 960

	return std::make_tuple(x / sx, y / sy);
}
