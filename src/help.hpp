#ifndef help_hpp
#define help_hpp

#include "text.hpp"

struct Help {
    static void init();
    static void term();

    static void on();
    static void off();
    static bool isOn();
    static void update();

private:
    static std::vector<Text *> helpList;
};

#endif // !help_hpp
