#include "button.hpp"

bool Button::state[Button::NUM_BUTTON];

void Button::init()
{
    clearAll();
}

void Button::term()
{
}

// Bottom-end functions: press() and release() are intended to be called
// from an async input event handler only.
void Button::press(enum Code b) {
    if (b >= 0 && b < NUM_BUTTON)
	state[b] = true;
}

void Button::release(enum Code b)
{
    if (b >= 0 && b < NUM_BUTTON)
	state[b] = false;
}

// Top-end functions, for the main game to get/clear the button state.
bool Button::isDown(enum Code b, bool clear)
{
    bool result;

    if (b >= 0 && b < NUM_BUTTON) {
	result = state[b];
	if (clear)
	    state[b] = false;
    } else
	result = false;

    return result;
}

void Button::clear(enum Code b)
{
    if (b >= 0 && b < NUM_BUTTON)
	state[b] = false;
}

void Button::clearAll()
{
    for (int i = 0; i < (int)NUM_BUTTON; i++)
	state[(enum Code)i] = false;
}
