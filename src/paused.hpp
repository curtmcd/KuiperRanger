#ifndef paused_hpp
#define paused_hpp

#include "text.hpp"

struct Paused {
    static void init();
    static void term();

    static void start();
    static void stop();
    static void update();

private:
    static Text *text1;
    static Text *text2;

    static bool enabled;

    static double blinkOffTime;
    static double blinkOnTime;
};

#endif // !paused_hpp
