#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

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
extern f32 func_00353040(f32 angle);
extern f32 func_00353140(f32 angle);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void sdfBuildVuRotationFromAxisAngle(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    func_00336638(angle, axis, &matrix);
    VU0_LOAD_MATRIX(&matrix);
}

INCLUDE_ASM(const s32, "game/code_00336768", func_00336798);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336818);

INCLUDE_ASM(const s32, "game/code_00336768", func_00336898);

/* Build an axis-angle matrix in the VU0 secondary matrix registers. */
void sdfVuLoadRotationMatrixFromAxisAngle(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    func_00336638(angle, axis, &matrix);
    VU0_LOAD_MATRIX_B(&matrix);
}

extern void func_003364B8(f32 angle);
extern void func_00336818(f32 angle);
extern void func_00336898(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf29*s - vf30*c) */
void sdfRotateVuMatrixAboutX(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $3, $vf3\n"
        "qmtc2.ni $2, $vf2\n"
        "vmove.xyzw $vf4, $vf29\n"
        "vmulax.xyzw ACC, $vf29, $vf3x\n"
        "vmaddx.xyzw $vf29, $vf30, $vf2x\n"
        "vmulax.xyzw ACC, $vf30, $vf3x\n"
        "vmsubx.xyzw $vf30, $vf4, $vf2x\n"
        ".set reorder\n"
        : : "f"(c), "f"(s) : "$2", "$3");
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf30*s - vf28*c, vf30 = vf30*c + vf28*s) */
void sdfRotateVuMatrixAboutY(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $3, $vf3\n"
        "qmtc2.ni $2, $vf2\n"
        "vmove.xyzw $vf4, $vf28\n"
        "vmulax.xyzw ACC, $vf28, $vf3x\n"
        "vmsubx.xyzw $vf28, $vf30, $vf2x\n"
        "vmulax.xyzw ACC, $vf4, $vf2x\n"
        "vmaddx.xyzw $vf30, $vf30, $vf3x\n"
        ".set reorder\n"
        : : "f"(c), "f"(s) : "$2", "$3");
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf29*s + vf28*c, vf29 = vf28*s - vf29*c) */
void sdfRotateVuMatrixAboutZ(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "mfc1 $2, %1\n"
        "qmtc2.ni $3, $vf3\n"
        "qmtc2.ni $2, $vf2\n"
        "vmove.xyzw $vf4, $vf28\n"
        "vmulax.xyzw ACC, $vf28, $vf3x\n"
        "vmaddx.xyzw $vf28, $vf29, $vf2x\n"
        "vmulax.xyzw ACC, $vf29, $vf3x\n"
        "vmsubx.xyzw $vf29, $vf4, $vf2x\n"
        ".set reorder\n"
        : : "f"(c), "f"(s) : "$2", "$3");
}

/* Compose the three axis rotations of a per-axis angle vector into the VU0 matrix. */
void vu0RotMatrixXYZFromVec3(const RwV3d *rot)
{
    func_003364B8(rot->x);
    func_00336818(rot->y);
    sdfMultiplyVuMatrixInPlace();
    func_00336898(rot->z);
    sdfMultiplyVuMatrixInPlace();
}

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void sdfComposeVuMatrixFromRegisters(void)
{
    VU0_APPLY_MATRIX(vf2, vf24);
    VU0_APPLY_MATRIX(vf3, vf25);
    VU0_APPLY_MATRIX(vf4, vf26);
    VU0_APPLY_MATRIX(vf31, vf27);
    VU0_MOVE_VF(vf28, vf2);
    VU0_MOVE_VF(vf29, vf3);
    VU0_MOVE_VF(vf30, vf4);
}

/* Persona 4 func_004bd450 @ 004BD450 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void sdfMultiplyVuMatrixInPlace(void)
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
