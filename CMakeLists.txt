cmake_minimum_required(VERSION 3.15)
project(chacha_prng_bench C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -march=native")

link_directories(/usr/local/lib)

add_executable(rng_bench
    main.c
    chacha/chacha_core.c
    chacha/chacha20.c
    chacha/chacha12.c
    chacha/chacha8.c
    chacha/chacha4.c
    chacha/chacha4_feedback.c
    chacha/bad_prng.c
)

# 3. Link the library files by their base name, as they are now in a path CMake knows
target_link_libraries(rng_bench
    testu01
    probdist
    mylib
    m
)