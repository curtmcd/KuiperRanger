#ifndef text_hpp
#define text_hpp

#include "linefont.hpp"

#define TEXT_MAXLEN 80

struct Text {
    Text();

    void on() { enabled = true; }
    void off() { enabled = false; }
    bool isOn() { return enabled; }

    Vect getSize();

    void set(const char *_str) {
	strncpy(str, _str, TEXT_MAXLEN);
	str[TEXT_MAXLEN] = '\0';
    }

    void setPos(const Point& _pos) { pos = _pos; }
    Point getPos() { return pos; }

    void setFont(Linefont *lf) { font = lf; }

    void update();

private:
    char str[TEXT_MAXLEN + 1];
    Point pos;
    bool enabled;
    Linefont *font;
};

#endif // !text_hpp
