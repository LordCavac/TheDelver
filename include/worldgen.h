#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "tile.h"
#include "types.h"
#include "arena.h"
#include "core.h"

// Error propagation for when a edge tile was chosen as a non-mountain to keep natural looking edges
#define INFLUENCERADIUS 10
#define INFLUENCESTRENGTH 10

typedef enum FalloffType {LINEAR, SIGMOID, QUADRATIC, INVERSE} FalloffType;
static FalloffType falltype = QUADRATIC;

void generateWorld(Tile *tileMap, i32 rows, i32 cols);
void placeDungeons(Tile *tileMap, i32 rows, i32 cols, GameState *game, i32 numDungeons, Arena *temp);

#endif
