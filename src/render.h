#ifndef RENDER_H
#define RENDER_H

#include "engine.h"
#include "state.h"

#include <SDL_render.h>

#define FOV (90 * (M_PI / 180))
#define PLANE_DISTANCE (2)
#define WALL_HEIGHT (HEIGHT / 2)

#define SKY_COLOR 255, 255, 255
#define FLOOR_COLOR 60, 99, 130

void render_view(struct state *s);

#endif
