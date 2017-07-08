#pragma once
#include <string>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_physfs.h>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include "Card.h"

class CardFactory
{
private:
	std::vector<std::string> meme_names;
	std::map<std::string, std::vector<int>> meme_list;
public:
	CardFactory();
	~CardFactory();
	void populate_memes(ALLEGRO_FS_ENTRY *dir);
	std::shared_ptr<Card> create_card();
	std::string choose_meme();
};
