#include "map.h"
#include "math.h"
#include "textures.h"
#include <stdbool.h>
#include <stdlib.h>

struct map game_map;

_Bool ray_segment(struct ray_cast *cast, double originX, double originY, double dirX, double dirY, struct segment *seg)
{
        struct vector direction = vector_direction(seg->a, seg->b);
        struct intersection inter = ray_ray(originX, originY, seg->a->x, seg->a->y, dirX, dirY, direction.x,
                                            direction.y);

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

        size_t segment_count = game_map.seg_count;

        for (size_t i = 0; i < segment_count; i++) {
                struct ray_cast tmp_cast;
                if (!ray_segment(&tmp_cast, origin->x, origin->y, direction->x, direction->y, &game_map.segments[i]))
                        continue;
                if (intersection && tmp_cast.real_distance >= closest.real_distance)
                        continue;
                closest = tmp_cast;
                intersection = true;
        }

        if (intersection) {
                closest.point = *direction;
                vector_mul(&closest.point, closest.real_distance);
                vector_add(&closest.point, origin);
                *cast = closest;
        }

        return intersection;
}
