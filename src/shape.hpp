#ifndef shape_hpp
#define shape_hpp

#include "type.hpp"
#include "line.hpp"

struct Shape {
    Shape() {};
    Shape(const Point *verts, int numVerts);
    Shape(const Line *lines, int numLines);

    void clear() { segs.clear(); }
    void append(const Line& l);
    void append(const Shape *sh, const Vect& offset);
    double getRadius() const { return radius; }

    void draw(const Point& pos) const;
    bool pointInside(const Point& pt) const;

public:
    std::vector<Line> segs;

private:
    double radius = 0.0;	// Cached radius
};

#endif // !shape_hpp
