#include "enemy.h"
#include "health.h"

#include <stdbool.h>

static void damage(struct state *s)
{
        s->flash_anim.armed = true;
        damage_health(1);
}

void update_skeleton(struct state *s, struct entity *e, double distance)
{
        // Skeletons have multiple states that do not overlap
        if (e->state != STATE_CHARGING) {
                if (distance <= SKELETON_REACH) {
                        e->state = STATE_CHARGING;
                        e->anim = get_animation(ANIMATION_CHARGING);
                        return;
                }

                // If the player is out of reach, the skeleton shall walk towards them
                if (e->state != STATE_WALKING) {
                        e->state = STATE_WALKING;
                        e->anim = get_animation(ANIMATION_WALKING);
                        return;
                }
        }

        // Damage is dealt at the end of the charging animation
        if (e->state == STATE_CHARGING && !e->anim.armed) {
                e->anim = get_animation(ANIMATION_IDLE);
                e->state = STATE_DEFAULT;

                if (distance <= SKELETON_REACH) {
                        damage(s);
                }
        }
}

void update_wisp(struct state *s, struct entity *e, double distance)
{
        if (distance <= THRESHOLD_DISTANCE) {
                e->type = ENTITY_NONE;
                damage(s);
        }
}