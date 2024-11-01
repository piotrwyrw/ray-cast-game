#ifndef ENTITY_H
#define ENTITY_H

#include "textures.h"
#include "math.h"

#define MAX_ENTITIES 100
#define MAX_PROJECTILE_DISTANCE 10

enum texture_type {
        TEXTURE_STATIC,
        TEXTURE_ANIMATED
};

enum entity_type {
        ENTITY_NONE,
        ENTITY_PROJECTILE
};

struct entity {
        enum entity_type type;

        union {
                struct vector velocity;
        } data;

        enum texture_type txt_type;
        unsigned int txt_index;         // Points to a texture or an animation
        struct vector location;
};

extern struct entity game_entities[];

void entity_render_all(struct state *s);

void entity_render_update(struct entity *e, struct state *s);

void entity_render(struct entity *e, struct state *s);

struct entity *entity_add(struct entity e);

#endif
