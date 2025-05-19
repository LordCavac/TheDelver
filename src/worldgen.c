#include "worldgen.h"
#include "arena.h"
#include "core.h"
#include "pathfinding.h"

#include "raylib.h"


#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

f32 computeFalloff(f32 distance, f32 radius) {
	f32 x = CLAMP((radius - distance) / radius, 0.0f, 1.0f);

	switch (falltype) {
		case LINEAR: 
			return x;
		case SIGMOID:
			return 1.0f / (1.0f + expf(-INFLUENCESTRENGTH * (x - 0.5f)));
		case QUADRATIC:
			return x * x;
		case INVERSE:
			return 1.0f - (1.0 / (1.0f + x * 10.0f));
		default:
			return x;
	}
}

void propagateError(Tile *tileMap, Image *noise, Color *pixels, i32 rows, i32 cols, i32 x, i32 y) {
	for (i32 errY = -INFLUENCERADIUS; errY <= INFLUENCERADIUS; errY++) {
		for (i32 errX = -INFLUENCERADIUS; errX <= INFLUENCERADIUS; errX++) {
			i32 nx = x + errX;
			i32 ny = y + errY;

			if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
				f32 dist = sqrtf(errX * errX + errY * errY);
				if (dist <= INFLUENCERADIUS) {
					f32 falloff = computeFalloff(dist, INFLUENCERADIUS);
					i32 delta = (int)(falloff * INFLUENCESTRENGTH);
					Color *neighbor = &pixels[ny * noise->width + nx];
					neighbor->r = (unsigned char)CLAMP(neighbor->r + delta, 0, 255);
				}
			}
		}
	}

}

void generateWorld(Tile *tileMap, i32 rows, i32 cols) {
	Tile mountain = { .spriteIdx = 9, .passable = false, .debugHighlight = false, };
	Tile grass = { .spriteIdx = 8, .passable = true, .debugHighlight = false, };
	Tile forest = { .spriteIdx = 10, .passable = true, .debugHighlight = false };
	Tile city = { .spriteIdx = 15, .passable = true, .debugHighlight = false, };

	// Get center of world
	f32 centerX = rows / 2.0f;
	f32 centerY = cols / 2.0f;
	f32 maxDist = sqrtf(centerX * centerX + centerY * centerY);
	
	// Generate Random Offset
	i32 offX = GetRandomValue(0, 10000);
	i32 offY = GetRandomValue(0, 10000);

	Image noise = GenImagePerlinNoise(rows, cols,  offX, offY, 5.0);
	Image forestNoise = GenImagePerlinNoise(rows, cols, offX + GetRandomValue(10, 10000), offY + GetRandomValue(10, 10000), 10.0);
	Color *pixels = LoadImageColors(noise);
	Color *forestPixels = LoadImageColors(forestNoise);

	for (i32 y = 0; y < noise.height; y++) {
		for (i32 x = 0; x < noise.width; x++) {
			
			// force map bounds
			if (x < MARGIN || x>= cols - MARGIN || y < MARGIN || y >= rows - MARGIN) {
				tileMap[y * cols + x] = mountain;
				
				propagateError(tileMap, &noise, pixels, rows, cols, x, y);
				continue;
			}

			Color pixel = pixels[y * noise.width + x];
			f32 noiseVal = pixel.r / 255.0f;

			// Distance from center
			f32 dx = x - centerX;
			f32 dy = y - centerY;
			f32 dist = sqrtf(dx * dx + dy * dy);
			f32 falloff = dist / maxDist;
			
			f32 adjusted = noiseVal * falloff;

			Color forestPixel = forestPixels[y * noise.width + x];
			f32 forestVal = forestPixel.r / 255.0f;

			if (adjusted > 0.3f) {
				tileMap[y * cols + x] = mountain;
				
			} else if (forestVal > 0.66) {
				tileMap[y * cols + x] = forest;
			}
			else {
				tileMap[y * cols + x] = grass;
				
			}
		}
	}
	
	// Place city in middle of the map
	tileMap[(i32)centerY * cols + (i32)centerX] = city;

	UnloadImageColors(pixels);
	UnloadImage(noise);
	UnloadImageColors(forestPixels);
	UnloadImage(forestNoise);
}

void placeDungeons(Tile *tileMap, i32 rows, i32 cols, GameState *game, i32 numDungeons, Arena *temp) {
	Tile dungeon = { .spriteIdx = 14, .passable = true, .debugHighlight = false };
	Vec2i center = { cols/2, rows / 2 };

	Vec2i candidates[cols * rows];
	i32 candidateCount = 0;

	// Collect valid tiles that border passable terrain
	for (i32 y = MARGIN; y < rows - MARGIN; y++) {
		for (i32 x = MARGIN; x < cols - MARGIN; x++) {
			i32 idx = y * cols + x;
			if (tileMap[idx].spriteIdx != 9) continue;

			bool hasPassableNeighbor = false;
			for (i32 dy = -1; dy <= 1 && !hasPassableNeighbor; dy++) {
				for (i32 dx = -1; dx <= 1 && !hasPassableNeighbor; dx++) {
					if (dx == 0 && dy == 0) continue;
					i32 nx = x + dx;
					i32 ny = y + dy;
					if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
						Tile neighbor = tileMap[ny * cols + nx];
						if (neighbor.passable) {
							hasPassableNeighbor = true;
						}
					}
				}
			}

			if(hasPassableNeighbor) {
				// tileMap[idx].debugHighlight = true;
				candidates[candidateCount++] = (Vec2i){ x, y };
			}
		}
	}

	// Shuffle Candidate list
	for (i32 i = candidateCount - 1; i > 0; i--) {
		i32 j = GetRandomValue(0, i);
		Vec2i tempSwap = candidates[i];
		candidates[i] = candidates[j];
		candidates[j] = tempSwap;
	}

	// Attempt Dungeon Placement
	i32 placedDungeons = 0;
	for (i32 i = 0; i < candidateCount && placedDungeons < numDungeons; i++) {
		Vec2i pos = candidates[i];

		bool tooClose = false;
		for (i32 j = 0; j < placedDungeons; j++) {
			if (abs(pos.x - game->dungeons[j].x) + abs(pos.y - game->dungeons[j].y) < VISIBLETILESX) {
				tooClose = true;
				break;
			}
		}
		if (tooClose) continue;
		
		// Check if path to center exists
		arenaReset(temp);
		if(canReachFromAdjacent(tileMap, rows, cols, pos, center, temp)) {
			tileMap[pos.y * cols + pos.x] = dungeon;
			game->dungeons[placedDungeons++] = pos;
		}
	}
}
