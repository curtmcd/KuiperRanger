#include "plot.hpp"
#include "sprite.hpp"

void Sprite::draw() const
{
    for (const auto& l : shape->segs) {
	// Transform line to display position
	Vect vf = ((l.f - Origin) * scale).rotate(angle);
	Vect vt = ((l.t - Origin) * scale).rotate(angle);

	Plot::line(pos.x + vf.x, pos.y + vf.y,
		   pos.x + vt.x, pos.y + vt.y);
    }
}

void Sprite::update()
{
    if (enabled && shape != NULL) {
	// Update position and rotation
	pos += vel * Plot::dt();
	angle += omega * Plot::dt();

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
	    double r = shape->getRadius() * scale;

	    if (pos.x - r < wa.ul.x || pos.x + r >= wa.lr.x ||
		pos.y - r < wa.ul.y || pos.y + r >= wa.lr.y)
		enabled = false;
	}

	if (enabled) {
	    Plot::setWrap(wrap);
	    draw();
	    Plot::setWrap(false);
	}
    }
}

// The following routine tells if a given line segment intersects a
// shape, given the position, rotation, and scale of the shape.
bool Sprite::lineTouches(const Line& line) const
{
    if (scale < 0.01)
	return false;

    Line wLine = line;

    Rect wa;
    Plot::getWrapArea(&wa);

    double wrapW = wa.lr.x - wa.ul.x;
    double wrapH = wa.lr.y - wa.ul.y;

    // Compensate for screen wrap to avoid missing collisions: if the line is
    // further from the sprite than half the distance across the wrap area in
    // any direction, wrap it around toward the sprite. It doesn't hurt to do
    // this even for non-wrapping objects because those can never get into a
    // wrapped position.
    if (wLine.f.x > pos.x + wrapW / 2) {
	wLine.f.x -= wrapW;
	wLine.t.x -= wrapW;
    }

    if (wLine.f.x < pos.x - wrapW / 2) {
	wLine.f.x += wrapW;
	wLine.t.x += wrapW;
    }

    if (wLine.f.y > pos.y + wrapH / 2) {
	wLine.f.y -= wrapH;
	wLine.t.y -= wrapH;
    }

    if (wLine.f.y < pos.y - wrapH / 2) {
	wLine.f.y += wrapH;
	wLine.t.y += wrapH;
    }

    // Bring the line segment into the shape's local coordinate system
    Line l;
    l.f = Origin + (wLine.f - pos).rotate(-angle) / scale;
    l.t = Origin + (wLine.t - pos).rotate(-angle) / scale;

    // Check if either endpoint is inside the shape
    if (shape->pointInside(l.f) || shape->pointInside(l.t))
        return true;

    // Check if the line segment crosses any of the shape's segments
    for (const auto& seg : shape->segs)
	if (l.intersects(seg))
	    return true;

    return false;
}

// Return true if any of the line segments of another sprite overlap
// this sprite. Slow O(M + N) algorithm.
bool Sprite::collision(const Sprite *other) const
{
    for (const auto& l : other->shape->segs) {
	// Transform line to display position
	Vect vf = ((l.f - Origin) * other->scale).rotate(other->angle);
	Vect vt = ((l.t - Origin) * other->scale).rotate(other->angle);
	if (lineTouches(Line(other->pos + vf, other->pos + vt)))
	    return true;
    }

    return false;
}
