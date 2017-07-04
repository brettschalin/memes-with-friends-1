#pragma once

#include "State.h"
#include "GameDisplay.h"
#include <allegro5/allegro_font.h>
#include <memory>
#include "Player1Hand.h"
#include "Player2Hand.h"
#include "CardFactory.h"

class GameState :
    public State {
private:
    bool debug;
    bool initialized = false;
    int mousex;
    int mousey;
    std::shared_ptr<CardFactory> card_factory;
    std::shared_ptr<Player1Hand> p1hand;
    std::shared_ptr<Player2Hand> p2hand;
    std::unique_ptr<Card> test_card;
    std::unique_ptr<Card> test_card2;
    std::shared_ptr<ALLEGRO_FONT> font;
public:
    virtual void enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) final;
    virtual PROCESS_CODE process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) final;
    virtual void draw(GameDisplay *gamedisplay) final;
};
