#pragma once
#include <stddef.h>

typedef short CharProp;

typedef ptrdiff_t Index;

typedef struct String {
	char* text;
	CharProp* props;
	Index length;
} String;

String String_new(Index size);
void String_print(String str);
void String_move(const String str, Index dest, Index src, Index length);
void String_trans(const String dest, Index d, const String src, Index s, Index length);
void String_rtrans(const String dest, Index d, const String src, Index s, Index length);
