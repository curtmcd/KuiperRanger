#ifndef ship_hpp
#define ship_hpp

#include "sprite.hpp"
#include "mlist.hpp"
#include "debris.hpp"
#include "param.hpp"

struct Ship {
    static void init();
    static void term();

    Ship(bool _dummy);
    ~Ship();

    void start();
    void explode();
    void on();
    void off();

    Vect getPos() { return sprite->getPos(); }
    Vect getVel() { return sprite->getVel(); }

    MList *getMissiles() { return missiles; }
    Sprite *getSprite() { return sprite; }

    bool isOn() { return sprite->isOn(); }
    bool isInHyperspace() { return (hyperspace > 0.0); }
    bool isExploding() { return (explosion > 0.0); }
    bool isDead() { return dead; }

    void update();

private:
    bool dummy;			// Ghost or Extra
    Sprite *sprite;
    Sprite *flames;
    double flameBlink;		// timer when thrusting
    double hyperspace;		// timer when >0
    double explosion;		// timer when >0
    bool dead;
    bool thrustSound;
    MList *missiles;
    Debris *debris;

    void _process();		// helper
};

#endif // !ship_hpp
