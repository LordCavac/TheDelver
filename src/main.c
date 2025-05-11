#include "arena.h"
#include "core.h"
#include "raylib.h"
#include "spriteatlas.h"
#include "worldgen.h"
#include <stdio.h>

// Maximum characters for world seed
#define MAX_INPUT 4

typedef enum GameScene { TITLE = 0, OVERWORLD, DUNGEON, TOWN, ENDING } GameScene;

typedef struct Vec2i { i32 x; i32 y; } Vec2i;

typedef struct Player {
	i32 spriteIdx;
	Vec2i position;
	i32 hitpoints;
	i32 strength;
	i32 dexterity;
	i32 stamina;
	i32 wisdom;
	i32 gold;
	f32 food;
} Player;


typedef struct GameState {
	i32 seed; // used to store the random seed chosen by user
	GameScene scene; // current screen the game is in
} GameState;

void updateCamera(Camera2D *camera, Player *player) {
	f32 halfWidth = GetScreenWidth() / (2.0f * camera->zoom);
	f32 halfHeight = GetScreenHeight() / (2.0f * camera->zoom);

	f32 mapWidth = WORLDX * TILESIZE;
	f32 mapHeight = WORLDY * TILESIZE;

	f32 targetX = player->position.x * TILESIZE;
	f32 targetY = player->position.y * TILESIZE;

	targetX = CLAMP(targetX, halfWidth, mapWidth - halfWidth);
	targetY = CLAMP(targetY, halfHeight, mapHeight - halfHeight);

	camera->target = (Vector2){ targetX, targetY };
}

int main() {
	Arena *game = arenaCreate(MB(64));

	GameState *gameState = arenaAlloc(game, sizeof(GameState));
	if (!gameState) return 1;

	Tile *worldMap = arenaAlloc(game, sizeof(Tile) * WORLDX * WORLDY);
	if (!worldMap) return 1;
	
	generateWorld(worldMap, WORLDX, WORLDY);
	
	InitWindow(1920, 1080, "The Delver");
	
	loadSpriteAtlas("../resources/TheDelverSpriteSheet.png");

	GameScene currentScene = TITLE;
	
	Camera2D camera = {.offset = {GetScreenWidth() / 2.0, GetScreenHeight() / 2.0}, .target = 0.0, .rotation = 0, .zoom = 5.0f};
	Player player = {0, (Vec2i){WORLDX / 2, WORLDY / 2}, 10, 10, 10, 10, 10, 0, 100};
	
	SetTargetFPS(60);
	while (!WindowShouldClose()) 
	{	
		updateCamera(&camera, &player);

		BeginDrawing();
		BeginMode2D(camera);
		ClearBackground(BLACK);

		// Draw map
		for (i32 x = 0; x < WORLDX; x++) {
			for (i32 y = 0; y < WORLDY; y++) {
				drawTile(worldMap[y * WORLDX + x], x * 16, y * 16);
			}
		}
		
		// Draw player
		drawPlayer(player.spriteIdx, player.position.x * TILESIZE, player.position.y * TILESIZE);

		if (IsKeyPressed(KEY_W) && worldMap[(player.position.y - 1) * WORLDX + player.position.x].passable) player.position.y--;
		if (IsKeyPressed(KEY_S) && worldMap[(player.position.y + 1) * WORLDX + player.position.x].passable) player.position.y++;
		if (IsKeyPressed(KEY_A) && worldMap[player.position.y * WORLDX + player.position.x - 1].passable) player.position.x--;
		if (IsKeyPressed(KEY_D) && worldMap[player.position.y * WORLDX + player.position.x + 1].passable) player.position.x++;
		
		EndMode2D();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}

