#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

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

void initPlayer(Player *player);
Player createPlayer();

#endif
