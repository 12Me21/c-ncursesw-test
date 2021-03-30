#include "buffer.h"

void Buffer_display(const Buffer* b, WINDOW* w) {
	const GapBuf* g = &b->gapBuf;
	Index i;
	cchar_t wc;
	init_pair(1, 19, 6);
	init_pair(2, 5, 9);
	
	setcchar(&wc, L" ", A_UNDERLINE, 0, &(int){1});
	int utf8 = 0;
	int utf8char = -2;
	//todo: set face for each char
	for (i=0; i<g->before+g->after; i++) {
		Index ix = i<g->before ? i : i+g->gap;
		unsigned char c = g->data->text[ix];
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
			CharProp prop = g->data->props[ix];
			const Face* face = &b->faces[prop];
			wc.attr = face->attr.attr;
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
			/*			if (utf8char == -1) {
				setcchar(&wc, L" ", 1, 0x0000, &(int){2});
				utf8char = '?';
				}*/
			wc.chars[0] = utf8char;
			wadd_wch(w, &wc);
			/*if (utf8char==-1) {
				setcchar(&wc, L" ", 1, 0x0000, &(int){1});
				}*/
			utf8char = -2;
		}
	}
	if (utf8char!=-2) {
		//unfinished utf8 sequence
		utf8char = -1;
		goto print;
	}
}
