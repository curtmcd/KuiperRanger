#include "paused.hpp"
#include "text.hpp"

namespace Paused {
    static Linefont *pausedFont1 = NULL;
    static Linefont *pausedFont2 = NULL;

    static const char *pausedString1 = "PAUSED";
    static const char *pausedString2 = "Press P to Resume";

    static Text *pausedText1;
    static Text *pausedText2;

    void init()
    {
	Vect screenSize = Plot::getSize();

	pausedFont1 = new Linefont(PERCENT(300), false);
	pausedFont2 = new Linefont(PERCENT(200), false);

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

    void on()
    {
	pausedText1->on();
	pausedText2->on();
    }

    void off()
    {
	pausedText1->off();
	pausedText2->off();
    }

    void update()
    {
	pausedText1->update();
	pausedText2->update();
    }
};
