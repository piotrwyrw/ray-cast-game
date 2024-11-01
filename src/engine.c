#include "engine.h"
#include "math.h"
#include "state.h"
#include "defines.h"
#include "render.h"
#include "entity.h"

#include <stdbool.h>

const Uint8 *keyState;

struct camera cam = {
        .location = vec(0.0, 0.0),
        .angle = 0.0
};

int WIDTH = 1500;
int HEIGHT = 900;

Uint32 last_projectile = 0;
_Bool machine_gun = false;

enum walk_direction {
        WALK_FORWARD,
        WALK_BACKWARD,

        WALK_LEFT,
        WALK_RIGHT
};

struct vector cam_direction;

void update_direction()
{
        cam_direction = vector_dir_angle(cam.angle);
}

#define SIDEWAYS(dir) (dir == WALK_LEFT || dir == WALK_RIGHT)

void move_camera(enum walk_direction direction)
{
        update_direction();

        struct vector extension = cam_direction;

        if (SIDEWAYS(direction)) {
                struct vector rotated = vec(-extension.y, extension.x);
                vector_mul(&rotated, (direction == WALK_RIGHT) ? WALK_SPEED : -WALK_SPEED);
                vector_add(&cam.location, &rotated);
                return;
        }

        vector_mul(&extension, (direction == WALK_FORWARD) ? WALK_SPEED : -WALK_SPEED);
        vector_add(&cam.location, &extension);
}

struct vector bullet_velocity()
{
        struct vector vel = vector_dir_angle(cam.angle);
        vector_mul(&vel, BULLET_SPEED);
        return vel;
}

void add_bullet()
{
        entity_add((struct entity) {
                .location = cam.location,
                .anim = get_animation(ANIMATION_PROJECTILE),
                .velocity = bullet_velocity(),
                .width = BULLET_SIZE,
                .height = BULLET_SIZE,
                .type = ENTITY_PROJECTILE,
                .spawn_time = SDL_GetTicks()
        });
}

void summon_wisp(struct vector location)
{
        entity_add((struct entity) {
                .location = location,
                .anim = get_animation(ANIMATION_WISP),
                .velocity = vec(0.0, 0.0),
                .width = WISP_SIZE,
                .height = WISP_SIZE,
                .type = ENTITY_ENEMY_WISP,
                .spawn_time = SDL_GetTicks()
        });
}

void render(struct state *s)
{
        render_view(s);

        SDL_RenderPresent(s->renderer);
}

void start()
{
        struct state s;

        if (!initialize(&s)) {
                close(&s);
                return;
        }

        if (s.close)
                goto _close;

        SDL_Event evt;

        Uint32 fps = 60;
        Uint32 frameDuration = 1000 / fps;
        Uint32 frameTime;

        while (!s.close) {
                frameTime = SDL_GetTicks();

                while (SDL_PollEvent(&evt) > 0) {
                        if (evt.type == SDL_QUIT)
                                s.close = true;

                        if (evt.type == SDL_MOUSEMOTION) {
                                int dx = evt.motion.xrel;
                                double dAngle = (dx + 0.0) * MOUSE_SENSITIVITY;
                                dAngle = fabs(dAngle) > MAX_ANGLE_DELTA ? MAX_ANGLE_DELTA * signum(dAngle) : dAngle;
                                cam.angle += dAngle;
                                if (cam.angle < 0.0)
                                        cam.angle = M_PI * 2.0;
                                if (cam.angle > M_PI * 2.0)
                                        cam.angle = 0.0;
                        }

                        if (evt.type == SDL_MOUSEBUTTONDOWN) {
                                if (evt.button.button == SDL_BUTTON_LEFT)
                                        add_bullet();

                                if (evt.button.button == SDL_BUTTON_RIGHT)
                                        machine_gun = true;

                                if (evt.button.button == SDL_BUTTON_MIDDLE)
                                        summon_wisp(cam.location);
                        }

                        if (evt.type == SDL_MOUSEBUTTONUP)
                                if (evt.button.button == SDL_BUTTON_RIGHT)
                                        machine_gun = false;

                        if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                                WIDTH = evt.window.data1;
                                HEIGHT = evt.window.data2;
                        }
                }

                keyState = SDL_GetKeyboardState(NULL);

                if (machine_gun) {
                        Uint32 current_time = SDL_GetTicks();
                        if (current_time - last_projectile > MACHINE_GUN_DELAY) {
                                last_projectile = current_time;
                                add_bullet();
                        }
                }

                if (keyState[SDL_SCANCODE_ESCAPE])
                        s.close = true;

                if (keyState[SDL_SCANCODE_W])
                        move_camera(WALK_FORWARD);

                if (keyState[SDL_SCANCODE_S])
                        move_camera(WALK_BACKWARD);

                if (keyState[SDL_SCANCODE_A])
                        move_camera(WALK_LEFT);

                if (keyState[SDL_SCANCODE_D])
                        move_camera(WALK_RIGHT);

                render(&s);

                frameTime = SDL_GetTicks() - frameTime;
                if (frameTime < frameDuration)
                        SDL_Delay(frameDuration - frameTime);
        }

        _close:
        close(&s);
}

