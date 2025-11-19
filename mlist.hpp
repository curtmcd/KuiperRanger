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
    void enumerate(bool (*proc)(Missile *, void *), void *rock);
    void fire(const Vect& frompos, const Vect& vel, double timeout, double radius);
    void on();
    void off();

    int getCount() const { return count; }
    bool empty() const { return count == 0; }

private:
    struct MListElement *queue;
    int count, show;
};

#endif // !mlist_hpp
