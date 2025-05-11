#include "spriterenderer.h"
#include "spriteatlas.h"

void drawTile(Tile tile, i32 screenX, i32 screenY) {
	DrawTextureRec(spriteSheet, spriteRects[tile.spriteIdx], (Vector2){screenX, screenY}, WHITE);
}

void drawPlayer(i32 spriteIdx, i32 screenX, i32 screenY) {
	DrawTextureRec(spriteSheet, spriteRects[spriteIdx], (Vector2){screenX, screenY}, WHITE);
}

