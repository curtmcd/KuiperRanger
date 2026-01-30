#ifndef sprite_hpp
#define sprite_hpp

#include "type.hpp"
#include "shape.hpp"
#include "point.hpp"
#include "plot.hpp"

struct Sprite {
    void setShape(Shape *s) { shape = s; }
    Shape *getShape() { return shape; }

    double getRadius() const { return shape->getRadius() * scale; }

    void setWrap(bool w) { wrap = w; }
    void setDisappear(bool d) { disappear = d; }

    void setPos(const Point& _pos) { pos = _pos; }
    Point getPos() const { return pos; }

    void setVel(const Vect& _vel) { vel = _vel; }
    Vect getVel() const { return vel; }

    void setAngle(double _angle) { angle = _angle; }
    double getAngle() const { return angle; }
    void incAngle(double deg) { angle += deg; }

    void setScale(double _scale) { scale = _scale; }
    double getScale() const { return scale; }

    void setAngularVelocity(double w) { angularVelocity = w; }
    double getAngularVelocity() { return angularVelocity; }

    void on() { enabled = true; }
    void off() { enabled = false; }
    bool isOn() const { return enabled; }

    void update();

    bool collision(const Sprite *other) const;
    bool lineTouches(const Line& line) const;

private:
    void draw() const;

    Shape *shape = NULL;

    bool enabled = false;
    bool wrap = false;		// Should wrap around when leaving plot area
    bool disappear = false;	// Should disappear when leaving plot area

    Point pos;
    double angle = 0.0;
    double scale = 1.0;

    Vect vel;
    double angularVelocity = 0.0;
};

#endif // !sprite_hpp
