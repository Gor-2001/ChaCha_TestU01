#include "../../include/common.h"
#include "../prng/prng.h"
#include <stdint.h>
#include <stdio.h>

#define BENCH_BYTES (64ULL * 1024ULL * 1024ULL)  // 64MB benchmark

double benchmark_rng(uint32_t (*rng)(void)) {
    uint64_t total = 0;
    double start = now_seconds();

    for (uint64_t i = 0; i < BENCH_BYTES / 4; i++) {
        total += rng();   // use output so it's not optimized out
    }

    double end = now_seconds();
    double seconds = end - start;
    double mbps = (BENCH_BYTES / (1024.0 * 1024.0)) / seconds;

    return mbps;   // return throughput in MB/s
}
