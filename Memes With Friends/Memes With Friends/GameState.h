#pragma once

#include "State.h"

class GameState :
    public State {
public:
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
};
