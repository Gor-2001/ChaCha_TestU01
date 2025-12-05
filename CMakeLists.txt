cmake_minimum_required(VERSION 3.15)
project(chacha_prng_bench C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -march=native")

# Path to TestU01
set(TESTU01_LIB_DIR "/home/vboxuser/TestU01/lib")
include_directories("/home/vboxuser/TestU01/include")

add_executable(rng_bench
    src/main.c
    src/prng/pcg.c
    src/prng/lcg.c
    src/chacha/chacha_core.c
    src/chacha/chacha20.c
    src/chacha/chacha12.c
    src/chacha/chacha8.c
    src/chacha/chacha4.c
    src/chacha/chacha4_feedback.c
    src/tests/testu01_wrapper.c
)

target_link_libraries(rng_bench
    ${TESTU01_LIB_DIR}/libtestu01.a
    ${TESTU01_LIB_DIR}/libprobdist.a
    ${TESTU01_LIB_DIR}/libmylib.a
    m
)
