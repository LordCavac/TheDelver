#include "scenes/overworld.h"
#include "core.h"
#include "raylib.h"
#include "spriterenderer.h"
#include "camera.h"

void updateOverworld(GameState *game) {
	if (IsKeyPressed(KEY_W) && game->worldMap[(game->player.position.y - 1) * WORLDX + game->player.position.x].passable) game->player.position.y--;
	if (IsKeyPressed(KEY_S) && game->worldMap[(game->player.position.y + 1) * WORLDX + game->player.position.x].passable) game->player.position.y++;
	if (IsKeyPressed(KEY_A) && game->worldMap[game->player.position.y * WORLDX + game->player.position.x - 1].passable) game->player.position.x--;
	if (IsKeyPressed(KEY_D) && game->worldMap[game->player.position.y * WORLDX + game->player.position.x + 1].passable) game->player.position.x++;

}

void drawOverworld(GameState *game) {
	// To get a resizable window with properly scaled tiles we need to draw a texture off screen then bring it into the camera
	// Set up the render texture
	static RenderTexture2D gameTexture = { 0 };
	if (gameTexture.id == 0) {
		gameTexture = LoadRenderTexture(VISIBLETILESX * TILESIZE, VISIBLETILESY * TILESIZE);
		SetTextureFilter(gameTexture.texture, TEXTURE_FILTER_POINT);
	}

	const i32 targetWidth = VISIBLETILESX * TILESIZE;
	const i32 targetHeight = VISIBLETILESY * TILESIZE;

	updateCamera(&game->camera, &game->player, WORLDX * TILESIZE, WORLDY * TILESIZE);

	// Render to off-screen texture
	BeginTextureMode(gameTexture);
	ClearBackground(BLACK);
		BeginMode2D(game->camera);
			// Draw map
			for (i32 x = 0; x < WORLDX; x++) {
				for (i32 y = 0; y < WORLDY; y++) {
					drawTile(game->worldMap[y * WORLDX + x], x * 16, y * 16);
				}
			}
		
			// Draw player
			drawPlayer(game->player.spriteIdx, game->player.position.x * TILESIZE, game->player.position.y * TILESIZE);
		EndMode2D();
	EndTextureMode();

	// Draw to the screen properly scaled
	BeginDrawing();
		ClearBackground(BLACK);
		
		f32 windowWidth = (f32)GetScreenWidth();
		f32 windowHeight = (f32)GetScreenHeight();
		f32 targetRatio = (f32)targetWidth / targetHeight;
		f32 windowRatio = windowWidth / windowHeight;

		Rectangle source = { 0, 0, (f32)targetWidth, -targetHeight };
		Rectangle dest;
		Vector2 origin = { 0 };
		
		if (windowRatio >= targetRatio) {
			//Window needs pillarbox
			f32 scale = windowHeight / targetHeight;
			f32 width = targetWidth * scale;
			dest = (Rectangle){ (windowWidth - width) / 2.0f, 0, width, windowHeight };
		} else {
			// Window needs letterbox
			f32 scale = windowWidth / targetWidth;
			f32 height = targetHeight * scale;
			dest = (Rectangle){0, (windowHeight - height) / 2.0f, windowWidth, height};
		}

		DrawTexturePro(gameTexture.texture, source, dest, origin, 0.0f, WHITE);
	EndDrawing();
}
