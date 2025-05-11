#include "spriteatlas.h"
#include "core.h"
#include "raylib.h"

Texture2D spriteSheet;
Rectangle spriteRects[MAXSPRITES];

void loadSpriteAtlas(const char *path) {
	spriteSheet = LoadTexture(path);

	for (i32 i = 0; i < MAXSPRITES; i++) {
		i32 row = i / SPRITESPERROW;
		i32 col = i % SPRITESPERROW;

		spriteRects[i] = (Rectangle){
			.x = col * SPRITESIZE,
			.y = row * SPRITESIZE,
			.width = SPRITESIZE,
			.height = SPRITESIZE,
		};
	}
}

void unloadSpriteAtlas(void) {
	UnloadTexture(spriteSheet);
}
