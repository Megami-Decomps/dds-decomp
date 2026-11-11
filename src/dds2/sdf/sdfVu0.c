#include "common.h"
#include "ps2asm.h"
#include "sdf_lighting.h"

extern f32 cosf(f32 angle);

extern f32 sinf(f32 angle);

void LoadIdentity(void);

void DiagMatrix(void);

void ReverseMatrix(void);

void InversMatrix(void);

void SetRotateMatrixX(f32 aAngle);

void SetRotateMatrixY(f32 aAngle);

void SetRotateMatrixZ(f32 aAngle);

void StoreRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis, sdfMatrix4_t *aMtx);

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis);

void SetRotateMatrixX_B(f32 aAngle);

void SetRotateMatrixY_B(f32 aAngle);

void SetRotateMatrixZ_B(f32 aAngle);

void SetRotateMatrixAxis_B(const sdfVector3_t* aAngle, f32 aAxis);

void RotateMatrixX(f32 aAngle);

void RotateMatrixY(f32 aAngle);

void RotateMatrixZ(f32 aAngle);

void SetRotateMatrixXYZ(const sdfVector3_t *aAngles);

void MultMatrixNoArg(void);

void MultMatrixRevNoArg(void);

void MultMatrix(void *aMatrix);

void MultMatrixRev(const void *aMatrix);

void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

void LoadMatrix(const void *aMatrix) {
    SDF_VU0_LOADMATRIX(aMatrix);
}

void LoadMatrix_B(const void *aMatrix) {
    SDF_VU0_LOADMATRIX_B(aMatrix);
}

void StoreMatrix(void *aMatrix) {
    SDF_VU0_STOREMATRIX(aMatrix);
}

void StoreMatrix_B(void *aMatrix) {
    SDF_VU0_STOREMATRIX_B(aMatrix);
}

void CopyMatrixAtoB(void) {
    SDF_VU0_COPYMATRIX_A_TO_B();
}

void CopyMatrixAtoC(void) {
    SDF_VU0_COPYMATRIX_A_TO_C();
}

void CopyMatrixBtoA(void) {
    SDF_VU0_COPYMATRIX_B_TO_A();
}

void CopyMatrixBtoC(void) {
    SDF_VU0_COPYMATRIX_B_TO_C();
}

void CopyMatrixCtoA(void) {
    SDF_VU0_COPYMATRIX_C_TO_A();
}

void CopyMatrixCtoB(void) {
    SDF_VU0_COPYMATRIX_C_TO_B();
}

void LoadIdentity(void) {
    SDF_VU0_LOADIDENTITY();
}

void LoadIdentity_B(void) {
    SDF_VU0_LOADIDENTITY_B();
}

/* vu0 routine: store the unit matrix built from vf0 by MMI lane shuffles */
void StoreIdentity(f32 *aMatrix) {
    SDF_VU0_STOREIDENTITY(aMatrix);
}

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

void SetRotateMatrixX(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void SetRotateMatrixY(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void SetRotateMatrixZ(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX((f32 *)m);
}

void StoreRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis, sdfMatrix4_t *aMtx) {
    sdfVector4_t normalizedAxis;
    f32 x;
    f32 y;
    f32 z;
    f32 cosine = cosf(aAngle);
    f32 sine = sinf(aAngle);

    SDF_VU0_LOADVECTOR(aAxis);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&normalizedAxis);

    x = normalizedAxis.x;
    y = normalizedAxis.y;
    z = normalizedAxis.z;

    aMtx->m[0] = x * x + (1.0f - x * x) * cosine;
    aMtx->m[1] = x * y * (1.0f - cosine) + z * sine;
    aMtx->m[2] = x * z * (1.0f - cosine) - y * sine;
    aMtx->m[3] = 0.0f;
    aMtx->m[4] = x * y * (1.0f - cosine) - z * sine;
    aMtx->m[5] = y * y + (1.0f - y * y) * cosine;
    aMtx->m[6] = y * z * (1.0f - cosine) + x * sine;
    aMtx->m[7] = 0.0f;
    aMtx->m[8] = x * z * (1.0f - cosine) + y * sine;
    aMtx->m[9] = y * z * (1.0f - cosine) - x * sine;
    aMtx->m[10] = z * z + (1.0f - z * z) * cosine;
    aMtx->m[11] = 0.0f;
    SDF_VU0_STOREIDENTITYVECTOR(&aMtx->m[12]);
}

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t* aAxis)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(aAngle, aAxis, &matrix);
    SDF_VU0_LOADMATRIX(&matrix);
}

void SetRotateMatrixX_B(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[1][1] = c;
    m[1][2] = s;
    m[2][1] = -s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixY_B(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][2] = -s;
    m[2][0] = s;
    m[2][2] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixZ_B(f32 aAngle) {
    volatile f32 m[4][4] __attribute__((aligned(16)));
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    SDF_VU0_STOREIDENTITY((f32 *)m);
    m[0][0] = c;
    m[0][1] = s;
    m[1][0] = -s;
    m[1][1] = c;
    SDF_VU0_LOADMATRIX_B((f32 *)m);
}

void SetRotateMatrixAxis_B(const sdfVector3_t* aAngle, f32 aAxis)
{
    sdfMatrix4_t matrix;
    StoreRotateMatrixAxis(aAxis, aAngle, &matrix);
    SDF_VU0_LOADMATRIX_B(&matrix);
}

/* vu0 routine: rotate the vf29/vf30 pair by angle (vf29 = vf29*c + vf30*s, vf30 = vf30*c - vf29*s) */
void RotateMatrixX(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_X(c, s);
}

/* vu0 routine: rotate the vf28/vf30 pair by angle (vf28 = vf28*c - vf30*s, vf30 = vf30*c + vf28*s) */
void RotateMatrixY(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_Y(c, s);
}

/* vu0 routine: rotate the vf28/vf29 pair by angle (vf28 = vf28*c + vf29*s, vf29 = vf29*c - vf28*s) */
void RotateMatrixZ(f32 aAngle)
{
    f32 c = cosf(aAngle);
    f32 s = sinf(aAngle);

    VU0_ROTATE_BASIS_Z(c, s);
}

void SetRotateMatrixXYZ(const sdfVector3_t *aAngles)
{
    SetRotateMatrixX(aAngles->x);
    SetRotateMatrixY_B(aAngles->y);
    MultMatrixRevNoArg();
    SetRotateMatrixZ_B(aAngles->z);
    MultMatrixRevNoArg();
}

/* vu0 routine: vf28-vf31 = vf24-vf27 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg(void)
{
    VU0_MATRIX4_MUL_BANK_B_RIGHT();
}

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf24-vf27 (4x4 product) */
void MultMatrixRevNoArg(void)
{
        VU0_MATRIX4_MUL_BANK_B_LEFT();
}

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
void MultMatrixNoArg_C(void)
{
    VU0_MATRIX4_MUL_BANK_C_RIGHT();
}

/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void MultMatrixRevNoArg_C(void)
{
    VU0_MATRIX4_MUL_BANK_C_LEFT();
}

void MultMatrix(void *aMatrix) {
    SDF_VU0_LOADMATRIX_B(aMatrix);
    MultMatrixNoArg();
}

void MultMatrixRev(const void *aMatrix) {
    SDF_VU0_LOADMATRIX_B(aMatrix);
    MultMatrixRevNoArg();
}

void ApplyMatrix(void *aReVector, const void *aVector) {
    SDF_VU0_LOADVECTOR(aVector);
    SDF_VU0_APPLYMATRIX();
    SDF_VU0_STOREVECTOR(aReVector);
}

f32 InnerProduct(const void *a, const void *b) {
    f32 product;

    SDF_VU0_LOADVECTOR(a);
    SDF_VU0_LOADVECTOR_B(b);
    SDF_VU0_INNERPRODUCT(product);
    return product;
}

void OuterProduct(void *aReVector, const void *aVectorA, const void *aVectorB) {
    SDF_VU0_LOADVECTOR(aVectorA);
    SDF_VU0_LOADVECTOR_B(aVectorB);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(aReVector);
}

/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown) {
    SDF_VU0_LOADVECTOR(aEye);
    SDF_VU0_SET_ROW3();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(aTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW2();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(aDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_SET_ROW1();
    ReverseMatrix();
}
