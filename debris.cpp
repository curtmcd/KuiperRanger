#include "type.hpp"
#include "debris.hpp"
#include "rand.hpp"
#include "shape.hpp"
#include "line.hpp"
#include "sprite.hpp"
#include "param.hpp"

static Line shardVec[] = {
    { Vect(-2.0 / 3, 0), Vect(1.0 / 3, 0) }
};

static Shape *baseShardShape;

void Debris::init()
{
    baseShardShape = new Shape(shardVec, ARRAYSIZE(shardVec));
}

void Debris::term()
{
    delete baseShardShape;
}

Debris::Debris(int _numShards)
{
    if (_numShards > MAXDEBRIS)
	_numShards = MAXDEBRIS;

    numShards = _numShards;

    for (int i = 0; i < numShards; i++) {
	shards[i] = new Sprite();
	shards[i]->setShape(baseShardShape);
#if 0
	shards[i]->setDisappear(true);
#else
	shards[i]->setWrap(true);
#endif
    }
}

Debris::~Debris()
{
    for (int i = 0; i < numShards; i++)
	delete shards[i];
}

void Debris::start(Sprite *whatExploded)
{
    for (int i = 0; i < numShards; i++) {
	double objSize = whatExploded->getRadius();
	Vect objPos = whatExploded->getPos();
	Vect objVel = whatExploded->getVel();

	shards[i]->setPos(objPos);

	// Extra shard velocity relative to center of moving object
	Vect shardVel(Rand::range(0, SHARDVEL),
		      Rand::range(0, SHARDVEL));
	shards[i]->setVel(objVel + shardVel * objSize);

	double initialSize = objSize * Rand::range(SHARDSIZE, SHARDSIZE * 2);
	shards[i]->setScale(initialSize);
	shards[i]->setAngle(Rand::range(0, 360));
	shards[i]->setAngularVelocity(Rand::range(0.0, SHARDMAXROT));

	shards[i]->on();
    }
}

void Debris::off()
{
    for (int i = 0; i < numShards; i++)
	shards[i]->off();
}

bool Debris::isClear()
{
    for (int i = 0; i < numShards; i++)
	if (shards[i]->isOn())
	    return false;

    return true;
}

void Debris::update()
{
    for (int i = 0; i < numShards; i++) {
	Sprite *s = shards[i];

	if (s->isOn()) {
	    if (s->getScale() <= 1.0)
		s->off();
	    else {
		double scale = s->getScale();
		scale *= std::pow(1.0 - SHARDDECAY, Plot::dt());
		s->setScale(scale);
	    }

	    s->update();
	}
    }
}
