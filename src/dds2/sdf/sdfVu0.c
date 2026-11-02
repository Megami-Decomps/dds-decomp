#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_vu_matrix.h"
#include "sdf_quaternion.h"
#include "rw_matrix.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_motion_bindings.h"
#include "sdf_motion_node_api.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "sdf_gs_header.h"
#include "sdf_vu_color_transform.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_heap_api.h"
#include "sdf_primitive.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "sdf_texture_file.h"
#include "sdfCorePacketPointBuff_tag.h"

void LoadMatrix(void *matrix) {
    VU0_LOAD_MATRIX(matrix);
}

void LoadMatrix_B(void *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
}

void StoreMatrix(void *dst) {
    VU0_STORE_MATRIX(dst);
}

void StoreMatrix_B(void *dst) {
    SDF_VU0_STOREMATRIX_B(dst);
}

/* vu0 routine: vf24-vf27 = vf28-vf31 */
void CopyMatrixAtoB(void) {
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
}

/* vu0 routine: vf20-vf23 = vf28-vf31 */
void CopyMatrixAtoC(void) {
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
}

/* vu0 routine: vf28-vf31 = vf24-vf27 */
void CopyMatrixBtoA(void) {
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
}

/* vu0 routine: vf20-vf23 = vf24-vf27 */
void CopyMatrixBtoC(void) {
    VU0_MOVE_VF(vf20, vf24);
    VU0_MOVE_VF(vf21, vf25);
    VU0_MOVE_VF(vf22, vf26);
    VU0_MOVE_VF(vf23, vf27);
}

/* vu0 routine: vf28-vf31 = vf20-vf23 */
void CopyMatrixCtoA(void) {
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

/* vu0 routine: vf24-vf27 = vf20-vf23 */
void CopyMatrixCtoB(void) {
    VU0_MOVE_VF(vf24, vf20);
    VU0_MOVE_VF(vf25, vf21);
    VU0_MOVE_VF(vf26, vf22);
    VU0_MOVE_VF(vf27, vf23);
}

void LoadIdentity(void) {
    SDF_VU0_LOADIDENTITY();
}

void LoadIdentity_B(void) {
    SDF_VU0_LOADIDENTITY_B();
}

/* libvu0: sceVu0UnitMatrix */
void StoreIdentity(void *dst) {
    EE_MMI_UNIT_MATRIX(dst);
}

/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void DiagMatrix(void) {
    SDF_VU0_TRANSMATRIX();
}

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void ReverseMatrix(void) {
    VU0_MATRIX4_INVERT_RIGID();
}

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void InversMatrix(void) {
    VU0_MATRIX4_INVERT_SCALED();
}

/* Rotation loaders build the matrix in a volatile scratch array: it is read
 * only by the lqc2 asm through its address, and retail stores its four
 * elements in program order. */
void SetRotateMatrixX(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    VU0_LOAD_MATRIX((f32 *)m);
}

void SetRotateMatrixY(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    VU0_LOAD_MATRIX((f32 *)m);
}

/* Build and load the Z-axis rotation matrix in primary VU0 registers. */
void SetRotateMatrixZ(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    VU0_LOAD_MATRIX((f32 *)m);
}

void StoreRotateMatrixAxis(f32 angle, const RwV3d *axis, RwMatrix *matrix) {
    f32 normalizedAxis[4];
    f32 x;
    f32 y;
    f32 z;
    f32 cosine = cosf(angle);
    f32 sine = sinf(angle);

    SDF_VU0_LOADVECTOR(axis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(normalizedAxis);

    x = normalizedAxis[0];
    y = normalizedAxis[1];
    z = normalizedAxis[2];

    matrix->right.x = x * x + (1.0f - x * x) * cosine;
    matrix->right.y = x * y * (1.0f - cosine) + z * sine;
    matrix->right.z = x * z * (1.0f - cosine) - y * sine;
    matrix->flags = 0;
    matrix->up.x = x * y * (1.0f - cosine) - z * sine;
    matrix->up.y = y * y + (1.0f - y * y) * cosine;
    matrix->up.z = y * z * (1.0f - cosine) + x * sine;
    matrix->pad1 = 0;
    matrix->at.x = x * z * (1.0f - cosine) + y * sine;
    matrix->at.y = y * z * (1.0f - cosine) - x * sine;
    matrix->at.z = z * z + (1.0f - z * z) * cosine;
    matrix->pad2 = 0;
    VU0_STORE_VF(vf0, &matrix->pos);
}

typedef float f32;

extern void StoreRotateMatrixAxis(f32 angle, const RwV3d* axis, RwMatrix* matrix);

/* Persona 4 func_004bd380 @ 004BD380 (src/Graphics/Effect/effMisc.c), recompiled unchanged */
void SetRotateMatrixAxis(f32 angle, const RwV3d* axis)
{
    RwMatrix matrix;
    StoreRotateMatrixAxis(angle, axis, &matrix);
    VU0_LOAD_MATRIX(&matrix);
}

/* Rotation loaders build the matrix in a volatile scratch array: it is read
 * only by the lqc2 asm through its address, and retail stores its four
 * elements in program order. */
void SetRotateMatrixX_B(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void sdfVuLoadSecondaryRotationMatrixY(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixZ_B(f32 angle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    EE_MMI_UNIT_MATRIX((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

/* Build an axis-angle matrix in the VU0 secondary matrix registers. */
void SetRotateMatrixAxis_B(const RwV3d* axis, f32 angle)
{
    RwMatrix matrix;
    StoreRotateMatrixAxis(angle, axis, &matrix);
    SDF_VU0_LOADMATRIX_B(&matrix);
}

extern void SetRotateMatrixZ_B(f32 angle);

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf29*s - vf30*c) */
void RotateMatrixX(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_X(c, s);
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf30*s - vf28*c, vf30 = vf30*c + vf28*s) */
void RotateMatrixY(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_Y(c, s);
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf29*s + vf28*c, vf29 = vf28*s - vf29*c) */
void RotateMatrixZ(f32 angle)
{
    f32 c = cosf(angle);
    f32 s = sinf(angle);

    VU0_ROTATE_BASIS_Z(c, s);
}

/* Compose the three axis rotations of a per-axis angle vector into the VU0 matrix. */
void SetRotateMatrixXYZ(const RwV3d *rot)
{
    SetRotateMatrixX(rot->x);
    sdfVuLoadSecondaryRotationMatrixY(rot->y);
    MultMatrixRevNoArg();
    SetRotateMatrixZ_B(rot->z);
    MultMatrixRevNoArg();
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
void MultMatrixRevNoArg(void)
{
        VU0_MATRIX4_MUL_BANK_B_LEFT();
}

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg_C(void) {
        VU0_MATRIX4_MUL_PRIMARY_LEFT();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void MultMatrixRevNoArg_C(void) {
        VU0_MATRIX4_MUL_SCRATCH_LEFT();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void MultMatrix(const f32 *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
    sdfComposeVuMatrixFromRegisters();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void MultMatrixRev(const struct Matrix4 *matrix) {
    SDF_VU0_LOADMATRIX_B(matrix);
    MultMatrixRevNoArg();
}

/* Transform inputVector by the matrix resident in VU0; write outputVector. */
void ApplyMatrix(f32 *outputVector, const f32 *inputVector) {
    SDF_VU0_LOADVECTOR(inputVector);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(outputVector);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
f32 InnerProduct(const f32 *left, const f32 *right) {
    f32 dot;
    SDF_VU0_LOADVECTOR(left);
    SDF_VU0_LOADVECTOR_B(right);
    SDF_VU0_INNERPRODUCT(dot);
    return dot;
}

/* Write the XYZ cross product of leftVector and rightVector via VU0. */
void OuterProduct(f32 *outputVector, const f32 *leftVector, const f32 *rightVector) {
    SDF_VU0_LOADVECTOR(leftVector);
    SDF_VU0_LOADVECTOR_B(rightVector);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(outputVector);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* Build direction from the first vector relative to the second, then derive up/right. */
/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void LookAtMatrix(const f32 *targetPoint, const f32 *eyePosition, const f32 *upVector) {
    SDF_VU0_LOADVECTOR(eyePosition);
    SDF_VU0_SET_ROW3();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(targetPoint);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR_BARRIER(upVector);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_SET_ROW1();
    ReverseMatrix();
}
