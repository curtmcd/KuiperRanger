#include "button.hpp"
#include "plot.hpp"
#include "ship.hpp"
#include "shape.hpp"
#include "vect.hpp"
#include "rand.hpp"
#include "sound.hpp"

static Point shipVec[] = {
    Point(-10, 10),
    Point(-5, 2),
    Point(-5, -2),
    Point(-10, -10),
    Point(20, 0)
};

static Point flameVec[] = {
    Point(-5, 2),
    Point(-5, -2),
    Point(-15, 0)
};

static Shape *shipShape;
static Shape *flameShape;

void Ship::init()
{
    shipShape = new Shape(shipVec, ARRAYSIZE(shipVec));
    flameShape = new Shape(flameVec, ARRAYSIZE(flameVec));
}

void Ship::term()
{
    delete shipShape;
    delete flameShape;
}

void Ship::explode()
{
    Sound::play(Sound::shipExplode);

    sprite->off();
    flames->off();
    flameBlink = 0.0;

    debris->start(sprite);

    explosion = SHIPDEATHTIME;
}

Ship::Ship(bool _dummy)
{
    dummy = _dummy;

    sprite = new Sprite();
    sprite->setShape(shipShape);

    flames = new Sprite();
    flames->setShape(flameShape);

    flameBlink = 0.0;

    missiles = new MList();
    debris = new Debris(SHIPSHARDS);

    sprite->setWrap(true);
    flames->setWrap(true);

    hyperspace = 0.0;
    explosion = 0.0;
    dead = false;
    thrustSound = false;
}

Ship::~Ship()
{
    delete missiles;
    delete debris;
    delete sprite;
    delete flames;
}

void Ship::start()
{
    Rect wa;
    Plot::getWrapArea(&wa);

    Point pos((wa.ul.x + wa.lr.x) / 2.0,
	      (wa.ul.y + wa.lr.y) / 2.0);

    sprite->setPos(pos);
    flames->setPos(pos);

    Vect vel(0.0, 0.0);
    sprite->setVel(vel);
    flames->setVel(vel);
    sprite->setAngularVelocity(0.0);
    flames->setAngularVelocity(0.0);
    sprite->setAngle(90.0);
    flames->setAngle(90.0);

    flameBlink = 0.0;
    hyperspace = 0.0;
    explosion = 0.0;
    dead = false;
    thrustSound = false;

    on();
}

void Ship::_process()
{
    bool paused = Plot::getPauseMode();

    missiles->update();

    if (explosion > 0.0) {
	explosion -= Plot::dt();

        if (debris->isClear()) {
            if (explosion <= 0.0)
                dead = true;
        } else
            debris->update();

	return;
    }

    if (dead)
	return;

    if (hyperspace > 0.0) {
        hyperspace -= Plot::dt();
        if (hyperspace > 0.0)
            return;

        // Come out of hyperspace

        Vect screenSize = Plot::getSize();
        Point pos(Rand::range(0, screenSize.x), Rand::range(0, screenSize.y));
        sprite->setPos(pos);
        Vect vel(0.0, 0.0);
        sprite->setVel(vel);

        sprite->on();

        // Ignore controls that might have been used during hyperspace
        Button::clear(Button::hyperspace);
        Button::clear(Button::fire);

        Sound::play(Sound::outHyperspace);
    }

    if (Button::isDown(Button::hyperspace, true) && !paused) {
        sprite->off();
        flames->off();
        flameBlink = 0.0;

        hyperspace = HYPERSPACETIME;

        Sound::play(Sound::inHyperspace);
        return;
    }

    if (!sprite->isOn())
        return;

    // Ship friction
    {
	Vect vel = sprite->getVel();
	vel *= std::pow(1.0 - SHIPFRICTION, Plot::dt());
	// Below a certain speed, come to a stop
	if (vel.magnitude() < SHIPSPEEDMIN)
	    vel = Vect(0.0, 0.0);
	sprite->setVel(vel);
    }

    if (Button::isDown(Button::thrust) && !paused) {
	Vect acl =
	    Vect(SHIPACCEL * Plot::dt(), 0.0).rotate(sprite->getAngle());

	Vect vel = sprite->getVel() + acl;
	if (vel.magnitude() < SHIPSPEEDMAX) {
	    sprite->setVel(vel);
	    flames->setVel(vel);
	}

	Point pos = sprite->getPos();
	flames->setPos(pos);
	flames->setAngle(sprite->getAngle());

	if (!thrustSound) {
	    Sound::play(Sound::shipThrust, true);
	    thrustSound = true;
	}

	flameBlink += Plot::dt();
	if (flameBlink < SHIPFLAMEBLINK * SHIPFLAMEDUTY)
	    flames->on();
	else
	    flames->off();
	if (flameBlink >= SHIPFLAMEBLINK)
	    flameBlink = 0.0;
    } else {
	if (thrustSound) {
	    thrustSound = false;
	    Sound::stop(Sound::shipThrust);
	}

	flameBlink = 0.0;
	flames->off();
    }

    if (Button::isDown(Button::rotateLeft))
	sprite->incAngle(SHIPROTSPEED * Plot::dt());

    if (Button::isDown(Button::rotateRight))
	sprite->incAngle(-SHIPROTSPEED * Plot::dt());

    if (Button::isDown(Button::fire, true) && !paused) {
	if (missiles->getCount() < MISSILELIMIT) {
	    Point shipPos = sprite->getPos();
	    Vect shipVel = sprite->getVel();

	    Vect missileVel =
		Vect(SHIPSHOTSPEED, 0.0).rotate(sprite->getAngle()) + shipVel;

	    missiles->fire(shipPos, missileVel, SHIPSHOTDUR,
			   sprite->getRadius());

	    Sound::play(Sound::shipFire);
	}
    }
}

void Ship::update()
{
    // Do full processing except for Ghost or Extra (no control or missiles)
    if (!dummy)
	_process();

    flames->update();
    sprite->update();

    if (thrustSound && !sprite->isOn()) {
	thrustSound = false;
	Sound::stop(Sound::shipThrust);
    }
}

void Ship::on()
{
    sprite->on();
    flames->off();
    missiles->on();
}

void Ship::off()
{
    if (thrustSound) {
	thrustSound = false;
	Sound::stop(Sound::shipThrust);
    }

    sprite->off();
    flames->off();
    flameBlink = 0.0;
    missiles->off();
    debris->off();
}
