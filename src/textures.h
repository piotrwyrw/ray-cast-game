#ifndef TEXTURES_H
#define TEXTURES_H

#include "state.h"

#include <SDL.h>

#define TEXTURE_COUNT 10

#define ASSET_PATH_CONCRETE "assets/concrete.jpg"
#define ASSET_PATH_BRICKS "assets/bricks.jpg"
#define ASSET_PATH_PLASTER "assets/plaster.jpg"

#define CONCRETE_INDEX 0
#define BRICKS_INDEX 1
#define PLASTER_INDEX 2

extern SDL_Texture *game_textures[];

_Bool load_textures(struct state *s);

#endif