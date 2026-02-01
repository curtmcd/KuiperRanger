#include "plot.hpp"
#include "paused.hpp"
#include "param.hpp"

static const char *string1 = "PAUSED";
static const char *string2 = "Press P to Resume";

Text *Paused::text1;
Text *Paused::text2;

bool Paused::enabled;

double Paused::blinkOffTime;
double Paused::blinkOnTime;

void Paused::init()
{
    Vect screenSize = Plot::getSize();

    text1 = new Text();
    text1->set(string1);
    text1->setFont(Font::fontBold);
    text1->setScale(PERCENT(300));
    text1->setPos(Point(screenSize.x / 2.0,
			screenSize.y * PERCENT(15)));

    text2 = new Text();
    text2->set(string2);
    text2->setFont(Font::fontItalic);
    text2->setScale(PERCENT(200));
    text2->setPos(Point(screenSize.x / 2.0,
			screenSize.y * PERCENT(20)));
}

void Paused::term()
{
    delete text1;
    delete text2;
}

void Paused::start()
{
    enabled = true;

    double now = Plot::runTime();

    // Use system clock for timing (Plot::dt() returns 0 while paused)
    blinkOffTime = now + PAUSEDBLINK * PAUSEDDUTY;
    blinkOnTime = now + PAUSEDBLINK;

    text1->on();
    text2->on();
}

void Paused::stop()
{
    enabled = false;

    text1->off();
    text2->off();
}

void Paused::update()
{
    if (enabled) {
	text1->update();

	double now = Plot::runTime();

	if (now > blinkOnTime) {
	    blinkOffTime = now + PAUSEDBLINK * PAUSEDDUTY;
	    blinkOnTime = now + PAUSEDBLINK;
	    text2->on();
	} else if (now > blinkOffTime)
	    text2->off();

	text2->update();
    }
}
