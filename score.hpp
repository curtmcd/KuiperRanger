#ifndef score_hpp
#define score_hpp

#include "text.hpp"

struct Score {
    Score();
    ~Score();

    void on() {
	pointsText->on();
	highText->on();
    }

    void off() {
	pointsText->off();
	highText->off();
    }

    int get() {
	return points;
    }

    void inc(int _points);

    void update();

private:
    void setText();

    Linefont *font;

    int points;
    Text *pointsText;

    int high;
    Text *highText;
};

#endif // !score_hpp
