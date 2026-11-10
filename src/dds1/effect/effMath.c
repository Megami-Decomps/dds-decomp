#include "common.h"
#include "sdfFrame.h"
#include "sdf_types.h"
#include "ps2asm.h"
#include "fpu.h"

/* DDS's names for the same objects. */
#define func_001AC068 effMathResetBezierSlot

/* The owner is stored after its contiguous cubic segments. */
typedef struct
{
    f32 controlPoints[4][3];
    f32 t;
    f32 step;
} dds3Bezier4PPart_t;

typedef struct
{
    void *pPart;
    u32 number;
    void *allocation;
} effBezier4P_t;

extern void *sdfMemAllocDirect(u32 aSize);
extern void *sdfMemLock(void *aHandle);
extern void sdfMemFreeDirect(void *aHandle);


/* 1/128 as binary32: the colour unpack scales 0..255 integer lanes to 0..2. */
#define EFF_RGBA_UNIT 0x3C000000

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct
{
    u32 word;
    u8 pad04[12];
} effColorWork_t;

/* Only the projection block from +0x30 on is read here: a 4x4 matrix, then the
 * viewport half-extent and centre as one quadword each. */
typedef struct
{
    u8 pad00[0x30];
    f32 matrix[16];
    sdfVector4_t scale;
    sdfVector4_t offset;
    u8 pad90[0x10];
} dds3ScreenParam_t;

extern dds3ScreenParam_t dds3ScreenParam;
extern f32 sdfViewMatrix[16];
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewDown;
extern void MultMatrixRev(f32 *aMatrix);

/* vu0 routine: project the world point in vf10 to screen space */
void effScreenOffset(void)
{
    f32 *matrix;

    SDF_VU0_LOADMATRIX(sdfViewMatrix);
    matrix = dds3ScreenParam.matrix;
    MultMatrixRev(matrix);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(matrix + 16);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.offset);
    SDF_VU0_ADDVECTOR();
}

/* vu0 routine: measure the projected camera-right offset. */
s32 effPCPBlurScreenOffset(f32 scale) {
    sdfVector4_t scaleVector;
    sdfVector4_t projectedEnd;
    sdfVector4_t projectedStart;
    sdfVector4_t original;
    f32 dx;
    f32 dy;
    f32 distance;

    SDF_VU0_STOREVECTOR_NOMEM(&original);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(&projectedStart);

    scaleVector.x = scaleVector.y = scaleVector.z = scale;
    SDF_VU0_LOADVECTOR(&original);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B_BARRIER(&scaleVector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();

    effScreenOffset();
    SDF_VU0_STOREVECTOR(&projectedEnd);
    dx = projectedEnd.x - projectedStart.x;
    dy = projectedEnd.y - projectedStart.y;
    SDF_VU0_LOADVECTOR(&projectedStart);
    distance = fsqrtf(dx * dx + dy * dy);
    return (s32)distance;
}

/* vu0 routine: blend two colours, `amount` of `from` */
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate)
{
    effColorWork_t work[3];
    u32 blended;
    u32 unit;

    if (rate >= 1.0f) {
        return abgr1;
    }
    unit = EFF_RGBA_UNIT;
    work[0].word = abgr1;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = abgr0;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_SCALEVECTOR(1.0f - rate);
    SDF_VU0_SCALEVECTOR_B(rate);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_dds3ColorFtoI(blended, 128.0f);
    work[2].word = blended;
    return blended;
}

/* vu0 routine: unpack two colours, multiply and pack */
u32 effMagaCylinderCalcColor(u32 color, u32 scale)
{
    effColorWork_t work[3];
    u32 unit = EFF_RGBA_UNIT;
    u32 out;

    work[0].word = color;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = scale;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

/* vu0 routine: distance from `to` to the line through `from` along `dir` */
f32 effLinePointLength(sdfVector4_t *pDir, sdfVector4_t *pLinePos, sdfVector4_t *pPos)
{
    sdfVector4_t along;
    sdfVector4_t delta;
    f32 projection;
    f32 length;

    SDF_VU0_LOADVECTOR(pPos);
    SDF_VU0_LOADVECTOR_B(pLinePos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&delta);
    SDF_VU0_LOADVECTOR_B(pDir);
    SDF_VU0_INNERPRODUCT(projection);
    along.x = pDir->x * projection;
    along.y = pDir->y * projection;
    along.z = pDir->z * projection;
    SDF_VU0_LOADVECTOR(&delta);
    SDF_VU0_LOADVECTOR_B(&along);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    return length;
}

effBezier4P_t *dds3Bezier4PCreate(s32 number) {
    void *allocation = sdfMemAllocDirect(number * sizeof(dds3Bezier4PPart_t) + sizeof(effBezier4P_t));
    void *retainedAddress = (void *)sdfMemLock(allocation);
    u32 slotIndex = 0;
    dds3Bezier4PPart_t *slot = retainedAddress;
    u8 *headerAddress = (u8 *)slot + number * sizeof(dds3Bezier4PPart_t);

    ((effBezier4P_t *)headerAddress)->allocation = allocation;
    ((effBezier4P_t *)headerAddress)->pPart = retainedAddress;
    ((effBezier4P_t *)headerAddress)->number = number;
    if (number != 0) {
        do {
            slotIndex++;
            slot->t = 0;
            slot->step = 0.05f;
            slot = (dds3Bezier4PPart_t *)((u8 *)slot + sizeof(dds3Bezier4PPart_t));
        } while (slotIndex < number);
    }
    return (effBezier4P_t *)headerAddress;
}

void dds3Bezier4PDelete(effBezier4P_t *pEffBezier4P) {
    sdfMemFreeDirect(pEffBezier4P->allocation);
}

s32 dds3Bezier4PPartRun(effBezier4P_t *table, s32 slotIndex, f32 *out) {
    dds3Bezier4PPart_t *slot = &((dds3Bezier4PPart_t *)table->pPart)[slotIndex];
    sdfVector4_t w;
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w.x = u * u * u;
    w.y = t * (u * u) * 3.0f;
    w.z = t * t * u * 3.0f;
    w.w = t * t * t;
    out[0] = slot->controlPoints[0][0] * w.x + slot->controlPoints[1][0] * w.y + slot->controlPoints[2][0] * w.z + slot->controlPoints[3][0] * w.w;
    out[1] = slot->controlPoints[0][1] * w.x + slot->controlPoints[1][1] * w.y + slot->controlPoints[2][1] * w.z + slot->controlPoints[3][1] * w.w;
    out[2] = slot->controlPoints[0][2] * w.x + slot->controlPoints[1][2] * w.y + slot->controlPoints[2][2] * w.z + slot->controlPoints[3][2] * w.w;
    t += SDF_FRAME_STEP(slot->step);
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

s32 nbCameraLerpVec(dds3Bezier4PPart_t *slot, f32 *out) {
    sdfVector4_t w;
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w.x = u * u * u;
    w.y = t * (u * u) * 3.0f;
    w.z = t * t * u * 3.0f;
    w.w = t * t * t;
    out[0] = slot->controlPoints[0][0] * w.x + slot->controlPoints[1][0] * w.y + slot->controlPoints[2][0] * w.z + slot->controlPoints[3][0] * w.w;
    out[1] = slot->controlPoints[0][1] * w.x + slot->controlPoints[1][1] * w.y + slot->controlPoints[2][1] * w.z + slot->controlPoints[3][1] * w.w;
    out[2] = slot->controlPoints[0][2] * w.x + slot->controlPoints[1][2] * w.y + slot->controlPoints[2][2] * w.z + slot->controlPoints[3][2] * w.w;
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return 0;
    }
    slot->t = t;
    return 1;
}

void func_001AC068(effBezier4P_t *table, s32 slotIndex) {
    dds3Bezier4PPart_t *slot = &((dds3Bezier4PPart_t *)table->pPart)[slotIndex];

    slot->step = 0.05f;
    slot->t = 0;
}

dds3Bezier4PPart_t *dds3Bezier4PGetPart(effBezier4P_t *pEffBezier4P, s32 index) {
    return &((dds3Bezier4PPart_t *)pEffBezier4P->pPart)[index];
}
