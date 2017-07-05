#include "GameManager.h"

GameManager::GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay)
{
    computercolor = al_map_rgb(255, 0, 0);
    playercolor = al_map_rgb(0, 0, 255);

    data.playerscore = 0;
    data.computerscore = 0;

	//Assign who's first to play
    srand(time(0));
	int curr_player = (rand() % 2);
    switch (curr_player) {
        case 0:
            set_current_player(PLAYER::COMPUTER);
            break;
        case 1:
        default:
            set_current_player(PLAYER::PLAYER);
            break;
    }

	//initialize player hands
    card_factory = std::make_shared<CardFactory>();
    data.computerCards = std::make_shared<ComputerHand>(font, gamedisplay, *card_factory);
    data.playerCards = std::make_shared<PlayerHand>(font, gamedisplay, *card_factory);
};

PLAYER GameManager::get_current_player()
{
	return data.current_player;
}

void GameManager::set_current_player(PLAYER player)
{
	data.current_player = player;
}

/* TEMPORARY */
gameData GameManager::get_data() {
    return data;
}

/* Only implements the game logic. Anything with animations or display is left to other functions. */
STATUS GameManager::play_card(std::shared_ptr<Card> card, int x, int y)
{
    if (data.board[x][y]) return STATUS::ILLEGALMOVE;

    draw_card_from_hand(card);

	data.board[x][y] = card;

	bool switch_color = false;

	for (int dx = -1; dx < 2; dx++)
	{
		for (int dy = -1; dy < 2; dy++)
		{
			//don't check out of bounds, or the card itself
			if (!in_bounds(x + dx, y + dy)) continue;
			if (dx == 0 && dy == 0) continue;

			std::shared_ptr<Card> other = get_card(x + dx, y + dy);
			if (other == NULL) continue;

			if (dx == -1 && dy == 0)
			{
				if ((*card).compare_to_left(*other))
				{
					switch_color = true;
				}
			}
			if (dx == 1 && dy == 0)
			{
				if ((*card).compare_to_right(*other))
				{
					switch_color = true;
				}
			}
			if (dx == 0 && dy == 1)
			{
				if ((*card).compare_to_up(*other))
				{
					switch_color = true;
				}
			}

			if (dx == 0 && dy == -1)
			{
				if ((*card).compare_to_down(*other))
				{
					switch_color = true;
				}
			}
			if(switch_color)
			{
                switch (data.current_player) {
                    case PLAYER::COMPUTER:
                        (*other).set_color(computercolor);
                        break;
                    case PLAYER::PLAYER:
                        (*other).set_color(playercolor);
                        break;
                }
				switch_color = false;
			}
		}
	}

    return STATUS::OK;
}

bool GameManager::in_bounds(int x, int y)
{
	return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}


std::shared_ptr<Card> GameManager::get_card(int x, int y)
{
	return data.board[x][y];
}

//Removes a card from the hand and returns it.
void GameManager::draw_card_from_hand(std::shared_ptr<Card> card)
{
    switch (data.current_player) {
        case PLAYER::COMPUTER:
            data.computerCards->remove_card(card);
            break;
        case PLAYER::PLAYER:
            data.playerCards->remove_card(card);
            break;
    }
}


int GameManager::get_score(PLAYER player)
{
    int score = 0;

    switch (data.current_player) {
        case PLAYER::COMPUTER:
            score = data.computerscore;
            break;
        case PLAYER::PLAYER:
            score = data.playerscore;
            break;
    }

	return score;
}

void GameManager::draw() {
    data.playerCards->draw();
    data.computerCards->draw();
}
