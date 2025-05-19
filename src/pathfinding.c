#include "pathfinding.h"
#include "stack.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	Vec2i position;
	i32 gcost;
	i32 hcost;
	struct Node *parent;
	bool inOpen;
	bool inClosed;
} Node;

static i32 heuristic(Vec2i a, Vec2i b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

static bool isValid(i32 x, i32 y, i32 rows, i32 cols) {
	return x >= 0 && y >= 0 && x < cols && y < rows;
}

Path findPath(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp) {
	i32 totalTiles = rows * cols;
	Node *nodes = (Node *)arenaAlloc(temp, sizeof(Node) * totalTiles);
	if (!nodes) return (Path){0};

	// Initialize nodes
	for (i32 y = 0; y < rows; y++) {
		for (i32 x = 0; x < cols; x++) {
			Node *n = &nodes[y * cols + x];
			n->position = (Vec2i){x, y};
			n->gcost = INT_MAX;
			n->hcost = heuristic((Vec2i){x, y}, goal);
			n->parent = NULL;
			n->inOpen = false;
			n->inClosed = false;
		}
	}
	
	Node *startNode = &nodes[start.y * cols + start.x];
	startNode->gcost = 0;
	startNode->inOpen = true;

	// Open list can be optimized later if needed
	Vec2i openList[rows * cols];
	i32 openCount = 0;
	openList[openCount++] = start;

	const Vec2i directions[4] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

	while (openCount > 0) {
		i32 bestIdx = 0;
		i32 bestCost = INT_MAX;
		for (i32 i = 0; i < openCount; i++) {
			Node *n = &nodes[openList[i].y * cols + openList[i].x];
			i32 f = n->gcost + n->hcost;
			if (f < bestCost) {
				bestCost = f;
				bestIdx = i;
			}
		}

		Vec2i currentPos = openList[bestIdx];
		Node *current = &nodes[currentPos.y * cols + currentPos.x];

		// Remove from open list
		openList[bestIdx] = openList[--openCount];
		current->inOpen = false;
		current->inClosed = true;

		if (currentPos.x == goal.x && currentPos.y == goal.y) {
			// Path found
			Path path = {0};
			Node *n = current;
			while(n) {
				path.count++;
				n = n->parent;
			}
			path.points = malloc(sizeof(Vec2i) * path.count);
			n = current;
			for (i32 i = path.count - 1; i >= 0; i--) {
				path.points[i] = n->position;
				n = n->parent;
			}
			free(nodes);
			return path;
		}

		// Check for neighbors
		for (i32 i = 0; i < 4; i++) {
			Vec2i dir = directions[i];
			i32 nx = currentPos.x + dir.x;
			i32 ny = currentPos.y + dir.y;

			if (!isValid(nx, ny, rows, cols)) continue;

			Node *neighbor = &nodes[ny * cols + nx];
			const Tile *tile = &map[ny * cols + nx];
			if (!tile->passable || neighbor->inClosed) continue;

			i32 tenativeG = current->gcost + 1;

			if (!neighbor->inOpen || tenativeG < neighbor->gcost) {
				neighbor->parent = current;
				neighbor->gcost = tenativeG;

				if (!neighbor->inOpen) {
					openList[openCount++] = neighbor->position;
					neighbor->inOpen = true;
				}
			}
		}
	}

	// No Path Found
	free(nodes);
	return (Path){0};
}

void freePath (Path *path) { free(path); }

bool canReach(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp) {
	const Vec2i directions[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	bool *visited = arenaAlloc(temp, sizeof(bool) * rows * cols);
	if (!visited) return false;
	memset(visited, 0, sizeof(bool) * rows * cols);

	Stack stack;
	stackInit(&stack, sizeof(Vec2i), rows * cols);
	stackPush(&stack, &start);

	while (!stackIsEmpty(&stack)) {
		Vec2i current;
		stackPop(&stack, &current);

		if (current.x == goal.x && current.y == goal.y) return true;

		i32 idx = current.y * cols + current.x;
		if (visited[idx]) continue;
		visited[idx] = true;

		for (i32 i = 0; i < 4; i++) {
			Vec2i next = { current.x + directions[i].x, current.y + directions[i].y };
			if (!isValid(next.x, next.y, rows, cols)) continue;

			i32 nextIdx = next.y * cols + next.x;
			if (!visited[nextIdx] && map[nextIdx].passable) {
				stackPush(&stack, &next);
			}
		}
	}

	return false;
}

bool canReachFromAdjacent(const Tile *map, i32 rows, i32 cols, Vec2i start, Vec2i goal, Arena *temp) {
	const Vec2i directions[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

	for (i32 i = 0; i < 4; i++) {
		Vec2i adj = { start.x + directions[i].x, start.y + directions[i].y };
		if (!isValid(adj.x, adj.y, rows, cols)) continue;

		i32 idx = adj.y * cols + adj.x;
		if (map[idx].passable) {
			arenaReset(temp); // Clear temp before each path search
			if (canReach(map, rows, cols, adj, goal, temp)) {
				return true;
			}
		}
	}
	return false;
}
