#include "plot.hpp"
#include "sprite.hpp"

Sprite::Sprite()
{
    enabled = false;

    shape = NULL;
    angle = 0.0;
    scale = 1.0;

    wrap = false;
    disappear = false;

    pos = Point(0.0, 0.0);
    vel = Vect(0.0, 0.0);
    angularVelocity = 0.0;
}

void Sprite::update()
{
    if (enabled && shape != NULL) {
	// Update position and rotation
	pos += vel * Plot::dt();
	angle += angularVelocity * Plot::dt();

	Rect wa;
	Plot::getWrapArea(&wa);

	if (wrap) {
	    if (pos.x < wa.ul.x)
		pos.x += wa.lr.x - wa.ul.x;

	    if (pos.x >= wa.lr.x)
		pos.x += wa.ul.x - wa.lr.x;

	    if (pos.y < wa.ul.y)
		pos.y += wa.lr.y - wa.ul.y;

	    if (pos.y >= wa.lr.y)
		pos.y += wa.ul.y - wa.lr.y;
	}

	if (disappear) {
	    double rad = shape->getRadius() * scale;

	    if (pos.x - rad < wa.ul.x ||
		pos.x + rad >= wa.lr.x ||
		pos.y - rad < wa.ul.y ||
		pos.y + rad >= wa.lr.y)
		enabled = false;
	}

	if (enabled) {
	    Plot::setWrap(wrap);
	    shape->draw(pos, angle, scale);
	    Plot::setWrap(false);
	}
    }
}
