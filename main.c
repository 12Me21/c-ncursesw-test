#include "gap-buffer.h"

int main(void) {
	GapBuf b;
	GapBuf_init(&b, 1000);

	String s = String_new(9);
	memcpy(s.text, "test 1234", 9);

	String s2 = String_new(5);
	memcpy(s2.text, "-=#=-", 5);

	GapBuf_insert(&b, 0, 9);
	GapBuf_write(&b, 0, s);
	
	GapBuf_insert(&b, 2, 5);
	GapBuf_write(&b, 2, s2);

	String s3 = String_new(9+5);
	GapBuf_read(&b, 0, s3);
	String_print(s3);

	return 0;
}
