#include "engine.h"
#include "map.h"
#include "math.h"
#include "state.h"
#include "defines.h"
#include "render.h"

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_render.h>
#include <SDL_scancode.h>
#include <stdbool.h>

const Uint8 *keyState;

struct camera cam = {
	.location = vec(0.0, 0.0),
	.angle = 0.0
};

struct vector cam_direction;

void update_direction()
{
	cam_direction = vector_dir_angle(cam.angle);
}

void walk(_Bool forward)
{
	update_direction();

	struct vector extension = cam_direction;
	vector_mul(&extension, forward ? WALK_SPEED : -WALK_SPEED);

	struct vector walk_direction = cam_direction;
	if (!forward)
		vector_mul(&walk_direction, -1.0);

	struct ray_cast cast;
	if (cast_ray(&cast, &cam.location, &walk_direction)) {
		if (cast.real_distance <= WALK_SPEED)
			return;
	}

	vector_add(&cam.location, &extension);

}

void render(struct state *s)
{
	render_view(s, &cam);

	SDL_RenderPresent(s->renderer);
}

void start()
{
	struct state s;
	initialize(&s);

	SDL_SetRelativeMouseMode(true);

	SDL_Event evt;

	while (!s.close) {
		while (SDL_PollEvent(&evt) > 0) {
			if (evt.type == SDL_QUIT)
				s.close = true;

			if (evt.type == SDL_MOUSEMOTION) {
				int dx = evt.motion.xrel;
				double dAngle = (dx + 0.0) * MOUSE_SENSITIVITY;
				dAngle = ((dAngle > MAX_ANGLE_DELTA) ? MAX_ANGLE_DELTA : dAngle);
				dAngle = ((dAngle < (- MAX_ANGLE_DELTA) ? (- MAX_ANGLE_DELTA) : dAngle));
				cam.angle += dAngle;
			}
		}

		keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_ESCAPE])
			s.close = true;

		if (keyState[SDL_SCANCODE_W])
			walk(true);

		if (keyState[SDL_SCANCODE_S])
			walk(false);

		render(&s);
	}
	close(&s);
}

