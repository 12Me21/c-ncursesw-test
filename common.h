#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef uint32_t Color;

// macros to be placed within structs
// MM = memory management

// structs which have corresponding lua types
#define MM_LUA int refcount; void* userdata;

// structs which don't have lua types but are reference counted
#define MM_REFCOUNT int refcount;
