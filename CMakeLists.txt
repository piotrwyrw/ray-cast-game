cmake_minimum_required(VERSION 3.29)

project(game C)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(game
        src/main.c
        src/state.c
        src/engine.c
        src/map.c
        src/math.c
        src/render.c
        src/textures.c
        src/entity.c
        src/health.c
        src/drugs.c
        src/enemy.c
        src/spawn.c
        src/parse.c
        src/util.c
)

target_link_libraries(game SDL2::SDL2 SDL2_image::SDL2_image)

set_property(TARGET game PROPERTY C_STANDARD 99)