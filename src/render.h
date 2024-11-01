#ifndef RENDER_H
#define RENDER_H

#include "engine.h"
#include "state.h"

#include <SDL_render.h>

#define FOV (90 * (M_PI / 180))
#define PLANE_DISTANCE (2)
#define WALL_HEIGHT (HEIGHT / 2)

#define LIGHT_DISTANCE (10.0)
#define LIGHT_FALLOFF (50.0)

#define SKY_COLOR 255, 255, 255
#define FLOOR_COLOR 223, 249, 251

void render_view(struct state *s);

#endif
