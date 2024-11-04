#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL.h>

#define TEXTURE_COUNT 20

#define ASSET_PATH_CONCRETE "assets/concrete.jpg"
#define ASSET_PATH_BRICKS "assets/bricks.jpg"
#define ASSET_PATH_PLASTER "assets/plaster.jpg"
#define ASSET_PATH_ROCK "assets/rock.jpg"
#define ASSET_PATH_TILES "assets/tiles.jpg"
#define ASSET_PATH_PROJECTILE "assets/projectile.png"
#define ASSET_PATH_HEART "assets/heart.png"
#define ASSET_PATH_EMPTY_HEART "assets/empty_heart.png"
#define ASSET_PATH_WISP "assets/wisp.png"
#define ASSET_PATH_FLASH "assets/flash.png"
#define ASSET_PATH_EXPLOSION "assets/explosion.png"
#define ASSET_PATH_SHROOM "assets/shroom.png"
#define ASSET_PATH_SKEL_IDLE "assets/idle.png"
#define ASSET_PATH_SKEL_CHARGING "assets/charging.png"
#define ASSET_PATH_SKEL_WALKING "assets/walking.png"

#define CONCRETE_INDEX 0
#define BRICKS_INDEX 1
#define PLASTER_INDEX 2
#define ROCK_INDEX 3
#define TILES_INDEX 4
#define PROJECTILE_INDEX 5
#define HEART_INDEX 6
#define EMPTY_HEART_INDEX 7
#define WISP_INDEX 8
#define FLASH_INDEX 9
#define EXPLOSION_INDEX 10
#define SHROOM_INDEX 11
#define IDLE_INDEX 12
#define CHARGING_INDEX 13
#define WALKING_INDEX 14

#define MAX_FRAMES 20

struct state;

extern SDL_Texture *game_textures[];

_Bool load_textures(struct state *s);

int find_texture(const char *id);

enum animation_type {
        ANIMATION_MODE_CONTINUOUS,
        ANIMATION_MODE_ONE_SHOT
};

struct animation {
        enum animation_type type;
        _Bool armed;
        unsigned int index;
        unsigned int frame_count;
        unsigned int current_frame;
        Uint32 frame_duration;
        Uint32 last_frame;
        SDL_Rect frames[MAX_FRAMES];
};

#define ANIMATION_PROJECTILE 0
#define ANIMATION_WISP 1
#define ANIMATION_FLASH 2
#define ANIMATION_EXPLOSION 3
#define ANIMATION_SHROOM 4
#define ANIMATION_IDLE 5
#define ANIMATION_CHARGING 6
#define ANIMATION_WALKING 7

extern struct animation game_animations[];

void animation_render(struct animation *anim, struct state *s, SDL_Rect dst);

struct animation get_animation(unsigned int index);

#endif