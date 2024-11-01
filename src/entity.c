#include "entity.h"
#include "engine.h"
#include "render.h"
#include "map.h"

struct entity game_entities[MAX_ENTITIES] = {ENTITY_NONE};

void entity_render_all(struct state *s)
{
        struct entity *e;
        for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
                e = &game_entities[i];
                if (e->type == ENTITY_NONE)
                        continue;
                entity_render_update(e, s);
        }
}

void entity_render_update(struct entity *e, struct state *s)
{
        if (e->type != ENTITY_PROJECTILE)
                return;

        vector_add(&e->location, &e->data.velocity);

        if (vector_distance(&cam.location, &e->location) > MAX_PROJECTILE_DISTANCE)
                e->type = ENTITY_NONE;

        entity_render(e, s);
}

// Figure out the X position of entity on the screen and render it.
// The Y coordinate is always HEIGHT / 2
void entity_render(struct entity *e, struct state *s)
{
        const int y = HEIGHT / 2;

        // Angle wrt. to the viewport
        double angle = atan2(e->location.y - cam.location.y, e->location.x - cam.location.x) - cam.angle;

        while (angle > M_PI) angle -= 2 * M_PI;
        while (angle < -M_PI) angle += 2 * M_PI;

        if (angle < -FOV / 2 || angle > FOV / 2)
                return;

        double distance = vector_distance(&e->location, &cam.location);

        // Check if the entity is covered by a wall
        struct ray_cast cast;
        struct vector ray_dir = vector_dir_angle(angle + cam.angle);
        if (cast_ray(&cast, &cam.location, &ray_dir))
                if (cast.real_distance < distance)
                        return;

        // [-1 .. 1]
        double offset_factor = angle / (FOV / 2.0);
        int sprite_x = (int) (WIDTH / 2.0 + (offset_factor * (WIDTH / 2.0)));

        int sprite_w = (int) (50 / distance);
        int sprite_h = (int) (50 / distance);

        if (sprite_w < MIN_ENTITY_SIZE) sprite_w = MIN_ENTITY_SIZE;
        if (sprite_h < MIN_ENTITY_SIZE) sprite_h = MIN_ENTITY_SIZE;

        animation_render(e->txt_index, s, (SDL_Rect) {
                .x = sprite_x - (sprite_w / 2),
                .y = y - (sprite_h / 2),
                .w = sprite_w,
                .h = sprite_h
        });
}

struct entity *entity_add(struct entity e)
{
        struct entity *tmp_e;
        for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
                tmp_e = &game_entities[i];
                if (tmp_e->type != ENTITY_NONE)
                        continue;
                *tmp_e = e;
                return tmp_e;
        }
        return NULL;
}