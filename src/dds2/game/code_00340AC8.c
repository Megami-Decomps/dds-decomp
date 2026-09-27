#include "common.h"

typedef struct EffRandState
{
    u32 x[4];
} EffRandState;

extern EffRandState D_0040BAE8;

/* Persona 4 effMiscQuatMultiplyVU @ 004BCE50 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void effMiscQuatMultiplyVU(void)
{
    __asm__ volatile (
        ".set noreorder               \n"
        "vmul.xyzw vf2, vf10, vf11    \n"
        "vopmula.xyz ACC, vf10, vf11  \n"
        "vmaddaw.xyz ACC, vf11, vf10  \n"
        "vmaddaw.xyz ACC, vf10, vf11  \n"
        "vopmsub.xyz vf10, vf11, vf10 \n"
        "vmulaw.w ACC, vf10, vf11     \n"
        "vmsubax.w ACC, vf0, vf2      \n"
        "vmsubay.w ACC, vf0, vf2      \n"
        "vmsubz.w vf10, vf0, vf2      \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340AF8);

/* Persona 4 effMiscNormalizeVU @ 004BCE80 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void effMiscNormalizeVU(void)
{
    __asm__ volatile (
        ".set noreorder            \n"
        "vmul.xyzw vf2, vf10, vf10 \n"
        "vaddax.w ACC, vf2, vf2    \n"
        "vmadday.w ACC, vf0, vf2   \n"
        "vmaddz.w vf3, vf0, vf2    \n"
        "vrsqrt Q, vf0w, vf3w      \n"
        "vwaitq                    \n"
        "vmulq.xyzw vf10, vf10, Q  \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340B50);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340B78);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340BA0);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340BF0);

/* Persona 4 func_004bceb0 @ 004BCEB0 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void func_00340C40(void)
{
    __asm__ volatile (
        ".set noreorder                              \n"
        "vaddw.xyz vf1, vf0, vf0w                    \n"
        "vadd.xyzw vf2, vf10, vf10                   \n"
        "vmulx.w vf28, vf0, vf0x                     \n"
        "vmulx.w vf29, vf0, vf0x                     \n"
        "vmulx.w vf30, vf0, vf0x                     \n"
        "vmul.xyzw vf3, vf10, vf2                    \n"
        "vmuly.xyzw vf4, vf10, vf2y                  \n"
        "vmulz.xyzw vf5, vf10, vf2z                  \n"
        "vmulx.xyzw vf6, vf10, vf2x                  \n"
        "vaddaw.xyz ACC, vf0, vf0w                   \n"
        "vmsubay.x ACC, vf1, vf3y                    \n"
        "vmsubz.x vf28, vf1, vf3z                    \n"
        "vmsubax.y ACC, vf1, vf3x                    \n"
        "vmsubz.y vf29, vf1, vf3z                    \n"
        "vmsubax.z ACC, vf1, vf3x                    \n"
        "vmsuby.z vf30, vf1, vf3y                    \n"
        "vmulax.y ACC, vf1, vf4x                     \n"
        "vmsubw.y vf28, vf1, vf5w                    \n"
        "vaddw.x vf29, vf4, vf5w                     \n"
        "vsubw.x vf30, vf5, vf4w                     \n"
        "vmulax.z ACC, vf1, vf5x                     \n"
        "vmaddw.z vf28, vf1, vf4w                    \n"
        "vmulay.z ACC, vf1, vf5y                     \n"
        "vmsubw.z vf29, vf1, vf6w                    \n"
        "vaddw.y vf30, vf5, vf6w                     \n"
        "vmove.xyzw vf31, vf0                        \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340CB0);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340DC8);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00340EE0);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00341028);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_00341120);

INCLUDE_ASM(const s32, "game/code_00340AC8", func_003411A0);

/* Persona 4 effMiscRand @ 004BD050 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
u32 effMiscRand(EffRandState* state)
{
    u32 x0;
    u32 x1;
    u32 x2;
    u32 x3;
    u32 rand;
    if (state == ((void*)0))
    {
        state = &D_0040BAE8;
    }
    x0 = state->x[0];
    x1 = state->x[1];
    x2 = state->x[2];
    x3 = state->x[3];
    rand = ((x1 << 0x02) | (((x0 >> 0x1e)) % 4)) ^ ((x3 << 0x01) | (((x2 >> 0x1f)) % 2));
    state->x[0] = rand;
    state->x[1] = x0;
    state->x[2] = x1;
    state->x[3] = x2;
    return rand;
}
