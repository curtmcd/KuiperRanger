#ifndef shape_hpp
#define shape_hpp

#include "type.hpp"
#include "line.hpp"
#include "plot.hpp"

struct Shape {
    Shape(int maxSegs);
    Shape(const Vect *verts, int numVerts);
    Shape(const Line *lines, int numLines);
    ~Shape();

    void append(const Line &l);
    void rotate(double ang);
    void scale(double scale);
    double getRadius() const { return radius; }

    void draw(const Vect &pos, double angle, double scale) const;
    bool pointInside(const Vect& pt) const;
    bool collision(const Vect &pos, double angle, double scale,
		   Shape *other,
		   const Vect &otherPos, double otherAngle, double otherScale) const;
    bool lineTouches(const Vect &pos,
		     double angle, double scale, const Line &line) const;

private:
    int maxSegs;	// Segments allocated
    Line *segs;		// Segments
    int count;		// Segments used
    double radius;	// Cached radius
};

#endif // !shape_hpp
