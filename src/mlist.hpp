#ifndef mlist_hpp
#define mlist_hpp

#include "missile.hpp"

struct MListElement {
    MListElement();
    ~MListElement();

    struct Missile *missile;
    struct MListElement *next;
};

struct MList {
    MList();
    ~MList();

    void update();
    void fire(const Point& frompos, const Vect& vel, double timeout, double radius);
    void on();
    void off();

    template <typename Callback>
    void enumerate(Callback proc) {
	for (MListElement *me = queue; me != NULL; me = me->next)
	    proc(me->missile);
    }

    int getCount() const { return count; }
    bool empty() const { return count == 0; }

private:
    struct MListElement *queue;
    int count, show;
};

#endif // !mlist_hpp
