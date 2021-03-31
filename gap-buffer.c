#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "gap-buffer.h"

void GapBuf_init(GapBuf* g, Index size) {
	*g = (GapBuf){
		.text = malloc(size),
		.props = malloc(size*sizeof(CharProp)),
		.size = size,
		.before = 0,
		.gap = size,
		.after = 0,
		.cont = size,
		.length = 0,
	};
}

static void move(GapBuf* g, RawIndex dest, RawIndex src, Index length) {
	memmove(&g->text[dest], &g->text[src], length*sizeof(char));
	memmove(&g->props[dest], &g->props[src], length*sizeof(CharProp));
}

// remove space from a gap buffer `g`
// at real index `start`, with size `size`
// (todo: specifying new gap position?)
void GapBuf_delete(GapBuf* g, Index start, Index size) {
	if (start+size > g->before+g->after) {
		// error
		return;
	}
	
	if (start+size<=g->before) {
		// remove space before the gap
		// AAAAB.....CCCC
		//   ^^
		// AAB.......CCCC
		move(g, start+size, start, g->before-(start+size));
		g->before -= size;
	} else if (start>=g->before) {
		// remove space after the gap
		// AAAAA...BBCCCC
		//           ^^^
		// AAAAA......BBC
		move(g, g->cont, g->cont+size, start-g->cont);
		g->after -= size;
		g->cont += size;
	} else {
		// remove space that spans the gap
		// AAABB...CCCDDDD
		//    ^^...^^^
		// AAA........DDDD
		Index before = g->before-start;
		Index after = size-before;
		g->before -= before;
		g->after -= after;
		g->cont += after;
	}
	g->gap += size;
	g->length -= size;
}

// insert new space into a gap buffer `g`
// at real index `start`, with size `size`
// (todo: specifying new gap position?)
void GapBuf_insert(GapBuf* g, Index start, Index size) {
	if (g->gap < size) {
		//error
		return;
	}
	// BBBBB........AAAA
	//  ^6
	// BnnnnnnBBBB..AAAA
	if (start<=g->before) {
		move(g, start+size, start, g->before-start);
		g->before += size;
	} else {
		// before  gap
		// BBBBB........AAAA
		//               ^4
		// BBBBB....AnnnnAAA
		move(g, g->cont-size, g->cont, start+g->gap-g->cont);
		g->after += size;
		g->cont -= size;
	}
	g->gap -= size;
	g->length += size;
}

// move the start of the gap to `gap`
// uses raw index
// todo: make this use a real index
void GapBuf_moveGap(GapBuf* g, Index gap) {
	Index amount;
	if (gap < g->before) { //gap is moving backwards
		// BBBBB........AAAA
		//   ^     
		// BB........BBBAAAA
		amount = gap-g->before;
		move(g, g->cont+amount, gap, -amount);
	} else if (gap > g->length) {
		// error
		return;
	} else if (gap > g->before) { //moving forwards
		// BBBBB........AAAA
		//      ........  ^ 
		// BBBBBAA........AA
		amount = gap-g->before + g->gap ;
		move(g, g->gap, g->cont, amount);
	} else
		return; //unmoved
	g->before += amount;
	g->after -= amount;
	g->cont += amount;
}

// copy data from the text of a gapbuffer into a char array
// uses a raw index
static void readText(const GapBuf* g, RawIndex start, char* dest, Index length) {
	memmove(dest, &g->text[start], length);
}

// copy data from a char* into the text part of a gap buffer
// and zero out the properties for these characters.
// uses a raw index
static void writeText(const GapBuf* g, RawIndex start, const char* source, Index size) {
	memmove(&g->text[start], source, size);
	memset(&g->props[start], 0, size*sizeof(CharProp));
}

void GapBuf_read(const GapBuf* g, Index start, const String s) {
	Index size = s->length;
	if (start+size > g->length)
		return; //error
	if (start+size <= g->before) {
		readText(g, start, s->text, size);
	} else if (start >= g->before) {
		readText(g, start+g->gap, s->text, size);
	} else {
		Index before = g->before-start;
		readText(g, start, s->text, before);
		readText(g, g->cont, &s->text[before], size-before);
	}
}

void GapBuf_write(const GapBuf* g, Index start, const String s) {
	Index size = s->length;
	if (start+size > g->length)
		return; //error
	if (start+size <= g->before) {
		writeText(g, start, s->text, size);
	} else if (start >= g->before) {
		writeText(g, start+g->gap, s->text, size);
	} else {
		Index before = g->before-start;
		writeText(g, start, s->text, before);
		writeText(g, g->cont, &s->text[before], size-before);
	}
}

void GapBuf_debug(const GapBuf* g) {
	fwrite(g->text, 1, g->before, stdout);
	printf("|<-%ld->|", g->gap);
	fwrite(&g->text[g->cont], 1, g->after, stdout);
	//printf(" a:%ld", g->after);
	fputc('\n', stdout);
}
