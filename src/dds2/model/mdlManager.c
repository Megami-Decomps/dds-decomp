#include "common.h"

extern u64 func_002C8108(u64);

extern u64 func_002C8110(void);

extern u32 func_00343F38(u64);

extern u32 D_00438F90;

extern u64 btlFindGroupedEntity(void);

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
    u32 resourceHandle; /* 0x8: released by func_00232F58 */
    u8 unkC[8];  /* 0xC */
    u32 *list;   /* 0x14: intrusive list walked by func_00232E38/80 */
    u8 unk18[4]; /* 0x18 */
    u32 unk1C;   /* 0x1C */
    u8 pad20[0x30];
    u128 vector50; /* 0x50 */
    u128 vector60; /* 0x60 */
    u128 vector70; /* 0x70 */
} MdlInner;

/* Context shared by the matched mdlManager helpers. */
typedef struct MdlCtx {
    u8 unk0[0xC];      /* 0x0 */
    MdlSub *sub;       /* 0xC */
    u32 unk10;         /* 0x10 */
    u32 *list14;       /* 0x14: intrusive list walked by func_00232B68 */
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
    s16 searchId;          /* 0x28: identifies a node in list lookups */
    s16 slotIndex;         /* 0x2A: slot index used by func_00216B78 */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;

extern u32 D_003C86B4[][2];

extern void func_003341B8(void *arg);

/* 8-byte prefix copied from D_003BBB60 by mdlBuildPrefixedString. */
typedef struct Hdr8 {
    u8 b[8];
} Hdr8;

extern u8 D_00436FA0[];

extern char *strcat(char *dst, const char *src);

void func_002327C0(MdlCtx *ctx, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5);

void mdlClearSlotAndRelease(void *ctx, MdlNode *node) {
    s32 off = node->slotIndex * 4 + 0x20;
    void **slot = (void **)((u8 *)ctx + off);

    if (*slot == node) {
        *slot = NULL;
    }
    func_003341B8(node);
}

void mdlReleaseFirstMatch(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)ctx->inner->list;

    while (node != NULL) {
        if (node->searchId == id) {
            mdlClearSlotAndRelease(ctx, node);
            break;
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231718);

void func_002317E0(u64 unused0, u64 unused1, u64 command) {
    u64 entity;

    entity = btlFindGroupedEntity();
    func_00231718(entity, command);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231810);

INCLUDE_ASM(const s32, "model/mdlManager", func_002318D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231980);

void func_002319D8(u32 job) {
    u16 *words;

    words = (u16 *)job;
    func_002318D0(*words, words[1], *(u32 *)(words + 4), words + 6);
    WaitSema(D_00438F90);
    func_002312F8(*words, words[1]);
    SignalSema(D_00438F90);
    func_00328E48(job);
}

void func_00231A30(u64 resource, s32 destination) {
    u64 handle;
    u32 resolved;

    handle = func_002C8110();
    resolved = func_00343F38(handle);
    *(u32 *)(destination + 0xc) = resolved;
    handle = func_002C8108(resource);
    func_003297C8(handle);
    func_002C7D00(resource);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231A80);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231AF8);

char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_00436FA0;
    return strcat(dst, src);
}

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

void mdlAddEntryFlagged(MdlCtx *ctx, s32 arg1, s32 arg2) {
    func_002327C0(ctx, arg1, arg2, 1, 0.0f, 0.0f);
}

void mdlAddEntryPlain(MdlCtx *ctx, s32 arg1, s32 arg2) {
    func_002327C0(ctx, arg1, arg2, 0, 0.0f, 0.0f);
}

void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 arg1, s32 arg2, f32 arg4, f32 arg5) {
    func_002327C0(ctx, arg1, arg2, 1, arg4, arg5);
}

void mdlAddEntryPlainEx(MdlCtx *ctx, s32 arg1, s32 arg2, f32 arg4, f32 arg5) {
    func_002327C0(ctx, arg1, arg2, 0, arg4, arg5);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232928);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeField2C);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeField2E);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeInt1C);

INCLUDE_ASM(const s32, "model/mdlManager", mdlCheckNodeByte30);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeFloat20);

INCLUDE_ASM(const s32, "model/mdlManager", mdlSetNodeFloat20);

/* These shims transfer vectors between model state and VU0 registers. */
void func_00232A88(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector50;
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
    vec = &ctx->inner->vector50;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00232AB8(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector60;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232AD0);

void func_00232B28(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00232B40(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

u32 func_00232B58(MdlCtx *ctx) {
    return ctx->inner->unk1C;
}

void func_00232B68(MdlCtx *ctx, u32 value) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        mdlSetResourceFrame(ctx, node, value);
    }
}

void mdlBroadcastMasked(MdlCtx *ctx, u32 arg1) {
    ctx->inner->unk1C = arg1;
    func_00232B68(ctx, (arg1 & 0xFF000000) | 0x808080);
}

void mdlBroadcastValue(MdlCtx *ctx, u32 arg1) {
    ctx->inner->unk1C = arg1;
    func_00232B68(ctx, arg1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C18);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C70);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232D40);

void func_00232E38(MdlCtx *ctx) {
    MdlNode *node;

    for (node = (MdlNode *)ctx->inner->list; node != NULL; node = node->next) {
        func_00334618(node);
    }
}

void func_00232E80(MdlCtx *ctx) {
    MdlNode *node;

    for (node = (MdlNode *)ctx->inner->list; node != NULL; node = node->next) {
        func_00334638(node);
    }
}

u8 mdlHasNode(void) {
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

u32 mdlGetTableWord(s32 idx) {
    return D_003C86B4[idx][0];
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeRefHalf);

void func_00232F58(MdlCtx *ctx) {
    func_00333060(ctx->inner->resourceHandle);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F78);

s32 mdlIsInnerSentinel(MdlCtx *ctx) {
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
