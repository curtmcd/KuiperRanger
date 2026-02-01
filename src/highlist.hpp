#ifndef highlist_hpp
#define highlist_hpp

#include "text.hpp"
#include "param.hpp"

struct HighList {
    static int getBest();

    // Insert a score into high score list if it is a top score
    static void record(const char *name, int score);

    // High score graphics
    static void init();
    static void term();

    static void on();
    static void off();
    static void update();

    struct highEntry {
	char name[NICKMAXLEN + 1];
	int score;
	time_t timeStamp;
	bool operator<(const highEntry& other) const {
	    return score > other.score;
	}
    };

private:
    static std::vector<highEntry> highs;
    static std::vector<Text *> texts;
    static bool modified;

    static void load();
    static void save();
};

#endif // !highlist_hpp
