#include <unistd.h>
#include <pwd.h>

#include "machine.hpp"
#include "game.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "sound.hpp"
#include "help.hpp"
#include "highlist.hpp"
#include "title.hpp"
#include "speaker.hpp"
#include "paused.hpp"

static bool soundIsOn = false;
static bool soundOverride = false;

static void getNickname(char name[NICKMAXLEN + 1], int player)
{
    struct passwd *pwd;
    const char *s = NULL;

    switch (player) {
    case 0:
	s = getenv("KR_NICK1");
	break;
    case 1:
	s = getenv("KR_NICK2");
	break;
    case 2:
	s = getenv("KR_NICK3");
	break;
    }

    if (s == NULL)
	s = getenv("KR_NICK");
    if (s == NULL)
	s = getenv("USER");
    if (s == NULL)
	s = getenv("LOGNAME");
    if (s == NULL && (pwd = getpwuid(getuid())) != NULL)
	s = pwd->pw_name;
    if (s == NULL)
	s = "anon";

    strncpy(name, s, NICKMAXLEN);
    name[NICKMAXLEN] = '\0';
}

static Game *attractGame;
static Game *activeGames[MAXPLAYERS];

static int activePlayer;

static void startAttract()
{
    attractGame = new Game(0);
    attractGame->startAttract();

    Title::on();
    HighList::on();
    Help::off();

    soundOverride = true;
    Sound::off();

    Speaker::setStyle(soundIsOn ? Speaker::soundOn : Speaker::soundOff);
    Speaker::stayOn(true);

    // Clear controls that might have been applied before Attract
    Button::clearAll();
}

static void stopAttract()
{
    attractGame->stopAttract();

    delete attractGame;
    attractGame = NULL;

    Title::off();
    Title::update();

    HighList::off();
    HighList::update();

    Help::off();
    Help::update();

    Speaker::stayOn(false);

    soundOverride = false;
    if (soundIsOn)
	Sound::on();
}

void Machine::init()
{
    soundIsOn = false;
    soundOverride = true;
    Sound::off();

    startAttract();
}

void Machine::term()
{
}

// Return false if game should quit
static bool updateAttract()
{
#ifndef __EMSCRIPTEN__
    // Quit from web page isn't supported (user can press P to pause)
    if (Button::isDown(Button::quit, true)) {
	stopAttract();
	return false;
    }
#endif // __EMSCRIPTEN__

    if (Button::isDown(Button::toggleHelp, true)) {
	if (Help::isOn()) {
	    Help::off();
	    Title::on();
	    HighList::on();
	} else {
	    Title::off();
	    HighList::off();
	    Help::on();
	}
    }

    Title::update();
    Help::update();
    HighList::update();

    attractGame->update();

    int nPlayers = 0;

    if (Button::isDown(Button::start3, true))
	nPlayers = 3;
    if (Button::isDown(Button::start2, true))
	nPlayers = 2;
    if (Button::isDown(Button::start1, true))
	nPlayers = 1;

    if (nPlayers > 0 && !Plot::getPauseMode()) {
	stopAttract();

	for (int i = 0; i < nPlayers; i++)
	    activeGames[i] = new Game(i);

	activePlayer = 0;
	activeGames[activePlayer]->startTurn();
    }

    return true;
}

static void updateTurn()
{
    bool playerOut = false;
    bool turnOver = false;

    if (Button::isDown(Button::quit, true)) {
	Plot::setPauseMode(false);
	Paused::off();

	turnOver = true;
	playerOut = true;
    }

    Game *g = activeGames[activePlayer];

    if (!g->update()) {
	turnOver = true;
	if (g->livesRemaining() == 0)
	    playerOut = true;
    }

    if (turnOver) {
	g->stopTurn();

	if (playerOut) {
	    char name[NICKMAXLEN + 1];
	    getNickname(name, activePlayer);

	    HighList::record(name, g->currentScore());

	    delete g;
	    activeGames[activePlayer] = NULL;
	}

	// Go on to the next player, if any
	for (int i = 0; i < MAXPLAYERS; i++) {
	    if (++activePlayer == MAXPLAYERS)
		activePlayer = 0;

	    if (activeGames[activePlayer] != NULL)
		break;
	}

	g = activeGames[activePlayer];
	if (g) {
	    g->startTurn();
	} else {
	    // Return to Attract mode
	    startAttract();
	}
    }
}

// Returns false if the program should exit
bool Machine::update()
{
    if (Button::isDown(Button::printFPS, true))
	Plot::printFPS();

    if (Button::isDown(Button::suspend, true))
	Plot::suspend();

    if (Button::isDown(Button::toggleSound, true)) {
	soundIsOn = !soundIsOn;
	if (soundIsOn && !soundOverride)
	    Sound::on();
	else
	    Sound::off();

	Speaker::setStyle(soundIsOn ? Speaker::soundOn : Speaker::soundOff);
	Speaker::flash(SPEAKERFLASH);
    }

    if (Button::isDown(Button::fullScreen, true))
	Plot::setFullScreen(!Plot::getFullScreen());

    if (Button::isDown(Button::togglePause, true)) {
	if (Plot::getPauseMode()) {
	    Plot::setPauseMode(false);
	    Paused::off();
	    if (soundIsOn && !soundOverride)
		Sound::on();
	} else {
	    Plot::setPauseMode(true);
	    Paused::on();
	    // Mute all sound (including continuous ones like alienMotor)
	    Sound::off();
	}
    }

    Speaker::update();
    Paused::update();

    if (attractGame != NULL) {
	if (!updateAttract())
	    return false;
    } else
	updateTurn();

    return true;
}
