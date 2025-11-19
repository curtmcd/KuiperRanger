#ifndef missile_hpp
#define missile_hpp

#include "vect.hpp"
#include "sprite.hpp"

struct Missile {
    static void init();
    static void term();

    Missile();
    ~Missile();

    void off() { sprite->off(); }
    void on() { sprite->on(); }
    bool isOn() const { return sprite->isOn(); }
    void setPos(const Vect& pos) { sprite->setPos(pos); }
    void setVel(const Vect& v);
    void setTimeout(double sec) { timeout = sec; }
    void update();
    bool hitSprite(Sprite *s);

private:
    Sprite *sprite;
    double timeout;
};

#endif // !missile_hpp
