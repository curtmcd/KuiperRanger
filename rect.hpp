#ifndef rect_hpp
#define rect_hpp

#include "vect.hpp"

struct Rect {
    Rect() : ul(Vect()), lr(Vect()) {};
    Rect(const Vect& _ul, const Vect& _lr) : ul(_ul), lr(_lr) {};
    Vect ul, lr;
};

#endif // !rect_hpp
