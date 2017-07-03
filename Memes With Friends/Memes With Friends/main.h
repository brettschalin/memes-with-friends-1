#pragma once

#include <memory>
#include <string>

enum class GAMESTATE {
    MENUSTATE,
    PAUSESTATE,
    CREDITSSTATE,
    GAMESTATE,
};

void switchstate(GAMESTATE newstate);
static int init_error(std::string message);
int main(void);
