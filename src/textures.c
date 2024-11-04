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
        LOAD_TEXTURE(HEART_INDEX, ASSET_PATH_HEART)
        LOAD_TEXTURE(EMPTY_HEART_INDEX, ASSET_PATH_EMPTY_HEART)
        LOAD_TEXTURE(WISP_INDEX, ASSET_PATH_WISP)
        LOAD_TEXTURE(FLASH_INDEX, ASSET_PATH_FLASH)
        LOAD_TEXTURE(EXPLOSION_INDEX, ASSET_PATH_EXPLOSION)
        LOAD_TEXTURE(SHROOM_INDEX, ASSET_PATH_SHROOM)
        LOAD_TEXTURE(PROJECTILE_INDEX, ASSET_PATH_PROJECTILE)
        LOAD_TEXTURE(IDLE_INDEX, ASSET_PATH_SKEL_IDLE)
        LOAD_TEXTURE(CHARGING_INDEX, ASSET_PATH_SKEL_CHARGING)
        LOAD_TEXTURE(WALKING_INDEX, ASSET_PATH_SKEL_WALKING)

        return true;
}

#define ON(str, index) if (strcmp(id, str) == 0) { return index; }

int find_texture(const char *id)
{
        ON("BRICKS", BRICKS_INDEX)
        ON("PLASTER", PLASTER_INDEX)
        ON("ROCK", ROCK_INDEX)
        ON("TILES", TILES_INDEX)

        return -1;
}

#undef ON

#define rect(_x, _y, _w, _h) (struct SDL_Rect) { .x = _x, .y = _y, .w = _w, .h = _h }

struct animation game_animations[] = {
        {
                .index = PROJECTILE_INDEX,
                .current_frame = 0,
                .frame_count = 4,
                .frame_duration = 30,
                .type = ANIMATION_MODE_CONTINUOUS,
                .frames = {
                        rect(0, 0, 32, 30),
                        rect(32, 0, 32, 30),
                        rect(32 * 2, 0, 32, 30),
                        rect(32 * 3, 0, 32, 30)
                }
        },
        {
                .index = WISP_INDEX,
                .current_frame = 0,
                .frame_count = 10,
                .frame_duration = 70,
                .type = ANIMATION_MODE_CONTINUOUS,
                .frames = {
                        rect(0, 0, 32, 32),
                        rect(32, 0, 32, 32),
                        rect(32 * 2, 0, 32, 32),
                        rect(32 * 3, 0, 32, 32),
                        rect(32 * 4, 0, 32, 32),
                        rect(32 * 5, 0, 32, 32),
                        rect(32 * 6, 0, 32, 32),
                        rect(32 * 7, 0, 32, 32),
                        rect(32 * 8, 0, 32, 32),
                        rect(32 * 9, 0, 32, 32),
                }
        },
        {
                .index = FLASH_INDEX,
                .current_frame = 0,
                .frame_count = 9,
                .frame_duration = 20,
                .type = ANIMATION_MODE_ONE_SHOT,
                .armed = false,
                .frames = {
                        rect(0, 0, 1, 1),
                        rect(1, 0, 1, 1),
                        rect(2, 0, 1, 1),
                        rect(3, 0, 1, 1),
                        rect(4, 0, 1, 1),
                        rect(5, 0, 1, 1),
                        rect(6, 0, 1, 1),
                        rect(7, 0, 1, 1),
                        rect(8, 0, 1, 1),
                        rect(9, 0, 1, 1),
                }
        },
        {
                .index = EXPLOSION_INDEX,
                .current_frame = 0,
                .frame_count = 11,
                .frame_duration = 15,
                .type = ANIMATION_MODE_ONE_SHOT,
                .armed = true,
                .frames = {
                        rect(0, 0, 512, 512),
                        rect(512, 0, 512, 512),
                        rect(512 * 2, 0, 512, 512),
                        rect(512 * 3, 0, 512, 512),
                        rect(512 * 4, 0, 512, 512),
                        rect(512 * 5, 0, 512, 512),
                        rect(512 * 6, 0, 512, 512),
                        rect(512 * 7, 0, 512, 512),
                        rect(512 * 8, 0, 512, 512),
                        rect(512 * 9, 0, 512, 512)
                }
        },
        {
                .index = SHROOM_INDEX,
                .current_frame = 0,
                .frame_count = 1,
                .frame_duration = 1000,
                .type = ANIMATION_MODE_CONTINUOUS,
                .frames = {
                        rect(0, 0, 70, 70)
                }
        },
        {
                .index = IDLE_INDEX,
                .current_frame = 0,
                .frame_count = 11,
                .frame_duration = 50,
                .type = ANIMATION_MODE_CONTINUOUS,
                .frames = {
                        rect(0, 0, 24, 32),
                        rect(24, 0, 24, 32),
                        rect(24 * 2, 0, 24, 32),
                        rect(24 * 3, 0, 24, 32),
                        rect(24 * 4, 0, 24, 32),
                        rect(24 * 5, 0, 24, 32),
                        rect(24 * 6, 0, 24, 32),
                        rect(24 * 7, 0, 24, 32),
                        rect(24 * 8, 0, 24, 32),
                        rect(24 * 9, 0, 24, 32),
                        rect(24 * 10, 0, 24, 32),
                }
        },
        {
                .index = CHARGING_INDEX,
                .current_frame = 0,
                .frame_count = 9,
                .frame_duration = 50,
                .type = ANIMATION_MODE_ONE_SHOT,
                .armed = true,
                .frames = {
                        rect(0, 0, 24, 37),
                        rect(43, 0, 24, 37),
                        rect(43 * 2, 0, 24, 37),
                        rect(43 * 3, 0, 24, 37),
                        rect(43 * 4, 0, 24, 37),
                        rect(43 * 5, 0, 24, 37),
                        rect(43 * 6, 0, 24, 37),
                        rect(43 * 7, 0, 24, 37),
                        rect(43 * 8, 0, 24, 37),
                        rect(43 * 9, 0, 24, 37),
                        rect(43 * 10, 0, 24, 37),
                        rect(43 * 11, 0, 24, 37),
                        rect(43 * 12, 0, 24, 37),
                        rect(43 * 13, 0, 24, 37),
                        rect(43 * 14, 0, 24, 37),
                        rect(43 * 15, 0, 24, 37),
                        rect(43 * 16, 0, 24, 37),
                        rect(43 * 17, 0, 24, 37)
                }
        },
        {
                .index = WALKING_INDEX,
                .current_frame = 0,
                .frame_count = 13,
                .frame_duration = 25,
                .type = ANIMATION_MODE_CONTINUOUS,
                .armed = true,
                .frames = {
                        rect(0, 0, 22, 33),
                        rect(22, 0, 22, 33),
                        rect(22 * 2, 0, 22, 33),
                        rect(22 * 3, 0, 22, 33),
                        rect(22 * 4, 0, 22, 33),
                        rect(22 * 5, 0, 22, 33),
                        rect(22 * 6, 0, 22, 33),
                        rect(22 * 7, 0, 22, 33),
                        rect(22 * 8, 0, 22, 33),
                        rect(22 * 9, 0, 22, 33),
                        rect(22 * 10, 0, 22, 33),
                        rect(22 * 11, 0, 22, 33),
                        rect(22 * 12, 0, 22, 33),
                }
        }
};

#undef rect

void animation_render(struct animation *anim, struct state *s, SDL_Rect dst)
{
        if (anim->frame_count == 0)
                return;

        if (anim->frame_count == 1)
                goto _render;

        if (anim->type == ANIMATION_MODE_ONE_SHOT && !anim->armed)
                return;

        Uint32 current_time = SDL_GetTicks();
        if (current_time > anim->last_frame + anim->frame_duration) {
                anim->current_frame++;
                anim->last_frame = current_time;
                if (anim->current_frame >= anim->frame_count) {
                        anim->current_frame = 0;
                        anim->armed = false;
                        if (anim->type == ANIMATION_MODE_ONE_SHOT)
                                return;
                }
        }

        _render:;
        SDL_Rect *src = &anim->frames[anim->current_frame];
        SDL_RenderCopy(s->renderer, game_textures[anim->index], src, &dst);
}

struct animation get_animation(unsigned int index)
{
        return game_animations[index];
}

#undef LOAD_TEXTURE