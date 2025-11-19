#include "game.hpp"
#include "sound.hpp"
#include "button.hpp"
#include "title.hpp"
#include "highlist.hpp"
#include "help.hpp"
#include "speaker.hpp"

Game::Game(int _player)
{
    score = new Score();
    scoreThreshold = EXTRASHIPPOINTS;
    ship = new Ship(false);
    extras = new Extras(STARTLIVES);
    ghost = new Ghost();
    rocks = new Rocks();
    alien = new Alien();

    player = _player;
    wave = 0;
    state = State::GHOST;
    deetTimer = deetInterval();
    deetHigh = false;
}

Game::~Game()
{
    delete alien;
    delete rocks;
    delete ghost;
    delete extras;
    delete ship;
    delete score;
}

struct missileCollData {
    int score;
    Alien *alien;
    Rocks *rocks;
};

static bool missileColl(Missile *m, void *rock)
{
    missileCollData *md = (missileCollData *)rock;

    int score = md->rocks->hitMissile(m);

    if (score == 0 && md->alien->alive())
	score = md->alien->hitMissile(m);

    if (score != 0) {
	m->off();
	md->score += score;
    }

    return true;
}

void Game::startAttract()
{
    state = State::ATTRACT;

    rocks->setWaveInc(0);
    rocks->start();
    rocks->on();
    alien->on();
    score->on();
}

void Game::stopAttract()
{
    rocks->off();
    (void)rocks->update();

    alien->off();
    alien->update(true, NULL, 0, wave);
}

// Returns false when ship dies
bool Game::update()
{
    if (rocks->update())
	if (state != State::ATTRACT)
	    wave++;

    Sprite *targets[2];
    // Pick a primary target (ship) and secondary target (random rock).
    // If either target is unavailable, it will be NULL.
    targets[0] = ship->isOn() ? ship->getSprite() : NULL;
    targets[1] = rocks->getRandom();

    bool inhibitNew = (state != State::ATTRACT && !ship->isOn());
    alien->update(inhibitNew, targets, ARRAYSIZE(targets), wave);
    alien->checkCollisions(rocks, NULL, NULL);

    extras->update();
    score->update();

    switch (state) {
    case State::ATTRACT:
	break;

    case State::GHOST:
	ghost->update();

	// Move to active play when player actives any ship control
	if (Button::isDown(Button::rotateLeft) ||
	    Button::isDown(Button::rotateRight) ||
	    Button::isDown(Button::thrust) ||
	    Button::isDown(Button::fire) ||
	    Button::isDown(Button::hyperspace)) {
	    ghost->stop();
	    ship->start();
	    state = State::ACTIVE;
	    break;
	}
	break;

    case State::ACTIVE:
	missileCollData md;

	ship->update();

	// Check for ship collisions

	md.score = 0;
	md.alien = alien;
	md.rocks = rocks;

	MList *ml = ship->getMissiles();
	ml->enumerate(missileColl, (void *)&md);

	score->inc(md.score);

	if (ship->isOn()) {
	    bool shipHit = false;

	    int consolationScore = rocks->hitSprite(ship->getSprite());
	    if (consolationScore != 0) {
		score->inc(consolationScore);
		shipHit = true;
	    }

	    consolationScore = 0;
	    if (alien->checkCollisions(rocks, ship, &consolationScore))
		shipHit = true;

	    score->inc(consolationScore);

	    if (shipHit)
		ship->explode();
	}

	// Ship takes a while to die after exploding, even after debris
	// dies out, so that the next turn is delayed appropriately.
	if (ship->isDead())
	    return false;

	if (score->get() >= scoreThreshold) {
	    scoreThreshold += EXTRASHIPPOINTS;
	    extras->inc();
	    Sound::play(Sound::extraShip);
	}

	// Make "deet" background sound as long as ship is alive and playing
	if (ship->isOn()) {
	    deetTimer -= Plot::dt();

	    if (deetTimer <= 0.0) {
		Sound::play(deetHigh ? Sound::highDeet : Sound::lowDeet);
		deetHigh = !deetHigh;
		deetTimer = deetInterval();
	    }
	}
    }

    return true;
}

void Game::startTurn()
{
    // Clear controls that might have been applied during Attract
    Button::clearAll();

    extras->dec();
    extras->on();
    extras->update();

    rocks->on();
    alien->on();
    score->on();

    ghost->start(player + 1);

    Button::clearAll();

    deetTimer = deetInterval();
    deetHigh = false;

    state = State::GHOST;
}

void Game::stopTurn()
{
    score->off();
    score->update();

    ghost->stop();

    ship->off();
    ship->update();

    extras->off();
    extras->update();

    rocks->off();
    if (rocks->update())
	wave++;

    alien->off();
    alien->update(true, NULL, 0, wave);
}
