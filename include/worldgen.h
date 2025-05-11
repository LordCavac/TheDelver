#ifndef WORLDGEN_H
#define WORLDGEN_H

#include "tile.h"
#include "types.h"

// Boundary Enforcement for worldgen
#define MARGIN 4
// Error propagation for when a edge tile was chosen as a non-mountain to keep natural looking edges
#define INFLUENCERADIUS 10
#define INFLUENCESTRENGTH 10

typedef enum FalloffType {LINEAR, SIGMOID, QUADRATIC, INVERSE} FalloffType;
static FalloffType falltype = QUADRATIC;

void generateWorld(Tile *tileMap, i32 rows, i32 cols);

#endif
