#include "common.h"
#include "pcp_vu0.h"

extern u8 D_003296F0[];
extern u8 D_00324610[];
extern u8 D_00324650[];
extern u8 D_00324660[];
extern void func_002DDD60(void *);
extern void func_002DDC50(void);

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by func_002183D0/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

/* Entry enabled by mdlEnableAllEntries. */
typedef struct MdlEntry {
    u8 unk0[0x14]; /* 0x0 */
    s16 enabled;   /* 0x14 */
} MdlEntry;

/* Entry table pointed to by the first word of MdlInner. */
typedef struct MdlEntryTable {
    u8 unk0[4];        /* 0x0 */
    s16 count;         /* 0x4 */
    u8 unk6[6];        /* 0x6 */
    MdlEntry **items;  /* 0xC */
} MdlEntryTable;

typedef struct MdlNode MdlNode;

/* Record behind MdlCtx.inner. */
typedef struct MdlInner {
    MdlEntryTable *entries; /* 0x0 */
    u8 unk4[4];  /* 0x4 */
    u32 resourceHandle; /* 0x8: released through func_002DA1B0 */
    u8 unkC[8];  /* 0xC */
    MdlNode *list; /* 0x14: intrusive list walked by func_00218320/368 */
    u8 unk18[4]; /* 0x18 */
    u32 broadcastValue; /* 0x1C: last value passed to mdlBroadcastValue/Masked */
    u128 vector20; /* 0x20: matrix row 0 (vf28) */
    u128 vector30; /* 0x30: matrix row 1 (vf29) */
    u128 vector40; /* 0x40: matrix row 2 (vf30) */
    u128 vector50; /* 0x50 */
    u128 vector60; /* 0x60 */
    u128 vector70; /* 0x70 */
} MdlInner;

/* Context shared by the matched mdlManager helpers. */
typedef struct MdlCtx {
    u8 unk0[0xC];      /* 0x0 */
    MdlSub *sub;       /* 0xC */
    u32 unk10;         /* 0x10 */
    u32 *list14;       /* 0x14: intrusive list walked by mdlSetAllResourceFrames */
    MdlInner *inner;   /* 0x18 */
    u8 unk1C[0x14];    /* 0x1C */
    struct MdlDevList *devList; /* 0x30: device slots released with the model */
} MdlCtx;

typedef struct MdlDevSlot {
    struct MdlDevSlot *next; /* 0x0 */
    void *slot;              /* 0x4 */
} MdlDevSlot;

typedef struct MdlDevList {
    MdlDevSlot *first; /* 0x0 */
} MdlDevList;

/* Packet parsed by mdlExecuteAndFreeJob. */
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
    u32 size;      /* 0xC: size read from the current file resource */
} MdlLoadReq;

/* Resource released by func_002189D8. */
typedef struct MdlRes {
    u8 unk0[8]; /* 0x0 */
    u32 unk8;   /* 0x8 */
} MdlRes;
/* Entry searched by mdlFindNodeById/mdlReleaseFirstMatch on its s16 id at +0x28.
 * Only the fields read by the matched helpers below are known. */
typedef struct MdlNode {
    struct MdlNode *next; /* 0x0 */
    u8 pad4[4];           /* 0x4 */
    void *unk8;           /* 0x8: dereferenced by mdlGetNodeRefHalf */
    u8 padC[0x10];        /* 0xC */
    f32 unk1C;            /* 0x1C: read as int by mdlGetNodeInt1C */
    f32 value;             /* 0x20: float slot of mdlGetNodeFloat20/F40 */
    u8 pad24[4];           /* 0x24 */
    s16 searchId;          /* 0x28: identifies a node in list lookups */
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

MdlNode *mdlFindNodeById(MdlCtx *ctx, s32 id);
void func_00217CA8(MdlCtx *ctx, s32 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5);

extern void *func_00288B90();
extern u32 func_002EB090(void *);
extern void mdlSetResourceAmount(MdlCtx *ctx, u32 *node, f32 amount);

extern u32 D_003BD878;

extern void *btlFindGroupedEntity();

void mdlClearSlotAndRelease(void *ctx, MdlNode *node) {
    s32 offset = node->slotIndex * 4 + 0x20;
    void **slot = (void **)((u8 *)ctx + offset);

    if (*slot == node) {
        *slot = NULL;
    }
    func_002DB308(node);
}

void mdlReleaseFirstMatch(MdlCtx *ctx, s32 id) {
    MdlNode *node = ctx->inner->list;

    while (node != NULL) {
        if (node->searchId == id) {
            mdlClearSlotAndRelease(ctx, node);
            break;
        }
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216C00);

void func_00216CC8(void *unused0, void *unused1, void *command) {
    void *entity;

    /* Only the grouped entity and command are forwarded to the worker. */
    entity = btlFindGroupedEntity();
    func_00216C00(entity, command);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216CF8);

/* Group setup record carried in the payload of an mdlRequestAsset job. */
typedef struct MdlGroupSetup {
    s32 unk0;   /* 0x0 */
    s32 unk4;   /* 0x4 */
    s32 unk8;   /* 0x8 */
    s32 flags;  /* 0xC */
    s32 unk10;  /* 0x10 */
    s32 unk14;  /* 0x14 */
    s32 unk18;  /* 0x18 */
    s32 unk1C;  /* 0x1C */
} MdlGroupSetup;

typedef struct MdlGroupEntity {
    u8 unk0[0xA0];
    void *unkA0;
    void *unkA4;
    void *unkA8;
} MdlGroupEntity;

extern void btlCreateGroupNode();

void mdlApplyGroupSetup(s32 group, s32 id, s32 mode, MdlGroupSetup *setup) {
    MdlGroupEntity *entity;

    btlCreateGroupNode(group, id, mode, setup->unk0, setup->unk4, setup->unk8);
    if (setup->flags != 0) {
        func_00216CF8(group, id, mode, 0, 0, 0, setup->flags, setup->unk10);
    }
    if (setup->unk14 != 0) {
        entity = btlFindGroupedEntity(group, id);
        entity->unkA4 = setup->unk14;
        entity->unkA0 = setup->unk18;
        entity->unkA8 = setup->unk1C;
    }
}

extern s32 btlGroupContainsId(s32 group, s32 id);
extern s32 fileManUpdate(void);

void *mdlWaitGroupThenFind(s32 group, s32 id) {
    while (btlGroupContainsId(group, id)) {
        fileManUpdate();
    }
    return btlFindGroupedEntity(group, id);
}

void mdlExecuteAndFreeJob(MdlPacket *packet) {
    mdlApplyGroupSetup(packet->unk0, packet->unk2, packet->unk8, packet->extra);
    WaitSema(D_003BD878);
    btlRemoveGroupId(packet->unk0, packet->unk2);
    SignalSema(D_003BD878);
    func_002CFF98(packet);
}

void func_00216F18(void *arg0, MdlLoadReq *req) {
    void *handle;
    u32 size;

    handle = func_00288B90();
    size = func_002EB090(handle);
    req->size = size;
    handle = fileGetResourceHandle(arg0);
    func_002D0918(handle);
    func_002887A0(arg0);
}

typedef struct MdlLoadCmd {
    u8 unk0[6];    /* 0x0 */
    u8 deferred;   /* 0x6: non-zero when the caller runs the job itself */
    u8 unk7[9];    /* 0x7 */
    u32 size;      /* 0x10 */
    u32 handle;    /* 0x14 */
} MdlLoadCmd;

extern s32 func_002EB168();

void func_00216F68(s32 arg0, MdlLoadCmd *cmd) {
    cmd->handle = fileGetResourceHandle(arg0);
    cmd->size = func_002EB168(func_00288B90(arg0));
    func_002887A0(arg0);
    if (cmd->deferred == 0) {
        mdlExecuteAndFreeJob((MdlPacket *)cmd);
    }
}

extern s32 func_002EB1F0();

typedef struct MdlLoadJob {
    u8 unk0[0x18]; /* 0x0 */
    u32 sizeWord;  /* 0x18 */
    u32 handle;    /* 0x1C */
} MdlLoadJob;

void func_00216FE0(s32 arg0, MdlLoadJob *job) {
    job->handle = fileGetResourceHandle(arg0);
    job->sizeWord = func_002EB1F0(func_00288B90(arg0));
    func_002887A0(arg0);
    mdlExecuteAndFreeJob((MdlPacket *)job);
}

char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_003BBB60;
    return strcat(dst, src);
}

INCLUDE_ASM(const s32, "model/mdlManager", mdlRequestAsset);

void func_00217298(u32 arg0, u32 arg1) {
    mdlRequestAsset(arg0, arg1, 1);
}

typedef struct MdlGroup {
    u8 unk0[0xC];
    u8 flag;
    u8 unkD[3];
    struct MdlLink *tail;
} MdlGroup;

typedef struct MdlLink {
    u8 unk0[4];
    struct MdlLink *prev;
    struct MdlLink *next;
    MdlGroup *group;
} MdlLink;

extern void btlDestroyGroupNode();

void func_002172B0(MdlLink *link) {
    MdlLink *prev = link->prev;
    MdlLink *next = link->next;
    MdlGroup *group;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    } else {
        group = link->group;
        if (prev != NULL) {
            group->tail = prev;
        } else {
            group->tail = NULL;
            if (group->flag != 0) {
                btlDestroyGroupNode(group);
            }
        }
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217310);

extern void sdfReleaseDevSlot(void *, s32, s32);

void mdlReleaseDevSlots(MdlCtx *ctx) {
    MdlDevList *list = ctx->devList;
    MdlDevSlot *node;
    MdlDevSlot *cur;

    if (list != NULL) {
        node = list->first;
        while (node != NULL) {
            cur = node;
            node = node->next;
            sdfReleaseDevSlot(cur->slot, 1, 1);
            func_002CFF98(cur);
        }
        func_002CFF98(list);
        ctx->devList = NULL;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002174C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217680);

extern void mdlDestroyResourceItem(u32 *);
extern void sdfResourceListRelease(u32, s32);

void mdlDestroyContext(MdlCtx *ctx) {
    MdlInner *inner = ctx->inner;
    u32 *node;
    u32 *next;

    while (inner->list != NULL) {
        func_002DB308(inner->list);
    }
    sdfResourceListRelease(inner->resourceHandle, 1);
    for (node = ctx->list14; node != NULL; node = next) {
        next = (u32 *)*node;
        mdlDestroyResourceItem(node);
    }
    mdlReleaseDevSlots(ctx);
    sdfReleaseDevSlot(inner, 1, 1);
    func_002172B0((MdlLink *)ctx);
    func_002CFF98(ctx);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217878);

INCLUDE_ASM(const s32, "model/mdlManager", func_002179A8);

void mdlEnableAllEntries(MdlCtx *ctx) {
    MdlEntryTable *table = ctx->inner->entries;
    s32 count = table->count;
    MdlEntry **items = table->items;
    s32 i;

    for (i = 0; i < count; i++) {
        items[i]->enabled = 1;
    }
}

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

MdlNode *mdlFindNodeById(MdlCtx *ctx, s32 id) {
    MdlNode *node;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        if (node->searchId == id) {
            return node;
        }
    }
    return NULL;
}

s32 mdlGetNodeField2C(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node == NULL) {
        return -1;
    }
    return node->unk2C;
}

s32 mdlGetNodeField2E(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return node->unk2E;
}

s32 mdlGetNodeInt1C(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return (s32)node->unk1C;
}

s32 mdlCheckNodeByte30(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node == NULL) {
        return 2;
    }
    return node->unk30 == 5;
}

f32 mdlGetNodeFloat20(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);
    f32 r = 0.0f;

    if (node != NULL) {
        r = node->value;
    }
    return r;
}

void mdlSetNodeFloat20(MdlCtx *ctx, s32 id, f32 value) {
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node != NULL) {
        node->value = value;
    }
}

/* These shims transfer vectors between model state and VU0 registers. */
void mdlLoadPrimaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector50;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void mdlStorePrimaryVectorVU(MdlCtx *ctx) {
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

void mdlLoadSecondaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector60;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

extern void effMiscQuaternionToMatrixVU(void);

/* Store vf10 as the secondary vector, then the rotation matrix rows built by the VU0 routine. */
void func_00217FB8(MdlCtx *ctx) {
    void *secondary;
    void *row0;
    void *row1;
    void *row2;

    secondary = &ctx->inner->vector60;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(secondary) : "memory");
    effMiscQuaternionToMatrixVU();
    row0 = &ctx->inner->vector20;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        ".set reorder"
        : : "r"(row0) : "memory");
    row1 = &ctx->inner->vector30;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf29, 0(%0)\n"
        ".set reorder"
        : : "r"(row1) : "memory");
    row2 = &ctx->inner->vector40;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf30, 0(%0)\n"
        ".set reorder"
        : : "r"(row2) : "memory");
}

void mdlLoadTertiaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

void mdlStoreTertiaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(vec) : "memory");
}

u32 mdlGetBroadcastValue(MdlCtx *ctx) {
    return ctx->inner->broadcastValue;
}

void mdlSetAllResourceFrames(MdlCtx *ctx, u32 value) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        mdlSetResourceFrame(ctx, node, value);
    }
}

void mdlBroadcastMasked(MdlCtx *ctx, u32 value) {
    ctx->inner->broadcastValue = value;
    mdlSetAllResourceFrames(ctx, (value & 0xFF000000) | 0x808080);
}

void mdlBroadcastValue(MdlCtx *ctx, u32 value) {
    ctx->inner->broadcastValue = value;
    mdlSetAllResourceFrames(ctx, value);
}

void func_00218100(MdlCtx *ctx, f32 amount) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        mdlSetResourceAmount(ctx, node, amount);
    }
}

/* vu0 routine: project `point` through the camera and the model's scaled matrix, result left in vf10 */
void func_00218158(MdlCtx *ctx, void *point)
{
    VU0_LOAD_MATRIX(D_003296F0);
    func_002DDD60(D_00324610);
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
    VU0_LOAD_MATRIX(&ctx->inner->vector20);
    VU0_SCALE_MATRIX_ROWS(vf10);
    func_002DDC50();
    VU0_LOAD_VF(vf10, point);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_00324650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
}

/* Project `count` points through the model's scaled matrix and the camera. */
void func_00218228(MdlCtx *ctx, f32 (*in)[4], f32 (*out)[4], s32 count)
{
    s32 i;

    VU0_LOAD_MATRIX(&ctx->inner->vector20);
    VU0_LOAD_VF(vf10, &ctx->inner->vector70);
    VU0_SCALE_MATRIX_ROWS(vf10);
    func_002DDD60(D_003296F0);
    func_002DDD60(D_00324610);
    for (i = 0; i < count; i++) {
        VU0_LOAD_VF(vf10, in[i]);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_PERSPECTIVE_DIVIDE_VF10();
        VU0_LOAD_VF(vf11, D_00324610 + 0x40);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, D_00324610 + 0x50);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out[i]);
    }
}

void func_00218320(MdlCtx *ctx) {
    MdlNode *node;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        sdfMotionSuspend(node);
    }
}

void func_00218368(MdlCtx *ctx) {
    MdlNode *node;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        sdfMotionResume(node);
    }
}

u8 mdlHasNode(MdlCtx *ctx, s32 id) {
    MdlNode *found;

    found = mdlFindNodeById(ctx, id);
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
    MdlNode *node = mdlFindNodeById(ctx, id);

    if (node == NULL) {
        return 0;
    }
    return *(u16 *)node->unk8;
}

void mdlReleaseInnerResourceHandle(MdlCtx *ctx) {
    func_002DA1B0(ctx->inner->resourceHandle);
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

/* Completion job created by mdlRequestLoadWithCallback and run by func_00218A08. */
typedef struct MdlDoneJob {
    u16 group;         /* 0x0 */
    u16 id;            /* 0x2 */
    u32 arg;           /* 0x4 */
    void *owner;       /* 0x8: request slot from func_002889D8 */
    void (*done)(u32); /* 0xC */
    u32 doneArg;       /* 0x10 */
} MdlDoneJob;

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A08);

extern void *func_002CFF68();
extern s32 func_002889D8();
extern void func_00288C50();
extern void func_00218A08();

s32 mdlRequestLoadWithCallback(s32 group, s32 id, s32 arg, s32 handle, void (*done)(u32), u32 doneArg) {
    MdlDoneJob *job = func_002CFF68(0x14);
    s32 slot;

    job->group = group;
    job->id = id;
    job->arg = arg;
    job->doneArg = doneArg;
    job->done = done;
    slot = func_002889D8(handle, 0, 0, func_00218A08, job);
    job->owner = (void *)slot;
    if (done == NULL) {
        func_00288C50(slot);
        func_002189D8((MdlRes *)job);
    }
    return 0;
}

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB60);

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB68);

