#ifndef rand_hpp
#define rand_hpp

#include <time.h>

namespace Rand {
    // Seed generator
    inline void init() { srandom((unsigned int)time(NULL)); }
    // 0 <= Rand::natural(n) < n
    inline int natural(int n) { return ((int)(random() >> 4)) % n; }
    // 0.0 <= Rand::frac(n) <= 1.0
    inline double frac() { return (double)natural(100001) / 100000.0; }
    inline float fracf() { return (float)natural(100001) / 100000.0f; }
    // min <= Rand::range(min, max) <= max
    inline double range(double min, double max) { return min + frac() * (max - min); }
    inline float rangef(float min, float max) { return min + fracf() * (max - min); }
};

#endif // !rand_hpp
