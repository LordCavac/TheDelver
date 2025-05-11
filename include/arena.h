#ifndef ARENA_H
#define ARENA_H

#include "types.h"
#include <stddef.h>

typedef struct Arena {
	u8 *base;
	size_t capacity;
	size_t offset;
} Arena;

Arena *arenaCreate(size_t size);
void arenaInit(Arena *arena, void *backingMemory, size_t size);
void *arenaAlloc(Arena *arena, size_t allocSize);
void arenaReset(Arena *arena);
void arenaDestroy(Arena *arena);

#endif
