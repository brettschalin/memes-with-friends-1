#include "GameDisplay.h"
#include <algorithm>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

GameDisplay::GameDisplay()
{
	al_get_display_mode(al_get_num_display_modes() - 1, &this->disp_data);

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(this->disp_data.width, this->disp_data.height);

	int windowWidth = al_get_display_width(display);
	int windowHeight = al_get_display_height(display);
	int screenWidth = this->SCREEN_W;
	int screenHeight = this->SCREEN_H;

	float sx = windowWidth / (float)screenWidth;
	float sy = windowHeight / (float)screenHeight;

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
