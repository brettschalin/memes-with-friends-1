#pragma once

#include "Point.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include "GameDisplay.h"
#include <string>
#include <memory>

class MenuButton {
private:
    Point topleft;
    Point bottomright;
    int width;
    int height;
    ALLEGRO_COLOR regular_color;
    ALLEGRO_COLOR highlight_color;
    ALLEGRO_COLOR color;
    std::string text;
public:
    MenuButton(int x, int y, int width, int height);
    void set_bounds(int x, int y, int width, int height);
    void set_regularcolor(ALLEGRO_COLOR color);
    void set_highlightcolor(ALLEGRO_COLOR color);
    void set_text(std::string text);
    void mouseover(Point mouse);
    void draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay);
    int get_x1();
    int get_y1();
    int get_x2();
    int get_y2();
    bool contains(Point point);
};

