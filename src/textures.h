#ifndef TEXTURES_H
#define TEXTURES_H

#include "state.h"

#include <SDL.h>

#define TEXTURE_COUNT 10

#define ASSET_PATH_CONCRETE "assets/concrete.jpg"
#define ASSET_PATH_BRICKS "assets/bricks.jpg"
#define ASSET_PATH_PLASTER "assets/plaster.jpg"
#define ASSET_PATH_ROCK "assets/rock.jpg"
#define ASSET_PATH_TILES "assets/tiles.jpg"
#define ASSET_PATH_PROJECTILE "assets/projectile.png"

#define CONCRETE_INDEX 0
#define BRICKS_INDEX 1
#define PLASTER_INDEX 2
#define ROCK_INDEX 3
#define TILES_INDEX 4
#define PROJECTILE_INDEX 5

#define MAX_FRAMES 10

extern SDL_Texture *game_textures[];

struct animation {
        unsigned int index;
        unsigned int frame_count;
        unsigned int current_frame;
        Uint32 frame_duration;
        Uint32 last_frame;
        SDL_Rect frames[MAX_FRAMES];
};

#define ANIMATION_PROJECTILE 0

extern struct animation game_animations[];

_Bool load_textures(struct state *s);

void animation_render(unsigned int index, struct state *s, SDL_Rect dst);

#endif