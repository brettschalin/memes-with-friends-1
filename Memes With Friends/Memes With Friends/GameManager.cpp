#include "GameManager.h"

GameManager::GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay)
{

	//Set colors. Player 1 is red, Player 2 is blue
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	data.colors.push_back(red);
	data.colors.push_back(blue);


	//Assign who's first to play
	int curr_player = (rand() % 2);
	set_current_player(curr_player);

	//initialize player hands
    card_factory = std::make_shared<CardFactory>();
    data.computerCards = std::make_shared<ComputerHand>(font, gamedisplay, *card_factory);
    data.playerCards = std::make_shared<PlayerHand>(font, gamedisplay, *card_factory);

	//initialize the board and scorekeeper
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			data.board[i][j] = NULL;
			data.scores[i*BOARDSIZE + j] = -1;
		}
	}

};

GameManager::~GameManager()
{

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			data.board[j][i] = NULL;
		}
	}

}

int GameManager::get_current_player()
{
	return data.current_player;
}

void GameManager::set_current_player(int player)
{
	data.current_player = player;
}


/*Only implements the game logic. Anything with animations or display is left to other functions.
Assumes that a valid move is played
*/
void GameManager::play_card(std::shared_ptr<Card> card, int x, int y)
{
	
	data.board[x][y] = card;
	data.scores[x*BOARDSIZE + y] = data.current_player;
	

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

				(*other).set_color(data.colors[data.current_player]);
				switch_color = false;
				data.scores[(x + dx)*BOARDSIZE + (y + dy)] = data.current_player;

			}
		}
	}
	

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
std::shared_ptr<Card> GameManager::draw_card_from_hand(int index)
{
	int curr = data.current_player;

	std::shared_ptr<Card> out = NULL;
	if (curr == 0)
	{
		out = data.computerCards->get_card(index);
		data.computerCards->remove_card(index);
	}
	else
	{
		out = data.playerCards->get_card(index);
		data.playerCards->remove_card(index);
	}

	return out;

}


int GameManager::get_score(int player)
{

	int score;
	if (player != 0)
	{
		score = data.playerCards->hand_size();
	}
	else
	{
		score = data.computerCards->hand_size();
	}

	for (int i = 0; i < BOARDSIZE*BOARDSIZE; i++)
	{
		if (data.scores[i] == player)
		{
			score++;
		}
	}

	
	return score;
}

void GameManager::draw() {
    data.playerCards->draw();
    data.computerCards->draw();
}
