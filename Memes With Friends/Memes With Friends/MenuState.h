#pragma once

#include "State.h"
#include "MenuButton.h"
#include "Point.h"
#include <memory>

class MenuState :
    public State {
private:
    std::shared_ptr<MenuButton> playbutton;
    std::shared_ptr<MenuButton> creditsbutton;
    std::shared_ptr<MenuButton> quitbutton;
    std::shared_ptr<ALLEGRO_FONT> font;
    bool initialized = false;
    bool pause;
public:
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(GameDisplay *gamedisplay) final;
    void set_pause(bool pause);
};
