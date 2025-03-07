#ifndef RENDER_H
#define RENDER_H

#include "engine.h"
#include "state.h"

#include <SDL_render.h>

#define ORIGINAL_FOV (90 * (M_PI / 180))

extern double FOV;

#define PLANE_DISTANCE (2)
#define WALL_HEIGHT (HEIGHT / 2)

#define LIGHT_DISTANCE (10.0)
#define LIGHT_FALLOFF (200.0)

#define CROSSHAIR_SIZE 10

#define SKY_COLOR 255, 255, 255
#define FLOOR_COLOR 90, 90, 90

#define FLOOR_HEIGHT 10.0

double light_intensity(double distance);

double outbound_angle(double x);

void render_view(struct state *s);

#endif
