#ifndef line_hpp
#define line_hpp

#include "vect.hpp"

struct Line {
    Vect f, t;

    bool intersects(const Line &other) const;
};

#endif // !line_hpp
