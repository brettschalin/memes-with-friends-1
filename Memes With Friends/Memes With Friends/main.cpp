#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <physfs.h>
#include <string>
#include "Card.h"
#include "GameDisplay.h"
#include "PlayerHand.h"
#include "Player1Hand.h"
#include "Player2Hand.h"

const float FPS = 60;

int main(void)
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	bool doexit = false;
	bool debug = false;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		al_uninstall_system();
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize the mouse!\n");
		al_uninstall_system();
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		al_uninstall_system();
		return -1;
	}

	GameDisplay *gamedisplay = new GameDisplay();

	if (!gamedisplay->valid_display()) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	PHYSFS_init(NULL);
	if (!PHYSFS_mount("assets.zip", "/", 1)) {
		fprintf(stderr, "failed to open assets.zip file!\n");
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_uninstall_system();
	}

	al_set_physfs_file_interface();

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf", gamedisplay->get_font_size(), 0);
	if (!font) {
		fprintf(stderr, "failed to load pirulen.ttf from assets.zip!\n");
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_uninstall_system();
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		PHYSFS_deinit();
		al_destroy_timer(timer);
		al_destroy_font(font);
		al_uninstall_system();
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(gamedisplay->get_display()));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_mouse_event_source());

	gamedisplay->set_background_color(al_map_rgb(241, 241, 212));
	
	al_start_timer(timer);

	Player1Hand *p1hand = new Player1Hand(font, gamedisplay);
	Player2Hand *p2hand = new Player2Hand(font, gamedisplay);

	/* Test cards 1 and 2 are only for number testing at this time and is not displayed on screen. Will be removed shortly */

	Card *test_card = new Card();
	test_card->set_font(font);
	test_card->set_gamedisplay(gamedisplay);
	test_card->set_color(al_map_rgb(255, 0, 0));
	test_card->set_pos(50, 50);

	Card *test_card2 = new Card();
	test_card2->set_font(font);
	test_card2->set_gamedisplay(gamedisplay);
	test_card2->set_color(al_map_rgb(255, 0, 0));
	test_card2->set_pos(50, 50);

	int mouse_x, mouse_y = 0;
	int sx, sy = 0;

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

			gamedisplay->clear_display();

			p1hand->draw();
			p2hand->draw();

			if (debug) {
				// if debug is toggled on, draw debug information above everything else
				std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);
				std::string mouse_pos_x = "Mouse X: " + std::to_string(sx) + "    Card 1 D: " + std::to_string(test_card->get_down()) + "    Card 1 turns Card 2: " + (test_card->compare_to_down(test_card2) ? "True" : "False");
				std::string mouse_pos_y = "Mouse Y: " + std::to_string(sy) + "    Card 2 U: " + std::to_string(test_card2->get_up());
				std::string mouse_pos = mouse_pos_x + "\n" + mouse_pos_y;
				al_draw_multiline_text(font, al_map_rgb(0, 0, 0), 10, 10, 700, 0, ALLEGRO_ALIGN_LEFT, mouse_pos.c_str());
			}

			// draw help text
			al_draw_multiline_text(font, al_map_rgb(0, 0, 0), 10, 1040, 500, 0, ALLEGRO_ALIGN_LEFT, "Press D to toggle DEBUG info\nPress ESC to exit");

			al_flip_display();

		}
	}

	PHYSFS_deinit();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_uninstall_system();

	return 0;
}