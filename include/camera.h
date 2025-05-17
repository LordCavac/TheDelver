#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "player.h"
#include "types.h"

void initCamera(Camera2D *camera);
void updateCamera(Camera2D *camera, const Player *player, i32 mapWidth, i32 mapHeight);

#endif
