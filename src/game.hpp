#ifndef game_hpp
#define game_hpp

#include "ship.hpp"
#include "ghost.hpp"
#include "extras.hpp"
#include "rocks.hpp"
#include "alien.hpp"
#include "score.hpp"

struct Game {
    Game(int _player);
    ~Game();

    bool update();

    void startAttract();
    void stopAttract();

    void startTurn();
    void stopTurn();

    double deetInterval() {	// current interval in sec
	double t = DEETINITPERIOD * std::pow(DEETSPEEDUP, (double)MAX(wave - 1, 0));
	return MAX(t, DEETMINPERIOD);
    };

    int livesRemaining() { return extras->get(); }
    int currentScore() { return score->get(); }

    Score *score;
    int scoreThreshold;
    Ship *ship;
    Extras *extras;
    Ghost *ghost;
    Rocks *rocks;
    Alien *alien;

    enum class State {
	ATTRACT,
	GHOST,
	ACTIVE
    };

    int player, wave;
    enum State state;
    double deetTimer;		// time until next deet
    bool deetHigh;		// whether next deet tone is high or low
};

#endif // !game_hpp
