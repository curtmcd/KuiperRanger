#include "line.hpp"

// Determine if two line segments intersect in any way.

#define SAME_SIGN(a, b) \
	(((a) < 0 && (b) < 0) || ((a) > 0 && (b) > 0))

bool Line::intersects(const Line &other) const
{
    double a1, a2, b1, b2, c1, c2;
    double r1, r2, r3, r4;

    a1 = t.y - f.y;
    b1 = f.x - t.x;
    c1 = t.x * f.y - f.x * t.y;

    r3 = a1 * other.f.x + b1 * other.f.y + c1;
    r4 = a1 * other.t.x + b1 * other.t.y + c1;

    if (r3 != 0 && r4 != 0 && SAME_SIGN(r3, r4))
        return false;

    a2 = other.t.y - other.f.y;
    b2 = other.f.x - other.t.x;
    c2 = other.t.x * other.f.y - other.f.x * other.t.y;

    r1 = a2 * f.x + b2 * f.y + c2;
    r2 = a2 * t.x + b2 * t.y + c2;

    if (r1 != 0 && r2 != 0 && SAME_SIGN(r1, r2))
        return false;

    if (fabs(a1 * b2 - a2 * b1) < 1e-9)
        return false;	// Consider colinear to be non-intersecting

    return true;
}
