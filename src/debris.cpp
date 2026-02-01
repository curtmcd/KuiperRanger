#include "type.hpp"
#include "debris.hpp"
#include "rand.hpp"
#include "shape.hpp"
#include "line.hpp"
#include "sprite.hpp"
#include "param.hpp"

static Line shardVec[] = {
    { Point(-2.0 / 3, 0), Point(1.0 / 3, 0) }
};

static Shape *baseShardShape;

void Debris::init()
{
    baseShardShape = new Shape(shardVec, std::size(shardVec));
}

void Debris::term()
{
    delete baseShardShape;
}

Debris::Debris(int numShards)
{
    for (int i = 0; i < numShards; i++) {
	Sprite *s = new Sprite();
	s->setShape(baseShardShape);
#if 0
	s->setDisappear(true);
#else
	s->setWrap(true);
#endif
	shards.push_back(s);
    }
}

Debris::~Debris()
{
    for (auto s : shards)
	delete s;
}

void Debris::start(Sprite *whatExploded)
{
    for (auto s : shards) {
	double objSize = whatExploded->getRadius();
	Point objPos = whatExploded->getPos();
	Vect objVel = whatExploded->getVel();

	s->setPos(objPos);

	// Extra shard velocity relative to center of moving object
	Vect shardVel(Rand::range(0, SHARDVEL),
		      Rand::range(0, SHARDVEL));
	s->setVel(objVel + shardVel * objSize);

	double initialSize = objSize * Rand::range(SHARDSIZE, SHARDSIZE * 2);
	s->setScale(initialSize);
	s->setAngle(Rand::range(0, 360));
	s->setOmega(Rand::range(0.0, SHARDMAXROT));

	s->on();
    }
}

void Debris::off()
{
    for (auto s : shards)
	s->off();
}

bool Debris::isClear()
{
    for (auto s : shards)
	if (s->isOn())
	    return false;

    return true;
}

void Debris::update()
{
    for (auto s : shards)
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
