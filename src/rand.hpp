#ifndef rand_hpp
#define rand_hpp

#include <time.h>

struct Rand {
    // Seed generator
    static inline void init() {
	srandom((unsigned int)time(NULL));
    }

    // 0 <= Rand::natural(n) < n
    static inline int natural(int n) {
	return ((int)(random() >> 4)) % n;
    }

    // 0.0 <= Rand::frac(n) <= 1.0
    static inline double frac() {
	return (double)natural(100001) / 100000.0;
    }

    static inline float fracf() {
	return (float)natural(100001) / 100000.0f;
    }

    // min <= Rand::range(min, max) <= max
    static inline double range(double min, double max) {
	return min + frac() * (max - min);
    }

    // min <= Rand::rangef(min, max) <= max
    static inline float rangef(float min, float max) {
	return min + fracf() * (max - min);
    }
};

#endif // !rand_hpp
