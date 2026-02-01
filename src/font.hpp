#ifndef font_hpp
#define font_hpp

#include "shape.hpp"

struct Font {
    static void init();
    static void term();

    static Font *fontRegular;
    static Font *fontBold;
    static Font *fontItalic;
    static Font *fontBoldItalic;

    Shape *getChar(int ch) { return ch < 0 || ch > 127 ? NULL : chars[ch]; }

    Vect getSpacing() { return spacing; }

private:
    Font(bool bold = false, bool italic = false);
    ~Font();

    std::vector<Shape *> chars;		// indexed by ASCII code

    Vect spacing;		// Offset to next char (x) and line (y)
};

#endif // !font_hpp
