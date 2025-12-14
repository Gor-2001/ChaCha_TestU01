#include "chacha.h"
#include <string.h>

static uint32_t chacha2_fb_state[16];
static uint32_t chacha2_fb_output[16];
static int chacha2_fb_position = 16;

static const uint32_t chacha_constant[4] = {
    0x61707865, 0x3320646e, 0x79622d32, 0x6b206574
};

void chacha2_feedback_seed(uint64_t seed) {
    memcpy(chacha2_fb_state, chacha_constant, sizeof(chacha_constant));

    chacha2_fb_state[4] = seed & 0xFFFFFFFF;
    chacha2_fb_state[5] = seed >> 32;
    chacha2_fb_state[6] = 0;
    chacha2_fb_state[7] = 0;

    chacha2_fb_state[8] = 0;
    chacha2_fb_state[9] = 0;
    chacha2_fb_state[10] = 0;
    chacha2_fb_state[11] = 0;

    chacha2_fb_state[12] = 0;
    chacha2_fb_state[13] = 0;
    chacha2_fb_state[14] = 0;
    chacha2_fb_state[15] = 0;

    chacha2_fb_position = 16;
}

uint32_t chacha2_feedback_next32(void) {
    if (chacha2_fb_position >= 16) {
        chacha_core(chacha2_fb_output, chacha2_fb_state, 2);

        memcpy(chacha2_fb_state, chacha_constant, sizeof(chacha_constant));

        for (int i = 0; i < 8; i++) {
            chacha2_fb_state[4 + i] = chacha2_fb_output[i];
        }

        chacha2_fb_position = 0;
    }
    
    return chacha2_fb_output[chacha2_fb_position++];
}

uint64_t chacha2_feedback_next64(void) {
    uint64_t high = (uint64_t)chacha2_feedback_next32() << 32;
    uint64_t low = (uint64_t)chacha2_feedback_next32();
    return high | low;
}