#include "GameManager.h"

GameManager::GameManager()
{

	//Set colors. Player 1 is red, Player 2 is blue
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	data.colors.push_back(red);
	data.colors.push_back(blue);

	//Initialize score array. Players start with their own cards,
	//and -1 doesn't belong to any player.
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
	{
		data.cards_by_player[i] = new int[HANDSIZE];
		std::fill_n(data.cards_by_player[i], HANDSIZE, i);
	}
	data.cards_by_player[NUMBER_OF_PLAYERS + 1] = new int[BOARDSIZE*BOARDSIZE];
	std::fill_n(data.cards_by_player[NUMBER_OF_PLAYERS + 1], BOARDSIZE*BOARDSIZE, -1);


	//Assign who's first to play
	int curr_player = (rand() % 2);
	set_current_player(curr_player);

	//Initialize everyone's hand...
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < HANDSIZE; j++)
		{
			data.player_cards[i][j] = card_factory.create_card();
		}
	}

	//...and the board
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			data.board[i][j] = NULL;
		}
	}

};

GameManager::~GameManager()
{
	for (int i = 0; i < HANDSIZE; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			delete data.player_cards[j][i];
		}
	}

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			delete data.board[j][i];
		}
	}

	for (int i = 0; i <= NUMBER_OF_PLAYERS; i++)
	{
		delete data.cards_by_player[i];
	}
	delete &card_factory;
	delete &data;

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
void GameManager::play_card(Card* card, int x, int y)
{
	
	data.board[x][y] = card;
	data.cards_by_player[NUMBER_OF_PLAYERS][x*BOARDSIZE + y] = data.current_player;

	

	bool switch_color = false;

	for (int dx = -1; dx < 2; dx++)
	{
		for (int dy = -1; dy < 2; dy++)
		{
			if (!in_bounds(x + dx, y + dy)) continue;

			Card* other = get_card(x + dx, y + dy);
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
				data.cards_by_player[NUMBER_OF_PLAYERS][(x+dx)*BOARDSIZE + (y+dy)] = data.current_player;
				switch_color = false;


			}
		}
	}
	

}

bool GameManager::in_bounds(int x, int y)
{
	return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}


Card* GameManager::get_card(int x, int y)
{

	return data.board[x][y];

}

//Removes a card from the hand and returns it.
Card* GameManager::draw_card_from_hand(int index)
{
	int curr = data.current_player;
	Card* out = data.player_cards[curr][index];
	data.player_cards[curr][index] = NULL;
	
	int i = 0;
	
	while (data.player_cards[curr][i] != NULL && i < HANDSIZE)
	{
		data.player_cards[curr][i - 1] = data.player_cards[curr][i];
		i++;
	}
	if (i == 0) i = 1; //make sure there's no indexing errors
	data.player_cards[curr][i-1] = NULL;
	data.cards_by_player[curr][i-1] = -1;

	return out;

}

int GameManager::get_score(int player)
{
	int score = 0;
	for (int outer = 0; outer <= NUMBER_OF_PLAYERS; outer++)
	{
		for (int inner = 0; inner < HANDSIZE; inner++)
		{
			if (data.cards_by_player[outer][inner] == player)
			{
				score++;
			}
		}
	}
	for (int i = 0; i < BOARDSIZE*BOARDSIZE; i++)
	{
		if (data.cards_by_player[NUMBER_OF_PLAYERS][i] == player)
		{
			score++;
		}
	}
	return score;

}
