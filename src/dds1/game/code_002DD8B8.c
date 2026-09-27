#include "common.h"

typedef float f32;

typedef struct RwV3d
{
    f32 x;
    f32 y;
    f32 z;
} RwV3d;

typedef struct RwMatrix
{
    RwV3d right;
    u32 flags;
    RwV3d up;
    u32 pad1;
    RwV3d at;
    u32 pad2;
    RwV3d pos;
    u32 pad3;
} RwMatrix;

extern void func_002DD788(f32 angle, const RwV3d* axis, RwMatrix* matrix);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void func_002DD8B8(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    func_002DD788(angle, axis, &matrix);
    __asm__ volatile (
        ".set noreorder          \n"
        "lqc2 vf28, 0(%0)        \n"
        "lqc2 vf29, 16(%0)       \n"
        "lqc2 vf30, 32(%0)       \n"
        "lqc2 vf31, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (&matrix)
        : "memory"
    );
}

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD8E8);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD968);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD9E8);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDA68);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDA98);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDAF8);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDB58);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDBB8);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DDBF8);

/* Persona 4 func_004bd450 @ 004BD450 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void func_002DDC50(void)
{
    __asm__ volatile (
        ".set noreorder                              \n"
        "vmulax.xyzw ACC, vf24, vf28x                \n"
        "vmadday.xyzw ACC, vf25, vf28y               \n"
        "vmaddaz.xyzw ACC, vf26, vf28z               \n"
        "vmaddw.xyzw vf28, vf27, vf28w               \n"
        "vmulax.xyzw ACC, vf24, vf29x                \n"
        "vmadday.xyzw ACC, vf25, vf29y               \n"
        "vmaddaz.xyzw ACC, vf26, vf29z               \n"
        "vmaddw.xyzw vf29, vf27, vf29w               \n"
        "vmulax.xyzw ACC, vf24, vf30x                \n"
        "vmadday.xyzw ACC, vf25, vf30y               \n"
        "vmaddaz.xyzw ACC, vf26, vf30z               \n"
        "vmaddw.xyzw vf30, vf27, vf30w               \n"
        "vmulax.xyzw ACC, vf24, vf31x                \n"
        "vmadday.xyzw ACC, vf25, vf31y               \n"
        "vmaddaz.xyzw ACC, vf26, vf31z               \n"
        "vmaddw.xyzw vf31, vf27, vf31w               \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}
