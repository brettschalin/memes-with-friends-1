#pragma once

enum class PLAYER {
	COMPUTER,
	PLAYER,
};

enum class STATUS {
	ILLEGALMOVE,
	OK,
};

enum class GAMESTATE {
    MENUSTATE,
    PAUSESTATE,
    CREDITSSTATE,
    GAMESTATE,
};

enum class DIFFICULTY {
    VERYEASY,
    EASY,
    MEDIUM,
    HARD,
    VERYHARD,
};
