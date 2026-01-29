#include "text.hpp"
#include "plot.hpp"

Text::Text()
{
    str[0] = '\0';
    pos = Point(0.0, 0.0);
    enabled = false;
    font = NULL;
}

Vect Text::getSize()
{
    Vect size = font->getCharSize();
    size.x = size.x * (double)strlen(str);
    return size;
}

void Text::update()
{
    if (enabled && font != NULL) {
	Point curpos = pos;
	Vect spacing = font->getCharSpacing();

	for (const char *cp = str; *cp != '\0'; cp++) {
	    Shape *s = font->getChar(*cp);
	    s->draw(curpos, 0.0, 1.0);
	    curpos += spacing;
	}
    }
}
