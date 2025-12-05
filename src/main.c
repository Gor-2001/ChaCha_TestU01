#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/rng_interface.h"
#include "prng/prng.h"
#include "chacha/chacha.h"
#include "tests/tests.h"

static rng_t pcg_rng = {
    .name = "pcg",
    .seed = pcg_seed,
    .next32 = pcg_next32,
    .next64 = pcg_next64
};

static rng_t lcg_rng = {
    .name = "lcg",
    .seed = lcg_seed,
    .next32 = lcg_next32,
    .next64 = lcg_next64
};

static rng_t chacha20_rng = {
    .name = "chacha20",
    .seed = chacha20_seed,
    .next32 = chacha20_next32,
    .next64 = chacha20_next64
};

static rng_t chacha12_rng = {
    .name = "chacha12",
    .seed = chacha12_seed,
    .next32 = chacha12_next32,
    .next64 = chacha12_next64
};

static rng_t chacha8_rng = {
    .name = "chacha8",
    .seed = chacha8_seed,
    .next32 = chacha8_next32,
    .next64 = chacha8_next64
};

static rng_t chacha4_rng = {
    .name = "chacha4",
    .seed = chacha4_seed,
    .next32 = chacha4_next32,
    .next64 = chacha4_next64
};

static rng_t chacha4_feedback_rng = {
    .name = "chacha4_feedback",
    .seed = chacha4_feedback_seed,
    .next32 = chacha4_feedback_next32,
    .next64 = chacha4_feedback_next64
};

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rng_name> <test_suite>\n", argv[0]);
        fprintf(stderr, "Available RNGs: pcg, lcg, chacha20, chacha12, chacha8, chacha4, chacha4_feedback\n");
        fprintf(stderr, "Available suites: smallcrush, crush, bigcrush\n");
        return 1;
    }
    
    const char *rng_name = argv[1];
    const char *suite = argv[2];
    
    rng_t *rng = NULL;
    
    if (strcmp(rng_name, "pcg") == 0) {
        rng = &pcg_rng;
    } else if (strcmp(rng_name, "lcg") == 0) {
        rng = &lcg_rng;
    } else if (strcmp(rng_name, "chacha20") == 0) {
        rng = &chacha20_rng;
    } else if (strcmp(rng_name, "chacha12") == 0) {
        rng = &chacha12_rng;
    } else if (strcmp(rng_name, "chacha8") == 0) {
        rng = &chacha8_rng;
    } else if (strcmp(rng_name, "chacha4") == 0) {
        rng = &chacha4_rng;
    } else if (strcmp(rng_name, "chacha4_feedback") == 0) {
        rng = &chacha4_feedback_rng;
    } else {
        fprintf(stderr, "Unknown RNG: %s\n", rng_name);
        return 1;
    }
    
    rng->seed(time(NULL));
    
    run_testu01_battery(rng, suite);
    
    return 0;
}