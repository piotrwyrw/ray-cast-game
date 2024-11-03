#include "spawn.h"
#include "entity.h"
#include "math.h"
#include "engine.h"
#include <stdbool.h>

#define on(type, id) \
        case type: \
                return spawn_##id(location); \


_Bool spawn_generic(enum entity_type type, struct vector location)
{
        switch (type) {
                on(ENTITY_ENEMY_WISP, wisp)
                on(ENTITY_SHROOM, shroom)
                on(ENTITY_ENEMY_SKELETON, skeleton)
                on(ENTITY_PROJECTILE, projectile)
                default:
                        printf("Could not summon entity %d\n", type);
                        return false;
        }
}

#undef on

#define spawnf(id, ...) _Bool spawn_##id(struct vector location) { return (entity_add((struct entity) __VA_ARGS__ ) != NULL); }

spawnf(wisp, {
        .location = location,
        .anim = get_animation(ANIMATION_WISP),
        .velocity = vec(0.0, 0.0),
        .width = WISP_SIZE,
        .height = WISP_SIZE,
        .type = ENTITY_ENEMY_WISP,
        .spawn_time = SDL_GetTicks(),
        .distance_dim = false
})

spawnf(skeleton, {
        .location = location,
        .anim = get_animation(ANIMATION_IDLE),
        .y_off = 100,
        .velocity = vec(0.0, 0.0),
        .width = SKELETON_WIDTH,
        .height = SKELETON_HEIGHT,
        .type = ENTITY_ENEMY_SKELETON,
        .spawn_time = SDL_GetTicks(),
        .distance_dim = true
})

spawnf(shroom, {
        .location = location,
        .anim = get_animation(ANIMATION_SHROOM),
        .y_off = 200,
        .velocity = vec(0.0, 0.0),
        .width = WISP_SIZE * 3,
        .height = WISP_SIZE * 3,
        .type = ENTITY_SHROOM,
        .spawn_time = SDL_GetTicks(),
        .distance_dim = true
})

static struct vector bullet_velocity()
{
        struct vector vel = vector_dir_angle(cam.angle);
        vector_mul(&vel, BULLET_SPEED);
        return vel;
}

spawnf(projectile, {
        .location = location,
        .anim = get_animation(ANIMATION_PROJECTILE),
        .velocity = bullet_velocity(),
        .width = BULLET_SIZE,
        .height = BULLET_SIZE,
        .type = ENTITY_PROJECTILE,
        .spawn_time = SDL_GetTicks(),
        .distance_dim = false
})

#undef spawnf