#ifndef ENTITY_H
#define ENTITY_H

#include "textures.h"
#include "math.h"

#define MAX_ENTITIES 100
#define MAX_PROJECTILE_DISTANCE 10
#define WISP_VELOCITY 0.03

#define BULLET_SIZE 50
#define WISP_SIZE 200

#define WISP_ACTIVATION_TIME 500

#define ENEMY_THRESHOLD_DISTANCE 0.1
#define BULLET_THRESHOLD_DISTANCE 0.1

enum entity_type {
        ENTITY_NONE,
        ENTITY_PROJECTILE,
        ENTITY_ENEMY_WISP
};

#define ENTITY_DYING (1 << 0)

struct entity {
        enum entity_type type;

        Uint8 flags;

        unsigned int width;
        unsigned int height;

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
