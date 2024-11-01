#include "textures.h"
#include "engine.h"

#include <stdbool.h>

_Bool initialize(struct state *s)
{
        s->renderer = NULL;
        s->window = NULL;

        SDL_Init(SDL_INIT_EVERYTHING);

        s->window = SDL_CreateWindow("Lysergic Acid Diethylamide", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        s->renderer = SDL_CreateRenderer(s->window, -1, SDL_RENDERER_ACCELERATED);

        if (!s->renderer) {
                printf("Could not create renderer.\n");
                s->close = true;
                return false;
        }

        if (!load_textures(s))
                return false;

        SDL_SetRelativeMouseMode(true);
        s->close = false;

        return true;
}

void close(struct state *s)
{
        if (s->renderer)
                SDL_DestroyRenderer(s->renderer);

        if (s->window)
                SDL_DestroyWindow(s->window);
}
