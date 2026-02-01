#include "param.hpp"
#include "help.hpp"
#include "sound.hpp"

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

std::vector<Text *> Help::helpList;

void Help::init()
{
    Vect screenSize = Plot::getSize();

    double widest = 0.0;

    for (size_t i = 0; i < std::size(helpStrings); i++) {
	if (helpStrings[i][0] == '!' && !Sound::available())
	    continue;

	Text *t = new Text();

	t->set(helpStrings[i] + 1);

	if (helpStrings[i][0] == '~') {
	    t->setFont(Font::fontBoldItalic);
	    t->setScale(PERCENT(215));
	} else
	    t->setScale(PERCENT(165));

	t->setJustify(Text::LEFT, Text::BOTTOM);

	Vect textSize = t->getSize();
	if (widest < textSize.x)
	    widest = textSize.x;

	helpList.push_back(t);
    }

    Point txtPos((screenSize.x - widest) / 2,
		 screenSize.y * PERCENT(30));

    for (const auto& t : helpList) {
	t->setPos(txtPos);

	Vect textSize = t->getSize();
	txtPos.y += textSize.y;
    }
}

void Help::term()
{
    for (auto h : helpList)
	delete h;
}

void Help::on()
{
    for (auto h : helpList)
	h->on();
}

void Help::off()
{
    for (auto h : helpList)
	h->off();
}

bool Help::isOn()
{
    return helpList[0]->isOn();
}

void Help::update()
{
    for (auto h : helpList)
	h->update();
}
