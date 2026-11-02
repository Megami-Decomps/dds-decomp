#include "sdk_ps2.h"
#include "eff_resource_browser.h"
#include "sdf_sif_command.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "sdf_column_packet_api.h"
#include "sdf_packet_list.h"
#include "kwln_sprite.h"
#include "eff_event_setup.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_packet_alloc.h"
#include "sdf_bordered_rectangle_packet.h"
#include "eff.h"
#include "eff_magatuhi_api.h"
#include "eff_math.h"
#include "eff_cubic_bezier_array.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_projection.h"
#include "eff_dirent.h"

#define EFF_MATRIX_BYTES 0x40

#define EFF_VECTOR_WORD_COUNT 4

#define EFF_COLOR_UNPACK_SCALE_BITS 0x3C000000

#define EFF_SLOT_BYTES 0x38

#define EFF_SLOT_HEADER_BYTES 0xC

extern char D_00436448[];

extern char D_00436450[];

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];

extern u8 sdfViewTargetVector[];

extern u8 aViewDown[];

/* vu0 routine: transform the point in vf10 by view/projection, divide by w, then scale/bias.
   No clipping or zero-w check is performed; the result remains in vf10. */
void effScreenOffset(void) {
    u8 *projectionData;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    projectionData = sdfProjectionMatrix;
    MultMatrixRev((const struct Matrix4 *)projectionData);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    projectionData += EFF_MATRIX_BYTES;
    SDF_VU0_LOADVECTOR_B_BARRIER(projectionData);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B_BARRIER(D_0037F660);
    SDF_VU0_ADDVECTOR();
}

/* vu0 routine: project a point and its camera-right offset, returning their rounded screen distance. */
s32 effPCPBlurScreenOffset(f32 scale) {
    f32 scaleVector[4];
    f32 projectedEnd[4];
    f32 projectedStart[4];
    f32 original[4];
    f32 dx;
    f32 dy;

    SDF_VU0_STOREVECTOR_NOMEM(original);
    effScreenOffset();
    SDF_VU0_STOREVECTOR(projectedStart);

    scaleVector[0] = scaleVector[1] = scaleVector[2] = scale;
    SDF_VU0_LOADVECTOR(original);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B_BARRIER(scaleVector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();

    effScreenOffset();
    SDF_VU0_STOREVECTOR(projectedEnd);
    dx = projectedEnd[0] - projectedStart[0];
    dy = projectedEnd[1] - projectedStart[1];
    SDF_VU0_LOADVECTOR(projectedStart);
    return (s32)fsqrtf(dx * dx + dy * dy);
}

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effColorBlend(u32 colorA, u32 colorB, f32 t) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;

    if (t >= 1.0f) {
        return colorB;
    }
    unit = 0x3C000000;
    color1[0] = colorB;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_SCALAR_OP_CLOBBER(1.0f - t, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3_CLOBBER(t, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_ADDVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;

    return blended[0];
}

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */
u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB) {
    s32 colorAWords[EFF_VECTOR_WORD_COUNT];
    s32 colorBWords[EFF_VECTOR_WORD_COUNT];
    s32 packedWords[EFF_VECTOR_WORD_COUNT];
    u32 packed;
    u32 unpackScaleBits = EFF_COLOR_UNPACK_SCALE_BITS;
    colorAWords[0] = colorA;
    EE_MMI_RGBA_UNPACK(colorAWords, unpackScaleBits);
    SDF_VU0_COPYVECTOR_A_TO_B();
    colorBWords[0] = colorB;
    EE_MMI_RGBA_UNPACK(colorBWords, unpackScaleBits);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    packedWords[0] = packed;
    return packedWords[0];
}

/* vu0 routine: distance from point to the line through origin along a unit direction.
   Direction is not normalized here; dot/length use xyz despite full quadword loads. */
f32 effLinePointLength(f32 *direction, f32 *origin, f32 *point) {
    f32 projectedOffset[EFF_VECTOR_WORD_COUNT];
    f32 pointOffset[EFF_VECTOR_WORD_COUNT];
    f32 projectionAmount;
    f32 distance;

    SDF_VU0_LOADVECTOR(point);
    SDF_VU0_LOADVECTOR_B(origin);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(pointOffset);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_INNERPRODUCT(projectionAmount);
    /* The fourth projection component remains unwritten: only xyz contributes to distance. */
    projectedOffset[0] = direction[0] * projectionAmount;
    projectedOffset[1] = direction[1] * projectionAmount;
    projectedOffset[2] = direction[2] * projectionAmount;
    SDF_VU0_LOADVECTOR(pointOffset);
    SDF_VU0_LOADVECTOR_B(projectedOffset);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    return distance;
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */
EffCubicBezierArray *dds3Bezier4PCreate(s32 count) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(count * EFF_SLOT_BYTES + EFF_SLOT_HEADER_BYTES);
    EffCubicBezierSlot *slot = (EffCubicBezierSlot *)sdfMemLock(allocation);
    u32 slotIndex = 0;
    u8 *headerAddress = (u8 *)(slot + count);

    ((EffCubicBezierArray *)headerAddress)->allocation = allocation;
    ((EffCubicBezierArray *)headerAddress)->slots = slot;
    ((EffCubicBezierArray *)headerAddress)->slotCount = count;
    if (count != 0) {
        do {
            slotIndex++;
            slot->t = 0;
            slot->step = 0.05f;
            slot++;
        } while (slotIndex < count);
    }
    return (EffCubicBezierArray *)headerAddress;
}

extern f32 D_00433528;

void dds3Bezier4PDelete(EffCubicBezierArray *work) {
    sdfMemFreeDirect(work->allocation);
}

/* Evaluate the curve at the current t, advance t, and report whether the
 * stored next position was clamped to the endpoint. */
EffCubicBezierStepResult dds3Bezier4PPartRun(
    EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex, f32 *out) {
    EffCubicBezierSlot *slot = &table->slots[slotIndex];
    f32 w[4];
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = slot->controlPoints[0][0] * w[0] + slot->controlPoints[1][0] * w[1] + slot->controlPoints[2][0] * w[2] + slot->controlPoints[3][0] * w[3];
    out[1] = slot->controlPoints[0][1] * w[0] + slot->controlPoints[1][1] * w[1] + slot->controlPoints[2][1] * w[2] + slot->controlPoints[3][1] * w[3];
    out[2] = slot->controlPoints[0][2] * w[0] + slot->controlPoints[1][2] * w[1] + slot->controlPoints[2][2] * w[2] + slot->controlPoints[3][2] * w[3];
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END;
    }
    slot->t = t;
    return EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE;
}

/* Direct-slot variant of dds3Bezier4PPartRun. */
EffCubicBezierStepResult nbCameraLerpVec(EffCubicBezierSlot *slot, f32 *out) {
    f32 w[4];
    f32 t = slot->t;
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = slot->controlPoints[0][0] * w[0] + slot->controlPoints[1][0] * w[1] + slot->controlPoints[2][0] * w[2] + slot->controlPoints[3][0] * w[3];
    out[1] = slot->controlPoints[0][1] * w[0] + slot->controlPoints[1][1] * w[1] + slot->controlPoints[2][1] * w[2] + slot->controlPoints[3][1] * w[3];
    out[2] = slot->controlPoints[0][2] * w[0] + slot->controlPoints[1][2] * w[1] + slot->controlPoints[2][2] * w[2] + slot->controlPoints[3][2] * w[3];
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        slot->t = 1.0f;
        return EFF_CUBIC_BEZIER_STEP_CLAMPED_TO_END;
    }
    slot->t = t;
    return EFF_CUBIC_BEZIER_STEP_WITHIN_RANGE;
}

void effMathResetBezierSlot(EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex) {
    EffCubicBezierSlot *slot = &table->slots[slotIndex];
    slot->step = 0.05f;
    slot->t = 0;
}

EffCubicBezierSlot *dds3Bezier4PGetPart(
    EffCubicBezierArray *table, EffCubicBezierSlotIndex slotIndex) {
    return &table->slots[slotIndex];
}
