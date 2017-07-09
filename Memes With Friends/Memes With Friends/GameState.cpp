#include "GameState.h"
#include "GameDisplay.h"
#include <allegro5/allegro_font.h>
#include "main.h"
#include "GameManager.h"

void GameState::enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    if (initialized) return;

    debug = false;

    this->font = font;

    gamemanager = std::make_shared<GameManager>(font, gamedisplay);

    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    mousex = state.x;
    mousey = state.y;

    initialized = true;
}

PROCESS_CODE GameState::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    int mouse_x = 0, mouse_y = 0;
    int sx = 0, sy = 0;

    mouse_x = ev.mouse.x;
    mouse_y = ev.mouse.y;
    std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);

    if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_D:
                debug = !debug; // toggle debug state
                break;
            case ALLEGRO_KEY_ESCAPE:
                // return to menustate
                if (!gamemanager->get_gameover()) switchstate(GAMESTATE::PAUSESTATE);
                else {
                    // exit game to prepare for new one
                    switchstate(GAMESTATE::MENUSTATE);
                    return PROCESS_CODE::OK;
                }
                break;
        }
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        mousex = sx;
        mousey = sy;
    }

    gamemanager->process(ev, gamedisplay);

    return PROCESS_CODE::OK;
}

void GameState::draw(GameDisplay *gamedisplay) {

    gamemanager->draw();

    if (debug) {
        // if debug is toggled on, draw debug information above everything else
        std::string mouse_pos = "Mouse: (" + std::to_string(mousex) + ", " + std::to_string(mousey) + ")";
        al_draw_multiline_text(this->font.get(), al_map_rgb(0, 0, 0), 10, 10, GameDisplay::SCREEN_W - 10, 0, ALLEGRO_ALIGN_LEFT, mouse_pos.c_str());
    }

    // draw help text
    al_draw_multiline_text(this->font.get(), al_map_rgb(0, 0, 0), 10, 1040, 500, 0, ALLEGRO_ALIGN_LEFT, "Press D to toggle DEBUG info\nPress ESC to return to menu");
}
