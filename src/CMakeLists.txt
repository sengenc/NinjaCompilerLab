cmake_minimum_required(VERSION 3.20)
project(Version8MK3 C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-g -Wall -pedantic)
include_directories(bigint/build/include)
link_directories(bigint/build/lib)
link_libraries(bigint)

add_executable(Version8MK3 main.c debug.c bigint/src/bigint.c bigint/tst/support.c debug.h structs.h)
