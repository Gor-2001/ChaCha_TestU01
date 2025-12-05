#ifndef RNG_INTERFACE_H
#define RNG_INTERFACE_H

#include <stdint.h>

typedef struct {
    const char *name;
    void (*seed)(uint64_t);
    uint32_t (*next32)(void);
    uint64_t (*next64)(void);
} rng_t;

#endif