#include "GameState.h"
#include "main.h"

PROCESS_CODE GameState::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_D:
                //debug = !debug; // toggle debug state
                break;
            case ALLEGRO_KEY_ESCAPE:
                // return to menustate
                switchstate(GAMESTATE::PAUSESTATE);
                break;
        }
    }

    return PROCESS_CODE::OK;
}

void GameState::draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {}
