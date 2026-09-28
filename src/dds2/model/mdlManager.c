#include "common.h"

extern u64 func_002C8108(u64);

extern u64 func_002C8110(void);

extern u32 func_00343F38(u64);

extern u32 D_00438F90;

extern u64 func_00231220(void);

extern s32 func_00232928(void);

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by func_002183D0/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

/* Record behind MdlCtx.inner. */
typedef struct MdlInner {
    u8 unk0[8];  /* 0x0 */
    u32 unk8;    /* 0x8: freed through func_002DA1B0 */
    u8 unkC[8];  /* 0xC */
    u32 *list;   /* 0x14: intrusive list walked by func_00218320/368 */
    u8 unk18[4]; /* 0x18 */
    u32 unk1C;   /* 0x1C */
} MdlInner;

/* Context shared by the matched mdlManager helpers. */
typedef struct MdlCtx {
    u8 unk0[0xC];      /* 0x0 */
    MdlSub *sub;       /* 0xC */
    u32 unk10;         /* 0x10 */
    u32 *list14;       /* 0x14: intrusive list walked by func_00218050 */
    MdlInner *inner;   /* 0x18 */
} MdlCtx;

/* Entry searched by func_00217E10/func_00216BB0 on its s16 id at +0x28.
 * Only the fields read by the matched helpers below are known. */
typedef struct MdlNode {
    struct MdlNode *next; /* 0x0 */
    u8 pad4[4];           /* 0x4 */
    void *unk8;           /* 0x8: dereferenced by func_00218410 */
    u8 padC[0x10];        /* 0xC */
    f32 unk1C;            /* 0x1C: read as int by func_00217EB0 */
    f32 unk20;            /* 0x20: float slot of func_00217F18/F40 */
    u8 pad24[4];          /* 0x24 */
    s16 unk28;            /* 0x28: search id */
    s16 unk2A;            /* 0x2A: slot index used by func_00216B78 */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;

extern u32 D_003C86B4[][2];

INCLUDE_ASM(const s32, "model/mdlManager", func_00231690);

void func_002316C8(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)ctx->inner->list;

    while (node != NULL) {
        if (node->unk28 == id) {
            func_00231690(ctx, node);
            break;
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231718);

void func_002317E0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00231220();
    func_00231718(temp_v0, arg2);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231810);

INCLUDE_ASM(const s32, "model/mdlManager", func_002318D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231980);

void func_002319D8(u32 arg0) {
    u16 *puVar1;

    puVar1 = (u16 *)arg0;
    func_002318D0(*puVar1, puVar1[1], *(u32 *)(puVar1 + 4), puVar1 + 6);
    WaitSema(D_00438F90);
    func_002312F8(*puVar1, puVar1[1]);
    SignalSema(D_00438F90);
    func_00328E48(arg0);
}

void func_00231A30(u64 arg0, s32 arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_002C8110();
    temp_v1 = func_00343F38(temp_v0);
    *(u32 *)(arg1 + 0xc) = temp_v1;
    temp_v0 = func_002C8108(arg0);
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231A80);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231AF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231B50);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231B80);

void func_00231DB0(u32 arg0, u32 arg1) {
    func_00231B80(arg0, arg1, 1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231DC8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231E28);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231F50);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231FD8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232198);

INCLUDE_ASM(const s32, "model/mdlManager", func_002322E8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232390);

INCLUDE_ASM(const s32, "model/mdlManager", func_002324C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232778);

INCLUDE_ASM(const s32, "model/mdlManager", func_002327C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328B8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328D8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232910);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232928);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232970);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329A0);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329C8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232A30);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232A58);

/* These shims transfer vectors between model state and VU0 registers. */
void func_00232A88(MdlCtx *ctx) {
    void *vec = (u8 *)ctx->inner + 0x50;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00232AA0(MdlCtx *ctx) {
    void *vec;
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.w vf10, vf0\n"
        ".set reorder"
        : : : "memory");
    vec = (u8 *)ctx->inner + 0x50;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00232AB8(MdlCtx *ctx) {
    void *vec = (u8 *)ctx->inner + 0x60;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232AD0);

void func_00232B28(MdlCtx *ctx) {
    void *vec = (u8 *)ctx->inner + 0x70;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00232B40(MdlCtx *ctx) {
    void *vec = (u8 *)ctx->inner + 0x70;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

u32 func_00232B58(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x1c);
}

void func_00232B68(u32 arg0, u32 arg1) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)((s32)arg0 + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_002350D0(arg0, puVar1, arg1);
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232BC0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232BF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C18);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C70);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232D40);

void func_00232E38(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_00334618(puVar1);
    }
}

void func_00232E80(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_00334638(puVar1);
    }
}

u8 func_00232EC8(void) {
    s64 temp_v0;

    temp_v0 = func_00232928();
    return temp_v0 != 0;
}

u16 func_00232EE8(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 8);
}

u16 func_00232EF8(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 10);
}

u32 func_00232F08(void) {
    return 8;
}

u32 func_00232F10(s32 idx) {
    return D_003C86B4[idx][0];
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F28);

void func_00232F58(s32 arg0) {
    func_00333060(*(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F78);

s32 func_00233098(MdlCtx *ctx) {
    s32 r = 0;

    if ((u8)ctx->unk10 == 1) {
        r = ctx->inner == (MdlInner *)0x30424950;
    }
    return r;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002330C8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00233280);

void func_002334F0(u32 arg0) {
    func_002C7CE8(*(u32 *)((s32)arg0 + 8));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00233520);

INCLUDE_ASM(const s32, "model/mdlManager", func_002335A0);


INCLUDE_SDATA(const s32, "model/mdlManager", D_00436FA0);


INCLUDE_SDATA(const s32, "model/mdlManager", D_00436FA8);

