#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern s32 (*D_0040B480[])(void *a0, s32 a1);

extern s32 (*D_0040B510[])(void *a0, s32 a1);

extern u8 D_0040B518[];

extern s32 sdfAllocSizeClassBlock(s32);

typedef struct MotionKey {
    s32 id;
    f32 value;
} MotionKey;

typedef struct MotionKeyPair {
    MotionKey keys[2];
} MotionKeyPair; /* 0x10 */

/* Bound entries allocate 0x20 bytes. Sampling uses the header's source and
 * keyframe table; blending updates current, while the snapshot retains both
 * keys for the next transition's weighted merge. */
typedef struct MotionKeyWork {
    u32 unk00;
    void *source;             /* 0x04: supplies duration and loop settings */
    void *keyframes;          /* 0x08: times followed by fixed-stride key data */
    MotionKeyPair *current;   /* 0x0C */
    MotionKeyPair previous;   /* 0x10 */
} MotionKeyWork; /* 0x20 */

typedef struct MotionKeySample {
    MotionKey *first;
    MotionKey *second;
    f32 weight;
} MotionKeySample;

extern void sdfFindMotionKeyInterval(void *, void *);

void func_00335EE8(void) {
}

s32 sdfDispatchMotionCommand(void *object, s32 command) {
    return D_0040B510[(u16)command](object, command);
}

/* Select a 16-byte entry from the source object's motion pointer table. */
void sdfSelectMotionPointerEntry(s32 destination, s32 source, void *unused, s32 entryIndex) {
    sdfSetMotionPointerPair();
    ((MotionKeyWork *)destination)->current = (MotionKeyPair *)(*(s32 *)(*(s32 *)(*(s32 *)(source + 4) + 0x10) + 0xc) + entryIndex * 0x10);
}

s32 sdfAllocateBoundMotionPointerEntry(s32 source, s32 unused, s32 entryIndex) {
    s32 entry = sdfAllocSizeClassBlock(0x20);

    sdfSelectMotionPointerEntry(entry, source, D_0040B518, entryIndex);
    return entry;
}

/* Interpolate the sampled values, retaining separate keys when their IDs differ. */
void sdfBlendMotionKeys(MotionKeyWork *motion) {
    MotionKeySample sample;
    MotionKey *out;
    s32 firstId;
    s32 secondId;
    f32 firstValue;
    f32 secondValue;
    f32 weight;
    f32 inverse;

    sdfFindMotionKeyInterval(motion, &sample);
    firstId = sample.first->id;
    secondId = sample.second->id;
    firstValue = sample.first->value;
    secondValue = sample.second->value;
    weight = sample.weight;
    inverse = 1.0f - weight;
    out = motion->current->keys;
    if (firstId == secondId) {
        out[0].id = firstId;
        out[0].value = firstValue * inverse + secondValue * weight;
        out[1].id = secondId;
        out[1].value = 0;
    } else {
        out[0].id = firstId;
        out[0].value = firstValue * inverse;
        out[1].id = secondId;
        out[1].value = secondValue * weight;
    }
}

INCLUDE_ASM(const s32, "game/code_00335EE8", func_00336068);

/* Preserve the complete current pair for the next weighted transition. */
void sdfCopyPoseRecord(MotionKeyWork *motion) {
    motion->previous = *motion->current;
}

void sdfLoadPrimaryMatrixVU(void *matrix) {
    VU0_LOAD_MATRIX(matrix);
}

void sdfLoadAlternateMatrixVU(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
}

void func_00336258(void *dst) {
    VU0_STORE_MATRIX(dst);
}

void func_00336270(void *dst) {
    VU0_STORE_MATRIX_B(dst);
}

/* vu0 routine: vf24-vf27 = vf28-vf31 */
void func_00336288(void) {
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
}

/* vu0 routine: vf20-vf23 = vf28-vf31 */
void func_003362A0(void) {
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
}

/* vu0 routine: vf28-vf31 = vf24-vf27 */
void func_003362B8(void) {
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
}

/* vu0 routine: vf20-vf23 = vf24-vf27 */
void func_003362D0(void) {
    VU0_MOVE_VF(vf20, vf24);
    VU0_MOVE_VF(vf21, vf25);
    VU0_MOVE_VF(vf22, vf26);
    VU0_MOVE_VF(vf23, vf27);
}

/* vu0 routine: vf28-vf31 = vf20-vf23 */
void func_003362E8(void) {
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

/* vu0 routine: vf24-vf27 = vf20-vf23 */
void func_00336300(void) {
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

INCLUDE_ASM(const s32, "game/code_00335EE8", func_003364B8);

INCLUDE_ASM(const s32, "game/code_00335EE8", func_00336538);

INCLUDE_ASM(const s32, "game/code_00335EE8", func_003365B8);

extern f32 func_00353040(f32 angle);
extern f32 func_00353140(f32 angle);

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
    f32 cosine = func_00353040(angle);
    f32 sine = func_00353140(angle);

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
