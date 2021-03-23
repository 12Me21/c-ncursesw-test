#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

String String_new(Index size) {
	return (String){
		.text = malloc(size*sizeof(char)),
		.props = malloc(size*sizeof(CharProp)),
		.length = size,
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

void String_print(const String str) {
	fwrite(str.text, 1, str.length, stdout);
	fputc('\n', stdout);
}
