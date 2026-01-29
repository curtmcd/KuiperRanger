#ifndef text_hpp
#define text_hpp

#include "linefont.hpp"

#define MAXTEXT 80

struct Text {
    Text();

    void on() { enabled = true; }
    void off() { enabled = false; }
    bool isOn() { return enabled; }

    Vect getSize();	// (unrotated) text size

    void set(const char *_str) {
	strncpy(str, _str, MAXTEXT);
	str[MAXTEXT - 1] = '\0';
    }

    void setPos(const Point& _pos) { pos = _pos; }
    Point getPos() { return pos; }

    void setFont(Linefont *lf) { font = lf; }

    void update();

private:
    char str[MAXTEXT];
    Point pos;
    bool enabled;
    Linefont *font;
};

#endif // !text_hpp
