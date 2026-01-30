#include "plot.hpp"
#include "sprite.hpp"

void Sprite::draw() const
{
    double rad = angle * PI / 180.0;
    double sin_a = -std::sin(rad);	// negative because Y axis upside-down
    double cos_a = std::cos(rad);

    for (const auto& l : shape->segs)
	Plot::line(pos.x + scale * (l.f.x * cos_a - l.f.y * sin_a),
		   pos.y + scale * (l.f.x * sin_a + l.f.y * cos_a),
		   pos.x + scale * (l.t.x * cos_a - l.t.y * sin_a),
		   pos.y + scale * (l.t.x * sin_a + l.t.y * cos_a));
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
	    draw();
	    Plot::setWrap(false);
	}
    }
}

// Returns true if any of the line segments of one sprite are
// overlapping any of the line segments of another.
// Bug: may malfunction if shape(s) wrap around screen
bool Sprite::collision(const Sprite *other) const
{
    // Quick check for shape radii
    double distSquared = (other->pos - pos).magnitudeSquared();
    double sumRad = (getRadius() * scale + other->getRadius() * other->scale);

    if (distSquared > sumRad * sumRad)
	return false;

    // Slow M * N algorithm tests intersection of all line segment pairs

    for (const auto& l0 : shape->segs)
	for (const auto& l1 : other->shape->segs) {
	    // Line A: transform l0 to display position
	    Vect vf = ((l0.f - Origin) * scale).rotate(angle);
	    Vect vt = ((l0.t - Origin) * scale).rotate(angle);
	    Line lA(pos + vf, pos + vt);

	    // Line B: transform l1 to display position
	    Vect ovf = ((l1.f - Origin) * other->scale).rotate(other->angle);
	    Vect ovt = ((l1.t - Origin) * other->scale).rotate(other->angle);
	    Line lB(other->pos + ovf, other->pos + ovt);

	    if (lA.intersects(lB))
		return true;
	}

    return false;
}

// The following routine tells if a given line segment intersects a
// shape, given the position, rotation, and scale of the shape.
bool Sprite::lineTouches(const Line& line) const
{
    if (scale < 0.01)
	return false;

    // Bring the line segment into the shape's local coordinate system
    Line l;
    l.f = Origin + (line.f - pos).rotate(-angle) / scale;
    l.t = Origin + (line.t - pos).rotate(-angle) / scale;

    // Check if either endpoint is inside the shape
    if (shape->pointInside(l.f) || shape->pointInside(l.t))
	return true;

    // Check if the line segment crosses any of the shape's segments
    for (const auto& seg : shape->segs)
	if (l.intersects(seg))
	    return true;

    return false;
}
