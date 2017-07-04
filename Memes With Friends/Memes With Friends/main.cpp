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
#include <stack>

#include "Card.h"
#include "GameDisplay.h"
#include "PlayerHand.h"
#include "ComputerHand.h"
#include "PlayerHand.h"
#include "GameManager.h"
#include "CardFactory.h"
#include "State.h"
#include "GameState.h"
#include "MenuState.h"
#include "CreditsState.h"
#include "main.h"

#ifndef DATADIR
#define DATADIR "./"
#endif

const float FPS = 60;

std::stack<std::shared_ptr<State>> statemachine;
GAMESTATE gamestate;
GameDisplay *g_gamedisplay;

static int init_error(std::string message) {
    std::cerr << "failed to initialize " << message << "\n";
    return EXIT_FAILURE;
}

int switchstate(GAMESTATE newstate) {

    int fontsize = g_gamedisplay->get_font_size();
    if (newstate != GAMESTATE::GAMESTATE) fontsize = 48;
    std::shared_ptr<ALLEGRO_FONT> font{
        al_load_ttf_font("pirulen.ttf", fontsize, 0),
        &al_destroy_font
    };
    if (!font) return init_error("font (pirulen.ttf)");

    switch (newstate) {
        case GAMESTATE::MENUSTATE:
        {
            if (gamestate == GAMESTATE::CREDITSSTATE) statemachine.pop();
            else {
                auto ms = std::make_shared<MenuState>();
                ms->enter(font, g_gamedisplay);
                statemachine.push(ms);
            }
        }
            break;
        case GAMESTATE::GAMESTATE:
        {
            if (gamestate == GAMESTATE::PAUSESTATE) statemachine.pop();
            else {
                auto gs = std::make_shared<GameState>();
                gs->enter(font, g_gamedisplay);
                statemachine.push(gs);
            }
        }
            break;
        case GAMESTATE::PAUSESTATE:
        {
            auto ps = std::make_shared<MenuState>();
            ps->enter(font, g_gamedisplay);
            ((MenuState *)ps.get())->set_pause(true);
            statemachine.push(ps);
        }
            break;
        case GAMESTATE::CREDITSSTATE:
        {
            auto cs = std::make_shared<CreditsState>();
            cs->enter(font, g_gamedisplay);
            statemachine.push(cs);
        }
            break;
    }

    gamestate = newstate;

    return 0;
}

/*
 * Makeshift scope guard to make sure PHYSFS_deinit happens AFTER all our fonts
 * get destroyed. Might be good to improve this eventually.
 */
struct PhysFS {
	PhysFS() { PHYSFS_init(NULL); }
	~PhysFS() { PHYSFS_deinit(); }
};

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
    g_gamedisplay = &gamedisplay;

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

    std::shared_ptr<ALLEGRO_FONT> menufont{
        al_load_ttf_font("pirulen.ttf", 48, 0),
        &al_destroy_font
    };
    if (!menufont) return init_error("font (pirulen.ttf)");

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

	bool doexit = false;

    switchstate(GAMESTATE::MENUSTATE);

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

        PROCESS_CODE pcode = statemachine.top()->process(ev, &gamedisplay);

        if (pcode == PROCESS_CODE::QUIT) {
            doexit = true;
            break;
        }

		if (redraw && al_is_event_queue_empty(event_queue.get())) {

			redraw = false;

			gamedisplay.clear_display();

            statemachine.top()->draw(&gamedisplay);

			al_flip_display();

		}
	}

    while (statemachine.size() > 0) {
        statemachine.pop();
    }

	return 0;
}
