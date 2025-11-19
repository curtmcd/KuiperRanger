#ifndef type_hpp
#define type_hpp

#include <stdio.h>

#include <cmath>
#include <cassert>
#include <cstring>
#include <cstdlib>

static const double PI = 3.14159265358979323846;

#define ARRAYSIZE(a)		((int)(sizeof (a) / sizeof ((a)[0])))

template<class T> T MIN(T a, T b) { return a <= b ? a : b; }
template<class T> T MAX(T a, T b) { return a >= b ? a : b; }
template<class T> T ABS(T a) { return a >= 0 ? a : -a; }

inline double PERCENT(double pct) { return pct / 100.0; }

#define DAY_SEC			(24 * 60 * 60)
#define WEEK_SEC		(7 * DAY_SEC)
#define YEAR_SEC		(52 * WEEK_SEC)

#endif // !type_hpp
