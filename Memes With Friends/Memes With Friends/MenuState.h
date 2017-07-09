#pragma once

#include "State.h"
#include "MenuButton.h"
#include "Point.h"
#include "enums.h"
#include "main.h"
#include <memory>

class MenuState :
    public State {
private:
    DIFFICULTY difficulty;
    std::shared_ptr<MenuButton> playbutton;
    std::shared_ptr<MenuButton> creditsbutton;
    std::shared_ptr<MenuButton> quitbutton;
    std::shared_ptr<MenuButton> difficultytoggle;
    std::shared_ptr<ALLEGRO_FONT> font;
    bool initialized = false;
    bool pause;
public:
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(GameDisplay *gamedisplay) final;
    void set_pause(bool pause);
    void change_difficulty(DIFFICULTY newdifficulty);
    DIFFICULTY get_difficulty();
};
