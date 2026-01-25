#include "button.hpp"

namespace Button {
    bool state[COUNT];

    void init()
    {
	Button::clearAll();
    }

    void term()
    {
    }

    // Bottom-end functions
    // press() and release() are only to be called from an async input
    // event handler.
    void press(enum Code b) {
	if (b >= 0 && b < COUNT)
	    state[b] = true;
    }

    void release(enum Code b)
    {
	if (b >= 0 && b < COUNT)
	    state[b] = false;
    }

    // Top-end functions
    bool isDown(enum Code b, bool clear)
    {
	bool result;

	if (b >= 0 && b < COUNT) {
	    result = state[b];
	    if (clear)
		state[b] = false;
	} else
	    result = false;

	return result;
    }

    void clear(enum Code b)
    {
	if (b >= 0 && b < COUNT)
	    state[b] = false;
    }

    void clearAll()
    {
	for (int i = 0; i < (int)COUNT; i++)
	    state[(enum Code)i] = false;
    }
};
