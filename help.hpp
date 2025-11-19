#ifndef help_hpp
#define help_hpp

#include "text.hpp"
#include "param.hpp"

namespace Help {
    void init();
    void term();

    void on();
    void off();
    bool isOn();
    void update();
};

#endif // !help_hpp
