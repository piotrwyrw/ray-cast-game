#include "map.h"
#include "math.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define sgt(idx1, idx2, clr) seg(&vertices[idx1], &vertices[idx2], clr)

struct vector vertices[] = {
	// Main Hall
	vert(-5.0, 5.0),
	vert(-5.0, -5.0),
	vert(5.0, -5.0),
	vert(5.0, 5.0),
	
	// Room 1
	vert(-8.0, 8.0),
	vert(-8.0, 2.0),
	vert(-5.0, 2.0),
	vert(-5.0, 8.0),
	
	// Room 2
	vert(8.0, 8.0),
	vert(8.0, 2.0),
	vert(5.0, 2.0),
	vert(5.0, 8.0),
	
	// Side Corridor
	vert(2.0, -5.0),
	vert(2.0, -8.0),
	vert(-2.0, -8.0),
	vert(-2.0, -5.0)
};

struct segment segments[] = {
	// Main Hall
	sgt(0, 1, COLOR_WHITE),
	sgt(1, 2, COLOR_GREEN),
	sgt(2, 3, COLOR_GRAY),
	sgt(3, 0, COLOR_BLUE),
};

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg)
{
	struct vector direction = vector_direction(seg->a, seg->b);
	struct intersection inter = ray_ray(originX, originY, seg->a->x, seg->a->y, dirX, dirY, direction.x,  direction.y);

	double seg_length = vector_distance(seg->a, seg->b);

	if (inter.t < 0)
		return false;

	if (inter.u < 0 || inter.u > seg_length)
		return false;

	cast->segment = seg;
	cast->real_distance = inter.t;
	cast->seg_distance = inter.u;
	return true;
}

_Bool cast_ray(struct ray_cast *cast, struct vector *origin, struct vector *direction)
{
	_Bool intersection = false;

	struct ray_cast closest;
	size_t segment_count = sizeof(segments) / sizeof(struct segment);

	for (size_t i = 0; i < segment_count; i ++) {
		struct ray_cast cast;
		if (!ray_segment(&cast, origin->x, origin->y, direction->x, direction->y, &segments[i]))
			continue;
		if (intersection && cast.real_distance >= closest.real_distance)
			continue;
		closest = cast;
		intersection = true;
	}

	if (intersection)
		*cast = closest;

	return intersection;
}

