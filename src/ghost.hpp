#ifndef ghost_hpp
#define ghost_hpp

#include "text.hpp"
#include "ship.hpp"

struct Ghost {
    Ghost();
    ~Ghost();

    void start(int player);
    void stop();

    void update();

private:
    Text *text;
    Ship *ghostShip;
    double ghostClock;
};

#endif // !ghost_hpp
