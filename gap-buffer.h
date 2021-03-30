#pragma once
#include <stdlib.h>
#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>
#include "string.h"

typedef struct GapBuf {
	String data;
	Index before; // number of chars before the gap
	Index gap; // size of gap

	// (redundant ↓)
	Index after; // number of chars after gap
	Index cont; // position of end of gap
	// length?
	
	Index* frontLines;
	Index* backLines;
} GapBuf;

void GapBuf_init(GapBuf* g, Index size);
void GapBuf_insert(GapBuf* g, Index start, Index size);
void GapBuf_read(const GapBuf* g, Index start, const String s);
void GapBuf_write(const GapBuf* g, Index start, const String s);
void GapBuf_moveGap(GapBuf* g, Index gap);
void GapBuf_debug(const GapBuf* g);
void GapBuf_display(const GapBuf* g, WINDOW* w);
