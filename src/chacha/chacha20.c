#include "chacha.h"
#include <string.h>

static uint32_t chacha20_state[16];
static uint32_t chacha20_output[16];
static int chacha20_position = 16;

static const uint32_t chacha_constant[4] = {
    0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
};

void chacha20_seed(uint64_t seed) {
    memcpy(chacha20_state, chacha_constant, 16);
    
    chacha20_state[4] = seed & 0xFFFFFFFF;
    chacha20_state[5] = seed >> 32;
    chacha20_state[6] = 0;
    chacha20_state[7] = 0;
    
    chacha20_state[8] = 0;
    chacha20_state[9] = 0;
    chacha20_state[10] = 0;
    chacha20_state[11] = 0;
    
    chacha20_state[12] = 0;
    chacha20_state[13] = 0;
    chacha20_state[14] = 0;
    chacha20_state[15] = 0;
    
    chacha20_position = 16;
}

uint32_t chacha20_next32(void) {
    if (chacha20_position >= 16) {
        chacha_core(chacha20_output, chacha20_state, 20);
        chacha20_state[12]++;
        if (chacha20_state[12] == 0) {
            chacha20_state[13]++;
        }
        chacha20_position = 0;
    }
    
    return chacha20_output[chacha20_position++];
}

uint64_t chacha20_next64(void) {
    uint64_t high = (uint64_t)chacha20_next32() << 32;
    uint64_t low = (uint64_t)chacha20_next32();
    return high | low;
}