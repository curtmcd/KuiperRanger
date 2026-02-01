#ifndef speaker_hpp
#define speaker_hpp

#include "sprite.hpp"

struct Speaker {
    static void init();
    static void term();

    enum Style {
	soundOff, soundOn
    };

    static void setStyle(enum Style style);

    static void stayOn(bool enable);

    // Flashing the speaker when stayOn is enabled just leaves it on
    static void flash(double duration);

    static void update();

private:
    static Shape *onShape;
    static Shape *offShape;

    static Sprite *sprite;

    static bool alwaysOn;

    static double timer;
};

#endif // !speaker_hpp
