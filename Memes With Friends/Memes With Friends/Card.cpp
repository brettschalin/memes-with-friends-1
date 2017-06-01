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

Card::Card()
{
	srand((int)time(0));

	fprintf(stdout, "Initializing new card...\n");
	ALLEGRO_FS_ENTRY *dir = al_create_fs_entry("memes");
	std::vector<const char *> meme_list = this->list_memes(dir);

	fprintf(stdout, "Listing meme list contents...\n");
	for (std::vector<const char *>::const_iterator i = meme_list.begin(); i != meme_list.end(); ++i)
		fprintf(stdout, "%s\n", *i);

	this->choose_meme(meme_list);

	this->up = (rand() % 9) + 1;
	this->down = (rand() % 9) + 1;
	this->left = (rand() % 9) + 1;
	this->right = (rand() % 9) + 1;
}

Card::~Card()
{
	al_destroy_bitmap(this->meme_image);
}

/* Wrote this when I was tired to try to prevent issues where a bad image or non image gets put in the memes directory. Not sure if works but seems to so far. */
const void Card::choose_meme(std::vector<const char *>& meme_list) {
	fprintf(stdout, "Shuffling meme_list...\n");
	std::random_shuffle(meme_list.begin(), meme_list.end());

	fprintf(stdout, "Trying meme '%s'...\n", meme_list.front());
	this->meme_image = al_load_bitmap(meme_list.front());

	if (!this->meme_image) {
		fprintf(stdout, "Meme '%s' failed to load...\n", meme_list.front());

		this->choose_meme(meme_list);
	}
	else {
		fprintf(stdout, "Chose meme '%s'...\n", meme_list.front());
		return;
	}
}

std::vector<const char *> Card::list_memes(ALLEGRO_FS_ENTRY *dir)
{
	std::vector<const char *> memes;

	ALLEGRO_FS_ENTRY *file;

	al_open_directory(dir);
	while (1) {
		file = al_read_directory(dir);
		if (!file) {
			fprintf(stdout, "Invalid file/directory or no more files left. Breaking...\n");
			break;
		}
		if (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR) {
			fprintf(stdout, "Skipping directory\n");
			continue;
		}

		fprintf(stdout, "Assigning '%s' to memes list\n", al_get_fs_entry_name(file));

		char *filename = _strdup(al_get_fs_entry_name(file));
		memes.push_back(filename);
		al_destroy_fs_entry(file);
	}
	al_close_directory(dir);

	return memes;
}

const void Card::draw()
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

const void Card::set_font(ALLEGRO_FONT *font)
{
	this->font = font;
}

const void Card::set_gamedisplay(GameDisplay *gamedisplay)
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
