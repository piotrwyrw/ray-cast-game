#ifndef SPAWN_H
#define SPAWN_H

#include "math.h"
#include "entity.h"

_Bool spawn_generic(enum entity_type type, struct vector location);

#define spawnf(id) _Bool spawn_##id(struct vector location);

spawnf(wisp)

spawnf(shroom)

spawnf(skeleton)

#undef spawnf

#endif