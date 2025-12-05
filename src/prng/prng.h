#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

void pcg_seed(uint64_t seed);
uint32_t pcg_next32(void);
uint64_t pcg_next64(void);

void lcg_seed(uint64_t seed);
uint32_t lcg_next32(void);
uint64_t lcg_next64(void);

#endif