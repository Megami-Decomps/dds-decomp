#include "common.h"
#include "pcp_vu0.h"
#include "mdl.h"

struct FileWork;
extern u32 fileGetResourceHandle(struct FileWork *);
extern u32 fileGetLoadedDataAddress(struct FileWork *);
extern DevRequest *sndBuildResourceHandleListFromOffsets(const void *);
extern void filePollEntryCleanup(struct FileWork *);

extern u32 mdlGroupJobSemaphore;

extern BattleGroupNode *btlFindGroupedEntity(s32, s32);



extern void sdfDestroyMotion(Motion *arg);

extern s32 btlGroupContainsId(s32 group, s32 id);

extern s32 fileManUpdate(void);

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


extern void btlCreateGroupNode(s32, s32, s32, DevRequest *, void *, s32);


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
