#include "GameManager.h"

GameManager::GameManager()
{

	//Set colors. Player 1 is red, Player 2 is blue
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	data->colors[0] = &red;
	data->colors[1] = &blue;


	//Assign who's first to play
	int currPlayer = (rand() % 2);
	setCurrentPlayer(currPlayer);

	//Initialize everyone's hand...
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < HANDSIZE; j++)
		{
			data->playerCards[i][j] = new Card();
		}
	}

	//...and the board
	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			data->board[i][j] = NULL;
		}
	}

};

GameManager::~GameManager()
{
	for (int i = 0; i < HANDSIZE; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (data->playerCards[j][i] != NULL)
				delete data->playerCards[j][i];
		}
	}

	for (int i = 0; i < BOARDSIZE; i++)
	{
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (data->board[j][i])
				delete data->board[j][i];
		}
	}

	delete data->playerCards;
	delete data->board;
	delete data;
}

int GameManager::getCurrentPlayer()
{
	return data->currentPlayer;
}

void GameManager::setCurrentPlayer(int player)
{
	data->currentPlayer = player;
}

void GameManager::playCard(int player, int x, int y)
{

	Card* current = getCard(x, y);

	bool switch_color = false;

	for (int dx = -1; dx < 2; dx++)
	{
		for (int dy = -1; dy < 2; dy++)
		{
			if (!in_bounds(x + dx, y + dy)) continue;

			Card* other = getCard(x + dx, y + dy);
			if (other == NULL) continue;

			if (dx == -1 && dy == 0)
			{
				if ((*current).compare_to_left(other))
				{
					switch_color = true;
				}
			}
			if (dx == 1 && dy == 0)
			{
				if ((*current).compare_to_right(other))
				{
					switch_color = true;
				}
			}
			if (dx == 0 && dy == 1)
			{
				if ((*current).compare_to_up(other))
				{
					switch_color = true;
				}
			}

			if (dx == 0 && dy == -1)
			{
				if ((*current).compare_to_down(other))
				{
					switch_color = true;
				}
			}
			if(switch_color)
			{
				(*other).set_color(*data->colors[player]);
				switch_color = false;
			}
		}
	}

}

bool GameManager::in_bounds(int x, int y)
{
	return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}


Card* GameManager::getCard(int x, int y)
{

	return data->board[x][y];

}
