#include "Point.h"

Point::Point(int x, int y) {
    set_point(x, y);
}

void Point::set_point(int x, int y) {
    this->x = x;
    this->y = y;
}

std::tuple<int, int> Point::get_point() {
    return std::make_tuple(this->x, this->y);
}
