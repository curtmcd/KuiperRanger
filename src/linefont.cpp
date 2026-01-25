#include "param.hpp"
#include "linefont.hpp"

// Font character spaces are 7 by 11 units.  These
// numbers are the appropriate char and line spacings.
// Uppercase characters are drawn in a 6 by 8
// area in the upper left of the character space.
// Lowercase characters are drawn in a 6 by 10
// area in the upper left, due to descenders.
//
// Descenders hang down an additional 2 units.
// The font character is drawn with the left side
// of the char at the specified x, and the baseline
// at the specified y.
//
// Chars are Shapes which may be used as Sprite
// shapes.  Thus chars may be scaled up, moving with,
// a constant velocity, rotated or spinning, etc.

static Line C_unknown[] = {
};

static Line C_space[] = {
};

static Line C_exclam[] = {
    { Point(3, 8), Point(3, 2) },
    { Point(3, 0), Point(3, 0) },
};

static Line C_dquote[] = {
    { Point(1, 8), Point(1, 6) },
    { Point(5, 8), Point(5, 6) },
};

static Line C_pound[] = {
    { Point(1, 8), Point(1, 0) },
    { Point(5, 8), Point(5, 0) },
    { Point(0, 6), Point(6, 6) },
    { Point(0, 2), Point(6, 2) },
};

static Line C_dollar[] = {
    { Point(6, 6), Point(5, 7) },
    { Point(5, 7), Point(1, 7) },
    { Point(1, 7), Point(0, 6) },
    { Point(0, 6), Point(0, 5) },
    { Point(0, 5), Point(1, 4) },
    { Point(1, 4), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 2) },
    { Point(6, 2), Point(5, 1) },
    { Point(5, 1), Point(1, 1) },
    { Point(1, 1), Point(0, 2) },
    { Point(2, 8), Point(2, 0) },
    { Point(4, 8), Point(4, 0) },
};

static Line C_percent[] = {
    { Point(1, 1), Point(5, 7) },
    { Point(1, 5), Point(2, 6) },
    { Point(4, 2), Point(5, 3) },
};

static Line C_amper[] = {
    { Point(1, 0), Point(6, 6) },
    { Point(6, 6), Point(6, 7) },
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(4, 8) },
    { Point(4, 8), Point(3, 7) },
    { Point(3, 7), Point(3, 6) },
    { Point(3, 6), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(3, 0) },
    { Point(3, 0), Point(1, 2) },
};

static Line C_squote[] = {
    { Point(3, 8), Point(3, 7) },
    { Point(3, 7), Point(2, 6) },
};

static Line C_lparen[] = {
    { Point(4, 8), Point(3, 6) },
    { Point(3, 6), Point(3, 2) },
    { Point(3, 2), Point(4, 0) },
};

static Line C_rparen[] = {
    { Point(2, 8), Point(3, 6) },
    { Point(3, 6), Point(3, 2) },
    { Point(3, 2), Point(2, 0) },
};

static Line C_aster[] = {
    { Point(3, 1), Point(3, 7) },
    { Point(1, 6), Point(5, 2) },
    { Point(1, 2), Point(5, 6) },
};

static Line C_plus[] = {
    { Point(0, 4), Point(6, 4) },
    { Point(3, 1), Point(3, 7) },
};

static Line C_comma[] = {
    { Point(2, 1), Point(2, 0) },
    { Point(2, 0), Point(1, -1) },
};

static Line C_minus[] = {
    { Point(0, 4), Point(6, 4) },
};

static Line C_period[] = {
    { Point(2, 0), Point(2, 0) },
};

static Line C_slash[] = {
    { Point(1, 0), Point(5, 8) },
};

static Line C_0[] = {
    { Point(0, 2), Point(0, 6) },
    { Point(0, 6), Point(2, 8) },
    { Point(2, 8), Point(4, 8) },
    { Point(4, 8), Point(6, 6) },
    { Point(6, 6), Point(6, 2) },
    { Point(6, 2), Point(4, 0) },
    { Point(4, 0), Point(2, 0) },
    { Point(2, 0), Point(0, 2) },
};

static Line C_1[] = {
    { Point(1, 6), Point(3, 8) },
    { Point(3, 8), Point(3, 0) },
    { Point(1, 0), Point(5, 0) },
};

static Line C_2[] = {
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(0, 1) },
    { Point(0, 1), Point(0, 0) },
    { Point(0, 0), Point(6, 0) },
};

static Line C_3[] = {
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
    { Point(1, 4), Point(5, 4) },
};

static Line C_4[] = {
    { Point(5, 8), Point(5, 0) },
    { Point(1, 8), Point(0, 4) },
    { Point(0, 4), Point(6, 4) },
};

static Line C_5[] = {
    { Point(6, 8), Point(0, 8) },
    { Point(0, 8), Point(0, 4) },
    { Point(0, 4), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
};

static Line C_6[] = {
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 1) },
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
    { Point(6, 1), Point(6, 3) },
    { Point(6, 3), Point(5, 4) },
    { Point(5, 4), Point(0, 4) },
};

static Line C_7[] = {
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(6, 8) },
    { Point(6, 8), Point(4, 2) },
    { Point(4, 2), Point(4, 0) },
};

static Line C_8[] = {
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
    { Point(0, 1), Point(0, 3) },
    { Point(0, 3), Point(1, 4) },
    { Point(1, 4), Point(0, 5) },
    { Point(0, 5), Point(0, 7) },
    { Point(0, 7), Point(1, 8) },
    { Point(1, 4), Point(5, 4) },
};

static Line C_9[] = {
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
    { Point(6, 1), Point(6, 7) },
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 5) },
    { Point(0, 5), Point(1, 4) },
    { Point(1, 4), Point(6, 4) },
};

static Line C_colon[] = {
    { Point(3, 6), Point(3, 6) },
    { Point(3, 2), Point(3, 2) },
};

static Line C_semi[] = {
    { Point(3, 6), Point(3, 6) },
    { Point(3, 3), Point(3, 2) },
    { Point(3, 2), Point(2, 1) },
};

static Line C_less[] = {
    { Point(5, 8), Point(1, 4) },
    { Point(1, 4), Point(5, 0) },
};

static Line C_equal[] = {
    { Point(0, 5), Point(6, 5) },
    { Point(0, 3), Point(6, 3) },
};

static Line C_greater[] = {
    { Point(1, 8), Point(5, 4) },
    { Point(5, 4), Point(1, 0) },
};

static Line C_ques[] = {
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(3, 2) },
    { Point(3, 2), Point(3, 1) },
    { Point(3, 0), Point(3, 0) },
};

static Line C_at[] = {
    { Point(3, 2), Point(4, 3) },
    { Point(4, 3), Point(4, 4) },
    { Point(4, 4), Point(3, 5) },
    { Point(3, 5), Point(2, 4) },
    { Point(2, 4), Point(2, 3) },
    { Point(2, 3), Point(3, 2) },
    { Point(3, 2), Point(5, 2) },
    { Point(5, 2), Point(6, 3) },
    { Point(6, 3), Point(6, 7) },
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 1) },
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
};

static Line C_A[] = {
    { Point(0, 0), Point(0, 6) },
    { Point(0, 6), Point(3, 8) },
    { Point(3, 8), Point(6, 6) },
    { Point(6, 6), Point(6, 0) },
    { Point(0, 4), Point(6, 4) },
};

static Line C_B[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(0, 0) },
    { Point(0, 4), Point(5, 4) },
};

static Line C_C[] = {
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 1) },
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
};

static Line C_D[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(4, 8) },
    { Point(4, 8), Point(6, 6) },
    { Point(6, 6), Point(6, 2) },
    { Point(6, 2), Point(4, 0) },
    { Point(4, 0), Point(0, 0) },
};

static Line C_E[] = {
    { Point(0, 0), Point(6, 0) },
    { Point(0, 4), Point(5, 4) },
    { Point(0, 8), Point(6, 8) },
    { Point(0, 0), Point(0, 8) },
};

static Line C_F[] = {
    { Point(0, 4), Point(5, 4) },
    { Point(0, 8), Point(6, 8) },
    { Point(0, 0), Point(0, 8) },
};

static Line C_G[] = {
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 1) },
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
    { Point(6, 1), Point(6, 4) },
    { Point(6, 4), Point(3, 4) },
};

static Line C_H[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(6, 0), Point(6, 8) },
    { Point(0, 4), Point(6, 4) },
};

static Line C_I[] = {
    { Point(1, 8), Point(5, 8) },
    { Point(1, 0), Point(5, 0) },
    { Point(3, 8), Point(3, 0) },
};

static Line C_J[] = {
    { Point(4, 8), Point(6, 8) },
    { Point(5, 8), Point(5, 1) },
    { Point(5, 1), Point(4, 0) },
    { Point(4, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
};

static Line C_K[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 4), Point(6, 8) },
    { Point(0, 4), Point(6, 0) },
};

static Line C_L[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 0), Point(6, 0) },
};

static Line C_M[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(3, 4) },
    { Point(3, 4), Point(6, 8) },
    { Point(6, 8), Point(6, 0) },
};

static Line C_N[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(6, 0) },
    { Point(6, 0), Point(6, 8) },
};

static Line C_O[] = {
    { Point(0, 1), Point(0, 7) },
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
};

static Line C_P[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(5, 4) },
    { Point(5, 4), Point(0, 4) },
};

static Line C_Q[] = {
    { Point(0, 1), Point(0, 7) },
    { Point(0, 7), Point(1, 8) },
    { Point(1, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 2) },
    { Point(6, 2), Point(4, 0) },
    { Point(4, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
    { Point(6, 0), Point(3, 3) },
};

static Line C_R[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(0, 8), Point(5, 8) },
    { Point(5, 8), Point(6, 7) },
    { Point(6, 7), Point(6, 5) },
    { Point(6, 5), Point(5, 4) },
    { Point(5, 4), Point(0, 4) },
    { Point(3, 4), Point(6, 0) },
};

static Line C_S[] = {
    { Point(6, 7), Point(5, 8) },
    { Point(5, 8), Point(1, 8) },
    { Point(1, 8), Point(0, 7) },
    { Point(0, 7), Point(0, 5) },
    { Point(0, 5), Point(1, 4) },
    { Point(1, 4), Point(5, 4) },
    { Point(5, 4), Point(6, 3) },
    { Point(6, 3), Point(6, 1) },
    { Point(6, 1), Point(5, 0) },
    { Point(5, 0), Point(1, 0) },
    { Point(1, 0), Point(0, 1) },
};

static Line C_T[] = {
    { Point(0, 8), Point(6, 8) },
    { Point(3, 8), Point(3, 0) },
};

static Line C_U[] = {
    { Point(0, 8), Point(0, 1) },
    { Point(0, 1), Point(1, 0) },
    { Point(1, 0), Point(5, 0) },
    { Point(5, 0), Point(6, 1) },
    { Point(6, 1), Point(6, 8) },
};

static Line C_V[] = {
    { Point(0, 8), Point(3, 0) },
    { Point(3, 0), Point(6, 8) },
};

static Line C_W[] = {
    { Point(0, 8), Point(0, 0) },
    { Point(0, 0), Point(3, 4) },
    { Point(3, 4), Point(6, 0) },
    { Point(6, 0), Point(6, 8) },
};

static Line C_X[] = {
    { Point(0, 0), Point(6, 8) },
    { Point(0, 8), Point(6, 0) },
};

static Line C_Y[] = {
    { Point(0, 8), Point(0, 6) },
    { Point(0, 6), Point(3, 4) },
    { Point(3, 4), Point(6, 6) },
    { Point(6, 6), Point(6, 8) },
    { Point(3, 4), Point(3, 0) },
};

static Line C_Z[] = {
    { Point(0, 8), Point(6, 8) },
    { Point(6, 8), Point(0, 0) },
    { Point(0, 0), Point(6, 0) },
};

static Line C_lbrack[] = {
    { Point(4, 8), Point(2, 8) },
    { Point(2, 8), Point(2, 0) },
    { Point(2, 0), Point(4, 0) },
};

static Line C_bslash[] = {
    { Point(1, 8), Point(5, 0) },
};

static Line C_rbrack[] = {
    { Point(2, 8), Point(4, 8) },
    { Point(4, 8), Point(4, 0) },
    { Point(4, 0), Point(2, 0) },
};

static Line C_caret[] = {
    { Point(0, 5), Point(3, 8) },
    { Point(3, 8), Point(6, 5) },
};

static Line C_uscore[] = {
    { Point(0, 0), Point(6, 0) },
};

static Line C_bquote[] = {
    { Point(3, 8), Point(3, 7) },
    { Point(3, 7), Point(4, 6) },
};

static Line C_lbrace[] = {
    { Point(4, 8), Point(3, 7) },
    { Point(3, 7), Point(3, 5) },
    { Point(3, 5), Point(2, 4) },
    { Point(2, 4), Point(3, 3) },
    { Point(3, 3), Point(3, 1) },
    { Point(3, 1), Point(4, 0) },
};

static Line C_vbar[] = {
    { Point(3, 8), Point(3, 0) },
};

static Line C_rbrace[] = {
    { Point(2, 8), Point(3, 7) },
    { Point(3, 7), Point(3, 5) },
    { Point(3, 5), Point(4, 4) },
    { Point(4, 4), Point(3, 3) },
    { Point(3, 3), Point(3, 1) },
    { Point(3, 1), Point(2, 0) },
};

static Line C_tilde[] = {
    { Point(0, 4), Point(1, 5) },
    { Point(1, 5), Point(2, 5) },
    { Point(2, 5), Point(4, 3) },
    { Point(4, 3), Point(5, 3) },
    { Point(5, 3), Point(6, 4) },
};

static Line C_block[] = {
    { Point(0, 0), Point(0, 8) },
    { Point(1, 0), Point(1, 8) },
    { Point(2, 0), Point(2, 8) },
    { Point(3, 0), Point(3, 8) },
    { Point(4, 0), Point(4, 8) },
    { Point(5, 0), Point(5, 8) },
    { Point(6, 0), Point(6, 8) },
    { Point(0, 0), Point(6, 0) },
    { Point(0, 1), Point(6, 1) },
    { Point(0, 2), Point(6, 2) },
    { Point(0, 3), Point(6, 3) },
    { Point(0, 4), Point(6, 4) },
    { Point(0, 5), Point(6, 5) },
    { Point(0, 6), Point(6, 6) },
    { Point(0, 7), Point(6, 7) },
    { Point(0, 8), Point(6, 8) },
};

// ASCII table

static struct lineCharEntry {
    Line *stroke;
    int nStroke;
} lineChar[128] = {
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_unknown, ARRAYSIZE(C_unknown) },
    { C_space, ARRAYSIZE(C_space) },
    { C_exclam, ARRAYSIZE(C_exclam) },
    { C_dquote, ARRAYSIZE(C_dquote) },
    { C_pound, ARRAYSIZE(C_pound) },
    { C_dollar, ARRAYSIZE(C_dollar) },
    { C_percent, ARRAYSIZE(C_percent) },
    { C_amper, ARRAYSIZE(C_amper) },
    { C_squote, ARRAYSIZE(C_squote) },
    { C_lparen, ARRAYSIZE(C_lparen) },
    { C_rparen, ARRAYSIZE(C_rparen) },
    { C_aster, ARRAYSIZE(C_aster) },
    { C_plus, ARRAYSIZE(C_plus) },
    { C_comma, ARRAYSIZE(C_comma) },
    { C_minus, ARRAYSIZE(C_minus) },
    { C_period, ARRAYSIZE(C_period) },
    { C_slash, ARRAYSIZE(C_slash) },
    { C_0, ARRAYSIZE(C_0) },
    { C_1, ARRAYSIZE(C_1) },
    { C_2, ARRAYSIZE(C_2) },
    { C_3, ARRAYSIZE(C_3) },
    { C_4, ARRAYSIZE(C_4) },
    { C_5, ARRAYSIZE(C_5) },
    { C_6, ARRAYSIZE(C_6) },
    { C_7, ARRAYSIZE(C_7) },
    { C_8, ARRAYSIZE(C_8) },
    { C_9, ARRAYSIZE(C_9) },
    { C_colon, ARRAYSIZE(C_colon) },
    { C_semi, ARRAYSIZE(C_semi) },
    { C_less, ARRAYSIZE(C_less) },
    { C_equal, ARRAYSIZE(C_equal) },
    { C_greater, ARRAYSIZE(C_greater) },
    { C_ques, ARRAYSIZE(C_ques) },
    { C_at, ARRAYSIZE(C_at) },
    { C_A, ARRAYSIZE(C_A) },
    { C_B, ARRAYSIZE(C_B) },
    { C_C, ARRAYSIZE(C_C) },
    { C_D, ARRAYSIZE(C_D) },
    { C_E, ARRAYSIZE(C_E) },
    { C_F, ARRAYSIZE(C_F) },
    { C_G, ARRAYSIZE(C_G) },
    { C_H, ARRAYSIZE(C_H) },
    { C_I, ARRAYSIZE(C_I) },
    { C_J, ARRAYSIZE(C_J) },
    { C_K, ARRAYSIZE(C_K) },
    { C_L, ARRAYSIZE(C_L) },
    { C_M, ARRAYSIZE(C_M) },
    { C_N, ARRAYSIZE(C_N) },
    { C_O, ARRAYSIZE(C_O) },
    { C_P, ARRAYSIZE(C_P) },
    { C_Q, ARRAYSIZE(C_Q) },
    { C_R, ARRAYSIZE(C_R) },
    { C_S, ARRAYSIZE(C_S) },
    { C_T, ARRAYSIZE(C_T) },
    { C_U, ARRAYSIZE(C_U) },
    { C_V, ARRAYSIZE(C_V) },
    { C_W, ARRAYSIZE(C_W) },
    { C_X, ARRAYSIZE(C_X) },
    { C_Y, ARRAYSIZE(C_Y) },
    { C_Z, ARRAYSIZE(C_Z) },
    { C_lbrack, ARRAYSIZE(C_lbrack) },
    { C_bslash, ARRAYSIZE(C_bslash) },
    { C_rbrack, ARRAYSIZE(C_rbrack) },
    { C_caret, ARRAYSIZE(C_caret) },
    { C_uscore, ARRAYSIZE(C_uscore) },
    { C_bquote, ARRAYSIZE(C_bquote) },
    { C_A, ARRAYSIZE(C_A) },
    { C_B, ARRAYSIZE(C_B) },
    { C_C, ARRAYSIZE(C_C) },
    { C_D, ARRAYSIZE(C_D) },
    { C_E, ARRAYSIZE(C_E) },
    { C_F, ARRAYSIZE(C_F) },
    { C_G, ARRAYSIZE(C_G) },
    { C_H, ARRAYSIZE(C_H) },
    { C_I, ARRAYSIZE(C_I) },
    { C_J, ARRAYSIZE(C_J) },
    { C_K, ARRAYSIZE(C_K) },
    { C_L, ARRAYSIZE(C_L) },
    { C_M, ARRAYSIZE(C_M) },
    { C_N, ARRAYSIZE(C_N) },
    { C_O, ARRAYSIZE(C_O) },
    { C_P, ARRAYSIZE(C_P) },
    { C_Q, ARRAYSIZE(C_Q) },
    { C_R, ARRAYSIZE(C_R) },
    { C_S, ARRAYSIZE(C_S) },
    { C_T, ARRAYSIZE(C_T) },
    { C_U, ARRAYSIZE(C_U) },
    { C_V, ARRAYSIZE(C_V) },
    { C_W, ARRAYSIZE(C_W) },
    { C_X, ARRAYSIZE(C_X) },
    { C_Y, ARRAYSIZE(C_Y) },
    { C_Z, ARRAYSIZE(C_Z) },
    { C_lbrace, ARRAYSIZE(C_lbrace) },
    { C_vbar, ARRAYSIZE(C_vbar) },
    { C_rbrace, ARRAYSIZE(C_rbrace) },
    { C_tilde, ARRAYSIZE(C_tilde) },
    { C_block, ARRAYSIZE(C_block) },
};

static Shape *GenShape(int ch, double scale, bool italic)
{
    Shape *s;
    lineCharEntry *ce = &lineChar[ch];

    s = new Shape(ce->nStroke);

    for (int i = 0; i < ce->nStroke; i++) {
        Line l;
	l.f = ce->stroke[i].f;
        l.t = ce->stroke[i].t;
	if (italic) {
	    l.f.x += l.f.y * ITALICSLOPE;
	    l.t.x += l.t.y * ITALICSLOPE;
	}
	l.f.x *= scale;
	l.f.y *= -scale;
	l.t.x *= scale;
	l.t.y *= -scale;
        s->append(l);
    }

    return s;
}

Linefont::Linefont(double scale, bool italic)
{
    for (int ch = 0; ch < 128; ch++)
        chars[ch] = GenShape(ch, scale, italic);

    // Char spacing as percent of char width
    double spaceMul = scale * PERCENT(145);

    charSize = spaceMul * Vect((double)LINEFONT_SCALE1_CHAR_W,
			       (double)LINEFONT_SCALE1_CHAR_H);
    charSpacing = spaceMul * Vect((double)LINEFONT_SCALE1_CHAR_W, 0.0);
    lineSpacing = spaceMul * Vect(0.0, (double)LINEFONT_SCALE1_CHAR_H);
}

Linefont::~Linefont()
{
    for (int ch = 0; ch < 128; ch++)
	delete(chars[ch]);
}
