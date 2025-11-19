#ifndef highlist_hpp
#define highlist_hpp

#include "text.hpp"
#include "param.hpp"

namespace HighList {
    int getBest();

    // Insert a score into high score list if it is a top score
    void record(const char *name, int score);

    // High score graphics
    void init();
    void term();

    void on();
    void off();
    void update();

};

#endif // !highlist_hpp
