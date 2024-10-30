#ifndef MAP_H
#define MAP_H

#include "math.h"

#include <SDL2/SDL.h>

struct color {
        Uint8 r;
        Uint8 g;
        Uint8 b;
};

struct segment {
        struct vector *a;
        struct vector *b;
        struct color color;
};

struct ray_cast {
        double seg_distance;
        double real_distance;
        struct segment *segment;
};

#define vec(_x, _y) ((struct vector) {.x = _x, .y = _y})
#define color(_r, _g, _b) ((struct color) {.r = _r, .g = _g, .b = _b})
#define seg(_a, _b, _color) ((struct segment) {.a = _a, .b = _b, .color = _color})
#define vert(_x, _y) vec(_x, _y)

#define COLOR_WHITE     color(255, 255, 255)
#define COLOR_GREEN     color(0, 250, 0)
#define COLOR_YELLOW    color(255, 195, 18)
#define COLOR_BLUE      color(9, 132, 227)

extern struct vector vertices[];
extern struct segment segments[];

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg);

_Bool cast_ray(struct ray_cast *cast, struct vector *origin, struct vector *direction);

#endif
