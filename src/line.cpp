#include "line.hpp"

// If lines intersect, return true and intersection point.
// If lines don't intersect, return false.
bool Line::intersection(const Line& other, Point& intersect) const {
    const double eps = 1e-7;

    Vect d1 = t - f;
    Vect d2 = other.t - other.f;

    double det = d1.cross(d2);
    double lenSq1 = d1.magnitudeSquared();
    double lenSq2 = d2.magnitudeSquared();

    // Relative collinear/parallel check, scaled to match tiny or huge lines.
    // Ignores overlapping collinear segments.
    if (det * det < eps * lenSq1 * eps * lenSq2)
        return false;

    Vect df = other.f - f;

    double u = df.cross(d2) / det;
    double v = df.cross(d1) / det;

    // Soft bounds check to prevent floating-point misses at the vertices
    if (u >= -eps && u <= 1.0 + eps && v >= -eps && v <= 1.0 + eps) {
        // Clamp 'u' strictly to [0.0, 1.0] before calculating the point
        // to guarantee the intersection lies visually on the segment
        u = std::max(0.0, std::min(1.0, u));
        intersect = f + u * d1;
        return true;
    }

    return false;
}

bool Line::intersects(const Line& other) const
{
    Point ignore;
    return intersection(other, ignore);
}
