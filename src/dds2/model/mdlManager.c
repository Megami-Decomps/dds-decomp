#include "common.h"
#include "pcp_vu0.h"

extern u64 fileGetResourceHandle(u64);

extern u64 func_002C8110();

extern u32 func_00343F38(u64);

extern u32 D_00438F90;

extern void *btlFindGroupedEntity();

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by func_002183D0/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

typedef struct MdlEntry {
    u8 unk0[0x14]; /* 0x0 */
    s16 enabled;   /* 0x14 */
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

extern void sdfDestroyMotion(void *arg);

extern s32 btlGroupContainsId(s32 group, s32 id);

extern s32 fileManUpdate(void);

void mdlClearSlotAndRelease(void *ctx, MdlNode *node) {
    s32 offset = node->slotIndex * 4 + 0x20;
    void **slot = (void **)((u8 *)ctx + offset);

    if (*slot == node) {
        *slot = NULL;
    }
    sdfDestroyMotion(node);
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

INCLUDE_ASM(const s32, "model/mdlManager", func_00231718);

void func_002317E0(u64 unused0, u64 unused1, u64 command) {
    u64 entity;

    entity = btlFindGroupedEntity();
    func_00231718(entity, command);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00231810);

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

extern void func_00231810();

void mdlApplyGroupSetup(s32 group, s32 id, s32 mode, MdlGroupSetup *setup) {
    MdlGroupEntity *entity;

    btlCreateGroupNode(group, id, mode, setup->unk0, setup->unk4, setup->unk8);
    if (setup->flags != 0) {
        func_00231810(group, id, mode, 0, 0, 0, setup->flags, setup->unk10);
    }
    if (setup->unk14 != 0) {
        entity = btlFindGroupedEntity(group, id);
        entity->unkA4 = setup->unk14;
        entity->unkA0 = setup->unk18;
        entity->unkA8 = setup->unk1C;
    }
}

void *mdlWaitGroupThenFind(s32 group, s32 id) {
    while (btlGroupContainsId(group, id)) {
        fileManUpdate();
    }
    return btlFindGroupedEntity(group, id);
}

void mdlExecuteAndFreeJob(u32 job) {
    u16 *words;

    words = (u16 *)job;
    mdlApplyGroupSetup(*words, words[1], *(u32 *)(words + 4), words + 6);
    WaitSema(D_00438F90);
    btlRemoveGroupId(*words, words[1]);
    SignalSema(D_00438F90);
    func_00328E48(job);
}

void func_00231A30(u64 resource, s32 destination) {
    u64 handle;
    u32 resolved;

    handle = func_002C8110();
    resolved = func_00343F38(handle);
    *(u32 *)(destination + 0xc) = resolved;
    handle = fileGetResourceHandle(resource);
    func_003297C8(handle);
    func_002C7D00(resource);
}
