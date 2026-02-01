#include "ghost.hpp"

Ghost::Ghost()
{
    text = new Text();
    ghostShip = new Ship(true);
    ghostClock = 0.0;
}

Ghost::~Ghost()
{
    delete text;
    delete ghostShip;
}

void Ghost::start(int player)
{
    ghostShip->start();

    char buf[80];
    sprintf(buf, "Player %d", player);
    text->set(buf);

    text->setScale(PERCENT(500));

    Vect textSize = text->getSize();
    Point textPos = ghostShip->getPos() + Vect(0.0, textSize.y);
    text->setPos(textPos);

    text->on();

    ghostClock = 0.0;
}

void Ghost::stop()
{
    ghostShip->off();
    text->off();
}

void Ghost::update()
{
    text->update();

    ghostClock += Plot::dt();

    if (ghostClock < GHOSTBLINK * GHOSTDUTY) {
	ghostShip->on();
	ghostShip->update();
    } else
	ghostShip->off();

    if (ghostClock >= GHOSTBLINK)
	ghostClock = 0.0;
}
