#pragma once
#include "common.h"
#include "term.h"

typedef struct Face {
	uint8_t color;
	uint8_t background; // allow transparency (i.e. show bg color of buffer through)
	Attr attr;
	/*bool bold;
	bool italic;
	bool underline;
	bool strike;*/
} Face;

/*typedef struct Faces {
	Face faces[256];
	} Faces;*/
