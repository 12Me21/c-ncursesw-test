#pragma once
#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>

/*   A_CHARTEXT:       FF */
/*      A_COLOR:     FF00 */
/*   A_STANDOUT:    10000 */
/*  A_UNDERLINE:    20000 */
/*    A_REVERSE:    40000 */
/*      A_BLINK:    80000 */
/*        A_DIM:   100000 */
/*       A_BOLD:   200000 */
/* A_ALTCHARSET:   400000 */
/*      A_INVIS:   800000 */
/*    A_PROTECT:  1000000 */
/*     A_ITALIC: 80000000 */

typedef union Attr {
	attr_t attr;
	struct {
		uint8_t chartext: 8;
		uint8_t color: 8;
		bool standout: 1;
		bool underline: 1;
		bool reverse: 1;
		bool blink: 1;
		bool dim: 1;
		bool bold: 1;
		bool altcharset: 1;
		bool invis: 1;
		bool protect: 1;
		int _unused: 6;
		bool italic: 1;
	};
} Attr;

void Term_init(void);
void Term_free(void);
