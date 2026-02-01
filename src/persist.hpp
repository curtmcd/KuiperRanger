#ifndef persist_hpp
#define persist_hpp

#include <string>

// The game pickles high score data into a single C++ string that can be
// saved and reloaded later. This module should implement a trivial
// platform-specific generic store capable of storing and retrieving a
// long C++ string.

struct Persist {
    // On success, return true with valid buf; else return false
    static bool load(std::string& buf);

    // Return true on success
    static bool save(const std::string& buf);
};

#endif // !persist_hpp
