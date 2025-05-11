#ifndef CORE_H
#define CORE_H

#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "tile.h"
#include "worldgen.h"
#include "arena.h"
#include "spriteatlas.h"
#include "spriterenderer.h"

// Basic world features definitions
#define TILESIZE 16
#define WORLDX 128
#define WORLDY 128

// Clamping function for i32 values
#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

// Defnitions for memory sizes
#define KB(x) ((x) * 1024UL)
#define MB(x) (KB(x) * 1024UL)
#define GB(x) (MB(x) * 1024UL)

#endif
