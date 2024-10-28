#include "state.h"
#include "defines.h"

#include <stdbool.h>

void initialize(struct state *s)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	s->window = SDL_CreateWindow("Doom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT, 0);
	s->renderer = SDL_CreateRenderer(s->window, -1, 0);
	s->close = false;
}

void close(struct state *s)
{
	SDL_DestroyRenderer(s->renderer);
	SDL_DestroyWindow(s->window);
}
