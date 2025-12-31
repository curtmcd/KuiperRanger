#include "shape.hpp"

#include <cmath>

Shape::Shape(int _maxSegs)
{
    maxSegs = _maxSegs;
    segs = new Line[maxSegs];
    count = 0;
    radius = 0;
}

Shape::Shape(const Point *verts, int numVerts)
{
    maxSegs = numVerts;
    segs = new Line[maxSegs];
    count = 0;
    radius = 0;

    for (int i = 0; i < numVerts; i++) {
	Line l;
	l.f = verts[i];
	l.t = verts[(i + 1) % numVerts];
	append(l);
    }
}

Shape::Shape(const Line *lines, int numLines)
{
    maxSegs = numLines;
    segs = new Line[maxSegs];
    count = 0;
    radius = 0;

    for (int i = 0; i < numLines; i++)
	append(lines[i]);
}

Shape::~Shape()
{
    delete []segs;
}

void Shape::append(const Line &l)
{
    if (count < maxSegs) {
	double r;

	segs[count++] = l;

	r = l.f.distance(Point(0.0, 0.0));
	if (r > radius)
	    radius = r;

	r = l.t.distance(Point(0.0, 0.0));
	if (r > radius)
	    radius = r;
    }
}

void Shape::draw(const Point &pos, double angle, double scale) const
{
    double rad = angle * PI / 180.0;
    double sin_a = -std::sin(rad);	// negative because Y axis upside-down
    double cos_a = std::cos(rad);

    for (int i = 0; i < count; i++) {
	double fx = segs[i].f.x * scale;
	double fy = segs[i].f.y * scale;
	double tx = segs[i].t.x * scale;
	double ty = segs[i].t.y * scale;

	Plot::line(pos.x + (fx * cos_a - fy * sin_a),
		   pos.y + (fx * sin_a + fy * cos_a),
		   pos.x + (tx * cos_a - ty * sin_a),
		   pos.y + (tx * sin_a + ty * cos_a));
    }
}

// This routine should be exact for all polygons
// (convex, non-convex, and even ones containing holes).
// The point is specified relative to the shape's origin.

bool Shape::pointInside(const Point& pt) const
{
    // Quick test for radius overlap
    Vect v(pt.x, pt.y);
    if (v.magnitude() > getRadius())
	return false;

    // Full test: count how many lines in the polygon intersect with a
    // ray extending from the point rightward toward infinity.

    double x = pt.x;
    double y = pt.y;

    int intersections = 0;

    for (int i = 0; i < count; i++) {
	double x0 = segs[i].f.x;
	double y0 = segs[i].f.y;
	double x1 = segs[i].t.x;
	double y1 = segs[i].t.y;

	if (MIN(y0, y1) < y && y <= MAX(y0, y1) &&
	    x <= MAX(x0, x1) && y0 != y1)
	    if (x0 == x1 ||
		(y1 > y0 && (y1 - y0) * (x - x0) < (y - y0) * (x1 - x0)) ||
		(y1 < y0 && (y1 - y0) * (x - x0) > (y - y0) * (x1 - x0)))
		intersections++;
    }

    return (intersections % 2) != 0;	    // true if odd
}

// Returns true if any of the line segments of one shape are overlapping
// any of the line segments of another, taking into account the shapes,
// positions, scales and rotations.
bool Shape::collision(const Point &pos, double angle, double scale,
		      Shape *other,
		      const Point &otherPos, double otherAngle, double otherScale) const
{
#if 0	// disabling because it doesn't take wrap into account
    // First make a quick check for shape radii
    double dist = (otherPos - pos).magnitude();
    double sumRad = (getRadius() * scale +
		    other->getRadius() * otherScale);

    if (dist > sumRad)
	return false;
#endif

    // Slow M * N algorithm tests intersection of all line segment pairs

    for (int i = 0; i < count; i++)
	for (int j = 0; j < other->count; j++) {
	    // Line 0: transform segs[i] to display position
	    Line l0 = segs[i];

	    Vect vf(l0.f.x, l0.f.y);
	    vf *= scale;
	    vf.rotate(angle);
	    l0.f = pos + vf;

	    Vect vt(l0.t.x, l0.t.y);
	    vt *= scale;
	    vt.rotate(angle);
	    l0.t = pos + vt;

	    // Line 1: transform other->segs[j] to display position
	    Line l1 = other->segs[j];

	    Vect ovf(l1.f.x, l1.f.y);
	    ovf *= otherScale;
	    ovf.rotate(otherAngle);
	    l1.f = otherPos + ovf;

	    Vect ovt(l1.t.x, l1.t.y);
	    ovt *= otherScale;
	    ovt.rotate(otherAngle);
	    l1.t = otherPos + ovt;

	    if (l0.intersects(l1))
		return true;
	}

    return false;
}

// The following routine tells if a given line segment intersects a
// shape, given the position, rotation, and scale of the shape.
bool Shape::lineTouches(const Point &pos,
			double angle, double scale, const Line &line) const
{
    Line l = line;

    if (scale < 0.01)
	return false;

    // Bring the line segment into the shape's local coordinate system
    Vect v(pos.x, pos.y);

    l.f -= v;
    Vect vf(l.f.x, l.f.y);
    vf.rotate(-angle);
    vf /= scale;
    l.f.x = vf.x;
    l.f.y = vf.y;

    l.t -= v;
    Vect vt(l.t.x, l.t.y);
    vt.rotate(-angle);
    vt /= scale;
    l.t.x = vt.x;
    l.t.y = vt.y;

    // Check if either endpoint is inside the shape
    if (pointInside(l.f) || pointInside(l.t))
	return true;

    // Check if the line segment crosses any of the shape's segments
    for (int i = 0; i < count; i++)
	if (l.intersects(segs[i]))
	    return true;

    return false;
}
