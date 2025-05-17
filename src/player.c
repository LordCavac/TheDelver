#include "player.h"
#include "core.h"

void initPlayer(Player *player) {
	player->spriteIdx = 0;
	player->position = (Vec2i){ WORLDX/2, WORLDY/2 };
	player->hitpoints = 10;
	player->strength = GetRandomValue(1, 10);
	player->dexterity = GetRandomValue(1, 10);
	player->stamina = GetRandomValue(1, 10);
	player->wisdom = GetRandomValue(1, 10);
	player->gold = 200;
	player->food = 200;

}

Player createPlayer() {
	return (Player){ 
		.spriteIdx = 0,
		.position = (Vec2i) { WORLDX/2, WORLDY/2 },
		.hitpoints = 10,
		.strength = GetRandomValue(1, 10),
		.dexterity = GetRandomValue(1, 10), 
		.stamina = GetRandomValue(1, 10),
		.wisdom = GetRandomValue(1, 10),
		.gold = 200,
		.food = 200 
	};
}	

