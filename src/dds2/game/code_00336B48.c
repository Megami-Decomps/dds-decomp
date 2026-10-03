#include "common.h"
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

extern u32 D_00439188;

extern u32 D_0043918C;

extern u32 D_00438A64;

extern u32 D_00438A68;

extern u32 D_00439194;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
extern u8 D_00476250[];
extern u8 D_00476210[];

typedef struct VuBlendNode {
    u8 pad00[0x30];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
} VuBlendNode;

extern f32 D_00439190;

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

extern F9B00Entry sdfPadPorts[];

extern void sdfPadUpdatePort(F9B00Entry *);

extern u32 D_00438AB4;

extern u8 D_00370B80[];

extern u32 D_0040B810[];

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

extern ConsNode *D_00438AB0;

extern void *sdfAllocSizeClassBlock(s32 size);

extern void sdfEnsureFreeRootWorkspace(u32 object);

extern void sdfSetPacketCursorAligned(s32);
extern s32 sdfGetPacketCursor(void);


extern u16 D_00439184;

typedef struct VuAsset {
    u8 pad00[4];
    u32 param4;            /* 0x04 */
    u32 param8;            /* 0x08 */
    u32 unkC;              /* 0x0C */
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
} VuAsset;

typedef struct {
    u8 pad00[0x40];
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50: geometry references submitted to VU in chunks */
    u8 pad52[2];
    VuAsset *asset;        /* 0x54 */
    u32 unk58;             /* 0x58 */
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
    u8 pad94[0x10];
    u32 unkA4;             /* 0xA4 */
} VuWork;

typedef struct VuGeomRef {
    u128 *a;
    u128 *b;
    u128 *c;
} VuGeomRef;

extern VuGeomRef D_00468210[];

extern void func_00336EC0(void *, u32, void *, u32, u32, f32, f32, f32);

extern void sdfVuEmitSelectedNodePacket(s32 workAddress);

extern u8 D_0037B080[];

extern u8 D_0037B610[];

extern s32 D_00438A40;

extern s32 D_00439180;

extern void *sceDmaGetChan(s32);

extern void sceDmaSendN(void *, void *, s32);

extern s32 sceDmaSync(void *, s32, s32);

extern void sdfReleaseMemorySlot(void *);

extern s32 sdfAllocGeneralBlock(s32);

extern s32 sdfResourceRetainAddress(s32);

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

extern u64 sdfTexGetPrimaryTextureState(void *);

extern u64 sdfTexGetPrimarySamplingState(void *);

extern u64 sdfTexGetPrimaryClampState(void *);

extern void *sdfAllocPacketAligned(s32);

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

extern u8 D_0040B730[];

extern void sdfAppendReferencePacket(s32, void *);

extern void sdfAppendReferencePacket(s32, void *);

extern void func_0033AC10(void);
extern void sdfInitializeObjectListRequest(void);
extern void sdfRegisterResourceQueueCallbacks(void);
extern s32 sdfTexAcquireAlternateResourceTexture(void *);
extern u8 D_00372C00[];
extern u8 D_00376C40[];
extern s32 D_0037F1B8[];
extern s32 D_0037F270[];
extern s32 D_0037F1F4[];
extern void *D_00438A6C;
extern void *D_00438A70;
extern u64 D_00438A78;
extern void *D_00438A80;

extern vu8 sdfCurrentBufferIndex;

extern void sdfAssetApplyEntryChanges(void *, s32);

extern void sdfInitNodeHeaderFromWords(void *, void *, s32);

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

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuTransformVector(void *dst, void *src) {
    VU0_LOAD_VF(vf10, src);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_STORE_VF(vf10, dst);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
f32 sdfVuDot3(void *left, void *right) {
    f32 dot;
    VU0_LOAD_VF(vf10, left);
    VU0_LOAD_VF(vf11, right);
    VU0_DOT_XYZ(dot, vf10, vf11);
    return dot;
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuCross3(void *dst, void *src1, void *src2) {
    VU0_LOAD_VF(vf10, src1);
    VU0_LOAD_VF(vf11, src2);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, dst);
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

void sdfVuConfigureWorkRingDma(VuWork *work, s32 n) {
    u32 end = (u32)work + 0x78;
    if (n < 0x80) {
        work->ringWrapCount = 0x80 - n;
        work->ringWrap = D_00439180;
        work->ringDst = 0x70001000 + n * 0x60;
        work->ringCount = 0x2000;
        work->ringSrc = 0x70001000;
    } else if (n == 0x80) {
        work->ringSrc = 0x70001000;
        work->ringDst = D_00439180;
        work->ringWrapCount = 0x2000;
        work->ringWrap = 0;
        work->ringCount = 0;
    } else {
        work->ringSrc = D_00439180;
        work->ringDst = 0x70001000;
        work->ringWrapCount = 0x80;
        work->ringWrap = D_00439180 + n * 0x60;
        work->ringCount = 0x2000 - n;
    }
    work->ringEnd = end;
}

void sdfInitializeVuWorkParameters(VuWork *work, u16 *params, u32 mask) {
    u8 *payload = (u8 *)(params + 4);
    u16 second;
    u16 flags;
    u16 next;
    u16 selected;
    work->param0 = params[0];
    second = params[1];
    work->param1 = second;
    flags = params[2];
    next = params[3];
    selected = flags & mask;
    work->flags = flags;
    work->nextParam = next;
    work->selectedFlags = selected;
    D_00439184 = selected & 0x78;
    work->payload = payload;
    work->state = 0;
    sdfVuConfigureWorkRingDma(work, second);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: rotate the three vectors at vectors + 0x40 by the 3x3 of the global matrix into vf24-vf26, store them */
void sdfVuRotateObjectBasis(void *vectors) {
    void *m = (void *)D_00439188;
    __asm__ volatile (
        ".set noreorder                \n"
        "lqc2 vf2, 0x40(%0)            \n"
        "lqc2 vf5, 0x10(%1)            \n"
        "lqc2 vf6, 0x20(%1)            \n"
        "lqc2 vf7, 0x30(%1)            \n"
        "lqc2 vf3, 0x50(%0)            \n"
        "lqc2 vf4, 0x60(%0)            \n"
        "vmulax.xyz ACC, vf5, vf2x    \n"
        "vmadday.xyz ACC, vf6, vf2y   \n"
        "vmaddz.xyz vf24, vf7, vf2z   \n"
        "vmulax.xyz ACC, vf5, vf3x    \n"
        "vmadday.xyz ACC, vf6, vf3y   \n"
        "vmaddz.xyz vf25, vf7, vf3z   \n"
        "vmulax.xyz ACC, vf5, vf4x    \n"
        "vmadday.xyz ACC, vf6, vf4y   \n"
        "vmaddz.xyz vf26, vf7, vf4z   \n"
        "sqc2 vf2, 0x0(%2)            \n"
        "sqc2 vf3, 0x10(%2)           \n"
        "sqc2 vf4, 0x20(%2) \n"
        ".set reorder"
        : : "r"(vectors), "r"(m), "r"(D_00476250) : "memory");
}

/* vu0 routine: expand packed colors and build the scaled transform vectors */
void func_00336EC0(void *out, u32 transform, void *reference, u32 packedA,
                   u32 packedB, f32 scale, f32 x, f32 y) {
    __asm__ volatile (
        ".set noreorder\n"
        ".set noat\n"
        "lui $1, 0x3F80\n"
        "mtc1 $1, $f0\n"
        "daddu $9, $4, $0\n"
        "c.lt.s $f13, $f0\n"
        "bc1f 1f\n"
        "daddu $10, $5, $0\n"
        "mov.s $f13, $f0\n"
        "1:\n"
        "mul.s $f1, $f14, $f13\n"
        "lui $1, 0x3C00\n"
        "mtc1 $1, $f0\n"
        "swc1 $f13, 0x0C($9)\n"
        "lui $5, 0x437F\n"
        "mfc1 $11, $f12\n"
        "mfc1 $12, $f0\n"
        "swc1 $f1, 0x04($9)\n"
        "pextlb $2, $0, $7\n"
        "pextlb $3, $0, $8\n"
        "pextlb $4, $0, $6\n"
        "pextlh $2, $0, $2\n"
        "pextlh $3, $0, $3\n"
        "pextlh $4, $0, $4\n"
        "qmtc2.ni $2, vf2\n"
        "qmtc2.ni $3, vf3\n"
        "qmtc2.ni $4, vf4\n"
        "qmtc2.ni $11, vf5\n"
        "qmtc2.ni $12, vf6\n"
        "qmtc2.ni $5, vf7\n"
        "vitof0.xyzw vf2, vf2\n"
        "vitof0.xyzw vf3, vf3\n"
        "vitof0.xyzw vf4, vf4\n"
        "lqc2 vf8, 0x40($10)\n"
        "lqc2 vf9, 0x80($10)\n"
        "vmulx.xyzw vf2, vf2, vf6x\n"
        "vmulx.xyzw vf3, vf3, vf6x\n"
        "vmove.w vf9, vf0\n"
        "vmulx.w vf8, vf8, vf0x\n"
        "vmulw.w vf4, vf4, vf2w\n"
        "vmul.xyz vf9, vf3, vf9\n"
        "vmul.xyz vf8, vf2, vf8\n"
        "vmulaw.xyz ACC, vf9, vf0w\n"
        "vmaddax.xyz ACC, vf8, vf5x\n"
        "vmsubx.xyz vf9, vf9, vf5x\n"
        "vmul.xyzw vf8, vf8, vf4\n"
        "vmul.xyzw vf9, vf9, vf4\n"
        "vminix.xyzw vf8, vf8, vf7x\n"
        "vsub.xyz vf8, vf8, vf9\n"
        "sqc2 vf9, 0x10($9)\n"
        "sqc2 vf8, 0x20($9)\n"
        ".set at\n"
        ".set reorder"
        :
        :
        : "$2", "$3", "$4", "$5", "$9", "$10", "$11", "$12", "memory");
}

void sdfVuTransformWorkAtOffset(void *out, VuAsset *work, void *reference, f32 deltaX, f32 deltaY) {
    func_00336EC0(out, D_00439188, reference,
                  work->param8, work->param4,
                  work->scale,
                  work->x + deltaX,
                  work->y + deltaY);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336FC8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337050);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337118);

/* vu0 routine: transform one vector per 0x60-byte row, then accumulate it by the record W */
void func_00337170(void *rows, s32 count, void *matrix, void *records) {
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0x00(%2)\n"
        "lqc2 vf29, 0x10(%2)\n"
        "lqc2 vf30, 0x20(%2)\n"
        "lqc2 vf31, 0x30(%2)\n"
        "1:\n"
        "ldr $2, 0x00(%3)\n"
        "ldl $2, 0x07(%3)\n"
        "ldr $3, 0x08(%3)\n"
        "ldl $3, 0x0F(%3)\n"
        "pcpyld $2, $3, $2\n"
        "qmtc2 $2, vf2\n"
        "lqc2 vf5, 0x00(%0)\n"
        "vmulax.xyzw ACC, vf28, vf2x\n"
        "vmadday.xyzw ACC, vf29, vf2y\n"
        "vmaddaz.xyzw ACC, vf30, vf2z\n"
        "vmaddw.xyzw vf4, vf31, vf0w\n"
        "addi %3, %3, 0x10\n"
        "addi %1, %1, -1\n"
        "vmulaw.xyzw ACC, vf5, vf0w\n"
        "vmaddw.xyzw vf4, vf4, vf2w\n"
        "addi %0, %0, 0x60\n"
        "bne $0, %1, 1b\n"
        "sqc2 vf4, -0x60(%0)\n"
        ".set reorder"
        : "+r"(rows), "+r"(count), "+r"(matrix), "+r"(records)
        :
        : "$2", "$3", "memory");
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_003371D0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337200);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337248);

/* vu0 routine: add each scaled packed vector to the XYZ components of a 0x60-byte row */
void func_003372B8(void *rows, s32 count, void *vectors, f32 scale) {
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $7, $f12\n"
        "qmtc2 $7, vf6\n"
        "1:\n"
        "lqc2 vf4, 0x00(%0)\n"
        "ldr $2, 0x00(%2)\n"
        "ldl $2, 0x07(%2)\n"
        "lw $3, 0x08(%2)\n"
        "pcpyld $2, $3, $2\n"
        "qmtc2 $2, vf2\n"
        "addi %2, %2, 0x0C\n"
        "vmulaw.xyz ACC, vf4, vf0w\n"
        "vmaddx.xyz vf2, vf2, vf6x\n"
        "addi %0, %0, 0x60\n"
        "addi %1, %1, -1\n"
        "bne $0, %1, 1b\n"
        "sqc2 vf2, -0x60(%0)\n"
        ".set reorder"
        : "+r"(rows), "+r"(count), "+r"(vectors)
        :
        : "$2", "$3", "$7", "memory");
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337300);

/* vu0 routine: transform packed points, derive clip weights, and store clip flags in 0x60-byte rows */
void func_00337338(void *rows, s32 count, void *points, void *weights) {
    void *clipParameters = D_00476210;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf19, 0x00(%4)\n"
        "lqc2 vf20, 0x10(%4)\n"
        "lqc2 vf21, 0x20(%4)\n"
        "vmulx.w vf24, vf19, vf24x\n"
        "vmulx.w vf25, vf19, vf25x\n"
        "vmulx.w vf26, vf19, vf26x\n"
        "1:\n"
        "ldr $2, 0x00(%3)\n"
        "ldl $2, 0x07(%3)\n"
        "lw $3, 0x08(%3)\n"
        "pcpyld $2, $3, $2\n"
        "qmtc2 $2, vf6\n"
        "addi %3, %3, 0x0C\n"
        "ldr $2, 0x00(%2)\n"
        "ldl $2, 0x07(%2)\n"
        "lw $3, 0x08(%2)\n"
        "pcpyld $2, $3, $2\n"
        "qmtc2 $2, vf2\n"
        "addi %2, %2, 0x0C\n"
        "vmulay.w ACC, vf0, vf19y\n"
        "vmaddax.w ACC, vf24, vf6x\n"
        "vmadday.w ACC, vf25, vf6y\n"
        "vmaddz.w vf7, vf26, vf6z\n"
        "vmulax.xyzw ACC, vf28, vf2x\n"
        "vmadday.xyzw ACC, vf29, vf2y\n"
        "vmaddaz.xyzw ACC, vf30, vf2z\n"
        "vmaddw.xyzw vf3, vf31, vf0w\n"
        "vaddaw.w ACC, vf7, vf7w\n"
        "vmsubw.w vf6, vf0, vf0w\n"
        "vmaxx.w vf8, vf7, vf0x\n"
        "vclipw.xyzw vf3, vf3w\n"
        "sqc2 vf3, 0x00(%0)\n"
        "addi %1, %1, -1\n"
        "vaddw.x vf9, vf0, vf6w\n"
        "vminiw.w vf8, vf8, vf0w\n"
        "sqc2 vf6, 0x10(%0)\n"
        "addi %0, %0, 0x60\n"
        "vmulaw.xyzw ACC, vf20, vf0w\n"
        "cfc2.ni $2, $vi18\n"
        "vmaddw.xyzw vf8, vf21, vf8w\n"
        "vclipw.xyzw vf9, vf0w\n"
        "vnop\n"
        "vnop\n"
        "sqc2 vf8, -0x40(%0)\n"
        "sb $2, -0x0F(%0)\n"
        "cfc2.ni $2, $vi18\n"
        "andi $2, $2, 3\n"
        "bne $0, %1, 1b\n"
        "sb $2, -0x10(%0)\n"
        ".set reorder"
        : "+r"(rows), "+r"(count), "+r"(points), "+r"(weights)
        : "r"(clipParameters)
        : "$2", "$3", "memory");
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337408);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003374B0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003375B0);

/* vu0 routine: derive clamped clip weights and low clip flags for 0x60-byte rows */
void func_00337688(u32 rows, s32 count) {
    void *clipParameters = D_00476210;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf19, 0x00(%2)\n"
        "lqc2 vf20, 0x10(%2)\n"
        "lqc2 vf21, 0x20(%2)\n"
        "vmulx.w vf24, vf19, vf24x\n"
        "vmulx.w vf25, vf19, vf25x\n"
        "vmulx.w vf26, vf19, vf26x\n"
        "1:\n"
        "lqc2 vf6, 0x10(%0)\n"
        "vsub.xyz vf6, vf0, vf6\n"
        "vmulay.w ACC, vf0, vf19y\n"
        "vmaddax.w ACC, vf24, vf6x\n"
        "vmadday.w ACC, vf25, vf6y\n"
        "vmaddz.w vf7, vf26, vf6z\n"
        "vaddaw.w ACC, vf7, vf7w\n"
        "vmsubw.w vf6, vf0, vf0w\n"
        "vmaxx.w vf8, vf7, vf0x\n"
        "vaddw.x vf9, vf0, vf6w\n"
        "vminiw.w vf8, vf8, vf0w\n"
        "vclipw.xyzw vf9, vf0w\n"
        "sqc2 vf6, 0x10(%0)\n"
        "vmulaw.xyzw ACC, vf20, vf0w\n"
        "vmaddw.xyzw vf8, vf21, vf8w\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "cfc2 $2, $vi18\n"
        "sqc2 vf8, 0x20(%0)\n"
        "addi %0, %0, 0x60\n"
        "addi %1, %1, -1\n"
        "andi $2, $2, 3\n"
        "bne $0, %1, 1b\n"
        "sb $2, -0x10(%0)\n"
        ".set reorder"
        : "+r"(rows), "+r"(count)
        : "r"(clipParameters)
        : "$2", "memory");
}

/* vu0 routine: transform packed XY pairs through a 2D affine basis into row +0x30 */
void func_00337718(void *rows, s32 count, void *points, void *basis) {
    __asm__ volatile (
        ".set noreorder\n"
        "ld $2, 0x00(%3)\n"
        "qmtc2 $2, vf16\n"
        "ld $3, 0x08(%3)\n"
        "qmtc2 $3, vf17\n"
        "ld $2, 0x10(%3)\n"
        "qmtc2 $2, vf18\n"
        "ld $2, 0x00(%2)\n"
        "qmtc2 $2, vf2\n"
        "vmulaw.xy ACC, vf18, vf0w\n"
        "vmaddax.xy ACC, vf16, vf2x\n"
        "vmaddy.xy vf3, vf17, vf2y\n"
        "1:\n"
        "addi %2, %2, 8\n"
        "ld $2, 0x00(%2)\n"
        "qmtc2 $2, vf2\n"
        "qmfc2 $2, vf3\n"
        "vmulaw.xy ACC, vf18, vf0w\n"
        "vmaddax.xy ACC, vf16, vf2x\n"
        "vmaddy.xy vf3, vf17, vf2y\n"
        "addi %1, %1, -1\n"
        "addi %0, %0, 0x60\n"
        "bne $0, %1, 1b\n"
        "sd $2, -0x30(%0)\n"
        ".set reorder"
        : "+r"(rows), "+r"(count), "+r"(points)
        : "r"(basis)
        : "$2", "$3", "memory");
}

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
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf2, 0x40(%0)\n"
            "lqc2 vf8, 0x30(%1)\n"
            "lqc2 vf9, 0x30(%2)\n"
            "lqc2 vf10, 0x20(%1)\n"
            "lqc2 vf11, 0x20(%2)\n"
            "vmulaw.xy ACC, vf8, vf0w\n"
            "vmaddaw.xy ACC, vf9, vf2w\n"
            "vmsubw.xy vf15, vf8, vf2w\n"
            "vmulaw.xyzw ACC, vf10, vf0w\n"
            "vmaddaw.xyzw ACC, vf11, vf2w\n"
            "vmsubw.xyzw vf16, vf10, vf2w\n"
            "sqc2 vf15, 0x30(%0)\n"
            "sqc2 vf16, 0x20(%0)\n"
            ".set reorder\n"
            : : "r"(node), "r"(sourceA), "r"(sourceB) : "memory");
        node = node->next;
    }
}

/* vu0 routine: apply a clamped weight to the row color vector and linked copies */
void func_00337830(void *work, void *parameters) {
    __asm__ volatile (
        ".set noreorder\n"
        ".set noat\n"
        "lui $1, 0x3F00\n"
        "mtc1 $1, $f0\n"
        "mfc1 $2, $f0\n"
        "vsub.xyzw vf12, vf0, vf0\n"
        "lqc2 vf10, 0x10(%1)\n"
        "lqc2 vf11, 0x20(%1)\n"
        "qmtc2 $2, vf2\n"
        "vaddx.xyzw vf12, vf12, vf2x\n"
        "lw %1, 0x64(%0)\n"
        "lh $2, 0x42(%0)\n"
        "lqc2 vf2, 0x10(%1)\n"
        "vmulaw.w ACC, vf12, vf0w\n"
        "vmaddw.w vf3, vf2, vf12w\n"
        "1:\n"
        "vmaxx.w vf4, vf3, vf0x\n"
        "addi %1, %1, 0x60\n"
        "lqc2 vf2, 0x10(%1)\n"
        "vminiw.w vf5, vf4, vf0w\n"
        "vmulaw.w ACC, vf12, vf0w\n"
        "vmaddw.w vf3, vf2, vf12w\n"
        "vmulaw.xyzw ACC, vf10, vf0w\n"
        "vmaddw.xyzw vf6, vf11, vf5w\n"
        "addi $2, $2, -1\n"
        "bne $0, $2, 1b\n"
        "sqc2 vf6, -0x40(%1)\n"
        "lw %1, 0x78(%0)\n"
        "beqz %1, 3f\n"
        "nop\n"
        "lw $3, 0x44(%1)\n"
        "2:\n"
        "addiu %0, %1, 0x20\n"
        "addiu $3, $3, 0x20\n"
        "lq $2, 0x00($3)\n"
        "sq $2, 0x00(%0)\n"
        "lw %1, 0x40(%1)\n"
        "nop\n"
        "bnel %1, $0, 2b\n"
        "lw $3, 0x44(%1)\n"
        "3:\n"
        ".set at\n"
        ".set reorder"
        : "+r"(work), "+r"(parameters)
        :
        : "$2", "$3", "memory");
}

/* ee/vu0 routine: multiply packed colors and publish the expanded row color */
void func_003378C8(void *work, u32 packedColor) {
    __asm__ volatile (
        ".set noreorder\n"
        ".set noat\n"
        "lui $1, 0x3C00\n"
        "mtc1 $1, $f0\n"
        "daddu $6, %0, $0\n"
        "lw %0, 0x58($6)\n"
        "mfc1 $7, $f0\n"
        "pextlb $2, $0, %0\n"
        "pextlb $3, $0, %1\n"
        "pextlh $2, $0, $2\n"
        "pextlh $3, $0, $3\n"
        "qmtc2 $2, vf2\n"
        "qmtc2 $3, vf3\n"
        "qmtc2 $7, vf4\n"
        "vitof0.xyzw vf2, vf2\n"
        "vitof0.xyzw vf3, vf3\n"
        "vmul.xyzw vf2, vf2, vf3\n"
        "vmulx.xyzw vf2, vf2, vf4x\n"
        "qmfc2 %0, vf2\n"
        "lh $2, 0x42($6)\n"
        "beqz $2, 2f\n"
        "lw %1, 0x64($6)\n"
        "1:\n"
        "addiu $2, $2, -1\n"
        "sq %0, 0x20(%1)\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "bnez $2, 1b\n"
        "addiu %1, %1, 0x60\n"
        "2:\n"
        "lw %1, 0x78($6)\n"
        "beqz %1, 4f\n"
        "nop\n"
        "lw $3, 0x44(%1)\n"
        "nop\n"
        "3:\n"
        "addiu %0, %1, 0x20\n"
        "addiu $3, $3, 0x20\n"
        "lq $2, 0x00($3)\n"
        "sq $2, 0x00(%0)\n"
        "lw %1, 0x40(%1)\n"
        "nop\n"
        "bnel %1, $0, 3b\n"
        "lw $3, 0x44(%1)\n"
        "4:\n"
        ".set at\n"
        ".set reorder"
        : "+r"(work), "+r"(packedColor)
        :
        : "$2", "$3", "$6", "$7", "memory");
}

/* vu0 routine: interpolate two keyframe rows and publish the VU status */
void func_00337970(void *work, void *sourceA, void *sourceB) {
    __asm__ volatile (
        ".set noreorder\n"
        ".set noat\n"
        "sw %1, 0x44(%0)\n"
        "sw %2, 0x48(%0)\n"
        "vaddx.w vf2, vf0, vf0x\n"
        "lbu $2, 0x50(%1)\n"
        "lqc2 vf4, 0x10(%1)\n"
        "lqc2 vf5, 0x10(%2)\n"
        "andi $2, $2, 1\n"
        "bne $0, $2, 1f\n"
        "lqc2 vf6, 0x00(%1)\n"
        "vsubw.w vf2, vf1, vf0w\n"
        "1:\n"
        "lqc2 vf7, 0x00(%2)\n"
        "lqc2 vf8, 0x30(%1)\n"
        "lqc2 vf9, 0x30(%2)\n"
        "vsubw.w vf12, vf2, vf4w\n"
        "vsubw.w vf13, vf5, vf4w\n"
        "vsub.xyzw vf14, vf7, vf6\n"
        "vdiv Q, vf12w, vf13w\n"
        "lqc2 vf10, 0x20(%1)\n"
        "vsub.xyzw vf15, vf9, vf8\n"
        "vmulaw.xyzw ACC, vf6, vf0w\n"
        "sqc2 vf10, 0x20(%0)\n"
        "vwaitq\n"
        "cfc2.ni $2, $vi22\n"
        "vmaddq.xyzw vf14, vf14, Q\n"
        "vmulaw.xyzw ACC, vf8, vf0w\n"
        "vmaddq.xy vf15, vf15, Q\n"
        "sqc2 vf14, 0x00(%0)\n"
        "sw $2, 0x4C(%0)\n"
        "sqc2 vf15, 0x30(%0)\n"
        "sqc2 vf2, 0x10(%0)\n"
        ".set at\n"
        ".set reorder"
        : "+r"(work), "+r"(sourceA), "+r"(sourceB)
        :
        : "$2", "memory");
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_003379F0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337FD8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003385C0);

void sdfVuEmitTexturedTriangleBatches(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    s32 chunk;
    s32 first;
    u32 *out;
    VuGeomRef *ref;
    VuAsset *asset;

    if (remaining != 0) {
        out = (u32 *)work->cursor;
        ref = D_00468210;
        first = 1;
        do {
            chunk = remaining < 0x11 ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)out & 0xC) != 4) {
                *out++ = 0;
            }
            if (first) {
                *out++ = 0x6501C000;
                *out++ = 4;
                *out++ = ((chunk * 9 + 5) << 16) | 0x6C00C001;
                asset = work->asset;
                *(u64 *)out = 0x1000000000000003ULL;
                out += 2;
                *(u64 *)out = 0xE;
                out += 2;
                first = 0;
                *(u64 *)out = asset->unk38;
                out += 2;
                *(u64 *)out = 0x14;
                out += 2;
                *(u64 *)out = asset->unk40;
                out += 2;
                *(u64 *)out = 6;
                out += 2;
                *(u64 *)out = asset->unk48;
                out += 2;
                *(u64 *)out = 8;
                out += 2;
            } else {
                *out++ = 0x6501C000;
                *out++ = 0;
                *out++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)out = ((u64)(D_00439184 | 3) << 47) | (u64)(chunk * 3) | 0x3000400000008000ULL;
            out += 2;
            *(u64 *)out = 0x412;
            out += 2;
            do {
                ((u128 *)out)[0] = ref->a[0];
                ((u128 *)out)[1] = ref->a[3];
                ((u128 *)out)[2] = ref->a[2];
                ((u128 *)out)[3] = ref->b[0];
                ((u128 *)out)[4] = ref->b[3];
                ((u128 *)out)[5] = ref->b[2];
                ((u128 *)out)[6] = ref->c[0];
                ((u128 *)out)[7] = ref->c[3];
                ((u128 *)out)[8] = ref->c[2];
                ref++;
                out += 36;
                chunk--;
            } while (chunk != 0);
            *out++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = (u8 *)out;
    }
}

void sdfBuildChunkedVuNodeTransfer(VuWork *work, u64 a, u64 b, u64 c, u64 d, s32 mask) {
    s32 remaining = work->nodeCount;
    s32 chunk;
    s32 first;
    u32 *out;
    VuGeomRef *ref;

    if (remaining != 0) {
        out = (u32 *)work->cursor;
        ref = D_00468210;
        first = 1;
        do {
            chunk = remaining < 0x11 ? remaining : 0x10;
            remaining -= chunk;
            while (((u32)out & 0xC) != 4) {
                *out++ = 0;
            }
            if (first) {
                *out++ = 0x6501C000;
                *out++ = 6;
                *out++ = ((chunk * 9 + 7) << 16) | 0x6C00C001;
                *(u64 *)out = 0x1000000000000005ULL;
                out += 2;
                *(u64 *)out = 0xE;
                out += 2;
                *(u64 *)out = a;
                out += 2;
                *(u64 *)out = 0x15;
                out += 2;
                *(u64 *)out = b;
                out += 2;
                *(u64 *)out = 7;
                out += 2;
                *(u64 *)out = c;
                out += 2;
                *(u64 *)out = 9;
                out += 2;
                *(u64 *)out = 0x51801;
                out += 2;
                *(u64 *)out = 0x48;
                out += 2;
                *(u64 *)out = d;
                out += 2;
                first = 0;
                *(u64 *)out = 0x43;
                out += 2;
            } else {
                *out++ = 0x6501C000;
                *out++ = 0;
                *out++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)out = ((u64)((D_00439184 & ~mask) | 0x203) << 47) | (u64)(chunk * 3) | 0x3000400000008000ULL;
            out += 2;
            *(u64 *)out = 0x412;
            out += 2;
            do {
                ((u128 *)out)[0] = ref->a[0];
                ((u128 *)out)[1] = ref->a[3];
                ((u128 *)out)[2] = ref->a[2];
                ((u128 *)out)[3] = ref->b[0];
                ((u128 *)out)[4] = ref->b[3];
                ((u128 *)out)[5] = ref->b[2];
                ((u128 *)out)[6] = ref->c[0];
                ((u128 *)out)[7] = ref->c[3];
                ((u128 *)out)[8] = ref->c[2];
                ref++;
                out += 36;
                chunk--;
            } while (chunk != 0);
            *out++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = (u8 *)out;
    }
}

void sdfVuEmitColoredTriangleBatches(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    s32 chunk;
    u32 *out;
    VuGeomRef *ref;

    if (remaining != 0) {
        out = (u32 *)work->cursor;
        ref = D_00468210;
        do {
            chunk = remaining < 0x19 ? remaining : 0x18;
            remaining -= chunk;
            while (((u32)out & 0xC) != 4) {
                *out++ = 0;
            }
            *out++ = 0x6501C000;
            *out++ = 0x10000;
            *out++ = ((chunk * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)out = (u64)(chunk * 3) | ((u64)(D_00439184 | 3) << 47) | 0x2000400000008000ULL;
            out += 2;
            *(u64 *)out = 0x41;
            out += 2;
            do {
                ((u128 *)out)[0] = ref->a[0];
                ((u128 *)out)[1] = ref->a[2];
                ((u128 *)out)[2] = ref->b[0];
                ((u128 *)out)[3] = ref->b[2];
                ((u128 *)out)[4] = ref->c[0];
                ((u128 *)out)[5] = ref->c[2];
                ref++;
                out += 24;
                chunk--;
            } while (chunk != 0);
            *out++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = (u8 *)out;
    }
}

void sdfVuEmitSelectedNodePacket(s32 workAddress) {
    if ((*(u32 *)(workAddress + 0x44) & 0x10) != 0) {
        sdfVuEmitTexturedTriangleBatches();
        return;
    }
    sdfVuEmitColoredTriangleBatches();
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339188);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339270);

void sdfVuApplySelectedWorkFlags(u32 workAddress) {
    u32 flags;
    s32 address;

    address = (s32)workAddress;
    sdfVuEmitSelectedNodePacket(address);
    flags = *(u32 *)(address + 0x44);
    if ((flags & 0x1000) != 0) {
        func_00339188(workAddress);
        flags = *(u32 *)(address + 0x44);
    }
    if ((flags & 1) != 0) {
        func_00339270(workAddress);
        return;
    }
}

extern void func_00337FD8(VuWork *);
extern void func_00337688(u32, s32);
extern void func_003385C0(VuWork *);

void sdfVuBeginPacketFromWork(VuWork *work) {
    u128 saved[3];
    u32 cursor = sdfGetPacketCursor();
    u32 base = (cursor + 0x3F) & ~0x3F;

    work->state = cursor;
    work->header = base + 0x30;
    work->cursor = work->dataStart = (u8 *)(((base + 0x40) & 0x0FFFFFFF) | 0x30000000);
    if (work->selectedFlags & 0x4000) {
        VU0_STORE_VF_UNCLOBBERED(vf24, saved);
        VU0_STORE_VF_AT_UNCLOBBERED(vf25, 16, saved);
        VU0_STORE_VF_AT_UNCLOBBERED(vf26, 32, saved);
        func_00337FD8(work);
        sdfVuApplySelectedWorkFlags((u32)work);
        sdfVuConfigureWorkRingDma(work, work->param1);
        VU0_LOAD_VF(vf24, saved);
        VU0_LOAD_VF_AT(vf25, 16, saved);
        VU0_LOAD_VF_AT(vf26, 32, saved);
        func_00337688(work->ringSrc, work->param1);
        func_003385C0(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    } else {
        func_00337FD8(work);
        sdfVuApplySelectedWorkFlags((u32)work);
    }
}

u64 *func_003394C8(VuWork *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->state == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->dataStart;
    if (cursor == start) {
        sdfSetPacketCursorAligned(work->state);
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_003395A0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003396D0);

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
                sdfEnsureFreeRootWorkspace(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfVuSelectTransformMatrix(u32 matrixAddress) {
    D_00439188 = matrixAddress;
}

extern u8 D_00476240[];

void sdfVuCacheObjectVector(u8 *object) {
    if (D_0043918C != (u32)object) {
        D_0043918C = (u32)object;
        PCP_COPY_VECTOR(D_00476240, object + 0x10);
    }
}

void sdfVuSetGlobalScale(f32 scale) {
    D_00439190 = scale;
}

void sdfVuClearTransformCache(void) {
    D_00439188 = 0;
    D_0043918C = 0;
}

void sdfConsUploadDmaProgram(s32 size) {
    u32 *chan = sceDmaGetChan(0);
    *chan &= ~0x40;
    sceDmaSendN(chan, D_0037B080, (D_0037B610 - D_0037B080) >> 4);
    sceDmaSync(chan, 0, 0);
    sdfReleaseMemorySlot(&D_00438A40);
    D_00438A40 = sdfAllocGeneralBlock(size);
    D_00439180 = sdfResourceRetainAddress(D_00438A40);
}

u32 sdfConsGetTextureDrawPacketSize(s32 tex) {
    return 0x50;
}

SdfDrawPacket *sdfConsInitTextureDrawPacket(SdfDrawPacket *p, void *tex, s32 data) {
    p->quadwords = 4;
    p->gifTag = 0x1000000000008003ULL;
    p->command = 0x50000004;
    p->reservedWord = 0;
    p->payloadHeader = 0xE;
    p->textureWordA = sdfTexGetPrimarySamplingState(tex);
    p->registerAddressA = data + 0x14;
    p->textureWordB = sdfTexGetPrimaryTextureState(tex);
    p->registerAddressB = data + 6;
    p->textureWordC = sdfTexGetPrimaryClampState(tex);
    p->registerAddressC = data + 8;
    return p;
}

s32 sdfConsCreateDrawPacket(s32 list, s32 tex, s32 data) {
    SdfDrawPacket *packet = sdfConsInitTextureDrawPacket(sdfAllocPacketAligned(sdfConsGetTextureDrawPacketSize(tex)), (void *)tex, data);
    sdfAppendPacket(list, (u32)packet);
    return (s32)packet;
}

u32 sdfConsFinalizePacketHeader(u32 packet, s32 size) {
    sdfInitializeDmaReferenceTag(packet, (size >> 4) - 2);
    return packet;
}

s32 sdfConsCalculateDrawPacketSize(s32 width, s32 height) {
    return (width * height + 2) << 4;
}

s32 sdfConsMeasurePacketWithHeader(s32 size) {
    return size + 0x20;
}

void *sdfConsInitPacketHeader(SdfDrawPacket *packet, s32 flags, s32 width, s64 command, s32 height) {
    s32 quadwords = width * height + 1;
    s64 header = height | ((s64)width << 60);

    header |= (s64)flags << 47;
    header |= 0x400000008000LL;
    packet->gifTag = header;
    packet->command = quadwords | 0x50000000;
    packet->payloadHeader = command;
    packet->reservedWord = 0;
    packet->quadwords = quadwords;
    return packet;
}

void *sdfConsAllocateColumnPacket(s32 height) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, height));
    sdfConsInitPacketHeader(packet, 0x156, 5, 0x53531, height);
    return packet;
}

typedef struct SdfVuBonePacket {
    u16 quadwords;
    u8 pad02[6];
    u32 reservedWord;
    u32 command;
    u8 matrixA[0x40]; /* 0x10 */
    u8 matrixB[0x40]; /* 0x50 */
    u8 vecA[0x10];    /* 0x90 */
    u8 vecB[0x10];    /* 0xA0 */
    u8 vecC[0x10];    /* 0xB0 */
    u32 stmodCommand;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
} SdfVuBonePacket;

void sdfConsBuildMatrixPacket(SdfVuBonePacket *packet, u8 *node, void *matrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->reservedWord = 0;
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(packet->matrixA);
    sdfPostmultiplyVuMatrixFromMemory(node + 0x30);
    VU0_STORE_MATRIX(packet->matrixB);
    VU0_LOAD_VF(vf10, node + 0x70);
        VU0_STORE_VF_UNCLOBBERED(vf10, packet->vecA);
    VU0_LOAD_VF(vf10, node + 0x80);
        VU0_STORE_VF_UNCLOBBERED(vf10, packet->vecB);
    VU0_LOAD_MATRIX(matrix);
    sdfInvertRigidVuTransform();
    VU0_MOVE_VF(vf10, vf31);
        VU0_STORE_VF_UNCLOBBERED(vf10, packet->vecC);
        VU0_STORE_VF_UNCLOBBERED(vf10, node + 0x90);
    packet->mscalCommand = 0x14000000;
    packet->stmodCommand = 0x04000002;
    packet->reservedA = 0;
    packet->reservedB = 0;
}

typedef struct SdfNodeBlock {
    u32 word[0xA0 / 4];
} SdfNodeBlock;

extern u8 D_0040B660[];
extern u8 D_0040B620[];
extern u8 D_0040B6A0[];
extern u8 D_0040B580[];

void sdfConsCacheTransformedNode(u8 *node, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(D_0040B660);
    sdfPostmultiplyVuMatrixFromMemory(node + 0x30);
        VU0_LOAD_VF(vf10, node + 0x90);
    VU0_STORE_MATRIX(D_0040B620);
    VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_STORE_VF_UNCLOBBERED(vf10, D_0040B6A0);
    *(SdfNodeBlock *)D_0040B580 = *(SdfNodeBlock *)node;
}

void func_0033A5B8(u32 arg0) {
    D_00438A64 = arg0;
}

void func_0033A5C0(u32 arg0) {
    D_00438A68 = arg0;
}

/* Camera/viewport record; builds the perspective matrix (vf28-vf31 -> matrix) and the screen offsets. */
typedef struct SdfCamera {
    u32 flags;       // 0x00: 1 half-height, 2 field-of-view projection
    f32 aspect;      // 0x04
    f32 scale;       // 0x08
    f32 fov;         // 0x0C
    f32 offsetX;     // 0x10
    f32 offsetY;     // 0x14
    f32 width;       // 0x18
    f32 height;      // 0x1C
    f32 top;         // 0x20
    f32 bottom;      // 0x24
    f32 nearZ;       // 0x28
    f32 farZ;        // 0x2C
    u8 matrix[0x40]; // 0x30
    f32 halfWidth;   // 0x70
    f32 halfHeight;  // 0x74
    f32 centerY;     // 0x78
    f32 one;         // 0x7C
    f32 originX;     // 0x80
    f32 originY;     // 0x84
    f32 bottomY;     // 0x88
    u32 zero;        // 0x8C
} SdfCamera;

extern s8 D_00438A50;
extern f32 D_00438A54;
extern f32 D_00438A58;
extern f32 D_00438A5C;
extern f32 D_00438A60;
extern f32 func_00353228(f32);

void sdfCameraBuildProjection(SdfCamera *cam) {
    f32 m[16];
    f32 farZ = cam->farZ;
    f32 nearZ = cam->nearZ;
    f32 range = farZ - nearZ;
    f32 halfWidth = cam->width * 0.5f;
    f32 halfHeight = cam->height * 0.5f;
    f32 v;
    f32 centerY;

    EE_MMI_UNIT_MATRIX(m);
    m[0] = 1.0f / (halfWidth * cam->aspect);
    m[5] = 1.0f / halfHeight;
    m[10] = farZ * nearZ * 2.0f / range;
    m[14] = -(nearZ + farZ) / range;
    VU0_LOAD_MATRIX(m);
    EE_MMI_UNIT_MATRIX(m);
    if (cam->flags & 2) {
        v = cam->height / (func_00353228(cam->fov * 0.5f) * 2.0f);
    } else {
        v = cam->scale;
    }
    m[5] = m[0] = v;
    m[10] = 0;
    m[15] = 0;
    m[14] = m[11] = 1.0f;
    sdfPremultiplyVuMatrixFromMemory(m);
    VU0_STORE_MATRIX(cam->matrix);
    cam->halfWidth = halfWidth;
    centerY = (cam->bottom - cam->top) * 0.5f;
    if (cam->flags & 1) {
        cam->halfHeight = halfHeight * 0.5f;
    } else {
        cam->halfHeight = halfHeight;
    }
    cam->centerY = centerY;
    cam->one = 1.0f;
    cam->originX = cam->offsetX;
    cam->originY = cam->offsetY;
    cam->bottomY = centerY + cam->top;
    cam->zero = 0;
    if (D_00438A50 != 0) {
        cam->halfWidth *= D_00438A54;
        cam->halfHeight *= D_00438A58;
        cam->originX += D_00438A5C;
        cam->originY += D_00438A60;
    }
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A7E8);

typedef struct SdfProjParams {
    f32 rangeMin;
    f32 rangeMax;
    f32 near;
    f32 far;
    u32 count;
} SdfProjParams;

/* DMA/VIF prefix, projection range terms, then a one-register GIF write
 * and the VU execution command. */
typedef struct SdfProjPacket {
    u64 dmaTag;
    u64 vifUnpackCode;
    f32 rangeMax;
    f32 rangeMin;
    f32 offset;
    f32 scale;
    u64 gifTag;
    u64 gifRegister;
    u64 registerValue;
    u64 fogColorRegister;
    u32 mscalCommand;
    u32 reservedA;
    u32 reservedB;
    u32 reservedC;
} SdfProjPacket;

void sdfConsBuildFrustumPacket(SdfProjPacket *packet, SdfProjParams *params) {
    f32 rangeMax = params->rangeMax;
    f32 rangeMin = params->rangeMin;
    f32 near = params->near;
    f32 far = params->far;
    u32 count = params->count;
    packet->dmaTag = 0x20000004;
    packet->vifUnpackCode = 0x6C03C00013000000ULL;
    packet->rangeMax = rangeMax;
    packet->rangeMin = rangeMin;
    packet->offset = (((rangeMax - rangeMin) * (far + near)) / (far - near) + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((far * near) * (rangeMin - rangeMax)) / (far - near);
    packet->gifTag = 0x1000000000008001ULL;
    packet->gifRegister = 0xE;
    packet->registerValue = count;
    packet->fogColorRegister = 0x3D;
    packet->mscalCommand = 0x14000014;
    packet->reservedA = 0;
    packet->reservedB = 0;
    packet->reservedC = 0;
}

void sdfConsInitDmaPacketHeader(DmaPacketHeader *packet, u32 address, s32 size) {
    s32 qwc = (size + 15) >> 4;
    packet->quadwords = qwc;
    packet->address = address & 0x0FFFFFFF;
    packet->tag = 0x13000000;
    packet->command = qwc | 0x50000000;
    packet->unused10 = 0;
    packet->unused18 = 0;
    packet->unused1C = 0;
}

extern u8 D_0037F330[];

void sdfConsAppendProgramReferencePacket(s32 list, DmaPacketHeader *packet) {
    packet->address = (u32)D_0037B610 & 0x0FFFFFFF;
    packet->quadwords = (D_0037F330 - D_0037B610) >> 4;
    packet->tag = 0;
    packet->command = 0;
    packet->unused10 = 0;
    packet->unused18 = 0;
    packet->unused1C = 0;
    sdfAppendReferencePacket(list, packet);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AC10);

void sdfInitializeResourceQueuesAndTextureWords(void) {
    u64 v;
    func_0033AC10();
    v = sdfTexGetPrimaryTextureState(D_00438A70);
    D_0037F1B8[0] = v;
    D_00438A78 = v;
    D_0037F1B8[1] = v >> 32;
    D_00438A6C = sdfTexAcquireAlternateResourceTexture(D_00372C00);
    v = sdfTexGetPrimaryTextureState(D_00438A6C);
    D_0037F270[0] = v;
    D_0037F270[1] = v >> 32;
    D_00438A80 = sdfTexAcquireAlternateResourceTexture(D_00376C40);
    v = sdfTexGetPrimaryTextureState(D_00438A80);
    D_0037F1F4[0] = v;
    D_0037F1F4[1] = v >> 32;
    sdfInitializeObjectListRequest();
    sdfRegisterResourceQueueCallbacks();
}

void sdfConsAppendClearPacket(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x20);
    packet[0] = ((u64)((u32)D_0040B730 & 0x0FFFFFFF) << 32) | 0x30000008;
    packet[1] = 0x6C07C000ULL << 32;
    *(u128 *)&packet[2] = 0;
    sdfAppendReferencePacket(list, packet);
}

typedef struct VuLightingPacket {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} VuLightingPacket;

/* vu0 routine: store vf28-vf31 and its rows scaled by the inverse column lengths, then the GIF tag words. */
void sdfWriteVuMatrixAndScaledRows(VuLightingPacket *packet) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(packet);
    packet->tag[0] = 0x04000010;
    packet->tag[1] = 0x14000000;
    packet->tag[2] = 0;
    packet->tag[3] = 0;
}

void sdfConsAppendVuPacket(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x90);
    packet[0] = ((u64)((u32)(packet + 2) & 0x0FFFFFFF) << 32) | 0x20000008;
    packet[1] = 0x6C07C000ULL << 32;
    sdfWriteVuMatrixAndScaledRows((VuLightingPacket *)(packet + 2));
    sdfAppendPacket(list, (u32)packet);
}

void sdfConsAppendAssetPacket(s32 list, void *asset, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    sdfAssetApplyEntryChanges(asset, (s8)sdfCurrentBufferIndex);
    packet = (u64 *)alloc(0x20);
    sdfInitNodeHeaderFromWords(asset, packet, (s8)sdfCurrentBufferIndex);
    *(u128 *)&packet[2] = 0;
    sdfAppendReferencePacket(list, packet);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B050);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B530);

s32 sdfMeasureVertexAttributePacketBytes(s32 count) {
    return count * 0x40 + 0x40;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 func_0033B688(const u128 *positions, const void *attributes, const void *halfAttributes, const void *wordAttributes, s32 count, void *(*alloc)(s32)) {
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
    n = count;
    code = n << 16;
    index = count + 2;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    n = count * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6500C000;
    index += n;
    code |= index;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    n *= 8;
    code |= 0x6400C000;
    *cursor++ = code;
    memcpy(cursor, wordAttributes, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = 0x04000002;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B8B0);

u32 sdfMeasureAlignedRecordBufferBytes(s32 count) {
    return (count * 0x54 + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 func_0033BA68(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BC98);

s32 sdfMeasureAlignedRecordStorage(s32 count) {
    return (count * 0x4C + 0x4B) & ~0xF;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BE18);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C050);

u32 sdfMeasureAlignedDrawPacketSize(s32 count) {
    return (count * 0x6c + 0x4bU) & 0xfffffff0;
}

/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 func_0033C240(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMeasureAlignedDrawPacketSize(count);
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
    packet[8] = 0x15D;
    packet[9] = (count << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);
    n = count * 8;
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
    cursor[0] = 0x04000008;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return (u32)packet;
}

extern u8 sdfPadActuatorAlignment[];
extern s32 func_0034B128(s32 port, s32 slot);
extern s32 func_0034B0A8(s32 port, s32 slot, void *data);
extern s32 func_0034B240(s32 port, s32 slot);
extern s32 func_0034B2C8(s32 port, s32 slot, s32 actNo, s32 term);
extern s32 func_0034B6F8(s32 port, s32 slot, void *actData);
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
    padState = func_0034B128(port, slot);
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
        if (func_0034B2C8(port, slot, -1, 0) != 0) {
            if (scePadSetActAlign(port, slot, sdfPadActuatorAlignment) != 0) {
                entry->state = 4;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 4:
        alignmentStatus = func_0034B240(port, slot);
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
            func_0034B0A8(port, slot, reply);
            smallMotor = entry->smallMotor;
            largeMotor = entry->largeMotor;
            if (smallMotor != entry->lastSmallMotor || largeMotor != entry->lastLargeMotor) {
                entry->lastSmallMotor = smallMotor;
                entry->lastLargeMotor = largeMotor;
                actuatorData[0] = smallMotor;
                actuatorData[1] = largeMotor;
                func_0034B6F8(port, slot, actuatorData);
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

/* Update each configured logical pad entry. */
void sdfPadUpdatePorts(void) {
    s32 padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadUpdatePort(&sdfPadPorts[padIndex]);
    }
}

extern s32 sdfThreadWakeTick;
extern u16 D_00438A90[4];
extern u8 sdfPadAnalogSticks[8];
extern u16 D_0040B7B0[16];
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
        D_00438A90[padIndex] = heldButtons;
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
            s32 buttonMask = D_0040B7B0[buttonIndex];
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

extern u8 D_00438A88[4];
extern u8 sdfPadPortBuffers[];
extern u8 D_00438A8C;
extern s32 func_0034AAF8(s32);
extern s32 scePadPortOpen(s32 port, s32 slot, void *buffer);

/* Open both configured port/slot pairs and reset the public input arrays. */
void sdfPadInit(void) {
    s32 padIndex;

    func_0034AAF8(0);
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        s32 port = D_00438A88[padIndex * 2];
        s32 slot = D_00438A88[padIndex * 2 + 1];
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
    D_00438A8C = 0;
}

/* Acquire and cache the shared console texture on first use. */
void sdfDevConsInit(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        D_00439194 = sdfTexAcquireResourceTexture(D_00370B80);
    }
}

/* Ensure initialization and return the cached console texture. */
u32 sdfDevConsGetResourceHandle(void) {
    sdfDevConsInit();
    return D_00439194;
}

u32 *func_0033CBE8(void) {
    return D_0040B810;
}

/* Append to the next-linked list and move its stored tail to this node. */
void sdfDevConsListInsert(ConsNode *node) {
    ConsNode *previousTail = D_00438AB0;

    node->next = NULL;
    node->prev = previousTail;
    if (previousTail != NULL) {
        previousTail->next = node;
    }
    D_00438AB0 = node;
}

/* Unlink this node, moving the stored tail when its next link is NULL. */
void sdfDevConsListRemove(node)
    ConsNode *node;
{
    ConsNode *nextNode = node->next;
    ConsNode *previousNode = node->prev;
    if (nextNode != NULL) {
        nextNode->prev = previousNode;
    } else {
        D_00438AB0 = previousNode;
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

INCLUDE_RODATA(const s32, "game/code_00336B48", D_0042E258);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A40);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A48);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A4C);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A50);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A54);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A58);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A5C);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A60);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A64);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A68);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A6C);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A70);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A78);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A80);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A88);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A8C);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A90);

INCLUDE_SDATA(const s32, "game/code_00336B48", sdfPadAnalogSticks);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A99);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438AB0);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438AB4);
