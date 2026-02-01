#include "speaker.hpp"
#include "sound.hpp"
#include "shape.hpp"
#include "sprite.hpp"

static Line onLines[] = {
    { Point(0, -10), Point(0, 10) },
    { Point(0, -10), Point(20, -10) },
    { Point(0, 10), Point(20, 10) },
    { Point(20, -10), Point(20, 10) },
    { Point(20, -10), Point(40, -30) },
    { Point(20, 10), Point(40, 30) },
    { Point(40, -30), Point(40, 30) },
    { Point(55, -10), Point(60, -5) },
    { Point(55, 10), Point(60, 5) },
    { Point(60, -5), Point(60, 5) },
    { Point(60, -20), Point(67, -10) },
    { Point(60, 20), Point(67, 10) },
    { Point(67, -10), Point(70, 0) },
    { Point(67, 10), Point(70, 0) }
};

static Line offLines[] = {
    { Point(0, -10), Point(0, 10) },
    { Point(0, -10), Point(20, -10) },
    { Point(0, 10), Point(20, 10) },
    { Point(20, -10), Point(20, 10) },
    { Point(20, -10), Point(40, -30) },
    { Point(20, 10), Point(40, 30) },
    { Point(40, -30), Point(40, 30) },
    { Point(50, -10), Point(70, 10) },
    { Point(50, 10), Point(70, -10) }
};

Shape *Speaker::onShape;
Shape *Speaker::offShape;

Sprite *Speaker::sprite;

bool Speaker::alwaysOn;

double Speaker::timer;

void Speaker::init()
{
    onShape = new Shape(onLines, std::size(onLines));
    offShape = new Shape(offLines, std::size(offLines));

    Vect screenSize = Plot::getSize();
    Point speakerPos(screenSize.x - onShape->getRadius() * PERCENT(150),
		     screenSize.y - onShape->getRadius() * PERCENT(120));

    sprite = new Sprite;
    sprite->setShape(onShape);
    sprite->setWrap(false);
    sprite->setPos(speakerPos);

    alwaysOn = false;
    timer = 0.0;
}

void Speaker::term()
{
    delete sprite;

    delete onShape;
    delete offShape;
}

void Speaker::setStyle(enum Style style)
{
    if (style == soundOn)
	sprite->setShape(onShape);
    else
	sprite->setShape(offShape);
}

void Speaker::stayOn(bool enable)
{
    if (enable) {
	if (Sound::available())
	    sprite->on();
    } else {
	sprite->off();
	timer = 0.0;
    }

    alwaysOn = enable;
}

void Speaker::flash(double duration)
{
    if (Sound::available())
	sprite->on();

    timer = duration;
}

void Speaker::update()
{
    if (!alwaysOn && sprite->isOn()) {
	timer -= Plot::dt();
	if (timer <= 0.0)
	    sprite->off();
    }

    sprite->update();
}
