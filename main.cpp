// Kuiper Ranger
//
//   by Curt McDowell
//      Miles Bader
//      William Lott

#include <signal.h>
#include <getopt.h>

#include "type.hpp"
#include "machine.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "sound.hpp"
#include "vect.hpp"
#include "rand.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "rocks.hpp"
#include "missile.hpp"
#include "score.hpp"
#include "highlist.hpp"
#include "speaker.hpp"
#include "title.hpp"
#include "help.hpp"
#include "param.hpp"
#include "debris.hpp"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void cleanup(int sig)
{
    Sound::term();

    Plot::clearBlack();
    Plot::term();

    Button::term();

    if (sig != 0) {
	(void)fprintf(stderr, "%s received signal %d.\n", TITLE, sig);
	exit(128 + sig);
    }

    // Manual says to call SDL_Quit even after quitting all subsystems
    SDL_Quit();

    exit(0);
}

void cleanupInit()
{
#ifndef TESTMODE
    signal(SIGINT, cleanup);
    signal(SIGQUIT, cleanup);
    signal(SIGHUP, cleanup);
    signal(SIGILL, cleanup);
    signal(SIGBUS, cleanup);
    signal(SIGSEGV, cleanup);
    signal(SIGTERM, cleanup);
    signal(SIGFPE, cleanup);
#endif
}

static void usage(const char *progName)
{
    fprintf(stderr,
	    TITLE " " VERSION "\n");
    fprintf(stderr,
	    "Usage: %s [-w WIDTH] [-fhv]\n", progName);
    fprintf(stderr,
	    "   -w WIDTH    Open with a specified window width\n"
	    "   -f          Open in full-screen mode (or press F during game)\n"
	    "   -h          Show this help\n"
	    "   -v          Show program version\n");
    fprintf(stderr,
	    "A faithful implementation of a space game using SDL2 cross-platform\n"
	    "graphics in which your ship shoots down rocks and flying saucers to\n"
	    "gain points.\n");
    exit(1);
}

static void version(const char *progName)
{
    printf("%s version %s\n", progName, VERSION);
    exit(0);
}

int main(int argc, char **argv)
{
    int i;
    int optWidth = Plot::INIT_WIDTH_DEFAULT;
    const char *progName;

    if ((progName = strrchr(argv[0], '/')) != NULL)
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

    Rand::init();

    Button::init();

    // Init plot before modules that may need display size
    if (!Plot::init(optWidth, TITLE)) {
	fprintf(stderr, "Couldn't initialize display.\n");
	exit(1);
    }

    Sound::init();

    // Cleanup handles termination of Button, Plot and Sound
    cleanupInit();

    Missile::init();
    Rocks::init();
    Ship::init();
    Debris::init();
    Alien::init();
    Title::init(TITLE);
    Speaker::init();
    Help::init();
    HighList::init();
    Machine::init();

    // The wrap area is the whole region below the score bar
    {
	Vect screenSize = Plot::getSize();

	Rect wa(Vect(0.0, (double)SCOREBARH),
		Vect(screenSize.x - 1.0, screenSize.y - 1.0));
	Plot::setWrapArea(wa);
    }

    Plot::clearBlack();

    // Plot::flush() governs the frame rate based on monitor vsync rate.
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
    Speaker::term();
    Title::term();
    Alien::term();
    Debris::term();
    Ship::term();
    Rocks::term();
    Missile::term();

    // Cleanup handles termination of Button, Plot and Sound
    cleanup(0);

    //NOTREACHED
    return 0;
}
