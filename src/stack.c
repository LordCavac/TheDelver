#include "stack.h"

#include <stdlib.h>
#include <string.h>

void stackInit(Stack *stack, size_t elemSize, size_t initialCapacity) {
	stack->elemSize = elemSize;
	stack->capacity = initialCapacity;
	stack->count = 0;
	stack->data = malloc(elemSize * initialCapacity);
}

void stackInitArena(Stack *stack, Arena *arena, size_t elemSize, size_t capacity) {
	stack->elemSize = elemSize;
	stack->capacity = capacity;
	stack->count = 0;
	stack->data = arenaAlloc(arena, elemSize * capacity);
}

void stackFree(Stack *stack) {
	free(stack->data);
	stack->data = NULL;
	stack->count = 0;
	stack->capacity = 0;
	stack->elemSize = 0;
}

bool stackPush(Stack *stack, const void *elem) {
	if (stack->count == stack->capacity) {
		size_t newCapacity = stack->capacity * 2;
		void *newData = realloc(stack->data, stack->elemSize * newCapacity);
		if (!newData) return false;
		stack->data = newData;
		stack->capacity = newCapacity;
	}
	memcpy((char *)stack->data + stack->count * stack->elemSize, elem, stack->elemSize);
	stack->count++;
	return true;
}

bool stackPop(Stack *stack, void *outElem) {
	if (stack->count == 0) return false;
	stack->count--;
	if (outElem) {
		memcpy(outElem, (char *)stack->data + stack->count * stack->elemSize, stack->elemSize);
	}
	return true;
}

bool stackPeek(const Stack *stack, void *outElem) {
	if (stack->count == 0) return false;
	if (outElem) {
		memcpy(outElem, (char *)stack->data + (stack->count - 1) * stack->elemSize, stack->elemSize);
	}
	return true;
}

bool stackIsEmpty(const Stack *stack) {
	return stack->count == 0;
}

void stackClear(Stack *stack) {
	stack->count = 0;
}
