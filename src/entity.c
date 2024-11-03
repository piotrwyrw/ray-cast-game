#include "entity.h"
#include "engine.h"
#include "textures.h"
#include "render.h"
#include "map.h"
#include "drugs.h"
#include "enemy.h"

struct entity game_entities[MAX_ENTITIES] = {ENTITY_NONE};

static double entity_distance(struct entity *e)
{
        return vector_distance(&e->location, &cam.location);
}

static int compare_entities(const void *a, const void *b)
{
        struct entity *e_a = (struct entity *) a;
        struct entity *e_b = (struct entity *) b;

        double dst_a = entity_distance(e_a);
        double dst_b = entity_distance(e_b);

        if (e_a->type != ENTITY_NONE && e_b->type != ENTITY_NONE)
                return (dst_a < dst_b) ? -1 : (dst_a == dst_b) ? 0 : 1;

        if (e_a->type != ENTITY_NONE)
                return -1;

        if (e_b->type != ENTITY_NONE)
                return 1;

        return 0;
}

static void sort_entities()
{
        qsort(game_entities, MAX_ENTITIES, sizeof(struct entity), compare_entities);
}

void entity_render_all(struct state *s)
{
        // This works in place of a "Z-Buffer" for the sprites
        sort_entities();

        struct entity *e;
        for (int i = MAX_ENTITIES - 1; i >= 0; i--) {
                e = &game_entities[i];
                if (e->type == ENTITY_NONE)
                        continue;
                entity_render_update(e, s);
        }
}

static void entity_update_projectile(struct entity *e)
{
        if (vector_distance(&cam.location, &e->location) > MAX_PROJECTILE_DISTANCE) {
                e->type = ENTITY_NONE;
                return;
        }

        struct entity *tmp_e;
        double distance;

        for (unsigned int i = 0; i < MAX_ENTITIES; i++) {
                tmp_e = &game_entities[i];

                if (!IS_ENEMY(tmp_e->type) || tmp_e->state == STATE_DYING)
                        continue;

                distance = vector_distance(&e->location, &tmp_e->location);

                if (distance > THRESHOLD_DISTANCE)
                        continue;

                e->type = ENTITY_NONE;
                tmp_e->state = STATE_DYING;
                tmp_e->anim = get_animation(ANIMATION_EXPLOSION);
                return;
        }

        struct vector direction = e->velocity;
        vector_normalize(&direction);

        struct ray_cast cast;
        if (!cast_ray(&cast, &e->location, &direction))
                return;

        if (cast.real_distance > THRESHOLD_DISTANCE)
                return;

        e->anim = get_animation(ANIMATION_EXPLOSION);
        e->velocity = vec(0.0, 0.0);
        e->accel = vec(0.0, 0.0);
        e->width = 300;
        e->height = e->width;
        e->state = STATE_DYING;
}

static void entity_update_enemy(struct state *s, struct entity *e)
{
        if (e->state == STATE_DYING || SDL_GetTicks() - e->spawn_time < ENEMY_ACTIVATION_TIME)
                return;

        double p_distance = vector_distance(&e->location, &cam.location);

        if (e->type == ENTITY_ENEMY_WISP)
                update_wisp(s, e, p_distance);

        if (e->type == ENTITY_ENEMY_SKELETON)
                update_skeleton(s, e, p_distance);

        if (e->type != ENTITY_ENEMY_WISP && e->state != STATE_WALKING) {
                e->accel = vec(0, 0);
                e->velocity = vec(0, 0);
                return;
        }

        struct vector direction = cam.location;
        vector_sub(&direction, &e->location);
        vector_normalize(&direction);
        e->accel = direction;
}

static void entity_update_drug(struct entity *e)
{
        double distance = vector_distance(&e->location, &cam.location);

        if (distance > THRESHOLD_DISTANCE)
                return;

        e->type = ENTITY_NONE;
        drugs_consume();
}

void entity_render_update(struct entity *e, struct state *s)
{
        if (e->state == STATE_DYING && !e->anim.armed) {
                e->type = ENTITY_NONE;
                return;
        }

        if (e->type == ENTITY_PROJECTILE)
                entity_update_projectile(e);

        if (IS_ENEMY(e->type))
                entity_update_enemy(s, e);

        if (e->type == ENTITY_SHROOM)
                entity_update_drug(e);

        vector_add(&e->velocity, &e->accel);

        if (IS_ENEMY(e->type))
                vector_limit(&e->velocity, WISP_VELOCITY);

        vector_add(&e->location, &e->velocity);

        entity_render(e, s);
}

// Figure out the X position of entity on the screen and render it.
// The Y coordinate is always HEIGHT / 2
void entity_render(struct entity *e, struct state *s)
{
        // Angle wrt. to the viewport
        double angle = atan2(e->location.y - cam.location.y, e->location.x - cam.location.x) - cam.angle;

        while (angle > M_PI) angle -= 2 * M_PI;
        while (angle < -M_PI) angle += 2 * M_PI;

        if (angle < -FOV / 2 || angle > FOV / 2)
                return;

        double distance = vector_distance(&e->location, &cam.location) * cos(angle);

        // Check if the entity is covered by a wall
        struct ray_cast cast;
        struct vector ray_dir = vector_dir_angle(angle + cam.angle);
        if (cast_ray(&cast, &cam.location, &ray_dir) && cast.real_distance < distance)
                return;

        // [-1 .. 1]
        double offset_factor = angle / (FOV / 2.0);
        int sprite_x = (int) (WIDTH / 2.0 + (offset_factor * (WIDTH / 2.0)));

        int sprite_w = (int) (e->width / distance);
        int sprite_h = (int) (e->height / distance);

        if (sprite_w < MIN_ENTITY_SIZE) sprite_w = MIN_ENTITY_SIZE;
        if (sprite_h < MIN_ENTITY_SIZE) sprite_h = MIN_ENTITY_SIZE;

        int sprite_y = HEIGHT / 2 - (sprite_h / 2) + (int) (e->y_off / distance);

        double light = light_intensity(distance);
        SDL_SetTextureColorMod(game_textures[e->anim.index], (Uint8) light, (Uint8) light, (Uint8) light);
        animation_render(&e->anim, s, (SDL_Rect) {
                .x = sprite_x - (sprite_w / 2),
                .y = sprite_y,
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