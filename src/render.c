#include "render.h"

#include "map.h"
#include "math.h"
#include "textures.h"
#include "entity.h"
#include "health.h"
#include "drugs.h"

#include <SDL.h>

#include <stdbool.h>

#define dim(f, r, g, b) f * r, f * g, f * b

double FOV = ORIGINAL_FOV;

double light_intensity(double distance)
{
        double light_distance = (distance > LIGHT_DISTANCE) ? LIGHT_DISTANCE : distance;
        return 255 - (LIGHT_FALLOFF / LIGHT_DISTANCE) * light_distance;
}

// Render the wall with the given segment texture via blitting
void render_wall_line(struct state *s, struct ray_cast *rc, int x, double distance, _Bool contour)
{
        int height = WALL_HEIGHT / distance;

        // Render the texture
        SDL_Rect dst = {
                .x = x,
                .y = HEIGHT / 2 - height,
                .w = 1,
                .h = height * 2
        };

        SDL_Texture *txt = game_textures[rc->segment->txt];
        int t_width, t_height;
        SDL_QueryTexture(txt, NULL, NULL, &t_width, &t_height);

        double length = vector_distance(rc->segment->a, rc->segment->b);

        SDL_Rect src = {
                .x = (int) ((rc->seg_distance / length) * ((double) t_width / rc->segment->txt_scale)) % t_width,
                .y = 0,
                .w = 1,
                .h = (int) (t_height / rc->segment->txt_scale)
        };

        double intensity = light_intensity(distance);

        SDL_SetTextureColorMod(txt, (Uint8) intensity, (Uint8) intensity, (Uint8) intensity);
        SDL_RenderCopy(s->renderer, txt, &src, &dst);

        SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);
        SDL_RenderDrawPoint(s->renderer, x, HEIGHT / 2 - height);
        SDL_RenderDrawPoint(s->renderer, x, HEIGHT / 2 + height);

        if (!contour)
                return;

        SDL_RenderDrawLine(s->renderer, x, HEIGHT / 2 - height, x, HEIGHT / 2 + height);
}

void render_gradient(struct state *s, int y, int h, Uint8 r, Uint8 g, Uint8 b, _Bool direction, double falloff)
{
        for (int i = 0; i < h; i++) {
                double df;
                if (direction)
                        df = falloff + ((1.0 - falloff) / (h + 0.0)) * i;
                else
                        df = 1.0 - (falloff / (h + 0.0)) * i;

                SDL_SetRenderDrawColor(s->renderer, dim(df, r, g, b), 255);
                SDL_RenderDrawLine(s->renderer, 0, i + y, WIDTH, i + y);
        }
}

#undef dim

// Calculate the ray angle via a projection plane. This is to tackle the distortion at the edges of the view.
double outbound_angle(double x)
{
        double plane_w = tan(FOV / 2) * PLANE_DISTANCE;
        plane_w *= 2.0;

        double plane_x = (x / (WIDTH - 1)) * plane_w;
        plane_x -= plane_w / 2;

        return atan(plane_x / PLANE_DISTANCE) + cam.angle;
}

void render_raycast(struct state *s)
{
        struct ray_cast cast;
        struct vector direction;

        for (int x = 0; x < WIDTH; x++) {
                double angle = outbound_angle(x);

                direction = vector_dir_angle(angle);
                vector_normalize(&direction);

                struct segment *last_segment = cast.segment;

                if (!cast_ray(&cast, &cam.location, &direction))
                        continue;

                double relativeAngle = angle - cam.angle;

                render_wall_line(s, &cast, x, cast.real_distance * cos(relativeAngle), cast.segment != last_segment);
        }
}

void render_sky_and_floor(struct state *s)
{
        render_gradient(s, 0, HEIGHT / 2, SKY_COLOR, false, 0.4);
        render_gradient(s, HEIGHT / 2, HEIGHT / 2, FLOOR_COLOR, true, 0.6);
}

void render_crosshair(struct state *s)
{
        SDL_SetRenderDrawColor(s->renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(s->renderer, WIDTH / 2 - CROSSHAIR_SIZE, HEIGHT / 2, WIDTH / 2 + CROSSHAIR_SIZE, HEIGHT / 2);
        SDL_RenderDrawLine(s->renderer, WIDTH / 2, HEIGHT / 2 - CROSSHAIR_SIZE, WIDTH / 2, HEIGHT / 2 + CROSSHAIR_SIZE);
}

void render_view(struct state *s)
{
        drugs_apply();
        render_sky_and_floor(s);
        render_raycast(s);
        entity_render_all(s);
        render_crosshair(s);

        SDL_SetRenderDrawBlendMode(s->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(s->renderer, 100, 0, 255, (255 / 2) * drugs_current_intensity(M_PI / 2));
        SDL_Rect rect = {0, 0, WIDTH, HEIGHT};
        SDL_RenderFillRect(s->renderer, &rect);

        animation_render(&s->flash_anim, s, (SDL_Rect) {.x = 0, .y = 0, .w = WIDTH, .h = HEIGHT});

        render_health_bar(s);

}
