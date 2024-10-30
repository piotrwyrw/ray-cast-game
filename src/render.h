#ifndef RENDER_H
#define RENDER_H

#include "engine.h"
#include "state.h"

#include <SDL_render.h>

#define LIGHT_DISTANCE 10.0
#define FOV (90 * (M_PI / 180))
#define WALL_HEIGHT (HEIGHT / 2)

#define SKY_COLOR 255, 255, 255
#define FLOOR_COLOR 200, 200, 200

void render_view(struct state *s, struct camera *cam);

#endif
