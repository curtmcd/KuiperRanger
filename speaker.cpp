#include "speaker.hpp"
#include "sound.hpp"
#include "shape.hpp"
#include "sprite.hpp"

static Line onLines[] = {
    { Vect(0, -10), Vect(0, 10) },
    { Vect(0, -10), Vect(20, -10) },
    { Vect(0, 10), Vect(20, 10) },
    { Vect(20, -10), Vect(20, 10) },
    { Vect(20, -10), Vect(40, -30) },
    { Vect(20, 10), Vect(40, 30) },
    { Vect(40, -30), Vect(40, 30) },
    { Vect(55, -10), Vect(60, -5) },
    { Vect(55, 10), Vect(60, 5) },
    { Vect(60, -5), Vect(60, 5) },
    { Vect(60, -20), Vect(67, -10) },
    { Vect(60, 20), Vect(67, 10) },
    { Vect(67, -10), Vect(70, 0) },
    { Vect(67, 10), Vect(70, 0) }
};

static Line offLines[] = {
    { Vect(0, -10), Vect(0, 10) },
    { Vect(0, -10), Vect(20, -10) },
    { Vect(0, 10), Vect(20, 10) },
    { Vect(20, -10), Vect(20, 10) },
    { Vect(20, -10), Vect(40, -30) },
    { Vect(20, 10), Vect(40, 30) },
    { Vect(40, -30), Vect(40, 30) },
    { Vect(50, -10), Vect(70, 10) },
    { Vect(50, 10), Vect(70, -10) }
};

static Shape *onShape;
static Shape *offShape;

static Sprite *sprite;

static bool alwaysOn;

static double timer;

namespace Speaker {

    void init()
    {
	onShape = new Shape(onLines, ARRAYSIZE(onLines));
	offShape = new Shape(offLines, ARRAYSIZE(offLines));

	Vect screenSize = Plot::getSize();
	Vect speakerPos(screenSize.x - onShape->getRadius() * PERCENT(150),
			screenSize.y - onShape->getRadius() * PERCENT(120));

	sprite = new Sprite;
	sprite->setShape(onShape);
	sprite->setWrap(false);
	sprite->setPos(speakerPos);

	alwaysOn = false;
	timer = 0.0;
    }

    void term()
    {
	delete sprite;

	delete onShape;
	delete offShape;
    }

    void setStyle(enum Style style)
    {
	if (style == soundOn)
	    sprite->setShape(onShape);
	else
	    sprite->setShape(offShape);
    }

    void stayOn(bool enable)
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

    void flash(double duration)
    {
	if (Sound::available())
	    sprite->on();

	timer = duration;
    }

    void update()
    {
	if (!alwaysOn && sprite->isOn()) {
	    timer -= Plot::dt();
	    if (timer <= 0.0)
		sprite->off();
	}

	sprite->update();
    }
};
