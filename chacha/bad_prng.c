#include "chacha.h"

static uint8_t counter = 0;

void bad_prng_seed(uint64_t seed) {
    counter = (uint8_t)(seed & 0xFF);
}

uint32_t bad_prng_next32(void) {
    uint32_t result = 0;
    result |= counter++;
    result |= ((uint32_t)counter++) << 8;
    result |= ((uint32_t)counter++) << 16;
    result |= ((uint32_t)counter++) << 24;
    return result;
}

uint64_t bad_prng_next64(void) {
    uint64_t low = bad_prng_next32();
    uint64_t high = bad_prng_next32();
    return (high << 32) | low;
}