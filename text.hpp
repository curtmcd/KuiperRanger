#ifndef text_hpp
#define text_hpp

#include "linefont.hpp"
#include "type.hpp"
#include "vect.hpp"

#define MAXTEXT 80

struct TextState {
    char str[MAXTEXT];
    Vect pos;
    bool on;
    Linefont *font;
};

struct Text {
    Text();
    ~Text();

    void on() { state.on = true; }
    void off() { state.on = false; }
    bool isOn() { return state.on; }

    Vect getSize();	// (unrotated) text size

    void set(const char *str) {
	strncpy(state.str, str, MAXTEXT);
	state.str[MAXTEXT - 1] = '\0';
    }

    void setPos(const Vect& pos) { state.pos = pos; }
    Vect getPos() { return state.pos; }

    void setFont(Linefont *lf) { state.font = lf; }

    void update();

private:
    TextState state, oldState;
};

#endif // !text_hpp
