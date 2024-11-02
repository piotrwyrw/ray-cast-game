#include "drugs.h"
#include "render.h"

#include <stdbool.h>

double intensity = M_PI;
Uint32 trip_duration = 5000;
Uint32 trip_start = 0;
_Bool tripping = false;

void drugs_consume()
{
        tripping = true;
        trip_start = SDL_GetTicks();
}

double drugs_current_intensity(double phase)
{
        if (!tripping)
                return 0.0;

        double intensity_now = intensity - ((intensity / trip_duration) * (SDL_GetTicks() - trip_start));
        double intensity_d = (M_PI / intensity) * intensity_now;

        return (sin(intensity_d + phase) + 1.0) / 2.0;
}

void drugs_apply()
{
        if (!tripping)
                return;

        Uint32 now = SDL_GetTicks();
        if (now - trip_start > trip_duration) {
                tripping = false;
                FOV = ORIGINAL_FOV;
                return;
        }

        double angle = (M_PI / trip_duration) * (now - trip_start);

        double curr_int = drugs_current_intensity(0.0);

        WALK_SPEED = ORIGINAL_WALK_SPEED + fabs(sin(angle) * curr_int) / 4;
        FOV = ORIGINAL_FOV + fabs(sin(angle) * curr_int) / 2;
}
