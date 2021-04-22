#pragma once
#include "common.h"
#include "gap-buffer.h"
#include "face.h"

typedef struct Buffer {
	GapBuf gapBuf;
	Face faces[256]; // ok so, different modes will use different faces and we need to allocate them etc.
	// - information about mode
	// - fonts?
	// - filename
	// - encoding
	// - undo info
} Buffer;

void Buffer_display(const Buffer* b, WINDOW* w);
