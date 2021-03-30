#include <stdbool.h>
#include <stdint.h>

typedef uint32_t Color;

typedef struct Face {
	Color color;
	Color background; // allow transparency (i.e. show bg color of buffer through)
	bool bold;
	bool italic;
	bool underline;
	bool strike;
} Face;

typedef struct Faces {
	Face faces[256];
} Faces;
