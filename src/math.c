#include "math.h"
#include "map.h"

#include <math.h>
#include "math.h"

struct vector vector_copy(struct vector *ptr)
{
        return (struct vector) {
                .x = ptr->x,
                .y = ptr->y
        };
}

void vector_sub(struct vector *target, struct vector *vec)
{
        target->x -= vec->x;
        target->y -= vec->y;
}

void vector_add(struct vector *target, struct vector *vec)
{
        target->x += vec->x;
        target->y += vec->y;
}

double vector_length(struct vector *vec)
{
        return sqrt(vec->x * vec->x + vec->y * vec->y);
}

double vector_distance(struct vector *a, struct vector *b)
{
        struct vector delta = vector_copy(a);
        vector_sub(&delta, b);
        return vector_length(&delta);
}

void vector_div(struct vector *vec, double d)
{
        if (d == 0.0)
                return;

        vec->x /= d;
        vec->y /= d;
}

void vector_mul(struct vector *vec, double d)
{
        vec->x *= d;
        vec->y *= d;
}

void vector_normalize(struct vector *vec)
{
        vector_div(vec, vector_length(vec));
}

void vector_limit(struct vector *vec, double d)
{
        if (vector_length(vec) <= d)
                return;

        vector_normalize(vec);
        vector_mul(vec, d);
}

double vector_dot(struct vector *a, struct vector *b)
{
        return a->x * b->x + a->y + b->y;
}

struct vector vector_direction(struct vector *origin, struct vector *end)
{
        struct vector direction = vector_copy(end);
        vector_sub(&direction, origin);
        vector_normalize(&direction);
        return direction;
}

struct vector vector_dir_angle(double angle)
{
        return vec(cos(angle), sin(angle));
}

struct intersection ray_ray(double o1x, double o1y, double o2x, double o2y, double d1x, double d1y, double d2x, double d2y)
{
        double t = (d2x * (o1y - o2y) - d2y * (o1x - o2x)) / (d1x * d2y - d1y * d2x);
        double u = (d1x * (o1y - o2y) - d1y * (o1x - o2x)) / (d1x * d2y - d1y * d2x);
        return (struct intersection) {.t = t, .u = u};
}
