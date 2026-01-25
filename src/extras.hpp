#ifndef extras_hpp
#define extras_hpp

#include "param.hpp"
#include "ship.hpp"

struct Extras {
    Extras(int startLives);
    ~Extras();

    int get() { return lives; }
    void inc() { if (lives < MAXLIVES) lives++; }
    void dec() { lives--; }
    void on() { show = true; }
    void off() { show = false; }

    void update();

private:
    int lives;		// Unplayed ships remaining
    bool show;
    Ship *ships[MAXLIVES];
};

#endif // !extras_hpp
