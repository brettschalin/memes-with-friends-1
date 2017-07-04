#pragma once

#include "State.h"
#include "MenuButton.h"
#include <allegro5/allegro_font.h>
#include <string>

class CreditsState :
    public State {
private:
    std::shared_ptr<MenuButton> backbutton;
    std::shared_ptr<ALLEGRO_FONT> font;
    std::string credits;
    bool initialized = false;
public:
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(GameDisplay *gamedisplay) final;
};
