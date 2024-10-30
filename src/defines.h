#ifndef DEFINES_H
#define DEFINES_H

#define rgb(r, g, b) r, g, b, 255
#define gray(v) rgb(v, v, v)

#define vec(_x, _y) ((struct vector) {.x = _x, .y = _y})

#define signum(d) ((d >= 0.0) ? 1 : -1)

#endif
