#include "sdk_ps2.h"
#include "snd_queued_command_dispatch_api.h"
#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"
#include "sdf_kernel_thread_param.h"

/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 dds3GetRandFloatA(EffRandState *state) {
    u32 value = dds3GetRandA(state) & 0xFFFFFF;

    return (f32)value * 5.9604644775390625e-8f;
}

u32 dds3GetRandIntA(EffRandState *state, u32 modulus) {
    return dds3GetRandA(state) % modulus;
}
