#pragma once

#include "State.h"
#include "GameDisplay.h"
#include <allegro5/allegro_font.h>
#include <memory>
#include "GameManager.h"

class GameState :
    public State {
private:
    bool debug;
    bool initialized = false;
    int mousex;
    int mousey;
    std::shared_ptr<GameManager> gamemanager;
    std::shared_ptr<ALLEGRO_FONT> font;
public:
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(GameDisplay *gamedisplay) final;
};
