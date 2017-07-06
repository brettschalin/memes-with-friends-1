#pragma once
#pragma once

#include <memory>
#include <allegro5/allegro_font.h>
#include "GameDisplay.h"

enum class PROCESS_CODE {
    OK,
    QUIT,
};

class State {
protected:
    State() {};
public:
    virtual ~State() {};
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) = 0;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) = 0;
    virtual void draw(GameDisplay *gamedisplay) = 0;
};

