#ifndef point_hpp
#define point_hpp

#include "vect.hpp"

struct Point;
Vect operator-(const Point& p1, const Point& p2);

struct Point {
    double x, y;

    Point(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}

    Point& operator+=(const Vect& v) {
	x += v.x;
	y += v.y;
	return *this;
    }

    Point& operator-=(const Vect& v) {
	x -= v.x;
	y -= v.y;
	return *this;
    }

    //double distance(const Point& other) const {
    //	return (other - *this).magnitude();
    //}
};

const Point Origin = Point(0.0, 0.0);

inline Point operator+(const Point& p, const Vect& v) {
    Point result = p;
    result += v;
    return result;
}

inline Point operator-(const Point& p, const Vect& v) {
    Point result = p;
    result -= v;
    return result;
}

inline Vect operator-(const Point& p1, const Point& p2) {
    Vect result(p1.x - p2.x, p1.y - p2.y);
    return result;
}

#endif // !point_hpp
