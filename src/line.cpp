#include "line.hpp"

bool Line::intersects(const Line& other) const {
    Vect d1 = t - f;
    Vect d2 = other.t - other.f;

    double cp1 = d1.cross(other.f - f);
    double cp2 = d1.cross(other.t - f);

    // If both points of other line are on same side, no intersection
    if (cp1 * cp2 > 0)
	return false;

    double cp3 = d2.cross(f - other.f);
    double cp4 = d2.cross(t - other.f);

    // If both points of this are on the same side of other, no intersection
    if (cp3 * cp4 > 0)
	return false;

    // Parallel/Collinear check:
    // This is the determinant of the 2D matrix formed by the two direction vectors.
    double det = d1.x * d2.y - d1.y * d2.x;
    if (std::abs(det) < 1e-9)
        return false;

    return true;
}

bool Line::intersection(const Line& other, Point& intersect) const {
    Vect d1 = t - f;
    Vect d2 = other.t - other.f;

    double det = d1.cross(d2);

    // Collinear/Parallel check
    if (std::abs(det) < 1e-9)
	return false;

    // Use the cross product of the vector between the starts
    // and the direction vectors to find the 't' parameter.
    Vect startDiff = other.f - f;
    double tParam = (startDiff.x * d2.y - startDiff.y * d2.x) / det;
    double uParam = (startDiff.x * d1.y - startDiff.y * d1.x) / det;

    // Intersection occurs if both parameters are between 0 and 1
    if (tParam >= 0.0 && tParam <= 1.0 && uParam >= 0.0 && uParam <= 1.0) {
        intersect.x = f.x + tParam * d1.x;
        intersect.y = f.y + tParam * d1.y;
        return true;
    }

    return false;
}
