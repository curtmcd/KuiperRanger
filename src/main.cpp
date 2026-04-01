// Kuiper Ranger
//
//   by Curt McDowell
//      Miles Bader
//      William Lott

#include <iostream>
#include <string>
#ifdef _WIN32
#include "getopt_win.hpp"
#else
#include <getopt.h>
#endif

#include "type.hpp"
#include "machine.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "sound.hpp"
#include "point.hpp"
#include "rand.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "rocks.hpp"
#include "missile.hpp"
#include "score.hpp"
#include "highlist.hpp"
#include "speaker.hpp"
#include "paused.hpp"
#include "title.hpp"
#include "help.hpp"
#include "param.hpp"
#include "debris.hpp"

#include <SDL2/SDL.h>		// Note: #defines main to SDL_main

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static void usage(const char *progName)
{
    std::string usageString;

    usageString += TITLE " " VERSION "\n";
    usageString += "Usage: ";
    usageString += progName;
    usageString += " [-w WIDTH] [-fhv]\n"
	"   -w WIDTH    Open with a specified window width\n"
	"   -f          Open in full-screen mode (or press F during game)\n"
	"   -h          Show this help\n"
	"   -v          Show program version\n";	// for help2man?

#ifdef _WIN32
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
			     "Kuiper Ranger Help",
			     usageString.c_str(),
			     NULL);
#else // !_WIN32
    std::cerr << usageString;
#endif // !_WIN32

    exit(1);
}

static void version(const char *progName)
{
    std::string versionString;

    versionString += progName;
    versionString += " version " VERSION "\n";

#ifdef _WIN32
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
			     "Kuiper Ranger Version",
			     versionString.c_str(),
			     NULL);
#else
    std::cerr << versionString;
#endif

    exit(0);
}

int main(int argc, char **argv)
{
    int i;
    int optWidth = Plot::INIT_WIDTH_DEFAULT;
    const char *progName;

    if ((progName = strrchr(argv[0], '/')) != NULL ||
	(progName = strrchr(argv[0], '\\')) != NULL)
	progName++;
    else
	progName = argv[0];

    while ((i = getopt(argc, argv, "w:fvh")) > 0)
	switch (i) {
	case 'w':
	    optWidth = atoi(optarg);
	    break;
	case 'f':
	    optWidth = Plot::INIT_WIDTH_FULLSCREEN;
	    break;
	case 'v':
	    version(progName);
	    break;
	default:
	    usage(progName);
	    break;
	}

    if (optind < argc)
	usage(progName);		// Disallow extra text on cmdline

    Rand::init();

    Button::init();

    // Init plot before modules that may need display size
    if (!Plot::init(optWidth, TITLE)) {
	std::cerr << "Couldn't initialize display\n";
	exit(1);
    }

    Sound::init();
    Font::init();
    Missile::init();
    Rocks::init();
    Ship::init();
    Debris::init();
    Alien::init();
    Title::init(TITLE);
    Speaker::init();
    Paused::init();
    Help::init();
    HighList::init();
    Machine::init();

    // The wrap area is the whole region below the score bar
    {
	Vect screenSize = Plot::getSize();

	Rect wa(Point(0.0, (double)SCOREBARH),
		Point(screenSize.x - 1.0, screenSize.y - 1.0));
	Plot::setWrapArea(wa);
    }

    // Plot::sync() governs the frame rate based on monitor vsync rate.
    // Valocities and accelerations must use Plot::dt in order
    // to update distances and velocities proportionately.

#ifdef __EMSCRIPTEN__

    // Not supporting Pause, Quit or Suspend for web version
    auto gameLoop = []() {
	Plot::pollEvents();
	Machine::update();
	Plot::sync();
    };

    emscripten_set_main_loop(gameLoop, 0, 1);

#else // !EMSCRIPTEN

    for (;;) {
	Plot::pollEvents();
	if (!Machine::update())
	    break;
	Plot::sync();
    }

#endif // !EMSCRIPTEN

    Machine::term();
    HighList::term();
    Help::term();
    Paused::term();
    Speaker::term();
    Title::term();
    Alien::term();
    Debris::term();
    Ship::term();
    Rocks::term();
    Missile::term();
    Font::term();
    Sound::term();
    Plot::term();
    Button::term();
    Rand::term();

    // Manual says to call SDL_Quit even after quitting all subsystems
    SDL_Quit();

    return 0;
}
