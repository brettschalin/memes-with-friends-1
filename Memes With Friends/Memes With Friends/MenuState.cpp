#include "MenuState.h"
#include "GameDisplay.h"
#include "MenuButton.h"
#include "Point.h"
#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

MenuState::MenuState() {
    playbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) - 200, 400, 100);
    playbutton->set_text("Play");

    creditsbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) - 50, 400, 100);
    creditsbutton->set_text("Credits");

    quitbutton = std::make_unique<MenuButton>((GameDisplay::SCREEN_W / 2) - 200, (GameDisplay::SCREEN_H / 2) + 100, 400, 100);
    quitbutton->set_text("Quit");
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
                if (playbutton->contains(mouse)) {
                    std::cout << "Play clicked" << std::endl;
                } else if (creditsbutton->contains(mouse)) {
                    std::cout << "Credits clicked" << std::endl;
                } else if (quitbutton->contains(mouse)) {
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

void MenuState::draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    playbutton->draw(font, gamedisplay);
    creditsbutton->draw(font, gamedisplay);
    quitbutton->draw(font, gamedisplay);
}
