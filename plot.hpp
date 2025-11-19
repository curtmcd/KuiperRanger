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

namespace Plot {
    enum Mode {
        SET,                    // drawing
        RES,                    // erasing
    };

    static const int INIT_WIDTH_FULLSCREEN = 0;
    static const int INIT_WIDTH_DEFAULT = -1;

    bool init(int plotWidth, const char *windowTitle);
    void setFullScreen(bool enable);
    bool getFullScreen();
    void setPauseMode(bool enabble);
    bool getPauseMode();
    void term();

    void suspend();
    void sync();		// flush and vsync wait
    double dt();		// sec that passed between frames,
                                //   updated once/frame
    double frameTime();		// long-term average of frame time
    void printFPS();		// secret debugging feature

    void setMode(enum Mode mode);

    void clearBlack();
    void clearWhite();

    // All of the following functions use virtual (nominal) coordinates.
    // Clip/wrap control:
    //   If wrap is on, drawn lines will wrap based on the wrap area.
    //   If wrap is off, drawn lines will be clipped to the full drawing area.
    Vect getSize();
    void setWrapArea(const Rect& wrapArea);
    void getWrapArea(Rect *wrapArea);
    void setWrap(bool w);
    void line(double fx, double fy, double tx, double ty);
    void drawLetterbox();

    void pollEvents();
};

#endif // !plot_hpp
