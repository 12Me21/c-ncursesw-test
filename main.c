#include <string.h>
#include "gap-buffer.h"

int main(void) {
	// create new gap buffer with space for 1000 chars
	GapBuf b;
	GapBuf_init(&b, 1000);

	// insert "test ABCD" into `g`
	String s = String_new(9);
	memcpy(s.text, "test ABCD", 9);
	GapBuf_insert(&b, 0, 9);
	GapBuf_write(&b, 0, s);
	// prints: `test ABCD|<-991->|` (showing the position+size of the gap)
	GapBuf_debug(&b);

	// move the gap
	GapBuf_moveGap(&b, 6);
	// prints: `test A|<-991->|BCD` (now the gap is in the middle of the text)
	GapBuf_debug(&b);

	// insert "-=#=-" into `g` at index 2
	String s2 = String_new(5);
	memcpy(s2.text, "-=#=-", 5);
	GapBuf_insert(&b, 2, 5);
	GapBuf_write(&b, 2, s2);
	// prints: `te-=#=-st A|<-986->|BCD`
	GapBuf_debug(&b);

	GapBuf_insert(&b, 13, 5);
	GapBuf_write(&b, 13, s2);
	GapBuf_debug(&b);
	
	// prints "te-=#=-st 1234" (the buffer contents)
	String s3 = String_new(9+5+5);
	GapBuf_read(&b, 0, s3);
	String_print(s3);

	return 0;
}
