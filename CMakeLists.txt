cmake_minimum_required(VERSION 3.15)
project(chacha_prng_bench C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -march=native")

# Path to TestU01
set(TESTU01_LIB_DIR "/home/vboxuser/TestU01/lib")
include_directories("/home/vboxuser/TestU01/include")

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

target_link_libraries(rng_bench
    ${TESTU01_LIB_DIR}/libtestu01.a
    ${TESTU01_LIB_DIR}/libprobdist.a
    ${TESTU01_LIB_DIR}/libmylib.a
    m
)
