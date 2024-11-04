#include "engine.h"
#include "parse.h"
#include "map.h"
#include "util.h"

int main(void)
{
        const char *input = file_read("map.wrld");

        if (!input) {
                printf("Could not load map file.\n");
                return 0;
        }

        if (!map_parse(input, &game_map)) {
                printf("Could not parse map.\n");
                free((void *) input);
                return 0;
        }

        free((void *) input);
        start();
        return 0;
}
