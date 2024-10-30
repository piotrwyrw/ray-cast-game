#include "state.h"
#include "defines.h"

#include <stdbool.h>

void initialize(struct state *s)
{
        SDL_Init(SDL_INIT_EVERYTHING);
        s->window = SDL_CreateWindow("Doom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     WIDTH, HEIGHT, 0);
        s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);
        if (!s->renderer) {
                printf("Could not create renderer.\n");
                s->close = true;
                return;
        }
        s->close = false;
}

void close(struct state *s)
{
        if (s->renderer)
                SDL_DestroyRenderer(s->renderer);

        if (s->window)
                SDL_DestroyWindow(s->window);
}
