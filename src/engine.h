#ifndef ENGINE_H
#define ENGINE_H

#include "math.h"

#define WALK_SPEED 0.005
#define MOUSE_SENSITIVITY 0.01

struct camera {
	struct vector location;
	double angle;
};

extern struct camera cam;

void start();

#endif
