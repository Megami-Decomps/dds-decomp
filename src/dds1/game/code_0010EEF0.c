#include "common.h"

extern u64 func_00119AF8(u64);
extern u64 func_0010D428(u64);

extern void *func_002CFEB8(s32 size);
extern void func_002CFF98(void *p);
extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);


typedef struct EEF0Node EEF0Node;

typedef struct {
    u8 pad00[0x4];              /* 0x00 */
    void (*notify)(EEF0Node *); /* 0x04 called with the node being destroyed */
} EEF0Owner;

/* Transform node (0xD0 bytes). The links at 0x10/0x20/0x24 tie a node into
 * its owner's list; inner points at a child node whose vectors live in VU
 * registers between calls (loaded with lqc2, stored with sqc2).
 */
struct EEF0Node {
    u8 pad00[0x10];   /* 0x00 */
    EEF0Owner *owner; /* 0x10 */
    u8 pad14[0x8];    /* 0x14 */
    EEF0Node *inner;  /* 0x1C */
    EEF0Node *prev;   /* 0x20 */
    EEF0Node *next;   /* 0x24 */
    u8 pad28[0x18];   /* 0x28 */
    u128 vec40;       /* 0x40 */
    u128 vec50;       /* 0x50 */
    u128 vec60;       /* 0x60 */
    u8 pad70[0x10];   /* 0x70 */
    u128 vec80;       /* 0x80 copy of vec40 */
    u128 vec90;       /* 0x90 copy of vec50 */
    u128 vecA0;       /* 0xA0 copy of vec60 */
    u8 vecB0[0x10];   /* 0xB0 cleared on alloc */
    u32 flags;        /* 0xC0 bit0 set, bit1 cleared on vector write */
    f32 unkC4;        /* 0xC4 */
    u32 unkC8;        /* 0xC8 */
};

/* 128-bit vector copy for the inner vectors. The scratch register is named
 * explicitly: a plain-C u128 copy lets gcc 2.96 fold the destination into
 * an sq offset and hoist the lq above the flag update, while the retail
 * code computes both addresses with addiu and keeps each copy together.
 * The destination is the first operand so its address is computed first.
 */
#define EEF0_COPY128(dst, src) __asm__ volatile ( \
    ".set noreorder \n" \
    "lq $2, 0(%1)   \n" \
    "sq $2, 0(%0)   \n" \
    ".set reorder" \
    : : "r" (dst), "r" (src) : "memory", "$2")

/* Store vf10 to base+off, recomputing the address. The tied output (no
 * early clobber) lets gcc reuse the base register for the address.
 */
#define EEF0_STORE_V10(dst, base, off) __asm__ volatile ( \
    ".set noreorder    \n" \
    "addiu %0, %1, %2  \n" \
    "sqc2 vf10, 0(%0)  \n" \
    ".set reorder" \
    : "=r" (dst) : "r" (base), "i" (off) : "memory")

u32 func_0010EEF0(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_00119B08(temp_v0);
    return 1;
}

u32 func_0010EF18(void) {
    func_00119E00();
    return 1;
}

u32 func_0010EF38(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_00119AF8(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010EF68);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F268);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F2F0);

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F418);

void func_0010F4F0(EEF0Node *arg0) {
    EEF0Owner *owner;
    EEF0Node *next;
    EEF0Node *prev;

    if (arg0 != NULL) {
        owner = arg0->owner;
        if (owner != NULL) {
            if (owner->notify != NULL) {
                owner->notify(arg0);
            }
        }
        next = arg0->next;
        if (next != NULL) {
            next->prev = arg0->prev;
        }
        prev = arg0->prev;
        if (prev != NULL) {
            prev->next = arg0->next;
        }
        func_002CFF98(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F570);

void func_0010F5E8(EEF0Node *arg0) {
    EEF0Node *inner;

    if (arg0 != NULL) {
        inner = arg0->inner;
        if (inner != NULL) {
            func_002CFF98(inner);
            arg0->inner = NULL;
        }
    }
}

void func_0010F628(EEF0Node *arg0, u32 arg1) {
    arg0->flags |= arg1;
}

void func_0010F638(EEF0Node *arg0, u32 arg1) {
    arg0->flags &= ~arg1;
}

u8 func_0010F650(EEF0Node *arg0, u32 arg1) {
    return (arg0->flags & arg1) != 0;
}

void func_0010F660(EEF0Node *arg0) {
    u8 *p40 = (u8 *)arg0 + 0x40;
    u8 *p50;
    u8 *p60;

    __asm__ volatile (
        ".set noreorder      \n"
        "sqc2 vf0, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p40)
        : "memory"
    );
    p50 = (u8 *)arg0 + 0x50;
    __asm__ volatile (
        ".set noreorder      \n"
        "sqc2 vf0, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p50)
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder            \n"
        "vaddw.xyz vf10, vf0, vf0w \n"
        "vmulx.w vf10, vf0, vf0x   \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    p60 = (u8 *)arg0 + 0x60;
    __asm__ volatile (
        ".set noreorder       \n"
        "sqc2 vf10, 0(%0)     \n"
        ".set reorder"
        :
        : "r" (p60)
        : "memory"
    );
}

void func_0010F688(EEF0Node *arg0) {
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0xA0), (u128 *)((u8 *)arg0 + 0x60));
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0x90), (u128 *)((u8 *)arg0 + 0x50));
    EEF0_COPY128((u128 *)((u8 *)arg0 + 0x80), (u128 *)((u8 *)arg0 + 0x40));
}

void func_0010F6C0(EEF0Node *arg0, f32 fparg0) {
    arg0->inner->unkC4 = fparg0;
}

f32 func_0010F6D0(EEF0Node *arg0) {
    return arg0->inner->unkC4;
}

void func_0010F6E0(EEF0Node *arg0, u128 *arg1) {
    EEF0Node *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x40);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void func_0010F710(EEF0Node *arg0, u128 *arg1) {
    EEF0Node *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x50);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void func_0010F740(EEF0Node *arg0, u128 *arg1) {
    EEF0Node *inner = arg0->inner;
    u128 *dst = (u128 *)((u8 *)inner + 0x60);

    inner->flags = (inner->flags | 1) & ~2;
    EEF0_COPY128(dst, arg1);
}

void func_0010F770(EEF0Node *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x40;

    __asm__ volatile (
        ".set noreorder       \n"
        "lqc2 vf10, 0(%0)     \n"
        "vmove.w vf10, vf0    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
}

void func_0010F788(EEF0Node *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x50;

    __asm__ volatile (
        ".set noreorder      \n"
        "lqc2 vf10, 0(%0)    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
    effMiscNormalizeVU();
}

void func_0010F7A8(EEF0Node *arg0) {
    u8 *p = (u8 *)arg0->inner + 0x60;

    __asm__ volatile (
        ".set noreorder      \n"
        "lqc2 vf10, 0(%0)    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
}

void func_0010F7C0(EEF0Node *arg0, void *arg1) {
    EEF0Node *inner = arg0->inner;
    u8 *src = (u8 *)inner + 0x40;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0(%0)           \n"
        "lqc2 vf11, 0(%1)           \n"
        "vadd.xyzw vf10, vf10, vf11 \n"
        ".set reorder"
        :
        : "r" (src), "r" (arg1)
        : "memory"
    );
    EEF0_STORE_V10(dst, inner, 0x40);
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010F7F8);

void func_0010F848(EEF0Node *arg0, void *arg1) {
    EEF0Node *inner = arg0->inner;
    u8 *src = (u8 *)inner + 0x60;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    __asm__ volatile (
        ".set noreorder             \n"
        "lqc2 vf10, 0(%0)           \n"
        "lqc2 vf11, 0(%1)           \n"
        "vmul.xyzw vf10, vf10, vf11 \n"
        ".set reorder"
        :
        : "r" (src), "r" (arg1)
        : "memory"
    );
    EEF0_STORE_V10(dst, inner, 0x60);
}



INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A0);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A8);


INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9B0);

