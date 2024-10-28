#include "render.h"

#include "defines.h"
#include "map.h"
#include "math.h"
#include <SDL_pixels.h>
#include <SDL_render.h>

#include <stdbool.h>
#include <math.h>

#define dim(f, r, g, b) f * r, f * g, f * b

void render_wall_line(struct state *s, int x, double distance, Uint8 r, Uint8 g, Uint8 b, _Bool contour)
{
	double df = (1.0 - (1.0 / (LIGHT_DISTANCE + 0.0)) * ((distance > LIGHT_DISTANCE) ? LIGHT_DISTANCE : distance));
	SDL_SetRenderDrawColor(s->renderer, dim(df, r, g, b), 255);

	if (contour)
		SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);

	int height = WALL_HEIGHT / distance;

	SDL_RenderDrawLine(s->renderer, x, HEIGHT / 2 - height, x, HEIGHT / 2 + height);

	SDL_SetRenderDrawColor(s->renderer, 0, 0, 0, 255);
	SDL_RenderDrawPoint(s->renderer, x, HEIGHT / 2 - height);
	SDL_RenderDrawPoint(s->renderer, x, HEIGHT / 2 + height);
}

void render_gradient(struct state *s, int y, int h, Uint8 r, Uint8 g, Uint8 b, _Bool direction)
{
	for (int i = 0; i < h; i ++) {
		double df;
		if (direction)
			df = (1.0 / (h + 0.0)) * i;
		else
			df = 1.0 - (1.0 / (h + 0.0)) * i;
		SDL_SetRenderDrawColor(s->renderer, dim(df, r, g, b), 255);
		SDL_RenderDrawLine(s->renderer, 0, i + y, WIDTH, i + y);
	}
}

void render_raycast(struct state *s, struct camera *cam)
{
	struct ray_cast cast;
	struct vector direction;

	for (int x = 0; x < WIDTH; x ++) {
		double angle = cam->angle - (FOV / 2) + (FOV / (WIDTH + 0.0)) * (x + 0.0);
		direction = vector_dir_angle(angle);

		struct segment *last_segment = cast.segment;

		if (!cast_ray(&cast, &cam->location, &direction))
			continue;

		double diff = cam->angle - angle;
		if (diff < 0 || diff > M_PI * 2)
			diff = M_PI * 2;

		render_wall_line(s, x, cast.real_distance * cos(diff), cast.segment->color.r, cast.segment->color.g, cast.segment->color.b, cast.segment != last_segment);
	}
}

void render_sky_and_floor(struct state *s)
{
	render_gradient(s, 0, HEIGHT / 2, SKY_COLOR, false);
	render_gradient(s, HEIGHT / 2, HEIGHT / 2, FLOOR_COLOR, true);
}


void render_view(struct state *s, struct camera *cam)
{
	render_sky_and_floor(s);
	render_raycast(s, cam);
}
