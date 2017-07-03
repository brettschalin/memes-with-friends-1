#include "MenuButton.h"
#include "Point.h"
#include <tuple>
#include <allegro5\allegro.h>
#include <string>
#include <memory>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "GameDisplay.h"

MenuButton::MenuButton(int x, int y, int width, int height) {
    set_bounds(x, y, width, height);
    set_regularcolor(al_map_rgb(124, 124, 124));
    set_highlightcolor(al_map_rgb(195, 181, 195));
    color = regular_color;
    text = "";
}

void MenuButton::set_bounds(int x, int y, int width, int height) {
    this->width = width;
    this->height = height;
    topleft.set_point(x, y);
    bottomright.set_point(x + width, y + height);
}

void MenuButton::set_regularcolor(ALLEGRO_COLOR color) {
    regular_color = color;
}

void MenuButton::set_highlightcolor(ALLEGRO_COLOR color) {
    highlight_color = color;
}

void MenuButton::set_text(std::string text) {
    this->text = text;
}

void MenuButton::mouseover(Point mouse) {
    if (contains(mouse)) {
        color = highlight_color;
    } else {
        color = regular_color;
    }
}

void MenuButton::draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay) {
    al_draw_rectangle(get_x1(),
                      get_y1(),
                      get_x2(),
                      get_y2(),
                      color, 2);

    int x = get_x1() + (width / 2);
    int y = get_y1() + (height / 4);

    al_draw_text(font.get(), color, x, y, ALLEGRO_ALIGN_CENTER, text.c_str());
}

int MenuButton::get_x1() {
    return std::get<0>(topleft.get_point());
}

int MenuButton::get_y1() {
    return std::get<1>(topleft.get_point());
}

int MenuButton::get_x2() {
    return std::get<0>(bottomright.get_point());
}

int MenuButton::get_y2() {
    return std::get<1>(bottomright.get_point());
}

bool MenuButton::contains(Point point) {
    // if mouseX is >= topleftX and mouseX <= bottomrightX
    // AND
    // mouseY is >= topleftY and mouseY <= bottomrightY
    return (
               (std::get<0>(point.get_point()) >= std::get<0>(topleft.get_point())) &&
               (std::get<0>(point.get_point()) <= std::get<0>(bottomright.get_point()))
           )
           &&
           (
               (std::get<1>(point.get_point()) >= std::get<1>(topleft.get_point())) &&
               (std::get<1>(point.get_point()) <= std::get<1>(bottomright.get_point()))
           );
}
