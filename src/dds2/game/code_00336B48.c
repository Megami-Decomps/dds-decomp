#include "sdf_gs_header.h"
#include "common.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"

extern u32 D_00439188;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
extern u8 D_00476250[];

typedef struct VuBlendNode {
    f32 position[4];
    u8 pad10[0x20];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
    f32 weight;
    u8 region;
    u8 clipFlags;
    u8 pad52[0xE];
} VuBlendNode;

extern u16 D_00439184;

typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50: geometry references submitted to VU in chunks */
    u8 pad52[2];
    SdfAssetEntry *asset;  /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 ringSrc;           /* 0x64 */
    u32 ringDst;           /* 0x68 */
    u32 ringWrap;          /* 0x6C */
    u32 ringWrapCount;     /* 0x70 */
    u32 ringCount;         /* 0x74 */
    VuBlendNode *blend;    /* 0x78 */
    u32 ringEnd;           /* 0x7C */
    u32 state;             /* 0x80 */
    u32 header;            /* 0x84 */
    u8 *dataStart;         /* 0x88 */
    u8 *cursor;            /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 *strip;             /* 0x94 */
    u8 *positions;         /* 0x98 */
    u8 *normals;           /* 0x9C */
    u8 *coordinates;       /* 0xA0 */
    u8 *secondCoordinates; /* 0xA4 */
    u8 *vertexColors;      /* 0xA8 */
    f32 depth;             /* 0xAC */
} VuWork;

typedef struct SdfVuColorTransform {
    u8 pad00[4];
    f32 scaledY;
    u8 pad08[4];
    f32 scale;
    f32 colorBase[4];
    f32 colorDelta[4];
} SdfVuColorTransform;

extern void func_00336EC0(SdfVuColorTransform *, u32, u32, u32, u32, f32, f32, f32);

extern s32 D_00439180;

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
void sdfVuMultiplyPrimaryByScratch(void) {
        VU0_MATRIX4_MUL_PRIMARY_LEFT();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
void sdfVuMultiplyScratchByPrimary(void) {
        VU0_MATRIX4_MUL_SCRATCH_LEFT();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfPremultiplyVuMatrixFromMemory(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
    sdfComposeVuMatrixFromRegisters();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfPostmultiplyVuMatrixFromMemory(void *matrix) {
    VU0_LOAD_MATRIX_B(matrix);
    sdfMultiplyVuMatrixInPlace();
}

/* Transform inputVector by the matrix resident in VU0; write outputVector. */
void sdfVuTransformVector(void *outputVector, void *inputVector) {
    VU0_LOAD_VF(vf10, inputVector);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, outputVector);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
f32 sdfVuDot3(void *left, void *right) {
    f32 dot;
    VU0_LOAD_VF(vf10, left);
    VU0_LOAD_VF(vf11, right);
    VU0_DOT_XYZ(dot, vf10, vf11);
    return dot;
}

/* Write the XYZ cross product of leftVector and rightVector via VU0. */
void sdfVuCross3(void *outputVector, void *leftVector, void *rightVector) {
    VU0_LOAD_VF(vf10, leftVector);
    VU0_LOAD_VF(vf11, rightVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, outputVector);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* Build a VU basis from the normalized target-origin direction and up vector. */
/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void sdfVuBuildLookAtBasis(void *target, void *origin, void *up) {
    VU0_LOAD_VF(vf10, origin);
    VU0_MOVE_VF(vf31, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, target);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF_MEMORY(vf10, up);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf30);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf29, vf10);
    sdfInvertRigidVuTransform();
}

void sdfConfigureScratchpadRingTransfer(void) {
}

/* Split ring addressing at position 128; retain the native 96-byte stride. */
void sdfVuConfigureWorkRingDma(VuWork *work, s32 ringPosition) {
    u32 ringEnd = (u32)work + 0x78;
    if (ringPosition < 0x80) {
        work->ringWrapCount = 0x80 - ringPosition;
        work->ringWrap = D_00439180;
        work->ringDst = 0x70001000 + ringPosition * 0x60;
        work->ringCount = 0x2000;
        work->ringSrc = 0x70001000;
    } else if (ringPosition == 0x80) {
        work->ringSrc = 0x70001000;
        work->ringDst = D_00439180;
        work->ringWrapCount = 0x2000;
        work->ringWrap = 0;
        work->ringCount = 0;
    } else {
        work->ringSrc = D_00439180;
        work->ringDst = 0x70001000;
        work->ringWrapCount = 0x80;
        work->ringWrap = D_00439180 + ringPosition * 0x60;
        work->ringCount = 0x2000 - ringPosition;
    }
    work->ringEnd = ringEnd;
}

/* Decode four leading halfwords; selectionMask filters flags, not the payload. */
void sdfInitializeVuWorkParameters(VuWork *work, u16 *parameterWords, u32 selectionMask) {
    u8 *payload = (u8 *)(parameterWords + 4);
    u16 ringPosition;
    u16 parameterFlags;
    u16 nextParameter;
    u16 selectedFlags;
    work->param0 = parameterWords[0];
    ringPosition = parameterWords[1];
    work->param1 = ringPosition;
    parameterFlags = parameterWords[2];
    nextParameter = parameterWords[3];
    selectedFlags = parameterFlags & selectionMask;
    work->flags = parameterFlags;
    work->nextParam = nextParameter;
    work->selectedFlags = selectedFlags;
    D_00439184 = selectedFlags & 0x78;
    work->payload = payload;
    work->state = 0;
    sdfVuConfigureWorkRingDma(work, ringPosition);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void sdfVuRotateObjectBasis(void *vectors) {
    void *m = (void *)D_00439188;
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, D_00476250);
}

/* vu0 routine: modulate and clamp packed colors against the reference rows. */
void func_00336EC0(SdfVuColorTransform *result, u32 reference, u32 packedColor,
                   u32 firstColor, u32 secondColor,
                   f32 blend, f32 scale, f32 y) {
    f32 scaledY;

    if (scale < 1.0f) {
        scale = 1.0f;
    }
    scaledY = y * scale;
    result->scale = scale;
    result->scaledY = scaledY;
    VU0_BUILD_PACKED_COLOR_TRANSFORM(result, reference, packedColor,
                                     firstColor, secondColor, blend);
}

void sdfVuTransformWorkAtOffset(SdfVuColorTransform *out, SdfAssetEntry *work, u32 packedColor, f32 deltaX, f32 deltaY) {
    func_00336EC0(out, D_00439188, packedColor,
                  work->unk08, work->unk04,
                  work->unk1C,
                  work->x + deltaX,
                  work->y + deltaY);
}
