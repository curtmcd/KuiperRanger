#ifndef title_hpp
#define title_hpp

#include "text.hpp"
#include "param.hpp"

struct Title {
    static void init(const char *title);
    static void term();

    static void display();
    static void undisplay();
    static void update();
    static void on();
    static void off();

private:
    static Text *titleText;
    static Text *instrText;
};

#endif // !title_hpp
