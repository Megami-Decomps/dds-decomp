#include "common.h"

typedef struct EffRandState
{
    u32 x[4];
} EffRandState;

extern EffRandState D_00398938;
extern f32 sdfSinPoly(f32 angle);
extern f32 func_002E78F8(f32 angle);
extern void sdfAtan2(f32 x, f32 y);
extern f32 D_00398380[4];

/* Persona 4 effMiscQuatMultiplyVU @ 004BCE50 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* vu0 routine: vf10 = quaternion product vf10 * vf11 */
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

/* VU-register calling convention: vf10 is the quaternion input and result. */
/* vu0 routine: vf10 = inverse of quaternion vf10 (conjugate / |q|^2) */
void effMiscInvertQuaternionVU(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "vmul.xyzw vf2, vf10, vf10\n"
        "vaddax.w ACC, vf2, vf2x\n"
        "vmadday.w ACC, vf0, vf2y\n"
        "vmaddz.w vf3, vf0, vf2z\n"
        "vmove.w vf2, vf10\n"
        "vdiv Q, vf0w, vf3w\n"
        "vsub.xyz vf2, vf0, vf10\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf2, Q\n"
        ".set reorder\n");
}

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

f32 effMiscQuatLengthSqVU(void)
{
    f32 result;
    __asm__ volatile (
        ".set noreorder\n"
        "vaddw.xyz vf1, vf0, vf0w\n"
        "vmul.xyzw vf2, vf10, vf10\n"
        "vmulay.x ACC, vf2, vf2y\n"
        "vmaddaz.x ACC, vf1, vf2z\n"
        "vmaddw.x vf2, vf1, vf2w\n"
        "qmfc2.ni $2, vf2\n"
        "mtc1 $2, %0\n"
        ".set reorder\n"
        : "=f"(result));
    return result;
}

/* Return the component-wise dot product of quaternions in vf10 and vf11. */
f32 effMiscQuaternionDotVU(void)
{
    f32 result;
    __asm__ volatile (
        ".set noreorder\n"
        "vaddw.xyz vf1, vf0, vf0w\n"
        "vmul.xyzw vf2, vf10, vf11\n"
        "vadday.x ACC, vf2, vf2y\n"
        "vmaddaz.x ACC, vf1, vf2z\n"
        "vmaddw.x vf2, vf1, vf2w\n"
        "qmfc2.ni $2, vf2\n"
        "mtc1 $2, %0\n"
        ".set reorder\n"
        : "=f"(result));
    return result;
}

/* vf10 initially holds the axis; write the sine-scaled axis and cosine W. */
void effMiscAxisAngleToQuaternionVU(f32 angle)
{
    f32 halfAngle = angle * 0.5f;
    f32 trigValue = sdfSinPoly(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder\n"
        : : "f"(trigValue));
    trigValue = func_002E78F8(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.w vf10, vf0, vf2x\n"
        ".set reorder\n"
        : : "f"(trigValue));
}

/* As above, but write the second VU quaternion register (vf11). */
void effMiscAxisAngleToQuaternionVf11(f32 angle)
{
    f32 halfAngle = angle * 0.5f;
    f32 trigValue = sdfSinPoly(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf11, vf11, vf2x\n"
        ".set reorder\n"
        : : "f"(trigValue));
    trigValue = func_002E78F8(halfAngle);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.w vf11, vf0, vf2x\n"
        ".set reorder\n"
        : : "f"(trigValue));
}

/* Persona 4 func_004bceb0 @ 004BCEB0 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* vu0 routine: rotation matrix vf28-vf31 from the quaternion in vf10 */
void effMiscQuaternionToMatrixVU(void)
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

INCLUDE_ASM(const s32, "game/code_002E7C20", func_002E7E08);

INCLUDE_ASM(const s32, "game/code_002E7C20", func_002E7F20);

INCLUDE_ASM(const s32, "game/code_002E7C20", func_002E8038);

INCLUDE_ASM(const s32, "game/code_002E7C20", func_002E8180);

/* vu0 routine: normalized lerp of quaternions vf10 and vf11 by amount (shorter arc), result in vf10 */
void effMiscQuaternionNlerpVU(f32 amount)
{
    f32 dot;
    __asm__ volatile (
        ".set noreorder\n"
        "vaddw.xyz vf1, vf0, vf0w\n"
        "vmul.xyzw vf2, vf10, vf11\n"
        "vadday.x ACC, vf2, vf2y\n"
        "vmaddaz.x ACC, vf1, vf2z\n"
        "vmaddw.x vf2, vf1, vf2w\n"
        "qmfc2.ni $2, vf2\n"
        "mtc1 $2, %0\n"
        ".set reorder\n"
        : "=f"(dot));
    if (dot < 0.0f) {
        __asm__ volatile (
            ".set noreorder\n"
            "vmulax.xyzw ACC, vf0, vf0x\n"
            "vmsubw.xyzw vf12, vf11, vf0w\n"
            ".set reorder\n");
    } else {
        __asm__ volatile ("vmove.xyzw vf12, vf11");
    }
    {
        f32 remaining = 1.0f - amount;
        __asm__ volatile (
            ".set noreorder\n"
            "mfc1 $2, %0\n"
            "mfc1 $3, %1\n"
            "qmtc2.ni $2, vf2\n"
            "qmtc2.ni $3, vf3\n"
            "vmulax.xyzw ACC, vf10, vf2x\n"
            "vmaddx.xyzw vf10, vf12, vf3x\n"
            ".set reorder\n"
            : : "f"(remaining), "f"(amount));
    }
    effMiscNormalizeVU();
}

/* vu0 routine: rotate D_00398380 by the quaternion matrix, atan2 of the result x and y */
void func_002E82F8(void)
{
    f32 x;
    f32 y;
    effMiscQuaternionToMatrixVU();
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%2)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddz.xyzw vf10, vf30, vf10z\n"
        "qmfc2.ni $2, vf10\n"
        "pexew $2, $2\n"
        "mtc1 $2, %0\n"
        "qmfc2.ni $2, vf10\n"
        "mtc1 $2, %1\n"
        ".set reorder\n"
        : "=f"(x), "=f"(y) : "r"(D_00398380) : "memory");
    sdfAtan2(x, y);
}

/* Persona 4 effMiscRand @ 004BD050 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* Four-word shift-register PRNG; NULL selects the shared effect RNG state. */
u32 effMiscRand(EffRandState* state)
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
    x0 = state->x[0];
    x1 = state->x[1];
    x2 = state->x[2];
    x3 = state->x[3];
    /* Mix the high two bits of x0 and high bit of x2 into the shifted taps. */
    rand = ((x1 << 0x02) | (((x0 >> 0x1e)) % 4)) ^ ((x3 << 0x01) | (((x2 >> 0x1f)) % 2));
    state->x[0] = rand;
    state->x[1] = x0;
    state->x[2] = x1;
    state->x[3] = x2;
    return rand;
}
