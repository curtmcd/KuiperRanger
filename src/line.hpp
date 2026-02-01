#ifndef line_hpp
#define line_hpp

#include "point.hpp"

struct Line {
    Line() = default;
    Line(const Point& _f, const Point& _t) : f(_f), t(_t) {}

    bool intersects(const Line& other) const;
    bool intersection(const Line& other, Point& intersect) const;

    Point f, t;
};

inline Line operator+(const Line& l, const Vect& v) {
    return Line(l.f + v, l.t + v);
}

#endif // !line_hpp
