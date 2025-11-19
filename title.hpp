#ifndef title_hpp
#define title_hpp

#include "text.hpp"
#include "param.hpp"

namespace Title {
    void init(const char *title);
    void term();

    void display();
    void undisplay();
    void update();
    void on();
    void off();
};

#endif // !title_hpp
