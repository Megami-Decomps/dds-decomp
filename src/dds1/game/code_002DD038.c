#include "common.h"
#include "sdf_chip.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "sdf_motion_bindings.h"

extern s32 (*D_003982D0[])(void *a0, s32 a1);

extern s32 (*D_00398360[])(void *a0, s32 a1);
extern u8 D_00398368[];

void func_002DD038(void) {
}

s32 sdfDispatchMotionCommand(void *object, s32 command) {
    return D_00398360[(u16)command](object, command);
}

/* Bind the selected pair of model slot indices and weights. */
void sdfSelectMotionPointerEntry(SdfMotionSlotPairBinding *destination,
                                 Motion *motion, void *dispatch, s32 entryIndex) {
    sdfSetMotionPointerPair((SdfMotionBindingHead *)&destination->keys,
                            motion, dispatch);
    destination->current = &((SdfSlotEntry *)motion->owner->slotPairs->buffer)[entryIndex];
}

SdfMotionSlotPairBinding *sdfAllocateBoundMotionPointerEntry(Motion *motion,
                                                           s32 unused, s32 entryIndex) {
    SdfMotionSlotPairBinding *entry = sdfAllocSizeClassBlock(sizeof(SdfMotionSlotPairBinding));

    sdfSelectMotionPointerEntry(entry, motion, D_00398368, entryIndex);
    return entry;
}

/* Sample the supplied frame and interpolate, retaining separate keys when IDs differ. */
void sdfBlendMotionKeys(SdfMotionSlotPairBinding *motion, f32 frame) {
    SdfMotionKeyInterval sample;
    SdfSlotPair *out;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 weight;
    f32 inverse;

    sdfFindMotionKeyInterval(&motion->keys, &sample, frame);
    firstId = ((SdfSlotPair *)sample.firstKey)->index;
    secondId = ((SdfSlotPair *)sample.secondKey)->index;
    firstValue = ((SdfSlotPair *)sample.firstKey)->weight;
    secondValue = ((SdfSlotPair *)sample.secondKey)->weight;
    weight = sample.weight;
    inverse = 1.0f - weight;
    out = motion->current->pair;
    if (firstId == secondId) {
        out[0].index = firstId;
        out[0].weight = firstValue * inverse + secondValue * weight;
        out[1].index = secondId;
        out[1].weight = 0;
    } else {
        out[0].index = firstId;
        out[0].weight = firstValue * inverse;
        out[1].index = secondId;
        out[1].weight = secondValue * weight;
    }
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD1B8);

/* Preserve the complete current pair for the next weighted transition. */
void sdfCopyPoseRecord(SdfMotionSlotPairBinding *motion) {
    motion->previous = *motion->current;
}

/* Matrix registers: vf28-vf31 are the primary matrix, vf24-vf27 its
 * alternate bank, and vf20-vf23 a third bank copied between the two. */

void sdfLoadPrimaryMatrixVU(void *matrix) {
    VU0_LOAD_MATRIX(matrix);
}

void sdfLoadAlternateMatrixVU(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
}

void sdfStorePrimaryMatrixVU(void *matrix) {
    VU0_STORE_MATRIX(matrix);
}

void sdfStoreAlternateMatrixVU(void *matrix) {
    VU0_STORE_MATRIX_B(matrix);
}

void sdfCopyPrimaryToAlternateMatrixVU(void) {
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
}

void sdfCopyPrimaryToTertiaryMatrixVU(void) {
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
}

void sdfCopyAlternateToPrimaryMatrixVU(void) {
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
}

void sdfCopyAlternateToTertiaryMatrixVU(void) {
    VU0_MOVE_VF(vf20, vf24);
    VU0_MOVE_VF(vf21, vf25);
    VU0_MOVE_VF(vf22, vf26);
    VU0_MOVE_VF(vf23, vf27);
}

void sdfCopyTertiaryToPrimaryMatrixVU(void) {
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

void sdfCopyTertiaryToAlternateMatrixVU(void) {
    VU0_MOVE_VF(vf24, vf20);
    VU0_MOVE_VF(vf25, vf21);
    VU0_MOVE_VF(vf26, vf22);
    VU0_MOVE_VF(vf27, vf23);
}

void sdfSetPrimaryIdentityMatrixVU(void) {
    VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
}

void sdfSetAlternateIdentityMatrixVU(void) {
    VU0_SET_UNIT_MATRIX(vf24, vf25, vf26, vf27);
}

/* libvu0: sceVu0UnitMatrix */
void sdfWriteIdentityMatrixToMemory(void *dst) {
    EE_MMI_UNIT_MATRIX(dst);
}

/* Transpose the four VU rows; EE MMI interleave is needed for packed vectors. */
/* libvu0: sceVu0TransposeMatrix, register form (vf28-vf31 in and out) */
void sdfTransposeVuMatrix(void) {
    VU0_MATRIX4_TRANSPOSE();
}

/* vu0 routine: rigid inverse of vf28-vf31 (transpose the 3x3, translation = -(R^T * t)) */
void sdfInvertRigidVuTransform(void) {
    VU0_MATRIX4_INVERT_RIGID();
}

/* vu0 routine: inverse of vf28-vf31 with per-axis scale removed (transpose, rows / |row|^2, translation = -(R^T * t)) */
void sdfInvertScaledVuTransform(void) {
    VU0_MATRIX4_INVERT_SCALED();
}

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD608);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD688);

INCLUDE_ASM(const s32, "game/code_002DD038", func_002DD708);

extern f32 func_002F9F60(f32 angle);
extern f32 func_002FA060(f32 angle);

typedef struct RwV3d {
    f32 x;
    f32 y;
    f32 z;
} RwV3d;

typedef struct RwMatrix {
    RwV3d right;
    u32 flags;
    RwV3d up;
    u32 pad1;
    RwV3d at;
    u32 pad2;
    RwV3d pos;
    u32 pad3;
} RwMatrix;

void sdfBuildRotationMatrixFromAxisAngle(f32 angle, const RwV3d *axis, RwMatrix *matrix) {
    f32 normalizedAxis[4];
    f32 x;
    f32 y;
    f32 z;
    f32 cosine = func_002F9F60(angle);
    f32 sine = func_002FA060(angle);

    VU0_LOAD_VF(vf10, axis);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, normalizedAxis);

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
