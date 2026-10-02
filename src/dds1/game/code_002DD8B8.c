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

extern void sdfBuildRotationMatrixFromAxisAngle(f32 angle, const RwV3d* axis, RwMatrix* matrix);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void sdfBuildVuRotationFromAxisAngle(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    sdfBuildRotationMatrixFromAxisAngle(angle, axis, &matrix);
    VU0_LOAD_MATRIX(&matrix);
}

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD8E8);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD968);

INCLUDE_ASM(const s32, "game/code_002DD8B8", func_002DD9E8);

/* Build an axis-angle matrix in the VU0 secondary matrix registers. */
void sdfVuLoadRotationMatrixFromAxisAngle(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    sdfBuildRotationMatrixFromAxisAngle(angle, axis, &matrix);
    VU0_LOAD_MATRIX_B(&matrix);
}

extern f32 func_002F9F60(f32 angle);
extern f32 func_002FA060(f32 angle);

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf29*s - vf30*c) */
void sdfRotateVuMatrixAboutX(f32 angle)
{
    f32 c = func_002F9F60(angle);
    f32 s = func_002FA060(angle);

    VU0_ROTATE_BASIS_X(c, s);
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf30*s - vf28*c, vf30 = vf30*c + vf28*s) */
void sdfRotateVuMatrixAboutY(f32 angle)
{
    f32 c = func_002F9F60(angle);
    f32 s = func_002FA060(angle);

    VU0_ROTATE_BASIS_Y(c, s);
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf29*s + vf28*c, vf29 = vf28*s - vf29*c) */
void sdfRotateVuMatrixAboutZ(f32 angle)
{
    f32 c = func_002F9F60(angle);
    f32 s = func_002FA060(angle);

    VU0_ROTATE_BASIS_Z(c, s);
}

extern void func_002DD608(f32 angle);
extern void func_002DD968(f32 angle);
extern void func_002DD9E8(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);

/* Compose the three axis rotations of a per-axis angle vector into the VU0 matrix. */
void vu0RotMatrixXYZFromVec3(const RwV3d *rot)
{
    func_002DD608(rot->x);
    func_002DD968(rot->y);
    sdfMultiplyVuMatrixInPlace();
    func_002DD9E8(rot->z);
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
        VU0_MATRIX4_MUL_BANK_B_LEFT();
}
