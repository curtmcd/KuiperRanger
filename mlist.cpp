#include "type.hpp"
#include "mlist.hpp"

MListElement::MListElement()
{
    missile = new Missile();
    next = NULL;
}

MListElement::~MListElement()
{
    delete missile;
}

MList::MList()
{
    queue = NULL;
    show = false;
    count = 0;
}

MList::~MList()
{
    MListElement *me, *next;

    for (me = queue; me != NULL; me = next) {
	next = me->next;
	delete me;
    }
}

void MList::update()
{
    MListElement *me;

    for (MListElement **prev = &queue; (me = *prev) != NULL; ) {
	me->missile->update();

	if (!me->missile->isOn()) {
	    *prev = me->next;
	    delete me;
	    count--;
	} else
	    prev = &me->next;
    }
}

void MList::fire(const Point &fromPos, const Vect& vel, double timeout, double radius)
{
    MListElement *me = new MListElement;

    // Start missile at outside radius of the firing object, less a
    // frame's distance to account for sprite not being on yet.  Well,
    // maybe half a frame's distance so it doesn't touch the ship's tip.
    Vect u = vel;
    u.unitize();
    me->missile->setPos(fromPos + u * radius - vel * (Plot::frameTime() / 2));

    me->missile->setVel(vel);
    me->missile->setTimeout(timeout);

    if (show)
	me->missile->on();
    else
	me->missile->off();

    me->next = queue;
    queue = me;
    count++;
}

void MList::enumerate(bool (*proc)(Missile *, void *), void *rock)
{
    for (MListElement *me = queue; me != NULL; me = me->next)
	if (!(*proc)(me->missile, rock))
	    break;
}

void MList::on()
{
    show = true;

    for (MListElement *me = queue; me != NULL; me = me->next)
	me->missile->on();
}

void MList::off()
{
    show = false;

    for (MListElement *me = queue; me != NULL; me = me->next)
	me->missile->off();
}
