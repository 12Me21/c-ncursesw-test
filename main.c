#include <string.h>
#include "string.h"
#include "term.h"
#include "gap-buffer.h"
#include "buffer.h"

int main(void) {
	Term_init();
	WINDOW* p = newpad(100,100);
	
	String x = String_new(10);
	String_free(x);
	// create new gap buffer with space for 1000 chars
	GapBuf b;
	GapBuf_init(&b, 100000);

	// insert "test ABCD" into `g`
	String s = String_new(13);
	memcpy(s->text, "test ABCD🖤", 13);
	//String_print(s);
	GapBuf_insert(&b, 0, 13);
	GapBuf_write(&b, 0, s);
	// prints: `test ABCD|<-991->|` (showing the position+size of the gap)
	//GapBuf_debug(&b);

	// move the gap
	GapBuf_moveGap(&b, 6);
	// prints: `test A|<-991->|BCD` (now the gap is in the middle of the text)
	//GapBuf_debug(&b);

	// insert "-=#=-" into `g` at index 2
	String s2 = String_new(5);
	memcpy(s2->text, "-=#=-", 5);
	GapBuf_insert(&b, 2, 5);
	GapBuf_write(&b, 2, s2);
	// prints: `te-=#=-st A|<-986->|BCD`
	//GapBuf_debug(&b);

	GapBuf_insert(&b, 13, 5);
	GapBuf_write(&b, 13, s2);
	//GapBuf_debug(&b);
	
	String s3 = String_new(9+5+5);
	GapBuf_read(&b, 0, s3);

	//String_print(s3);
	Buffer bb = {0};
	bb.gapBuf = b;
	bb.faces[0].attr.underline = 1;
	bb.faces[0].color = 1;
	bb.faces[0].background = 2;
	bb.faces[1].attr.italic = 1;
	b.props[1] = 1;
	b.props[2] = 1;
	Buffer_display(&bb, p);

	prefresh(p, 0,0, 3,3, 20+3,20+3);
	wgetch(p);
	Term_free();
	return 0;
}
