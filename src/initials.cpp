#include "plot.hpp"
#include "initials.hpp"
#include "button.hpp"

Initials::Initials()
{
    instrText = new Text();

    nick[0] = '\0';
    nickText = new Text();

    nickPos = 0;
    nickClock = 0.0;
    nickExpire = 0.0;
}

Initials::~Initials()
{
    delete nickText;
    delete instrText;
}

void Initials::start()
{
    for (int i = 0; i < NICKMAXLEN; i++)
	nick[i] = '_';
    nick[NICKMAXLEN] = '\0';

    nickPos = 0;
    nickClock = 0.0;
    nickExpire = Plot::runTime() + NICKTIMEOUT;

    Vect screenSize = Plot::getSize();

    instrText->setJustify(Text::CENTER, Text::MIDDLE);
    instrText->setPos(Point(screenSize.x / 2.0, screenSize.y * PERCENT(66)));
    instrText->setScale(PERCENT(200));
    instrText->set("ENTER NICKNAME FOR HIGH SCORE!");
    instrText->on();

    nickText->setJustify(Text::CENTER, Text::MIDDLE);
    nickText->setPos(Point(screenSize.x / 2.0, screenSize.y / 2.0));
    nickText->setScale(PERCENT(300));
    nickText->set(nick);
    nickText->on();

    Plot::setTextInputMode(true);
}

// Return true if nickname entry is completed, false otherwise.
// - If completed, copy final nickname to nickReturn[MAXNICKLEN + 1]
// - If user aborts nickname entry, nickReturn will be the empty string.
bool Initials::update(char *nickReturn)
{
    nickReturn[0] = '\0';

    if (Plot::runTime() > nickExpire)
	return true;

    instrText->update();

    {
	char buf[NICKMAXLEN + 1];
	strcpy(buf, nick);

	nickClock += Plot::dt();

	// Current character blinks
	if (nickClock >= NICKBLINK * NICKDUTY)
	    if (nickPos < NICKMAXLEN)
		buf[nickPos] = ' ';

	if (nickClock >= NICKBLINK)
	    nickClock = 0.0;

	nickText->set(buf);
	nickText->update();
    }

    int c, i;
    while ((c = Button::charGet()) != 0) {
	switch (c) {
	case 8:		// Backspace
	case 127:	// DEL
	    if (nickPos > 0) {
		nickPos--;
		nick[nickPos] = '_';
	    }
	    break;

	case 10:	// LF
	case 13:	// CR
	    for (i = 0; nick[i] != '_' && nick[i] != '\0'; i++)
		nickReturn[i] = nick[i];
	    nickReturn[i] = '\0';
	    return true;

	case 3:		// Ctrl-C
	case 4:		// Ctrl-D
	case 27:	// ESC
	    return true;

	default:
	    if (nickPos < NICKMAXLEN) {
		if (c >= 'a' && c <= 'z')
		    c = c - 'a' + 'A';
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')) {
		    nick[nickPos] = (char)c;
		    nickPos++;
		}
	    }
	    break;
	}
    }

    return false;
}

void Initials::stop()
{
    Plot::setTextInputMode(false);

    instrText->off();
    nickText->off();
}
