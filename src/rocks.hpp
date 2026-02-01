#ifndef rocks_hpp
#define rocks_hpp

#include "type.hpp"
#include "sprite.hpp"
#include "missile.hpp"
#include "debris.hpp"

struct Rock {
    Rock(int size, const Point& pos, const Vect& vel, double omega);
    ~Rock();

    int size;
    bool whole;
    Sprite *sprite;
    Debris *debris;

    Rock *next;
};

struct Rocks {
    static void init();
    static void term();

    Rocks();
    ~Rocks();

    void start();
    int hitSprite(Sprite *s);
    int hitMissile(Missile *m);
    bool update();	// returns true if update() started a new wave
    void on();
    void off();
    void setWaveInc(int inc) { waveInc = inc; }
    Sprite *getRandom() { return queue ? queue->sprite : NULL; }

private:
    int split(Rock *r);

    Rock *queue;
    int waveSize, waveInc;
    double waveDelay;
};

#endif // !rocks_hpp
