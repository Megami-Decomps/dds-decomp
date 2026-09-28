#include "common.h"

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
    u32 *list14;       /* 0x14: intrusive list walked by func_00218050 */
    MdlInner *inner;   /* 0x18 */
} MdlCtx;

/* Packet parsed by func_00216EC0. */
typedef struct MdlPacket {
    u16 unk0;     /* 0x0 */
    u16 unk2;     /* 0x2 */
    u8 unk4[4];   /* 0x4 */
    u32 unk8;     /* 0x8 */
    u16 extra[1]; /* 0xC: start of the variable payload */
} MdlPacket;

/* Load request touched by func_00216F18. */
typedef struct MdlLoadReq {
    u8 unk0[0xC]; /* 0x0 */
    u32 unkC;     /* 0xC */
} MdlLoadReq;

/* Resource released by func_002189D8. */
typedef struct MdlRes {
    u8 unk0[8]; /* 0x0 */
    u32 unk8;   /* 0x8 */
} MdlRes;
/* Entry searched by func_00217E10/mdlReleaseFirstMatch on its s16 id at +0x28.
 * Only the fields read by the matched helpers below are known. */
typedef struct MdlNode {
    struct MdlNode *next; /* 0x0 */
    u8 pad4[4];           /* 0x4 */
    void *unk8;           /* 0x8: dereferenced by mdlGetNodeRefHalf */
    u8 padC[0x10];        /* 0xC */
    f32 unk1C;            /* 0x1C: read as int by mdlGetNodeInt1C */
    f32 value;             /* 0x20: float slot of mdlGetNodeFloat20/F40 */
    u8 pad24[4];           /* 0x24 */
    s16 id;                /* 0x28: search id */
    s16 slotIndex;         /* 0x2A: slot index used by mdlClearSlotAndRelease */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;
/* 8-byte prefix copied from D_003BBB60 by mdlBuildPrefixedString. */
typedef struct Hdr8 {
    u8 b[8];
} Hdr8;

extern u32 D_00367904[][2];
extern u8 D_003BBB60[];
extern void func_002DB308(void *arg);
extern char *strcat(char *dst, const char *src);

MdlNode *func_00217E10(MdlCtx *ctx, s32 id);
void func_00217CA8(MdlCtx *ctx, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5);

extern void *func_00288B90(void);
extern u32 func_002EB090(void *);

extern u32 D_003BD878;

extern void *battleFindGroupedEntity(void);

void mdlClearSlotAndRelease(void *ctx, MdlNode *node) {
    s32 off = node->slotIndex * 4 + 0x20;
    void **slot = (void **)((u8 *)ctx + off);

    if (*slot == node) {
        *slot = NULL;
    }
    func_002DB308(node);
}

void mdlReleaseFirstMatch(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)ctx->inner->list;

    while (node != NULL) {
        if (node->id == id) {
            mdlClearSlotAndRelease(ctx, node);
            break;
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216C00);

void func_00216CC8(void *arg0, void *arg1, void *arg2) {
    void *handle;

    /* arg0/arg1 are ignored. */
    handle = battleFindGroupedEntity();
    func_00216C00(handle, arg2);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216CF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216DB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216E68);

void func_00216EC0(MdlPacket *packet) {
    func_00216DB8(packet->unk0, packet->unk2, packet->unk8, packet->extra);
    WaitSema(D_003BD878);
    func_002167E0(packet->unk0, packet->unk2);
    SignalSema(D_003BD878);
    func_002CFF98(packet);
}

void func_00216F18(void *arg0, MdlLoadReq *req) {
    void *handle;
    u32 size;

    handle = func_00288B90();
    size = func_002EB090(handle);
    req->unkC = size;
    handle = func_00288B88(arg0);
    func_002D0918(handle);
    func_002887A0(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216F68);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216FE0);

char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_003BBB60;
    return strcat(dst, src);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217068);

void func_00217298(u32 arg0, u32 arg1) {
    func_00217068(arg0, arg1, 1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002172B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217310);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217438);

INCLUDE_ASM(const s32, "model/mdlManager", func_002174C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217680);

INCLUDE_ASM(const s32, "model/mdlManager", func_002177D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217878);

INCLUDE_ASM(const s32, "model/mdlManager", func_002179A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217C60);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217CA8);

void mdlAddEntryFlagged(MdlCtx *ctx, s32 arg1, s32 arg2) {
    func_00217CA8(ctx, arg1, arg2, 1, 0.0f, 0.0f);
}

void mdlAddEntryPlain(MdlCtx *ctx, s32 arg1, s32 arg2) {
    func_00217CA8(ctx, arg1, arg2, 0, 0.0f, 0.0f);
}

void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 arg1, s32 arg2, f32 arg4, f32 arg5) {
    func_00217CA8(ctx, arg1, arg2, 1, arg4, arg5);
}

void mdlAddEntryPlainEx(MdlCtx *ctx, s32 arg1, s32 arg2, f32 arg4, f32 arg5) {
    func_00217CA8(ctx, arg1, arg2, 0, arg4, arg5);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E10);

s32 mdlGetNodeField2C(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node == NULL) {
        return -1;
    }
    return node->unk2C;
}

s32 mdlGetNodeField2E(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return node->unk2E;
}

s32 mdlGetNodeInt1C(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return (s32)node->unk1C;
}

s32 mdlCheckNodeByte30(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node == NULL) {
        return 2;
    }
    return node->unk30 == 5;
}

f32 mdlGetNodeFloat20(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);
    f32 r = 0.0f;

    if (node != NULL) {
        r = node->value;
    }
    return r;
}

void mdlSetNodeFloat20(MdlCtx *ctx, s32 id, f32 value) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node != NULL) {
        node->value = value;
    }
}

/* These shims transfer vectors between model state and VU0 registers. */
void func_00217F70(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector50;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00217F88(MdlCtx *ctx) {
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

void func_00217FA0(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector60;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217FB8);

void func_00218010(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void func_00218028(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

u32 func_00218040(MdlCtx *ctx) {
    return ctx->inner->unk1C;
}

void func_00218050(MdlCtx *ctx, u32 arg1) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        setModelResourceFrame(ctx, node, arg1);
    }
}

void mdlBroadcastMasked(MdlCtx *ctx, u32 arg1) {
    ctx->inner->unk1C = arg1;
    func_00218050(ctx, (arg1 & 0xFF000000) | 0x808080);
}

void mdlBroadcastValue(MdlCtx *ctx, u32 arg1) {
    ctx->inner->unk1C = arg1;
    func_00218050(ctx, arg1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218100);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218158);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218228);

void func_00218320(MdlCtx *ctx) {
    MdlNode *node;

    for (node = (MdlNode *)ctx->inner->list; node != NULL; node = node->next) {
        func_002DB768(node);
    }
}

void func_00218368(MdlCtx *ctx) {
    MdlNode *node;

    for (node = (MdlNode *)ctx->inner->list; node != NULL; node = node->next) {
        func_002DB788(node);
    }
}

u8 mdlHasNode(MdlCtx *ctx, s32 id) {
    MdlNode *found;

    found = func_00217E10(ctx, id);
    return found != NULL;
}

u16 func_002183D0(MdlCtx *ctx) {
    return ctx->sub->unk8;
}

u16 func_002183E0(MdlCtx *ctx) {
    return ctx->sub->unkA;
}

u32 func_002183F0(void) {
    return 8;
}

u32 mdlGetTableWord(s32 idx) {
    return D_00367904[idx][0];
}

s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 id) {
    MdlNode *node = func_00217E10(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return *(u16 *)node->unk8;
}

void func_00218440(MdlCtx *ctx) {
    func_002DA1B0(ctx->inner->unk8);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218460);

s32 mdlIsInnerSentinel(MdlCtx *ctx) {
    s32 r = 0;

    if ((u8)ctx->unk10 == 1) {
        r = ctx->inner == (MdlInner *)0x30424950;
    }
    return r;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002185B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218768);

void func_002189D8(MdlRes *res) {
    func_00288788(res->unk8);
    func_002CFF98(res);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A08);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A88);

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB60);

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB68);

