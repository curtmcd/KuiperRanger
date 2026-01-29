#ifndef sprite_hpp
#define sprite_hpp

#include "type.hpp"
#include "shape.hpp"
#include "point.hpp"
#include "plot.hpp"

struct Sprite {
    Sprite();

    void setShape(Shape *s) { shape = s; }
    Shape *getShape() { return shape; }

    double getRadius() const { return shape->getRadius() * scale; }

    void setWrap(bool w) { wrap = w; }
    void setDisappear(bool d) { disappear = d; }

    void setPos(const Point &_pos) { pos = _pos; }
    Point getPos() const { return pos; }

    void setVel(const Vect &_vel) { vel = _vel; }
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

    bool lineTouches(const Line& l) const {
	return shape->lineTouches(pos, angle, scale, l);
    }

    bool collision(const Sprite *other) const {
	return shape->collision(pos, angle, scale,
				other->shape,
				other->pos, other->angle, other->scale);
    }

private:
    bool enabled;

    Shape *shape;
    double angle;
    double scale;

    bool wrap;     	// Should wrap around when going out of plot area
    bool disappear;	// Should disappear when going out of plot area

    Point pos;
    Vect vel;
    double angularVelocity;
};

#endif // !sprite_hpp
