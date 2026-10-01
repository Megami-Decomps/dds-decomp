#include "common.h"
#include "pcp_vu0.h"

extern u8 D_003846F0[];

extern u8 D_0037F610[];

extern u8 D_0037F650[];

extern u8 D_0037F660[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *);

extern void sdfMultiplyVuMatrixInPlace(void);

extern struct MdlNode *mdlFindNodeById();

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by func_002183D0/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

typedef struct MdlEntry {
    u8 unk0[0x14]; /* 0x0 */
    s16 enabled;   /* 0x14 */
    u8 pad16[0x6A];
    f32 row0[3];   /* 0x80: basis rows eased by func_002324C0 */
    u8 pad8C[4];
    f32 row1[3];   /* 0x90 */
    u8 pad9C[4];
    f32 row2[3];   /* 0xA0 */
} MdlEntry;

/* Entry table pointed to by the first word of MdlInner. */
typedef struct MdlEntryTable {
    u8 unk0[4];       /* 0x0 */
    s16 count;        /* 0x4 */
    u8 unk6[6];       /* 0x6 */
    MdlEntry **items; /* 0xC */
} MdlEntryTable;

/* Record behind MdlCtx.inner. */
typedef struct MdlInner {
    MdlEntryTable *entries; /* 0x0 */
    u8 unk4[4];  /* 0x4 */
    u32 resourceHandle; /* 0x8: released by mdlReleaseInnerResourceHandle */
    u8 unkC[8];  /* 0xC */
    struct MdlNode *list; /* 0x14: intrusive node list */
    u8 unk18;    /* 0x18 */
    u8 flags19;  /* 0x19: bit 0x10 enables anchor dispatch */
    u8 unk1A[2]; /* 0x1A */
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
    u32 flags;         /* 0x0: 1 = skip update, 2 = skip anchors, 4 = needs inner flag 0x10 */
    u8 unk4[8];        /* 0x4 */
    MdlSub *sub;       /* 0xC */
    union {
        u32 word;      /* 0x10: low byte read by mdlIsInnerSentinel */
        struct {
            s16 id;    /* 0x10 */
            s16 arg;   /* 0x12 */
        } h;
    } current;
    u32 *list14;       /* 0x14: intrusive list walked by mdlSetAllResourceFrames */
    MdlInner *inner;   /* 0x18 */
    struct MdlNode *first;    /* 0x1C */
    struct MdlNode *slots[4]; /* 0x20 */
    struct MdlDevList *devList; /* 0x30: device slots released with the model */
} MdlCtx;

typedef struct MdlDevSlot {
    struct MdlDevSlot *next; /* 0x0 */
    void *slot;              /* 0x4 */
} MdlDevSlot;

typedef struct MdlDevList {
    MdlDevSlot *first; /* 0x0 */
} MdlDevList;

/* Entry searched by func_00217E10/func_00216BB0 on its s16 id at +0x28.
 * Only the fields read by the matched helpers below are known. */
typedef struct MdlNode {
    struct MdlNode *next; /* 0x0 */
    u8 pad4[4];           /* 0x4 */
    void *unk8;           /* 0x8: dereferenced by func_00218410 */
    u8 padC[0x10];        /* 0xC */
    f32 unk1C;            /* 0x1C: read as int by func_00217EB0 */
    f32 floatValue;       /* 0x20: accessed as a float by mdlGet/SetNodeFloat20 */
    u8 pad24[4];          /* 0x24 */
    s16 searchId;          /* 0x28: identifies a node in list lookups */
    s16 slotIndex;         /* 0x2A: slot index used by func_00216B78 */
    u16 unk2C;            /* 0x2C */
    u16 unk2E;            /* 0x2E */
    u8 unk30;             /* 0x30: compared against 5 */
    u8 pad31[7];          /* 0x31 */
} MdlNode;

extern u32 D_003C86B4[][2];

extern void sdfDestroyMotion(void *arg);

/* 8-byte prefix copied from D_003BBB60 by mdlBuildPrefixedString. */
typedef struct Hdr8 {
    u8 b[8];
} Hdr8;

extern u8 D_00436FA0[];

extern char *strcat(char *dst, const char *src);

void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 id, s32 mode, s32 flag, f32 scaleX, f32 scaleY);

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

extern u32 sdfRelocatePackedResourcePayload(u64);

typedef struct MdlLoadCmd {
    u8 unk0[6];    /* 0x0 */
    u8 deferred;   /* 0x6: non-zero when the caller runs the job itself */
    u8 unk7[9];    /* 0x7 */
    u32 size;      /* 0x10 */
    u32 handle;    /* 0x14 */
} MdlLoadCmd;

extern u32 fileGetResourceHandle(u32);
extern u64 func_002C8110();
extern void filePollEntryCleanup();
extern void mdlExecuteAndFreeJob(void *job);

void mdlFinishLoadCmd(s32 entryId, MdlLoadCmd *cmd) {
    cmd->handle = fileGetResourceHandle(entryId);
    cmd->size = sdfRelocatePackedResourcePayload(func_002C8110(entryId));
    filePollEntryCleanup(entryId);
    if (cmd->deferred == 0) {
        mdlExecuteAndFreeJob(cmd);
    }
}

extern u32 sdfRelocatePackedResourceWordsFromHeader(u64);

typedef struct MdlLoadJob {
    u8 unk0[0x18]; /* 0x0 */
    u32 sizeWord;  /* 0x18 */
    u32 handle;    /* 0x1C */
} MdlLoadJob;

void mdlFinishLoadJob(s32 entryId, MdlLoadJob *job) {
    job->handle = fileGetResourceHandle(entryId);
    job->sizeWord = sdfRelocatePackedResourceWordsFromHeader(func_002C8110(entryId));
    filePollEntryCleanup(entryId);
    mdlExecuteAndFreeJob(job);
}

char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_00436FA0;
    return strcat(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00231A80", mdlRequestAsset);

void func_00231DB0(u32 arg0, u32 arg1) {
    mdlRequestAsset(arg0, arg1, 1);
}

void mdlUnlinkGroupEntry(MdlLink *link) {
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

INCLUDE_ASM(const s32, "game/code_00231A80", func_00231E28);

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
            sdfReleaseChipBlock(cur);
        }
        sdfReleaseChipBlock(list);
        ctx->devList = NULL;
    }
}

INCLUDE_ASM(const s32, "game/code_00231A80", func_00231FD8);

INCLUDE_ASM(const s32, "game/code_00231A80", func_00232198);

extern void mdlDestroyResourceItem(u32 *);

extern void sdfResourceListRelease(u32, s32);

void mdlDestroyContext(MdlCtx *ctx) {
    MdlInner *inner = ctx->inner;
    u32 *node;
    u32 *next;

    while (inner->list != NULL) {
        sdfDestroyMotion(inner->list);
    }
    sdfResourceListRelease(inner->resourceHandle, 1);
    for (node = ctx->list14; node != NULL; node = next) {
        next = (u32 *)*node;
        mdlDestroyResourceItem(node);
    }
    mdlReleaseDevSlots(ctx);
    sdfReleaseDevSlot(inner, 1, 1);
    mdlUnlinkGroupEntry((MdlLink *)ctx);
    sdfReleaseChipBlock(ctx);
}

extern void func_00231FD8();
extern void func_00334510();
extern void sdfModelUpdateCurrentFrameTransforms();
extern void func_003320E8();
extern void mdlDispatchViewerAnchorRecord();

/* Per-frame update: step the active slot nodes, refresh the transforms, dispatch anchor records. */
void func_00232390(MdlCtx *ctx, s32 arg) {
    MdlNode **slot = ctx->slots;
    MdlInner *inner;
    u32 *rec;
    s32 i;

    for (i = 0; i != 4; i++) {
        if (*slot != NULL) {
            if ((*slot)->unk30 != 0) {
                func_00334510(*slot);
            }
        }
        slot++;
    }
    if (ctx->flags & 1) {
        return;
    }
    inner = ctx->inner;
    sdfModelUpdateCurrentFrameTransforms(inner);
    func_003320E8(arg, inner);
    if (ctx->flags & 2) {
        return;
    }
    if (ctx->flags & 4) {
        if ((inner->flags19 & 0x10) == 0) {
            return;
        }
    }
    for (rec = ctx->list14; rec != NULL; rec = (u32 *)*rec) {
        mdlDispatchViewerAnchorRecord(ctx, rec);
    }
    if (ctx->devList == NULL) {
        return;
    }
    func_00231FD8(ctx, arg);
}

extern void sdfSetPrimaryIdentityMatrixVU(void *);
extern void sdfRotateVuMatrixAboutX(f32 angle);
extern void sdfRotateVuMatrixAboutY(f32 angle);

/* Same update as func_00232390, first easing the entry `index` towards a pitch/yaw rotation (degrees). */
void func_002324C0(MdlCtx *ctx, s32 arg, s32 index, f32 pitch, f32 yaw) {
    MdlInner *inner;
    MdlEntry *entry = NULL;
    f32 rows[4][4];
    f32 amount;
    f32 weight;
    f32 keep;
    u32 *rec;
    s32 i;

    inner = ctx->inner;
    if (index != -1) {
        entry = inner->entries->items[index];
        sdfSetPrimaryIdentityMatrixVU(inner->entries);
        sdfRotateVuMatrixAboutX(pitch * 0.017453293f);
        sdfRotateVuMatrixAboutY(yaw * 0.017453293f);
        VU0_STORE_MATRIX(rows);
    }
    for (i = 0; i != 4; i++) {
        if (ctx->slots[i] != NULL) {
            if (ctx->slots[i]->unk30 != 0) {
                func_00334510(ctx->slots[i]);
            }
        }
    }
    amount = pitch;
    if (index != -1) {
        if (amount < 0.0f) {
            amount = -amount;
        }
        weight = 1.0f;
        if (amount < 25.0f) {
            weight = amount / 25.0f;
        }
        keep = 1.0f - weight;
        entry->row0[0] = rows[0][0] * weight + entry->row0[0] * keep;
        entry->row0[1] = rows[0][1] * weight + entry->row0[1] * keep;
        entry->row0[2] = rows[0][2] * weight + entry->row0[2] * keep;
        entry->row1[0] = rows[1][0] * weight + entry->row1[0] * keep;
        entry->row1[1] = rows[1][1] * weight + entry->row1[1] * keep;
        entry->row1[2] = rows[1][2] * weight + entry->row1[2] * keep;
        entry->row2[0] = rows[2][0] * weight + entry->row2[0] * keep;
        entry->row2[1] = rows[2][1] * weight + entry->row2[1] * keep;
        entry->row2[2] = rows[2][2] * weight + entry->row2[2] * keep;
    }
    if (ctx->flags & 1) {
        return;
    }
    inner = ctx->inner;
    sdfModelUpdateCurrentFrameTransforms(inner);
    func_003320E8(arg, inner);
    if (ctx->flags & 2) {
        return;
    }
    if (ctx->flags & 4) {
        if ((inner->flags19 & 0x10) == 0) {
            return;
        }
    }
    for (rec = ctx->list14; rec != NULL; rec = (u32 *)*rec) {
        mdlDispatchViewerAnchorRecord(ctx, rec);
    }
    if (ctx->devList == NULL) {
        return;
    }
    func_00231FD8(ctx, arg);
}

void mdlEnableAllEntries(MdlCtx *ctx) {
    MdlEntryTable *table = ctx->inner->entries;
    s32 count = table->count;
    MdlEntry **items = table->items;
    s32 i;

    for (i = 0; i < count; i++) {
        items[i]->enabled = 1;
    }
}

extern struct MdlNode *motionOwnerCreateObjectForRecord(MdlCtx *, s32);
extern void func_00334280(struct MdlNode *, s32, s32, f32, f32);
extern void mdlRemoveResourceSubtype(MdlCtx *, s32);
extern void mdlApplyResourceEntries(MdlCtx *, s32, s32);

/* Select (or create) the node for `id`, make it the current node of its slot
 * and apply its resource entries. */
void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 id, s32 mode, s32 flag, f32 scaleX, f32 scaleY) {
    MdlNode *node;
    s16 slot;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        if (node->searchId == id) {
            break;
        }
    }
    if (node == NULL) {
        node = motionOwnerCreateObjectForRecord(ctx, id);
    }
    slot = node->slotIndex;
    ctx->slots[slot] = node;
    if (slot == 0) {
        ctx->first = node;
    }
    func_00334280(node, mode, flag, scaleX, scaleY);
    ctx->current.h.id = id;
    ctx->current.h.arg = mode;
    mdlRemoveResourceSubtype(ctx, slot);
    mdlApplyResourceEntries(ctx, mode, slot);
}

void mdlAddEntryFlagged(MdlCtx *ctx, s32 searchId, s32 mode) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, mode, 1, 0.0f, 0.0f);
}

void mdlAddEntryPlain(MdlCtx *ctx, s32 searchId, s32 mode) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, mode, 0, 0.0f, 0.0f);
}

void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 searchId, s32 mode, f32 scaleX, f32 scaleY) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, mode, 1, scaleX, scaleY);
}

void mdlAddEntryPlainEx(MdlCtx *ctx, s32 searchId, s32 mode, f32 scaleX, f32 scaleY) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, mode, 0, scaleX, scaleY);
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
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return -1;
    }
    return node->unk2C;
}

u16 mdlGetNodeField2E(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return 0;
    }
    return node->unk2E;
}

s32 mdlGetNodeInt1C(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return 0;
    }
    return (s32)node->unk1C;
}

s32 mdlCheckNodeByte30(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return 2;
    }
    return node->unk30 == 5;
}

f32 mdlGetNodeFloat20(MdlCtx *ctx, s32 id) {
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return 0.0f;
    }
    return node->floatValue;
}

void mdlSetNodeFloat20(MdlCtx *ctx, s32 id, f32 value) {
    MdlNode *node = (MdlNode *)mdlFindNodeById(ctx, id);
    if (node != NULL) {
        node->floatValue = value;
    }
}

/* These shims transfer vectors between model state and VU0 registers. */
void mdlLoadPrimaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector50;
    VU0_LOAD_VF_MEMORY(vf10, vec);
}

void mdlStorePrimaryVectorVU(MdlCtx *ctx) {
    void *vec;
    VU0_SET_W_ONE(vf10);
    vec = &ctx->inner->vector50;
    VU0_STORE_VF(vf10, vec);
}

void mdlLoadSecondaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector60;
    VU0_LOAD_VF_MEMORY(vf10, vec);
}

extern void effMiscQuaternionToMatrixVU(void);

/* Store vf10 as the secondary vector, then the rotation matrix rows built by the VU0 routine. */
void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *ctx) {
    void *secondary;
    void *row0;
    void *row1;
    void *row2;

    secondary = &ctx->inner->vector60;
    VU0_STORE_VF(vf10, secondary);
    effMiscQuaternionToMatrixVU();
    row0 = &ctx->inner->vector20;
    VU0_STORE_VF(vf28, row0);
    row1 = &ctx->inner->vector30;
    VU0_STORE_VF(vf29, row1);
    row2 = &ctx->inner->vector40;
    VU0_STORE_VF(vf30, row2);
}

void mdlLoadTertiaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    VU0_LOAD_VF_MEMORY(vf10, vec);
}

void mdlStoreTertiaryVectorVU(MdlCtx *ctx) {
    void *vec = &ctx->inner->vector70;
    VU0_STORE_VF(vf10, vec);
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

extern void mdlSetResourceAmount(MdlCtx *ctx, u32 *node, f32 amount);

void mdlSetAmountOnAllContextResources(MdlCtx *ctx, f32 amount) {
    u32 *node;

    for (node = ctx->list14; node != NULL; node = (u32 *)*node) {
        mdlSetResourceAmount(ctx, node, amount);
    }
}

/* vu0 routine: project `point` through the camera and the model's scaled matrix, result left in vf10 */
void mdlProjectPointVU(MdlCtx *ctx, void *point)
{
    VU0_LOAD_MATRIX(D_003846F0);
    sdfPostmultiplyVuMatrixFromMemory(D_0037F610);
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
    VU0_LOAD_MATRIX(&ctx->inner->vector20);
    VU0_SCALE_MATRIX_ROWS(vf10);
    sdfMultiplyVuMatrixInPlace();
    VU0_LOAD_VF(vf10, point);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
}

/* Project `count` points through the model's scaled matrix and the camera. */
void mdlProjectPoints(MdlCtx *ctx, f32 (*in)[4], f32 (*out)[4], s32 count)
{
    s32 i;

    VU0_LOAD_MATRIX(&ctx->inner->vector20);
    VU0_LOAD_VF(vf10, &ctx->inner->vector70);
    VU0_SCALE_MATRIX_ROWS(vf10);
    sdfPostmultiplyVuMatrixFromMemory(D_003846F0);
    sdfPostmultiplyVuMatrixFromMemory(D_0037F610);
    for (i = 0; i < count; i++) {
        VU0_LOAD_VF(vf10, in[i]);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_PERSPECTIVE_DIVIDE_VF10();
        VU0_LOAD_VF(vf11, D_0037F610 + 0x40);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, D_0037F610 + 0x50);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out[i]);
    }
}

void mdlSuspendAllContextMotions(MdlCtx *ctx) {
    MdlNode *node;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        sdfMotionSuspend(node);
    }
}

void mdlResumeAllContextMotions(MdlCtx *ctx) {
    MdlNode *node;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        sdfMotionResume(node);
    }
}

u8 mdlHasNode(MdlCtx *ctx, s32 id) {
    s64 foundNode;

    foundNode = mdlFindNodeById(ctx, id);
    return foundNode != 0;
}

u16 func_00232EE8(MdlCtx *ctx) {
    return ctx->sub->unk8;
}

u16 func_00232EF8(MdlCtx *ctx) {
    return ctx->sub->unkA;
}

u32 func_00232F08(void) {
    return 8;
}

u32 mdlGetTableWord(s32 idx) {
    return D_003C86B4[idx][0];
}

u16 mdlGetNodeRefHalf(MdlCtx *ctx, s32 id) {
    MdlNode *node = mdlFindNodeById(ctx, id);
    if (node == NULL) {
        return 0;
    }
    return *(u16 *)node->unk8;
}

void mdlReleaseInnerResourceHandle(MdlCtx *ctx) {
    sdfUpdateActiveResourceListScalars(ctx->inner->resourceHandle);
}

INCLUDE_ASM(const s32, "game/code_00231A80", func_00232F78);

s32 mdlIsInnerSentinel(MdlCtx *ctx) {
    s32 r = 0;

    if ((u8)ctx->current.word == 1) {
        r = ctx->inner == (MdlInner *)0x30424950;
    }
    return r;
}

INCLUDE_ASM(const s32, "game/code_00231A80", func_002330C8);

INCLUDE_ASM(const s32, "game/code_00231A80", func_00233280);

void mdlDestroyLoadRequestOwner(u32 arg0) {
    func_002C7CE8(*(u32 *)((s32)arg0 + 8));
    sdfReleaseChipBlock(arg0);
}

/* Completion job created by mdlRequestLoadWithCallback and run by mdlCompleteGroupedJobAndNotify. */
typedef struct MdlDoneJob {
    u16 group;         /* 0x0 */
    u16 id;            /* 0x2 */
    u32 arg;           /* 0x4 */
    void *owner;       /* 0x8: request slot from func_002C7F38 */
    void (*done)(u32); /* 0xC */
    u32 doneArg;       /* 0x10 */
} MdlDoneJob;

/* Request slot handed back by func_002C7F38; its +0x60 word feeds the load apply. */
typedef struct MdlLoadSlot {
    u8 pad00[0x60];
    u32 handle; /* 0x60 */
} MdlLoadSlot;

extern u32 D_00438F90;
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void btlRemoveGroupId(s32, s32);
extern s32 func_00233280();

/* Run a completed load job: apply it, drop its group id, then call its done callback and free it. */
void mdlCompleteGroupedJobAndNotify(MdlLoadSlot *owner, MdlDoneJob *job) {
    job->owner = owner;
    func_00233280(owner->handle, job->group, job->id, job->arg);
    WaitSema(D_00438F90);
    btlRemoveGroupId(job->group, job->id);
    SignalSema(D_00438F90);
    if (job->done != NULL) {
        job->done(job->doneArg);
        mdlDestroyLoadRequestOwner((u32)job);
    }
}

extern void *sdfAllocAndClearQuadwords();

extern s32 func_002C7F38();

extern void func_002C81D0();

extern void mdlCompleteGroupedJobAndNotify();

s32 mdlRequestLoadWithCallback(s32 group, s32 id, s32 arg, s32 handle, void (*done)(u32), u32 doneArg) {
    MdlDoneJob *job = sdfAllocAndClearQuadwords(0x14);
    s32 slot;

    job->group = group;
    job->id = id;
    job->arg = arg;
    job->doneArg = doneArg;
    job->done = done;
    slot = func_002C7F38(handle, 0, 0, mdlCompleteGroupedJobAndNotify, job);
    job->owner = slot;
    if (done == NULL) {
        func_002C81D0(slot);
        mdlDestroyLoadRequestOwner((u32)job);
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_00231A80", D_00436FA0);

INCLUDE_SDATA(const s32, "game/code_00231A80", D_00436FA8);

