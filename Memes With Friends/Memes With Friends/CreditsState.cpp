#include "CreditsState.h"
#include "State.h"
#include "main.h"
#include "MenuButton.h"
#include <allegro5/allegro_font.h>
#include <iostream>
#include <string>
#include <memory>

void CreditsState::enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    if (initialized) return;

    credits.append("Programmers:\n\n");
    credits.append("/u/CSTutor\n");
    credits.append("/u/the1spaceman\n");
    credits.append("/u/EliteTK\n\n");
    credits.append("Memes:\n\n");
    credits.append("/u/xgfdgfbdbgcxnhgc\n");
    credits.append("TBD\n");
    credits.append("TBD\n");
    credits.append("TBD\n\n");
    credits.append("Testing:\n\n");
    credits.append("/u/Killa-Byte\n");

    backbutton = std::make_shared<MenuButton>(50, GameDisplay::SCREEN_H - 150, 400, 100);
    backbutton->set_text("<- Back");

    this->font = font;
    initialized = true;
}

PROCESS_CODE CreditsState::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    int mouse_x = 0, mouse_y = 0;
    int sx = 0, sy = 0;

    mouse_x = ev.mouse.x;
    mouse_y = ev.mouse.y;
    std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);
    Point mouse(sx, sy);

    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        switch (ev.mouse.button) {
            case 1:
                // left button
                if (backbutton->is_enabled() && backbutton->contains(mouse)) {
                    std::cout << "Back button clicked" << std::endl;
                    switchstate(GAMESTATE::MENUSTATE);
                }
                break;
        }
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        backbutton->mouseover(mouse);
    }

    return PROCESS_CODE::OK;
}

void CreditsState::draw(GameDisplay *gamedisplay) {
    al_draw_multiline_text(this->font.get(), al_map_rgb(0, 0, 0), GameDisplay::SCREEN_W / 2, 50, GameDisplay::SCREEN_W, 0, ALLEGRO_ALIGN_CENTER, credits.c_str());
    backbutton->draw(this->font, gamedisplay);
}
