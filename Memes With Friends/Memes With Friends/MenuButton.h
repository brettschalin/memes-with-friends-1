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
    bool enabled;
    bool visible;
    ALLEGRO_COLOR regular_color;
    ALLEGRO_COLOR highlight_color;
    ALLEGRO_COLOR disabled_color;
    ALLEGRO_COLOR color;
    std::string text;
public:
    MenuButton(int x, int y, int width, int height);
    void set_bounds(int x, int y, int width, int height);
    void set_regularcolor(ALLEGRO_COLOR color);
    void set_highlightcolor(ALLEGRO_COLOR color);
    void set_disabledcolor(ALLEGRO_COLOR color);
    void set_text(std::string text);
    void set_enabled(bool enabled);
    void set_visible(bool visible);
    void mouseover(Point mouse);
    void draw(std::shared_ptr<ALLEGRO_FONT> font, GameDisplay *gamedisplay);
    bool is_enabled();
    int get_x1();
    int get_y1();
    int get_x2();
    int get_y2();
    bool contains(Point point);
};

