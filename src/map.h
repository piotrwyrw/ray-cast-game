#ifndef MAP_H
#define MAP_H

#include "math.h"

#include "SDL.h"

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
        struct vector point;
};

#define MAP_MAX_VERTICES 100
#define MAP_MAX_SEGMENTS (MAP_MAX_VERTICES * 2)

struct map {
        unsigned long vert_count;
        unsigned long seg_count;
        struct vector vertices[MAP_MAX_VERTICES];
        struct segment segments[MAP_MAX_SEGMENTS];
};

extern struct map game_map;

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg);

_Bool cast_ray(struct ray_cast *cast, struct vector *origin, struct vector *direction);

#endif
