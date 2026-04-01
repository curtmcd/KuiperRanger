#ifndef rand_hpp
#define rand_hpp

#include <random>

struct Rand {
    // Initialize RNG with random seed
    static void init();
    static void term();

    // 0 <= Rand::natural(n) < n
    static inline int natural(int n) {
        std::uniform_int_distribution<int> dist(0, n - 1);
        return dist(*gen);
    }

    // 0.0 <= Rand::frac() <= 1.0
    static inline double frac() {
        return natural(100001) / 100000.0;
    }

    // 0.0 <= Rand::fracf() <= 1.0
    static inline float fracf() {
        return (float)frac();
    }

    // min <= Rand::range(min, max) <= max
    static inline double range(double min, double max) {
        return min + frac() * (max - min);
    }

    // min <= Rand::rangef(min, max) <= max
    static inline float rangef(float min, float max) {
        return min + fracf() * (max - min);
    }

private:
    static std::minstd_rand *gen;
};

#endif // !rand_hpp
