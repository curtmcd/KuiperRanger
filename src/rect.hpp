#ifndef rect_hpp
#define rect_hpp

#include "point.hpp"

struct Rect {
    Rect() : ul(Point()), lr(Point()) {};
    Rect(const Point& _ul, const Point& _lr) : ul(_ul), lr(_lr) {};
    Point ul, lr;
};

#endif // !rect_hpp
