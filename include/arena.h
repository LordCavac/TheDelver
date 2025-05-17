#ifndef ARENA_H
#define ARENA_H

#include "types.h"
#include <stddef.h>

// Defnitions for memory sizes
#define KB(x) ((x) * 1024UL)
#define MB(x) (KB(x) * 1024UL)
#define GB(x) (MB(x) * 1024UL)

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
