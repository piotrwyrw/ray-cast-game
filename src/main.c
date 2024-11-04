#include "engine.h"
#include "parse.h"
#include "map.h"

int main(void)
{
        const char *input = "MERGE 0.001\n"
                            "SCALE 0.5\n"
                            "\n"
                            "WALL -5 5 5 5\n"
                            "WALL -5 5 -5 -2";

        if (!map_parse(input, &game_map)) {
                printf("Could not parse map.\n");
                return 0;
        }

        start();
        return 0;
}
