#include "health.h"
#include "textures.h"

Uint32 last_healing = 0;
int health_points = MAX_HEALTH;

void render_health_bar(struct state *s)
{
        const int x_org = 20;
        const int y_org = 20;

        const int x_spacing = 2;

        const int dst_size = 30;

        int src_width, src_height;
        SDL_QueryTexture(game_textures[HEART_INDEX], NULL, NULL, &src_width, &src_height);

        SDL_Rect src = {0, 0, src_width, src_height};

        for (int i = 0; i < MAX_HEALTH; i++) {
                SDL_Texture *texture;

                if (i < health_points)
                        texture = game_textures[HEART_INDEX];
                else
                        texture = game_textures[EMPTY_HEART_INDEX];

                SDL_Rect dst = (SDL_Rect) {.x = x_org + i * dst_size +
                                                x_spacing * i, .y = y_org, .w = dst_size, .h = dst_size};

                SDL_RenderCopy(s->renderer, texture, &src, &dst);
        }

        heal();
}

void damage_health(int n)
{
        if (n <= 0)
                return;

        health_points -= n;
        if (health_points < 0)
                health_points = 0;

        last_healing = SDL_GetTicks();
}

void heal()
{
        if (health_points >= MAX_HEALTH)
                return;

        Uint32 current_ticks = SDL_GetTicks();

        if (current_ticks - last_healing < HEALING_DELAY)
                return;

        last_healing = current_ticks;
        health_points++;
}
