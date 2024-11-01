#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>

#include "math.h"

#define WALK_SPEED 0.07
#define MOUSE_SENSITIVITY 0.001
#define MAX_ANGLE_DELTA 1.0
#define BULLET_SPEED 0.1

#define MIN_ENTITY_SIZE 5

#define MACHINE_GUN_DELAY 100

struct camera {
        struct vector location;
        double angle;
};

extern int WIDTH;
extern int HEIGHT;

extern struct camera cam;

extern Uint32 last_projectile;
extern _Bool machine_gun;

void start();

#endif
