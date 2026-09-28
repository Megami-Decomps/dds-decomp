#include "common.h"

extern void func_002E02B0(s32 arg0);

extern u32 D_003BDA34;

extern u32 D_003BD378;

extern u32 D_003BD374;

extern u32 D_003BDA2C;

extern u32 D_003BDA28;

typedef struct ConsNode {
    /* 0x00 */ struct ConsNode *next;
    /* 0x04 */ struct ConsNode *prev;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ s16 width;
    /* 0x0E */ s16 height;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 bufferHandle;
    /* 0x1C */ u8 *pixels;
} ConsNode;

typedef struct F9B00Entry {
    /* 0x00 */ u8 unk0[5];
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 pad06[0x1A];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u8 pad24[4];
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
extern void func_002D78B8(u32 object);
extern void *sdfAllocPacketAligned(s32);
extern void *func_002E1428(void *, s64, s64, s64, s32);
extern void func_002DE010(void *, u32, void *, u32, u32, f32, f32, f32);
extern void func_002DDE88(void *, s32);
extern s16 D_003BDA24;

typedef struct VuBlendNode {
    u8 pad00[0x30];
    u8 result[0x10];
    struct VuBlendNode *next;
    void *sourceA;
    void *sourceB;
} VuBlendNode;

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_002DDC98(void) {
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
void func_002DDCF0(void) {
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
void func_002DDDF0(void *arg0, void *arg1, void *arg2) {
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

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DDE88);

typedef struct {
    u8 pad00[0x40];
    u16 param0;            /* 0x40 */
    u16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    u8 pad50[0x30];
    u32 state;             /* 0x80 */
    u8 pad84[0x0C];
    u8 *payload;           /* 0x90 */
} VuWork;

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
    work->state = 0;
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

void func_002DE0D8(void *out, u8 *work, void *reference, f32 deltaX, f32 deltaY) {
    func_002DE010(out, D_003BDA28, reference,
                  *(u32 *)(work + 8), *(u32 *)(work + 4),
                  *(f32 *)(work + 0x1c),
                  *(f32 *)(work + 0x2c) + deltaX,
                  *(f32 *)(work + 0x28) + deltaY);
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

void func_002DE8C8(VuBlendNode *node) {
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

void func_002DE918(VuBlendNode *node) {
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

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DFC80);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002DFEC8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0150);

void func_002E02B0(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x44) & 0x10) != 0) {
        func_002DFC80();
        return;
    }
    func_002E0150();
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E02D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E03C0);

void func_002E04E0(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    func_002E02B0(temp_v1);
    temp_v0 = *(u32 *)(temp_v1 + 0x44);
    if ((temp_v0 & 0x1000) != 0) {
        func_002E02D8(arg0);
        temp_v0 = *(u32 *)(temp_v1 + 0x44);
    }
    if ((temp_v0 & 1) != 0) {
        func_002E03C0(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0540);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0618);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E06F0);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E0820);

void func_002E1168(u8 **context, u8 *source) {
    u32 *objects = *(u32 **)(*context + 12);
    u16 *indices = (u16 *)(source + 24);
    if ((*(u16 *)(source + 22) & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                func_002D78B8(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void func_002E11E0(u32 arg0) {
    D_003BDA28 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E11E8);

void func_002E1210(f32 arg0) {
    D_003BDA30 = arg0;
}

void func_002E1218(void) {
    D_003BDA28 = 0;
    D_003BDA2C = 0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1228);

u32 func_002E12C0(s32 width) {
    return 0x50;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E12C8);

s32 sdfConsCreateDrawPacket(s32 owner, s32 width, s32 height) {
    s32 size = func_002E12C0(width);
    void *packet = (void *)sdfAllocPacketAligned(size);
    s32 result = func_002E12C8(packet, width, height);
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

void *func_002E1478(s32 height) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, height));
    func_002E1428(packet, 0x156, 5, 0x53531, height);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E14D8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E15D0);

void func_002E1708(u32 arg0) {
    D_003BD374 = arg0;
}

void func_002E1710(u32 arg0) {
    D_003BD378 = arg0;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1718);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1938);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1BF0);

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

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D08);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1D60);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1EB8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1F78);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E1FF8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2088);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E2110);

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

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E35C8);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3970);

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E39C8);

void func_002E3B40(s32 arg0, u8 arg1) {
    D_003F9B00[arg0].unk5 = arg1;
}

void func_002E3B60(s32 arg0, u16 arg1) {
    D_003F9B00[arg0].unk20 = arg1;
}

void func_002E3B80(s32 arg0, u8 arg1) {
    D_003F9B00[arg0].unk22 = arg1;
}

void sdfDevConsSetEntryPair(s32 index, s32 arg1, s32 arg2) {
    F9B00Entry *entry = &D_003F9B00[index];
    entry->unk20 = arg1 & 0xFF;
    D_003F9B00[index].unk22 = arg2 & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_002DDC98", func_002E3BD8);

void sdfDevConsInit(void) {
    if (D_003BD3C4 == 0) {
        D_003BD3C4 = 1;
        D_003BDA34 = func_002D3288(D_00315BA0);
    }
}

u32 func_002E3D18(void) {
    sdfDevConsInit();
    return D_003BDA34;
}

u32 *func_002E3D38(void) {
    return D_00398660;
}

void sdfDevConsListInsert(ConsNode *arg0) {
    ConsNode *head = D_003BD3C0;

    arg0->next = NULL;
    arg0->prev = head;
    if (head != NULL) {
        head->next = arg0;
    }
    D_003BD3C0 = arg0;
}

void sdfDevConsListRemove(ConsNode *arg0) {
    ConsNode *next = arg0->next;
    ConsNode *prev = arg0->prev;

    if (next != NULL) {
        next->prev = prev;
    } else {
        D_003BD3C0 = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
}

void sdfDevConsNodeDestroy(ConsNode *arg0) {
    sdfDevConsListRemove(arg0);
    func_002D0918(arg0->bufferHandle);
    func_002CFF98(arg0);
}

void sdfDevConsNodeClear(ConsNode *arg0) {
    arg0->unk10 = 0;
    arg0->unk12 = 0;
    memset(arg0->pixels, 0, arg0->width * arg0->height * 2);
}

void func_002E3E00(ConsNode *arg0) {
    sdfDevConsNodeClear(arg0);
}

ConsNode *sdfDevConsNodeCreate(u32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    ConsNode *node;
    u32 h;

    sdfDevConsInit();
    node = func_002CFEB8(0x20);
    node->unk8 = arg0;
    node->unkA = arg1;
    node->width = arg2;
    node->height = arg3;
    node->unk17 = 8;
    node->unk14 = 0;
    node->unk16 = 0;
    h = func_002D03F8((arg2 * arg3) * 2);
    node->bufferHandle = h;
    node->pixels = (u8 *)sdfResourceRetainAddress(h);
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

