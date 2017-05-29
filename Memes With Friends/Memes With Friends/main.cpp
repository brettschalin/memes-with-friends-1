#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>
#include "Card.h"
#include "GameDisplay.h"

const float FPS = 60;

int main(int argc, char **argv)
{
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	bool doexit = false;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	GameDisplay *gamedisplay = new GameDisplay();

	if (!gamedisplay->valid_display()) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		al_destroy_timer(timer);
		return -1;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "failed to initialize image addon!\n");
		al_destroy_timer(timer);
		return -1;
	}

	PHYSFS_init(NULL);
	if (!PHYSFS_mount("assets.zip", "/", 1)) {
		fprintf(stderr, "failed to open assets.zip file!\n");
		PHYSFS_deinit();
		al_shutdown_primitives_addon();
		al_shutdown_image_addon();
		al_destroy_timer(timer);
	}

	al_set_physfs_file_interface();

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		PHYSFS_deinit();
		al_shutdown_primitives_addon();
		al_shutdown_image_addon();
		al_destroy_timer(timer);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(gamedisplay->get_display()));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	gamedisplay->set_background_color(al_map_rgb(241, 241, 212));
	
	al_start_timer(timer);

	Card *test_card = new Card();
	test_card->set_color(al_map_rgb(255, 0, 0));
	test_card->set_pos(50, 50);

	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		gamedisplay->clear_display();

		test_card->draw();

		al_flip_display();
	}

	PHYSFS_deinit();
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	return 0;
}