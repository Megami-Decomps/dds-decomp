#include "common.h"

extern void *func_002DAAA0(s32 arg0, s32 arg1, s32 arg2);
extern void *func_002CFEB8(s32 arg0);
extern void *func_002E75F0(s32 arg0, s32 arg1, s32 arg2);
extern void func_002EFD30(s32 arg0);
extern void *memcpy(void *dst, const void *src, u32 n);
extern void func_002D78F0(void);
extern void func_002D78B8(void *arg0);
extern void func_002D8600(void *arg0);
extern void func_002D83F8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_002D86E0(void *arg0, void *arg1);
extern void func_002D8650(void *arg0, void *arg1);
extern void func_002DDC50(void);
extern void func_002E1FF8(u32 arg0);
extern vu8 D_003BD2EA;

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
    u8 pad_0x00[0x18];
    u32 unk18;
} SdfEntry;

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

typedef struct {
    u8 pad_0x00[0x0C];
    s32 *unkC;
} SdfChunk;

typedef struct {
    u8 pad_0x00[0x04];
    u32 unk4;
    u32 unk8;
} SdfInfo;

typedef struct {
    void *unk0;       /* 0x0 */
    u8 unk4;          /* 0x4 */
    s8 unk5;          /* 0x5 */
    s16 unk6;         /* 0x6 */
    u8 pad_0x08[0x4]; /* 0x8 */
    s32 unkC;         /* 0xC */
} SdfNode; /* 0x10 */

typedef struct {
    u8 pad_0x00[0x28];
    void *unk28;
} SdfLink;

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    void *unkC;
} SdfBuf;

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    void *unkC;
} SdfObj;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} SdfMsg;

typedef struct {
    s32 unk0;            /* 0x00: entry count */
    u8 pad_0x04[0x0C];   /* 0x04 */
    u8 unk10;            /* 0x10: entries, 0x50 stride */
} SdfItemList;

typedef struct {
    SdfItemList *unk0;   /* 0x00 */
} SdfItemListRef;

extern SdfModel *func_002D8918(void *arg0, void *arg1);


INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7D68);

SdfPacket *func_002D7DF0(SdfModel *arg0, SdfPacket *arg1, s32 arg2) {
    u32 a = (arg0->unk30 + (arg2 << 7)) & 0x0FFFFFFF;

    arg1->u0.q = ((s64)a << 32) | 0x30000008;
    arg1->u8.p.wC = 0x6C07C000;
    arg1->u8.p.w8 = 0;
    return arg1 + 1;
}

SdfPacket *func_002D7E38(SdfPacket *arg0) {
    arg0->u8.q = 0;
    arg0->u0.q = 0x60000000;
    return arg0 + 1;
}

void *func_002D7E50(SdfChunk *arg0, s32 arg1, s32 arg2, s32 arg3) {
    SdfChunk *p = (SdfChunk *)arg0->unkC;
    return func_002DAAA0(((s32 *)p->unkC)[arg1], arg2, arg3);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7E80);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7F40);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7FB0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8130);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8388);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D83F8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8600);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8650);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D86E0);

void func_002D8800(SdfModel *model, s32 arg1, s32 arg2) {
    s32 i = 0;
    s32 j = 0;

    model->unk38 = arg1;
    func_002D78F0();
    func_002D78B8(model);
    func_002D8600(model);
    /* The re-initialization below is load-bearing for a byte-identical build. */
    i = 0;
    j = 0;
    do {
        i++;
        func_002D83F8(model, arg1, arg2, 0, j);
        j = i;
    } while (i != 2);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8890);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8918);

SdfModel *func_002D8A08(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = func_002D8918(data, listRef);
    SdfItemList *list = listRef->unk0;
    s32 count = list->unk0;
    u8 *item = &list->unk10;

    if (count != i) {
        do {
            func_002D86E0(model->unk0->unkC[i], item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

SdfModel *func_002D8A90(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = func_002D8918(data, listRef);
    SdfItemList *list;
    s32 count;
    u8 *item;

    model->unk19 |= 4;
    list = listRef->unk0;
    count = list->unk0;
    item = &list->unk10;
    if (count != i) {
        do {
            func_002D8650(model->unk0->unkC[i], item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

void func_002D8B20(void *arg0, void *buf, s32 arg2) {
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
    func_002DDC50();
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
        func_002E1FF8(base + (arg2 << 7));
    }
    node = *(void **)(p + 0x0C);
    if (node == 0) {
        return;
    }
    do {
        func_002D8B20(node, dst, arg2);
        node = *(void **)((u8 *)node + 4);
    } while (node != *(void **)(p + 0x0C));
}

void func_002D8C00(SdfModel *arg0, s32 arg1) {
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
    func_002D8B20(list->unkC[0], buf, arg1);
}

void func_002D8C68(void *arg0) {
    func_002D8C00(arg0, (s8)D_003BD2EA);
}

void func_002D8C88(SdfMsg *msg, s32 unk0, s32 unk4, s32 unk8, s32 unkC) {
    msg->unkC = unkC;
    msg->unk0 = unk0;
    msg->unk8 = unk8;
    msg->unk4 = unk4;
    func_002EFD30(unk4);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8CB8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8DD0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D9238);

void sdfModelCopyData(SdfObj *dst, SdfObj *src) {
    s16 count;

    if (dst == NULL) {
        return;
    }
    count = dst->unk4;
    if (count > 0) {
        memcpy(dst->unkC, src->unkC, count * 16);
    }
}
