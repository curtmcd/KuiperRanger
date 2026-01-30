#ifndef linefont_hpp
#define linefont_hpp

#include "shape.hpp"

struct Linefont {
    Linefont(double scale, bool bold = false, bool italic = false);
    ~Linefont();

    Shape *getChar(int ch) { return ch < 0 || ch > 127 ? NULL : chars[ch]; }

    Vect getCharSpacing() { return charSpacing; }
    Vect getLineSpacing() { return lineSpacing; }

private:
    Shape *chars[128];

    Vect charSpacing;      // Offset to next char pos
    Vect lineSpacing;      // Offset to next line
};

#endif // !linefont_hpp
