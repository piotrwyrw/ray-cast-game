#ifndef ENTITY_H
#define ENTITY_H

#include "textures.h"
#include "math.h"

#define MAX_ENTITIES 100
#define MAX_PROJECTILE_DISTANCE 10
#define WISP_VELOCITY 0.03

#define BULLET_SIZE 50
#define WISP_SIZE 200

#define SKELETON_WIDTH (24 * 35)
#define SKELETON_HEIGHT (32 * 35)

#define ENEMY_ACTIVATION_TIME 500

#define THRESHOLD_DISTANCE 0.2
#define SKELETON_REACH 2.0

#define IS_ENEMY(type) (type == ENTITY_ENEMY_WISP || type == ENTITY_ENEMY_SKELETON)

enum entity_type {
        ENTITY_NONE,
        ENTITY_PROJECTILE,
        ENTITY_ENEMY_WISP,
        ENTITY_SHROOM,
        ENTITY_SPRITE,
        ENTITY_ENEMY_SKELETON
};

enum entity_state {
        STATE_DEFAULT,
        STATE_DYING,
        STATE_CHARGING,
        STATE_WALKING
};

struct entity {
        enum entity_type type;

        enum entity_state state;

        unsigned int width;
        unsigned int height;

        unsigned int y_off;

        struct vector accel;
        struct vector velocity;

        Uint32 spawn_time;

        struct animation anim;
        struct vector location;
};

extern struct entity game_entities[];

void entity_render_all(struct state *s);

void entity_render_update(struct entity *e, struct state *s);

void entity_render(struct entity *e, struct state *s);

struct entity *entity_add(struct entity e);

#endif
