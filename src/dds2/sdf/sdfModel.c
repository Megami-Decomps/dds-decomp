#include "common.h"

/* 16-byte packet, written at quadword, word, halfword and byte granularity. */
typedef struct {
    union {
        s64 q;
        struct {
            u16 h0;
            u8 b2;
            u8 b3;
            u32 w4;
        } p;
    } u0;
    union {
        s64 q;
        struct {
            s32 w8;
            s32 wC;
        } p;
    } u8;
} SdfPacket; /* 0x10 */

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    void **unkC;
} SdfList;

typedef struct {
    SdfList *unk0;     /* 0x00 */
    u8 pad_0x04[0x0C]; /* 0x04 */
    void *unk10;       /* 0x10 */
    u8 pad_0x14[0x05]; /* 0x14 */
    u8 unk19;          /* 0x19 */
    u8 pad_0x1A[0x16]; /* 0x1A */
    s32 unk30;         /* 0x30 */
    u8 pad_0x34[0x04]; /* 0x34 */
    s32 unk38;         /* 0x38 */
} SdfModel;

extern void func_003307A0(void);

extern void func_00330768(void *arg0);

extern void func_003314B0(void *arg0);

extern void func_003312A8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void func_00331590(void *arg0, void *arg1);

typedef struct {
    s32 unk0;            /* 0x00: entry count */
    u8 pad_0x04[0x0C];   /* 0x04 */
    u8 unk10;            /* 0x10: entries, 0x50 stride */
} SdfItemList;

typedef struct {
    SdfItemList *unk0;   /* 0x00 */
} SdfItemListRef;

extern SdfModel *func_003317C8(void *arg0, void *arg1);

extern void func_00331500(void *arg0, void *arg1);

extern void func_00336B00(void);

extern void func_0033AEA8(u32 arg0);

extern void *memcpy(void *dst, const void *src, u32 n);

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    void *unkC;
} SdfObj;

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330C18);

SdfPacket *func_00330CA0(SdfModel *arg0, SdfPacket *arg1, s32 arg2) {
    u32 a = (arg0->unk30 + (arg2 << 7)) & 0x0FFFFFFF;

    arg1->u0.q = ((s64)a << 32) | 0x30000008;
    arg1->u8.p.wC = 0x6C07C000;
    arg1->u8.p.w8 = 0;
    return arg1 + 1;
}

SdfPacket *func_00330CE8(SdfPacket *arg0) {
    arg0->u8.q = 0;
    arg0->u0.q = 0x60000000;
    return arg0 + 1;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330D00);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330D30);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330DF0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330E60);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330FE0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331238);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003312A8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003314B0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331500);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331590);

void func_003316B0(SdfModel *arg0, s32 arg1, s32 arg2) {
    s32 i = 0;
    s32 j = 0;

    arg0->unk38 = arg1;
    func_003307A0();
    func_00330768(arg0);
    func_003314B0(arg0);
    i = 0;
    j = 0;
    do {
        i++;
        func_003312A8(arg0, arg1, arg2, 0, j);
        j = i;
    } while (i != 2);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331740);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003317C8);

SdfModel *func_003318B8(void *arg0, SdfItemListRef *arg1) {
    s32 i = 0;
    SdfModel *ret = func_003317C8(arg0, arg1);
    SdfItemList *arr = arg1->unk0;
    s32 n = arr->unk0;
    u8 *item = &arr->unk10;

    if (n != i) {
        do {
            func_00331590(ret->unk0->unkC[i], item);
            item += 0x50;
            i++;
        } while (i != n);
    }
    return ret;
}

SdfModel *func_00331940(void *arg0, SdfItemListRef *arg1) {
    s32 i = 0;
    SdfModel *ret = func_003317C8(arg0, arg1);
    SdfItemList *arr;
    s32 n;
    u8 *item;

    ret->unk19 |= 4;
    arr = arg1->unk0;
    n = arr->unk0;
    item = &arr->unk10;
    if (n != i) {
        do {
            func_00331500(ret->unk0->unkC[i], item);
            item += 0x50;
            i++;
        } while (i != n);
    }
    return ret;
}

void func_003319D0(void *arg0, void *buf, s32 arg2) {
    u8 *p = (u8 *)arg0;
    u8 *b1 = p + 0x80;
    u8 *b2;
    u8 *b3;
    u8 *b4;
    u8 *b5;
    u8 *dst;
    u32 base;
    void *node;

    __asm__ volatile ("lqc2 vf28, 0(%0)" :: "r" (b1) : "memory");
    b2 = p + 0x90;
    __asm__ volatile ("lqc2 vf29, 0(%0)" :: "r" (b2) : "memory");
    b3 = p + 0xA0;
    __asm__ volatile ("lqc2 vf30, 0(%0)" :: "r" (b3) : "memory");
    b4 = p + 0x70;
    __asm__ volatile ("lqc2 vf10, 0(%0)" :: "r" (b4) : "memory");
    __asm__ volatile (
        ".set noreorder                   \n"
        "vmulx.xyzw vf28, vf28, vf10x     \n"
        "vmuly.xyzw vf29, vf29, vf10y     \n"
        "vmulz.xyzw vf30, vf30, vf10z     \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    b5 = p + 0x60;
    __asm__ volatile ("lqc2 vf31, 0(%0)" :: "r" (b5) : "memory");
    __asm__ volatile (
        ".set noreorder          \n"
        "lqc2 vf24, 0(%0)        \n"
        "lqc2 vf25, 16(%0)       \n"
        "lqc2 vf26, 32(%0)       \n"
        "lqc2 vf27, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (buf)
        : "memory"
    );
    func_00336B00();
    dst = p + 0xC0;
    __asm__ volatile (
        ".set noreorder          \n"
        "sqc2 vf28, 0(%0)        \n"
        "sqc2 vf29, 16(%0)       \n"
        "sqc2 vf30, 32(%0)       \n"
        "sqc2 vf31, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (dst)
        : "memory"
    );
    base = *(u32 *)(p + 0x30);
    if (base != 0) {
        func_0033AEA8(base + (arg2 << 7));
    }
    node = *(void **)(p + 0x0C);
    if (node == 0) {
        return;
    }
    do {
        func_003319D0(node, dst, arg2);
        node = *(void **)((u8 *)node + 4);
    } while (node != *(void **)(p + 0x0C));
}

void func_00331AB0(SdfModel *arg0, s32 arg1) {
    u128 buf[4];
    u8 *p = (u8 *)arg0;
    u8 *b1 = p + 0x20;
    u8 *b2;
    SdfList *list;

    __asm__ volatile (
        ".set noreorder          \n"
        "lqc2 vf28, 0(%0)        \n"
        "lqc2 vf29, 16(%0)       \n"
        "lqc2 vf30, 32(%0)       \n"
        "lqc2 vf31, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (b1)
        : "memory"
    );
    b2 = p + 0x70;
    __asm__ volatile (
        "lqc2 vf10, 0(%0)"
        :
        : "r" (b2)
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder               \n"
        "vmul.xyz vf28, vf28, vf10    \n"
        "vmul.xyz vf29, vf29, vf10    \n"
        "vmul.xyz vf30, vf30, vf10    \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder          \n"
        "sqc2 vf28, %0           \n"
        "sqc2 vf29, %1           \n"
        "sqc2 vf30, %2           \n"
        "sqc2 vf31, %3           \n"
        ".set reorder"
        :
        : "m" (buf[0]), "m" (buf[1]), "m" (buf[2]), "m" (buf[3])
        : "memory"
    );
    list = arg0->unk0;
    func_003319D0(list->unkC[0], buf, arg1);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331B18);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331B38);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331B68);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331C80);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003320E8);

void sdfModelCopyData(SdfObj *arg0, SdfObj *arg1) {
    s16 n;

    if (arg0 == NULL) {
        return;
    }
    n = arg0->unk4;
    if (n > 0) {
        memcpy(arg0->unkC, arg1->unkC, n * 16);
    }
}
