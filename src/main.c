#include "core.h"
#include "raylib.h"
#include "types.h"
#include "tile.h"
#include "worldgen.h"
#include "arena.h"
#include "spriteatlas.h"
#include "scenemanager.h"
#include "player.h"
#include "camera.h"
#include <stdio.h>

// Maximum characters for world seed
#define MAX_INPUT 4

int main() {
	Arena *game = arenaCreate(MB(64));

	GameState *gameState = arenaAlloc(game, sizeof(GameState));
	if (!gameState) return 1;

	gameState->worldMap = arenaAlloc(game, sizeof(Tile) * WORLDX * WORLDY);
	if (!gameState->worldMap) return 1;
	
	generateWorld(gameState->worldMap, WORLDX, WORLDY);
	
	InitWindow(1920, 1080, "The Delver");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	
	loadSpriteAtlas("../resources/TheDelverSpriteSheet.png");

	gameState->scene = OVERWORLD;
	initCamera(&gameState->camera);
	initPlayer(&gameState->player);
	gameState->camera.target = (Vector2){ gameState->player.position.x, gameState->player.position.y };

	SetTargetFPS(60);
	while (!WindowShouldClose()) 
	{	
		updateScene(gameState);
		drawScene(gameState);
	}

	CloseWindow();

	return 0;
}

