#include "type.hpp"
#include "missile.hpp"
#include "param.hpp"

static Line missileVec[] = {
    { Vect(-1, 0), Vect(1, 0) }
};

static Shape *casing;

void Missile::init()
{
    casing = new Shape(missileVec, ARRAYSIZE(missileVec));
}

void Missile::term()
{
    delete casing;
}

Missile::Missile()
{
    timeout = 0.0;

    sprite = new Sprite();
    sprite->setShape(casing);
    sprite->setWrap(true);
}

Missile::~Missile()
{
    delete sprite;
}

// Setting the velocity also rotates the sprite to orient the missile in
// the direction of movement.
void Missile::setVel(const Vect& v)
{
    double angle = v.argument();

    sprite->setVel(v);
    sprite->setAngle(-angle);	// negative because y is upside-down
}

void Missile::update()
{
    if (sprite->isOn()) {
	if (timeout > 0.0)
	    timeout -= Plot::dt();
	else
	    sprite->off();
    }

    sprite->update();
}

bool Missile::hitSprite(Sprite *s)
{
    // Create a line segment from the current position to the future
    // position of the missile, then see if it intersects the shape.

    Vect vel = sprite->getVel();

    Line l;
    l.f = sprite->getPos();
    l.t = l.f + vel * Plot::frameTime();

    return s->lineTouches(l);
}
