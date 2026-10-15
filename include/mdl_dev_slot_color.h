#ifndef MDL_DEV_SLOT_COLOR_H
#define MDL_DEV_SLOT_COLOR_H

#include "common.h"

/* vf10 is the initial color, vf11 its per-frame step, and vf12 the source
 * model's tint. Evaluate the age-dependent color and return its packed RGBA
 * word. The caller loads all three vectors. The 16-byte word carrier
 * follows the existing eePackRGBAWord pattern. */
static inline u32 mdlPackDevSlotColor(s32 age, f32 unit) {
    u32 rgba[4];

    __asm__ volatile (
        ".set noreorder\n\t"
        "qmtc2.ni %1, vf2\n\t"
        "vitof0.xyzw vf2, vf2\n\t"
        "vmulaw.xyzw ACC, vf10, vf0w\n\t"
        "vmaddx.xyzw vf2, vf11, vf2x\n\t"
        "vmul.xyzw vf10, vf2, vf12\n\t"
        "qmtc2.ni %2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vftoi0.xyzw vf10, vf10\n\t"
        "qmfc2.ni %0, vf10\n\t"
        "ppach %0, $0, %0\n\t"
        "ppacb %0, $0, %0\n\t"
        ".set reorder"
        : "=&r"(rgba[0])
        : "r"(age), "r"(unit));
    return rgba[0];
}

#endif /* MDL_DEV_SLOT_COLOR_H */
