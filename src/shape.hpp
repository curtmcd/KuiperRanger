#ifndef shape_hpp
#define shape_hpp

#include "type.hpp"
#include "line.hpp"
#include "plot.hpp"

struct Shape {
    Shape(int maxSegs);
    Shape(const Point *verts, int numVerts);
    Shape(const Line *lines, int numLines);
    ~Shape();

    void append(const Line &l);
    double getRadius() const { return radius; }

    void draw(const Point &pos, double angle, double scale) const;
    bool pointInside(const Point& pt) const;
    bool collision(const Point &pos, double angle, double scale,
		   Shape *other,
		   const Point &otherPos, double otherAngle, double otherScale) const;
    bool lineTouches(const Point &pos,
		     double angle, double scale, const Line &line) const;

private:
    int maxSegs;	// Segments allocated
    Line *segs;		// Segments
    int count;		// Segments used
    double radius;	// Cached radius
};

#endif // !shape_hpp
