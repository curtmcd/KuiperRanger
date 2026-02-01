#ifndef vect_hpp
#define vect_hpp

#include "type.hpp"

struct Vect {
    double x, y;

    Vect() : x(0.0), y(.0) {}
    Vect(double _x, double _y) : x(_x), y(_y) {}

    double cross(const Vect& other) const {
	return x * other.y - y * other.x;
    }

    double magnitudeSquared() const {
	return x * x + y * y;
    }

    double magnitude() const {
	return std::sqrt(magnitudeSquared());
    }

    double argument() const {
	return std::atan2(y, x) * 180.0 / M_PI;
    }

    Vect unitize() const {
	double mag = magnitude();
	if (mag < 0.01)
	    mag = 0.01;
	return Vect(x / mag, y / mag);
    }

    Vect rotate(double degrees) {
	double rad = degrees * M_PI / 180.0;
	double s = -std::sin(rad);
	double c = std::cos(rad);
	return Vect(x * c - y * s, x * s + y * c);
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
