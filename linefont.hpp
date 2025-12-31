#ifndef linefont_hpp
#define linefont_hpp

#include "shape.hpp"

#define LINEFONT_SCALE1_CHAR_W	6
#define LINEFONT_SCALE1_CHAR_H	8

struct Linefont {
    Linefont(double scale, bool italic);
    ~Linefont();

    Shape *getChar(int ch) { return ch < 0 || ch > 127 ? NULL : chars[ch]; }

    Vect getCharSize() {  return charSize; }
    Vect getCharSpacing() { return charSpacing; }
    Vect getLineSpacing() { return lineSpacing; }

private:
    Shape *chars[128];

    Vect charSize;         // (Unrotated) character size
    Vect charSpacing;      // Offset to next char pos
    Vect lineSpacing;      // Offset to next line
};

#endif // !linefont_hpp
