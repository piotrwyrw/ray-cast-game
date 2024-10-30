#include "textures.h"
#include "state.h"

#include <SDL_image.h>
#include <SDL.h>
#include <stdbool.h>

#define LOAD_TEXTURE(index, path) \
        game_textures[index] = IMG_LoadTexture(s->renderer, path); \
        if (!game_textures[index]) {                               \
                printf("Could not load texture '" path "'");\
                return false;               \
        }

SDL_Texture* game_textures[TEXTURE_COUNT] = { NULL };

_Bool load_textures(struct state *s)
{
        LOAD_TEXTURE(BRICKS_INDEX, ASSET_PATH_BRICKS)
        LOAD_TEXTURE(CONCRETE_INDEX, ASSET_PATH_CONCRETE)
        LOAD_TEXTURE(PLASTER_INDEX, ASSET_PATH_PLASTER)

        return true;
}

#undef LOAD_TEXTURE