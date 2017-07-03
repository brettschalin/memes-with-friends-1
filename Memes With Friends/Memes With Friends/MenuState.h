#pragma once

#include "State.h"
#include "MenuButton.h"
#include "Point.h"
#include <memory>

class MenuState :
    public State {
private:
    std::unique_ptr<MenuButton> playbutton;
    std::unique_ptr<MenuButton> creditsbutton;
    std::unique_ptr<MenuButton> quitbutton;
    bool pause;
public:
    MenuState();
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    void set_pause(bool pause);
};
