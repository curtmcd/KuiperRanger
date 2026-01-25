#ifndef debris_hpp
#define debris_hpp

#include "sprite.hpp"

#define MAXDEBRIS 25

struct Debris {
    static void init();
    static void term();

    Debris(int numShards);
    ~Debris();

    void update();
    void start(Sprite *whatExploded);
    void off();
    bool isClear();

private:
    int numShards;
    double decayFactor;
    Sprite *shards[MAXDEBRIS];
};

#endif // !debris_hpp
