#include "arena.h"
#include "core.h"

Arena *arenaCreate(size_t size) {
	void *block = malloc(sizeof(Arena) + size);
	if (!block) return NULL;

	Arena *arena = (Arena *)block;
	arena->base = (u8 *)block + sizeof(Arena);
	arena->capacity = size;
	arena->offset = 0;
	return arena;
}

void arenaInit(Arena *arena, void *backingMemory, size_t size) {
	arena->base = (u8 *)backingMemory;
	arena->capacity = size;
	arena->offset = 0;
}

void *arenaAlloc(Arena *arena, size_t size) {
	if (arena->offset + size > arena->capacity) return NULL;
	void *ptr = arena->base + arena->offset;
	arena->offset += size;
	return ptr;
}

void arenaReset(Arena *arena) {
	arena->offset = 0;
}

void arenaDestroy(Arena *arena) {
	free(arena);
}
