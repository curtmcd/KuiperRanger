#ifndef type_hpp
#define type_hpp

// For Windows, the following should be defined in the build:
//   NOMINMAX _CRT_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include <iterator>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstring>
#include <cstdlib>

inline double PERCENT(double pct) { return pct / 100.0; }

static const double PI = 3.1415926535897932;
static const float PIf = 3.1415926f;

#endif // !type_hpp
