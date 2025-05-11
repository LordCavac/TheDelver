#ifndef SPRITE_ATLAS_H
#define SPRITE_ATLAS_H

#include "raylib.h"

#define SPRITESIZE 16
#define SPRITESPERROW 8
#define MAXSPRITES 64

extern Texture2D spriteSheet;
extern Rectangle spriteRects[MAXSPRITES];

void loadSpriteAtlas(const char *path);
void unloadSpriteAtlas(void);

#endif
