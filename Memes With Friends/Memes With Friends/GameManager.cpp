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

    std::cout << "Playing card (L: " << card->get_left() << ", U: " << card->get_up() << ", R: " << card->get_right() << ", D: " << card->get_down() << ") on (" << x << ", " << y << ")" << std::endl;

    int score = 1;

    std::shared_ptr<Card> up = get_card(x, y - 1);
    if (up) {
        if ((*card).compare_to_up(*up)) {
            std::cout << "Flipping UP card (L: " << up->get_left() << ", U: " << up->get_up() << ", R: " << up->get_right() << ", D: " << up->get_down() << ") on (" << x << ", " << y << ")" << std::endl;
            score++;
            flip_color(up);
        }
    }

    std::shared_ptr<Card> down = get_card(x, y + 1);
    if (down) {
        if ((*card).compare_to_down(*down)) {
            std::cout << "Flipping DOWN card (L: " << down->get_left() << ", U: " << down->get_up() << ", R: " << down->get_right() << ", D: " << down->get_down() << ") on (" << x << ", " << y << ")" << std::endl;
            score++;
            flip_color(down);
        }
    }

    std::shared_ptr<Card> left = get_card(x - 1, y);
    if (left) {
        if ((*card).compare_to_left(*left)) {
            std::cout << "Flipping LEFT card (L: " << left->get_left() << ", U: " << left->get_up() << ", R: " << left->get_right() << ", D: " << left->get_down() << ") on (" << x << ", " << y << ")" << std::endl;
            score++;
            flip_color(left);
        }
    }

    std::shared_ptr<Card> right = get_card(x + 1, y);
    if (right) {
        if ((*card).compare_to_right(*right)) {
            std::cout << "Flipping RIGHT card (L: " << right->get_left() << ", U: " << right->get_up() << ", R: " << right->get_right() << ", D: " << right->get_down() << ") on (" << x << ", " << y << ")" << std::endl;
            score++;
            flip_color(right);
        }
    }

    switch (data.current_player) {
        case PLAYER::COMPUTER:
            data.computerscore += score;
            data.playerscore -= score - 1;
            break;
        case PLAYER::PLAYER:
            data.playerscore += score;
            data.computerscore -= score - 1;
            break;
    }

    std::cout << "C: " << data.computerscore << " | P: " << data.playerscore << std::endl;

    return STATUS::OK;
}

void GameManager::flip_color(std::shared_ptr<Card> card) {
    switch (data.current_player) {
        case PLAYER::COMPUTER:
            (*card).set_color(computercolor);
            break;
        case PLAYER::PLAYER:
            (*card).set_color(playercolor);
            break;
    }
}

void GameManager::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    std::shared_ptr<Card> card = data.playerCards->process(ev, gamedisplay);

    if (card) {
        // card was clicked
    }
}

bool GameManager::in_bounds(int x, int y)
{
	return x >= 0 && x < BOARDSIZE && y >= 0 && y < BOARDSIZE;
}


std::shared_ptr<Card> GameManager::get_card(int x, int y)
{
    if (!in_bounds(x, y)) return NULL;

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
