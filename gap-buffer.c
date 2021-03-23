#include <stdio.h>
#include "gap-buffer.h"

// plan:
// buffer text stored in simple gap buffer

// properties are stored relative to each end, depending on which half they're in

String String_new(Index size) {
	return (String){
		.text = malloc(size*sizeof(char)),
		.props = malloc(size*sizeof(CharProp)),
		.length = size,
	};
}

void GapBuf_init(GapBuf* g, Index size) {
	*g = (GapBuf){
		.data = String_new(size),
		.before = 0,
		.gap = size,
		.after = 0,
		.cont = size,
	};
}

void String_move(const String str, Index dest, Index src, Index length) {
	memmove(&str.text[dest], &str.text[src], length);
	memmove(&str.props[dest], &str.props[src], length);
}

void String_trans(const String dest, Index d, const String src, Index s, Index length) {
	memmove(&dest.text[d], &src.text[s], length);
	memmove(&dest.props[d], &src.props[s], length);
}

void String_rtrans(const String dest, Index d, const String src, Index s, Index length) {
	memmove(&src.text[s], &dest.text[d], length);
	memmove(&src.props[s], &dest.props[d], length);
}


// todo: specifying new gap position
void GapBuf_insert(GapBuf* g, Index start, Index size) {
	if (g->gap >= size) {
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
			String_move(g->data, g->cont-size, g->cont, g->after-(g->data.length-start));
			g->after += size;
			g->cont -= size;
		}
		g->gap -= size;
	}
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

void String_print(const String str) {
	fwrite(str.text, 1, str.length, stdout);
	fputc('\n', stdout);
}
