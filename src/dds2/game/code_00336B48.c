#include "common.h"
#include "pcp_vu0.h"

extern u32 D_00439188;

extern u32 D_0043918C;

extern u32 D_00438A64;

extern u32 D_00438A68;

extern u32 D_00439194;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
extern u8 D_00476250[];

typedef struct VuBlendNode {
    u8 pad00[0x30];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
} VuBlendNode;

extern f32 D_00439190;

typedef struct F9B00Entry {
    /* 0x00 */ u8 port;
    /* 0x01 */ u8 slot;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 pad03;
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 requestedMode;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u8 pad0A[0x16];
    /* 0x20 */ u16 smallMotor;
    /* 0x22 */ u16 largeMotor;
    /* 0x24 */ u8 pad24[4];
} F9B00Entry;

extern F9B00Entry D_00476480[];

extern void sdfPadUpdatePort(F9B00Entry *);

extern u32 D_00438AB4;

extern u8 D_00370B80[];

extern u32 D_0040B810[];

typedef struct ConsNode {
    /* 0x00 */ struct ConsNode *next;
    /* 0x04 */ struct ConsNode *prev;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ s16 width;
    /* 0x0E */ s16 height;
    /* 0x10 */ u16 cursorColumn;
    /* 0x12 */ u16 cursorRow;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 bufferHandle;
    /* 0x1C */ u8 *pixels;
} ConsNode;

extern ConsNode *D_00438AB0;

extern void *func_00328D68(s32 size);

extern void sdfEnsureFreeRootWorkspace(u32 object);

extern void sdfSetPacketCursorAligned(s32);
extern s32 sdfGetPacketCursor(void);


extern u16 D_00439184;

typedef struct VuAsset {
    u8 pad00[4];
    u32 unk4;              /* 0x04 */
    u32 unk8;              /* 0x08 */
    u32 unkC;              /* 0x0C */
    u8 pad10[0xC];
    f32 unk1C;             /* 0x1C */
    u32 unk20;             /* 0x20 */
    u32 kind;              /* 0x24 */
    f32 unk28;             /* 0x28 */
    f32 unk2C;             /* 0x2C */
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
    s16 unk50;             /* 0x50 */
    u8 pad52[2];
    VuAsset *asset;        /* 0x54 */
    u32 unk58;             /* 0x58 */
    f32 unk5C;             /* 0x5C */
    f32 unk60;             /* 0x60 */
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

extern void func_00339160(s32 arg0);

extern u8 D_0037B080[];

extern u8 D_0037B610[];

extern s32 D_00438A40;

extern s32 D_00439180;

extern void *sceDmaGetChan(s32);

extern void sceDmaSendN(void *, void *, s32);

extern s32 sceDmaSync(void *, s32, s32);

extern void sdfReleaseMemorySlot(void *);

extern s32 func_003292A8(s32);

extern s32 sdfResourceRetainAddress(s32);

typedef struct SdfDrawPacket {
    u16 quadwords;
    u8 pad02[6];
    u32 unk8;
    u32 command;
    u64 unk10;
    u64 unk18;
    u64 unk20;
    u64 unk28;
    u64 unk30;
    u64 unk38;
    u64 unk40;
    u64 unk48;
} SdfDrawPacket;

extern u64 func_0032B318(void *);

extern u64 func_0032B328(void *);

extern u64 func_0032B338(void *);

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

extern void func_0033AEA8(void *);

extern vu8 D_004389DA;

extern void sdfAssetApplyEntryChanges(void *, s32);

extern void sdfInitNodeHeaderFromWords(void *, void *, s32);

/* vu0 routine: vf28-vf31 = vf20-vf23 * vf28-vf31 (4x4 product) */
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
/* vu0 routine: vf28-vf31 = vf28-vf31 * vf20-vf23 (4x4 product) */
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
void func_00336BE8(void *arg0) {
    VU0_LOAD_MATRIX_B(arg0);
    func_00336AA8();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_00336C10(void *arg0) {
    VU0_LOAD_MATRIX_B(arg0);
    func_00336B00();
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
f32 sdfVuDot3(void *left, void *right) {
    f32 dot;
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
        : "=f"(dot) : "r"(left), "r"(right) : "memory");
    return dot;
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
/* Build a VU basis from the normalized target-origin direction and up vector. */
/* vu0 routine: look-at basis in vf28-vf31 (forward, right, up, eye), then its rigid inverse */
void sdfVuBuildLookAtBasis(void *target, void *origin, void *up) {
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
        : : "r"(target), "r"(origin), "r"(up) : "memory");
    func_003363D0();
}

void func_00336D30(void) {
}

void func_00336D38(VuWork *work, s32 n) {
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
    func_00336D38(work, second);
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
/* vu0 routine: rotate the three vectors at arg0 + 0x40 by the 3x3 of the global matrix into vf24-vf26, store them */
void func_00336E60(void *arg0) {
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
        : : "r"(arg0), "r"(m), "r"(D_00476250) : "memory");
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336EC0);

void func_00336F88(void *out, u8 *work, void *reference, f32 deltaX, f32 deltaY) {
    func_00336EC0(out, D_00439188, reference,
                  *(u32 *)(work + 8), *(u32 *)(work + 4),
                  *(f32 *)(work + 0x1c),
                  *(f32 *)(work + 0x2c) + deltaX,
                  *(f32 *)(work + 0x28) + deltaY);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336FC8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337050);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337118);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337170);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003371D0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337200);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337248);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003372B8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337300);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337338);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337408);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003374B0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003375B0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337688);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337718);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337830);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003378C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337970);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003379F0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00337FD8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003385C0);

void func_00338B30(work)
    VuWork *work;
{
    s32 remaining = work->unk50;
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

void func_00338D78(VuWork *work, u64 a, u64 b, u64 c, u64 d, s32 mask) {
    s32 remaining = work->unk50;
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

void func_00339000(work)
    VuWork *work;
{
    s32 remaining = work->unk50;
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

void func_00339160(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x44) & 0x10) != 0) {
        func_00338B30();
        return;
    }
    func_00339000();
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339188);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339270);

void func_00339390(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    func_00339160(temp_v1);
    temp_v0 = *(u32 *)(temp_v1 + 0x44);
    if ((temp_v0 & 0x1000) != 0) {
        func_00339188(arg0);
        temp_v0 = *(u32 *)(temp_v1 + 0x44);
    }
    if ((temp_v0 & 1) != 0) {
        func_00339270(arg0);
        return;
    }
}

extern void func_00337FD8(VuWork *);
extern void func_00337688(u32, s32);
extern void func_003385C0(VuWork *);

void func_003393F0(VuWork *work) {
    u128 saved[3];
    u32 cursor = sdfGetPacketCursor();
    u32 base = (cursor + 0x3F) & ~0x3F;

    work->state = cursor;
    work->header = base + 0x30;
    work->cursor = work->dataStart = (u8 *)(((base + 0x40) & 0x0FFFFFFF) | 0x30000000);
    if (work->selectedFlags & 0x4000) {
        __asm__ volatile (
            ".set noreorder\n"
            "sqc2 vf24, 0x0(%0)\n"
            "sqc2 vf25, 0x10(%0)\n"
            "sqc2 vf26, 0x20(%0)\n"
            ".set reorder"
            : : "r"(saved));
        func_00337FD8(work);
        func_00339390((u32)work);
        func_00336D38(work, work->param1);
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf24, 0x0(%0)\n"
            "lqc2 vf25, 0x10(%0)\n"
            "lqc2 vf26, 0x20(%0)\n"
            ".set reorder"
            : : "r"(saved));
        func_00337688(work->ringSrc, work->param1);
        func_003385C0(work);
        func_00339390((u32)work);
    } else {
        func_00337FD8(work);
        func_00339390((u32)work);
    }
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_003394C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003395A0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003396D0);

void sdfProcessReferencedObjects(u8 **context, u8 *source) {
    u32 *objects = *(u32 **)(*context + 12);
    u16 *indices = (u16 *)(source + 24);
    if ((*(u16 *)(source + 22) & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfEnsureFreeRootWorkspace(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void func_0033A090(u32 arg0) {
    D_00439188 = arg0;
}

extern u8 D_00476240[];

void func_0033A098(u8 *object) {
    if (D_0043918C != (u32)object) {
        D_0043918C = (u32)object;
        PCP_COPY_VECTOR(D_00476240, object + 0x10);
    }
}

void func_0033A0C0(f32 arg0) {
    D_00439190 = arg0;
}

void func_0033A0C8(void) {
    D_00439188 = 0;
    D_0043918C = 0;
}

void sdfConsUploadDmaProgram(s32 size) {
    u32 *chan = sceDmaGetChan(0);
    *chan &= ~0x40;
    sceDmaSendN(chan, D_0037B080, (D_0037B610 - D_0037B080) >> 4);
    sceDmaSync(chan, 0, 0);
    sdfReleaseMemorySlot(&D_00438A40);
    D_00438A40 = func_003292A8(size);
    D_00439180 = sdfResourceRetainAddress(D_00438A40);
}

u32 func_0033A170(s32 tex) {
    return 0x50;
}

SdfDrawPacket *sdfConsInitTextureDrawPacket(SdfDrawPacket *p, void *tex, s32 data) {
    p->quadwords = 4;
    p->unk10 = 0x1000000000008003ULL;
    p->command = 0x50000004;
    p->unk8 = 0;
    p->unk18 = 0xE;
    p->unk20 = func_0032B328(tex);
    p->unk28 = data + 0x14;
    p->unk30 = func_0032B318(tex);
    p->unk38 = data + 6;
    p->unk40 = func_0032B338(tex);
    p->unk48 = data + 8;
    return p;
}

s32 sdfConsCreateDrawPacket(s32 list, s32 tex, s32 data) {
    SdfDrawPacket *packet = sdfConsInitTextureDrawPacket(sdfAllocPacketAligned(func_0033A170(tex)), (void *)tex, data);
    sdfAppendPacket(list, (u32)packet);
    return (s32)packet;
}

u32 func_0033A290(u32 arg0, s32 arg1) {
    func_0032D460(arg0, (arg1 >> 4) - 2);
    return arg0;
}

s32 sdfConsCalculateDrawPacketSize(s32 width, s32 height) {
    return (width * height + 2) << 4;
}

s32 func_0033A2D0(s32 arg0) {
    return arg0 + 0x20;
}

extern void *func_0033A2D8(void *, s32, s32, s64, s32);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A2D8);

void *sdfConsAllocateColumnPacket(s32 height) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, height));
    func_0033A2D8(packet, 0x156, 5, 0x53531, height);
    return packet;
}

typedef struct SdfVuBonePacket {
    u16 quadwords;
    u8 pad02[6];
    u32 unk8;
    u32 command;
    u8 matrixA[0x40]; /* 0x10 */
    u8 matrixB[0x40]; /* 0x50 */
    u8 vecA[0x10];    /* 0x90 */
    u8 vecB[0x10];    /* 0xA0 */
    u8 vecC[0x10];    /* 0xB0 */
    u32 unkC0;
    u32 unkC4;
    u32 unkC8;
    u32 unkCC;
} SdfVuBonePacket;

void func_0033A388(SdfVuBonePacket *packet, u8 *node, void *matrix) {
    packet->quadwords = 0xC;
    packet->command = 0x6C0BC000;
    packet->unk8 = 0;
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(packet->matrixA);
    func_00336C10(node + 0x30);
    VU0_STORE_MATRIX(packet->matrixB);
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(node + 0x70));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(packet->vecA));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(node + 0x80));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(packet->vecB));
    VU0_LOAD_MATRIX(matrix);
    func_003363D0();
    __asm__ volatile(".set noreorder\n\tvmove.xyzw vf10, vf31\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(packet->vecC));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(node + 0x90));
    packet->unkC4 = 0x14000000;
    packet->unkC0 = 0x04000002;
    packet->unkC8 = 0;
    packet->unkCC = 0;
}

typedef struct SdfNodeBlock {
    u32 word[0xA0 / 4];
} SdfNodeBlock;

extern u8 D_0040B660[];
extern u8 D_0040B620[];
extern u8 D_0040B6A0[];
extern u8 D_0040B580[];

void func_0033A480(u8 *node, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_STORE_MATRIX(D_0040B660);
    func_00336C10(node + 0x30);
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(node + 0x90));
    VU0_STORE_MATRIX(D_0040B620);
    __asm__ volatile(
        ".set noreorder\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(D_0040B6A0));
    *(SdfNodeBlock *)D_0040B580 = *(SdfNodeBlock *)node;
}

void func_0033A5B8(u32 arg0) {
    D_00438A64 = arg0;
}

void func_0033A5C0(u32 arg0) {
    D_00438A68 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A5C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A7E8);

typedef struct SdfProjParams {
    f32 rangeMin;
    f32 rangeMax;
    f32 near;
    f32 far;
    u32 count;
} SdfProjParams;

typedef struct SdfProjPacket {
    u64 header;
    u64 vifCode;
    f32 rangeMax;
    f32 rangeMin;
    f32 offset;
    f32 scale;
    u64 giftag;
    u64 giftagReg;
    u64 count;
    u64 unk38;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
} SdfProjPacket;

void func_0033AAA0(SdfProjPacket *packet, SdfProjParams *params) {
    f32 rangeMax = params->rangeMax;
    f32 rangeMin = params->rangeMin;
    f32 near = params->near;
    f32 far = params->far;
    u32 count = params->count;
    packet->header = 0x20000004;
    packet->vifCode = 0x6C03C00013000000ULL;
    packet->rangeMax = rangeMax;
    packet->rangeMin = rangeMin;
    packet->offset = (((rangeMax - rangeMin) * (far + near)) / (far - near) + (rangeMax + rangeMin)) * 0.5f;
    packet->scale = ((far * near) * (rangeMin - rangeMax)) / (far - near);
    packet->giftag = 0x1000000000008001ULL;
    packet->giftagReg = 0xE;
    packet->count = count;
    packet->unk38 = 0x3D;
    packet->unk40 = 0x14000014;
    packet->unk44 = 0;
    packet->unk48 = 0;
    packet->unk4C = 0;
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

void func_0033ABB8(s32 list, DmaPacketHeader *packet) {
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AD68);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AEA8);

void sdfConsAppendVuPacket(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x90);
    packet[0] = ((u64)((u32)(packet + 2) & 0x0FFFFFFF) << 32) | 0x20000008;
    packet[1] = 0x6C07C000ULL << 32;
    func_0033AEA8(packet + 2);
    sdfAppendPacket(list, (u32)packet);
}

void sdfConsAppendAssetPacket(s32 list, void *asset, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    sdfAssetApplyEntryChanges(asset, (s8)D_004389DA);
    packet = (u64 *)alloc(0x20);
    sdfInitNodeHeaderFromWords(asset, packet, (s8)D_004389DA);
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

s32 func_0033B678(s32 arg0) {
    return arg0 * 0x40 + 0x40;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B688);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B8B0);

u32 func_0033BA40(s32 arg0) {
    return (arg0 * 0x54 + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BA68);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BC98);

s32 func_0033BDF0(s32 count) {
    return (count * 0x4C + 0x4B) & ~0xF;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BE18);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C050);

u32 func_0033C218(s32 arg0) {
    return (arg0 * 0x6c + 0x4bU) & 0xfffffff0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C240);

INCLUDE_ASM(const s32, "game/code_00336B48", sdfPadUpdatePort);

void sdfPadUpdatePorts(void) {
    s32 i;
    for (i = 0; i != 2; i++) {
        sdfPadUpdatePort(&D_00476480[i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C878);

void sdfPadRequestMode(s32 padIndex, u8 mode) {
    D_00476480[padIndex].requestedMode = mode;
}

void sdfPadSetSmallMotor(s32 padIndex, u16 strength) {
    D_00476480[padIndex].smallMotor = strength;
}

void sdfPadSetLargeMotor(s32 padIndex, u8 strength) {
    D_00476480[padIndex].largeMotor = strength;
}

void sdfDevConsSetEntryPair(s32 index, s32 small, s32 large) {
    F9B00Entry *entry = &D_00476480[index];
    entry->smallMotor = small & 0xFF;
    D_00476480[index].largeMotor = large & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CA88);

void sdfDevConsInit(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        D_00439194 = func_0032C138(D_00370B80);
    }
}

u32 sdfDevConsGetResourceHandle(void) {
    sdfDevConsInit();
    return D_00439194;
}

u32 *func_0033CBE8(void) {
    return D_0040B810;
}

void sdfDevConsListInsert(ConsNode *node) {
    ConsNode *head = D_00438AB0;

    node->next = NULL;
    node->prev = head;
    if (head != NULL) {
        head->next = node;
    }
    D_00438AB0 = node;
}

void sdfDevConsListRemove(node)
    ConsNode *node;
{
    ConsNode *next = node->next;
    ConsNode *prev = node->prev;
    if (next != NULL) {
        next->prev = prev;
    } else {
        D_00438AB0 = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
}

void sdfDevConsNodeDestroy(ConsNode *node) {
    sdfDevConsListRemove(node);
    func_003297C8(node->bufferHandle);
    func_00328E48(node);
}

void sdfDevConsNodeClear(ConsNode *node) {
    node->cursorColumn = 0;
    node->cursorRow = 0;
    memset(node->pixels, 0, node->width * node->height * 2);
}

void sdfDevConsResetNode(ConsNode *node) {
    sdfDevConsNodeClear(node);
}

ConsNode *sdfDevConsNodeCreate(u32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    ConsNode *node;
    u32 h;

    sdfDevConsInit();
    node = func_00328D68(0x20);
    node->unk8 = arg0;
    node->unkA = arg1;
    node->width = arg2;
    node->height = arg3;
    node->unk17 = 8;
    node->unk14 = 0;
    node->unk16 = 0;
    h = func_003292A8((arg2 * arg3) * 2);
    node->bufferHandle = h;
    node->pixels = (u8 *)sdfResourceRetainAddress(h);
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

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A8C);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A90);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A98);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438A99);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438AB0);

INCLUDE_SDATA(const s32, "game/code_00336B48", D_00438AB4);

