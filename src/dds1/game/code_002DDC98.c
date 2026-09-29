#include "common.h"
#include "pcp_vu0.h"


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
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 bufferHandle;
    /* 0x1C */ u8 *cells;
} ConsNode;

/* Per-port pad state, D_003F9B00[2] (0x28 bytes each). */
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
    /* 0x0C */ u32 repeatTime;
    /* 0x10 */ u8 stick[4];
    /* 0x14 */ u8 pressure[12];
    /* 0x20 */ s16 smallMotor;
    /* 0x22 */ s16 largeMotor;
    /* 0x24 */ s16 lastActuatorX;
    /* 0x26 */ s16 lastActuatorY;
} F9B00Entry;

extern ConsNode *D_003BD3C0;
extern u32 D_003BD3C4;
extern u8 D_00315BA0[];
extern F9B00Entry D_003F9B00[];
extern u32 D_00398660[];
extern u128 D_003F9890;
extern f32 D_003BDA30;
extern void *func_002CFEB8(s32 size);
extern u8 D_003F98A0[];
extern u128 *D_003EB860[][3];
extern void *D_003BD37C;
extern void *D_003BD380;
extern void *D_003BD390;
extern void *func_002D3288(void *);
extern void *func_002D32A0(void *);
extern void *func_002E1428(void *, s64, s64, s64, s32);
extern void sdfEnsureFreeRootWorkspace(u32 object);
extern void *sdfAllocPacketAligned(s32);
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
    u32 unk84;             /* 0x84 */
    u32 *ringStart;        /* 0x88 */
    u32 *cursor;           /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 pad94[0x10];
    void *unkA4;           /* 0xA4 */
} VuWork;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuMultiplyPrimaryByScratch(void) {
    __asm__ volatile (
        ".set noreorder               \n"
        "vmulax.xyzw ACC, vf28, vf20x \n"
        "vmadday.xyzw ACC, vf29, vf20y \n"
        "vmaddaz.xyzw ACC, vf30, vf20z \n"
        "vmaddw.xyzw vf2, vf31, vf20w \n"
        "vmulax.xyzw ACC, vf28, vf21x \n"
        "vmadday.xyzw ACC, vf29, vf21y \n"
        "vmaddaz.xyzw ACC, vf30, vf21z \n"
        "vmaddw.xyzw vf3, vf31, vf21w \n"
        "vmulax.xyzw ACC, vf28, vf22x \n"
        "vmadday.xyzw ACC, vf29, vf22y \n"
        "vmaddaz.xyzw ACC, vf30, vf22z \n"
        "vmaddw.xyzw vf4, vf31, vf22w \n"
        "vmulax.xyzw ACC, vf28, vf23x \n"
        "vmadday.xyzw ACC, vf29, vf23y \n"
        "vmaddaz.xyzw ACC, vf30, vf23z \n"
        "vmaddw.xyzw vf31, vf31, vf23w \n"
        "vmove.xyzw vf28, vf2        \n"
        "vmove.xyzw vf29, vf3        \n"
        "vmove.xyzw vf30, vf4        \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuMultiplyScratchByPrimary(void) {
    __asm__ volatile (
        ".set noreorder               \n"
        "vmulax.xyzw ACC, vf20, vf28x \n"
        "vmadday.xyzw ACC, vf21, vf28y \n"
        "vmaddaz.xyzw ACC, vf22, vf28z \n"
        "vmaddw.xyzw vf28, vf23, vf28w \n"
        "vmulax.xyzw ACC, vf20, vf29x \n"
        "vmadday.xyzw ACC, vf21, vf29y \n"
        "vmaddaz.xyzw ACC, vf22, vf29z \n"
        "vmaddw.xyzw vf29, vf23, vf29w \n"
        "vmulax.xyzw ACC, vf20, vf30x \n"
        "vmadday.xyzw ACC, vf21, vf30y \n"
        "vmaddaz.xyzw ACC, vf22, vf30z \n"
        "vmaddw.xyzw vf30, vf23, vf30w \n"
        "vmulax.xyzw ACC, vf20, vf31x \n"
        "vmadday.xyzw ACC, vf21, vf31y \n"
        "vmaddaz.xyzw ACC, vf22, vf31z \n"
        "vmaddw.xyzw vf31, vf23, vf31w \n"
        ".set reorder"
        :
        :
        : "memory"
    );
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_002DDD38(void *arg0) {
    __asm__ volatile (
        ".set noreorder         \n"
        "lqc2 vf24, 0x0(%0)    \n"
        "lqc2 vf25, 0x10(%0)   \n"
        "lqc2 vf26, 0x20(%0)   \n"
        "lqc2 vf27, 0x30(%0)   \n"
        ".set reorder"
        : : "r"(arg0) : "memory");
    func_002DDBF8();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_002DDD60(void *arg0) {
    __asm__ volatile (
        ".set noreorder         \n"
        "lqc2 vf24, 0x0(%0)    \n"
        "lqc2 vf25, 0x10(%0)   \n"
        "lqc2 vf26, 0x20(%0)   \n"
        "lqc2 vf27, 0x30(%0)   \n"
        ".set reorder"
        : : "r"(arg0) : "memory");
    func_002DDC50();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuTransformVector(void *dst, void *src) {
    __asm__ volatile (
        ".set noreorder               \n"
        "lqc2 vf10, 0x0(%1)           \n"
        "vmulax.xyzw ACC, vf28, vf10x \n"
        "vmadday.xyzw ACC, vf29, vf10y \n"
        "vmaddaz.xyzw ACC, vf30, vf10z \n"
        "vmaddw.xyzw vf10, vf31, vf10w \n"
        "sqc2 vf10, 0x0(%0) \n"
        ".set reorder"
        : : "r"(dst), "r"(src) : "memory");
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
f32 sdfVuDot3(void *arg0, void *arg1) {
    f32 ret;
    __asm__ volatile (
        ".set noreorder           \n"
        "lqc2 vf10, 0x0(%1)       \n"
        "lqc2 vf11, 0x0(%2)       \n"
        "vmul.xyz vf2, vf10, vf11 \n"
        "vaddy.x vf2, vf2, vf2y   \n"
        "vaddz.x vf2, vf2, vf2z   \n"
        "qmfc2.ni $2, vf2        \n"
        "mtc1 $2, %0 \n"
        ".set reorder"
        : "=f"(ret) : "r"(arg0), "r"(arg1) : "memory");
    return ret;
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuCross3(void *dst, void *src1, void *src2) {
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0x0(%1)         \n"
        "lqc2 vf11, 0x0(%2)         \n"
        "vopmula.xyz ACC, vf10, vf11 \n"
        "vopmsub.xyz vf10, vf11, vf10 \n"
        "sqc2 vf10, 0x0(%0) \n"
        ".set reorder"
        : : "r"(dst), "r"(src1), "r"(src2) : "memory");
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void sdfVuBuildLookAtBasis(void *arg0, void *arg1, void *arg2) {
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0x0(%1)         \n"
        "vmove.xyzw vf31, vf10     \n"
        "vmove.xyzw vf11, vf10     \n"
        "lqc2 vf10, 0x0(%0)         \n"
        "vsub.xyzw vf10, vf10, vf11 \n"
        "vmul.xyz vf2, vf10, vf10  \n"
        "vmulax.w ACC, vf0, vf2x   \n"
        "vmadday.w ACC, vf0, vf2y  \n"
        "vmaddz.w vf2, vf0, vf2z   \n"
        "vrsqrt Q, vf0w, vf2w      \n"
        "vwaitq                    \n"
        "vmulq.xyz vf10, vf10, Q   \n"
        "vmove.xyzw vf30, vf10     \n"
        "vmove.xyzw vf11, vf10     \n"
        "lqc2 vf10, 0x0(%2)         \n"
        "vopmula.xyz ACC, vf10, vf11 \n"
        "vopmsub.xyz vf10, vf11, vf10 \n"
        "vmul.xyz vf2, vf10, vf10  \n"
        "vmulax.w ACC, vf0, vf2x   \n"
        "vmadday.w ACC, vf0, vf2y  \n"
        "vmaddz.w vf2, vf0, vf2z   \n"
        "vrsqrt Q, vf0w, vf2w      \n"
        "vwaitq                    \n"
        "vmulq.xyz vf10, vf10, Q   \n"
        "vmove.xyzw vf28, vf10     \n"
        "vmove.xyzw vf11, vf10     \n"
        "vmove.xyzw vf10, vf30     \n"
        "vopmula.xyz ACC, vf10, vf11 \n"
        "vopmsub.xyz vf10, vf11, vf10 \n"
        "vmove.xyzw vf29, vf10 \n"
        ".set reorder"
        : : "r"(arg0), "r"(arg1), "r"(arg2) : "memory");
    func_002DD520();
}

void func_002DDE80(void) {
}

void func_002DDE88(VuWork *work, s32 count) {
    void *end = (u8 *)work + 0x78;
    if (count < 0x80) {
        work->dmaCountA = 0x80 - count;
        work->dmaAddrB = D_003BDA20;
        work->dmaAddrA = count * 96 + 0x70001000;
        work->dmaCountB = 0x2000;
        work->dmaBase = 0x70001000;
    } else if (count == 0x80) {
        work->dmaBase = 0x70001000;
        work->dmaAddrA = D_003BDA20;
        work->dmaCountA = 0x2000;
        work->dmaAddrB = 0;
        work->dmaCountB = 0;
    } else {
        work->dmaCountA = 0x80;
        work->dmaBase = D_003BDA20;
        work->dmaAddrA = 0x70001000;
        work->dmaAddrB = D_003BDA20 + count * 96;
        work->dmaCountB = 0x2000 - count;
    }
    work->dmaEnd = end;
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
    D_003BDA24 = selected & 0x78;
    work->payload = payload;
    work->packetStart = 0;
    func_002DDE88(work, second);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_002DDFB0(void *arg0) {
    void *m = (void *)D_003BDA28;
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
        : : "r"(arg0), "r"(m), "r"(D_003F98A0) : "memory");
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE010);

void func_002DE0D8(void *out, VuTransformWork *work, void *reference, f32 deltaX, f32 deltaY) {
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
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf2, 0x40(%0)\n"
            "lqc2 vf8, 0x30(%1)\n"
            "lqc2 vf9, 0x30(%2)\n"
            "vmulaw.xy ACC, vf8, vf0w\n"
            "vmaddaw.xy ACC, vf9, vf2w\n"
            "vmsubw.xy vf15, vf8, vf2w\n"
            "sqc2 vf15, 0x30(%0)\n"
            ".set reorder\n"
            : : "r"(node), "r"(sourceA), "r"(sourceB) : "memory");
        node = node->next;
    }
}

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

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DE980);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEA18);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEAC0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DEB40);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF128);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DF710);

void func_002DFC80(work)
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


void func_002DFEC8(VuWork *work, u64 a, u64 b, u64 c, u64 d, s32 clearMask) {
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
                *cursor++ = 0x6501C000;
                *cursor++ = 6;
                *cursor++ = ((chunk * 9 + 7) << 16) | 0x6C00C001;
                first = 0;
                *(u64 *)cursor = 0x1000000000000005ULL;
                cursor += 2;
                *(u64 *)cursor = 0xE;
                cursor += 2;
                *(u64 *)cursor = a;
                cursor += 2;
                *(u64 *)cursor = 0x15;
                cursor += 2;
                *(u64 *)cursor = b;
                cursor += 2;
                *(u64 *)cursor = 7;
                cursor += 2;
                *(u64 *)cursor = c;
                cursor += 2;
                *(u64 *)cursor = 9;
                cursor += 2;
                *(u64 *)cursor = 0x51801;
                cursor += 2;
                *(u64 *)cursor = 0x48;
                cursor += 2;
                *(u64 *)cursor = d;
                cursor += 2;
                *(u64 *)cursor = 0x43;
                cursor += 2;
            } else {
                *cursor++ = 0x6501C000;
                *cursor++ = 0;
                *cursor++ = ((chunk * 9 + 1) << 16) | 0x6C00C001;
            }
            *(u64 *)cursor = ((u64)((D_003BDA24 & ~clearMask) | 0x203) << 47) | (chunk * 3) | 0x3000400000008000ULL;
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


void func_002E0150(work)
    VuWork *work;
{
    s32 remaining = work->nodeCount;
    if (remaining != 0) {
        u128 *(*nodes)[3] = D_003EB860;
        u32 *cursor = work->cursor;
        do {
            s32 chunk = (remaining <= 0x18) ? remaining : 0x18;
            remaining -= chunk;
            while (((u32)cursor & 0xC) != 4) {
                *cursor++ = 0;
            }
            *cursor++ = 0x6501C000;
            *cursor++ = 0x10000;
            *cursor++ = ((chunk * 6 + 1) << 16) | 0x6C00C001;
            *(u64 *)cursor = (chunk * 3) | ((u64)(D_003BDA24 | 3) << 47) | 0x2000400000008000ULL;
            cursor += 2;
            *(u64 *)cursor = 0x41;
            cursor += 2;
            do {
                u128 *a = (*nodes)[0];
                u128 *b = (*nodes)[1];
                u128 *c = (*nodes)[2];
                nodes++;
                ((u128 *)cursor)[0] = a[0];
                ((u128 *)cursor)[1] = a[2];
                ((u128 *)cursor)[2] = b[0];
                ((u128 *)cursor)[3] = b[2];
                ((u128 *)cursor)[4] = c[0];
                ((u128 *)cursor)[5] = c[2];
                cursor += 0x18;
            } while (--chunk != 0);
            *cursor++ = 0x14000004;
        } while (remaining != 0);
        work->cursor = cursor;
    }
}


void func_002E02B0(VuWork *work) {
    if ((work->selectedFlags & 0x10) != 0) {
        func_002DFC80();
        return;
    }
    func_002E0150();
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E02D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E03C0);

void func_002E04E0(u32 arg0) {
    u32 flags;
    s32 workAddress;

    workAddress = (s32)arg0;
    func_002E02B0((VuWork *)workAddress);
    flags = ((VuWork *)workAddress)->selectedFlags;
    if ((flags & 0x1000) != 0) {
        func_002E02D8(arg0);
        flags = ((VuWork *)workAddress)->selectedFlags;
    }
    if ((flags & 1) != 0) {
        func_002E03C0(arg0);
        return;
    }
}

extern void func_002DF128(VuWork *work);
extern void func_002DF710(VuWork *work);
extern void func_002DE7D8(u32 base, s32 count);

void func_002E0540(VuWork *work) {
    u32 cursor = sdfGetPacketCursor();
    u32 aligned = (cursor + 0x3F) & ~0x3F;
    u32 ring = ((aligned + 0x40) & 0x0FFFFFFF) | 0x30000000;
    work->packetStart = cursor;
    work->unk84 = aligned + 0x30;
    work->ringStart = (u32 *)ring;
    work->cursor = (u32 *)ring;
    if ((work->selectedFlags & 0x4000) != 0) {
        u8 saved[0x30];
        __asm__ volatile (
            ".set noreorder\n"
            "sqc2 vf24, 0x0(%0)\n"
            "sqc2 vf25, 0x10(%0)\n"
            "sqc2 vf26, 0x20(%0)\n"
            ".set reorder"
            : : "r"(saved));
        func_002DF128(work);
        func_002E04E0((u32)work);
        func_002DDE88(work, work->param1);
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf24, 0x0(%0)\n"
            "lqc2 vf25, 0x10(%0)\n"
            "lqc2 vf26, 0x20(%0)\n"
            ".set reorder"
            : : "r"(saved));
        func_002DE7D8(work->dmaBase, work->param1);
        func_002DF710(work);
        func_002E04E0((u32)work);
    } else {
        func_002DF128(work);
        func_002E04E0((u32)work);
    }
}


INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0618);

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
                sdfEnsureFreeRootWorkspace(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void func_002E11E0(u32 arg0) {
    D_003BDA28 = arg0;
}

void func_002E11E8(u8 *object) {
    if (D_003BDA2C != (u32)object) {
        D_003BDA2C = (u32)object;
        PCP_COPY_VECTOR(&D_003F9890, object + 0x10);
    }
}


void func_002E1210(f32 arg0) {
    D_003BDA30 = arg0;
}

void func_002E1218(void) {
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
extern s32 func_002D03F8(s32);
extern s32 sdfResourceRetainAddress(s32);

void sdfConsUploadDmaProgram(s32 size) {
    u32 *chan = sceDmaGetChan(0);
    *chan &= ~0x40;
    sceDmaSendN(chan, D_003200A0, (D_00320630 - D_003200A0) >> 4);
    sceDmaSync(chan, 0, 0);
    sdfReleaseMemorySlot(&D_003BD350);
    D_003BD350 = func_002D03F8(size);
    D_003BDA20 = sdfResourceRetainAddress(D_003BD350);
}

u32 func_002E12C0(s32 width) {
    return 0x50;
}

typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 unk8;
    u32 command;
    u64 unk10;
    u64 unk18;
    u64 smallMotor;
    u64 unk28;
    u64 unk30;
    u64 unk38;
    u64 unk40;
    u64 unk48;
} SdfDrawPacket;

extern u64 func_002D2468(void *);
extern u64 func_002D2478(void *);
extern u64 func_002D2488(void *);

SdfDrawPacket *sdfConsInitTextureDrawPacket(SdfDrawPacket *p, void *tex, s32 data) {
    p->quadwords = 4;
    p->unk10 = 0x1000000000008003ULL;
    p->command = 0x50000004;
    p->unk8 = 0;
    p->unk18 = 0xE;
    p->smallMotor = func_002D2478(tex);
    p->unk28 = data + 0x14;
    p->unk30 = func_002D2468(tex);
    p->unk38 = data + 6;
    p->unk40 = func_002D2488(tex);
    p->unk48 = data + 8;
    return p;
}

s32 sdfConsCreateDrawPacket(s32 owner, s32 width, s32 height) {
    s32 size = func_002E12C0(width);
    void *packet = (void *)sdfAllocPacketAligned(size);
    s32 result = sdfConsInitTextureDrawPacket(packet, width, height);
    sdfAppendPacket(owner, result);
    return result;
}

u32 func_002E13E0(u32 arg0, s32 arg1) {
    func_002D45B0(arg0, (arg1 >> 4) - 2);
    return arg0;
}

s32 sdfConsCalculateDrawPacketSize(s32 width, s32 height) {
    return (width * height + 2) << 4;
}

s32 func_002E1420(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1428);

void *sdfConsAllocateColumnPacket(s32 height) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, height));
    func_002E1428(packet, 0x156, 5, 0x53531, height);
    return packet;
}

typedef struct ConsMatrixPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 unk8;
    u32 command;
    u8 matrixA[0x40];
    u8 matrixB[0x40];
    u8 vecC[0x10];
    u8 vecD[0x10];
    u8 vecE[0x10];
    u32 word0;
    u32 word4;
    u32 word8;
    u32 wordC;
} ConsMatrixPacket;

#define VU0_LOAD_VF10(src) __asm__ volatile ( \
    ".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(src) : "memory")
#define VU0_STORE_VF10(dst) __asm__ volatile ( \
    ".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(dst) : "memory")

void func_002E14D8(ConsMatrixPacket *packet, u8 *src, void *matrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->unk8 = 0;
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(packet->matrixA);
    func_002DDD60(src + 0x30);
    VU0_STORE_MATRIX(packet->matrixB);
    VU0_LOAD_VF10(src + 0x70);
    VU0_STORE_VF10(packet->vecC);
    VU0_LOAD_VF10(src + 0x80);
    VU0_STORE_VF10(packet->vecD);
    VU0_LOAD_MATRIX(matrix);
    func_002DD520();
    __asm__ volatile (".set noreorder\n\tvmove.xyzw vf10, vf31\n\t.set reorder");
    VU0_STORE_VF10(packet->vecE);
    VU0_STORE_VF10(src + 0x90);
    packet->word0 = 0x04000002;
    packet->word4 = 0x14000000;
    packet->word8 = 0;
    packet->wordC = 0;
}


extern u8 D_003983D0[];
extern u8 D_00398470[];
extern u8 D_003984B0[];
extern u8 D_003984F0[];

void func_002E15D0(u8 *node, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(D_003984B0);
    func_002DDD60(node + 0x30);
    VU0_LOAD_VF10(node + 0x90);
    VU0_STORE_MATRIX(D_00398470);
    __asm__ volatile (
        ".set noreorder\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        ".set reorder");
    VU0_STORE_VF10(D_003984F0);
    memcpy(D_003983D0, node, 0xA0);
}


void func_002E1708(u32 arg0) {
    D_003BD374 = arg0;
}

void func_002E1710(u32 arg0) {
    D_003BD378 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1718);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1938);

typedef struct ConsFrustumParams {
    f32 left;
    f32 right;
    f32 nearZ;
    f32 farZ;
    s32 mask;
} ConsFrustumParams;

typedef struct ConsFrustumPacket {
    u64 unk0;
    u64 unk8;
    f32 right;
    f32 left;
    f32 mid;
    f32 scale;
    u64 smallMotor;
    u64 unk28;
    u64 unk30;
    u64 unk38;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
} ConsFrustumPacket;

void func_002E1BF0(ConsFrustumPacket *packet, ConsFrustumParams *params) {
    f32 right = params->right;
    f32 left = params->left;
    f32 nearZ = params->nearZ;
    f32 farZ = params->farZ;
    f32 range = farZ - nearZ;
    s32 mask = params->mask;
    packet->unk0 = 0x20000004;
    packet->unk8 = 0x6C03C00013000000ULL;
    packet->smallMotor = 0x1000000000008001ULL;
    packet->unk28 = 0xE;
    packet->unk30 = (u32)mask;
    packet->unk38 = 0x3D;
    packet->unk40 = 0x14000014;
    packet->unk4C = 0;
    packet->right = right;
    packet->left = left;
    packet->unk44 = 0;
    packet->unk48 = 0;
    packet->mid = (((right - left) * (farZ + nearZ)) / range + (right + left)) * 0.5f;
    packet->scale = ((farZ * nearZ) * (left - right)) / range;
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

extern u8 D_00324350[];
extern void sdfAppendReferencePacket(s32, void *);

void func_002E1D08(s32 list, DmaPacketHeader *packet) {
    packet->address = (u32)D_00320630 & 0x0FFFFFFF;
    packet->quadwords = (D_00324350 - D_00320630) >> 4;
    packet->tag = 0;
    packet->command = 0;
    packet->unused10 = 0;
    packet->unused18 = 0;
    packet->unused1C = 0;
    sdfAppendReferencePacket(list, packet);
}


INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D60);

extern u64 D_003BD388;
extern s32 D_003241D8[];
extern s32 D_00324290[];
extern s32 D_00324214[];
extern u8 D_00317C20[];
extern u8 D_0031BC60[];
extern void func_002D7810(void);
extern void sdfRegisterResourceQueueCallbacks(void);

extern void func_002E1D60(void);
extern void *D_003BD37C;
extern void *D_003BD380;
extern u64 D_003BD388;
extern void *D_003BD390;
extern s32 D_003241D8[];
extern s32 D_00324290[];
extern s32 D_00324214[];
extern u8 D_00317C20[];
extern u8 D_0031BC60[];
extern void *func_002D32A0(void *);
extern void func_002D7810(void);
extern void sdfRegisterResourceQueueCallbacks(void);

void func_002E1EB8(void) {
    s64 value;
    func_002E1D60();
    value = func_002D2468(D_003BD380);
    D_003BD388 = value;
    D_003241D8[0] = value;
    D_003241D8[1] = value >> 32;
    D_003BD37C = func_002D32A0(D_00317C20);
    value = func_002D2468(D_003BD37C);
    D_00324290[0] = value;
    D_00324290[1] = value >> 32;
    D_003BD390 = func_002D32A0(D_0031BC60);
    value = func_002D2468(D_003BD390);
    D_00324214[0] = value;
    D_00324214[1] = value >> 32;
    func_002D7810();
    sdfRegisterResourceQueueCallbacks();
}


extern u8 D_00398580[];
extern void sdfAppendReferencePacket(s32, void *);

void sdfConsAppendClearPacket(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x20);
    packet[0] = ((u64)((u32)D_00398580 & 0x0FFFFFFF) << 32) | 0x30000008;
    packet[1] = 0x6C07C000ULL << 32;
    *(u128 *)&packet[2] = 0;
    sdfAppendReferencePacket(list, packet);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1FF8);

extern void func_002E1FF8(void *);

void sdfConsAppendVuPacket(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x90);
    packet[0] = ((u64)((u32)(packet + 2) & 0x0FFFFFFF) << 32) | 0x20000008;
    packet[1] = 0x6C07C000ULL << 32;
    func_002E1FF8(packet + 2);
    sdfAppendPacket(list, (u32)packet);
}

extern vu8 D_003BD2EA;
extern void sdfAssetApplyEntryChanges(void *, s32);
extern void sdfInitNodeHeaderFromWords(void *, void *, s32);
extern void sdfAppendReferencePacket(s32, void *);

void sdfConsAppendAssetPacket(s32 list, void *asset, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    sdfAssetApplyEntryChanges(asset, (s8)D_003BD2EA);
    packet = (u64 *)alloc(0x20);
    sdfInitNodeHeaderFromWords(asset, packet, (s8)D_003BD2EA);
    *(u128 *)&packet[2] = 0;
    sdfAppendReferencePacket(list, packet);
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E21A0);

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

s32 func_002E27C8(s32 arg0) {
    return arg0 * 0x40 + 0x40;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E27D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2A00);

u32 func_002E2B90(s32 arg0) {
    return (arg0 * 0x54 + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2BB8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2DE8);

u32 func_002E2F40(s32 count) {
    return (count * 0x4c + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2F68);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E31A0);

u32 func_002E3368(s32 arg0) {
    return (arg0 * 0x6c + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3390);

extern u8 D_00398620[];
extern s32 func_002F2280(s32 port, s32 slot);
extern s32 func_002F2200(s32 port, s32 slot, void *data);
extern s32 func_002F2398(s32 port, s32 slot);
extern s32 func_002F2420(s32 port, s32 slot, s32 actNo, s32 term);
extern s32 func_002F2850(s32 port, s32 slot, void *actData);
extern s32 scePadSetMainMode(s32 port, s32 slot, s32 offs, s32 lock);
extern s32 scePadSetActAlign(s32 port, s32 slot, void *data);

void func_002E35C8(F9B00Entry *entry) {
    u8 data[0x20];
    u8 act[6];
    s32 port = entry->port;
    s32 slot = entry->slot;
    s32 padState;
    s32 hasButtons;
    s32 hasAnalog;
    s32 hasPressure;
    s32 result;
    s32 mode;
    s32 small;
    s32 large;
    data[0] = -1;
    padState = func_002F2280(port, slot);
    switch (entry->state) {
    case 0:
        if (padState == 2 || padState == 6) {
            entry->lastActuatorX = -1;
            entry->lastActuatorY = -1;
            mode = entry->mode = entry->requestedMode;
            switch (mode) {
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
            if (scePadSetActAlign(port, slot, D_00398620) != 0) {
                entry->state = 4;
            }
        } else {
            entry->state = 5;
        }
        break;
    case 4:
        result = func_002F2398(port, slot);
        if (result != 0) {
            if (result == 1) {
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
            func_002F2200(port, slot, data);
            small = entry->smallMotor;
            large = entry->largeMotor;
            if (small != entry->lastActuatorX || large != entry->lastActuatorY) {
                entry->lastActuatorX = small;
                entry->lastActuatorY = large;
                act[0] = small;
                act[1] = large;
                func_002F2850(port, slot, act);
            }
        }
        break;
    }
    hasAnalog = 0;
    hasPressure = 0;
    hasButtons = 0;
    if (data[0] == 0) {
        switch (data[1]) {
        case 0x41:
            hasButtons = 1;
            break;
        case 0x73:
            hasButtons = 1;
            hasAnalog = 1;
            break;
        case 0x79:
            hasButtons = 1;
            hasAnalog = 1;
            hasPressure = 1;
            break;
        }
    }
    if (hasButtons) {
        memset(entry->pressure, 0, 12);
        entry->buttons = ~(data[3] | (data[2] << 8));
    } else {
        entry->buttons = 0;
    }
    if (hasAnalog) {
        entry->stick[0] = data[4];
        entry->stick[1] = data[5];
        entry->stick[2] = data[6];
        entry->stick[3] = data[7];
    } else {
        entry->stick[0] = 0x80;
        entry->stick[1] = 0x80;
        entry->stick[2] = 0x80;
        entry->stick[3] = 0x80;
    }
    if (hasPressure) {
        memcpy(entry->pressure, &data[8], 12);
    } else {
        memset(entry->pressure, 0, 12);
    }
}


extern void func_002E35C8(F9B00Entry *entry);

void func_002E3970(void) {
    s32 i;
    for (i = 0; i != 2; i++) {
        func_002E35C8(&D_003F9B00[i]);
    }
}



extern s32 D_003BD2D8;
extern u16 D_003BD3A0[4];
extern u8 D_003BD3A8[8];
extern u16 D_00398600[16];
extern u8 D_00398628[0x20];
extern u8 D_00398648[0x18];

void func_002E39C8(void) {
    s32 now = D_003BD2D8;
    s32 i;
    s32 bit;
    for (i = 0; i != 2; i++) {
        F9B00Entry *entry = &D_003F9B00[i];
        s32 buttons = entry->buttons;
        s32 prev = entry->prevButtons;
        s32 pressed;
        s32 repeat;
        D_003BD3A0[i] = buttons;
        entry->prevButtons = buttons;
        pressed = (buttons ^ prev) & buttons;
        if (buttons != prev) {
            entry->repeatTime = now + 15;
            repeat = pressed;
        } else {
            repeat = 0;
            if (buttons != 0) {
                s32 late = now - entry->repeatTime;
                if (late >= 0) {
                    repeat = buttons;
                    entry->repeatTime = now + late + 4;
                }
            }
        }
        for (bit = 0; bit != 16; bit++) {
            s32 mask = D_00398600[bit];
            s32 state = (buttons & mask) != 0;
            if (repeat & mask) {
                state |= 2;
            }
            if (pressed & mask) {
                state |= 0x80;
            }
            D_00398628[i * 0x10 + bit] = state;
        }
        memcpy(&D_003BD3A8[i * 4], entry->stick, 4);
        memcpy(&D_00398648[i * 12], entry->pressure, 12);
    }
}


void func_002E3B40(s32 arg0, u8 arg1) {
    D_003F9B00[arg0].requestedMode = arg1;
}

void func_002E3B60(s32 arg0, u16 arg1) {
    D_003F9B00[arg0].smallMotor = arg1;
}

void func_002E3B80(s32 arg0, u8 arg1) {
    D_003F9B00[arg0].largeMotor = arg1;
}

void sdfDevConsSetEntryPair(s32 index, s32 arg1, s32 arg2) {
    F9B00Entry *entry = &D_003F9B00[index];
    entry->smallMotor = arg1 & 0xFF;
    D_003F9B00[index].largeMotor = arg2 & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3BD8);

void sdfDevConsInit(void) {
    if (D_003BD3C4 == 0) {
        D_003BD3C4 = 1;
        D_003BDA34 = func_002D3288(D_00315BA0);
    }
}

void *sdfDevConsGetResourceHandle(void) {
    sdfDevConsInit();
    return D_003BDA34;
}

u32 *func_002E3D38(void) {
    return D_00398660;
}

void sdfDevConsListInsert(ConsNode *node) {
    ConsNode *head = D_003BD3C0;

    node->next = NULL;
    node->prev = head;
    if (head != NULL) {
        head->next = node;
    }
    D_003BD3C0 = node;
}

void sdfDevConsListRemove(ConsNode *node) {
    ConsNode *next = node->next;
    ConsNode *prev = node->prev;

    if (next != NULL) {
        next->prev = prev;
    } else {
        D_003BD3C0 = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
}

void sdfDevConsNodeDestroy(ConsNode *node) {
    sdfDevConsListRemove(node);
    func_002D0918(node->bufferHandle);
    func_002CFF98(node);
}

void sdfDevConsNodeClear(ConsNode *node) {
    node->cursorColumn = 0;
    node->cursorRow = 0;
    memset(node->cells, 0, node->columns * node->rows * 2);
}

void sdfDevConsResetNode(ConsNode *node) {
    sdfDevConsNodeClear(node);
}

ConsNode *sdfDevConsNodeCreate(u32 arg0, u32 arg1, s32 columns, s32 rows) {
    ConsNode *node;
    u32 bufferHandle;

    sdfDevConsInit();
    node = func_002CFEB8(0x20);
    node->unk8 = arg0;
    node->unkA = arg1;
    node->columns = columns;
    node->rows = rows;
    node->unk17 = 8;
    node->unk14 = 0;
    node->unk16 = 0;
    bufferHandle = func_002D03F8((columns * rows) * 2);
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

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD39C);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3A0);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3A8);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3A9);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3C0);

INCLUDE_SDATA(const s32, "game/code_002DDC98", D_003BD3C4);

