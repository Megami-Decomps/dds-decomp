#include "common.h"
#include "sdf_vu0.h"
#include "fpu.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"

extern EffRandState D_00398938;

/* Persona 4 dds3GetRandA @ 004BD050 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* Four-word shift-register PRNG; NULL selects the shared effect RNG state. */
u32 dds3GetRandA(EffRandState* state)
{
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 rand;
    if (state == ((void*)0))
    {
        state = &D_00398938;
    }
    x0 = state->words[0];
    x1 = state->words[1];
    x2 = state->words[2];
    x3 = state->words[3];
    /* Mix the high two bits of x0 and high bit of x2 into the shifted taps. */
    rand = ((x1 << 0x02) | (((x0 >> 0x1e)) % 4)) ^ ((x3 << 0x01) | (((x2 >> 0x1f)) % 2));
    state->words[0] = rand;
    state->words[1] = x0;
    state->words[2] = x1;
    state->words[3] = x2;
    return rand;
}
