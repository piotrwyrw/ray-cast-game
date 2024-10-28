#ifndef RENDER_H
#define RENDER_H

#include "engine.h"
#include "state.h"

#include <SDL_render.h>

#define Z_FAR 100.0
#define LIGHT_DISTANCE 5.0
#define FOV (60 * (M_PI / 180))
#define WALL_HEIGHT (HEIGHT / 2 - 200)

#define SKY_COLOR 52, 31, 151
#define FLOOR_COLOR 231, 76, 60
#define WALL_COLOR 255, 238, 201

void render_view(struct state *s, struct camera *cam);

#endif
