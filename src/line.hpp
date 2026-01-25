#ifndef line_hpp
#define line_hpp

#include "point.hpp"

struct Line {
    Point f, t;

    bool intersects(const Line &other) const;
};

#endif // !line_hpp
