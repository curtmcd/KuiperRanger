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
    { Vect(3, 8), Vect(3, 2) },
    { Vect(3, 0), Vect(3, 0) },
};

static Line C_dquote[] = {
    { Vect(1, 8), Vect(1, 6) },
    { Vect(5, 8), Vect(5, 6) },
};

static Line C_pound[] = {
    { Vect(1, 8), Vect(1, 0) },
    { Vect(5, 8), Vect(5, 0) },
    { Vect(0, 6), Vect(6, 6) },
    { Vect(0, 2), Vect(6, 2) },
};

static Line C_dollar[] = {
    { Vect(6, 6), Vect(5, 7) },
    { Vect(5, 7), Vect(1, 7) },
    { Vect(1, 7), Vect(0, 6) },
    { Vect(0, 6), Vect(0, 5) },
    { Vect(0, 5), Vect(1, 4) },
    { Vect(1, 4), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 2) },
    { Vect(6, 2), Vect(5, 1) },
    { Vect(5, 1), Vect(1, 1) },
    { Vect(1, 1), Vect(0, 2) },
    { Vect(2, 8), Vect(2, 0) },
    { Vect(4, 8), Vect(4, 0) },
};

static Line C_percent[] = {
    { Vect(1, 1), Vect(5, 7) },
    { Vect(1, 5), Vect(2, 6) },
    { Vect(4, 2), Vect(5, 3) },
};

static Line C_amper[] = {
    { Vect(1, 0), Vect(6, 6) },
    { Vect(6, 6), Vect(6, 7) },
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(4, 8) },
    { Vect(4, 8), Vect(3, 7) },
    { Vect(3, 7), Vect(3, 6) },
    { Vect(3, 6), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(3, 0) },
    { Vect(3, 0), Vect(1, 2) },
};

static Line C_squote[] = {
    { Vect(3, 8), Vect(3, 7) },
    { Vect(3, 7), Vect(2, 6) },
};

static Line C_lparen[] = {
    { Vect(4, 8), Vect(3, 6) },
    { Vect(3, 6), Vect(3, 2) },
    { Vect(3, 2), Vect(4, 0) },
};

static Line C_rparen[] = {
    { Vect(2, 8), Vect(3, 6) },
    { Vect(3, 6), Vect(3, 2) },
    { Vect(3, 2), Vect(2, 0) },
};

static Line C_aster[] = {
    { Vect(3, 1), Vect(3, 7) },
    { Vect(1, 6), Vect(5, 2) },
    { Vect(1, 2), Vect(5, 6) },
};

static Line C_plus[] = {
    { Vect(0, 4), Vect(6, 4) },
    { Vect(3, 1), Vect(3, 7) },
};

static Line C_comma[] = {
    { Vect(2, 1), Vect(2, 0) },
    { Vect(2, 0), Vect(1, -1) },
};

static Line C_minus[] = {
    { Vect(0, 4), Vect(6, 4) },
};

static Line C_period[] = {
    { Vect(2, 0), Vect(2, 0) },
};

static Line C_slash[] = {
    { Vect(1, 0), Vect(5, 8) },
};

static Line C_0[] = {
    { Vect(0, 2), Vect(0, 6) },
    { Vect(0, 6), Vect(2, 8) },
    { Vect(2, 8), Vect(4, 8) },
    { Vect(4, 8), Vect(6, 6) },
    { Vect(6, 6), Vect(6, 2) },
    { Vect(6, 2), Vect(4, 0) },
    { Vect(4, 0), Vect(2, 0) },
    { Vect(2, 0), Vect(0, 2) },
};

static Line C_1[] = {
    { Vect(1, 6), Vect(3, 8) },
    { Vect(3, 8), Vect(3, 0) },
    { Vect(1, 0), Vect(5, 0) },
};

static Line C_2[] = {
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(0, 1) },
    { Vect(0, 1), Vect(0, 0) },
    { Vect(0, 0), Vect(6, 0) },
};

static Line C_3[] = {
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
    { Vect(1, 4), Vect(5, 4) },
};

static Line C_4[] = {
    { Vect(5, 8), Vect(5, 0) },
    { Vect(1, 8), Vect(0, 4) },
    { Vect(0, 4), Vect(6, 4) },
};

static Line C_5[] = {
    { Vect(6, 8), Vect(0, 8) },
    { Vect(0, 8), Vect(0, 4) },
    { Vect(0, 4), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
};

static Line C_6[] = {
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 1) },
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
    { Vect(6, 1), Vect(6, 3) },
    { Vect(6, 3), Vect(5, 4) },
    { Vect(5, 4), Vect(0, 4) },
};

static Line C_7[] = {
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(6, 8) },
    { Vect(6, 8), Vect(4, 2) },
    { Vect(4, 2), Vect(4, 0) },
};

static Line C_8[] = {
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
    { Vect(0, 1), Vect(0, 3) },
    { Vect(0, 3), Vect(1, 4) },
    { Vect(1, 4), Vect(0, 5) },
    { Vect(0, 5), Vect(0, 7) },
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 4), Vect(5, 4) },
};

static Line C_9[] = {
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
    { Vect(6, 1), Vect(6, 7) },
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 5) },
    { Vect(0, 5), Vect(1, 4) },
    { Vect(1, 4), Vect(6, 4) },
};

static Line C_colon[] = {
    { Vect(3, 6), Vect(3, 6) },
    { Vect(3, 2), Vect(3, 2) },
};

static Line C_semi[] = {
    { Vect(3, 6), Vect(3, 6) },
    { Vect(3, 3), Vect(3, 2) },
    { Vect(3, 2), Vect(2, 1) },
};

static Line C_less[] = {
    { Vect(5, 8), Vect(1, 4) },
    { Vect(1, 4), Vect(5, 0) },
};

static Line C_equal[] = {
    { Vect(0, 5), Vect(6, 5) },
    { Vect(0, 3), Vect(6, 3) },
};

static Line C_greater[] = {
    { Vect(1, 8), Vect(5, 4) },
    { Vect(5, 4), Vect(1, 0) },
};

static Line C_ques[] = {
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(3, 2) },
    { Vect(3, 2), Vect(3, 1) },
    { Vect(3, 0), Vect(3, 0) },
};

static Line C_at[] = {
    { Vect(3, 2), Vect(4, 3) },
    { Vect(4, 3), Vect(4, 4) },
    { Vect(4, 4), Vect(3, 5) },
    { Vect(3, 5), Vect(2, 4) },
    { Vect(2, 4), Vect(2, 3) },
    { Vect(2, 3), Vect(3, 2) },
    { Vect(3, 2), Vect(5, 2) },
    { Vect(5, 2), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 7) },
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 1) },
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
};

static Line C_A[] = {
    { Vect(0, 0), Vect(0, 6) },
    { Vect(0, 6), Vect(3, 8) },
    { Vect(3, 8), Vect(6, 6) },
    { Vect(6, 6), Vect(6, 0) },
    { Vect(0, 4), Vect(6, 4) },
};

static Line C_B[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(0, 0) },
    { Vect(0, 4), Vect(5, 4) },
};

static Line C_C[] = {
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 1) },
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
};

static Line C_D[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(4, 8) },
    { Vect(4, 8), Vect(6, 6) },
    { Vect(6, 6), Vect(6, 2) },
    { Vect(6, 2), Vect(4, 0) },
    { Vect(4, 0), Vect(0, 0) },
};

static Line C_E[] = {
    { Vect(0, 0), Vect(6, 0) },
    { Vect(0, 4), Vect(5, 4) },
    { Vect(0, 8), Vect(6, 8) },
    { Vect(0, 0), Vect(0, 8) },
};

static Line C_F[] = {
    { Vect(0, 4), Vect(5, 4) },
    { Vect(0, 8), Vect(6, 8) },
    { Vect(0, 0), Vect(0, 8) },
};

static Line C_G[] = {
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 1) },
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
    { Vect(6, 1), Vect(6, 4) },
    { Vect(6, 4), Vect(3, 4) },
};

static Line C_H[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(6, 0), Vect(6, 8) },
    { Vect(0, 4), Vect(6, 4) },
};

static Line C_I[] = {
    { Vect(1, 8), Vect(5, 8) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(3, 8), Vect(3, 0) },
};

static Line C_J[] = {
    { Vect(4, 8), Vect(6, 8) },
    { Vect(5, 8), Vect(5, 1) },
    { Vect(5, 1), Vect(4, 0) },
    { Vect(4, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
};

static Line C_K[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 4), Vect(6, 8) },
    { Vect(0, 4), Vect(6, 0) },
};

static Line C_L[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 0), Vect(6, 0) },
};

static Line C_M[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(3, 4) },
    { Vect(3, 4), Vect(6, 8) },
    { Vect(6, 8), Vect(6, 0) },
};

static Line C_N[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(6, 0) },
    { Vect(6, 0), Vect(6, 8) },
};

static Line C_O[] = {
    { Vect(0, 1), Vect(0, 7) },
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
};

static Line C_P[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(5, 4) },
    { Vect(5, 4), Vect(0, 4) },
};

static Line C_Q[] = {
    { Vect(0, 1), Vect(0, 7) },
    { Vect(0, 7), Vect(1, 8) },
    { Vect(1, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 2) },
    { Vect(6, 2), Vect(4, 0) },
    { Vect(4, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
    { Vect(6, 0), Vect(3, 3) },
};

static Line C_R[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(0, 8), Vect(5, 8) },
    { Vect(5, 8), Vect(6, 7) },
    { Vect(6, 7), Vect(6, 5) },
    { Vect(6, 5), Vect(5, 4) },
    { Vect(5, 4), Vect(0, 4) },
    { Vect(3, 4), Vect(6, 0) },
};

static Line C_S[] = {
    { Vect(6, 7), Vect(5, 8) },
    { Vect(5, 8), Vect(1, 8) },
    { Vect(1, 8), Vect(0, 7) },
    { Vect(0, 7), Vect(0, 5) },
    { Vect(0, 5), Vect(1, 4) },
    { Vect(1, 4), Vect(5, 4) },
    { Vect(5, 4), Vect(6, 3) },
    { Vect(6, 3), Vect(6, 1) },
    { Vect(6, 1), Vect(5, 0) },
    { Vect(5, 0), Vect(1, 0) },
    { Vect(1, 0), Vect(0, 1) },
};

static Line C_T[] = {
    { Vect(0, 8), Vect(6, 8) },
    { Vect(3, 8), Vect(3, 0) },
};

static Line C_U[] = {
    { Vect(0, 8), Vect(0, 1) },
    { Vect(0, 1), Vect(1, 0) },
    { Vect(1, 0), Vect(5, 0) },
    { Vect(5, 0), Vect(6, 1) },
    { Vect(6, 1), Vect(6, 8) },
};

static Line C_V[] = {
    { Vect(0, 8), Vect(3, 0) },
    { Vect(3, 0), Vect(6, 8) },
};

static Line C_W[] = {
    { Vect(0, 8), Vect(0, 0) },
    { Vect(0, 0), Vect(3, 4) },
    { Vect(3, 4), Vect(6, 0) },
    { Vect(6, 0), Vect(6, 8) },
};

static Line C_X[] = {
    { Vect(0, 0), Vect(6, 8) },
    { Vect(0, 8), Vect(6, 0) },
};

static Line C_Y[] = {
    { Vect(0, 8), Vect(0, 6) },
    { Vect(0, 6), Vect(3, 4) },
    { Vect(3, 4), Vect(6, 6) },
    { Vect(6, 6), Vect(6, 8) },
    { Vect(3, 4), Vect(3, 0) },
};

static Line C_Z[] = {
    { Vect(0, 8), Vect(6, 8) },
    { Vect(6, 8), Vect(0, 0) },
    { Vect(0, 0), Vect(6, 0) },
};

static Line C_lbrack[] = {
    { Vect(4, 8), Vect(2, 8) },
    { Vect(2, 8), Vect(2, 0) },
    { Vect(2, 0), Vect(4, 0) },
};

static Line C_bslash[] = {
    { Vect(1, 8), Vect(5, 0) },
};

static Line C_rbrack[] = {
    { Vect(2, 8), Vect(4, 8) },
    { Vect(4, 8), Vect(4, 0) },
    { Vect(4, 0), Vect(2, 0) },
};

static Line C_caret[] = {
    { Vect(0, 5), Vect(3, 8) },
    { Vect(3, 8), Vect(6, 5) },
};

static Line C_uscore[] = {
    { Vect(0, 0), Vect(6, 0) },
};

static Line C_bquote[] = {
    { Vect(3, 8), Vect(3, 7) },
    { Vect(3, 7), Vect(4, 6) },
};

static Line C_lbrace[] = {
    { Vect(4, 8), Vect(3, 7) },
    { Vect(3, 7), Vect(3, 5) },
    { Vect(3, 5), Vect(2, 4) },
    { Vect(2, 4), Vect(3, 3) },
    { Vect(3, 3), Vect(3, 1) },
    { Vect(3, 1), Vect(4, 0) },
};

static Line C_vbar[] = {
    { Vect(3, 8), Vect(3, 0) },
};

static Line C_rbrace[] = {
    { Vect(2, 8), Vect(3, 7) },
    { Vect(3, 7), Vect(3, 5) },
    { Vect(3, 5), Vect(4, 4) },
    { Vect(4, 4), Vect(3, 3) },
    { Vect(3, 3), Vect(3, 1) },
    { Vect(3, 1), Vect(2, 0) },
};

static Line C_tilde[] = {
    { Vect(0, 4), Vect(1, 5) },
    { Vect(1, 5), Vect(2, 5) },
    { Vect(2, 5), Vect(4, 3) },
    { Vect(4, 3), Vect(5, 3) },
    { Vect(5, 3), Vect(6, 4) },
};

static Line C_block[] = {
    { Vect(0, 0), Vect(0, 8) },
    { Vect(1, 0), Vect(1, 8) },
    { Vect(2, 0), Vect(2, 8) },
    { Vect(3, 0), Vect(3, 8) },
    { Vect(4, 0), Vect(4, 8) },
    { Vect(5, 0), Vect(5, 8) },
    { Vect(6, 0), Vect(6, 8) },
    { Vect(0, 0), Vect(6, 0) },
    { Vect(0, 1), Vect(6, 1) },
    { Vect(0, 2), Vect(6, 2) },
    { Vect(0, 3), Vect(6, 3) },
    { Vect(0, 4), Vect(6, 4) },
    { Vect(0, 5), Vect(6, 5) },
    { Vect(0, 6), Vect(6, 6) },
    { Vect(0, 7), Vect(6, 7) },
    { Vect(0, 8), Vect(6, 8) },
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

static Shape *GenShape(int ch, double scale, double angle, bool italic)
{
    Shape *s;
    lineCharEntry *ce = &lineChar[ch];

    s = new Shape(ce->nStroke);

    for (int i = 0; i < ce->nStroke; i++) {
        Line l;
	l.f = ce->stroke[i].f;
        l.t = ce->stroke[i].t;
	l.f.y = -l.f.y;
	l.t.y = -l.t.y;
	l.f *= scale;
	l.t *= scale;
	if (italic) {
	    l.f.x -= l.f.y * ITALICSLOPE;
	    l.t.x -= l.t.y * ITALICSLOPE;
	}
        s->append(l);
    }

    s->rotate(angle);

    return s;
}

Linefont::Linefont(double scale, double angle, bool italic)
{
    for (int ch = 0; ch < 128; ch++)
        chars[ch] = GenShape(ch, scale, angle, italic);

    // Char spacing as percent of char width
    double spaceMul = scale * PERCENT(145);

    charSize = spaceMul * Vect((double)LINEFONT_SCALE1_CHAR_W,
			       (double)LINEFONT_SCALE1_CHAR_H);

    charSpacing = spaceMul * Vect((double)LINEFONT_SCALE1_CHAR_W, 0.0);
    charSpacing.rotate(angle);

    lineSpacing = spaceMul * Vect(0.0, (double)LINEFONT_SCALE1_CHAR_H);
    lineSpacing.rotate(angle);
}

Linefont::~Linefont()
{
    for (int ch = 0; ch < 128; ch++)
	delete(chars[ch]);
}
