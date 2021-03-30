#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "gap-buffer.h"

// plan:
// buffer text stored in simple gap buffer

// properties are stored relative to each end, depending on which half they're in

void GapBuf_init(GapBuf* g, Index size) {
	*g = (GapBuf){
		.data = String_new(size),
		.before = 0,
		.gap = size,
		.after = 0,
		.cont = size,
	};
}
	
void GapBuf_delete(GapBuf* g, Index start, Index size) {
}

// todo: specifying new gap position?
void GapBuf_insert(GapBuf* g, Index start, Index size) {
	if (g->gap < size) {
		//error
		return;
	}
	// BBBBB........AAAA
	//  ^6
	// BnnnnnnBBBB..AAAA
	if (start<=g->before) {
		String_move(g->data, start+size, start, g->before-start);
		g->before += size;
	} else {
		// before  gap
		// BBBBB........AAAA
		//               ^4
		// BBBBB....AnnnnAAA
		String_move(g->data, g->cont-size, g->cont, g->after-(g->data->length-(start+g->gap)));
		g->after += size;
		g->cont -= size;
	}
	g->gap -= size;
}

void GapBuf_read(const GapBuf* g, Index start, const String s) {
	Index size = s->length;
	if (start+size > g->data->length)
		return; //error
	if (start+size <= g->before) {
		String_trans(s, 0, g->data, start, size);
	} else if (start >= g->before) {
		String_trans(s, 0, g->data, start+g->gap, size);
	} else {
		Index before = g->before-start;
		String_trans(s, 0, g->data, start, before);
		String_trans(s, before, g->data, g->cont, size-before);
	}
}

void GapBuf_moveGap(GapBuf* g, Index gap) {
	if (gap < g->before) { //gap is moving backwards
		// BBBBBBBBBBB..AAAAAAAAAAA
		//
		// BBBBBBB..BBBBAAAAAAAAAAA
		
		// BBBBB........AAAA
		//   ^     
		// BB........BBBAAAA
		Index amount = g->before - gap;
		String_move(g->data, g->cont-amount, gap, amount);
		g->before -= amount;
		g->after += amount;
		g->cont -= amount;
	} else if (gap > g->data->length) {
		// error
	} else if (gap > g->before) { //moving forwards
		// BBBBB........AAAA
		//        ^         
		// BBBBBAA........AA
		Index amount = gap - g->before;
		String_move(g->data, g->gap, g->cont, amount);
		g->before += amount;
		g->after -= amount;
		g->cont += amount;
	}
}

void GapBuf_write(const GapBuf* g, Index start, const String s) {
	Index size = s->length;
	if (start+size > g->data->length)
		return; //error
	if (start+size <= g->before) {
		String_rtrans(s, 0, g->data, start, size);
	} else if (start >= g->before) {
		String_rtrans(s, 0, g->data, start+g->gap, size);
	} else {
		Index before = g->before-start;
		String_rtrans(s, 0, g->data, start, before);
		String_rtrans(s, before, g->data, g->cont, size-before);
	}
}

void GapBuf_display(const GapBuf* g, WINDOW* w) {
	int i;
	cchar_t wc;
	init_pair(1, 19, 6);
	init_pair(2, 5, 9);
	
	setcchar(&wc, L" ", 1, 0x0000, &(int){1});
	int utf8 = 0;
	int utf8char = -2;
	//todo: set face for each char
	for (i=0; i<g->before+g->after; i++) {
		unsigned char c = g->data->text[i<g->before ? i : i+g->gap];
		if (utf8) { //continuation bytes
			if (c>=0b10000000 && c<=0b10111111) {
				utf8char <<= 6;
				utf8char |= c & 0b00111111;
				utf8--;
			} else {
				utf8 = 0;
				utf8char = -1;
				// invalid
			}
		} else {
			if (c<=0b01111111) {
				// ascii
				utf8char = c;
			} else if (c<=0b10111111) {
				// continuation (invalid here)
				utf8char = -1;
			} else if (c<=0b11011111) {
				utf8char = c & 0b00011111;
				utf8 = 1;
			} else if (c<=0b11101111) {
				utf8char = c & 0b00001111;
				utf8 = 2;
			} else if (c<=0b11110111) {
				utf8char = c & 0b00000111;
				utf8 = 3;
			} else {
				//invalid
				utf8char = -1;
			}
		}
		if (!utf8) {
		print:;
			if (utf8char == -1) {
				setcchar(&wc, L" ", 1, 0x0000, &(int){2});
				utf8char = '?';
			}
			wc.chars[0] = utf8char;
			wadd_wch(w, &wc);
			if (utf8char==-1) {
				setcchar(&wc, L" ", 1, 0x0000, &(int){1});
			}
			utf8char = -2;
		}
	}
	if (utf8char!=-2) {
		//unfinished utf8 sequence
		utf8char = -1;
		goto print;
	}
}

void GapBuf_debug(const GapBuf* g) {
	fwrite(g->data->text, 1, g->before, stdout);
	printf("|<-%ld->|", g->gap);
	fwrite(&g->data->text[g->cont], 1, g->after, stdout);
	//printf(" a:%ld", g->after);
	fputc('\n', stdout);
}
