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
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 unk12;
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ u8 *unk1C;
} ConsNode;

extern ConsNode *D_00438AB0;

extern void *func_00328D68(s32 size);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336BE8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336C10);

/* VU0 macro math via inline asm (plain C cannot emit COP2 macro insns) */
void func_00336C38(void *dst, void *src) {
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
f32 func_00336C58(void *arg0, void *arg1) {
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
void func_00336C80(void *dst, void *src1, void *src2) {
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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336CA0);

void func_00336D30(void) {
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336D38);

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336E08);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00336F88);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_00339390);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003393F0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003394C8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003395A0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_003396D0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A018);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A0D8);

u32 func_0033A170(void) {
    return 0x50;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A178);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A220);

u32 func_0033A290(u32 arg0, s32 arg1) {
    func_0032D460(arg0, (arg1 >> 4) - 2);
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A2C0);

s32 func_0033A2D0(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A2D8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033A328);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AB78);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033ABB8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AC10);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AD68);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AE28);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AEA8);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AF38);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033AFC0);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B050);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033B4B0);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033BDF0);

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

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CA58);

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CA88);

void func_0033CB90(void) {
    if (D_00438AB4 == 0) {
        D_00438AB4 = 1;
        D_00439194 = func_0032C138(D_00370B80);
    }
}

u32 func_0033CBC8(void) {
    func_0033CB90();
    return D_00439194;
}

u32 *func_0033CBE8(void) {
    return D_0040B810;
}

void func_0033CBF8(ConsNode *arg0) {
    ConsNode *head = D_00438AB0;

    arg0->next = NULL;
    arg0->prev = head;
    if (head != NULL) {
        head->next = arg0;
    }
    D_00438AB0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CC18);

void func_0033CC48(u32 arg0) {
    func_0033CC18();
    func_003297C8(*(u32 *)((s32)arg0 + 0x18));
    func_00328E48(arg0);
}

void func_0033CC78(s32 arg0) {
    *(u16 *)(arg0 + 0x10) = 0;
    *(u16 *)(arg0 + 0x12) = 0;
    memset(*(u32 *)(arg0 + 0x1c), 0,
                  (s32)*(s16 *)(arg0 + 0xc) * (s32)*(s16 *)(arg0 + 0xe) * 2);
}

INCLUDE_ASM(const s32, "game/code_00336B48", func_0033CCB0);

ConsNode *func_0033CCC8(u32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    ConsNode *node;
    u32 h;

    func_0033CB90();
    node = func_00328D68(0x20);
    node->unk8 = arg0;
    node->unkA = arg1;
    node->unkC = arg2;
    node->unkE = arg3;
    node->unk17 = 8;
    node->unk14 = 0;
    node->unk16 = 0;
    h = func_003292A8((arg2 * arg3) * 2);
    node->unk18 = h;
    node->unk1C = (u8 *)func_003298F8(h);
    func_0033CC78(node);
    func_0033CBF8(node);
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

