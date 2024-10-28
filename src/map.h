#ifndef MAP_H
#define MAP_H

#include "math.h"

struct segment {
	struct vector *a;
	struct vector *b;
};

struct ray_cast {
	double seg_distance;
	double real_distance;
	struct segment *segment;
};

#define vec(_x, _y) ((struct vector) {.x = _x, .y = _y})
#define seg(_a, _b) ((struct segment) {.a = _a, .b = _b})
#define vert(_x, _y) vec(_x, _y)

extern struct vector vertices[];
extern struct segment segments[];

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg);

_Bool cast_ray(struct ray_cast *cast, struct vector *origin, struct vector *direction);

#endif
