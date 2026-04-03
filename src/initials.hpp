#ifndef initials_hpp
#define initials_hpp

#include "param.hpp"
#include "font.hpp"
#include "text.hpp"

struct Initials {
    Initials();
    ~Initials();

    void start();
    bool update(char *nickReturn);
    void stop();

    Text *instrText;

    char nick[NICKMAXLEN + 1];
    Text *nickText;

    int nickPos;
    double nickClock;
    double nickExpire;
};

#endif // !initials_hpp
