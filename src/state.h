#ifndef STATE_H
#define STATE_H

#include <SDL2/SDL.h>

struct state {
	SDL_Window *window;
	SDL_Renderer *renderer;

	_Bool close;
};

void initialize(struct state *s);
void close(struct state *s);

#endif
