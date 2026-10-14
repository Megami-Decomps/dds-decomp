#include "common.h"
#include "sdf_quaternion.h"
#include "rw_matrix.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "ee_mmi.h"

typedef float f32;


extern void sdfBuildRotationMatrixFromAxisAngle(f32 angle, const RwV3d* axis, RwMatrix* matrix);
extern f32 func_00353040(f32 angle);
extern f32 func_00353140(f32 angle);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void sdfBuildVuRotationFromAxisAngle(f32 angle, const RwV3d* axis)
{
    RwMatrix matrix;
    sdfBuildRotationMatrixFromAxisAngle(angle, axis, &matrix);
    VU0_LOAD_MATRIX(&matrix);
}

INCLUDE_ASM(const s32, "game/code_00336768", func_00336798);

INCLUDE_ASM(const s32, "game/code_00336768", sdfVuLoadSecondaryRotationMatrixY);

INCLUDE_ASM(const s32, "game/code_00336768", sdfVuLoadSecondaryRotationMatrixZ);

/* Build an axis-angle matrix in the VU0 secondary matrix registers. */
void sdfVuLoadRotationMatrixFromAxisAngle(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    sdfBuildRotationMatrixFromAxisAngle(angle, axis, &matrix);
    VU0_LOAD_MATRIX_B(&matrix);
}

extern void sdfVuLoadRotationMatrixX(f32 angle);
extern void sdfVuLoadSecondaryRotationMatrixY(f32 angle);
extern void sdfVuLoadSecondaryRotationMatrixZ(f32 angle);

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf29*s - vf30*c) */
void sdfRotateVuMatrixAboutX(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    VU0_ROTATE_BASIS_X(c, s);
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf30*s - vf28*c, vf30 = vf30*c + vf28*s) */
void sdfRotateVuMatrixAboutY(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    VU0_ROTATE_BASIS_Y(c, s);
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf29*s + vf28*c, vf29 = vf28*s - vf29*c) */
void sdfRotateVuMatrixAboutZ(f32 angle)
{
    f32 c = func_00353040(angle);
    f32 s = func_00353140(angle);

    VU0_ROTATE_BASIS_Z(c, s);
}

/* Compose the three axis rotations of a per-axis angle vector into the VU0 matrix. */
void vu0RotMatrixXYZFromVec3(const RwV3d *rot)
{
    sdfVuLoadRotationMatrixX(rot->x);
    sdfVuLoadSecondaryRotationMatrixY(rot->y);
    sdfMultiplyVuMatrixInPlace();
    sdfVuLoadSecondaryRotationMatrixZ(rot->z);
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
