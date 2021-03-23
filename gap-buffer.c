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
		String_move(g->data, g->cont-size, g->cont, g->after-(g->data.length-(start+g->gap)));
		g->after += size;
		g->cont -= size;
	}
	g->gap -= size;
}

void GapBuf_read(const GapBuf* g, Index start, const String s) {
	Index size = s.length;
	if (start+size > g->data.length)
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
	} else if (gap > g->data.length) {
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
	Index size = s.length;
	if (start+size > g->data.length)
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

void GapBuf_debug(const GapBuf* g) {
	fwrite(g->data.text, 1, g->before, stdout);
	printf("|<-%ld->|", g->gap);
	fwrite(&g->data.text[g->cont], 1, g->after, stdout);
	//printf(" a:%ld", g->after);
	fputc('\n', stdout);
}
