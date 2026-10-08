#include "ee_mmi.h"
#include "common.h"
#include "pcp_vu0.h"
#include "mdl.h"

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_00324650[];
extern u8 D_00324660[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);
extern void sdfMultiplyVuMatrixInPlace(void);



/* Completion job created by mdlRequestLoadWithCallback and run by mdlCompleteGroupedJobAndNotify. */
typedef struct MdlDoneJob {
    u16 group;         /* 0x0 */
    u16 id;            /* 0x2 */
    u32 arg;           /* 0x4 */
    void *owner;       /* 0x8: request slot from fileAllocateDispatchRequest */
    void (*done)(u32); /* 0xC */
    u32 doneArg;       /* 0x10 */
} MdlDoneJob;
/* 8-byte prefix copied from D_003BBB60 by mdlBuildPrefixedString. */
typedef struct Hdr8 {
    u8 b[8];
} Hdr8;

extern u32 D_00367904[][2];
extern u8 D_003BBB60[];
extern void sdfDestroyMotion(Motion *arg);
extern char *strcat(char *dst, const char *src);

typedef struct MdlResourceSelection {
    u16 pathTable;
    u16 pathIndex;
    u32 unk4;
} MdlResourceSelection;

typedef struct MdlResourcePath {
    u32 unk0;
    char *path;
    u32 unk8;
} MdlResourcePath;

typedef struct MdlResourceTable {
    void *entries;
    s32 count;
} MdlResourceTable;

extern MdlResourceTable D_00367900[];
extern MdlResourceTable D_00365858[];

Motion *mdlFindNodeById(MdlCtx *ctx, s32 id);
void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames);

struct FileWork;
extern u32 fileGetLoadedDataAddress(struct FileWork *);
extern u32 fileGetResourceHandle(struct FileWork *);
extern DevRequest *sndBuildResourceHandleListFromOffsets(const void *);
extern void mdlSetResourceAmount(MdlCtx *ctx, MdlResourceItem *node, f32 amount);

extern u32 mdlGroupJobSemaphore;

extern BattleGroupNode *btlFindGroupedEntity(s32 group, s32 id);

void mdlClearSlotAndRelease(MdlCtx *ctx, Motion *node) {
    if (ctx->slots[node->slotIndex] == node) {
        ctx->slots[node->slotIndex] = NULL;
    }
    sdfDestroyMotion(node);
}

void mdlReleaseFirstMatch(MdlCtx *ctx, s32 id) {
    Motion *node = ctx->inner->motionList;

    while (node != NULL) {
        if (node->searchId == id) {
            mdlClearSlotAndRelease(ctx, node);
            break;
        }
        node = node->next;
    }
}


extern void sdfReleaseResourceAllocation();

/* Release slot `index`: destroy its motions in every context and free the attached resource. */
void mdlReleaseOwnerSlotResources(BattleGroupNode *owner, s32 index) {
    MdlCtx *ctx;

    if (owner != NULL) {
        if (owner->slots[index].data == NULL) {
            return;
        }
        for (ctx = owner->modelContext; ctx != NULL; ctx = ctx->next) {
            mdlReleaseFirstMatch(ctx, index);
        }
        if (owner->ownsResources != 0) {
            if (owner->slots[index].resourceHandle != 0) {
                sdfReleaseResourceAllocation(owner->slots[index].resourceHandle);
            }
        }
        owner->slots[index].data = NULL;
        owner->slots[index].resourceHandle = 0;
    }
}

void mdlApplyCommandToGroupedEntity(s32 group, s32 id, s32 index) {
    BattleGroupNode *entity;

    entity = btlFindGroupedEntity(group, id);
    mdlReleaseOwnerSlotResources(entity, index);
}

void mdlConfigureGroupedEntitySlot(s32 group, s32 id, u32 mode, s32 motionIndex, s32 index, s32 slotIndex, void *data, u32 resourceHandle) {
    BattleGroupNode *owner = btlFindGroupedEntity(group, id);
    BattleGroupSlot *slot;

    mdlReleaseOwnerSlotResources(owner, index);
    slot = &owner->slots[index];
    slot->slot = slotIndex;
    slot->motionIndex = motionIndex;
    slot->data = data;
    slot->resourceHandle = resourceHandle;
    slot->flags = 0;
    if (mode & 0x100) {
        slot->flags = 1;
    }
    if (mode & 0x200) {
        slot->flags |= 2;
    }
}


extern void btlCreateGroupNode(s32 group, s32 id, s32 mode, DevRequest *resourceList, void *itemList, s32 requestHandle);

void mdlApplyGroupSetup(s32 group, s32 id, s32 mode, MdlLoadPayload *setup) {
    BattleGroupNode *entity;

    btlCreateGroupNode(group, id, mode, setup->resourceList, setup->itemList, setup->requestHandle);
    if (setup->motionData != NULL) {
        mdlConfigureGroupedEntitySlot(group, id, mode, 0, 0, 0, setup->motionData, setup->motionResource);
    }
    if (setup->partInfo != NULL) {
        entity = btlFindGroupedEntity(group, id);
        entity->partInfo = setup->partInfo;
        entity->resourceHandle = setup->resourceHandle;
        entity->partList = setup->partList;
    }
}

extern s32 btlGroupContainsId(s32 group, s32 id);
extern s32 fileManUpdate(void);

BattleGroupNode *mdlWaitGroupThenFind(s32 group, s32 id) {
    while (btlGroupContainsId(group, id)) {
        fileManUpdate();
    }
    return btlFindGroupedEntity(group, id);
}

void mdlExecuteAndFreeJob(MdlLoadRequest *request) {
    mdlApplyGroupSetup(request->group, request->id, request->options, &request->payload);
    WaitSema(mdlGroupJobSemaphore);
    btlRemoveGroupId(request->group, request->id);
    SignalSema(mdlGroupJobSemaphore);
    sdfReleaseChipBlock(request);
}

void mdlRecordLoadedSizeAndReleaseHandle(struct FileWork *resource, MdlLoadRequest *destination) {
    u32 handle;
    DevRequest *resourceList;

    handle = fileGetLoadedDataAddress(resource);
    resourceList = sndBuildResourceHandleListFromOffsets((const void *)handle);
    destination->payload.resourceList = resourceList;
    handle = fileGetResourceHandle(resource);
    sdfReleaseResourceAllocation(handle);
    filePollEntryCleanup(resource);
}


extern s32 sdfRelocatePackedResourcePayload();

/* Retain the resource handle, relocate the loaded payload and retire the file
 * entry. Execute the group job now only when the command is not deferred. */
void mdlFinishLoadCmd(struct FileWork *resource, MdlLoadRequest *request) {
    request->payload.requestHandle = fileGetResourceHandle(resource);
    request->payload.itemList = (void *)sdfRelocatePackedResourcePayload(fileGetLoadedDataAddress(resource));
    filePollEntryCleanup(resource);
    if (request->deferred == 0) {
        mdlExecuteAndFreeJob(request);
    }
}

extern s32 sdfRelocatePackedResourceWordsFromHeader();


/* Retain the handle and relocated motion data, retire the file entry, then run
 * the group job. This callback completes the additional file request. */
void mdlFinishLoadJob(struct FileWork *resource, MdlLoadRequest *request) {
    request->payload.motionResource = fileGetResourceHandle(resource);
    request->payload.motionData = (void *)sdfRelocatePackedResourceWordsFromHeader(fileGetLoadedDataAddress(resource));
    filePollEntryCleanup(resource);
    mdlExecuteAndFreeJob(request);
}

/* Copy the fixed eight-byte prefix, then append src. Prefix termination and
 * sufficient destination capacity are obligations of the data/caller. */
char *mdlBuildPrefixedString(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_003BBB60;
    return strcat(dst, src);
}

extern s32 mdlRequestAsset(s32 group, s32 id, s32 blocking);

INCLUDE_ASM(const s32, "model/mdlManager", mdlRequestAsset);

/* The blocking SDK request returns its group in a native status/address word. */
BattleGroupNode *func_00217298(u32 group, u32 id) {
    return (BattleGroupNode *)mdlRequestAsset(group, id, 1);
}


extern void btlDestroyGroupNode(BattleGroupNode *group);

/* Unlink without clearing this context's own links. A resource-owning group is
 * destroyed when its last context leaves. */
void mdlUnlinkGroupEntry(MdlCtx *ctx) {
    MdlCtx *nextContext = ctx->next;
    MdlCtx *previousContext = ctx->previous;
    BattleGroupNode *group;

    if (nextContext != NULL) {
        nextContext->previous = previousContext;
    }
    if (previousContext != NULL) {
        previousContext->next = nextContext;
    } else {
        group = ctx->sub;
        if (nextContext != NULL) {
            group->modelContext = nextContext;
        } else {
            group->modelContext = NULL;
            if (group->ownsResources != 0) {
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

extern void mdlDestroyResourceItem(MdlResourceItem *);
extern void sdfResourceListRelease(DevRequest *, s32);

/* Destroy motions, resources, device slots and the context itself. Motion
 * destruction must unlink inner->motionList; save each resource's next link before
 * destroying it. ctx and inner are required, not checked here. */
void mdlDestroyContext(MdlCtx *ctx) {
    SdfModel *inner = ctx->inner;
    MdlResourceItem *resourceNode;
    MdlResourceItem *nextResource;

    while (inner->motionList != NULL) {
        sdfDestroyMotion(inner->motionList);
    }
    sdfResourceListRelease(inner->assetData, 1);
    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = nextResource) {
        nextResource = resourceNode->next;
        mdlDestroyResourceItem(resourceNode);
    }
    mdlReleaseDevSlots(ctx);
    sdfReleaseDevSlot(inner, 1, 1);
    mdlUnlinkGroupEntry(ctx);
    sdfReleaseChipBlock(ctx);
}

extern void func_002174C0();
extern s32 sdfMotionUpdate(void *motion);
extern void sdfModelUpdateCurrentFrameTransforms();
extern void func_002D9238();
extern void mdlDispatchViewerAnchorRecord(MdlCtx *, MdlResourceItem *);

/* Per-frame update: step the active slot nodes, refresh the transforms, dispatch anchor records. */
void mdlProcessContextNodesAndTransforms(MdlCtx *ctx, s32 arg) {
    Motion **slot = ctx->slots;
    SdfModel *inner;
    MdlResourceItem *rec;
    s32 i;

    for (i = 0; i != 4; i++) {
        if (*slot != NULL) {
            if ((*slot)->state != 0) {
                sdfMotionUpdate(*slot);
            }
        }
        slot++;
    }
    if (ctx->flags & MDL_SKIP_TRANSFORMS) {
        return;
    }
    inner = ctx->inner;
    sdfModelUpdateCurrentFrameTransforms(inner);
    func_002D9238(arg, inner);
    if (ctx->flags & MDL_SKIP_ANCHORS) {
        return;
    }
    if (ctx->flags & MDL_REQUIRE_ANCHOR_ENABLE) {
        if ((inner->flags & 0x10) == 0) {
            return;
        }
    }
    for (rec = ctx->resourceItems; rec != NULL; rec = rec->next) {
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
    SdfModel *inner;
    SdfDrawNode *entry = NULL;
    f32 targetRows[4][4];
    f32 pitchMagnitude;
    f32 targetWeight;
    f32 existingWeight;
    MdlResourceItem *resourceNode;
    s32 slotIndex;

    inner = ctx->inner;
    if (entryIndex != MDL_NO_BLEND_ENTRY) {
        entry = ((SdfDrawNode **)inner->list->buffer)[entryIndex];
        sdfSetPrimaryIdentityMatrixVU(inner->list);
        sdfRotateVuMatrixAboutX(pitch * MDL_RADIANS_PER_DEGREE);
        sdfRotateVuMatrixAboutY(yaw * MDL_RADIANS_PER_DEGREE);
        VU0_STORE_MATRIX(targetRows);
    }
    for (slotIndex = 0; slotIndex != MDL_MOTION_SLOT_COUNT; slotIndex++) {
        if (ctx->slots[slotIndex] != NULL) {
            if (ctx->slots[slotIndex]->state != 0) {
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
        entry->localMatrix[0][0] = targetRows[0][0] * targetWeight + entry->localMatrix[0][0] * existingWeight;
        entry->localMatrix[0][1] = targetRows[0][1] * targetWeight + entry->localMatrix[0][1] * existingWeight;
        entry->localMatrix[0][2] = targetRows[0][2] * targetWeight + entry->localMatrix[0][2] * existingWeight;
        entry->localMatrix[1][0] = targetRows[1][0] * targetWeight + entry->localMatrix[1][0] * existingWeight;
        entry->localMatrix[1][1] = targetRows[1][1] * targetWeight + entry->localMatrix[1][1] * existingWeight;
        entry->localMatrix[1][2] = targetRows[1][2] * targetWeight + entry->localMatrix[1][2] * existingWeight;
        entry->localMatrix[2][0] = targetRows[2][0] * targetWeight + entry->localMatrix[2][0] * existingWeight;
        entry->localMatrix[2][1] = targetRows[2][1] * targetWeight + entry->localMatrix[2][1] * existingWeight;
        entry->localMatrix[2][2] = targetRows[2][2] * targetWeight + entry->localMatrix[2][2] * existingWeight;
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
        if ((inner->flags & MDL_ANCHOR_ENABLE_BIT) == 0) {
            return;
        }
    }
    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
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
    DevRequest *table = ctx->inner->list;
    s32 entryCount = table->usedCount;
    SdfDrawNode **entries = table->buffer;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        entries[entryIndex]->flags = MDL_ENTRY_ENABLED;
    }
}

extern Motion *motionOwnerCreateObjectForRecord(MdlCtx *, s32);
extern void sdfMotionInitialize(Motion *, s32, s32, f32, f32);
extern void mdlRemoveResourceSubtype(MdlCtx *, s32);
extern void mdlApplyResourceEntries(MdlCtx *, s32, s32);

/* Select the first matching searchId, or create it, and make it current in its
 * signed slot index. Allocation success/slot bounds are assumed. Slot zero
 * also becomes ctx->first; id and motionIndex narrow into the current pair. */
void mdlFindOrCreateMotionRecordNode(MdlCtx *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames) {
    Motion *node;
    s16 slotIndex;

    for (node = ctx->inner->motionList; node != NULL; node = node->next) {
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

Motion *mdlFindNodeById(MdlCtx *ctx, s32 id) {
    Motion *node;

    for (node = ctx->inner->motionList; node != NULL; node = node->next) {
        if (node->searchId == id) {
            return node;
        }
    }
    return NULL;
}

#define MDL_NODE_FIELD_MISSING (-1)
#define MDL_NODE_BYTE_CHECK_MISSING 2
#define MDL_NODE_BYTE_CHECK_MATCH 5

/* Read the motion selector, widened to s32. Missing nodes return -1,
 * distinct from a present selector of 0xFFFF. */
s32 mdlGetNodeField2C(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return MDL_NODE_FIELD_MISSING;
    }
    return matchedNode->motionIndex;
}

/* Read the selected motion's frame count; return zero for a missing node. */
s32 mdlGetNodeField2E(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return matchedNode->frameCount;
}

/* Numerically convert the stored float to s32, not a bit reinterpretation.
 * Return zero when the searched node is absent. */
s32 mdlGetNodeInt1C(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return (s32)matchedNode->currentFrame;
}

/* Three outcomes: 2 for a missing node, otherwise 1/0 for byte equal/not equal
 * to 5. The meaning of that byte value is not established here. */
s32 mdlCheckNodeByte30(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return MDL_NODE_BYTE_CHECK_MISSING;
    }
    return matchedNode->state == MDL_NODE_BYTE_CHECK_MATCH;
}

/* Read the stored float, defaulting to zero when the searched node is absent. */
f32 mdlGetNodeFrameStep(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);
    f32 result = 0.0f;

    if (matchedNode != NULL) {
        result = matchedNode->frameStep;
    }
    return result;
}

/* Replace the stored float only when the searched node exists. */
void mdlSetNodeFrameStep(MdlCtx *ctx, s32 searchId, f32 value) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode != NULL) {
        matchedNode->frameStep = value;
    }
}

/* vu0 routine: load the primary vector into vf10, not a C return value. */
void mdlLoadPrimaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, ctx->inner->matrix[3]);
}

/* vu0 routine: force vf10.w to one and store it as the primary vector. */
void mdlStorePrimaryVectorVU(MdlCtx *ctx) {
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, ctx->inner->matrix[3]);
}

/* vu0 routine: load the secondary vector into vf10 without interpreting it. */
void mdlLoadSecondaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, ctx->inner->unk60);
}

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: consume vf10 as a quaternion, preserve it in the secondary vector,
 * then store the resulting basis rows from vf28-vf30. No C argument supplies vf10. */
void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *ctx) {
    VU0_STORE_VF(vf10, ctx->inner->unk60);
    effMiscQuaternionToMatrixVU();
    VU0_STORE_VF(vf28, ctx->inner->matrix[0]);
    VU0_STORE_VF(vf29, ctx->inner->matrix[1]);
    VU0_STORE_VF(vf30, ctx->inner->matrix[2]);
}

/* vu0 routine: load the tertiary vector into vf10; projection uses its row scale. */
void mdlLoadTertiaryVectorVU(MdlCtx *ctx) {
    VU0_LOAD_VF_MEMORY(vf10, ctx->inner->scaleVector);
}

/* vu0 routine: store all vf10 components as the tertiary vector, without forcing w. */
void mdlStoreTertiaryVectorVU(MdlCtx *ctx) {
    VU0_STORE_VF(vf10, ctx->inner->scaleVector);
}

/* Return the last original value stored by either broadcast path. */
u32 mdlGetBroadcastValue(MdlCtx *ctx) {
    return ctx->inner->color;
}

/* Forward value to each next-linked resource; an empty resource list is a no-op. */
void mdlSetAllResourceFrames(MdlCtx *ctx, u32 value) {
    MdlResourceItem *resourceNode;

    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
        mdlSetResourceFrame(ctx, resourceNode, value);
    }
}

void mdlBroadcastMasked(MdlCtx *ctx, u32 value) {
    ctx->inner->color = value;
    mdlSetAllResourceFrames(ctx, (value & 0xFF000000) | 0x808080);
}

/* Cache the original value and send that same value to every resource. */
void mdlBroadcastValue(MdlCtx *ctx, u32 value) {
    ctx->inner->color = value;
    mdlSetAllResourceFrames(ctx, value);
}

/* Forward the floating amount to each resource without changing broadcastValue. */
void mdlSetAmountOnAllContextResources(MdlCtx *ctx, f32 amount) {
    MdlResourceItem *resourceNode;

    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
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
    VU0_LOAD_MATRIX(ctx->inner->matrix);
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

    VU0_LOAD_MATRIX(ctx->inner->matrix);
    VU0_LOAD_VF(vf10, ctx->inner->scaleVector);
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
    Motion *motionNode;

    for (motionNode = ctx->inner->motionList; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionSuspend(motionNode);
    }
}

/* Resume every node in the inner motion list, not just the active slots. */
void mdlResumeAllContextMotions(MdlCtx *ctx) {
    Motion *motionNode;

    for (motionNode = ctx->inner->motionList; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionResume(motionNode);
    }
}

/* Return whether the searched node exists; do not expose the lookup result. */
u8 mdlHasNode(MdlCtx *ctx, s32 searchId) {
    Motion *lookupResult;

    lookupResult = mdlFindNodeById(ctx, searchId);
    return lookupResult != NULL;
}

/* Read the context resource-group halfword; ctx/sub are required. */
u16 mdlGetContextResourceGroup(MdlCtx *ctx) {
    return ctx->sub->group;
}

/* Read the context resource-id halfword; ctx/sub are required. */
u16 mdlGetContextResourceId(MdlCtx *ctx) {
    return ctx->sub->type;
}

u32 func_002183F0(void) {
    return 8;
}

/* Return the first word of the selected two-word table row, without bounds checks. */
u32 mdlGetTableWord(s32 tableIndex) {
    return D_00367904[tableIndex][0];
}

/* Read the referenced halfword or return zero for a missing node. A present
 * node's motion-table reference is dereferenced without a separate NULL check. */
s32 mdlGetNodeRefHalf(MdlCtx *ctx, s32 searchId) {
    Motion *matchedNode = mdlFindNodeById(ctx, searchId);

    if (matchedNode == NULL) {
        return 0;
    }
    return matchedNode->motionTable->unk00;
}

extern void sdfUpdateActiveResourceListScalars(DevRequest *, s32, f32);

/* Forward the packed scalar word and blend value to each active inner asset. */
void mdlReleaseInnerResourceHandle(MdlCtx *ctx, s32 value, f32 scalar) {
    sdfUpdateActiveResourceListScalars(ctx->inner->assetData, value, scalar);
}

/* Copy a resource path's basename without its extension into destination. */
void mdlCopyResourceBasename(s32 selectionListIndex, s32 selectionIndex, char *destination, s32 capacity) {
    MdlResourceSelection *selection;
    MdlResourcePath *pathEntry;
    char *path;
    char *basename;
    s32 pathLength;
    s32 startIndex;
    s32 endIndex;
    s32 copyLength;

    selection = (MdlResourceSelection *)D_00367900[selectionListIndex].entries;
    selection += selectionIndex;
    pathEntry = (MdlResourcePath *)D_00365858[selection->pathTable].entries;
    pathEntry += selection->pathIndex;
    path = pathEntry->path;
    startIndex = strlen(path);
    pathLength = startIndex;
    startIndex--;

    while (1) {
        basename = &path[startIndex];
        if (*basename == '/') {
            startIndex++;
            basename = &path[startIndex];
            break;
        }
        if (startIndex == 0) {
            break;
        }
        startIndex--;
    }

    endIndex = startIndex;
    do {
        endIndex++;
        if (endIndex >= pathLength) {
            break;
        }
    } while (path[endIndex] != '.');

    copyLength = endIndex - startIndex;
    capacity--;
    if (capacity < copyLength) {
        copyLength = capacity;
    }
    memcpy(destination, basename, copyLength);
    destination[copyLength] = '\0';
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
#define MDL_PART_PACKET_COMMAND 1
#define MDL_PART_INFO_TAG 0x30424950

/* A part-info packet has command 1 and the PIB0 tag in its inline packet header. */
s32 mdlIsInnerSentinel(PacWork *packetWork) {
    s32 isSentinel = 0;

    if (((PacHead *)packetWork->packet)->command == MDL_PART_PACKET_COMMAND) {
        isSentinel = ((PacHead *)packetWork->packet)->tag == MDL_PART_INFO_TAG;
    }
    return isSentinel;
}



extern PacWork *mdlApplyQueuedGroupPackets(PacWork *, s32, s32, s32);
INCLUDE_ASM(const s32, "model/mdlManager", mdlApplyQueuedGroupPackets);

extern PacWork *func_00218768(PacWork *, s32, s32, s32);
INCLUDE_ASM(const s32, "model/mdlManager", func_00218768);

/* Clean up the retained dispatch request, then release its completion job. */
void mdlDestroyLoadRequestOwner(MdlDoneJob *ownerBlock) {
    func_00288788(ownerBlock->owner);
    sdfReleaseChipBlock(ownerBlock);
}


/* Request slot handed back by fileAllocateDispatchRequest; its +0x60 word feeds the load apply. */
typedef struct MdlLoadSlot {
    u8 pad00[0x60];
    PacWork *handle; /* 0x60 */
} MdlLoadSlot;

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
        mdlDestroyLoadRequestOwner(completionJob);
    }
}

extern void *sdfAllocAndClearQuadwords();
extern void *fileAllocateDispatchRequest();
extern void func_00288C50();
extern void mdlCompleteGroupedJobAndNotify();

#define MDL_DONE_JOB_BYTES 0x14

/* Allocate a completion job and dispatch the request. Group/id narrow to u16.
 * Without onComplete, run the existing no-callback completion path and clean up
 * here; otherwise the completion callback path owns cleanup. Always return zero.
 * Preserve the provider's existing short-arity/unprototyped calling convention. */
s32 mdlRequestLoadWithCallback(s32 group, s32 id, s32 jobArg, s32 requestHandle, void (*onComplete)(u32), u32 callbackArg) {
    MdlDoneJob *completionJob = sdfAllocAndClearQuadwords(MDL_DONE_JOB_BYTES);
    void *requestSlot;

    completionJob->group = group;
    completionJob->id = id;
    completionJob->arg = jobArg;
    completionJob->doneArg = callbackArg;
    completionJob->done = onComplete;
    requestSlot = fileAllocateDispatchRequest(requestHandle, 0, 0, mdlCompleteGroupedJobAndNotify, completionJob);
    completionJob->owner = requestSlot;
    if (onComplete == NULL) {
        func_00288C50(requestSlot);
        mdlDestroyLoadRequestOwner(completionJob);
    }
    return 0;
}

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB60);

INCLUDE_SDATA(const s32, "model/mdlManager", D_003BBB68);

