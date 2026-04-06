#include "shape.hpp"

void Shape::append(const Line& l)
{
    segs.push_back(l);

    double r;

    r = l.f.distance(Origin);
    if (r > radius)
	radius = r;

    r = l.t.distance(Origin);
    if (r > radius)
	radius = r;
}

void Shape::append(const Shape *sh, const Vect &offset)
{
    for (const auto& l : sh->segs)
	append(l + offset);
}

Shape::Shape(const Point *verts, int numVerts)
{
    for (int i = 0; i < numVerts; i++) {
	Line l(verts[i],
	       verts[(i + 1) % numVerts]);
	append(l);
    }
}

Shape::Shape(const Line *lines, int numLines)
{
    for (int i = 0; i < numLines; i++)
	append(lines[i]);
}

// Determine if a point in the shape's coordinate system is inside the
// shape. Exact for all polygons (convex, non-convex, and ones containing
// holes).
bool Shape::pointInside(const Point& pt) const
{
    // Quick radius test
    if (pt.distance(Origin) > radius)
	return false;

    // Count how many lines in the polygon intersect with a ray
    // extending from the point rightward toward infinity.
    int intersections = 0;

    for (const auto& l : segs)
	if ((l.f.y > pt.y) != (l.t.y > pt.y)) {
	    double dy = l.t.y - l.f.y;
	    double cp = (l.t - l.f).cross(pt - l.f);
	    intersections += (dy * cp < 0);
	}

    return (intersections % 2) != 0;	// true if odd
}
