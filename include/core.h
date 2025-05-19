#ifndef CORE_H
#define CORE_H

#include "raylib.h"
#include "gamescene.h"
#include "tile.h"
#include "types.h"
#include "player.h"

// Initial Window Size
#define INITSCREENX 640
#define INITSCREENY 360

// Basic world features definitions
#define TILESIZE 16
#define WORLDX 128
#define WORLDY 128
#define MARGIN 1 // how thick should the border mountains should be
#define DUNGEONS 15 // How many dungeons should be spawned

// How many tiles should be visible on screen
#define VISIBLETILESX 16
#define VISIBLETILESY 9

// Clamping function for i32 values
#define CLAMP(val, min, max) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

typedef struct GameState {
	i32 seed; // used to store the random seed chosen by user
	GameScene scene; // current screen the game is in
	Player player; // Holds information for the player
	Camera2D camera; // 2d game camera
	Tile* worldMap; // Overworld map stored as array
	Vec2i dungeons[DUNGEONS];
} GameState;

#endif
