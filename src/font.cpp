#include "param.hpp"
#include "font.hpp"

// Each character is defined in a grid of points (0..6, 0..8), where (0,
// 0) is at the lower left and (6, 8) is at the upper right.  Everything
// is uppercase so there are no descenders (if there were, they'd drop
// down an additional 2 grid units).

#define GRID_W		6
#define GRID_H		8

// Character and line spacing as percent of grid size
#define SPACING_CHAR	PERCENT(145)
#define SPACING_LINE	PERCENT(174)

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
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },	// space
    { C_exclam, std::size(C_exclam) },
    { C_dquote, std::size(C_dquote) },
    { C_pound, std::size(C_pound) },
    { C_dollar, std::size(C_dollar) },
    { C_percent, std::size(C_percent) },
    { C_amper, std::size(C_amper) },
    { C_squote, std::size(C_squote) },
    { C_lparen, std::size(C_lparen) },
    { C_rparen, std::size(C_rparen) },
    { C_aster, std::size(C_aster) },
    { C_plus, std::size(C_plus) },
    { C_comma, std::size(C_comma) },
    { C_minus, std::size(C_minus) },
    { C_period, std::size(C_period) },
    { C_slash, std::size(C_slash) },
    { C_0, std::size(C_0) },
    { C_1, std::size(C_1) },
    { C_2, std::size(C_2) },
    { C_3, std::size(C_3) },
    { C_4, std::size(C_4) },
    { C_5, std::size(C_5) },
    { C_6, std::size(C_6) },
    { C_7, std::size(C_7) },
    { C_8, std::size(C_8) },
    { C_9, std::size(C_9) },
    { C_colon, std::size(C_colon) },
    { C_semi, std::size(C_semi) },
    { C_less, std::size(C_less) },
    { C_equal, std::size(C_equal) },
    { C_greater, std::size(C_greater) },
    { C_ques, std::size(C_ques) },
    { C_at, std::size(C_at) },
    { C_A, std::size(C_A) },
    { C_B, std::size(C_B) },
    { C_C, std::size(C_C) },
    { C_D, std::size(C_D) },
    { C_E, std::size(C_E) },
    { C_F, std::size(C_F) },
    { C_G, std::size(C_G) },
    { C_H, std::size(C_H) },
    { C_I, std::size(C_I) },
    { C_J, std::size(C_J) },
    { C_K, std::size(C_K) },
    { C_L, std::size(C_L) },
    { C_M, std::size(C_M) },
    { C_N, std::size(C_N) },
    { C_O, std::size(C_O) },
    { C_P, std::size(C_P) },
    { C_Q, std::size(C_Q) },
    { C_R, std::size(C_R) },
    { C_S, std::size(C_S) },
    { C_T, std::size(C_T) },
    { C_U, std::size(C_U) },
    { C_V, std::size(C_V) },
    { C_W, std::size(C_W) },
    { C_X, std::size(C_X) },
    { C_Y, std::size(C_Y) },
    { C_Z, std::size(C_Z) },
    { C_lbrack, std::size(C_lbrack) },
    { C_bslash, std::size(C_bslash) },
    { C_rbrack, std::size(C_rbrack) },
    { C_caret, std::size(C_caret) },
    { C_uscore, std::size(C_uscore) },
    { C_bquote, std::size(C_bquote) },
    { C_A, std::size(C_A) },
    { C_B, std::size(C_B) },
    { C_C, std::size(C_C) },
    { C_D, std::size(C_D) },
    { C_E, std::size(C_E) },
    { C_F, std::size(C_F) },
    { C_G, std::size(C_G) },
    { C_H, std::size(C_H) },
    { C_I, std::size(C_I) },
    { C_J, std::size(C_J) },
    { C_K, std::size(C_K) },
    { C_L, std::size(C_L) },
    { C_M, std::size(C_M) },
    { C_N, std::size(C_N) },
    { C_O, std::size(C_O) },
    { C_P, std::size(C_P) },
    { C_Q, std::size(C_Q) },
    { C_R, std::size(C_R) },
    { C_S, std::size(C_S) },
    { C_T, std::size(C_T) },
    { C_U, std::size(C_U) },
    { C_V, std::size(C_V) },
    { C_W, std::size(C_W) },
    { C_X, std::size(C_X) },
    { C_Y, std::size(C_Y) },
    { C_Z, std::size(C_Z) },
    { C_lbrace, std::size(C_lbrace) },
    { C_vbar, std::size(C_vbar) },
    { C_rbrace, std::size(C_rbrace) },
    { C_tilde, std::size(C_tilde) },
    { C_block, std::size(C_block) },
};

// Shapes are generated with the left side of the character at the
// specified x and the baseline at the specified y, from the vertically
// flipped Shape coordinate system.
static Shape *GenShape(int ch, bool bold, bool italic)
{
    Shape *s;
    lineCharEntry *ce = &lineChar[ch];

    s = new Shape();

    for (int i = 0; i < ce->nStroke; i++) {
        Line l = ce->stroke[i];

	if (italic) {
	    l.f.x += l.f.y * ITALICSLOPE;
	    l.t.x += l.t.y * ITALICSLOPE;
	}

	l.f.y = -l.f.y;
	l.t.y = -l.t.y;

        s->append(l);

	if (bold) {
	    double shift = 0.15;

	    s->append(l + Vect(shift, 0.0));
	    s->append(l + Vect(shift, shift));
	    s->append(l + Vect(0.0, shift));
	}
    }

    return s;
}

Font::Font(bool bold, bool italic)
{
    for (int ch = 0; ch < 128; ch++)
	chars.push_back(GenShape(ch, bold, italic));

    spacing = Vect(GRID_W * SPACING_CHAR,
		   GRID_H * SPACING_LINE);
}

Font::~Font()
{
    for (auto s : chars)
	delete s;
}

Font *Font::fontRegular;
Font *Font::fontBold;
Font *Font::fontItalic;
Font *Font::fontBoldItalic;

void Font::init()
{
    fontRegular = new Font(false, false);
    fontBold = new Font(true, false);
    fontItalic = new Font(false, true);
    fontBoldItalic = new Font(true, true);
}

void Font::term()
{
    delete fontBoldItalic;
    delete fontItalic;
    delete fontBold;
    delete fontRegular;
}
