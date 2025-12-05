#include "prng.h"
#include <stdint.h>

static uint64_t pcg_state = 0x853c49e6748fea9bULL;
static uint64_t pcg_inc = 0xda3e39cb94b95bdbULL;

void pcg_seed(uint64_t seed) {
    pcg_state = seed + pcg_inc;
    pcg_next32();
}

uint32_t pcg_next32(void) {
    uint64_t oldstate = pcg_state;
    pcg_state = oldstate * 6364136223846793005ULL + pcg_inc;
    
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint64_t pcg_next64(void) {
    uint64_t high = (uint64_t)pcg_next32() << 32;
    uint64_t low = (uint64_t)pcg_next32();
    return high | low;
}