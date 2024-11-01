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

SDL_Texture *game_textures[TEXTURE_COUNT] = {NULL};

_Bool load_textures(struct state *s)
{
        LOAD_TEXTURE(BRICKS_INDEX, ASSET_PATH_BRICKS)
        LOAD_TEXTURE(CONCRETE_INDEX, ASSET_PATH_CONCRETE)
        LOAD_TEXTURE(PLASTER_INDEX, ASSET_PATH_PLASTER)
        LOAD_TEXTURE(ROCK_INDEX, ASSET_PATH_ROCK)
        LOAD_TEXTURE(TILES_INDEX, ASSET_PATH_TILES)

        LOAD_TEXTURE(PROJECTILE_INDEX, ASSET_PATH_PROJECTILE)

        return true;
}

struct animation game_animation[] = {
        {
                .index = PROJECTILE_INDEX,
                .current_frame = 0,
                .frame_count = 4,
                .frame_duration = 70,
                .frames = {
                        (struct SDL_Rect) {
                                .x = 0, .y = 0,
                                .w = 32, .h = 30
                        },
                        (struct SDL_Rect) {
                                .x = 32, .y = 0,
                                .w = 32, .h = 30
                        },
                        (struct SDL_Rect) {
                                .x = 32 * 2, .y = 0,
                                .w = 32, .h = 30
                        },
                        (struct SDL_Rect) {
                                .x = 32 * 3, .y = 0,
                                .w = 32, .h = 30
                        }
                }
        }
};

void animation_render(unsigned int index, struct state *s, SDL_Rect dst)
{
        struct animation *anim = &game_animation[index];

        if (anim->frame_count == 0)
                return;

        Uint32 current_time = SDL_GetTicks();
        if (current_time > anim->last_frame + anim->frame_duration) {
                anim->current_frame++;
                anim->last_frame = current_time;
                if (anim->current_frame >= anim->frame_count)
                        anim->current_frame = 0;
        }

        SDL_Rect *src = &anim->frames[anim->current_frame];
        SDL_RenderCopy(s->renderer, game_textures[anim->index], src, &dst);
}

#undef LOAD_TEXTURE