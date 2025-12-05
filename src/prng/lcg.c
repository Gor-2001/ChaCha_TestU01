#include "prng.h"
#include <stdint.h>

static uint64_t lcg_state = 1;

#define LCG_A 6364136223846793005ULL
#define LCG_C 1442695040888963407ULL

void lcg_seed(uint64_t seed) {
    lcg_state = seed;
}

uint32_t lcg_next32(void) {
    lcg_state = lcg_state * LCG_A + LCG_C;
    return (uint32_t)(lcg_state >> 32);
}

uint64_t lcg_next64(void) {
    uint64_t high = (uint64_t)lcg_next32() << 32;
    uint64_t low = (uint64_t)lcg_next32();
    return high | low;
}