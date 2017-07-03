#include "MenuState.h"
#include "GameDisplay.h"
#include "MenuButton.h"
#include "Point.h"
#include "main.h"
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void MenuState::enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    if (initialized) return;

    playbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) - 200, 400, 100);
    playbutton->set_text("Play");

    creditsbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) - 50, 400, 100);
    creditsbutton->set_text("Credits");

    quitbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) + 100, 400, 100);
    quitbutton->set_text("Quit");

    set_pause(false);

    this->font = font;
    initialized = true;
}

PROCESS_CODE MenuState::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
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
                if (playbutton->is_enabled() && playbutton->contains(mouse)) {
                    if (!pause) std::cout << "Play clicked" << std::endl;
                    else std::cout << "Resume clicked" << std::endl;
                    switchstate(GAMESTATE::GAMESTATE);
                } else if (creditsbutton->is_enabled() && creditsbutton->contains(mouse)) {
                    std::cout << "Credits clicked" << std::endl;
                    switchstate(GAMESTATE::CREDITSSTATE);
                } else if (quitbutton->is_enabled() && quitbutton->contains(mouse)) {
                    std::cout << "Quit clicked" << std::endl;
                    return PROCESS_CODE::QUIT;
                }
                break;
        }
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        playbutton->mouseover(mouse);
        creditsbutton->mouseover(mouse);
        quitbutton->mouseover(mouse);
    }

    return PROCESS_CODE::OK;
}

void MenuState::draw(GameDisplay *gamedisplay) {
    al_draw_text(font.get(), al_map_rgb(0, 0, 0), GameDisplay::SCREEN_W / 2, playbutton->get_y1() - 100, ALLEGRO_ALIGN_CENTER, "MEMES WITH FRIENDS");
    playbutton->draw(this->font, gamedisplay);
    creditsbutton->draw(this->font, gamedisplay);
    quitbutton->draw(this->font, gamedisplay);
}

void MenuState::set_pause(bool pause) {
    this->pause = pause;
    if (pause) playbutton->set_text("Resume");
    else playbutton->set_text("Play");
}
