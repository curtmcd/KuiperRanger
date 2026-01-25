#ifndef button_hpp
#define button_hpp

#include "type.hpp"

namespace Button {
    void init();
    void term();

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
	COUNT
    };

    void press(enum Code b);
    void release(enum Code b);
    bool isDown(enum Code b, bool clear = false);
    void clear(enum Code b);
    void clearAll();
};

#endif // !button_hpp
