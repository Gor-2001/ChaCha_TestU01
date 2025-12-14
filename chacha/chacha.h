#ifndef CHACHA_H
#define CHACHA_H

#include <stdint.h>

void chacha_core(uint32_t output[16], const uint32_t input[16], int rounds);

void chacha20_seed(uint64_t seed);
uint32_t chacha20_next32(void);
uint64_t chacha20_next64(void);

void chacha12_seed(uint64_t seed);
uint32_t chacha12_next32(void);
uint64_t chacha12_next64(void);

void chacha8_seed(uint64_t seed);
uint32_t chacha8_next32(void);
uint64_t chacha8_next64(void);

void chacha4_seed(uint64_t seed);
uint32_t chacha4_next32(void);
uint64_t chacha4_next64(void);

void chacha4_feedback_seed(uint64_t seed);
uint32_t chacha4_feedback_next32(void);
uint64_t chacha4_feedback_next64(void);

void chacha2_feedback_seed(uint64_t seed);
uint32_t chacha2_feedback_next32(void);
uint64_t chacha2_feedback_next64(void);

void bad_prng_seed(uint64_t seed);
uint32_t bad_prng_next32(void);
uint64_t bad_prng_next64(void);

#endif