#include "spriterenderer.h"
#include "spriteatlas.h"

void drawTile(Tile tile, i32 screenX, i32 screenY) {
	if (!tile.debugHighlight) DrawTextureRec(spriteSheet, spriteRects[tile.spriteIdx], (Vector2){screenX, screenY}, WHITE);
	else DrawTextureRec(spriteSheet, spriteRects[63], (Vector2){screenX, screenY}, WHITE);
}

void drawPlayer(i32 spriteIdx, i32 screenX, i32 screenY) {
	DrawTextureRec(spriteSheet, spriteRects[spriteIdx], (Vector2){screenX, screenY}, WHITE);
}

