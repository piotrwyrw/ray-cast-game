#ifndef ENGINE_H
#define ENGINE_H

#include "math.h"

#define WALK_SPEED 0.08
#define MOUSE_SENSITIVITY 0.001
#define MAX_ANGLE_DELTA 1.0

struct camera {
	struct vector location;
	double angle;
};

extern struct camera cam;

void start();

#endif
