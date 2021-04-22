#pragma once
#include "common.h"
#include "string.h"

typedef uint8_t CharProp;

typedef Index RawIndex; // raw index. this is used when referring to positions within the buffer of a gap buffer. Index is used for lengths and "real" indexes (ie. corrected for the gap position). Rindex values should never be exposed externally.
// before the gap: index = rawindex
// inside the gap: no index exists
// after the gap: index = rawindex + .gap
// ex:
//               gap
// index:    [012...3456]
// rawindex: [0123456789]

typedef struct GapBuf {
	char* text;
	CharProp* props;
	Index size;
	
	Index before; // number of chars before the gap
	Index gap; // size of gap

	// (redundant ↓)
	Index after; // number of chars after gap
	RawIndex cont; // position of end of gap
	Index length;
	
	Index* frontLines;
	Index* backLines;
} GapBuf;

void GapBuf_init(GapBuf* g, Index size);
void GapBuf_insert(GapBuf* g, Index start, Index size);
void GapBuf_read(const GapBuf* g, Index start, const String s);
void GapBuf_write(const GapBuf* g, Index start, const String s);
void GapBuf_moveGap(GapBuf* g, Index gap);
void GapBuf_debug(const GapBuf* g);
