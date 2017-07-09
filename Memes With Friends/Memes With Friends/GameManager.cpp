#include "GameManager.h"
#include "Point.h"

GameManager::GameManager(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay)
{
    computercolor = al_map_rgb(255, 0, 0);
    playercolor = al_map_rgb(0, 0, 255);

    std::shared_ptr<ALLEGRO_FONT> turnfont{
        al_load_ttf_font("pirulen.ttf", 32, 0),
        &al_destroy_font
    };

    this->font = turnfont;

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

    int newx = BOARD_LEFT + (BOARD_SLOTSIZE_W * x);
    int newy = BOARD_TOP + (BOARD_SLOTSIZE_H * y);

    card->set_pos(newx + (Card::CARD_BORDER_WIDTH*2), newy + 5 + (Card::CARD_BORDER_WIDTH*2));
    card->set_played(true);

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

bool GameManager::gridcontains(int x, int y, Point topleft, Point bottomright) {
    // if mouseX is >= topleftX and mouseX <= bottomrightX
    // AND
    // mouseY is >= topleftY and mouseY <= bottomrightY
    return (
               (x >= std::get<0>(topleft.get_point())) &&
               (x <= std::get<0>(bottomright.get_point()))
           )
           &&
           (
               (y >= std::get<1>(topleft.get_point())) &&
               (y <= std::get<1>(bottomright.get_point()))
           );
}

std::tuple<int, int> GameManager::gridslotat(int x, int y) {
    for (int r = 0; r < BOARDSIZE; r++) {
        for (int c = 0; c < BOARDSIZE; c++) {
            int offsetx = BOARD_LEFT + (BOARD_SLOTSIZE_W * c);
            int offsety = BOARD_TOP + (BOARD_SLOTSIZE_H * r);
            Point topleft{offsetx + 5, offsety + 10};
            Point bottomright{offsetx + 5 + Card::CARD_W + (Card::CARD_BORDER_WIDTH * 2), offsety + 10 + Card::CARD_H + (Card::CARD_BORDER_WIDTH * 2)};
            if (gridcontains(x, y, topleft, bottomright)) {
                return std::make_tuple(c, r);
            }
        }
    }

    return std::make_tuple(-1, -1);
}

void GameManager::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    if (get_current_player() == PLAYER::COMPUTER) return;

    // a card is selected so let's test if the grid is being hovered over
    if (selected_card) {
        int mouse_x = 0, mouse_y = 0;
        int sx = 0, sy = 0;

        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);
        mouse_x = state.x;
        mouse_y = state.y;

        std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);

        int gridx = 0, gridy = 0;
        std::tie(gridx, gridy) = gridslotat(sx, sy);

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && ev.mouse.button == 1 && gridx != -1 && !grid_occupied(gridx, gridy)) {
            std::cout << "PLAY CARD ON (" << gridx << ", " << gridy << ")" << std::endl;
            play_card(selected_card, gridx, gridy);
            hoverdata.shouldhighlight = false;
            selected_card = NULL;
            set_current_player(PLAYER::COMPUTER);
            return;
        }

        if (gridx != -1) {
            if (!grid_occupied(gridx, gridy)) {
                hoverdata.gridx = gridx;
                hoverdata.gridy = gridy;
                hoverdata.shouldhighlight = true;
            } else {
                hoverdata.shouldhighlight = false;
            }
        } else {
            hoverdata.shouldhighlight = false;
        }
    }

    // check if a card is being clicked from the player's hands
    std::shared_ptr<Card> card = data.playerCards->process(ev, gamedisplay);

    if (card && card != selected_card) {
        // a card was clicked that's different than the currently selected card (if any)
        std::cout << "A new card was clicked" << std::endl;

        // change position on the new card
        card->set_pos(std::get<0>(card->get_pos()), std::get<1>(card->get_pos()) - 25);

        // if there's a currently selected card, reset it's position
        if (selected_card) {
            selected_card->set_pos(std::get<0>(selected_card->get_pos()), std::get<1>(selected_card->get_pos()) + 25);
            selected_card = NULL;
            hoverdata.shouldhighlight = false;
        }

        selected_card = card;
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && ev.mouse.button == 1) {
        if (selected_card) {
            selected_card->set_pos(std::get<0>(selected_card->get_pos()), std::get<1>(selected_card->get_pos()) + 25);
            selected_card = NULL;
            hoverdata.shouldhighlight = false;
        }
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

    switch (player) {
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

    ALLEGRO_COLOR turncolor;
    std::string turntext;

    switch (data.current_player) {
        case PLAYER::COMPUTER:
            turntext = "Computer's Turn";
            turncolor = al_map_rgb(255, 0, 0);
            break;
        case PLAYER::PLAYER:
        default:
            turntext = "Player's Turn";
            turncolor = al_map_rgb(0, 0, 255);
            break;
    }

    al_draw_text(font.get(), turncolor, BOARD_LEFT + (BOARD_W / 2), BOARD_TOP - 50, ALLEGRO_ALIGN_CENTER, turntext.c_str());

    std::string scoretext = "Player: ";
    scoretext.append(std::to_string(data.playerscore));
    scoretext.append(" | Computer: ");
    scoretext.append(std::to_string(data.computerscore));

    al_draw_text(font.get(), al_map_rgb(0, 0, 0), BOARD_LEFT + (BOARD_W / 2), BOARD_BOTTOM + 25, ALLEGRO_ALIGN_CENTER, scoretext.c_str());

    ALLEGRO_COLOR line_color = al_map_rgb(0, 0, 0);

	//draw the board
	for (int i = 0; i <= BOARDSIZE; i++)
	{
		draw_horizontal_line(BOARD_TOP + (i * 1.0 / BOARDSIZE)*BOARD_H, line_color);
		draw_vertical_line(BOARD_LEFT + (i * 1.0 / BOARDSIZE)*BOARD_W, line_color);
	}

    //and the cards
    for (auto &outer : data.board) {
        for (auto &card : outer) {
            if (card != NULL) {
                card->draw();
            }
        }
    }

    if (hoverdata.shouldhighlight) {
        int offsetx = BOARD_LEFT + (BOARD_SLOTSIZE_W * hoverdata.gridx);
        int offsety = BOARD_TOP + (BOARD_SLOTSIZE_H * hoverdata.gridy);
        Point highlight_topleft{offsetx + 5, offsety + 10};
        Point highlight_bottomright{offsetx + 5 + Card::CARD_W + (Card::CARD_BORDER_WIDTH * 2), offsety + 10 + Card::CARD_H + (Card::CARD_BORDER_WIDTH * 2)};
        ALLEGRO_COLOR highlight_color = al_map_rgba(0, 0, 0, 20); // 0, 0, 0, 20
        al_draw_filled_rectangle(std::get<0>(highlight_topleft.get_point()), std::get<1>(highlight_topleft.get_point()), std::get<0>(highlight_bottomright.get_point()), std::get<1>(highlight_bottomright.get_point()), highlight_color);
    }
}

void GameManager::draw_horizontal_line(float y, ALLEGRO_COLOR color)
{
	al_draw_line(BOARD_LEFT, y, BOARD_RIGHT, y, color, 2);	
}

void GameManager::draw_vertical_line(float x, ALLEGRO_COLOR color)
{
	al_draw_line(x, BOARD_TOP, x, BOARD_BOTTOM, color, 2);
}

bool GameManager::grid_occupied(int x, int y) {
    if (data.board[x][y] != NULL) return true;

    return false;
}
