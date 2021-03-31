#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string.h"

void* memdup(void* data, size_t size) {
	return memcpy(malloc(size), data, size);
}

void String_free(String str) {
	if (str->refs<=1) {
		free(str->text);
		//		free(str->props);
		free((void*)str);
	} else
		((struct String*)str)->refs--;
}

String String_new(Index size) {
	return memdup(&(struct String){
		.text = malloc(size*sizeof(char)),
		//		.props = calloc(size,sizeof(CharProp)),
		.refs = 1,
		.length = size,
	}, sizeof(struct String));
}

void String_move(String str, Index dest, Index src, Index length) {
	memmove(&str->text[dest], &str->text[src], length);
}

void String_trans(String dest, Index d, String src, Index s, Index length) {
	memmove(&dest->text[d], &src->text[s], length);
}

void String_rtrans(String dest, Index d, String src, Index s, Index length) {
	memmove(&src->text[s], &dest->text[d], length);
}

void String_print(String str) {
	fwrite(str->text, 1, str->length, stdout);
	fputc('\n', stdout);
}
