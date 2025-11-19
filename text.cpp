#include "text.hpp"
#include "plot.hpp"

static void InitState(TextState &ts)
{
    ts.str[0] = '\0';
    ts.pos = Vect(0.0, 0.0);
    ts.on = false;
    ts.font = NULL;
}

Text::Text()
{
    InitState(state);
    InitState(oldState);
}

Text::~Text()
{
    off();
    update();
}

Vect Text::getSize()
{
    Vect size = state.font->getCharSize();
    size.x = size.x * (double)strlen(state.str);
    return size;
}

static void DrawString(Vect *pos, Linefont *font, const char *str)
{
    Vect curpos = *pos;
    Vect spacing = font->getCharSpacing();

    while (*str != '\0') {
	Shape *s = font->getChar(*str);
	s->draw(curpos, 0.0, 1.0);
	curpos += spacing;
	str++;
    }
}

void Text::update()
{
    if (oldState.on && oldState.font != NULL &&
	(!state.on ||
	 (memcmp(&oldState, &state, sizeof (TextState)) != 0))) {
	Plot::setMode(Plot::RES);
	DrawString(&oldState.pos, oldState.font, oldState.str);
    }

    if (state.on && state.font != NULL) {
	Plot::setMode(Plot::SET);
	DrawString(&state.pos, state.font, state.str);
    }

    oldState = state;
}
