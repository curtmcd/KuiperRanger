#ifndef alien_hpp
#define alien_hpp

#include "sprite.hpp"
#include "mlist.hpp"
#include "type.hpp"
#include "debris.hpp"
#include "rocks.hpp"
#include "text.hpp"

struct Alien {
    static void init();
    static void term();

    Alien();
    ~Alien();

    void start();
    void update(bool inhibitNew, Sprite *targets[], int nTargets, int level);
    void cancel();
    void on();
    void off();
    void fire(Sprite *targets[], int nTargets, int wave);
    bool alive() { return sprite->isOn(); }

    bool checkCollisions(Rocks *rocks, Ship *ship, int *score);
    int hitMissile(Missile *m);
    void die(int score);

private:
    int size;
    Sprite *sprite;
    MList *missiles;
    Debris *debris;
    Text *pointsText;
    double dirTimeout;
    double pointsTimeout;

    enum class State {
	ALIVE,
	DEBRIS,
	GONE
    };

    enum State state;
};

#endif // !alien_hpp
