#ifndef ENEMY_H
#define ENEMY_H

#include "state.h"
#include "entity.h"

void update_skeleton(struct state *s, struct entity *e, double distance);
void update_wisp(struct state *s, struct entity *e, double distance);

#endif