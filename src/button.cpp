#include "button.hpp"

bool Button::state[Button::NUM_BUTTON];
int Button::textQueue[textQueueSize];

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

void Button::charIn(int c)
{
    int i;

    for (i = 0; i < textQueueSize; i++)
	if (i == 0)
	    break;

    if (i < textQueueSize - 1) {
	textQueue[i++] = c;
	textQueue[i] = 0;
    }
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

// Return text input character if one is available, else return 0
int Button::charGet()
{
    int c = textQueue[0];

    if (c != 0)
	for (int i = 0; textQueue[i] != 0; i++)
	    textQueue[i] = textQueue[i + 1];

    return c;
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

    textQueue[0] = 0;
}
