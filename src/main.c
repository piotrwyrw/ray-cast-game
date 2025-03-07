#include "engine.h"
#include "parse.h"
#include "map.h"

int main(void)
{
        map_load("map.wrld", &game_map);
        start();
        return 0;
}
