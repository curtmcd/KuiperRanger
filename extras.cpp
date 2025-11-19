#include "extras.hpp"
#include "param.hpp"
#include "vect.hpp"
#include "plot.hpp"

Extras::Extras(int startLives)
{
    Vect pos;

    pos.x = (double)EXTRAX;
    pos.y = (double)(SCOREBARH * 2 / 3);

    lives = startLives;
    show = false;

    for (int i = 0; i < MAXLIVES; i++) {
	Vect spacing;
        Ship *sh = new Ship(true);
        Sprite *sp;

        sh->start();
        sh->off();

	sp = sh->getSprite();

	sp->setAngle(270);
	sp->setAngularVelocity(-EXTRAROTSPEED);
        sp->setPos(pos);
	sp->setWrap(false);

	spacing.x = sp->getRadius() * 1.75;
	spacing.y = 0.0;

	pos += spacing;

        ships[i] = sh;
    }
}

Extras::~Extras()
{
    for (int i = 0; i < MAXLIVES; i++)
	delete ships[i];
}

void Extras::update()
{
    Ship *sh0 = ships[0];

    for (int i = 0; i < lives; i++) {
	Ship *sh = ships[i];

	if (show && !sh->isOn()) {
	    sh->on();
	    if (i > 0)		// synchronize angle with that of first ship
		sh->getSprite()->setAngle(sh0->getSprite()->getAngle());
	}

	if (!show && sh->isOn())
	    sh->off();

	sh->update();
    }

    for (int i = lives; i < MAXLIVES; i++) {
	Ship *sh = ships[i];

	if (sh->isOn())
	    sh->off();

	sh->update();
    }
}
