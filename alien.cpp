#include "line.hpp"
#include "shape.hpp"
#include "rand.hpp"
#include "rocks.hpp"
#include "ship.hpp"
#include "param.hpp"
#include "sound.hpp"
#include "alien.hpp"

static Line alienVec[] = {
    { Point(-11, 11), Point(11, 11) },
    { Point(11, 11), Point(23, 0) },
    { Point(-23, 0), Point(-11, 11) },
    { Point(-23, 0), Point(23, 0) },
    { Point(-23, 0), Point(-11, -8) },
    { Point(23, 0), Point(11, -8) },
    { Point(-11, -8), Point(11, -8) },
    { Point(-8, -8), Point(-5, -18) },
    { Point(-5, -18), Point(5, -18) },
    { Point(5, -18), Point(8, -8) }
};

static Shape *alienShape;
static Linefont *pointsFont;

void Alien::init()
{
    alienShape = new Shape(alienVec, ARRAYSIZE(alienVec));
    pointsFont = new Linefont(PERCENT(130), false);
}

void Alien::term()
{
    delete alienShape;
    delete pointsFont;
}

static double getSpeed(int size)
{
    // Smaller aliens move faster
    return ALIENSPEED + ALIENSPEEDINC * size;
}

Alien::Alien()
{
    size = 0;
    sprite = new Sprite();
    missiles = new MList();
    debris = new Debris(ALIENSHARDS);
    pointsText = new Text();
    pointsTimeout = 0.0;

    pointsText->setFont(pointsFont);

    sprite->setShape(alienShape);
    sprite->setWrap(false);
    sprite->setDisappear(true);

    state = State::GONE;
}

Alien::~Alien()
{
    delete sprite;
    delete missiles;
    delete debris;
    delete pointsText;
}

void Alien::start()
{
    size = Rand::natural(ALIENSIZES);	// 0=biggest

    state = State::ALIVE;

    // Determine randomly whether to come from left or right side of screen
    Rect wa;
    Plot::getWrapArea(&wa);

    double margin = sprite->getRadius() * 1.25;
    Point pos;
    Vect vel;

    if (Rand::natural(2) == 0) {
	pos.x = wa.ul.x + margin;
	vel.x = getSpeed(size);
    } else {
	pos.x = wa.lr.x - margin;
	vel.x = -getSpeed(size);
    }

    pos.y = (wa.lr.y - wa.ul.y - 2.0 * margin) * Rand::frac() + (wa.ul.y + margin);

    // Set initial vertical direction either zero or away from edge

    if (pos.y < (wa.ul.y + wa.lr.y) / 2)
	vel.y = getSpeed(size) * Rand::natural(2);
    else
	vel.y = -getSpeed(size) * Rand::natural(2);

    dirTimeout = Rand::range(0.0, ALIENWANDERING * 2.0);

    sprite->setPos(pos);
    sprite->setVel(vel);
    sprite->setAngle(0.0);
    sprite->setScale((double)(ALIENSIZES - size) / ALIENSIZES);

    sprite->on();
    Sound::play(Sound::alienMotor, true);	// Track sprite state

    missiles->on();
}

void Alien::on()
{
    if (state == State::ALIVE) {
	sprite->on();
	Sound::play(Sound::alienMotor, true);	// Track sprite state
    }

    missiles->on();
    missiles->update();
}

void Alien::off()
{
    if (state == State::ALIVE) {
	Sound::stop(Sound::alienMotor);		// Track sprite state
	sprite->off();
    } else if (state == State::DEBRIS)
	debris->off();

    missiles->off();

    if (pointsText->isOn()) {
	pointsTimeout = 0.0;
	pointsText->off();
	pointsText->update();
    }
}

void Alien::fire(Sprite *targets[], int nTargets, int wave)
{
    Point fireOrigin = sprite->getPos();

    // Pick a random direction to fire by default, in case no targets
    // are suitable.
    Vect fireVel(ALIENSHOTSPEED, 0.0);
    fireVel.rotate(Rand::range(0, 360));

    for (int tryTarget = 0; tryTarget < nTargets; tryTarget++) {
	if (targets[tryTarget] == NULL)
	    continue;

	// Try to figure out where the target will be by the time the
	// missile gets there. This does not take into account ship
	// thrust or ship friction.

	Point targetPos = targets[tryTarget]->getPos();
	Vect targetVel = targets[tryTarget]->getVel();
	double targetDistance = (targetPos - fireOrigin).magnitude();
	double eta = targetDistance / ALIENSHOTSPEED;

	// Don't try if too far away
	if (eta > ALIENSHOTDUR)
	    continue;

	Point predictedPos = targetPos + targetVel * eta;

	// Compute unit vector in direction from alien to target
	Vect u = predictedPos - fireOrigin;
	u.unitize();

	fireVel = u * ALIENSHOTSPEED;

	// Add random deviation that goes down as the wave goes up.
	// wave might be 0 briefly during the start of the first wave.
	double maxDeviation = 45.0 / (wave ? wave : 1);
	fireVel.rotate(Rand::range(-maxDeviation, maxDeviation));

	break;
    }

    missiles->fire(fireOrigin, fireVel, ALIENSHOTDUR, sprite->getRadius());

    Sound::play(Sound::alienFire);
}

void Alien::update(bool inhibitNew, Sprite *targets[], int nTargets, int wave)
{
    bool paused = Plot::getPauseMode();

    switch (state) {
    case State::ALIVE:
	// Fire a missile continuously
	if (!paused && missiles->empty() && nTargets > 0)
	    fire(targets, nTargets, wave);

	dirTimeout -= Plot::dt();
	if (dirTimeout < 0.0) {
	    dirTimeout = Rand::range(0.0, ALIENWANDERING * 2.0);

	    Vect vel = sprite->getVel();
	    vel.y = getSpeed(size) * Rand::natural(2);
	    sprite->setVel(vel);
	}
	break;

    case State::DEBRIS:
	debris->update();

	if (missiles->empty() && debris->isClear())
	    state = State::GONE;

	break;

    case State::GONE:
	if (!paused && !inhibitNew && Rand::frac() < Plot::dt() / ALIENSPAWNTIME)
	    start();
	break;
    }

    bool wasOn = sprite->isOn();

    sprite->update();

    // Check if alien went off edge of screen
    if (wasOn && !sprite->isOn()) {
	Sound::stop(Sound::alienMotor);		// Track sprite state

	state = State::GONE;
    }

    missiles->update();

    if (pointsText->isOn()) {
	pointsTimeout -= Plot::dt();
	if (pointsTimeout <= 0.0)
	    pointsText->off();
	pointsText->update();
    }
}

void Alien::die(int score)
{
    sprite->off();
    Sound::stop(Sound::alienMotor);		// Track sprite state

    state = State::DEBRIS;

    debris->start(sprite);

    if (score) {
	// Position score x% of the alien's radius in the direction
	// opposite to which alien was moving, where it is less likely
	// to interfere with the debris.

	Vect scoreOffset = sprite->getVel();
	scoreOffset.unitize();
	scoreOffset *= -PERCENT(250) * sprite->getRadius();

	Point scorePos = sprite->getPos() + scoreOffset;

	// Center text on computed position

	char scoreText[16];
	sprintf(scoreText, "%d", score);
	pointsText->set(scoreText);
	Vect textOffset = pointsText->getSize();
	pointsText->setPos(scorePos - textOffset / 2.0);

	pointsText->on();
	pointsTimeout = ASCORETIMEOUT;
    }
}

// hitMissile returns points scored and kills alien, if alien is hit.
int Alien::hitMissile(Missile *m)
{
    if (m->hitSprite(sprite)) {
	int score = BASEALIENSCORE + MULALIENSCORE * size;
	die(score);
	Sound::play(Sound::alienPoints);
	return score;
    }

    return 0;
}

struct checkMissileData {
    Rocks *rocks;
    Ship *ship;
    bool shipHit;
};

static bool checkMissile(Missile *m, void *rock)
{
    checkMissileData *md = (checkMissileData *)rock;

    if (md->rocks->hitMissile(m) != 0) {
	m->off();
	return true;
    }

    if (!md->shipHit &&
	md->ship != NULL &&
	md->ship->isOn() &&
	m->hitSprite(md->ship->getSprite())) {
	m->off();
	md->shipHit = true;
    }

    return true;
}

bool Alien::checkCollisions(Rocks *rocks, Ship *ship, int *score)
{
    checkMissileData md;

    md.rocks = rocks;
    md.ship = ship;
    md.shipHit = false;

    if (state == State::ALIVE) {
	if (ship != NULL && ship->isOn()) {
	    if (sprite->collision(ship->getSprite())) {
		md.shipHit = true;
		*score = BASEALIENSCORE + MULALIENSCORE * size;
		die(*score);
		Sound::play(Sound::alienPoints);
	    }
	}

	if (rocks->hitSprite(sprite) != 0) {
	    die(0);
	    Sound::play(Sound::alienExplode);
	}
    }

    missiles->enumerate(checkMissile, (void *)&md);
    return md.shipHit;
}
