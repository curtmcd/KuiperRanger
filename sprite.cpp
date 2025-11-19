#include "plot.hpp"
#include "sprite.hpp"

static void InitState(SpriteState &sp)
{
    sp.on = false;

    sp.shape = NULL;
    sp.angle = 0.0;
    sp.scale = 1.0;

    sp.wrap = false;
    sp.disappear = false;
    sp.pos = Vect(0.0, 0.0);
}

Sprite::Sprite()
{
    InitState(state);
    InitState(oldState);

    vel = Vect(0.0, 0.0);
    angularVelocity = 0.0;
}

Sprite::~Sprite()
{
    off();
    update();
}

void Sprite::update()
{
    if (!state.on && !oldState.on)
	return;

    // Update position and rotation
    state.pos += vel * Plot::dt();
    state.angle += angularVelocity * Plot::dt();

    Rect wa;
    Plot::getWrapArea(&wa);

    if (state.wrap) {
	if (state.pos.x < wa.ul.x)
	    state.pos.x += wa.lr.x - wa.ul.x;

	if (state.pos.x >= wa.lr.x)
	    state.pos.x += wa.ul.x - wa.lr.x;

	if (state.pos.y < wa.ul.y)
	    state.pos.y += wa.lr.y - wa.ul.y;

	if (state.pos.y >= wa.lr.y)
	    state.pos.y += wa.ul.y - wa.lr.y;
    } else if (state.disappear) {
	double rad = state.shape->getRadius() * state.scale;

	if (state.pos.x - rad < wa.ul.x ||
	    state.pos.x + rad >= wa.lr.x ||
	    state.pos.y - rad < wa.ul.y ||
	    state.pos.y + rad >= wa.lr.y)
	    state.on = false;
    }

    if (oldState.on && oldState.shape) {
	Plot::setMode(Plot::RES);
	Plot::setWrap(state.wrap);
	oldState.shape->draw(oldState.pos, oldState.angle, oldState.scale);
	Plot::setWrap(false);
    }

    if (state.on && state.shape) {
	Plot::setMode(Plot::SET);
	Plot::setWrap(state.wrap);
	state.shape->draw(state.pos, state.angle, state.scale);
	Plot::setWrap(false);
    }

    oldState = state;
}
