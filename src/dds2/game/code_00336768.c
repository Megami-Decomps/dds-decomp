#include "common.h"
#include "pcp_vu0.h"

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

extern void func_00336638(f32 angle, const RwV3d* axis, RwMatrix* matrix);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void func_00336768(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    func_00336638(angle, axis, &matrix);
    VU0_LOAD_MATRIX(&matrix);
}

INCLUDE_ASM(const s32, "game/code_00336768", func_00336798);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336818);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336898);

/* Build an axis-angle matrix in the VU0 secondary matrix registers. */
void func_00336918(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    func_00336638(angle, axis, &matrix);
    VU0_LOAD_MATRIX_B(&matrix);
}

INCLUDE_ASM(const s32, "game/code_00336768", func_00336948);

INCLUDE_ASM(const s32, "game/code_00336768", func_003369A8);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336A08);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336A68);

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void func_00336AA8(void)
{
    __asm__ volatile (
        ".set noreorder\n"
        "vmulax.xyzw ACC, vf28, vf24x\n"
        "vmadday.xyzw ACC, vf29, vf24y\n"
        "vmaddaz.xyzw ACC, vf30, vf24z\n"
        "vmaddw.xyzw vf2, vf31, vf24w\n"
        "vmulax.xyzw ACC, vf28, vf25x\n"
        "vmadday.xyzw ACC, vf29, vf25y\n"
        "vmaddaz.xyzw ACC, vf30, vf25z\n"
        "vmaddw.xyzw vf3, vf31, vf25w\n"
        "vmulax.xyzw ACC, vf28, vf26x\n"
        "vmadday.xyzw ACC, vf29, vf26y\n"
        "vmaddaz.xyzw ACC, vf30, vf26z\n"
        "vmaddw.xyzw vf4, vf31, vf26w\n"
        "vmulax.xyzw ACC, vf28, vf27x\n"
        "vmadday.xyzw ACC, vf29, vf27y\n"
        "vmaddaz.xyzw ACC, vf30, vf27z\n"
        "vmaddw.xyzw vf31, vf31, vf27w\n"
        "vmove.xyzw vf28, vf2\n"
        "vmove.xyzw vf29, vf3\n"
        "vmove.xyzw vf30, vf4\n"
        ".set reorder\n");
}

/* Persona 4 func_004bd450 @ 004BD450 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void func_00336B00(void)
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
