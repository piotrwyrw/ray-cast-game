#ifndef MATH_H
#define MATH_H

#define vec(_x, _y) ((struct vector) {.x = _x, .y = _y})

struct vector {
        double x;
        double y;
};

struct vector vector_copy(struct vector *ptr);

void vector_sub(struct vector *target, struct vector *vec);

void vector_add(struct vector *target, struct vector *vec);

double vector_length(struct vector *vec);

double vector_distance(struct vector *a, struct vector *b);

void vector_div(struct vector *vec, double d);

void vector_mul(struct vector *vec, double d);

void vector_normalize(struct vector *vec);

void vector_limit(struct vector *vec, double d);

double vector_dot(struct vector *a, struct vector *b);

void vector_fmod(struct vector *vec, double d);

void vector_fmod2(struct vector *vec, double dx, double dy);

struct vector vector_dir_angle(double angle);

void vector_zero(struct vector *vec);

void vector_set(struct vector *dst, struct vector *src);

struct vector vector_direction(struct vector *origin, struct vector *end);

struct intersection {
        double t;
        double u;
};

struct intersection ray_ray(double o1x, double o1y, double o2x, double o2y, double d1x, double d1y, double d2x, double d2y);

struct vector perspective(double worldX, double worldY, double worldZ);

struct vector inverse_perspective(double screenX, double screenY, double worldY);

struct tex_uv {
        double u1;
        double v1;

        double u2;
        double v2;
};

struct tex_uv texture_uv(double screenY, double scale, double xOffset, double yOffset);

double lerp(double a, double b, double t);

#endif
