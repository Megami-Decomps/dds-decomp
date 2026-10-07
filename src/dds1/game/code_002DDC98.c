#include "common.h"
#include "sdf_primitive.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

#define SDF_PAD_ENTRY_COUNT 2
#define SDF_PAD_REPLY_BUFFER_BYTES 0x20
#define SDF_PAD_ACTUATOR_BYTES 6
#define SDF_PAD_REPLY_DIGITAL 0x41
#define SDF_PAD_REPLY_ANALOG 0x73
#define SDF_PAD_REPLY_PRESSURE 0x79
#define SDF_PAD_STICK_COUNT 4
#define SDF_PAD_PRESSURE_COUNT 12
#define SDF_PAD_STICK_CENTER 0x80
#define SDF_PAD_BUTTON_COUNT 16
#define SDF_PAD_BUTTON_TRIGGER_BIT 2
#define SDF_PAD_BUTTON_NEW_PRESS_BIT 0x80
#define SDF_PAD_REPEAT_DELAY 15
#define SDF_PAD_REPEAT_STEP 4
#define SDF_PAD_MOTOR_VALUE_MASK 0xFF
#define SDF_PAD_PORT_BUFFER_BYTES 0x100
#define SDF_PAD_BUTTON_STATE_BYTES 0x20
#define SDF_PAD_STICK_STATE_BYTES 8
#define SDF_PAD_PRESSURE_STATE_BYTES 0x18
#define SDF_CONSOLE_CELL_BYTES 2
#define SDF_CONSOLE_NODE_BYTES 0x20

#define SDF_DMA_QWORD_SHIFT 4
#define SDF_DMA_CHCR_TTE 0x40
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF
#define SDF_QWORD_ALIGNMENT_MASK 0xF
#define SDF_TEXTURE_DRAW_PACKET_BYTES 0x50
#define SDF_GIF_REGISTER_AD 0xE
#define SDF_GS_FOGCOL_REGISTER 0x3D
#define SDF_GIF_NREG_SHIFT 60
#define SDF_GIF_PRIM_SHIFT 47
#define SDF_GIF_PRE_EOP_FLAGS 0x400000008000LL
#define SDF_VIF_DIRECT_COMMAND 0x50000000
#define SDF_VIF_FLUSHA_COMMAND 0x13000000
#define SDF_VIF_ITOP_MATRIX 0x04000002
#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002
#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008
#define SDF_VIF_ITOP_LIGHTING 0x04000010
#define SDF_VIF_MSCAL_COMMAND 0x14000000
#define SDF_VIF_MSCAL_TRIANGLES 0x14000004
#define SDF_VIF_MSCAL_VERTICES 0x14000008
#define SDF_VU_TEXTURED_FLAG 0x10
#define SDF_VU_TEXTURED_BATCH_LIMIT 0x10
#define SDF_VU_COLORED_BATCH_LIMIT 0x18
#define SDF_CAMERA_USE_FOV_FLAG 2
#define SDF_CAMERA_HALF_HEIGHT_FLAG 1


extern void *D_003BDA34;

extern u32 D_003BD378;

extern u32 D_003BD374;

extern u32 D_003BDA2C;

extern u32 D_003BDA28;

typedef struct ConsNode {
    /* 0x00 */ struct ConsNode *next;
    /* 0x04 */ struct ConsNode *prev;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ s16 columns;
    /* 0x0E */ s16 rows;
    /* 0x10 */ u16 cursorColumn;
    /* 0x12 */ u16 cursorRow;
    /* 0x14 */ u8 controlByte;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 textAttribute;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 bufferHandle;
    /* 0x1C */ u8 *cells;
} ConsNode;

/* Logical pad entries, sdfPadPorts[2] (0x28 bytes each). */
typedef struct F9B00Entry {
    /* 0x00 */ u8 port;
    /* 0x01 */ u8 slot;
    /* 0x02 */ u8 state;
    /* 0x03 */ u8 pad03;
    /* 0x04 */ u8 mode;
    /* 0x05 */ u8 requestedMode;
    /* 0x06 */ u16 buttons;
    /* 0x08 */ u16 prevButtons;
    /* 0x0A */ u8 pad0A[2];
    /* 0x0C */ u32 repeatDeadline;
    /* 0x10 */ u8 stick[4];
    /* 0x14 */ u8 pressure[12];
    /* 0x20 */ s16 smallMotor;
    /* 0x22 */ s16 largeMotor;
    /* 0x24 */ s16 lastSmallMotor;
    /* 0x26 */ s16 lastLargeMotor;
} F9B00Entry;

extern ConsNode *D_003BD3C0;
extern u32 D_003BD3C4;
extern u8 D_00315BA0[];
extern F9B00Entry sdfPadPorts[];
extern u32 D_00398660[];
extern u128 D_003F9890;
extern f32 D_003BDA30;
extern void *sdfAllocSizeClassBlock(s32 size);
extern u8 D_003F98A0[];
extern u8 D_003F9860[];
extern u128 *D_003EB860[][3];
extern SdfTex *D_003BD37C;
extern SdfTex *D_003BD380;
extern SdfTex *D_003BD390;
extern SdfTex *sdfTexAcquireResourceTexture(void *);
extern SdfTex *sdfTexAcquireAlternateResourceTexture(void *);
extern void *sdfEnsureFreeRootWorkspace(void *object);
extern void *sdfAllocPacketAligned(s32);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void func_002DE010(void *, u32, void *, u32, u32, f32, f32, f32);
extern s32 sdfGetPacketCursor(void);
extern u16 D_003BDA24;
extern s32 D_003BDA20;

typedef struct VuBlendNode {
    u8 pad00[0x30];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
} VuBlendNode;

typedef struct VuTransformWork {
    u8 pad00[4];
    u32 param4;            /* 0x04 */
    u32 param8;            /* 0x08 */
    u32 paramC;            /* 0x0C */
    u8 pad10[0xC];
    f32 scale;             /* 0x1C */
    u32 unk20;             /* 0x20 */
    u32 mode;              /* 0x24 */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];
    u64 unk38;             /* 0x38 */
    u64 unk40;             /* 0x40 */
    u64 unk48;             /* 0x48 */
    u64 unk50;             /* 0x50 */
    u64 unk58;             /* 0x58 */
    u64 unk60;             /* 0x60 */
} VuTransformWork;

typedef struct {
    u8 pad00[0x40];
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50 */
    u8 pad52[2];
    VuTransformWork *node; /* 0x54 */
    void *reference;       /* 0x58 */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 dmaBase;           /* 0x64 */
    u32 dmaAddrA;          /* 0x68 */
    u32 dmaAddrB;          /* 0x6C */
    u32 dmaCountA;         /* 0x70 */
    u32 dmaCountB;         /* 0x74 */
    VuBlendNode *blendList; /* 0x78 */
    void *dmaEnd;          /* 0x7C */
    u32 packetStart;       /* 0x80 */
    u32 header;            /* 0x84: aligned packet cursor + 0x30 */
    u32 *ringStart;        /* 0x88 */
    u32 *cursor;           /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 pad94[0x10];
    void *unkA4;           /* 0xA4 */
} VuWork;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
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
    void *ringEnd = (u8 *)work + 0x78;
    if (ringPosition < 0x80) {
        work->dmaCountA = 0x80 - ringPosition;
        work->dmaAddrB = D_003BDA20;
        work->dmaAddrA = ringPosition * 96 + 0x70001000;
        work->dmaCountB = 0x2000;
        work->dmaBase = 0x70001000;
    } else if (ringPosition == 0x80) {
        work->dmaBase = 0x70001000;
        work->dmaAddrA = D_003BDA20;
        work->dmaCountA = 0x2000;
        work->dmaAddrB = 0;
        work->dmaCountB = 0;
    } else {
        work->dmaCountA = 0x80;
        work->dmaBase = D_003BDA20;
        work->dmaAddrA = 0x70001000;
        work->dmaAddrB = D_003BDA20 + ringPosition * 96;
        work->dmaCountB = 0x2000 - ringPosition;
    }
    work->dmaEnd = ringEnd;
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
    D_003BDA24 = selectedFlags & 0x78;
    work->payload = payload;
    work->packetStart = 0;
    sdfVuConfigureWorkRingDma(work, ringPosition);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: rotate the basis at vectors+0x40 into vf24-vf26 and cache the original vectors. */
void sdfVuRotateObjectBasis(void *vectors) {
    void *m = (void *)D_003BDA28;
    VU0_ROTATE_BASIS_AND_CACHE(vectors, m, D_003F98A0);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE010);

void sdfVuTransformWorkAtOffset(void *out, VuTransformWork *work, void *reference, f32 deltaX, f32 deltaY) {
    func_002DE010(out, D_003BDA28, reference,
                  work->param8, work->param4,
                  work->scale,
                  work->x + deltaX,
                  work->y + deltaY);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE118);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE1A0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE268);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE2C0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE320);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE350);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE398);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE408);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE450);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE488);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE558);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE600);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE700);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE7D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE868);

void sdfVuBlendNodeXY(VuBlendNode *node) {
    while (node != NULL) {
        void *sourceA = node->sourceA;
        void *sourceB = node->sourceB;
        VU0_BLEND_NODE_XY(node, sourceA, sourceB);
        node = node->next;
    }
}

/* vu0 routine: lerp of two source rows (+0x20, +0x30) by the weight at node + 0x40 */
void sdfVuBlendNodeVectors(VuBlendNode *node) {
    while (node != NULL) {
        void *sourceA = node->sourceA;
        void *sourceB = node->sourceB;
        VU0_BLEND_NODE_VECTORS(node, sourceA, sourceB);
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE980);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEA18);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEAC0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEB40);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF128);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF710);

void sdfVuEmitTexturedTriangleBatches(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    if (remaining != 0) {
        u128 *(*nodes)[3] = D_003EB860;
        u32 *cursor = work->cursor;
        s32 first = 1;
        do {
            s32 chunk = (remaining <= 0x10) ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)cursor & 0xC) != 4) {
                *cursor++ = 0;
            }
            if (first) {
                VuTransformWork *node;
                *cursor++ = 0x6501C000;
                *cursor++ = 4;
                *cursor++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                node = work->node;
                first = 0;
                *(u64 *)cursor = 0x1000000000000003ULL;
                cursor += 2;
                *(u64 *)cursor = 0xE;
                cursor += 2;
                *(u64 *)cursor = node->unk38;
                cursor += 2;
                *(u64 *)cursor = 0x14;
                cursor += 2;
                *(u64 *)cursor = node->unk40;
                cursor += 2;
                *(u64 *)cursor = 6;
                cursor += 2;
                *(u64 *)cursor = node->unk48;
                cursor += 2;
                *(u64 *)cursor = 8;
                cursor += 2;
            } else {
                *cursor++ = 0x6501C000;
                *cursor++ = 0;
                *cursor++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)cursor = ((u64)(D_003BDA24 | 3) << 47) | (chunk * 3) | 0x3000400000008000ULL;
            cursor += 2;
            *(u64 *)cursor = 0x412;
            cursor += 2;
            do {
                u128 *a = (*nodes)[0];
                u128 *b = (*nodes)[1];
                u128 *c = (*nodes)[2];
                nodes++;
                ((u128 *)cursor)[0] = a[0];
                ((u128 *)cursor)[1] = a[3];
                ((u128 *)cursor)[2] = a[2];
                ((u128 *)cursor)[3] = b[0];
                ((u128 *)cursor)[4] = b[3];
                ((u128 *)cursor)[5] = b[2];
                ((u128 *)cursor)[6] = c[0];
                ((u128 *)cursor)[7] = c[3];
                ((u128 *)cursor)[8] = c[2];
                cursor += 0x24;
            } while (--chunk != 0);
            *cursor++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = cursor;
    }
}


/* Emit up to 16 triangles per batch; TEX1/TEX0/CLAMP/ALPHA context 2 state
 * precedes the first batch only. clearPrimitiveFlags filters GIF PRIM bits. */
void sdfBuildChunkedVuNodeTransfer(VuWork *work, u64 samplingState, u64 textureState, u64 clampState, u64 alphaState, s32 clearPrimitiveFlags) {
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = D_003EB860;
        u32 *packetCursor = work->cursor;
        s32 firstBatch = 1;
        do {
            s32 batchTriangles = (remainingTriangles <= SDF_VU_TEXTURED_BATCH_LIMIT) ? remainingTriangles : SDF_VU_TEXTURED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            if (firstBatch) {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 6;
                *packetCursor++ = ((batchTriangles * 9 + 7) << 16) | 0x6C00C001;
                firstBatch = 0;
                *(u64 *)packetCursor = 0x1000000000000005ULL;
                packetCursor += 2;
                *(u64 *)packetCursor = SDF_GIF_REGISTER_AD;
                packetCursor += 2;
                *(u64 *)packetCursor = samplingState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x15;
                packetCursor += 2;
                *(u64 *)packetCursor = textureState;
                packetCursor += 2;
                *(u64 *)packetCursor = 7;
                packetCursor += 2;
                *(u64 *)packetCursor = clampState;
                packetCursor += 2;
                *(u64 *)packetCursor = 9;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x51801;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x48;
                packetCursor += 2;
                *(u64 *)packetCursor = alphaState;
                packetCursor += 2;
                *(u64 *)packetCursor = 0x43;
                packetCursor += 2;
            } else {
                *packetCursor++ = 0x6501C000;
                *packetCursor++ = 0;
                *packetCursor++ = ((batchTriangles * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)packetCursor = ((u64)((D_003BDA24 & ~clearPrimitiveFlags) | 0x203) << SDF_GIF_PRIM_SHIFT) | (batchTriangles * 3) | 0x3000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x412;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[3];
                ((u128 *)packetCursor)[2] = vertexA[2];
                ((u128 *)packetCursor)[3] = vertexB[0];
                ((u128 *)packetCursor)[4] = vertexB[3];
                ((u128 *)packetCursor)[5] = vertexB[2];
                ((u128 *)packetCursor)[6] = vertexC[0];
                ((u128 *)packetCursor)[7] = vertexC[3];
                ((u128 *)packetCursor)[8] = vertexC[2];
                packetCursor += 0x24;
            } while (--batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}


/* Emit up to 24 colored triangles per batch, copying vertex quadwords 0 and 2.
 * Keep the native K&R declaration and caller convention. */
void sdfVuEmitColoredTriangleBatches(work)
    VuWork *work;
{
    s32 remainingTriangles = work->nodeCount;
    if (remainingTriangles != 0) {
        u128 *(*triangleRefs)[3] = D_003EB860;
        u32 *packetCursor = work->cursor;
        do {
            s32 batchTriangles = (remainingTriangles <= SDF_VU_COLORED_BATCH_LIMIT) ? remainingTriangles : SDF_VU_COLORED_BATCH_LIMIT;
            remainingTriangles -= batchTriangles;
            while (((u32)packetCursor & 0xC) != 4) {
                *packetCursor++ = 0;
            }
            *packetCursor++ = 0x6501C000;
            *packetCursor++ = 0x10000;
            *packetCursor++ = ((batchTriangles * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)packetCursor = (batchTriangles * 3) | ((u64)(D_003BDA24 | 3) << SDF_GIF_PRIM_SHIFT) | 0x2000400000008000ULL;
            packetCursor += 2;
            *(u64 *)packetCursor = 0x41;
            packetCursor += 2;
            do {
                u128 *vertexA = (*triangleRefs)[0];
                u128 *vertexB = (*triangleRefs)[1];
                u128 *vertexC = (*triangleRefs)[2];
                triangleRefs++;
                ((u128 *)packetCursor)[0] = vertexA[0];
                ((u128 *)packetCursor)[1] = vertexA[2];
                ((u128 *)packetCursor)[2] = vertexB[0];
                ((u128 *)packetCursor)[3] = vertexB[2];
                ((u128 *)packetCursor)[4] = vertexC[0];
                ((u128 *)packetCursor)[5] = vertexC[2];
                packetCursor += 0x18;
            } while (--batchTriangles != 0);
            *packetCursor++ = SDF_VIF_MSCAL_TRIANGLES;
        } while (remainingTriangles != 0);
        work->cursor = packetCursor;
    }
}


/* Select textured or colored emission; preserve the native no-argument calls. */
void sdfVuEmitSelectedNodePacket(VuWork *work) {
    if ((work->selectedFlags & SDF_VU_TEXTURED_FLAG) != 0) {
        sdfVuEmitTexturedTriangleBatches();
        return;
    }
    sdfVuEmitColoredTriangleBatches();
}

/* Finish the selected node's transformed rows and emit its textured geometry. */
void func_002E02D8(u32 workAddress) {
    VuWork *work = (VuWork *)workAddress;
    VuTransformWork *node;
    u32 mode;
    u32 paramC;
    u32 param8;

    func_002DE868((void *)work->dmaBase, work->param1, work->unkA4,
                  (u8 *)work->node + 0x80);
    sdfVuBlendNodeXY(work->blendList);
    node = work->node;
    mode = node->mode;
    paramC = node->paramC;
    param8 = node->param8;
    switch (mode) {
        case 0:
        case 1:
        case 2:
            if (param8 != paramC) {
                u128 parameters[3];

                func_002DE010(parameters, D_003BDA28, work->reference,
                              paramC, node->param4, node->scale,
                              node->x + work->offsetX, node->y + work->offsetY);
                func_002DE980(work, parameters);
            }
            break;
        case 3:
            func_002DEA18(work, paramC);
            break;
    }
    sdfBuildChunkedVuNodeTransfer(work, node->unk50, node->unk58,
                                  node->unk60, node->unk20, 0);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E03C0);

/* Emit selected geometry, then apply flag callbacks; the first may change flags. */
void sdfVuApplySelectedWorkFlags(u32 workAddress) {
    u32 selectedFlags;
    s32 signedWorkAddress;

    signedWorkAddress = (s32)workAddress;
    sdfVuEmitSelectedNodePacket((VuWork *)signedWorkAddress);
    selectedFlags = ((VuWork *)signedWorkAddress)->selectedFlags;
    if ((selectedFlags & 0x1000) != 0) {
        func_002E02D8(workAddress);
        selectedFlags = ((VuWork *)signedWorkAddress)->selectedFlags;
    }
    if ((selectedFlags & 1) != 0) {
        func_002E03C0(workAddress);
        return;
    }
}

extern void func_002DF128(VuWork *work);
extern void func_002DF710(VuWork *work);
extern void func_002DE7D8(u32 base, s32 count);

/* Align the packet to 64 bytes; the optional second pass preserves vf24-vf26. */
void sdfVuBeginPacketFromWork(VuWork *work) {
    u32 packetStart = sdfGetPacketCursor();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = ((alignedStart + 0x40) & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    work->packetStart = packetStart;
    work->header = alignedStart + 0x30;
    work->ringStart = (u32 *)ringAddress;
    work->cursor = (u32 *)ringAddress;
    if ((work->selectedFlags & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF_UNCLOBBERED(vf24, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf25, 16, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf26, 32, savedVectors);
        func_002DF128(work);
        sdfVuApplySelectedWorkFlags((u32)work);
        sdfVuConfigureWorkRingDma(work, work->param1);
        VU0_LOAD_VF(vf24, savedVectors);
        VU0_LOAD_VF_AT(vf25, 16, savedVectors);
        VU0_LOAD_VF_AT(vf26, 32, savedVectors);
        func_002DE7D8(work->dmaBase, work->param1);
        func_002DF710(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    } else {
        func_002DF128(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    }
}


u64 *func_002E0618(VuWork *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->packetStart == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->ringStart;
    if (cursor == start) {
        sdfSetPacketCursorAligned(work->packetStart);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *(u32 *)cursor = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        *(u128 *)cursor = 0;
        cursor += 0x10;
    }

    sdfSetPacketCursorAligned(cursor & 0x0FFFFFFF);
    header = (u64 *)work->header;
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E06F0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0820);

typedef struct VuObjectContext {
    u8 pad00[0x0C];
    u32 *objects; /* 0x0C: indexed object handles */
} VuObjectContext;

typedef struct VuObjectRefCommand {
    u8 pad00[0x16];
    u16 flags; /* 0x16 */
    u16 count; /* 0x18 */
    u16 objectIndices[1]; /* 0x1A */
} VuObjectRefCommand;

void sdfProcessReferencedObjects(VuObjectContext **context, VuObjectRefCommand *source) {
    u32 *objects = (*context)->objects;
    u16 *indices = &source->count;
    if ((source->flags & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfEnsureFreeRootWorkspace((void *)objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfVuSelectTransformMatrix(u32 matrixAddress) {
    D_003BDA28 = matrixAddress;
}

/* Cache the object's vector only when its address changes, not its contents. */
void sdfVuCacheObjectVector(u8 *sourceObject) {
    if (D_003BDA2C != (u32)sourceObject) {
        D_003BDA2C = (u32)sourceObject;
        PCP_COPY_VECTOR(&D_003F9890, sourceObject + 0x10);
    }
}


void sdfVuSetGlobalScale(f32 scale) {
    D_003BDA30 = scale;
}

void sdfVuClearTransformCache(void) {
    D_003BDA28 = 0;
    D_003BDA2C = 0;
}

extern u8 D_003200A0[];
extern u8 D_00320630[];
extern s32 D_003BD350;
extern s32 D_003BDA20;
extern void *sceDmaGetChan(s32);
extern void sceDmaSendN(void *, void *, s32);
extern s32 sceDmaSync(void *, s32, s32);
extern void sdfReleaseMemorySlot(void *);
extern s32 sdfAllocGeneralBlock(s32);
extern s32 sdfResourceRetainAddress(s32);

/* Upload the VIF0 program synchronously, then replace the ring workspace. */
void sdfConsUploadDmaProgram(s32 workspaceBytes) {
    u32 *dmaChannel = sceDmaGetChan(0);
    *dmaChannel &= ~SDF_DMA_CHCR_TTE;
    sceDmaSendN(dmaChannel, D_003200A0, (D_00320630 - D_003200A0) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync(dmaChannel, 0, 0);
    sdfReleaseMemorySlot(&D_003BD350);
    D_003BD350 = sdfAllocGeneralBlock(workspaceBytes);
    D_003BDA20 = sdfResourceRetainAddress(D_003BD350);
}

/* Fixed allocation size; the texture is deliberately unused. */
u32 sdfConsGetTextureDrawPacketSize(SdfTex *texture) {
    return SDF_TEXTURE_DRAW_PACKET_BYTES;
}

/* Texture draw packet: three resource-derived values alternate with their
 * GS register addresses after the GIF tag and payload header. */
typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 reservedWord;
    u32 command;
    u64 gifTag;
    u64 payloadHeader;
    u64 textureWordA;
    u64 registerAddressA;
    u64 textureWordB;
    u64 registerAddressB;
    u64 textureWordC;
    u64 registerAddressC;
} SdfDrawPacket;

extern u64 sdfTexGetPrimaryTextureState(SdfTex *);
extern u64 sdfTexGetPrimarySamplingState(SdfTex *);
extern u64 sdfTexGetPrimaryClampState(SdfTex *);

/* Fill TEX1, TEX0 and CLAMP A+D writes; contextOffset 0/1 selects GS context. */
SdfDrawPacket *sdfConsInitTextureDrawPacket(SdfDrawPacket *drawPacket, SdfTex *texture, s32 contextOffset) {
    drawPacket->quadwords = 4;
    drawPacket->gifTag = 0x1000000000008003ULL;
    drawPacket->command = 0x50000004;
    drawPacket->reservedWord = 0;
    drawPacket->payloadHeader = SDF_GIF_REGISTER_AD;
    drawPacket->textureWordA = sdfTexGetPrimarySamplingState(texture);
    drawPacket->registerAddressA = contextOffset + 0x14;
    drawPacket->textureWordB = sdfTexGetPrimaryTextureState(texture);
    drawPacket->registerAddressB = contextOffset + 6;
    drawPacket->textureWordC = sdfTexGetPrimaryClampState(texture);
    drawPacket->registerAddressC = contextOffset + 8;
    return drawPacket;
}

/* Allocate and append texture state; return its packet address. */
s32 sdfConsCreateDrawPacket(SdfListHead *packetList, SdfTex *texture, s32 contextOffset) {
    s32 packetBytes = sdfConsGetTextureDrawPacketSize(texture);
    SdfDrawPacket *packet = sdfAllocPacketAligned(packetBytes);
    s32 packetAddress = (s32)sdfConsInitTextureDrawPacket(packet, texture, contextOffset);
    sdfAppendPacket(packetList, packetAddress);
    return packetAddress;
}

/* Exclude the two header quadwords from the reference payload count. */
u32 sdfConsFinalizePacketHeader(u32 packetAddress, s32 packetBytes) {
    sdfInitializeDmaReferenceTag(packetAddress, (packetBytes >> SDF_DMA_QWORD_SHIFT) - 2);
    return packetAddress;
}

/* Packed GIF loops contain registerCount values, plus two header quadwords. */
s32 sdfConsCalculateDrawPacketSize(s32 registerCount, s32 loopCount) {
    return (registerCount * loopCount + 2) << SDF_DMA_QWORD_SHIFT;
}

/* Include the DMA/GIF header space in a measured payload byte count. */
s32 sdfConsMeasurePacketWithHeader(s32 payloadBytes) {
    return payloadBytes + 0x20;
}

/* Build packed GIF NLOOP/NREG/PRIM fields and its VIF DIRECT command. */
void *sdfConsInitPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags, s32 registerCount, s64 registerList, s32 loopCount) {
    s32 packetQuadwords = registerCount * loopCount + 1;
    s64 gifTag = loopCount | ((s64)registerCount << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)primitiveFlags << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    packet->gifTag = gifTag;
    packet->command = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    packet->payloadHeader = registerList;
    packet->reservedWord = 0;
    packet->quadwords = packetQuadwords;
    return packet;
}

/* Allocate loopCount sprite loops with RGBAQ, UV, XYZ2, UV, XYZ2 registers. */
void *sdfConsAllocateColumnPacket(s32 loopCount) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, loopCount));
    sdfConsInitPacketHeader(packet, 0x156, 5, 0x53531, loopCount);
    return packet;
}

typedef struct ConsMatrixPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 reservedWord;
    u32 command;
    u8 matrixA[0x40];
    u8 matrixB[0x40];
    u8 vecC[0x10];
    u8 vecD[0x10];
    u8 vecE[0x10];
    u32 stmodCommand;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
} ConsMatrixPacket;


/* Emit two matrices and three vectors; also cache the inverse origin in node. */
void sdfConsBuildMatrixPacket(ConsMatrixPacket *packet, SdfProjectionRecord *node, void *transformMatrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->reservedWord = 0;
    VU0_LOAD_MATRIX(transformMatrix);
    VU0_STORE_MATRIX(packet->matrixA);
    sdfPostmultiplyVuMatrixFromMemory(node->camera.matrix);
    VU0_STORE_MATRIX(packet->matrixB);
    VU0_LOAD_VF_MEMORY(vf10, &node->camera.halfWidth);
    VU0_STORE_VF(vf10, packet->vecC);
    VU0_LOAD_VF_MEMORY(vf10, &node->camera.originX);
    VU0_STORE_VF(vf10, packet->vecD);
    VU0_LOAD_MATRIX(transformMatrix);
    sdfInvertRigidVuTransform();
    VU0_MOVE_VF(vf10, vf31);
    VU0_STORE_VF(vf10, packet->vecE);
    VU0_STORE_VF(vf10, node->inverseOrigin);
    packet->stmodCommand = SDF_VIF_ITOP_MATRIX;
    packet->mscalCommand = SDF_VIF_MSCAL_COMMAND;
    packet->reservedA = 0;
    packet->reservedB = 0;
}


extern u8 D_003983D0[];
extern u8 D_00398470[];
extern u8 D_003984B0[];
extern u8 D_003984F0[];

/* Cache node data, its composed transform, and the transformed cached origin. */
void sdfConsCacheTransformedNode(SdfProjectionRecord *node, void *transformMatrix) {
    VU0_LOAD_MATRIX(transformMatrix);
    VU0_STORE_MATRIX(D_003984B0);
    sdfPostmultiplyVuMatrixFromMemory(node->camera.matrix);
    VU0_LOAD_VF_MEMORY(vf10, node->inverseOrigin);
    VU0_STORE_MATRIX(D_00398470);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_STORE_VF(vf10, D_003984F0);
    memcpy(D_003983D0, node, sizeof(*node));
}


void func_002E1708(u32 arg0) {
    D_003BD374 = arg0;
}

void func_002E1710(u32 arg0) {
    D_003BD378 = arg0;
}


extern s8 D_003BD360;
extern f32 D_003BD364;
extern f32 D_003BD368;
extern f32 D_003BD36C;
extern f32 D_003BD370;
extern f32 func_002FA148(f32);

/* Compose camera projection and screen parameters without reassociating floats. */
void sdfCameraBuildProjection(SdfCamera *camera) {
    f32 projectionMatrix[16];
    f32 farZ = camera->farZ;
    f32 nearZ = camera->nearZ;
    f32 depthRange = farZ - nearZ;
    f32 halfWidth = camera->width * 0.5f;
    f32 halfHeight = camera->height * 0.5f;
    f32 projectionScale;
    f32 centerY;

    EE_MMI_UNIT_MATRIX(projectionMatrix);
    projectionMatrix[0] = 1.0f / (halfWidth * camera->aspect);
    projectionMatrix[5] = 1.0f / halfHeight;
    projectionMatrix[10] = farZ * nearZ * 2.0f / depthRange;
    projectionMatrix[14] = -(nearZ + farZ) / depthRange;
    VU0_LOAD_MATRIX(projectionMatrix);
    EE_MMI_UNIT_MATRIX(projectionMatrix);
    if (camera->flags & SDF_CAMERA_USE_FOV_FLAG) {
        projectionScale = camera->height / (func_002FA148(camera->fov * 0.5f) * 2.0f);
    } else {
        projectionScale = camera->scale;
    }
    projectionMatrix[5] = projectionMatrix[0] = projectionScale;
    projectionMatrix[10] = 0;
    projectionMatrix[15] = 0;
    projectionMatrix[14] = projectionMatrix[11] = 1.0f;
    sdfPremultiplyVuMatrixFromMemory(projectionMatrix);
    VU0_STORE_MATRIX(camera->matrix);
    camera->halfWidth = halfWidth;
    centerY = (camera->bottom - camera->top) * 0.5f;
    if (camera->flags & SDF_CAMERA_HALF_HEIGHT_FLAG) {
        camera->halfHeight = halfHeight * 0.5f;
    } else {
        camera->halfHeight = halfHeight;
    }
    camera->centerY = centerY;
    camera->one = 1.0f;
    camera->originX = camera->offsetX;
    camera->originY = camera->offsetY;
    camera->bottomY = centerY + camera->top;
    camera->zero = 0;
    if (D_003BD360 != 0) {
        camera->halfWidth *= D_003BD364;
        camera->halfHeight *= D_003BD368;
        camera->originX += D_003BD36C;
        camera->originY += D_003BD370;
    }
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1938);

typedef struct ConsFrustumParams {
    f32 left;
    f32 right;
    f32 nearZ;
    f32 farZ;
    s32 mask;
} ConsFrustumParams;

/* DMA/VIF prefix, left/right projection terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct ConsFrustumPacket {
    u64 dmaTag;
    u64 vifUnpackCode;
    f32 right;
    f32 left;
    f32 mid;
    f32 scale;
    u64 gifTag;
    u64 gifRegister;
    u64 registerValue;
    u64 fogColorRegister;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
    u32 reservedC;
} ConsFrustumPacket;

/* Emit projection depth coefficients and a GS FOGCOL A+D write. */
void sdfConsBuildFrustumPacket(ConsFrustumPacket *packet, ConsFrustumParams *projectionParams) {
    f32 rangeMax = projectionParams->right;
    f32 rangeMin = projectionParams->left;
    f32 nearZ = projectionParams->nearZ;
    f32 farZ = projectionParams->farZ;
    f32 depthRange = farZ - nearZ;
    s32 fogColor = projectionParams->mask;
    packet->dmaTag = 0x20000004;
    packet->vifUnpackCode = 0x6C03C00013000000ULL;
    packet->gifTag = 0x1000000000008001ULL;
    packet->gifRegister = SDF_GIF_REGISTER_AD;
    packet->registerValue = (u32)fogColor;
    packet->fogColorRegister = SDF_GS_FOGCOL_REGISTER;
    packet->mscalCommand = 0x14000014;
    packet->reservedC = 0;
    packet->right = rangeMax;
    packet->left = rangeMin;
    packet->reservedA = 0;
    packet->reservedB = 0;
    packet->mid = (((rangeMax - rangeMin) * (farZ + nearZ)) / depthRange + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((farZ * nearZ) * (rangeMin - rangeMax)) / depthRange;
}


typedef struct DmaPacketHeader {
    u16 quadwords;
    u16 pad02;
    u32 address;
    u32 tag;
    u32 command;
    u16 unused10;
    u8 pad12[6];
    u32 unused18;
    u32 unused1C;
} DmaPacketHeader;

/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */
void sdfConsInitDmaPacketHeader(DmaPacketHeader *packet, u32 sourceAddress, s32 payloadBytes) {
    s32 quadwordCount = (payloadBytes + 15) >> SDF_DMA_QWORD_SHIFT;
    packet->quadwords = quadwordCount;
    packet->address = sourceAddress & SDF_DMA_ADDRESS_MASK;
    packet->tag = SDF_VIF_FLUSHA_COMMAND;
    packet->command = quadwordCount | SDF_VIF_DIRECT_COMMAND;
    packet->unused10 = 0;
    packet->unused18 = 0;
    packet->unused1C = 0;
}

extern u8 D_00324350[];
extern void sdfAppendReferencePacket(s32, void *);

/* Append the second fixed program block as a DMA reference packet. */
void sdfConsAppendProgramReferencePacket(s32 packetList, DmaPacketHeader *packet) {
    packet->address = (u32)D_00320630 & SDF_DMA_ADDRESS_MASK;
    packet->quadwords = (D_00324350 - D_00320630) >> SDF_DMA_QWORD_SHIFT;
    packet->tag = 0;
    packet->command = 0;
    packet->unused10 = 0;
    packet->unused18 = 0;
    packet->unused1C = 0;
    sdfAppendReferencePacket(packetList, packet);
}


INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D60);

extern u64 D_003BD388;
extern s32 D_003241D8[];
extern s32 D_00324290[];
extern s32 D_00324214[];
extern u8 D_00317C20[];
extern u8 D_0031BC60[];
extern void sdfInitializeObjectListRequest(void);
extern void sdfRegisterResourceQueueCallbacks(void);

extern void func_002E1D60(void);
extern u64 D_003BD388;
extern s32 D_003241D8[];
extern s32 D_00324290[];
extern s32 D_00324214[];
extern u8 D_00317C20[];
extern u8 D_0031BC60[];
extern void sdfInitializeObjectListRequest(void);
extern void sdfRegisterResourceQueueCallbacks(void);

/* Acquire texture resources, split their TEX0 words, and initialize queues. */
void sdfInitializeResourceQueuesAndTextureWords(void) {
    s64 textureWord;
    func_002E1D60();
    textureWord = sdfTexGetPrimaryTextureState(D_003BD380);
    D_003BD388 = textureWord;
    D_003241D8[0] = textureWord;
    D_003241D8[1] = textureWord >> 32;
    D_003BD37C = sdfTexAcquireAlternateResourceTexture(D_00317C20);
    textureWord = sdfTexGetPrimaryTextureState(D_003BD37C);
    D_00324290[0] = textureWord;
    D_00324290[1] = textureWord >> 32;
    D_003BD390 = sdfTexAcquireAlternateResourceTexture(D_0031BC60);
    textureWord = sdfTexGetPrimaryTextureState(D_003BD390);
    D_00324214[0] = textureWord;
    D_00324214[1] = textureWord >> 32;
    sdfInitializeObjectListRequest();
    sdfRegisterResourceQueueCallbacks();
}


extern u8 D_00398580[];
extern void sdfAppendReferencePacket(s32, void *);

/* Append the fixed clear block; NULL allocatePacket selects the packet allocator. */
void sdfConsAppendClearPacket(s32 packetList, s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfAllocPacketAligned;
    }
    referencePacket = (u64 *)allocatePacket(0x20);
    referencePacket[0] = ((u64)((u32)D_00398580 & SDF_DMA_ADDRESS_MASK) << 32) | 0x30000008;
    referencePacket[1] = 0x6C07C000ULL << 32;
    *(u128 *)&referencePacket[2] = 0;
    sdfAppendReferencePacket(packetList, referencePacket);
}

typedef struct VuLightingPacket {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} VuLightingPacket;

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdfWriteVuLightingPacket(VuLightingPacket *lightingPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(lightingPacket);
    lightingPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    lightingPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    lightingPacket->tag[2] = 0;
    lightingPacket->tag[3] = 0;
}

/* Append inline matrix/lighting data using allocatePacket or the default allocator. */
void sdfConsAppendVuPacket(s32 packetList, s32 (*allocatePacket)(s32)) {
    u64 *dmaPacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfAllocPacketAligned;
    }
    dmaPacket = (u64 *)allocatePacket(0x90);
    dmaPacket[0] = ((u64)((u32)(dmaPacket + 2) & SDF_DMA_ADDRESS_MASK) << 32) | 0x20000008;
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdfWriteVuLightingPacket((VuLightingPacket *)(dmaPacket + 2));
    sdfAppendPacket((SdfListHead *)packetList, (u32)dmaPacket);
}

extern vu8 sdfCurrentBufferIndex;
extern void sdfAssetApplyEntryChanges(void *, s32);
extern void sdfInitNodeHeaderFromWords(void *, void *, s32);
extern void sdfAppendReferencePacket(s32, void *);

/* Apply current-buffer changes and append the asset reference; keep both index reads. */
void sdfConsAppendAssetPacket(s32 packetList, void *asset, s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfAllocPacketAligned;
    }
    sdfAssetApplyEntryChanges(asset, (s8)sdfCurrentBufferIndex);
    referencePacket = (u64 *)allocatePacket(0x20);
    sdfInitNodeHeaderFromWords(asset, referencePacket, (s8)sdfCurrentBufferIndex);
    *(u128 *)&referencePacket[2] = 0;
    sdfAppendReferencePacket(packetList, referencePacket);
}

extern f32 D_003BD358;
extern f32 D_003BD35C;
extern u32 D_00398500[];

/* Serialize the requested VIF attribute streams into one aligned DMA packet. */
void *func_002E21A0(SdfPrimitiveRequest *request) {
    s32 stripWords = request->stripWordCount;
    s32 count = request->vertexCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = request->primitiveFlags | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocate)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (request->normals != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (request->coordinates != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (request->secondCoordinates != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (request->vertexColors != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocate = request->allocate;
    if (allocate == NULL) allocate = sdfAllocPacketAligned;
    packet = allocate(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~request->clipMask) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = request->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)request->depth;
    *cursor++ = (u32)D_003BD358;
    *cursor++ = (u32)D_003BD35C;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (request->strip == NULL) {
        memcpy(cursor, D_00398500, stripWords * 4);
    } else {
        memcpy(cursor, request->strip, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = request->positions;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = request->normals;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = request->coordinates;
    if (coordinates != NULL) {
        secondCoordinates = request->secondCoordinates;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (request->vertexColors != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, request->vertexColors, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while ((u32)++cursor & 15) {
        *cursor = 0;
    }
    return packet;
}



void sdfInitGeometryDmaPacket(u8 *packet, const f32 *matrix) {
    *(u16 *)packet = 2;
    *(u32 *)(packet + 8) = 0x6403C000;
    *(f32 *)(packet + 12) = matrix[0];
    *(f32 *)(packet + 16) = matrix[1];
    *(f32 *)(packet + 20) = matrix[4];
    *(f32 *)(packet + 24) = matrix[5];
    *(f32 *)(packet + 28) = matrix[12];
    *(f32 *)(packet + 32) = matrix[13];
    *(u32 *)(packet + 36) = 0x04000000;
    *(u32 *)(packet + 40) = 0x14000008;
    memset(packet + 0x2C, 0, 12);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2680);

s32 sdfMeasureVertexAttributePacketBytes(s32 count) {
    return count * 0x40 + 0x40;
}

/* Build XYZ and attribute VIF streams; return the packet address.
 * vertexCount must be positive; n retains its native count/byte-size reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 sdfBuildCompactVertexVifPacket(const u128 *positions, const void *byteAttributes, const void *halfAttributes, const void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMeasureVertexAttributePacketBytes(vertexCount);
    if (allocatePacket == NULL) {
        cursor = sdfAllocPacketAligned(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount;
    unpackCode = n << 16;
    vuAddress = vertexCount + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, wordAttributes, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2A00);

u32 sdfMeasureAlignedRecordBufferBytes(s32 count) {
    return (count * 0x54 + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 func_002E2BB8(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMeasureAlignedRecordBufferBytes(count);
    if (alloc == NULL) {
        cursor = sdfAllocPacketAligned(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);
    n = count * 2;
    index = count + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    n = count * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2DE8);

u32 func_002E2F40(s32 count) {
    return (count * 0x4c + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack an indexed-attribute stream into a VIF packet. */
u32 func_002E2F68(const u128 *positions, const void *attributes,
                  const void *halfAttributes, const void *wordAttributes,
                  s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMeasureVertexAttributePacketBytes(count);
    if (alloc == NULL) {
        cursor = sdfAllocPacketAligned(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;

    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);

    code = count << 18;
    index = count + 2;
    n = count * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    index += n;
    n = count * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E31A0);

u32 sdfMeasureAlignedDrawPacketSize(s32 count) {
    return (count * 0x6c + 0x4bU) & 0xfffffff0;
}

/* Build the wider attribute VIF stream; return the packet address.
 * vertexCount must be positive; preserve the native copy lengths and n reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 sdfBuildWideVertexVifPacket(u128 *positions, void *byteAttributes, void *halfAttributes, void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMeasureAlignedDrawPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = sdfAllocPacketAligned(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x15D;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount * 8;
    vuAddress = vertexCount + 2;
    *cursor++ = (n << 16) | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    vuAddress += n;
    unpackCode |= vuAddress;
    *cursor++ = unpackCode | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = SDF_VIF_ITOP_WIDE_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

extern u8 sdfPadActuatorAlignment[];
extern s32 func_002F2280(s32 port, s32 slot);
extern s32 func_002F2200(s32 port, s32 slot, void *data);
extern s32 func_002F2398(s32 port, s32 slot);
extern s32 func_002F2420(s32 port, s32 slot, s32 actNo, s32 term);
extern s32 func_002F2850(s32 port, s32 slot, void *actData);
extern s32 scePadSetMainMode(s32 port, s32 slot, s32 offs, s32 lock);
extern s32 scePadSetActAlign(s32 port, s32 slot, void *data);

/* Drive mode/actuator setup and normalize this pad's latest reply.
 * Motor requests are sent only when their cached values change. */
void sdfPadUpdatePort(F9B00Entry *entry) {
    u8 reply[SDF_PAD_REPLY_BUFFER_BYTES];
    u8 actuatorData[SDF_PAD_ACTUATOR_BYTES];
    s32 port = entry->port;
    s32 slot = entry->slot;
    s32 padState;
    s32 hasButtons;
    s32 hasAnalog;
    s32 hasPressure;
    s32 alignmentStatus;
    s32 requestedMode;
    s32 smallMotor;
    s32 largeMotor;
    reply[0] = -1;
    padState = func_002F2280(port, slot);
    switch (entry->state) {
    case 0:
        if (padState == 2 || padState == 6) {
            entry->lastSmallMotor = -1;
            entry->lastLargeMotor = -1;
            requestedMode = entry->mode = entry->requestedMode;
            switch (requestedMode) {
            case 0:
                entry->state = 3;
                break;
            case 1:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 0) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            case 2:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 0, 3) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            case 3:
                if (padState == 6) {
                    if (scePadSetMainMode(port, slot, 1, 3) == 1) {
                        entry->state = 1;
                    }
                } else {
                    entry->state = 3;
                }
                break;
            }
        }
        break;
    case 1:
        if (padState == 6) {
            entry->state = 3;
        } else if (padState != 5) {
            entry->state = 0;
        }
        break;
    case 2:
        break;
    case 3:
        if (func_002F2420(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadActuatorAlignment) != 0) {
                entry->state = 4;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 4:
        alignmentStatus = func_002F2398(port, slot);
        if (alignmentStatus != 0) {
            if (alignmentStatus == 1) {
                entry->state = 3;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 5:
        if (padState != 2 && padState != 6) {
            entry->state = 0;
        } else if (entry->mode != entry->requestedMode) {
            entry->state = 0;
        } else {
            func_002F2200(port, slot, reply);
            smallMotor = entry->smallMotor;
            largeMotor = entry->largeMotor;
            if (smallMotor != entry->lastSmallMotor || largeMotor != entry->lastLargeMotor) {
                entry->lastSmallMotor = smallMotor;
                entry->lastLargeMotor = largeMotor;
                actuatorData[0] = smallMotor;
                actuatorData[1] = largeMotor;
                func_002F2850(port, slot, actuatorData);
            }
        }
        break;
    }
    /* Pad reply IDs: digital, analog-stick, and pressure-sensitive modes.
     * Missing channels are normalized before consumers see this port. */
    hasAnalog = 0;
    hasPressure = 0;
    hasButtons = 0;
    if (reply[0] == 0) {
        switch (reply[1]) {
        case SDF_PAD_REPLY_DIGITAL:
            hasButtons = 1;
            break;
        case SDF_PAD_REPLY_ANALOG:
            hasButtons = 1;
            hasAnalog = 1;
            break;
        case SDF_PAD_REPLY_PRESSURE:
            hasButtons = 1;
            hasAnalog = 1;
            hasPressure = 1;
            break;
        }
    }
    if (hasButtons) {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
        entry->buttons = ~(reply[3] | (reply[2] << 8));
    } else {
        entry->buttons = 0;
    }
    if (hasAnalog) {
        entry->stick[0] = reply[4];
        entry->stick[1] = reply[5];
        entry->stick[2] = reply[6];
        entry->stick[3] = reply[7];
    } else {
        entry->stick[0] = SDF_PAD_STICK_CENTER;
        entry->stick[1] = SDF_PAD_STICK_CENTER;
        entry->stick[2] = SDF_PAD_STICK_CENTER;
        entry->stick[3] = SDF_PAD_STICK_CENTER;
    }
    if (hasPressure) {
        memcpy(entry->pressure, &reply[8], SDF_PAD_PRESSURE_COUNT);
    } else {
        memset(entry->pressure, 0, SDF_PAD_PRESSURE_COUNT);
    }
}


extern void sdfPadUpdatePort(F9B00Entry *entry);

/* Update each configured logical pad entry. */
void sdfPadUpdatePorts(void) {
    s32 padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadUpdatePort(&sdfPadPorts[padIndex]);
    }
}



extern s32 sdfThreadWakeTick;
extern u16 D_003BD3A0[4];
extern u8 sdfPadAnalogSticks[8];
extern u16 sdfPadButtonMasks[16];
extern u8 sdfPadButtonStates[0x20];
extern u8 sdfPadButtonPressure[0x18];

/* Emit held bit 0, press/repeat trigger bit 1, and new-press bit 7 per button.
 * Changes arm a 15-tick deadline; repeats set it to current tick + lateness + 4,
 * retaining the original extra delay when a deadline is missed. */
void sdfPadBuildButtonStates(void) {
    s32 currentTick = sdfThreadWakeTick;
    s32 padIndex;
    s32 buttonIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        F9B00Entry *entry = &sdfPadPorts[padIndex];
        s32 heldButtons = entry->buttons;
        s32 previousButtons = entry->prevButtons;
        s32 newPressMask;
        s32 triggerMask;
        D_003BD3A0[padIndex] = heldButtons;
        entry->prevButtons = heldButtons;
        newPressMask = (heldButtons ^ previousButtons) & heldButtons;
        if (heldButtons != previousButtons) {
            entry->repeatDeadline = currentTick + SDF_PAD_REPEAT_DELAY;
            triggerMask = newPressMask;
        } else {
            triggerMask = 0;
            if (heldButtons != 0) {
                s32 ticksLate = currentTick - entry->repeatDeadline;
                if (ticksLate >= 0) {
                    triggerMask = heldButtons;
                    entry->repeatDeadline = currentTick + ticksLate + SDF_PAD_REPEAT_STEP;
                }
            }
        }
        for (buttonIndex = 0; buttonIndex != SDF_PAD_BUTTON_COUNT; buttonIndex++) {
            s32 buttonMask = sdfPadButtonMasks[buttonIndex];
            s32 buttonState = (heldButtons & buttonMask) != 0;
            if (triggerMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_TRIGGER_BIT;
            }
            if (newPressMask & buttonMask) {
                buttonState |= SDF_PAD_BUTTON_NEW_PRESS_BIT;
            }
            sdfPadButtonStates[padIndex * SDF_PAD_BUTTON_COUNT + buttonIndex] = buttonState;
        }
        memcpy(&sdfPadAnalogSticks[padIndex * SDF_PAD_STICK_COUNT], entry->stick, SDF_PAD_STICK_COUNT);
        memcpy(&sdfPadButtonPressure[padIndex * SDF_PAD_PRESSURE_COUNT], entry->pressure, SDF_PAD_PRESSURE_COUNT);
    }
}


/* Store a mode request for the indexed logical pad; setup applies it later. */
void sdfPadRequestMode(s32 padIndex, u8 mode) {
    sdfPadPorts[padIndex].requestedMode = mode;
}

/* Store the small-motor request without validating the logical pad index. */
void sdfPadSetSmallMotor(s32 padIndex, u16 strength) {
    sdfPadPorts[padIndex].smallMotor = strength;
}

/* Store the large-motor request without validating the logical pad index. */
void sdfPadSetLargeMotor(s32 padIndex, u8 strength) {
    sdfPadPorts[padIndex].largeMotor = strength;
}

/* Despite the legacy console-prefixed name, set both pad motor low bytes. */
void sdfDevConsSetEntryPair(s32 padIndex, s32 smallMotor, s32 largeMotor) {
    F9B00Entry *entry = &sdfPadPorts[padIndex];
    entry->smallMotor = smallMotor & SDF_PAD_MOTOR_VALUE_MASK;
    sdfPadPorts[padIndex].largeMotor = largeMotor & SDF_PAD_MOTOR_VALUE_MASK;
}

extern u8 sdfPadPortSlotPairs[4];
extern u8 sdfPadPortBuffers[];
extern u8 D_003BD39C;
extern s32 func_002F1C50(s32);
extern s32 scePadPortOpen(s32 port, s32 slot, void *buffer);

/* Open both configured port/slot pairs and reset the public input arrays. */
void sdfPadInit(void) {
    s32 padIndex;

    func_002F1C50(0);
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        s32 port = sdfPadPortSlotPairs[padIndex * 2];
        s32 slot = sdfPadPortSlotPairs[padIndex * 2 + 1];
        F9B00Entry *entry;

        scePadPortOpen(port, slot, &sdfPadPortBuffers[padIndex * SDF_PAD_PORT_BUFFER_BYTES]);
        entry = &sdfPadPorts[padIndex];
        entry->port = port;
        entry->slot = slot;
        entry->state = 0;
        entry->mode = 0;
        entry->requestedMode = 0;
        entry->buttons = 0;
        entry->prevButtons = 0;
        entry->smallMotor = 0;
        entry->largeMotor = 0;
    }
    memset(sdfPadButtonStates, 0, SDF_PAD_BUTTON_STATE_BYTES);
    memset(sdfPadAnalogSticks, SDF_PAD_STICK_CENTER, SDF_PAD_STICK_STATE_BYTES);
    memset(sdfPadButtonPressure, 0, SDF_PAD_PRESSURE_STATE_BYTES);
    D_003BD39C = 0;
}

/* Acquire and cache the shared console texture on first use. */
void sdfDevConsInit(void) {
    if (D_003BD3C4 == 0) {
        D_003BD3C4 = 1;
        D_003BDA34 = sdfTexAcquireResourceTexture(D_00315BA0);
    }
}

/* Ensure initialization and return the cached console texture. */
void *sdfDevConsGetResourceHandle(void) {
    sdfDevConsInit();
    return D_003BDA34;
}

u32 *func_002E3D38(void) {
    return D_00398660;
}

/* Append to the next-linked list and move its stored tail to this node. */
void sdfDevConsListInsert(ConsNode *node) {
    ConsNode *previousTail = D_003BD3C0;

    node->next = NULL;
    node->prev = previousTail;
    if (previousTail != NULL) {
        previousTail->next = node;
    }
    D_003BD3C0 = node;
}

/* Unlink this node, moving the stored tail when its next link is NULL. */
void sdfDevConsListRemove(ConsNode *node) {
    ConsNode *nextNode = node->next;
    ConsNode *previousNode = node->prev;

    if (nextNode != NULL) {
        nextNode->prev = previousNode;
    } else {
        D_003BD3C0 = previousNode;
    }
    if (previousNode != NULL) {
        previousNode->next = nextNode;
    }
}

/* Unlink the console, release its cell-buffer handle, then free the node. */
void sdfDevConsNodeDestroy(ConsNode *node) {
    sdfDevConsListRemove(node);
    sdfReleaseResourceAllocation(node->bufferHandle);
    sdfReleaseChipBlock(node);
}

/* Reset both cursor coordinates and clear the two-byte character-cell grid. */
void sdfDevConsNodeClear(ConsNode *node) {
    node->cursorColumn = 0;
    node->cursorRow = 0;
    memset(node->cells, 0, node->columns * node->rows * SDF_CONSOLE_CELL_BYTES);
}

/* Reset a console through the existing clear operation. */
void sdfDevConsResetNode(ConsNode *node) {
    sdfDevConsNodeClear(node);
}

/* Allocate a two-byte character grid and link it for cleanup.
 * Allocation uses the supplied dimensions; clearing uses their stored s16
 * values. Dimensions and the two opaque halfword inputs are not validated. */
ConsNode *sdfDevConsNodeCreate(u32 first, u32 second, s32 columns, s32 rows) {
    ConsNode *node;
    u32 bufferHandle;

    sdfDevConsInit();
    node = sdfAllocSizeClassBlock(SDF_CONSOLE_NODE_BYTES);
    node->unk8 = first;
    node->unkA = second;
    node->columns = columns;
    node->rows = rows;
    node->unk17 = 8;
    node->controlByte = 0;
    node->textAttribute = 0;
    bufferHandle = sdfAllocGeneralBlock((columns * rows) * SDF_CONSOLE_CELL_BYTES);
    node->bufferHandle = bufferHandle;
    node->cells = (u8 *)sdfResourceRetainAddress(bufferHandle);
    sdfDevConsNodeClear(node);
    sdfDevConsListInsert(node);
    return node;
}

INCLUDE_RODATA(const s32, "game/code_002DDC98", D_003B4548);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD350);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD358);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD35C);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD360);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD364);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD368);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD36C);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD370);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD374);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD378);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD37C);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD380);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD388);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD390);

INCLUDE_SDATA(const s32, "game/code_002DDC98", sdfPadPortSlotPairs);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD39C);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3A0);

INCLUDE_SDATA(const s32, "game/code_002DDC98", sdfPadAnalogSticks);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3A9);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3C0);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3C4);

