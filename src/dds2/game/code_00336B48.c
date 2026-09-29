#include "common.h"

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
    /* 0x00 */ u8 unk0[5];
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 pad06[0x1A];
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u8 pad24[4];
} F9B00Entry;

extern F9B00Entry D_00476480[];

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
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 bufferHandle;
    /* 0x1C */ u8 *pixels;
} ConsNode;

extern ConsNode *D_00438AB0;

extern void *func_00328D68(s32 size);

extern void func_00330768(u32 object);

extern void func_00336D38(void *, s32);

extern s16 D_00439184;

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

extern void *func_0033A2D8(void *, s64, s64, s64, s32);

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

extern void func_0032CF58(s32, void *);

extern void func_0032CF58(s32, void *);

extern void func_0033AEA8(void *);

extern vu8 D_004389DA;

extern void sdfAssetApplyEntryChanges(void *, s32);

extern void func_00333950(void *, void *, s32);

void func_00336B48(void) {
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
void func_00336BA0(void) {
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
    __asm__ volatile (
        ".set noreorder         \n"
        "lqc2 vf24, 0x0(%0)    \n"
        "lqc2 vf25, 0x10(%0)   \n"
        "lqc2 vf26, 0x20(%0)   \n"
        "lqc2 vf27, 0x30(%0)   \n"
        ".set reorder"
        : : "r"(arg0) : "memory");
    func_00336AA8();
}

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_00336C10(void *arg0) {
    __asm__ volatile (
        ".set noreorder         \n"
        "lqc2 vf24, 0x0(%0)    \n"
        "lqc2 vf25, 0x10(%0)   \n"
        "lqc2 vf26, 0x20(%0)   \n"
        "lqc2 vf27, 0x30(%0)   \n"
        ".set reorder"
        : : "r"(arg0) : "memory");
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
void func_00336CA0(void *arg0, void *arg1, void *arg2) {
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
    func_003363D0();
}

void func_00336D30(void) {
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336D38);

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

void func_00337778(VuBlendNode *node) {
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

void func_003377C8(VuBlendNode *node) {
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00338B30);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00338D78);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339000);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_003393F0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003394C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003395A0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003396D0);

void func_0033A018(u8 **context, u8 *source) {
    u32 *objects = *(u32 **)(*context + 12);
    u16 *indices = (u16 *)(source + 24);
    if ((*(u16 *)(source + 22) & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                func_00330768(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void func_0033A090(u32 arg0) {
    D_00439188 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A098);

void func_0033A0C0(f32 arg0) {
    D_00439190 = arg0;
}

void func_0033A0C8(void) {
    D_00439188 = 0;
    D_0043918C = 0;
}

void func_0033A0D8(s32 size) {
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

SdfDrawPacket *func_0033A178(SdfDrawPacket *p, void *tex, s32 data) {
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
    SdfDrawPacket *packet = func_0033A178(sdfAllocPacketAligned(func_0033A170(tex)), (void *)tex, data);
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A2D8);

void *func_0033A328(s32 height) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, height));
    func_0033A2D8(packet, 0x156, 5, 0x53531, height);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A388);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A480);

void func_0033A5B8(u32 arg0) {
    D_00438A64 = arg0;
}

void func_0033A5C0(u32 arg0) {
    D_00438A68 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A5C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A7E8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AAA0);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033ABB8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AC10);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AD68);

void func_0033AE28(s32 list, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (u64 *)alloc(0x20);
    packet[0] = ((u64)((u32)D_0040B730 & 0x0FFFFFFF) << 32) | 0x30000008;
    packet[1] = 0x6C07C000ULL << 32;
    *(u128 *)&packet[2] = 0;
    func_0032CF58(list, packet);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AEA8);

void func_0033AF38(s32 list, s32 (*alloc)(s32)) {
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

void func_0033AFC0(s32 list, void *asset, s32 (*alloc)(s32)) {
    u64 *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    sdfAssetApplyEntryChanges(asset, (s8)D_004389DA);
    packet = (u64 *)alloc(0x20);
    func_00333950(asset, packet, (s8)D_004389DA);
    *(u128 *)&packet[2] = 0;
    func_0032CF58(list, packet);
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C478);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C820);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033C878);

void func_0033C9F0(s32 arg0, u8 arg1) {
    D_00476480[arg0].unk5 = arg1;
}

void func_0033CA10(s32 arg0, u16 arg1) {
    D_00476480[arg0].unk20 = arg1;
}

void func_0033CA30(s32 arg0, u8 arg1) {
    D_00476480[arg0].unk22 = arg1;
}

void sdfDevConsSetEntryPair(s32 index, s32 arg1, s32 arg2) {
    F9B00Entry *entry = &D_00476480[index];
    entry->unk20 = arg1 & 0xFF;
    D_00476480[index].unk22 = arg2 & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CA88);

void sdfDevConsInit(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        D_00439194 = func_0032C138(D_00370B80);
    }
}

u32 func_0033CBC8(void) {
    sdfDevConsInit();
    return D_00439194;
}

u32 *func_0033CBE8(void) {
    return D_0040B810;
}

void sdfDevConsListInsert(ConsNode *arg0) {
    ConsNode *head = D_00438AB0;

    arg0->next = NULL;
    arg0->prev = head;
    if (head != NULL) {
        head->next = arg0;
    }
    D_00438AB0 = arg0;
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
    node->unk10 = 0;
    node->unk12 = 0;
    memset(node->pixels, 0, node->width * node->height * 2);
}

void func_0033CCB0(ConsNode *arg0) {
    sdfDevConsNodeClear(arg0);
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
