#include "text.hpp"
#include "plot.hpp"

Text::Text()
{
    str[0] = '\0';
    pos = Point();
    enabled = false;
    font = NULL;
}

// Bounding box of drawn string
Vect Text::getSize()
{
    return (font->getCharSpacing() * (double)strlen(str) +
	    font->getLineSpacing() * 1.0);
}

void Text::update()
{
    if (enabled && font != NULL) {
	Vect spacing = font->getCharSpacing();
	Point cursor = pos;

	for (const char *cp = str; *cp != '\0'; cp++) {
	    Shape *s = font->getChar(*cp);
	    s->draw(cursor);
	    cursor += spacing;
	}
    }
}
