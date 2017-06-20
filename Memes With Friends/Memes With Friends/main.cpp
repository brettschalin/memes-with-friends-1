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
#include <vector>
#include <memory>

#include "Card.h"
#include "GameDisplay.h"
#include "PlayerHand.h"
#include "Player1Hand.h"
#include "Player2Hand.h"
#include "GameManager.h"
#include "CardFactory.h"

#ifndef DATADIR
#define DATADIR "./"
#endif

const float FPS = 60;

/*
 * Makeshift scope guard to make sure PHYSFS_deinit happens AFTER all our fonts
 * get destroyed. Might be good to improve this eventually.
 */
struct PhysFS {
	PhysFS() { PHYSFS_init(NULL); }
	~PhysFS() { PHYSFS_deinit(); }
};

static int init_error(std::string message)
{
	std::cerr << "failed to initialize " << message << "\n";
	return EXIT_FAILURE;
}

int main(void)
{
	if (!al_init()) return init_error("allegro");
	al_init_font_addon();
	al_init_ttf_addon();
	if (!al_install_keyboard()) return init_error("keyboard");
	if (!al_install_mouse()) return init_error("mouse");

	std::unique_ptr<ALLEGRO_TIMER, decltype(&al_destroy_timer)> timer{al_create_timer(1.0 / FPS), &al_destroy_timer};
	if (timer == nullptr) return init_error("timer");

	GameDisplay gamedisplay;
	if (!gamedisplay.valid_display()) return init_error("display");
	if (!al_init_primitives_addon()) return init_error("primitives addon");
	if (!al_init_image_addon()) return init_error("image addon");

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

	PhysFS physfs_guard;
	std::cout << "loading assets from: " << datadir << "\n";
	if (!PHYSFS_mount(datadir.c_str(), "/", 1)) return init_error("assets");

	al_set_physfs_file_interface();

	std::shared_ptr<ALLEGRO_FONT> font{
		al_load_ttf_font("pirulen.ttf", gamedisplay.get_font_size(), 0),
		&al_destroy_font
	};
	if (!font) return init_error("font (pirulen.ttf)");

	std::unique_ptr<ALLEGRO_EVENT_QUEUE, decltype(&al_destroy_event_queue)> event_queue{
		al_create_event_queue(),
		&al_destroy_event_queue
	};
	if (!event_queue) return init_error("event_queue");

	al_register_event_source(event_queue.get(), al_get_display_event_source(gamedisplay.get_display()));

	al_register_event_source(event_queue.get(), al_get_timer_event_source(timer.get()));

	al_register_event_source(event_queue.get(), al_get_keyboard_event_source());

	al_register_event_source(event_queue.get(), al_get_mouse_event_source());

	gamedisplay.set_background_color(al_map_rgb(241, 241, 212));
	
	al_start_timer(timer.get());

	CardFactory card_factory;

	Player1Hand p1hand{font, &gamedisplay, card_factory};
	Player2Hand p2hand{font, &gamedisplay, card_factory};

	/* Test cards 1 and 2 are only for number testing at this time and is not displayed on screen. Will be removed shortly */

	std::unique_ptr<Card> test_card{card_factory.create_card()};
	test_card->set_font(font);
	test_card->set_gamedisplay(&gamedisplay);
	test_card->set_color(al_map_rgb(255, 0, 0));
	test_card->set_pos(50, 50);

	std::unique_ptr<Card> test_card2{card_factory.create_card()};
	test_card2->set_font(font);
	test_card2->set_gamedisplay(&gamedisplay);
	test_card2->set_color(al_map_rgb(255, 0, 0));
	test_card2->set_pos(50, 50);

	bool doexit = false;
	bool debug = false;

	int mouse_x = 0, mouse_y = 0;
	int sx = 0, sy = 0;

	//Testing and example usage for the GameManager class
	GameManager game = GameManager();
	int currentPlayer = game.get_current_player();
	game.play_card(game.draw_card_from_hand(3), 0, 0);
	int testdown = (*game.get_card(0, 0)).get_down();
	game.set_current_player(!currentPlayer);
	game.play_card(game.draw_card_from_hand(2), 0, 1);
	int testup = (*game.get_card(0, 1)).get_up();
	if (!((testdown > testup) == (*game.get_card(0,0)).compare_to_down(game.get_card(0, 1))))
	{
		std::cout << "Either card comparisons or card placement is broken." << std::endl;
	}

	int player1Score = game.get_score(0);
	int player2Score = game.get_score(1);
	std::cout << "Scores: " << player1Score << ", " << player2Score << std::endl;

	delete &game;








	bool redraw = true;

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue.get(), &ev);

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

		if (redraw && al_is_event_queue_empty(event_queue.get())) {

			redraw = false;

			gamedisplay.clear_display();

			p1hand.draw();
			p2hand.draw();

			if (debug) {
				// if debug is toggled on, draw debug information above everything else
				std::tie(sx, sy) = gamedisplay.convert_coordinates(mouse_x, mouse_y);
				std::string mouse_pos_x = "Mouse X: " + std::to_string(sx) + "    Card 1 D: " + std::to_string(test_card->get_down()) + "    Card 1 turns Card 2: " + (test_card->compare_to_down(*test_card2.get()) ? "True" : "False");
				std::string mouse_pos_y = "Mouse Y: " + std::to_string(sy) + "    Card 2 U: " + std::to_string(test_card2->get_up());
				std::string mouse_pos = mouse_pos_x + "\n" + mouse_pos_y;
				al_draw_multiline_text(font.get(), al_map_rgb(0, 0, 0), 10, 10, 700, 0, ALLEGRO_ALIGN_LEFT, mouse_pos.c_str());
			}

			// draw help text
			al_draw_multiline_text(font.get(), al_map_rgb(0, 0, 0), 10, 1040, 500, 0, ALLEGRO_ALIGN_LEFT, "Press D to toggle DEBUG info\nPress ESC to exit");

			al_flip_display();

		}
	}

	return 0;
}
