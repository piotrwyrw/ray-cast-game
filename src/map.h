#ifndef MAP_H
#define MAP_H

#include "math.h"

#include "SDL.h"

struct color {
        Uint8 r;
        Uint8 g;
        Uint8 b;
};

struct segment {
        struct vector *a;
        struct vector *b;
        unsigned int txt;
        double txt_scale;
};

struct ray_cast {
        double seg_distance;
        double real_distance;
        struct segment *segment;
};

#define seg(_a, _b, _txt, _scale) ((struct segment) {.a = _a, .b = _b, .txt = _txt, .txt_scale = _scale})
#define vert(_x, _y) vec(_x, _y)

extern struct vector vertices[];
extern struct segment segments[];

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg);

_Bool cast_ray(struct ray_cast *cast, struct vector *origin, struct vector *direction);

#endif
