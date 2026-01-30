#include "type.hpp"
#include "missile.hpp"
#include "rocks.hpp"
#include "vect.hpp"
#include "rand.hpp"
#include "param.hpp"
#include "sound.hpp"

// There are currently three different shapes.
// Maybe new ones could be generated at random so each is unique.
static Point rockVec1[] = {
    Point(2, 20),
    Point(11, 25),
    Point(23, 6),
    Point(19, -5),
    Point(8, -7),
    Point(18, -11),
    Point(14, -19),
    Point(-8, -21),
    Point(-19, -15),
    Point(-24, 3),
    Point(-16, 19),
    Point(-9, 12),
    Point(-8, 26)
};

static Point rockVec2[] = {
    Point(10, -20),
    Point(21, -17),
    Point(18, -2),
    Point(23, 8),
    Point(13, 25),
    Point(1, 21),
    Point(-13, 24),
    Point(-24, 12),
    Point(-16, 0),
    Point(-22, -11),
    Point(-14, -23),
    Point(1, -15)
};

static Point rockVec3[] = {
    Point(5, -22),
    Point(14, -16),
    Point(16, -4),
    Point(23, 3),
    Point(17, 18),
    Point(2, 25),
    Point(-16, 22),
    Point(-26, 5),
    Point(-19, -5),
    Point(-15, -18)
};

static Point *rockVecs[] = {
    rockVec1,
    rockVec2,
    rockVec3
};

static int rockVecLens[] = {
    ARRAYSIZE(rockVec1),
    ARRAYSIZE(rockVec2),
    ARRAYSIZE(rockVec3)
};

#define NUM_ROCK_SHAPES	ARRAYSIZE(rockVecs)

static Shape *rockShapes[3];

// Create an individual rock
Rock::Rock(int _size, Point *pos, Vect *vel, double angularVelocity)
{
    size = _size;
    whole = true;

    sprite = new Sprite();
    int shapeNo = Rand::natural(NUM_ROCK_SHAPES);
    sprite->setShape(rockShapes[shapeNo]);
    sprite->setAngle(Rand::range(0, 360));
    sprite->setAngularVelocity(angularVelocity);
    sprite->setPos(*pos);
    sprite->setVel(*vel);
    sprite->setScale(ROCKBASESIZE * (double)(ROCKSIZES - size) / ROCKSIZES);
    sprite->setWrap(true);

    int numShards = MAX(2, 11 - size * 2);
    debris = new Debris(numShards);

    sprite->on();
}

Rock::~Rock()
{
    delete sprite;
    delete debris;
}

void Rocks::init()
{
    for (int i = 0; i < NUM_ROCK_SHAPES; i++)
        rockShapes[i] = new Shape(rockVecs[i], rockVecLens[i]);
}

void Rocks::term()
{
    for (int i = 0; i < NUM_ROCK_SHAPES; i++)
	delete(rockShapes[i]);
}

Rocks::Rocks()
{
    queue = NULL;
    waveSize = INITWAVESIZE;
    waveInc = INCWAVESIZE;
    waveDelay = 0.0;
}

Rocks::~Rocks()
{
    Rock *next;

    for (Rock *r = queue; r != NULL; r = next) {
        next = r->next;
	delete r;
    }
}

// Pick an initial random position and velocity
// margin_frac is the fraction of the screen size along the
// edges where rocks can initially occur.

static void randomRock(Point *pos, Vect *vel)
{
    Vect screenSize = Plot::getSize();
    Vect margin = screenSize * ROCKMARGIN;

    // Pick a random edge of the screen to start on

    switch (Rand::natural(4)) {
    case 0:		// top
	pos->x = Rand::range(0, screenSize.x);
	pos->y = Rand::range(0, margin.y);
	break;
    case 1:		// bottom
	pos->x = Rand::range(0, screenSize.x);
	pos->y = Rand::range(screenSize.y - margin.y, screenSize.y);
	break;
    case 2:		// left
	pos->x = Rand::range(0, margin.x);
	pos->y = Rand::range(0, screenSize.y);
	break;
    case 3:		// right
    default:
	pos->x = Rand::range(screenSize.x - margin.x, screenSize.x);
	pos->y = Rand::range(0, screenSize.y);
	break;
    }

    vel->x = Rand::range(-MAXROCKVEL, MAXROCKVEL);
    vel->y = Rand::range(-MAXROCKVEL, MAXROCKVEL);
}

void Rocks::start()
{
    assert(queue == NULL);

    for (int i = 0; i < waveSize; i++) {
	int size = 0;

        Point pos;
	Vect vel;
        randomRock(&pos, &vel);

	Rock *r = new Rock(size, &pos, &vel, Rand::range(-MAXROCKROT, MAXROCKROT));
	r->next = queue;
	queue = r;
    }

    waveDelay = WAVEDELAY;
    waveSize += waveInc;
}

void Rocks::on()
{
    for (Rock *r = queue; r != NULL; r = r->next)
        if (r->whole)
            r->sprite->on();
}

void Rocks::off()
{
    for (Rock *r = queue; r != NULL; r = r->next)
        if (r->whole)
            r->sprite->off();
}

bool Rocks::update()
{
    bool newWave = false;

    // Proceed to next wave after the last rock is gone and the
    // inter-wave delay timer has expired.
    if (queue == NULL) {
	if (waveDelay > 0.0)
	    waveDelay -= Plot::dt();
	else {
	    start();
	    newWave = true;
	}
    }

    Rock *r;

    for (Rock **prev = &queue; (r = *prev) != NULL; ) {
        if (r->whole) {
            r->sprite->update();
            prev = &r->next;
        } else {
            r->debris->update();
            if (r->debris->isClear()) {
                *prev = r->next;
                delete r;
            } else
                prev = &r->next;
        }
    }

    return newWave;
}

// Returns number of points scored
int Rocks::split(Rock *r)
{
    if (r->size < ROCKSIZES - 1)
	for (int i = 0; i < SPLITINTO; i++) {
	    Point newPos = r->sprite->getPos();
	    Vect newVel = r->sprite->getVel();

	    Vect v(Rand::range(-MAXROCKVEL, MAXROCKVEL),
		   Rand::range(-MAXROCKVEL, MAXROCKVEL));

	    newVel += v;

	    double oldAv = r->sprite->getAngularVelocity();
	    double newAv = oldAv + Rand::range(-MAXROCKROT, MAXROCKROT);

	    Rock *nr = new Rock(r->size + 1, &newPos, &newVel, newAv);
	    nr->next = queue;
	    queue = nr;
	}

    r->whole = false;

    r->sprite->off();

    r->debris->start(r->sprite);

    Sound::play(Sound::rockExplode);

    int score = MULROCKSCORE * r->size + BASEROCKSCORE;

    return score;
}

// Test collision of any rock with shape sh at position p.
// Splits/destroys rocks.
// Returns number of points scored.

int Rocks::hitSprite(Sprite *s)
{
    for (Rock *r = queue; r != NULL; r = r->next)
        if (r->whole && s->collision(r->sprite))
            return split(r);

    return 0;
}

// Test collision of any rock with a missile.
// Splits/destroys rocks.
// Returns number of points scored.

int Rocks::hitMissile(Missile *m)
{
    for (Rock *r = queue; r != NULL; r = r->next)
        if (!r->whole)
            continue;
        else if (m->hitSprite(r->sprite))
            return split(r);

    return 0;
}
