#include "camera.h"
#include "core.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

void updateCameraZoom(Camera2D *camera, i32 screenWidth, i32 screenHeight) {
	f32 zoomX = (f32)screenWidth / (VISIBLETILESX * TILESIZE);
	f32 zoomY = (f32)screenHeight / (VISIBLETILESY * TILESIZE);

	camera->zoom = fminf(zoomX, zoomY);
}

void initCamera(Camera2D *camera) {
	camera->offset = (Vector2){ (VISIBLETILESX * TILESIZE) / 2.0f, (VISIBLETILESY * TILESIZE) / 2.0f };
	camera->target = (Vector2){ 0 };
	camera->rotation = 0.0f;
	camera->zoom = 1.0f;
}

void updateCamera(Camera2D *camera, const Player *player, i32 mapWidth, i32 mapHeight) {
	f32 halfWidth = (VISIBLETILESX * TILESIZE) / 2.0f;
	f32 halfHeight = (VISIBLETILESY * TILESIZE) / 2.0f;

	f32 targetX = (player->position.x + 0.5f) * TILESIZE;
	f32 targetY = (player->position.y + 0.5f) * TILESIZE;

	targetX = CLAMP(targetX, halfWidth, mapWidth - halfWidth);
	targetY = CLAMP(targetY, halfHeight, mapHeight - halfHeight);

	camera->target = (Vector2){ targetX, targetY };
}

