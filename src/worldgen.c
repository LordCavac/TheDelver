#include "worldgen.h"
#include "core.h"
#include "raylib.h"

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
	Tile mountain = { .spriteIdx = 9, .passable = false };
	Tile grass = { .spriteIdx = 8, .passable = true };
	Tile forest = { .spriteIdx = 10, .passable = true };
	Tile city = { .spriteIdx = 15, .passable = true };

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

