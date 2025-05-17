#include "scenemanager.h"
#include "core.h"
#include "scenes/title.h"
#include "scenes/overworld.h"
#include "scenes/dungeon.h"
#include "scenes/town.h"
#include "scenes/ending.h"

typedef struct SceneHandlers {
	void (*update)(GameState *game);
	void (*draw)(GameState *game);
} SceneHandlers;

static SceneHandlers handlers[STATECOUNT] = {
	[TITLE] = { updateTitle, drawTitle },
	[OVERWORLD] = { updateOverworld, drawOverworld },
	[DUNGEON] = { updateDungeon, drawDungeon },
	[TOWN] = { updateTown, drawTown },
	[ENDING] = { updateEnding, drawEnding },
};

void updateScene(GameState *game) {
	if (handlers[game->scene].update) handlers[game->scene].update(game);
}

void drawScene(GameState *game) {
	if (handlers[game->scene].draw) handlers[game->scene].draw(game);
}
