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
    s16 searchId;          /* 0x28: identifies a node in list lookups */
    s16 slotIndex;         /* 0x2A: slot index used by func_00216B78 */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;

extern u32 D_003C86B4[][2];

INCLUDE_ASM(const s32, "model/mdlManager", mdlClearSlotAndRelease);

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

INCLUDE_ASM(const s32, "model/mdlManager", mdlBuildPrefixedString);

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

INCLUDE_ASM(const s32, "model/mdlManager", mdlAddEntryFlagged);

INCLUDE_ASM(const s32, "model/mdlManager", mdlAddEntryPlain);

INCLUDE_ASM(const s32, "model/mdlManager", mdlAddEntryFlaggedEx);

INCLUDE_ASM(const s32, "model/mdlManager", mdlAddEntryPlainEx);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232928);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeField2C);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeField2E);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeInt1C);

INCLUDE_ASM(const s32, "model/mdlManager", mdlCheckNodeByte30);

INCLUDE_ASM(const s32, "model/mdlManager", mdlGetNodeFloat20);

INCLUDE_ASM(const s32, "model/mdlManager", mdlSetNodeFloat20);

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
        mdlSetResourceFrame(arg0, puVar1, arg1);
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlBroadcastMasked);

INCLUDE_ASM(const s32, "model/mdlManager", mdlBroadcastValue);

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

void func_00232F58(s32 arg0) {
    func_00333060(*(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
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

