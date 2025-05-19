#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "types.h"
#include "tile.h"
#include "arena.h"

typedef struct Path {
	Vec2i *points;
	i32 count;
} Path;

// A* implementation for finding shortest path
Path findPath(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp);
void freePath (Path *path);

// Uses DFS to determine if a location can be reached
bool canReach(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp);
bool canReachFromAdjacent(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp);

#endif
