#ifndef SPRITE_RENDERER
#define SPRITE_RENDERER

#include "tile.h"
#include "types.h"

void drawTile(Tile tile, i32 screenX, i32 screenY);
void drawPlayer(i32 spriteIdx, i32 screenX, i32 screenY);

#endif
