#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <physfs.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include "Card.h"
#include "GameDisplay.h"
#include "PlayerHand.h"
#include "Player1Hand.h"
#include "Player2Hand.h"
#include "CardFactory.h"

#ifndef DATADIR
#define DATADIR "./"
#endif

const float FPS = 60;

int main(void)
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	bool doexit = false;
	bool debug = false;

	if (!al_init()) {
		std::cerr << "failed to initialize allegro!" << std::endl;
		return -1;
	}
	
	al_init_font_addon();
	al_init_ttf_addon();

	if (!al_install_keyboard()) {
		std::cerr << "failed to initialize the keyboard!" << std::endl;
		al_uninstall_system();
		return -1;
	}

	if (!al_install_mouse()) {
		std::cerr << "failed to initialize the mouse!" << std::endl;
		al_uninstall_system();
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		std::cerr << "failed to create timer!" << std::endl;
		al_uninstall_system();
		return -1;
	}

	GameDisplay gamedisplay = GameDisplay();

	if (!gamedisplay.valid_display()) {
		std::cerr << "failed to create display!" << std::endl;
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	if (!al_init_primitives_addon()) {
		std::cerr << "failed to initialize primitives addon!" << std::endl;
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	if (!al_init_image_addon()) {
		std::cerr << "failed to initialize image addon!" << std::endl;
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	// std::string from NULL char * is undefined behavior and causes crashing
	// to resolve, we will try to grab the environment variable into datadir_raw
	// if it's NULL or blank, initialize with contents of DATADIR
	const char *datadir_raw = std::getenv("MWF_DATADIR");
	if (datadir_raw == NULL || std::strlen(datadir_raw) == 0) {
		datadir_raw = DATADIR;
	}

	// then make a std::string out of datadir_raw
	std::string datadir(datadir_raw);

	// attempt to open assets.zip file at datadir's location
	// if it fails to load, mount the directory instead
	std::ifstream infile(datadir + "/assets.zip");
	if (infile.good()) {
		datadir += "/assets.zip";
	}

	PHYSFS_init(NULL);
	if (!PHYSFS_mount(datadir.c_str(), "/", 1)) {
		std::cerr << "failed to open " << datadir << "!" << std::endl;
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_uninstall_system();
	}

	al_set_physfs_file_interface();

	std::shared_ptr<ALLEGRO_FONT> font{
		al_load_ttf_font("pirulen.ttf", gamedisplay.get_font_size(), 0),
		&al_destroy_font
	};
	if (!font) {
		std::cerr << "failed to load pirulen.ttf from " << datadir << "!" << std::endl;
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		std::cerr << "failed to create event_queue!" << std::endl;
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(gamedisplay.get_display()));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_mouse_event_source());

	gamedisplay.set_background_color(al_map_rgb(241, 241, 212));
	
	al_start_timer(timer);

	CardFactory card_factory = CardFactory();

	Player1Hand p1hand = Player1Hand(font, &gamedisplay, &card_factory);
	Player2Hand p2hand = Player2Hand(font, &gamedisplay, &card_factory);

	/* Test cards 1 and 2 are only for number testing at this time and is not displayed on screen. Will be removed shortly */

	Card *test_card = card_factory.create_card();
	test_card->set_font(font);
	test_card->set_gamedisplay(&gamedisplay);
	test_card->set_color(al_map_rgb(255, 0, 0));
	test_card->set_pos(50, 50);

	Card *test_card2 = card_factory.create_card();
	test_card2->set_font(font);
	test_card2->set_gamedisplay(&gamedisplay);
	test_card2->set_color(al_map_rgb(255, 0, 0));
	test_card2->set_pos(50, 50);

	int mouse_x = 0, mouse_y = 0;
	int sx = 0, sy = 0;

	bool redraw = true;

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
			mouse_x = ev.mouse.x;
			mouse_y = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_D:
				debug = !debug; // toggle debug state
				break;
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {

			redraw = false;

			gamedisplay.clear_display();

			p1hand.draw();
			p2hand.draw();

			if (debug) {
				// if debug is toggled on, draw debug information above everything else
				std::tie(sx, sy) = gamedisplay.convert_coordinates(mouse_x, mouse_y);
				std::string mouse_pos_x = "Mouse X: " + std::to_string(sx) + "    Card 1 D: " + std::to_string(test_card->get_down()) + "    Card 1 turns Card 2: " + (test_card->compare_to_down(test_card2) ? "True" : "False");
				std::string mouse_pos_y = "Mouse Y: " + std::to_string(sy) + "    Card 2 U: " + std::to_string(test_card2->get_up());
				std::string mouse_pos = mouse_pos_x + "\n" + mouse_pos_y;
				al_draw_multiline_text(font.get(), al_map_rgb(0, 0, 0), 10, 10, 700, 0, ALLEGRO_ALIGN_LEFT, mouse_pos.c_str());
			}

			// draw help text
			al_draw_multiline_text(font.get(), al_map_rgb(0, 0, 0), 10, 1040, 500, 0, ALLEGRO_ALIGN_LEFT, "Press D to toggle DEBUG info\nPress ESC to exit");

			al_flip_display();

		}
	}

	delete test_card;
	delete test_card2;
	PHYSFS_deinit();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_uninstall_system();

	return 0;
}
