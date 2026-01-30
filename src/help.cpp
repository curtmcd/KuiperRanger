#include "param.hpp"
#include "help.hpp"
#include "text.hpp"
#include "sound.hpp"

#include <vector>

#define HELPCOUNT	ARRAYSIZE(helpStrings)

namespace Help {
    // The first character of each line is a property.
    // '~' for bold-italic, '!' for sound-availability.
    static const char *helpStrings[] = {
	"~Welcome to " TITLE " " VERSION,
	" ",
	"   By: Curt McDowell, Miles Bader,",
	"   William Lott, and others!",
	" ",
	"~Game Controls:",
	" ",
	"   One Player Start ...... 1",
	"   Two Player Start ...... 2",
	"   Three Player Start .... 3",
	" ",
	"   Pause ................. P",
#ifndef __EMSCRIPTEN__
	"   Quit .................. Q",
	"   Suspend ............... Ctrl+Z",
#endif // !__EMSCRIPTEN__
	" ",
	"   Toggle Help ........... H",
	"!  Toggle Sound .......... S",
	"   Toggle Full Screen .... F",
	" ",
	"~Ship Controls:",
	" ",
	"   Rotate Left ........... Z",
	"   Rotate Right .......... X",
	"   Thrust ................ Period",
	"   Fire .................. Slash",
	"   Hyperspace ............ Space Bar",
    };

    static Linefont *helpFont = NULL;
    static Linefont *helpFontBI = NULL;
    static std::vector<Text *> helpList;

    void init()
    {
	Vect screenSize = Plot::getSize();

	helpFont = new Linefont(PERCENT(170));
	helpFontBI = new Linefont(PERCENT(220), true, true);

	Vect lineSpacing = helpFont->getLineSpacing();
	Vect charSpacing = helpFont->getCharSpacing();

	double sizeX = 0;
	for (int i = 0; i < HELPCOUNT; i++)
	    if (sizeX < (double)strlen(helpStrings[i] + 1) * charSpacing.x)
		sizeX = (double)strlen(helpStrings[i] + 1) * charSpacing.x;

	double sizeY = HELPCOUNT * lineSpacing.y;
	if (!Sound::available())
	    sizeY -= lineSpacing.y;

	Point txtPos((screenSize.x - sizeX) / 2,
		     screenSize.y * PERCENT(30));

	for (int i = 0; i < HELPCOUNT; i++) {
	    if (helpStrings[i][0] == '!' && !Sound::available())
		continue;

	    Text *t = new Text();

	    t->set(helpStrings[i] + 1);
	    t->setFont(helpStrings[i][0] == '~' ?
		       helpFontBI :
		       helpFont);
	    t->setPos(txtPos);
	    txtPos += lineSpacing;

	    helpList.push_back(t);
	}
    }

    void term()
    {
	for (auto h : helpList)
	    delete h;

	helpList.clear();

	delete helpFont;
	delete helpFontBI;
    }

    void on()
    {
	for (auto h : helpList)
	    h->on();
    }

    void off()
    {
	for (auto h : helpList)
	    h->off();
    }

    bool isOn()
    {
	return helpList[0]->isOn();
    }

    void update()
    {
	for (auto h : helpList)
	    h->update();
    }
};
