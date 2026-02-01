#ifndef plot_hpp
#define plot_hpp

// SDL2 plotting support
//
// The rest of the game plots using virtual coordinates on a fixed size
// of NOMINAL_WIDTH by NOMINAL_HEIGHT. plot_GetSize() returns
// this virtual size. All shape coordinates, pixel distances and
// pixel/frame velocities that the game uses (such as specified in
// params.h), are assumed to be designed for this virtual size.
//
// This module scales those coordinates up or down to the actual window
// or display size right when plotting. As a consequence, window resize
// can be handled entirely by the Plot module.

#include "vect.hpp"
#include "rect.hpp"
#include "type.hpp"

struct Plot {
    static const int INIT_WIDTH_FULLSCREEN = 0;
    static const int INIT_WIDTH_DEFAULT = -1;

    static bool init(int plotWidth, const char *windowTitle);
    static void setFullScreen(bool enable);
    static bool getFullScreen();
    static void setPauseMode(bool enabble);
    static bool getPauseMode();
    static void term();

    static void suspend();
    static void sync();		// flush and vsync wait
    static double dt();		// sec that passed between frames,
                                //   updated once/frame
    static double runTime();		// time since startup
    static double frameTime();		// long-term average of frame time
    static void printFPS();		// secret debugging feature

    // All of the following functions use virtual (nominal) coordinates.
    // Clip/wrap control:
    //   If wrap is on, drawn lines will wrap based on the wrap area.
    //   If wrap is off, drawn lines will be clipped to the full drawing area.
    static Vect getSize();
    static void setWrapArea(const Rect& wrapArea);
    static void getWrapArea(Rect *wrapArea);
    static void setWrap(bool w);
    static void line(double fx, double fy, double tx, double ty);
    static void drawLetterbox();

    static void pollEvents();
};

#endif // !plot_hpp
