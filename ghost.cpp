#include "ghost.hpp"

Ghost::Ghost()
{
    text = new Text();
    ghostShip = new Ship(true);
    font = new Linefont(PERCENT(500), false);
    text->setFont(font);
}

Ghost::~Ghost()
{
    delete text;
    delete font;
    delete ghostShip;
}

void Ghost::start(int player)
{
    ghostShip->start();

    char buf[80];
    sprintf(buf, "Player %d", player);
    text->set(buf);

    Vect textSize = text->getSize();
    Vect lineSpacing = font->getLineSpacing();
    Point textPos = ghostShip->getPos() + lineSpacing * 2 - textSize / 2;
    text->setPos(textPos);

    text->on();

    ghostClock = 0.0;
}

void Ghost::stop()
{
    ghostShip->off();
    ghostShip->update();

    text->off();
    text->update();
}

void Ghost::update()
{
    text->update();

    ghostClock += Plot::dt();
    if (ghostClock < GHOSTBLINK * GHOSTDUTY)
	ghostShip->on();
    else
	ghostShip->off();
    if (ghostClock >= GHOSTBLINK)
	ghostClock = 0.0;

    ghostShip->update();
}
