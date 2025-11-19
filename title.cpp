#include "title.hpp"

static Linefont *titleFont;
static Text *titleText;

static Linefont *instrFont;
static Text *instrText;

namespace Title {
    void init(const char *title)
    {
	Vect screenSize = Plot::getSize();

	titleText = new Text();
	titleFont = new Linefont(PERCENT(800), 0, true);
	titleText->setFont(titleFont);
	titleText->set(title);

	Vect titleTextSize = titleText->getSize();
	Vect titleTextPos((screenSize.x - titleTextSize.x) / 2, screenSize.y * PERCENT(40));
	titleText->setPos(titleTextPos);

	instrText = new Text();
	instrFont = new Linefont(PERCENT(200), 0, false);
	instrText->setFont(instrFont);
	instrText->set("Press H for Commands");
	Vect instrTextSize = instrText->getSize();
	Vect instrTextPos((screenSize.x - instrTextSize.x) / 2,
			  screenSize.y * PERCENT(95));
	instrText->setPos(instrTextPos);
    }

    void term()
    {
	delete titleText;
	delete instrText;

	delete titleFont;
	delete instrFont;
    }

    void on()
    {
	titleText->on();
	instrText->on();
    }

    void off()
    {
	titleText->off();
	instrText->off();
    }

    void update()
    {
	titleText->update();
	instrText->update();
    }
};
