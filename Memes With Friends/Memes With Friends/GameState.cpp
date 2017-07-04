#include "GameState.h"
#include "GameDisplay.h"
#include <allegro5/allegro_font.h>
#include "main.h"
#include "CardFactory.h"
#include "Player1Hand.h"
#include "Player2Hand.h"
#include "GameManager.h"

void GameState::enter(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    if (initialized) return;

    debug = false;

    this->font = font;
    card_factory = std::make_shared<CardFactory>();

    p1hand = std::make_shared<Player1Hand>(font, gamedisplay, *card_factory);
    p2hand = std::make_shared<Player2Hand>(font, gamedisplay, *card_factory);
    
    /****************************** TEMPORARY CODE BEGIN ******************************/

    /* Test cards 1 and 2 are only for number testing at this time and is not displayed on screen. Will be removed shortly */
    Card *card1 = card_factory->create_card();
    test_card = std::make_shared<Card>(*card1);
    test_card->set_font(font);
    test_card->set_gamedisplay(gamedisplay);
    test_card->set_color(al_map_rgb(255, 0, 0));
    test_card->set_pos(50, 50);

    Card *card2 = card_factory->create_card();
    test_card2 = std::make_shared<Card>(*card2);
    test_card2->set_font(font);
    test_card2->set_gamedisplay(gamedisplay);
    test_card2->set_color(al_map_rgb(255, 0, 0));
    test_card2->set_pos(50, 50);

    //Testing and example usage for the GameManager class
    GameManager game = GameManager(font, gamedisplay);
    int currentPlayer = game.get_current_player();
    game.play_card(game.draw_card_from_hand(3), 0, 0);
    int testdown = (*game.get_card(0, 0)).get_down();
    game.set_current_player(!currentPlayer);
    game.play_card(game.draw_card_from_hand(2), 0, 1);
    int testup = (*game.get_card(0, 1)).get_up();
    if (!((testdown > testup) == (*game.get_card(0, 0)).compare_to_down(*game.get_card(0, 1)))) {
        std::cout << "Either card comparisons or card placement is broken." << std::endl;
    }

    int player1Score = game.get_score(0);
    int player2Score = game.get_score(1);
    std::cout << "Scores: " << player1Score << ", " << player2Score << std::endl;

    /****************************** TEMPORARY CODE END ******************************/

    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);
    mousex = state.x;
    mousey = state.y;

    initialized = true;
}

PROCESS_CODE GameState::process(ALLEGRO_EVENT ev, GameDisplay *gamedisplay) {
    int mouse_x = 0, mouse_y = 0;
    int sx = 0, sy = 0;

    mouse_x = ev.mouse.x;
    mouse_y = ev.mouse.y;
    std::tie(sx, sy) = gamedisplay->convert_coordinates(mouse_x, mouse_y);

    if (ev.type == ALLEGRO_EVENT_KEY_UP) {
        switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_D:
                debug = !debug; // toggle debug state
                break;
            case ALLEGRO_KEY_ESCAPE:
                // return to menustate
                switchstate(GAMESTATE::PAUSESTATE);
                break;
        }
    } else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        mousex = sx;
        mousey = sy;
    }

    return PROCESS_CODE::OK;
}

void GameState::draw(GameDisplay *gamedisplay) {
    p1hand->draw();
    p2hand->draw();

    if (debug) {
        // if debug is toggled on, draw debug information above everything else
        std::string mouse_pos_x = "Mouse X: " + std::to_string(mousex) + "    Card 1 D: " + std::to_string(test_card->get_down()) + "    Card 1 turns Card 2: " + (test_card->compare_to_down(*test_card2.get()) ? "True" : "False");
        std::string mouse_pos_y = "Mouse Y: " + std::to_string(mousey) + "    Card 2 U: " + std::to_string(test_card2->get_up());
        std::string mouse_pos = mouse_pos_x + "\n" + mouse_pos_y;
        al_draw_multiline_text(this->font.get(), al_map_rgb(0, 0, 0), 10, 10, GameDisplay::SCREEN_W - 10, 0, ALLEGRO_ALIGN_LEFT, mouse_pos.c_str());
    }

    // draw help text
    al_draw_multiline_text(this->font.get(), al_map_rgb(0, 0, 0), 10, 1040, 500, 0, ALLEGRO_ALIGN_LEFT, "Press D to toggle DEBUG info\nPress ESC to return to menu");
}
