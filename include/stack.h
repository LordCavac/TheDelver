#ifndef STACK_H
#define STACK_H

#include "arena.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct Stack {
	void *data;
	size_t elemSize;
	size_t capacity;
	size_t count;
} Stack;

void stackInit(Stack *stack, size_t elemSize, size_t initialCapacity);
void stackInitArena(Stack *stack, Arena *arena, size_t elemSize, size_t capacity);
void stackFree(Stack *stack);

bool stackPush(Stack *stack, const void *elem);
bool stackPop(Stack *stack, void *outElem);
bool stackPeek(const Stack *stack, void *outElem);
bool stackIsEmpty(const Stack *stack);
void stackClear(Stack *stack);

#endif
