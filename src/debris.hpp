#ifndef debris_hpp
#define debris_hpp

#include <vector>

#include "sprite.hpp"

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
    double decayFactor;
    std::vector<Sprite *> shards;
};

#endif // !debris_hpp
