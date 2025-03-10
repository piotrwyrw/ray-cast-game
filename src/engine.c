#include "engine.h"
#include "math.h"
#include "state.h"
#include "defines.h"
#include "render.h"
#include "entity.h"
#include "spawn.h"
#include "map.h"

#include <stdbool.h>

const Uint8 *keyState;

struct camera cam = {
        .location = vec(0.0, 0.0),
        .angle = 0.0
};

double WALK_SPEED = 0.07;

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

void collisions_detect(struct vector *ext)
{
        struct vector dir = *ext;
        vector_normalize(&dir);

        struct ray_cast cast;
        if (!cast_ray(&cast, &cam.location, &dir))
                return;

        if (cast.real_distance > WALK_SPEED)
                return;

        // TODO
}

void move_camera(enum walk_direction direction)
{
        update_direction();

        struct vector extension = cam_direction;

        if (SIDEWAYS(direction)) {
                struct vector rotated = vec(-extension.y, extension.x);
                vector_mul(&rotated, (direction == WALK_RIGHT) ? WALK_SPEED : -WALK_SPEED);
                extension.x = rotated.x;
                extension.y = rotated.y;
        } else vector_mul(&extension, (direction == WALK_FORWARD) ? WALK_SPEED : -WALK_SPEED);

        collisions_detect(&extension);

        vector_add(&cam.location, &extension);
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

        spawn_generic(ENTITY_SHROOM, vec(4, 4));

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
                                        spawn_generic(ENTITY_PROJECTILE, cam.location);

                                if (evt.button.button == SDL_BUTTON_RIGHT)
                                        machine_gun = true;

                                if (evt.button.button == SDL_BUTTON_MIDDLE)
                                        spawn_generic(ENTITY_ENEMY_WISP, cam.location);
                        }

                        if (evt.type == SDL_MOUSEBUTTONUP)
                                if (evt.button.button == SDL_BUTTON_RIGHT)
                                        machine_gun = false;

                        if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                                WIDTH = evt.window.data1;
                                HEIGHT = evt.window.data2;
                        }

                        if (evt.type == SDL_KEYDOWN && evt.key.keysym.scancode == SDL_SCANCODE_G) {
                                spawn_generic(ENTITY_ENEMY_SKELETON, cam.location);
                        }
                }

                keyState = SDL_GetKeyboardState(NULL);

                if (machine_gun) {
                        Uint32 current_time = SDL_GetTicks();
                        if (current_time - last_projectile > MACHINE_GUN_DELAY) {
                                last_projectile = current_time;
                                spawn_generic(ENTITY_PROJECTILE, cam.location);
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

