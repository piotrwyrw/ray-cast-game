cmake_minimum_required(VERSION 3.30)

project(doom C)

add_executable(doom
		src/main.c)

set_property(TARGET doom PROPERTY C_STANDARD 99)


