#include "tests.h"
#include "../../include/common.h"
#include <stdio.h>
#include <string.h>

// List of supported RNG names
const char* RNG_NAMES[] = {
    "pcg", "lcg", "chacha20", "chacha12",
    "chacha8", "chacha4", "chacha4_feedback"
};

#define RNG_COUNT 7

// Map rng name → function pointer
random_func32 get_rng(const char* name) {
    if (strcmp(name, "pcg") == 0) return pcg_random;
    if (strcmp(name, "lcg") == 0) return lcg_random;
    if (strcmp(name, "chacha20") == 0) return chacha20_random;
    if (strcmp(name, "chacha12") == 0) return chacha12_random;
    if (strcmp(name, "chacha8") == 0) return chacha8_random;
    if (strcmp(name, "chacha4") == 0) return chacha4_random;
    if (strcmp(name, "chacha4_feedback") == 0) return chacha4_feedback_random;

    return NULL;
}

void run_test(const char* rng_name, const char* suite) {
    random_func32 rng = get_rng(rng_name);

    if (!rng) {
        fprintf(stderr, "Unknown RNG: %s\n", rng_name);
        return;
    }

    // Print metadata header
    printf("# RNG: %s\n", rng_name);
    printf("# Suite: %s\n", suite);

    // Run benchmark
    double mbps = benchmark_rng(rng);
    printf("# Throughput_MBps: %.2f\n", mbps);

    // Run test suite via TestU01 wrapper
    run_testu01_suite(rng, suite);
}
