#include "text.hpp"

// Text manages a Sprite that has the Shape of a text string.
// When the Sprite has a scale of 1.0, the character size is 6 x 8.
Text::Text()
{
    set("");
    setFont(Font::fontRegular);
    setJustify(CENTER, MIDDLE);

    shape = new Shape;

    sprite = new Sprite;
    sprite->setShape(shape);
}

Text::~Text()
{
    delete sprite;
    delete shape;
}

// Size occupied by scaled text. This is a bit larger than the bounding
// box because it includes character and line spacing.
Vect Text::getSize()
{
    Vect spacing = font->getSpacing();
    double scale = sprite->getScale();

    Vect size(scale * spacing.x * (double)strlen(str),
	      scale * spacing.y * 1.0);

    return size;
}

void Text::update()
{
    if (modified) {
	// Build shape for text by combining character shapes
	shape->clear();

	Vect spacing = font->getSpacing();

	double w = spacing.x * (double)strlen(str);
	double h = spacing.y;

	// Starting point
	Vect cursor(justifyH == RIGHT ? -w : justifyH == CENTER ? -w / 2 : 0.0,
		    justifyV == TOP ? h : justifyV == MIDDLE ? h / 2 : 0.0);

	for (const char *cp = str; *cp != '\0'; cp++) {
	    shape->append(font->getChar(*cp), cursor);
	    cursor.x += spacing.x;
	}

	modified = false;
    }

    sprite->update();
}
