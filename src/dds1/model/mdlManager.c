#include "ee_mmi.h"
#include "common.h"
#include "pcp_vu0.h"

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_00324650[];
extern u8 D_00324660[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);
extern void sdfMultiplyVuMatrixInPlace(void);

/* Sub-record behind MdlCtx.sub (+0x8/+0xA read by mdlGetContextResourceGroup/E0). */
typedef struct MdlSub {
    u8 unk0[8]; /* 0x0 */
    u16 unk8;   /* 0x8 */
    u16 unkA;   /* 0xA */
} MdlSub;

/* Entry enabled by mdlEnableAllEntries. */
typedef struct MdlEntry {
    u8 unk0[0x14]; /* 0x0 */
    s16 enabled;   /* 0x14 */
    u8 pad16[0x6A];
    f32 row0[3];   /* 0x80: basis rows eased by mdlBlendEntryPitchYawAndUpdate */
    u8 pad8C[4];
    f32 row1[3];   /* 0x90 */
    u8 pad9C[4];
    f32 row2[3];   /* 0xA0 */
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
    u32 resourceHandle; /* 0x8: released through sdfUpdateActiveResourceListScalars */
    u8 unkC[8];  /* 0xC */
    MdlNode *list; /* 0x14: intrusive list walked by mdlSuspendAllContextMotions/368 */
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
    struct MdlCtx *next; /* 0x4: link in the owner's context list */
    u8 unk8[4];        /* 0x8 */
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
    MdlNode *first;    /* 0x1C */
    MdlNode *slots[4]; /* 0x20 */
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

/* Load request touched by mdlRecordLoadedSizeAndReleaseHandle. */
typedef struct MdlLoadReq {
    u8 unk0[0xC]; /* 0x0 */
    u32 size;      /* 0xC: size read from the current file resource */
} MdlLoadReq;

/* Resource released by mdlDestroyLoadRequestOwner. */
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
    f32 unk1C;            /* 0x1C: numerically converted to s32 by mdlGetNodeInt1C */
    f32 floatValue;       /* 0x20: accessed as a float by mdlGet/SetNodeFloat20 */
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
extern void sdfDestroyMotion(void *arg);
extern char *strcat(char *dst, const char *src);

MdlNode *mdlFindNodeById(MdlCtx *ctx, s32 id);
void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames);

extern void *fileGetLoadedDataAddress();
extern u32 sndBuildResourceHandleListFromOffsets(void *);
extern void mdlSetResourceAmount(MdlCtx *ctx, u32 *node, f32 amount);

extern u32 mdlGroupJobSemaphore;

extern void *btlFindGroupedEntity();

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

typedef struct MdlSlot {
    u32 flags;      /* 0x0: 1 = bit 0x100 of the request mode, 2 = bit 0x200 */
    s16 value4;     /* 0x4 */
    s16 value6;     /* 0x6 */
    u32 first;      /* 0x8 */
    u32 resource;   /* 0xC: released through sdfReleaseResourceAllocation */
} MdlSlot;

typedef struct MdlSlotOwner {
    u8 pad00[0xC];
    u8 hasResources;    /* 0x0C */
    u8 pad0D[3];
    MdlCtx *contexts;   /* 0x10 */
    u8 pad14[0xC];
    MdlSlot slots[1];   /* 0x20 */
} MdlSlotOwner;

extern void sdfReleaseResourceAllocation();

/* Release slot `index`: destroy its motions in every context and free the attached resource. */
void mdlReleaseOwnerSlotResources(MdlSlotOwner *owner, s32 index) {
    MdlCtx *ctx;

    if (owner != NULL) {
        if (owner->slots[index].first == 0) {
            return;
        }
        for (ctx = owner->contexts; ctx != NULL; ctx = ctx->next) {
            mdlReleaseFirstMatch(ctx, index);
        }
        if (owner->hasResources != 0) {
            if (owner->slots[index].resource != 0) {
                sdfReleaseResourceAllocation(owner->slots[index].resource);
            }
        }
        owner->slots[index].first = 0;
        owner->slots[index].resource = 0;
    }
}

void mdlApplyCommandToGroupedEntity(void *unused0, void *unused1, void *command) {
    void *entity;

    /* Only the grouped entity and command are forwarded to the worker. */
    entity = btlFindGroupedEntity();
    mdlReleaseOwnerSlotResources(entity, command);
}

void mdlConfigureGroupedEntitySlot(s32 group, s32 id, u32 mode, s32 value6, s32 index, s32 value4, u32 first, u32 resource) {
    MdlSlotOwner *owner = btlFindGroupedEntity(group, id);
    MdlSlot *slot;

    mdlReleaseOwnerSlotResources(owner, index);
    slot = &owner->slots[index];
    slot->value4 = value4;
    slot->value6 = value6;
    slot->first = first;
    slot->resource = resource;
    slot->flags = 0;
    if (mode & 0x100) {
        slot->flags = 1;
    }
    if (mode & 0x200) {
        slot->flags |= 2;
    }
}

/* Group setup record carried in the payload of an mdlRequestAsset job. */
typedef struct MdlGroupSetup {
    s32 resourceList;  /* 0x0: 4th arg of btlCreateGroupNode */
    s32 unk4;          /* 0x4: 5th arg of btlCreateGroupNode */
    s32 requestHandle; /* 0x8: 6th arg of btlCreateGroupNode */
    s32 flags;         /* 0xC */
    s32 resource;      /* 0x10: resource of mdlConfigureGroupedEntitySlot */
    s32 handleA;       /* 0x14: stored to MdlGroupEntity +0xA4 */
    s32 handleB;       /* 0x18: stored to MdlGroupEntity +0xA0 */
    s32 handleC;       /* 0x1C: stored to MdlGroupEntity +0xA8 */
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

    btlCreateGroupNode(group, id, mode, setup->resourceList, setup->unk4, setup->requestHandle);
    if (setup->flags != 0) {
        mdlConfigureGroupedEntitySlot(group, id, mode, 0, 0, 0, setup->flags, setup->resource);
    }
    if (setup->handleA != 0) {
        entity = btlFindGroupedEntity(group, id);
        entity->unkA4 = setup->handleA;
        entity->unkA0 = setup->handleB;
        entity->unkA8 = setup->handleC;
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
    WaitSema(mdlGroupJobSemaphore);
    btlRemoveGroupId(packet->unk0, packet->unk2);
    SignalSema(mdlGroupJobSemaphore);
    sdfReleaseChipBlock(packet);
}

void mdlRecordLoadedSizeAndReleaseHandle(void *resource, MdlLoadReq *destination) {
    void *handle;
    u32 size;

    handle = fileGetLoadedDataAddress();
    size = sndBuildResourceHandleListFromOffsets(handle);
    destination->size = size;
    handle = fileGetResourceHandle(resource);
    sdfReleaseResourceAllocation(handle);
    filePollEntryCleanup(resource);
}

typedef struct MdlLoadCmd {
    u8 unk0[6];    /* 0x0 */
    u8 deferred;   /* 0x6: non-zero when the caller runs the job itself */
    u8 unk7[9];    /* 0x7 */
    u32 size;      /* 0x10 */
    u32 handle;    /* 0x14 */
} MdlLoadCmd;

extern s32 sdfRelocatePackedResourcePayload();

/* Retain the resource handle, relocate the loaded payload and retire the file
 * entry. Execute the group job now only when the command is not deferred. */
void mdlFinishLoadCmd(s32 entryId, MdlLoadCmd *cmd) {
    cmd->handle = fileGetResourceHandle(entryId);
    cmd->size = sdfRelocatePackedResourcePayload(fileGetLoadedDataAddress(entryId));
    filePollEntryCleanup(entryId);
    if (cmd->deferred == 0) {
        mdlExecuteAndFreeJob((MdlPacket *)cmd);
    }
}

extern s32 sdfRelocatePackedResourceWordsFromHeader();

typedef struct MdlLoadJob {
    u8 unk0[0x18]; /* 0x0 */
    u32 sizeWord;  /* 0x18 */
    u32 handle;    /* 0x1C */
} MdlLoadJob;

/* Retain the handle and relocated size word, retire the file entry, then run
 * the group job. Unlike mdlFinishLoadCmd, this path has no deferred flag. */
void mdlFinishLoadJob(s32 entryId, MdlLoadJob *job) {
    job->handle = fileGetResourceHandle(entryId);
    job->sizeWord = sdfRelocatePackedResourceWordsFromHeader(fileGetLoadedDataAddress(entryId));
    filePollEntryCleanup(entryId);
    mdlExecuteAndFreeJob((MdlPacket *)job);
}

/* Copy the fixed eight-byte prefix, then append src. Prefix termination and
 * sufficient destination capacity are obligations of the data/caller. */
char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_003BBB60;
    return strcat(dst, src);
}

extern void *mdlRequestAsset(u32 group, u32 id, u32 option);

INCLUDE_ASM(const s32, "model/mdlManager", mdlRequestAsset);

/* Request the group/id asset with option 1; that option's meaning is not
 * established by this forwarding body. */
void *func_00217298(u32 group, u32 id) {
    return mdlRequestAsset(group, id, 1);
}

typedef struct MdlGroup {
    u8 unk0[0xC];
    u8 destroyWhenEmpty;
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

/* Unlink without clearing this entry's own links. Update the group tail and
 * destroy the group only when its last entry leaves and destroyWhenEmpty is set. */
void mdlUnlinkGroupEntry(MdlLink *link) {
    MdlLink *previousEntry = link->prev;
    MdlLink *nextEntry = link->next;
    MdlGroup *group;

    if (previousEntry != NULL) {
        previousEntry->next = nextEntry;
    }
    if (nextEntry != NULL) {
        nextEntry->prev = previousEntry;
    } else {
        group = link->group;
        if (previousEntry != NULL) {
            group->tail = previousEntry;
        } else {
            group->tail = NULL;
            if (group->destroyWhenEmpty != 0) {
                btlDestroyGroupNode(group);
            }
        }
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00217310);

extern void sdfReleaseDevSlot(void *, s32, s32);

/* Release every device slot and its list node, then the list owner. A missing
 * list is a no-op; a released list is removed from the context. */
void mdlReleaseDevSlots(MdlCtx *ctx) {
    MdlDevList *deviceList = ctx->devList;
    MdlDevSlot *nextSlot;
    MdlDevSlot *currentSlot;

    if (deviceList != NULL) {
        nextSlot = deviceList->first;
        while (nextSlot != NULL) {
            currentSlot = nextSlot;
            nextSlot = nextSlot->next;
            sdfReleaseDevSlot(currentSlot->slot, 1, 1);
            sdfReleaseChipBlock(currentSlot);
        }
        sdfReleaseChipBlock(deviceList);
        ctx->devList = NULL;
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002174C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217680);

extern void mdlDestroyResourceItem(u32 *);
extern void sdfResourceListRelease(u32, s32);

/* Destroy motions, resources, device slots and the context itself. Motion
 * destruction must unlink inner->list; save each resource's next link before
 * destroying it. ctx and inner are required, not checked here. */
void mdlDestroyContext(MdlCtx *ctx) {
    MdlInner *inner = ctx->inner;
    u32 *resourceNode;
    u32 *nextResource;

    while (inner->list != NULL) {
        sdfDestroyMotion(inner->list);
    }
    sdfResourceListRelease(inner->resourceHandle, 1);
    for (resourceNode = ctx->list14; resourceNode != NULL; resourceNode = nextResource) {
        nextResource = (u32 *)*resourceNode;
        mdlDestroyResourceItem(resourceNode);
    }
    mdlReleaseDevSlots(ctx);
    sdfReleaseDevSlot(inner, 1, 1);
    mdlUnlinkGroupEntry((MdlLink *)ctx);
    sdfReleaseChipBlock(ctx);
}

extern void func_002174C0();
extern s32 sdfMotionUpdate(void *motion);
extern void sdfModelUpdateCurrentFrameTransforms();
extern void func_002D9238();
extern void mdlDispatchViewerAnchorRecord();

/* Per-frame update: step the active slot nodes, refresh the transforms, dispatch anchor records. */
void mdlProcessContextNodesAndTransforms(MdlCtx *ctx, s32 arg) {
    MdlNode **slot = ctx->slots;
    MdlInner *inner;
    u32 *rec;
    s32 i;

    for (i = 0; i != 4; i++) {
        if (*slot != NULL) {
            if ((*slot)->unk30 != 0) {
                sdfMotionUpdate(*slot);
            }
        }
        slot++;
    }
    if (ctx->flags & 1) {
        return;
    }
    inner = ctx->inner;
    sdfModelUpdateCurrentFrameTransforms(inner);
    func_002D9238(arg, inner);
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
    func_002174C0(ctx, arg);
}

extern void sdfSetPrimaryIdentityMatrixVU(void *);
extern void sdfRotateVuMatrixAboutX(f32 angle);
extern void sdfRotateVuMatrixAboutY(f32 angle);

#define MDL_MOTION_SLOT_COUNT 4
#define MDL_NO_BLEND_ENTRY (-1)
#define MDL_RADIANS_PER_DEGREE 0.017453293f
#define MDL_FULL_BLEND_PITCH 25.0f
#define MDL_SKIP_TRANSFORMS 1
#define MDL_SKIP_ANCHORS 2
#define MDL_REQUIRE_ANCHOR_ENABLE 4
#define MDL_ANCHOR_ENABLE_BIT 0x10
#define MDL_ENTRY_ENABLED 1
#define MDL_PRIMARY_MOTION_SLOT 0
#define MDL_MOTION_LOOP_ENABLED 1
#define MDL_MOTION_LOOP_DISABLED 0

/* Blend the selected basis towards pitch/yaw (degrees), then update transforms
 * and anchors. For abs(pitch)<25, weight is abs(pitch)/25; otherwise one;
 * -1 skips basis blending. Slot motions/blending precede the skip flags.
 * updateArg is forwarded unchanged to the remaining update routines. */
void mdlBlendEntryPitchYawAndUpdate(MdlCtx *ctx, s32 updateArg, s32 entryIndex, f32 pitch, f32 yaw) {
    MdlInner *inner;
    MdlEntry *entry = NULL;
    f32 targetRows[4][4];
    f32 pitchMagnitude;
    f32 targetWeight;
    f32 existingWeight;
    u32 *resourceNode;
    s32 slotIndex;

    inner = ctx->inner;
    if (entryIndex != MDL_NO_BLEND_ENTRY) {
        entry = inner->entries->items[entryIndex];
        sdfSetPrimaryIdentityMatrixVU(inner->entries);
        sdfRotateVuMatrixAboutX(pitch * MDL_RADIANS_PER_DEGREE);
        sdfRotateVuMatrixAboutY(yaw * MDL_RADIANS_PER_DEGREE);
        VU0_STORE_MATRIX(targetRows);
    }
    for (slotIndex = 0; slotIndex != MDL_MOTION_SLOT_COUNT; slotIndex++) {
        if (ctx->slots[slotIndex] != NULL) {
            if (ctx->slots[slotIndex]->unk30 != 0) {
                sdfMotionUpdate(ctx->slots[slotIndex]);
            }
        }
    }
    pitchMagnitude = pitch;
    if (entryIndex != MDL_NO_BLEND_ENTRY) {
        if (pitchMagnitude < 0.0f) {
            pitchMagnitude = -pitchMagnitude;
        }
        targetWeight = 1.0f;
        if (pitchMagnitude < MDL_FULL_BLEND_PITCH) {
            targetWeight = pitchMagnitude / MDL_FULL_BLEND_PITCH;
        }
        existingWeight = 1.0f - targetWeight;
        entry->row0[0] = targetRows[0][0] * targetWeight + entry->row0[0] * existingWeight;
        entry->row0[1] = targetRows[0][1] * targetWeight + entry->row0[1] * existingWeight;
        entry->row0[2] = targetRows[0][2] * targetWeight + entry->row0[2] * existingWeight;
        entry->row1[0] = targetRows[1][0] * targetWeight + entry->row1[0] * existingWeight;
        entry->row1[1] = targetRows[1][1] * targetWeight + entry->row1[1] * existingWeight;
        entry->row1[2] = targetRows[1][2] * targetWeight + entry->row1[2] * existingWeight;
        entry->row2[0] = targetRows[2][0] * targetWeight + entry->row2[0] * existingWeight;
        entry->row2[1] = targetRows[2][1] * targetWeight + entry->row2[1] * existingWeight;
        entry->row2[2] = targetRows[2][2] * targetWeight + entry->row2[2] * existingWeight;
    }
    if (ctx->flags & MDL_SKIP_TRANSFORMS) {
        return;
    }
    inner = ctx->inner;
    sdfModelUpdateCurrentFrameTransforms(inner);
    func_002D9238(updateArg, inner);
    if (ctx->flags & MDL_SKIP_ANCHORS) {
        return;
    }
    if (ctx->flags & MDL_REQUIRE_ANCHOR_ENABLE) {
        if ((inner->flags19 & MDL_ANCHOR_ENABLE_BIT) == 0) {
            return;
        }
    }
    for (resourceNode = ctx->list14; resourceNode != NULL; resourceNode = (u32 *)*resourceNode) {
        mdlDispatchViewerAnchorRecord(ctx, resourceNode);
    }
    if (ctx->devList == NULL) {
        return;
    }
    func_002174C0(ctx, updateArg);
}

/* Enable each table entry. The signed table count governs iteration; entry
 * pointers and the context's inner/table pointers are required. */
void mdlEnableAllEntries(MdlCtx *ctx) {
    MdlEntryTable *table = ctx->inner->entries;
    s32 entryCount = table->count;
    MdlEntry **entries = table->items;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        entries[entryIndex]->enabled = MDL_ENTRY_ENABLED;
    }
}

extern MdlNode *motionOwnerCreateObjectForRecord(MdlCtx *, s32);
extern void sdfMotionInitialize(MdlNode *, s32, s32, f32, f32);
extern void mdlRemoveResourceSubtype(MdlCtx *, s32);
extern void mdlApplyResourceEntries(MdlCtx *, s32, s32);

/* Select the first matching searchId, or create it, and make it current in its
 * signed slot index. Allocation success/slot bounds are assumed. Slot zero
 * also becomes ctx->first; id and motionIndex narrow into the current pair. */
void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames) {
    MdlNode *node;
    s16 slotIndex;

    for (node = ctx->inner->list; node != NULL; node = node->next) {
        if (node->searchId == searchId) {
            break;
        }
    }
    if (node == NULL) {
        node = motionOwnerCreateObjectForRecord(ctx, searchId);
    }
    slotIndex = node->slotIndex;
    ctx->slots[slotIndex] = node;
    if (slotIndex == MDL_PRIMARY_MOTION_SLOT) {
        ctx->first = node;
    }
    sdfMotionInitialize(node, motionIndex, loopEnabled, blendLeadFrames, blendDurationFrames);
    ctx->current.h.id = searchId;
    ctx->current.h.arg = motionIndex;
    mdlRemoveResourceSubtype(ctx, slotIndex);
    mdlApplyResourceEntries(ctx, motionIndex, slotIndex);
}

/* Select a looping motion with both blend-frame parameters zero. */
void mdlAddEntryFlagged(MdlCtx *ctx, s32 searchId, s32 motionIndex) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, motionIndex, MDL_MOTION_LOOP_ENABLED, 0.0f, 0.0f);
}

/* Select a nonlooping motion with both blend-frame parameters zero. */
void mdlAddEntryPlain(MdlCtx *ctx, s32 searchId, s32 motionIndex) {
    mdlFindOrCreateMotionRecordNode(ctx, searchId, motionIndex, MDL_MOTION_LOOP_DISABLED, 0.0f, 0.0f);
}

/* Select a looping motion with the supplied blend-frame parameters. */
void mdlAddEntryFlaggedEx(MdlCtx *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames) {
    mdlFindOrCreateMotionRecordNode(
        ctx, searchId, motionIndex, MDL_MOTION_LOOP_ENABLED, blendLeadFrames, blendDurationFrames);
}

/* Select a nonlooping motion with the supplied blend-frame parameters. */
void mdlAddEntryPlainEx(MdlCtx *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                        f32 blendDurationFrames) {
    mdlFindOrCreateMotionRecordNode(
        ctx, searchId, motionIndex, MDL_MOTION_LOOP_DISABLED, blendLeadFrames, blendDurationFrames);
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

#define MDL_NODE_FIELD_MISSING (-1)
#define MDL_NODE_BYTE_CHECK_MISSING 2
#define MDL_NODE_BYTE_CHECK_MATCH 5

/* Read the node's unknown halfword, widened to s32. Missing nodes return -1,
 * distinct from a present halfword of 0xFFFF. */
s32 mdlGetNodeField2C(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return MDL_NODE_FIELD_MISSING;
    }
    return matchedNode->unk2C;
}

/* Read the other unknown halfword; return zero for a missing node. */
s32 mdlGetNodeField2E(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return matchedNode->unk2E;
}

/* Numerically convert the stored float to s32, not a bit reinterpretation.
 * Return zero when the searched node is absent. */
s32 mdlGetNodeInt1C(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return (s32)matchedNode->unk1C;
}

/* Three outcomes: 2 for a missing node, otherwise 1/0 for byte equal/not equal
 * to 5. The meaning of that byte value is not established here. */
s32 mdlCheckNodeByte30(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return MDL_NODE_BYTE_CHECK_MISSING;
    }
    return matchedNode->unk30 == MDL_NODE_BYTE_CHECK_MATCH;
}

/* Read the stored float, defaulting to zero when the searched node is absent. */
f32 mdlGetNodeFloat20(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);
    f32 result = 0.0f;

    if (matchedNode != NULL) {
        result = matchedNode->floatValue;
    }
    return result;
}

/* Replace the stored float only when the searched node exists. */
void mdlSetNodeFloat20(MdlCtx *ctx, s32 searchId, f32 value) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode != NULL) {
        matchedNode->floatValue = value;
    }
}

/* vu0 routine: load the primary vector into vf10, not a C return value. */
void mdlLoadPrimaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, &ctx->inner->vector50);
}

/* vu0 routine: force vf10.w to one and store it as the primary vector. */
void mdlStorePrimaryVectorVU(MdlCtx *ctx) {
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, &ctx->inner->vector50);
}

/* vu0 routine: load the secondary vector into vf10 without interpreting it. */
void mdlLoadSecondaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, &ctx->inner->vector60);
}

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: consume vf10 as a quaternion, preserve it in the secondary vector,
 * then store the resulting basis rows from vf28-vf30. No C argument supplies vf10. */
void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *ctx) {
    VU0_STORE_VF(vf10, &ctx->inner->vector60);
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF(vf28, &ctx->inner->vector20);
    VU0_STORE_VF(vf29, &ctx->inner->vector30);
    VU0_STORE_VF(vf30, &ctx->inner->vector40);
}

/* vu0 routine: load the tertiary vector into vf10; projection uses its row scale. */
void mdlLoadTertiaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, &ctx->inner->vector70);
}

/* vu0 routine: store all vf10 components as the tertiary vector, without forcing w. */
void mdlStoreTertiaryVectorVU(MdlCtx *ctx) {
    VU0_STORE_VF(vf10, &ctx->inner->vector70);
}

/* Return the last original value stored by either broadcast path. */
u32 mdlGetBroadcastValue(MdlCtx *ctx) {
    return ctx->inner->broadcastValue;
}

/* Forward value to each next-linked resource; an empty resource list is a no-op. */
void mdlSetAllResourceFrames(MdlCtx *ctx, u32 value) {
    u32 *resourceNode;

    for (resourceNode = ctx->list14; resourceNode != NULL; resourceNode = (u32 *)*resourceNode) {
        mdlSetResourceFrame(ctx, resourceNode, value);
    }
}

void mdlBroadcastMasked(MdlCtx *ctx, u32 value) {
    ctx->inner->broadcastValue = value;
    mdlSetAllResourceFrames(ctx, (value & 0xFF000000) | 0x808080);
}

/* Cache the original value and send that same value to every resource. */
void mdlBroadcastValue(MdlCtx *ctx, u32 value) {
    ctx->inner->broadcastValue = value;
    mdlSetAllResourceFrames(ctx, value);
}

/* Forward the floating amount to each resource without changing broadcastValue. */
void mdlSetAmountOnAllContextResources(MdlCtx *ctx, f32 amount) {
    u32 *resourceNode;

    for (resourceNode = ctx->list14; resourceNode != NULL; resourceNode = (u32 *)*resourceNode) {
        mdlSetResourceAmount(ctx, resourceNode, amount);
    }
}

/* vu0 routine: project point with the model/camera matrices and viewport vectors.
 * Row scaling uses live vf10 before loading point; the result is left in vf10.
 * Preserve the distinct matrix-composition order of this single-point path. */
void mdlProjectPointVU(MdlCtx *ctx, void *point)
{
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
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
    VU0_LOAD_VF(vf11, D_00324650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
}

#define MDL_PROJECTION_SCALE_OFFSET 0x40
#define MDL_PROJECTION_BIAS_OFFSET 0x50

/* vu0 routine: project pointCount four-float vectors using the stored tertiary
 * row scale. Matrix setup still runs for nonpositive counts. Output stores all
 * four components; perspective division is not guarded against zero w. */
void mdlProjectPoints(MdlCtx *ctx, f32 (*inputPoints)[4], f32 (*outputPoints)[4], s32 pointCount)
{
    s32 pointIndex;

    VU0_LOAD_MATRIX(&ctx->inner->vector20);
    VU0_LOAD_VF(vf10, &ctx->inner->vector70);
    VU0_SCALE_MATRIX_ROWS(vf10);
    sdfPostmultiplyVuMatrixFromMemory(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
        VU0_LOAD_VF(vf10, inputPoints[pointIndex]);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_PERSPECTIVE_DIVIDE_VF10();
        VU0_LOAD_VF(vf11, sdfProjectionMatrix + MDL_PROJECTION_SCALE_OFFSET);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, sdfProjectionMatrix + MDL_PROJECTION_BIAS_OFFSET);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, outputPoints[pointIndex]);
    }
}

/* Suspend every node in the inner motion list, not just the active slots. */
void mdlSuspendAllContextMotions(MdlCtx *ctx) {
    MdlNode *motionNode;

    for (motionNode = ctx->inner->list; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionSuspend(motionNode);
    }
}

/* Resume every node in the inner motion list, not just the active slots. */
void mdlResumeAllContextMotions(MdlCtx *ctx) {
    MdlNode *motionNode;

    for (motionNode = ctx->inner->list; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionResume(motionNode);
    }
}

/* Return whether the searched node exists; do not expose the lookup result. */
u8 mdlHasNode(MdlCtx *ctx, s32 searchId) {
    MdlNode *lookupResult;

    lookupResult = mdlFindNodeById(ctx, searchId);
    return lookupResult != NULL;
}

/* Read the context resource-group halfword; ctx/sub are required. */
u16 mdlGetContextResourceGroup(MdlCtx *ctx) {
    return ctx->sub->unk8;
}

/* Read the context resource-id halfword; ctx/sub are required. */
u16 mdlGetContextResourceId(MdlCtx *ctx) {
    return ctx->sub->unkA;
}

u32 func_002183F0(void) {
    return 8;
}

/* Return the first word of the selected two-word table row, without bounds checks. */
u32 mdlGetTableWord(s32 tableIndex) {
    return D_00367904[tableIndex][0];
}

/* Read the referenced halfword or return zero for a missing node. A present
 * node's unk8 reference is dereferenced without a separate NULL check. */
s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 searchId) {
    MdlNode *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return *(u16 *)matchedNode->unk8;
}

/* Pass the inner resource handle to its release/update routine. ctx is required. */
void mdlReleaseInnerResourceHandle(MdlCtx *ctx) {
    sdfUpdateActiveResourceListScalars(ctx->inner->resourceHandle);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218460);

#define MDL_SENTINEL_CURRENT_BYTE 1
#define MDL_PART_INFO_TAG 0x30424950

/* Test the raw PIB0 marker only when the low current byte is one. This is a
 * pointer-value comparison, not a dereference or a check of the complete id. */
s32 mdlIsInnerSentinel(MdlCtx *ctx) {
    s32 isSentinel = 0;

    if ((u8)ctx->current.word == MDL_SENTINEL_CURRENT_BYTE) {
        isSentinel = ctx->inner == (MdlInner *)MDL_PART_INFO_TAG;
    }
    return isSentinel;
}

typedef struct PacWork {
    struct PacWork *next;
    struct PacState *owner;
    s32 resourceHandle;
    u8 *dataCursor;
    u8 packet[1];
} PacWork;

typedef struct PacHead {
    u8 command;
    u8 flags;
    u8 pad2[2];
    s32 payloadSize;
    u32 tag;
    s32 decodedSize;
    u8 payload[1];
} PacHead;

typedef struct MdlPartList MdlPartList;
typedef struct MdlRecord MdlRecord;
extern PacWork *sdfPacRemovePacket(PacWork *);
extern u16 func_002193E8(MdlRecord *);
extern MdlPartList *mdlCreateBufferedPartRequest(u32);
extern void mdlAddBillboardPart(MdlPartList *, s32);
extern void mdlAddEffectPart(MdlPartList *, s32);
extern void sdfReleaseResourceAllocation(s32);
extern void *memset(void *, s32, u32);

#define MDL_PART_PACKET_COMMAND 1
#define MDL_RESOURCE_LIST_PACKET_COMMAND 9
#define MDL_REQUEST_PACKET_COMMAND 6
#define MDL_RESOURCE_PACKET_COMMAND 8
#define MDL_BILLBOARD_PART_TAG 0x413250
#define MDL_EFFECT_PART_TAG 0x503344

/* Consume all packet work, then apply the accumulated group setup once.
 * Request/resource packets latch a nonzero handle; part-info replaces its data
 * and handle, but retains an existing part list when its requested count is zero.
 * Resource-list packets overwrite their handle. Part additions assume a list.
 * Unknown commands/tags are still removed. Normal completion returns NULL. */
PacWork *mdlApplyQueuedGroupPackets(PacWork *packetWork, s32 group, s32 id, s32 mode) {
    MdlGroupSetup groupSetup;
    MdlPartList *partList;
    s32 requestedPartCount;

    memset(&groupSetup, 0, sizeof(groupSetup));
    partList = NULL;
    while (packetWork != NULL) {
        switch (((PacHead *)packetWork->packet)->command) {
        case MDL_PART_PACKET_COMMAND:
            switch (((PacHead *)packetWork->packet)->tag) {
            case MDL_PART_INFO_TAG: /* PIB0: model part information. */
                groupSetup.handleA = (s32)packetWork->dataCursor;
                groupSetup.handleB = packetWork->resourceHandle;
                requestedPartCount = func_002193E8((MdlRecord *)packetWork->dataCursor);
                if (requestedPartCount > 0) {
                    partList = mdlCreateBufferedPartRequest(requestedPartCount);
                    groupSetup.handleC = (s32)partList;
                }
                break;
            case MDL_BILLBOARD_PART_TAG: /* P2A: billboard part. */
                mdlAddBillboardPart(partList, (s32)packetWork->dataCursor);
                sdfReleaseResourceAllocation(packetWork->resourceHandle);
                break;
            case MDL_EFFECT_PART_TAG: /* D3P: effect part. */
                mdlAddEffectPart(partList, (s32)packetWork->dataCursor);
                sdfReleaseResourceAllocation(packetWork->resourceHandle);
                break;
            }
            break;
        case MDL_RESOURCE_LIST_PACKET_COMMAND:
            groupSetup.resourceList = packetWork->resourceHandle;
            break;
        case MDL_REQUEST_PACKET_COMMAND:
            if (groupSetup.requestHandle == 0) {
                groupSetup.requestHandle = packetWork->resourceHandle;
                groupSetup.unk4 = (s32)packetWork->dataCursor;
            }
            break;
        case MDL_RESOURCE_PACKET_COMMAND:
            if (groupSetup.resource == 0) {
                groupSetup.resource = packetWork->resourceHandle;
                groupSetup.flags = (s32)packetWork->dataCursor;
            }
            break;
        }
        packetWork = sdfPacRemovePacket(packetWork);
    }
    mdlApplyGroupSetup(group, id, mode, &groupSetup);
    return packetWork;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218768);

/* Clean up the owner word at +8, then release its containing block. */
void mdlDestroyLoadRequestOwner(MdlRes *ownerBlock) {
    func_00288788(ownerBlock->unk8);
    sdfReleaseChipBlock(ownerBlock);
}

/* Completion job created by mdlRequestLoadWithCallback and run by mdlCompleteGroupedJobAndNotify. */
typedef struct MdlDoneJob {
    u16 group;         /* 0x0 */
    u16 id;            /* 0x2 */
    u32 arg;           /* 0x4 */
    void *owner;       /* 0x8: request slot from fileAllocateDispatchRequest */
    void (*done)(u32); /* 0xC */
    u32 doneArg;       /* 0x10 */
} MdlDoneJob;

/* Request slot handed back by fileAllocateDispatchRequest; its +0x60 word feeds the load apply. */
typedef struct MdlLoadSlot {
    u8 pad00[0x60];
    u32 handle; /* 0x60 */
} MdlLoadSlot;

extern s32 func_00218768();

/* Apply the completed load and remove its group id under the semaphore.
 * A non-NULL callback is invoked before owner/job cleanup; without a callback,
 * this function leaves cleanup to the request path. */
void mdlCompleteGroupedJobAndNotify(MdlLoadSlot *requestOwner, MdlDoneJob *completionJob) {
    completionJob->owner = requestOwner;
    func_00218768(requestOwner->handle, completionJob->group, completionJob->id, completionJob->arg);
    WaitSema(mdlGroupJobSemaphore);
    btlRemoveGroupId(completionJob->group, completionJob->id);
    SignalSema(mdlGroupJobSemaphore);
    if (completionJob->done != NULL) {
        completionJob->done(completionJob->doneArg);
        mdlDestroyLoadRequestOwner((MdlRes *)completionJob);
    }
}

extern void *sdfAllocAndClearQuadwords();
extern s32 fileAllocateDispatchRequest();
extern void func_00288C50();
extern void mdlCompleteGroupedJobAndNotify();

#define MDL_DONE_JOB_BYTES 0x14

/* Allocate a completion job and dispatch the request. Group/id narrow to u16.
 * Without onComplete, run the existing no-callback completion path and clean up
 * here; otherwise the completion callback path owns cleanup. Always return zero.
 * Preserve the provider's existing short-arity/unprototyped calling convention. */
s32 mdlRequestLoadWithCallback(s32 group, s32 id, s32 jobArg, s32 requestHandle, void (*onComplete)(u32), u32 callbackArg) {
    MdlDoneJob *completionJob = sdfAllocAndClearQuadwords(MDL_DONE_JOB_BYTES);
    s32 requestSlot;

    completionJob->group = group;
    completionJob->id = id;
    completionJob->arg = jobArg;
    completionJob->doneArg = callbackArg;
    completionJob->done = onComplete;
    requestSlot = fileAllocateDispatchRequest(requestHandle, 0, 0, mdlCompleteGroupedJobAndNotify, completionJob);
    completionJob->owner = requestSlot;
    if (onComplete == NULL) {
        func_00288C50(requestSlot);
        mdlDestroyLoadRequestOwner((MdlRes *)completionJob);
    }
    return 0;
}

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB60);

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB68);

