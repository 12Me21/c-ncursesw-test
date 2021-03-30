#pragma once
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

// ok wait
// why are we storing properties in our string type
// they are used only for displaying,
// and can't be uh
// freely copied between buffers
// since the font table won't
// ghhhh

typedef uint8_t CharProp;

typedef ptrdiff_t Index;

// so, this data needs to be garbage collected..
// let's use reference counting.

typedef const struct String {
	char* text;
	CharProp* props;
	Index length;
	int refs;
}* String;

String String_new(Index size);
void String_free(String str);
void String_print(String str);
void String_move(String str, Index dest, Index src, Index length);
void String_trans(String dest, Index d, String src, Index s, Index length);
void String_rtrans(String dest, Index d, String src, Index s, Index length);
