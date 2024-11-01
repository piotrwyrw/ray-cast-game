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
                .txt_index = ANIMATION_PROJECTILE,
                .txt_type = TEXTURE_ANIMATED,
                .data.velocity = bullet_velocity(),
                .type = ENTITY_PROJECTILE
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
                        }

                        if (evt.type == SDL_WINDOWEVENT && evt.window.event == SDL_WINDOWEVENT_RESIZED) {
                                WIDTH = evt.window.data1;
                                HEIGHT = evt.window.data2;
                        }
                }

                keyState = SDL_GetKeyboardState(NULL);

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

