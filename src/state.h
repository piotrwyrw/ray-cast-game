#ifndef STATE_H
#define STATE_H

#include "SDL.h"

struct state {
	SDL_Window *window;
	SDL_Renderer *renderer;

	_Bool close;
};

_Bool initialize(struct state *s);
void close(struct state *s);

#endif
