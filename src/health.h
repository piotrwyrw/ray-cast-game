#ifndef GAME_HEALTH_H
#define GAME_HEALTH_H

#include "state.h"

#define MAX_HEALTH 10
#define HEALING_DELAY 3000

extern Uint32 last_healing;
extern int health_points;

void render_health_bar(struct state *s);

void damage_health(int n);

void heal();

#endif