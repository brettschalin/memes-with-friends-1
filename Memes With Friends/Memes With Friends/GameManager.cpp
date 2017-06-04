#include "GameManager.h"

GameManager::GameManager()
{

	//Set colors. Player 1 is red, Player 2 is blue
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	data->colors[0] = &red;
	data->colors[1] = &blue;

	//Set initial scores
	data->scores = new int[2];
	data->scores[0] = HANDSIZE;
	data->scores[1] = HANDSIZE;

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

	//These throw warnings about using the wrong form of delete.
	//Would gladly appreciate corrections.
	delete data->playerCards;
	delete data->board;
	delete data->scores;
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


//Only implements the game logic. Anything with animations or display is left to other functions
//Assumes that a valid move is played
void GameManager::playCard(Card* card, int x, int y)
{
	
	*data->board[x, y] = card;

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
				if ((*card).compare_to_left(other))
				{
					switch_color = true;
				}
			}
			if (dx == 1 && dy == 0)
			{
				if ((*card).compare_to_right(other))
				{
					switch_color = true;
				}
			}
			if (dx == 0 && dy == 1)
			{
				if ((*card).compare_to_up(other))
				{
					switch_color = true;
				}
			}

			if (dx == 0 && dy == -1)
			{
				if ((*card).compare_to_down(other))
				{
					switch_color = true;
				}
			}
			if(switch_color)
			{
				(*other).set_color(*data->colors[data->currentPlayer]);
				switch_color = false;
				
				data->scores[data->currentPlayer]++;
				data->scores[!(data->currentPlayer)]--;

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

//Removes a card from the hand and returns it.
Card* GameManager::drawCardFromHand(int index)
{
	int curr = data->currentPlayer;
	Card* out = data->playerCards[curr][index];
	data->playerCards[curr][index] = NULL;
	for (int i = index + 1; i < HANDSIZE; i++)
	{
		data->playerCards[curr][i - 1] = data->playerCards[curr][i];
	}
	data->playerCards[curr][HANDSIZE - 1] = NULL;
	return out;

}

int GameManager::getScore(int player)
{
	return data->scores[player];
}
