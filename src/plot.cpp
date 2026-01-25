#include <signal.h>

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "plot.hpp"
#include "param.hpp"
#include "button.hpp"

using namespace Plot;

#define DECORATION_HEIGHT	90	// guessed titlebar height
#define WINDOW_WIDTH_MIN	512	// 512 x 384
#define FPS_DEFAULT		30.0	// only used if vsync unavailable

// Window management
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static bool fullScreen = false;
static bool vsyncSupported = false;
static double counterFrequency;
static int lineThickness = 1;

// Dimensions
static int screenW, screenH;        // Display resolution
static int windowW, windowH;        // Window size
static int renderW, renderH;        // Actual 4:3 render area
static int renderX, renderY;        // Render area offset for letterboxing

// The wrap area is stored as fraction of screen size so that the window
// size can be dynamic. If wrap is off, clip to entire render area.
static bool wrap;
static Rect wrapFrac;

// Drawing state
static SDL_Color drawColor;

// Presentation pacing
static double startTime;
static double prevTime;
static double nextTime;
static double deltaTime;
static int frameNo;
static bool pauseMode;

// Input state
static bool capsLockDown;

void Plot::drawLetterbox()
{
    // Use gray5 for letterbox regions (rgb 13,13,13)
    SDL_SetRenderDrawColor(renderer, 13, 13, 13, 255);

    // Vertical letterboxing
    if (renderX > 0) {
        SDL_Rect leftBox = {0, 0, renderX, windowH};
        SDL_Rect rightBox = {renderX + renderW, 0, renderX, windowH};
        SDL_RenderFillRect(renderer, &leftBox);
        SDL_RenderFillRect(renderer, &rightBox);
    }

    // Horizontal letterboxing
    if (renderY > 0) {
        SDL_Rect topBox = {0, 0, windowW, renderY};
        SDL_Rect bottomBox = {0, renderY + renderH, windowW, renderY};
        SDL_RenderFillRect(renderer, &topBox);
        SDL_RenderFillRect(renderer, &bottomBox);
    }
}

static void handleResize(int newWidth, int newHeight)
{
    windowW = newWidth;
    windowH = newHeight;

    // Calculate 4:3 render area size and position
    double windowRatio = (double)windowW / windowH;
    double targetRatio = (double)ASPECT_X / ASPECT_Y;

    if (windowRatio > targetRatio) {
        // Window is too wide - letterbox sides
        renderH = windowH;
        renderW = (renderH * ASPECT_X) / ASPECT_Y;
        renderX = (windowW - renderW) / 2;
        renderY = 0;
    } else if (windowRatio < targetRatio) {
        // Window is too tall - letterbox top/bottom
        renderW = windowW;
        renderH = (renderW * ASPECT_Y) / ASPECT_X;
        renderX = 0;
        renderY = (windowH - renderH) / 2;
    } else {
        // Perfect 4:3 ratio
        renderW = windowW;
        renderH = windowH;
        renderX = 0;
        renderY = 0;
    }

    drawLetterbox();

    // Thicken lines according to display resolution
    lineThickness = 1 + MIN(windowW / 1280, 2);
}

bool Plot::init(int requestWidth, const char *windowTitle)
{
    // Initialize SDL
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    // Handle request width
    fullScreen = (requestWidth == INIT_WIDTH_FULLSCREEN);

#ifdef __EMSCRIPTEN__
    {
	screenW = (int)EM_ASM_INT({ return window.innerWidth; });
	screenH = (int)EM_ASM_INT({ return window.innerHeight; });
    }
#else // !__EMSCRIPTEN__
    {
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	screenW = dm.w;
	screenH = dm.h - DECORATION_HEIGHT;
    }
#endif // !__EMSCRIPTEN__

    // Deal with INIT_WIDTH_DEFAULT
    if (requestWidth == INIT_WIDTH_DEFAULT) {
        const char* envWidthVar = getenv(ENVVAR_WIDTH);
        int envWidth = envWidthVar ? atoi(envWidthVar) : 0;
        if (envWidth > 0)
            requestWidth = envWidth;
        else {
            // Choose largest 4:3 resolution that fits screen
            requestWidth = (screenH * ASPECT_X / ASPECT_Y) <= screenW ?
                          (screenH * ASPECT_X / ASPECT_Y) : screenW;
        }
    }

    // Deal with INIT_WIDTH_FULLSCREEN
    if (requestWidth == INIT_WIDTH_FULLSCREEN)
        requestWidth = screenW;

    // Bound the width
    if (requestWidth < WINDOW_WIDTH_MIN)
        requestWidth = WINDOW_WIDTH_MIN;
    if (requestWidth > screenW)
        requestWidth = screenW;

    // Create window
    window = SDL_CreateWindow(windowTitle,
			      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			      requestWidth, (requestWidth * ASPECT_Y) / ASPECT_X,
			      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |
			      (fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));

    if (!window) {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return false;
    }

    // Set minimum window size to maintain aspect ratio
    SDL_SetWindowMinimumSize(window,
			     WINDOW_WIDTH_MIN,
			     (WINDOW_WIDTH_MIN * ASPECT_Y) / ASPECT_X);

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1,
				  SDL_RENDERER_ACCELERATED |
				  SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return false;
    }

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) != 0)
	fprintf(stderr, "Could not get SDL2 renderer info\n");
    else if (info.flags & SDL_RENDERER_PRESENTVSYNC)
	vsyncSupported = true;
    else
	vsyncSupported = false;

    // Hide cursor in both window and fullscreen modes
    SDL_ShowCursor(SDL_DISABLE);

    // Get initial window size and set up render area
    SDL_GetWindowSize(window, &windowW, &windowH);
    handleResize(windowW, windowH);

    // Set blend mode for anti-aliasing
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Initial wrap area is full drawing area
    Rect wa(Point(0.0, 0.0),
	    Point(NOMINAL_WIDTH, NOMINAL_HEIGHT));
    Plot::setWrapArea(wa);
    wrap = false;

    // Initial drawing color
    drawColor = {255, 255, 255, 255};  // White

    // Start time accounting
    counterFrequency = (double)SDL_GetPerformanceFrequency();
    startTime = (double)SDL_GetPerformanceCounter() / counterFrequency;
    prevTime = startTime;
    nextTime = startTime + 1.0 / FPS_DEFAULT;
    frameNo = 0;
    pauseMode = false;

    return true;
}

void Plot::setFullScreen(bool enable)
{
    if (enable && !fullScreen) {
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	fullScreen = true;
    }

    if (!enable && fullScreen) {
	SDL_SetWindowFullscreen(window, 0);
	fullScreen = false;
    }
}

bool Plot::getFullScreen()
{
    return fullScreen;
}

// Pause works by making Plot::dt() always return 0.0, stopping all
// motion. Other types of updates (e.g. ship firing) can also be
// inhibited by checking if Plot::dt() == 0.0.
void Plot::setPauseMode(bool enable)
{
    pauseMode = enable;
}

bool Plot::getPauseMode()
{
    return pauseMode;
}

void Plot::term()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Plot::suspend()
{
    // Temporarily hide window
    SDL_HideWindow(window);

    // Suspend process
    raise(SIGTSTP);

    // Recover window
    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);

    // RaiseWindow alone doesn't get back focus
    SDL_SetWindowInputFocus(window);
}

// In vsync mode, SDL_RenderPresent() returns synchronously to vsync.
// In the absence of vsync mode, implement a delay to limit frame rate.
void Plot::sync()
{
    frameNo++;

#ifdef __EMSCRIPTEN__
    drawLetterbox();	// Screen clears each frame with Emscripten but not X11
#endif

    SDL_RenderPresent(renderer);

    double curTime = (double)SDL_GetPerformanceCounter() / counterFrequency;

    if (!vsyncSupported) {
	double remainingTime;

	do {
	    nextTime += 1.0 / FPS_DEFAULT;
	    remainingTime = nextTime - curTime;
	} while (remainingTime < 0.0);

	int delayMs = (Uint32)(remainingTime * 1000.0);
	if (delayMs > 0)
	    SDL_Delay(delayMs);
    }

    deltaTime = curTime - prevTime;
    prevTime = curTime;
}

// Return elapsed time between previous and current update.
// This value should be used for calculating velocities and
// accelerations so that distances "integrate" properly, regardless of
// frame rate and in the face of frame rate instability.
double Plot::dt()
{
    return pauseMode ? 0.0 : deltaTime;
}

double Plot::frameTime()
{
    double curTime = (double)SDL_GetPerformanceCounter() / counterFrequency;
    double runTime = curTime - startTime;

    return runTime / (double)frameNo;
}

void Plot::printFPS()
{
    printf("Average over %d frames is %.2lf FPS\n", frameNo, 1.0 / frameTime());
}

void Plot::setMode(enum Mode mode)
{
    switch (mode) {
    case SET:
        drawColor = {255, 255, 255, 255};  // White
        break;
    case RES:
        drawColor = {0, 0, 0, 255};        // Black
        break;
    }

    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
}

void Plot::clearBlack()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Plot::clearWhite()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
}

Vect Plot::getSize()
{
    return Vect(NOMINAL_WIDTH, NOMINAL_HEIGHT);
}

void Plot::setWrapArea(const Rect& wrapArea)
{
    wrapFrac.ul.x = wrapArea.ul.x / NOMINAL_WIDTH;
    wrapFrac.ul.y = wrapArea.ul.y / NOMINAL_HEIGHT;
    wrapFrac.lr.x = wrapArea.lr.x / NOMINAL_WIDTH;
    wrapFrac.lr.y = wrapArea.lr.y / NOMINAL_HEIGHT;
}

void Plot::getWrapArea(Rect *wrapArea)
{
    wrapArea->ul.x = wrapFrac.ul.x * NOMINAL_WIDTH;
    wrapArea->ul.y = wrapFrac.ul.y * NOMINAL_HEIGHT;
    wrapArea->lr.x = wrapFrac.lr.x * NOMINAL_WIDTH;
    wrapArea->lr.y = wrapFrac.lr.y * NOMINAL_HEIGHT;
}

void Plot::setWrap(bool w)
{
    wrap = w;
}

// Similar triangle macro for clipping
#define ClipCase(left, middle, right, start, end) \
    ((left) == (right) ? ((start) + (end)) / 2 : \
     (((start) * ((right) - (middle)) + \
       (end) * ((middle) - (left))) / ((right) - (left))))

static void DrawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer,
		       renderX + x1, renderY + y1,
		       renderX + x2, renderY + y2);

    // Approximate thick lines (SDL2 doesn't support line thickness)
    if (lineThickness > 1) {
	int dx, dy;

	if (abs(x2 - x1) > abs(y2 - y1)) {
	    dx = 0;	// Line is more horizontal (slope < 1)
	    dy = 1;
	} else {
	    dx = 1;	// Line is more vertical (slope >= 1)
	    dy = 0;
	}

	SDL_RenderDrawLine(renderer,
			   renderX + x1 - dx, renderY + y1 - dy,
			   renderX + x2 - dx, renderY + y2 - dy);

	if (lineThickness > 2)
	    SDL_RenderDrawLine(renderer,
			       renderX + x1 + dx, renderY + y1 + dy,
			       renderX + x2 + dx, renderY + y2 + dy);
    }
}

static void LineClip(int fx, int fy, int tx, int ty,
                    int minX, int minY, int maxX, int maxY)
{
    // If line is entirely off any side, draw nothing
    if ((fx < minX && tx < minX) || (fx > maxX && tx > maxX) ||
        (fy < minY && ty < minY) || (fy > maxY && ty > maxY))
        return;

    // Hack off pieces that cross outside clipping window
    if (fx < minX) {
        fx = minX;
        fy = ClipCase(fx, minX, tx, fy, ty);
    }
    if (tx < minX) {
        tx = minX;
        ty = ClipCase(fx, minX, tx, fy, ty);
    }
    if (fx > maxX) {
        fx = maxX;
        fy = ClipCase(fx, maxX, tx, fy, ty);
    }
    if (tx > maxX) {
        tx = maxX;
        ty = ClipCase(fx, maxX, tx, fy, ty);
    }
    if (fy < minY) {
        fx = ClipCase(fy, minY, ty, fx, tx);
        fy = minY;
    }
    if (ty < minY) {
        tx = ClipCase(fy, minY, ty, fx, tx);
        ty = minY;
    }
    if (fy > maxY) {
        fx = ClipCase(fy, maxY, ty, fx, tx);
        fy = maxY;
    }
    if (ty > maxY) {
        tx = ClipCase(fy, maxY, ty, fx, tx);
        ty = maxY;
    }

    DrawLine(fx, fy, tx, ty);
}

static void LineWrap(int fx, int fy, int tx, int ty,
                    int minX, int minY, int maxX, int maxY)
{
    // If line is entirely off a side, wrap both ends around
    if (fx < minX && tx < minX) {
        LineWrap(fx + maxX - minX + 1, fy, tx + maxX - minX + 1, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (fx > maxX && tx > maxX) {
        LineWrap(fx - maxX + minX - 1, fy, tx - maxX + minX - 1, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (fy < minY && ty < minY) {
        LineWrap(fx, fy + maxY - minY + 1, tx, ty + maxY - minY + 1,
                minX, minY, maxX, maxY);
        return;
    }

    if (fy > maxY && ty > maxY) {
        LineWrap(fx, fy - maxY + minY - 1, tx, ty - maxY + minY - 1,
                minX, minY, maxX, maxY);
        return;
    }

    // Break down cases that cross clipping boundary edges
    if (fx < minX) {
        LineWrap(fx, fy, minX - 1, ClipCase(fx, minX - 1, tx, fy, ty),
                minX, minY, maxX, maxY);
        LineWrap(minX, ClipCase(fx, minX, tx, fy, ty), tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (tx < minX) {
        LineWrap(fx, fy, minX, ClipCase(fx, minX, tx, fy, ty),
                minX, minY, maxX, maxY);
        LineWrap(minX - 1, ClipCase(fx, minX - 1, tx, fy, ty), tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (fx > maxX) {
        LineWrap(fx, fy, maxX + 1, ClipCase(fx, maxX + 1, tx, fy, ty),
                minX, minY, maxX, maxY);
        LineWrap(maxX, ClipCase(fx, maxX, tx, fy, ty), tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (tx > maxX) {
        LineWrap(fx, fy, maxX, ClipCase(fx, maxX, tx, fy, ty),
                minX, minY, maxX, maxY);
        LineWrap(maxX + 1, ClipCase(fx, maxX + 1, tx, fy, ty), tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (fy < minY) {
        LineWrap(fx, fy, ClipCase(fy, minY - 1, ty, fx, tx), minY - 1,
                minX, minY, maxX, maxY);
        LineWrap(ClipCase(fy, minY, ty, fx, tx), minY, tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (ty < minY) {
        LineWrap(fx, fy, ClipCase(fy, minY, ty, fx, tx), minY,
                minX, minY, maxX, maxY);
        LineWrap(ClipCase(fy, minY - 1, ty, fx, tx), minY - 1, tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (fy > maxY) {
        LineWrap(fx, fy, ClipCase(fy, maxY + 1, ty, fx, tx), maxY + 1,
                minX, minY, maxX, maxY);
        LineWrap(ClipCase(fy, maxY, ty, fx, tx), maxY, tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    if (ty > maxY) {
        LineWrap(fx, fy, ClipCase(fy, maxY, ty, fx, tx), maxY,
                minX, minY, maxX, maxY);
        LineWrap(ClipCase(fy, maxY + 1, ty, fx, tx), maxY + 1, tx, ty,
                minX, minY, maxX, maxY);
        return;
    }

    // Line is fully on-screen
    DrawLine(fx, fy, tx, ty);
}

void Plot::line(double fx, double fy, double tx, double ty)
{
    // Scale from virtual coordinates to actual render area
    int ifx = (int)(fx * renderW / NOMINAL_WIDTH);
    int ify = (int)(fy * renderH / NOMINAL_HEIGHT);
    int itx = (int)(tx * renderW / NOMINAL_WIDTH);
    int ity = (int)(ty * renderH / NOMINAL_HEIGHT);

    if (wrap) {
        int clip_ulx = (int)(renderW * wrapFrac.ul.x);
        int clip_uly = (int)(renderH * wrapFrac.ul.y);
        int clip_lrx = (int)(renderW * wrapFrac.lr.x);
        int clip_lry = (int)(renderH * wrapFrac.lr.y);

        LineWrap(ifx, ify, itx, ity,
                clip_ulx, clip_uly, clip_lrx, clip_lry);
    } else {
        LineClip(ifx, ify, itx, ity,
                0, 0, renderW - 1, renderH - 1);
    }
}

// Poll for window events such as resize and keyboard events that
// correspond valid game controls. Game controls are propagated to the
// Buttons module.
void Plot::pollEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
	case SDL_QUIT:
	    Button::press(Button::quit);
	    break;

	case SDL_WINDOWEVENT:
	    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
		clearBlack();
		handleResize(event.window.data1, event.window.data2);
	    }
	    break;

	case SDL_KEYDOWN:
	case SDL_KEYUP: {
	    // Ignore key repeat events
	    if (event.key.repeat)
		break;

	    // Allow CapsLock-as-Control
	    if (event.key.keysym.sym == SDLK_CAPSLOCK)
		capsLockDown = (event.type == SDL_KEYDOWN);

	    bool keyDown = (event.type == SDL_KEYDOWN);
	    bool shiftDown = (event.key.keysym.mod & KMOD_SHIFT);
	    bool ctrlDown = ((event.key.keysym.mod & KMOD_CTRL) ||
			     capsLockDown);

	    switch (event.key.keysym.sym) {
	    case SDLK_z:
		if (keyDown && ctrlDown)
		    Button::press(Button::suspend);
		else {
		    if (keyDown)
			Button::press(Button::rotateLeft);
		    else
			Button::release(Button::rotateLeft);
		}
		break;

	    case SDLK_x:
		if (keyDown)
		    Button::press(Button::rotateRight);
		else
		    Button::release(Button::rotateRight);
		break;

	    case SDLK_PERIOD:
		if (keyDown)
		    Button::press(Button::thrust);
		else
		    Button::release(Button::thrust);
		break;

	    case SDLK_SLASH:
		if (keyDown) {
		    if (shiftDown)
			Button::press(Button::toggleHelp);
		    else
			Button::press(Button::fire);
		}
		break;

	    case SDLK_h:
		if (keyDown)
		    Button::press(Button::toggleHelp);
		break;

	    case SDLK_SPACE:
		if (keyDown)
		    Button::press(Button::hyperspace);
		break;

	    case SDLK_p:
		if (keyDown)
		    Button::press(Button::togglePause);
		break;

	    case SDLK_ESCAPE:
	    case SDLK_q:
		if (keyDown)
		    Button::press(Button::quit);
		break;

	    case SDLK_c:
		if (keyDown && ctrlDown)
		    Button::press(Button::quit);
		break;

	    case SDLK_f:
		if (keyDown)
		    Button::press(Button::fullScreen);
		break;

	    case SDLK_1:
		if (keyDown) {
		    if (shiftDown)
			// Exclamation mark prints average FPS
			Button::press(Button::printFPS);
		    else
			Button::press(Button::start1);
		}
		break;

	    case SDLK_2:
		if (keyDown)
		    Button::press(Button::start2);
		break;

	    case SDLK_3:
		if (keyDown)
		    Button::press(Button::start3);
		break;

	    case SDLK_s:
		if (keyDown)
		    Button::press(Button::toggleSound);
		break;
	    }
	    break;
	}
        }
    }
}
