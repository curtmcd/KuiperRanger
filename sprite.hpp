#ifndef sprite_hpp
#define sprite_hpp

#include "type.hpp"
#include "shape.hpp"
#include "vect.hpp"
#include "plot.hpp"

struct SpriteState {
    bool on;

    Shape *shape;
    double angle;
    double scale;

    bool wrap;     	// Should wrap around when going out of plot window
    bool disappear;	// Should disappear when going out of plot window

    Vect pos;
};

struct Sprite {
    Sprite();
    ~Sprite();

    void setShape(Shape *s) { state.shape = s; }
    Shape *getShape() { return state.shape; }

    double getRadius() const { return state.shape->getRadius() * state.scale; }

    void setWrap(bool w) { state.wrap = w; }
    void setDisappear(bool d) { state.disappear = d; }

    void setPos(const Vect &pos) { state.pos = pos; }
    Vect getPos() const { return state.pos; }

    void setVel(const Vect &_vel) { vel = _vel; }
    Vect getVel() const { return vel; }

    void setAngle(double angle) { state.angle = angle; }
    double getAngle() const { return state.angle; }
    void incAngle(double deg) { state.angle += deg; }

    void setScale(double scale) { state.scale = scale; }
    double getScale() const { return state.scale; }

    void setAngularVelocity(double w) { angularVelocity = w; }
    double getAngularVelocity() { return angularVelocity; }

    void on() { state.on = true; }
    void off() { state.on = false; }
    bool isOn() const { return state.on; }

    void update();

    bool lineTouches(const Line& l) const {
	return state.shape->lineTouches(state.pos, state.angle, state.scale, l);
    }

    bool collision(const Sprite *other) const {
	return state.shape->collision(state.pos, state.angle, state.scale,
				      other->state.shape,
				      other->state.pos, other->state.angle, other->state.scale);
    }

private:
    SpriteState state, oldState;

    Vect vel;
    double angularVelocity;
};

#endif // !sprite_hpp
