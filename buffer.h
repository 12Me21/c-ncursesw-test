#pragma once
#include "gap-buffer.h"
#include "face.h"

typedef struct Buffer {
	GapBuf gapBuf;
	Face faces[256];
	// - information about mode
	// - fonts?
	// - filename
	// - encoding
	// - undo info
} Buffer;

void Buffer_display(const Buffer* b, WINDOW* w);
