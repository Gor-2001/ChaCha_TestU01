#include "chacha.h"
#include <string.h>

static uint32_t chacha12_state[16];
static uint32_t chacha12_output[16];
static int chacha12_position = 16;

static const uint32_t chacha_constant[4] = {
    0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
};

void chacha12_seed(uint64_t seed) {
    memcpy(chacha12_state, chacha_constant, 16);
    
    chacha12_state[4] = seed & 0xFFFFFFFF;
    chacha12_state[5] = seed >> 32;
    chacha12_state[6] = 0;
    chacha12_state[7] = 0;
    
    chacha12_state[8] = 0;
    chacha12_state[9] = 0;
    chacha12_state[10] = 0;
    chacha12_state[11] = 0;
    
    chacha12_state[12] = 0;
    chacha12_state[13] = 0;
    chacha12_state[14] = 0;
    chacha12_state[15] = 0;
    
    chacha12_position = 16;
}

uint32_t chacha12_next32(void) {
    if (chacha12_position >= 16) {
        chacha_core(chacha12_output, chacha12_state, 12);
        chacha12_state[12]++;
        if (chacha12_state[12] == 0) {
            chacha12_state[13]++;
        }
        chacha12_position = 0;
    }
    
    return chacha12_output[chacha12_position++];
}

uint64_t chacha12_next64(void) {
    uint64_t high = (uint64_t)chacha12_next32() << 32;
    uint64_t low = (uint64_t)chacha12_next32();
    return high | low;
}