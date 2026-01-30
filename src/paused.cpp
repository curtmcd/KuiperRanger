#include "plot.hpp"
#include "paused.hpp"
#include "text.hpp"
#include "param.hpp"

namespace Paused {
    static Linefont *pausedFont1 = NULL;
    static Linefont *pausedFont2 = NULL;

    static const char *pausedString1 = "PAUSED";
    static const char *pausedString2 = "Press P to Resume";

    static Text *pausedText1;
    static Text *pausedText2;

    static bool enabled;

    static double blinkOffTime;
    static double blinkOnTime;

    void init()
    {
	Vect screenSize = Plot::getSize();

	pausedFont1 = new Linefont(PERCENT(300));
	pausedFont2 = new Linefont(PERCENT(200), false, true);

	Vect charSpacing1 = pausedFont1->getCharSpacing();
	Vect charSpacing2 = pausedFont2->getCharSpacing();

	double size1 = (double)strlen(pausedString1) * charSpacing1.x;
	double size2 = (double)strlen(pausedString2) * charSpacing2.x;

	Point txtPos1((screenSize.x - size1) / 2.0, (screenSize.y * PERCENT(15)));
	Point txtPos2((screenSize.x - size2) / 2.0, (screenSize.y * PERCENT(20)));

	pausedText1 = new Text();
	pausedText1->set(pausedString1);
	pausedText1->setFont(pausedFont1);
	pausedText1->setPos(txtPos1);

	pausedText2 = new Text();
	pausedText2->set(pausedString2);
	pausedText2->setFont(pausedFont2);
	pausedText2->setPos(txtPos2);
    }

    void term()
    {
	delete pausedText1;
	delete pausedText2;

	delete pausedFont1;
	delete pausedFont2;
    }

    void start()
    {
	enabled = true;

	double now = Plot::runTime();

	// Use system clock for timing (Plot::dt() returns 0 while paused)
	blinkOffTime = now + PAUSEDBLINK * PAUSEDDUTY;
	blinkOnTime = now + PAUSEDBLINK;

	pausedText1->on();
	pausedText2->on();
    }

    void stop()
    {
	enabled = false;

	pausedText1->off();
	pausedText2->off();
    }

    void update()
    {
	if (enabled) {
	    pausedText1->update();

	    double now = Plot::runTime();

	    if (now > blinkOnTime) {
		blinkOffTime = now + PAUSEDBLINK * PAUSEDDUTY;
		blinkOnTime = now + PAUSEDBLINK;
		pausedText2->on();
	    } else if (now > blinkOffTime)
		pausedText2->off();

	    pausedText2->update();
	}
    }
};
