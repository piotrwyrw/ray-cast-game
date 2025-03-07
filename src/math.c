#include "math.h"
#include "map.h"

#include <math.h>
#include "math.h"

#include "render.h"

struct vector vector_copy(struct vector *ptr)
{
        return (struct vector){
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

void vector_fmod(struct vector *vec, double d)
{
        vec->x = fmod(vec->x, d);
        vec->y = fmod(vec->y, d);
}

void vector_fmod2(struct vector *vec, double dx, double dy)
{
        vec->x = fmod(vec->x, dx);
        vec->y = fmod(vec->y, dy);
}

void vector_fabs(struct vector *vec)
{
        vec->x = fabs(vec->x);
        vec->y = fabs(vec->y);
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

void vector_zero(struct vector *vec)
{
        vec->x = 0;
        vec->y = 0;
}

void vector_set(struct vector *dst, struct vector *src)
{
        dst->x = src->x;
        dst->y = src->y;
}

struct intersection ray_ray(double o1x, double o1y, double o2x, double o2y, double d1x, double d1y, double d2x,
                            double d2y)
{
        double t = (d2x * (o1y - o2y) - d2y * (o1x - o2x)) / (d1x * d2y - d1y * d2x);
        double u = (d1x * (o1y - o2y) - d1y * (o1x - o2x)) / (d1x * d2y - d1y * d2x);
        return (struct intersection){.t = t, .u = u};
}

struct vector perspective(double worldX, double worldY, double worldZ)
{
        return (struct vector){
                .x = (worldX / worldZ) * (HEIGHT / (2.0 * tan(ORIGINAL_FOV / 2.0))) + (WIDTH / 2.0),
                .y = (worldY / worldZ) * (HEIGHT / (2.0 * tan(ORIGINAL_FOV / 2.0))) + (HEIGHT / 2.0),
        };
}

struct vector inverse_perspective(double screenX, double screenY, double worldY)
{
        return (struct vector){
                .x = -((worldY * (2.0 * screenX - WIDTH)) / (HEIGHT - 2.0 * screenY)),
                .y = -((worldY * HEIGHT) / (tan(FOV / 2.0) * (HEIGHT - 2.0 * screenY)))
        };
}

void rotate_y(struct vector *vec, double angle)
{
        double new_x = vec->x * cos(angle) - sin(angle) * vec->y;
        double new_y = vec->y * cos(angle) + sin(angle) * vec->x;
        vec->x = new_x;
        vec->y = new_y;
}

struct tex_uv texture_uv(double screenY, double scale, double xOffset, double yOffset)
{
        struct vector offset = {xOffset, yOffset};

        struct vector left = inverse_perspective(0, screenY, FLOOR_HEIGHT);
        rotate_y(&left, -cam.angle);
        vector_mul(&left, scale);
        vector_add(&left, &offset);

        struct vector right = inverse_perspective(WIDTH, screenY, FLOOR_HEIGHT);
        rotate_y(&right, -cam.angle);
        vector_mul(&right, scale);
        vector_add(&right, &offset);

        return (struct tex_uv){
                .u1 = left.x,
                .v1 = left.y,
                .u2 = right.x,
                .v2 = right.y,
        };
}

double lerp(double a, double b, double t)
{
        return a + (b - a) * t;
}
