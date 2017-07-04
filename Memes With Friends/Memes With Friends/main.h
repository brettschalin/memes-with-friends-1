#pragma once

#include <memory>
#include <string>

enum class GAMESTATE {
    MENUSTATE,
    PAUSESTATE,
    CREDITSSTATE,
    GAMESTATE,
};

int switchstate(GAMESTATE newstate);
int main(void);
