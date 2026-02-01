#ifndef text_hpp
#define text_hpp

#include "font.hpp"
#include "sprite.hpp"

#define TEXT_MAXLEN 80

struct Text {
    enum Justify {
	LEFT = 0, TOP = 0,
	CENTER = 1, MIDDLE = 1,
	RIGHT = 2, BOTTOM = 2
    };

    Text();
    ~Text();

    void on() { sprite->on(); }
    void off() { sprite->off(); }
    bool isOn() { return sprite->isOn(); }

    void set(const char *_str) {
	if (strcmp(_str, str) != 0) {
	    strncpy(str, _str, TEXT_MAXLEN);
	    str[TEXT_MAXLEN] = '\0';
	    modified = true;
	}
    };

    void setFont(Font *_font) {
	font = _font;
	modified = true;
    }

    void setJustify(Justify jh, Justify jv) {
	justifyH = jh;
	justifyV = jv;
	modified = true;
    }

    Vect getSize();

    void setPos(const Point& pos) { sprite->setPos(pos); }
    Point getPos() { return sprite->getPos(); }

    void setScale(double scale) { sprite->setScale(scale); }
    double getScale() { return sprite->getScale(); }

    void update();

private:
    Font *font;
    Justify justifyH;
    Justify justifyV;
    char str[TEXT_MAXLEN + 1];
    bool modified;

    Shape *shape;
    Sprite *sprite;
};

#endif // !text_hpp
