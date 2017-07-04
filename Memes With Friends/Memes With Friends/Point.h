#pragma once

#include <tuple>

class Point {
private:
    int x;
    int y;
public:
    Point() {};
    Point(int x, int y);
    void set_point(int x, int y);
    std::tuple<int, int> get_point();
};

