#include "CardFactory.h"
#include <string>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_physfs.h>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include "Card.h"

CardFactory::CardFactory()
{
	std::cout << "Searching for memes..." << std::endl;
	ALLEGRO_FS_ENTRY *dir = al_create_fs_entry("memes");
	this->populate_memes(dir);
	al_destroy_fs_entry(dir);

	std::cout << "Listing meme list contents..." << std::endl;

	for (auto& x : meme_list)
		std::cout << "Meme ['" << x.first << "'] numbers [" << x.second.at(0) << ", " << x.second.at(1) << ", " << x.second.at(2) << ", " << x.second.at(3) << "]" << std::endl;
}

CardFactory::~CardFactory()
{
}

void CardFactory::populate_memes(ALLEGRO_FS_ENTRY *dir)
{
	ALLEGRO_FS_ENTRY *file;

	al_open_directory(dir);
	while (1) {
		file = al_read_directory(dir);
		if (!file) {
			std::cout << "No more memes found" << std::endl;
			break;
		}
		if (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR) {
			std::cout << "Skipping directory" << std::endl;
			continue;
		}

		std::cout << "Found new meme '" << al_get_fs_entry_name(file) << "'" << std::endl;

		std::cout << "Initializing meme card numbers..." << std::endl;

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dist(1, 10000);
		srand(dist(mt));

		std::vector<int> numbers;
		numbers.push_back((rand() % 9) + 1);
		numbers.push_back((rand() % 9) + 1);
		numbers.push_back((rand() % 9) + 1);
		numbers.push_back((rand() % 9) + 1);

		std::string name = al_get_fs_entry_name(file);

		meme_names.push_back(name);
		this->meme_list[name] = numbers;
		al_destroy_fs_entry(file);
	}
	al_close_directory(dir);
}

std::shared_ptr<Card> CardFactory::create_card()
{
	std::string meme = choose_meme();
	std::vector<int> numbers = meme_list[meme];
	std::shared_ptr<Card> card = std::make_shared<Card>(Card(meme, numbers));
	return card;
}

std::string CardFactory::choose_meme() {
	ALLEGRO_BITMAP *temp;

	std::cout << "Shuffling meme_list..." << std::endl;
	std::random_shuffle(meme_names.begin(), meme_names.end());

	std::cout << "Trying meme '" << meme_names.front() << "'" << std::endl;
	temp = al_load_bitmap(meme_names.front().c_str());

	if (!temp) {
		std::cout << "Meme '" << meme_names.front() << "' unable to load. Trying to find another one..." << std::endl;
		return this->choose_meme();
	}
	else {
		std::cout << "Chose meme '" << meme_names.front() << "'" << std::endl;

		al_destroy_bitmap(temp);
		return meme_names.front();
	}
}
