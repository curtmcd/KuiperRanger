#ifndef vect_hpp
#define vect_hpp

#include "type.hpp"

struct Vect {
    double x, y;

    Vect(double _x = 0.0, double _y = 0.0) : x(_x), y(_y) {}

    double magnitude() const {
	return std::sqrt(x * x + y * y);
    }

    double argument() const {
	return std::atan2(y, x) * 180.0 / PI;
    }

    void unitize() {
	double mag = magnitude();
	if (mag > 0.01) {
	    x /= mag;
	    y /= mag;
	}
    }

    void rotate(double degrees) {
	double rad = degrees * PI / 180.0;
	double s = -std::sin(rad);
	double c = std::cos(rad);
	double new_x = x * c - y * s;
	double new_y = x * s + y * c;
	x = new_x;
	y = new_y;
    }

    Vect& operator+=(const Vect& other) {
	x += other.x;
	y += other.y;
	return *this;
    }

    Vect& operator-=(const Vect& other) {
	x -= other.x;
	y -= other.y;
	return *this;
    }

    Vect& operator*=(double f) {
	x *= f;
	y *= f;
	return *this;
    }

    Vect& operator/=(double f) {
	x /= f;
	y /= f;
	return *this;
    }
};

inline Vect operator+(const Vect& v1, const Vect& v2) {
    Vect result = v1;
    result += v2;
    return result;
}

inline Vect operator-(const Vect& v1, const Vect& v2) {
    Vect result = v1;
    result -= v2;
    return result;
}

inline Vect operator-(const Vect& v) {
    return Vect(-v.x, -v.y);
}

inline Vect operator*(const Vect& v, double f)
{
    Vect result = v;
    result *= f;
    return result;
}

inline Vect operator*(double f, const Vect& v)
{
    return v * f;
}

inline Vect operator/(const Vect& v, double f)
{
    Vect result = v;
    result /= f;
    return result;
}

#endif // !vect_hpp
