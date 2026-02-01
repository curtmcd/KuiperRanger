#ifndef machine_hpp
#define machine_hpp

#include "type.hpp"

struct Machine {
    static void init();
    static void term();
    static bool update();
};

#endif // !!machine_hpp
