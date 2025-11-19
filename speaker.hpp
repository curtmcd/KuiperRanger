#ifndef speaker_hpp
#define speaker_hpp

#include "type.hpp"

namespace Speaker {
    void init();
    void term();

    enum Style {
	soundOff, soundOn
    };

    void setStyle(enum Style style);

    void stayOn(bool enable);

    // Flashing the speaker when stayOn is enabled just leaves it on
    void flash(double duration);

    void update();
};

#endif // !speaker_hpp
