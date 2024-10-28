cmake_minimum_required(VERSION 3.30)

project(game C)

find_package(SDL2 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(game
		src/main.c
		src/state.c
		src/engine.c
		src/map.c
		src/math.c
		src/render.c)

target_link_libraries(game ${SDL2_LIBRARIES})

set_property(TARGET game PROPERTY C_STANDARD 99)


