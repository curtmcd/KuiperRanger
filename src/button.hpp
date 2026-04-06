#ifndef button_hpp
#define button_hpp

#include "type.hpp"

const int textQueueSize = 10;

struct Button {
    enum Code {
	togglePause,
	toggleSound,
	toggleHelp,
	fullScreen,
	quit,
	suspend,
	help,
	start1,
	start2,
	start3,
	fire,
	hyperspace,
	thrust,			// not cleared on read
	rotateLeft,		// not cleared on read
	rotateRight,		// not cleared on read
	printFPS,
	NUM_BUTTON
    };

    static void press(enum Code b);
    static void release(enum Code b);
    static void charIn(int c);

    static bool isDown(enum Code b, bool clear = false);
    static int charGet();
    static void clear(enum Code b);
    static void clearAll();

    static void init();
    static void term();

private:
    static bool state[NUM_BUTTON];
    static int textQueue[textQueueSize];
};

#endif // !button_hpp
