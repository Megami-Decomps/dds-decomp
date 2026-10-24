#include "sdk_ps2.h"
#include "mdl_dev_slot_color.h"
#include "sdf_model_packet_callback.h"
#include "ee_mmi.h"
#include "common.h"
#include "sdf_vu0.h"
#include "btl_group_id_api.h"
#include "sdf_resource_list_release_api.h"
#include "btl_stage_task_cleanup.h"
#include "mdl_motion_api.h"
#include "mdl_motion_end_status.h"
#include "mdl_part_tag_record.h"
#include "sdf_motion.h"
#include "sdf_texture_draw_packet.h"
#include "fr_font.h"
#include "sdf_packet_list.h"
#include "sdf_chip.h"
#include "snd_slot.h"
#include "sdf_resource.h"
#include "file_pac.h"
#include "btl_state.h"
#include "btl_model_record.h"
#include "btl_sound.h"
#include "btl_task_args.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_projection.h"
#include "sdf_linked_packet.h"
#include "sdf_pac_packet.h"
#include "sdf_packet_builders.h"
#include "mdl.h"
#include "btl_grouped_entity_api.h"
#include "sdf_model.h"
#include "sdf_model_copy.h"
#include "sdf_dev_slot_api.h"
#include "mdl_resource_entries.h"
#include "mdl_asset_request.h"
#include "mdl_resource_table.h"
#include "evt_unit.h"
#include "evt_event_pack.h"
#include "evt_task.h"
#include "scr.h"
#include "dat_state.h"
#include "dat_command.h"
#include "file_request_api.h"
#include "file_manager_api.h"
#include "btl_group_id_entry.h"
#include "mdl_grouped_parts_api.h"
#include "mdl_done_job.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"

#define BTL_GROUP_COUNT 8

#define BTL_GROUP_RESOURCE_SLOT_COUNT 8

#define BTL_GROUP_OWNS_RESOURCES_FLAG 1
extern u32 mdlManagerWork;

extern dds3ModelResrc_t *mdlResrcMajorList[];

extern BattleGroupIdEntry *mdlLoadingMajorList[];


extern void mdlEffectResrcListFree(DevRequest *);


/* Create the model-job semaphore and clear both lists for all eight groups. */
void mdlInitManager(void) {
    s32 groupIndex;

    mdlManagerWork = sdfCreateSema(1, 0x7f, 0);
    for (groupIndex = 0; groupIndex != 8; groupIndex++) {
        mdlResrcMajorList[groupIndex] = 0;
        mdlLoadingMajorList[groupIndex] = 0;
    }
}

/* Return the first node of this type in group, or NULL. */
dds3ModelResrc_t *mdlSearchResrc(s32 group, s32 type) {
    dds3ModelResrc_t *entry = mdlResrcMajorList[group];
    while (entry != 0) {
        if (entry->type == type) {
            break;
        }
        entry = entry->next;
    }
    return entry;
}

/* Return whether this group's separate ID list contains id. */
s32 mdlCheckLoading(s32 groupIndex, s32 wantedId) {
    BattleGroupIdEntry *idCursor = mdlLoadingMajorList[groupIndex];
    while (idCursor != 0) {
        if (idCursor->id == wantedId) {
            return 1;
        }
        idCursor = idCursor->next;
    }
    return 0;
}

/* Prepend id to this group's ID list; duplicate IDs are allowed. */
void mdlRegLoading(s32 groupIndex, s32 newId) {
    BattleGroupIdEntry *idEntry = sdfChipAllocDirect(sizeof(BattleGroupIdEntry));
    BattleGroupIdEntry **groupHead = &mdlLoadingMajorList[groupIndex];
    idEntry->id = newId;
    idEntry->next = *groupHead;
    *groupHead = idEntry;
}

/* Remove the first matching ID using its incoming link, including the head. */
void mdlUnRegLoading(s32 groupIndex, s32 wantedId) {
    BattleGroupIdEntry **idLink;
    BattleGroupIdEntry *idCursor;

    idLink = &mdlLoadingMajorList[groupIndex];
    idCursor = *idLink;
    if (idCursor == 0) {
        return;
    }
    do {
        if (idCursor->id == wantedId) {
            *idLink = idCursor->next;
            sdfChipFreeDirect(idCursor);
            break;
        } else {
            idLink = &idCursor->next;
            idCursor = idCursor->next;
        }
    } while (idCursor != 0);
}

/* Replace the group/type node; only flags bit 0 selects resource ownership. */
void mdlRegist(s32 groupIndex, s32 entityType, s32 ownershipFlags, DevRequest *resourceList, struct sdfModelDef_t *itemList, struct sdfMemHandle_t *requestAllocation) {
    dds3ModelResrc_t *groupNode;
    dds3ModelResrc_t *previousHead;
    s32 slotIndex;
    mdlUnRegist(groupIndex, entityType);
    groupNode = sdfChipAllocDirect(sizeof(dds3ModelResrc_t));
    previousHead = mdlResrcMajorList[groupIndex];
    if (previousHead != NULL) {
        previousHead->prev = groupNode;
    }
    mdlResrcMajorList[groupIndex] = groupNode;
    groupNode->next = previousHead;
    groupNode->group = groupIndex;
    groupNode->type = entityType;
    groupNode->resourceList = resourceList;
    groupNode->itemList = itemList;
    groupNode->requestAllocation = requestAllocation;
    groupNode->prev = NULL;
    groupNode->modelContext = NULL;
    for (slotIndex = 0; slotIndex != BTL_GROUP_RESOURCE_SLOT_COUNT; slotIndex++) {
        groupNode->slots[slotIndex].flags = 0;
        groupNode->slots[slotIndex].motionTable = NULL;
        groupNode->slots[slotIndex].resourceHandle = 0;
    }
    groupNode->ownsResources = ownershipFlags & BTL_GROUP_OWNS_RESOURCES_FLAG;
    groupNode->resourceHandle = 0;
    groupNode->partInfo = NULL;
    groupNode->partList = NULL;
    groupNode->unk_AC = 1.0f;
    groupNode->objectRadius = 100.0f;
}

/* Capture ownership before clearing it for callbacks; keep the context-release
 * loop and resource-release order intact. NULL is allowed. */
void mdlUnRegistResrc(dds3ModelResrc_t *groupNode) {
    dds3ModelResrc_t *previousNode;
    dds3ModelResrc_t *nextNode;
    u8 ownsResources;
    s32 slotIndex;
    if (groupNode == NULL) {
        return;
    }
    previousNode = groupNode->prev;
    nextNode = groupNode->next;
    if (previousNode == NULL) {
        mdlResrcMajorList[groupNode->group] = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    if (nextNode != NULL) {
        nextNode->prev = previousNode;
    }
    ownsResources = groupNode->ownsResources;
    groupNode->ownsResources = 0;
    if (groupNode->modelContext != NULL) {
        do {
            mdlFree(groupNode->modelContext);
        } while (groupNode->modelContext != NULL);
    }
    if (ownsResources != 0) {
        sdfTexListFree(groupNode->resourceList, 1);
        sdfMemFreeDelayedDirect(groupNode->requestAllocation);
        for (slotIndex = 0; slotIndex != BTL_GROUP_RESOURCE_SLOT_COUNT; slotIndex++) {
            if (groupNode->slots[slotIndex].resourceHandle != 0) {
                sdfMemFreeDirect(groupNode->slots[slotIndex].resourceHandle);
            }
        }
    }
    mdlEffectResrcListFree(groupNode->partList);
    sdfMemFreeDirect(groupNode->resourceHandle);
    sdfChipFreeDirect(groupNode);
}

/* Forward the group/type pair explicitly, then destroy its node if present. */
void mdlUnRegist(s32 groupIndex, s32 entityType) {
    dds3ModelResrc_t *groupNode;

    groupNode = mdlSearchResrc(groupIndex, entityType);
    mdlUnRegistResrc(groupNode);
}

/* Destroy every group node, saving the next link before each unlink/free. */
void mdlUnRegistAll(void) {
    u32 groupIndex = 0;
    dds3ModelResrc_t **groupHead = mdlResrcMajorList;
    do {
        dds3ModelResrc_t *groupNode = *groupHead;
        while (groupNode != 0) {
            dds3ModelResrc_t *nextNode = groupNode->next;
            mdlUnRegistResrc(groupNode);
            groupNode = nextNode;
        }
        groupIndex++;
        groupHead++;
    } while (groupIndex < BTL_GROUP_COUNT);
}

/* Create and attach the motion selected by the resource record's search ID. */
sdfMotionCtrlr_t *mdlCreateMotion(dds3ModelHandle_t *owner, s32 searchId) {
    MotionTable *motionTable = owner->sub->slots[searchId].motionTable;
    s16 motionSlotIndex = owner->sub->slots[searchId].motionSlotIndex;
    sdfMotionCtrlr_t *object = sdfMotionCtrlrNew(owner->inner, motionTable);

    object->searchId = searchId;
    owner->slots[motionSlotIndex] = object;
    object->slotIndex = motionSlotIndex;
    if (motionSlotIndex == 0) {
        owner->first = object;
    }
    return object;
}

#include "common.h"
#include "sdf_chip.h"
#include "file_request_api.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "sdf_texture_offset_list.h"


extern u32 mdlManagerWork;

void mdlFreeMotionCtrlr(dds3ModelHandle_t *ctx, sdfMotionCtrlr_t *node) {
    if (ctx->slots[node->slotIndex] == node) {
        ctx->slots[node->slotIndex] = NULL;
    }
    sdfMotionCtrlrFree(node);
}

void mdlFreeMotion(dds3ModelHandle_t *ctx, s32 searchId) {
    sdfMotionCtrlr_t *node = ctx->inner->motionList;

    while (node != NULL) {
        if (node->searchId == searchId) {
            mdlFreeMotionCtrlr(ctx, node);
            break;
        }
        node = node->next;
    }
}



/* Release the grouped resource record and matching motions for `searchId`. */
void mdlUnRegistResrcMotion(dds3ModelResrc_t *owner, s32 searchId) {
    dds3ModelHandle_t *ctx;

    if (owner != NULL) {
        if (owner->slots[searchId].motionTable == NULL) {
            return;
        }
        for (ctx = owner->modelContext; ctx != NULL; ctx = ctx->next) {
            mdlFreeMotion(ctx, searchId);
        }
        if (owner->ownsResources != 0) {
            if (owner->slots[searchId].resourceHandle != 0) {
                sdfMemFreeDirect(owner->slots[searchId].resourceHandle);
            }
        }
        owner->slots[searchId].motionTable = NULL;
        owner->slots[searchId].resourceHandle = 0;
    }
}

void mdlUnRegistMotion(s32 group, s32 id, s32 searchId) {
    dds3ModelResrc_t *entity;

    entity = mdlSearchResrc(group, id);
    mdlUnRegistResrcMotion(entity, searchId);
}

void mdlRegistMotion(s32 group, s32 id, u32 mode, s32 motionIndex, s32 searchId, s32 slotIndex, MotionTable *data, struct sdfMemHandle_t *resourceHandle) {
    dds3ModelResrc_t *owner = mdlSearchResrc(group, id);
    BattleGroupSlot *slot;

    mdlUnRegistResrcMotion(owner, searchId);
    slot = &owner->slots[searchId];
    slot->motionSlotIndex = slotIndex;
    slot->motionIndex = motionIndex;
    slot->motionTable = data;
    slot->resourceHandle = resourceHandle;
    slot->flags = 0;
    if (mode & 0x100) {
        slot->flags = 1;
    }
    if (mode & 0x200) {
        slot->flags |= 2;
    }
}


void mdlRegistModelSet(s32 group, s32 id, s32 mode, MdlLoadPayload *setup) {
    dds3ModelResrc_t *entity;

    mdlRegist(group, id, mode, setup->resourceList, setup->itemList, setup->requestAllocation);
    if (setup->motionTable != NULL) {
        mdlRegistMotion(group, id, mode, 0, 0, 0, setup->motionTable, setup->motionResource);
    }
    if (setup->partInfo != NULL) {
        entity = mdlSearchResrc(group, id);
        entity->partInfo = setup->partInfo;
        entity->resourceHandle = setup->resourceHandle;
        entity->partList = setup->partList;
    }
}

dds3ModelResrc_t *mdlWaitLoadEnd(s32 group, s32 id) {
    while (mdlCheckLoading(group, id)) {
        fileManager();
    }
    return mdlSearchResrc(group, id);
}


void TbMbAbLoadEnd(MdlLoadRequest *request) {
    mdlRegistModelSet(request->group, request->id, request->options, &request->payload);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(request->group, request->id);
    SignalSema(mdlManagerWork);
    sdfChipFreeDirect(request);
}

void TbLoadCallback(struct FileRequest *resource, MdlLoadRequest *destination) {
    const SdfTextureOffsetListHeader *loadedData;
    DevRequest *resourceList;

    loadedData = fileGetLoadPtr(resource);
    resourceList = sdfTexSetTB(loadedData);
    destination->payload.resourceList = resourceList;
    sdfMemFreeDirect(fileGetMemHandle(resource));
    fileHandleFree(resource);
}

#include "common.h"
#include "sdf_chip.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_work.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "file_request_api.h"

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F650[];

extern u8 D_0037F660[];



extern sdfMotionCtrlr_t *mdlGetMotionCtrlr(dds3ModelHandle_t *ctx, s32 searchId);



extern MdlResourceTable D_003C86B0[];
extern MdlResourceTable D_003C6588[];


/* 8-byte prefix copied from D_003BBB60 by mdlSetFileName. */
typedef struct Hdr8 {
    u8 b[8];
} Hdr8;

extern u8 D_00436FA0[];


void mdlMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames);


extern void TbMbAbLoadEnd(MdlLoadRequest *);

/* Retain the resource handle, relocate the loaded payload and retire the file
 * entry. Execute the group job now only when the command is not deferred. */
void MbLoadCallback(struct FileRequest *resource, MdlLoadRequest *request) {
    request->payload.requestAllocation = fileGetMemHandle(resource);
    request->payload.itemList = (struct sdfModelDef_t *)sdfRelocatePackedResourcePayload(
        (SdfPackedRelocationHeader *)fileGetLoadPtr(resource));
    fileHandleFree(resource);
    if (request->deferred == 0) {
        TbMbAbLoadEnd(request);
    }
}

/* Retain the handle and relocated motion data, retire the file entry, then run
 * the group job. This callback completes the additional file request. */
void AbLoadCallback(struct FileRequest *resource, MdlLoadRequest *request) {
    request->payload.motionResource = fileGetMemHandle(resource);
    request->payload.motionTable = (struct MotionTable *)sdfRelocatePackedResourceWordsFromHeader(
        (SdfPackedRelocationHeader *)fileGetLoadPtr(resource));
    fileHandleFree(resource);
    TbMbAbLoadEnd(request);
}

/* Copy the fixed eight-byte prefix, then append src. Prefix termination and
 * sufficient destination capacity are obligations of the data/caller. */
char *mdlSetFileName(char *dst, const char *src) {
    *(Hdr8 *)dst = *(Hdr8 *)D_00436FA0;
    return strcat(dst, src);
}

/* The nonblocking API returns 0/-1 status or a ready group address word. */
extern u32 mdlManagerWork;
extern dds3ModelResrc_t *mdlWaitLoadEnd(s32 group, s32 id);
extern const char D_00436FA8[];
extern void sdfCreateSemaphoreFromOptions(u32 callbackArg);
extern void TbLoadCallback(struct FileRequest *, MdlLoadRequest *);
extern s32 mdlFileLoad(s32 group, s32 id, s32 jobArg,
                                     const char *requestPath,
                                     void (*onComplete)(u32), u32 callbackArg);

/* Return a ready group address or queue its resources. Pending and new
 * requests each perform their own optional blocking wait after unlocking. */
s32 mdlPrepare(s32 group, s32 id, s32 blocking) {
    char pathBuffer[0x80];
    dds3ModelResrc_t *entity;
    MdlResourceSelection *selection;
    MdlResourcePath *path;
    MdlLoadRequest *request;
    const char *extension;
    s32 packed;

    WaitSema(mdlManagerWork);
    entity = mdlSearchResrc(group, id);
    if (entity != NULL) {
        SignalSema(mdlManagerWork);
        return (s32)entity;
    }
    if (mdlCheckLoading(group, id) != 0) {
        SignalSema(mdlManagerWork);
        if (blocking != 0) {
            return (s32)mdlWaitLoadEnd(group, id);
        }
        return -1;
    }
    selection = &((MdlResourceSelection *)D_003C86B0[group].entries)[id];
    path = &((MdlResourcePath *)D_003C6588[selection->pathTable].entries)[selection->pathIndex];
    packed = 0;
    extension = strrchr(path->path, '.');
    if (extension != NULL) {
        packed = strcasecmp(extension, D_00436FA8) == 0;
    }
    mdlRegLoading(group, id);
    SignalSema(mdlManagerWork);
    if (packed != 0) {
        mdlSetFileName(pathBuffer, path->path);
        mdlFileLoad(group, id, 0x101, pathBuffer,
                                  sdfCreateSemaphoreFromOptions, 0);
    } else {
        request = sdfAllocAndClearQuadwords(sizeof(MdlLoadRequest));
        request->options = 0x101;
        request->group = group;
        request->id = id;
        if (path->motionPath != NULL) {
            request->deferred = 1;
        }
        if (path->resourceListPath != NULL) {
            request->resourceListRequested = 1;
            mdlSetFileName(pathBuffer, path->resourceListPath);
            fileLoadEx(pathBuffer, 0,
                                      (s32)TbLoadCallback, (s32)request);
        }
        request->itemsRequested = 1;
        mdlSetFileName(pathBuffer, path->path);
        fileLoadEx(pathBuffer, 0, (s32)MbLoadCallback, (s32)request);
        if (path->motionPath != NULL) {
            mdlSetFileName(pathBuffer, path->motionPath);
            fileLoadEx(pathBuffer, 0, (s32)AbLoadCallback, (s32)request);
        }
    }
    if (blocking != 0) {
        return (s32)mdlWaitLoadEnd(group, id);
    }
    return 0;
}


/* Blocking requests return the ready group through the SDK address-word API. */
dds3ModelResrc_t *mdlLoadPrepare(u32 group, u32 id) {
    return (dds3ModelResrc_t *)mdlPrepare(group, id, 1);
}

/* Unlink without clearing this context's own links. A resource-owning group is
 * destroyed when its last context leaves. */
void mdlUnlinkResrc(dds3ModelHandle_t *ctx) {
    dds3ModelHandle_t *nextContext = ctx->next;
    dds3ModelHandle_t *previousContext = ctx->previous;
    dds3ModelResrc_t *group;

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
                mdlUnRegistResrc(group);
            }
        }
    }
}

extern void mdlFreeBlur(dds3ModelHandle_t *ctx);

/* vu0 routine: initialize the slot-list endpoint color and interpolation step. */
void mdlSetBlur(dds3ModelHandle_t *ctx, s32 stride, s32 capacity,
                         u32 endColor, u32 startColor) {
    MdlDevList *deviceList;
    f32 inverseSpan;
    /* Both retail versions home the packed colors 16 bytes apart. */
    u32 startCarrier[4];
    u32 endCarrier[4];

    mdlFreeBlur(ctx);
    if (capacity > 0) {
        deviceList = sdfAllocAndClearQuadwords(sizeof(MdlDevList));
        deviceList->growStep = 1;
        ctx->devList = deviceList;
        deviceList->usedCount = capacity;
        deviceList->capacity = capacity;
        deviceList->stride = stride;
        /* Final argument use: see docs/idioms.md, "Final argument use: mdlCreateDevSlotList". */
        deviceList->byteCount = capacity * stride;
        inverseSpan = 1.0f / deviceList->byteCount;
        startCarrier[0] = startColor;
        EE_MMI_RGBA_UNPACK(startCarrier, 0.0078125f);
        SDF_VU0_STOREVECTOR(deviceList->color);
        SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
        endCarrier[0] = endColor;
        EE_MMI_RGBA_UNPACK(endCarrier, 0.0078125f);
        SDF_VU0_SUBVECTOR_EXTENDED();
        VU0_SCALAR_OP(inverseSpan, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_STOREVECTOR(deviceList->colorStep);
    }
}

/* Release every device slot and its list node, then the list owner. A missing
 * list is a no-op; a released list is removed from the context. */
void mdlFreeBlur(dds3ModelHandle_t *ctx) {
    MdlDevList *deviceList = ctx->devList;
    MdlDevSlot *nextSlot;
    MdlDevSlot *currentSlot;

    if (deviceList != NULL) {
        nextSlot = deviceList->first;
        while (nextSlot != NULL) {
            currentSlot = nextSlot;
            nextSlot = nextSlot->next;
            sdfModelRootFree(currentSlot->model, 1, 1);
            sdfChipFreeDirect(currentSlot);
        }
        sdfChipFreeDirect(deviceList);
        ctx->devList = NULL;
    }
}


/* Grow the afterimage list, advance each model, then apply its age and tint. */
void mdlLinkBlur(dds3ModelHandle_t *ctx, struct SdfPoolNode **surfaces) {
    MdlDevList *list = ctx->devList;
    sdfModelRootNode_t *original = ctx->inner;
    MdlDevSlot *slot;
    sdfModelRootNode_t *model;
    f32 tint[4];
    s32 remaining;
    u32 color;

    if (list->usedCount != 0 && --list->growStep == 0) {
        list->usedCount--;
        list->growStep = list->stride;
        slot = sdfChipAllocDirect(sizeof(MdlDevSlot));
        model = sdfModelMake(ctx->sub->resourceList, ctx->sub->itemList);
        /* Finish initialization before publishing the new list head. */
        slot->next = list->first;
        slot->model = model;
        slot->remaining = 1;
        model->modelScale = 1000.0f;
        list->first = slot;
    }
    eeUnpackRGBAWordFloat(original->color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR(tint);
    slot = list->first;
    while (slot != NULL) {
        remaining = slot->remaining;
        model = slot->model;
        remaining--;
        if (remaining == 0) {
            remaining = list->byteCount;
            sdfModelCopyStat(model, original);
        } else {
            sdfModelEvalTm(model);
        }
        /* Both callbacks complete before the countdown is published. */
        slot->remaining = remaining;
        SDF_VU0_LOADVECTOR(list->color);
        SDF_VU0_LOADVECTOR_B(list->colorStep);
        SDF_VU0_LOADVECTOR_C(tint);
        color = mdlPackDevSlotColor(remaining - 1, 128.0f);
        model->color = color;
        sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, surfaces, model);
        slot = slot->next;
    }
}

extern DevRequest *sdfTexListDup(DevRequest *list);
extern sdfMotionCtrlr_t *mdlCreateMotion(dds3ModelHandle_t *owner, s32 searchId);

/* Instantiate a model context from the group's resource list and item list, then
 * start every populated slot's motion owner. */
dds3ModelHandle_t *mdlLoad(u32 group, u32 id) {
    dds3ModelResrc_t *node = mdlLoadPrepare(group, id);
    dds3ModelHandle_t *ctx = sdfAllocAndClearQuadwords(sizeof(dds3ModelHandle_t));
    dds3ModelHandle_t *head;
    sdfModelRootNode_t *inner;
    f32 modelScale;
    f32 scale[4];
    s32 searchId;

    ctx->sub = node;
    head = node->modelContext;
    ctx->next = head;
    if (head != NULL) {
        head->previous = ctx;
    }
    node->modelContext = ctx;
    inner = sdfModelMake(sdfTexListDup(node->resourceList), node->itemList);
    modelScale = node->unk_AC;
    ctx->inner = inner;
    scale[0] = modelScale;
    scale[1] = modelScale;
    scale[2] = modelScale;
    scale[3] = 1.0f;
    SDF_VU0_COPYVECTOR(inner->scaleVector, scale);
    ctx->objectRadius = node->objectRadius;
    ctx->current.h.id = -1;
    ctx->current.h.motionIndex = -1;
    for (searchId = 0; searchId != 8; searchId++) {
        if (node->slots[searchId].motionTable != NULL && (node->slots[searchId].flags & 1)) {
            mdlCreateMotion(ctx, searchId);
            if (searchId > 0) {
                if (node->slots[searchId].motionIndex >= 0) {
                    mdlLoopMotion(ctx, searchId, node->slots[searchId].motionIndex);
                }
            }
        }
    }
    ctx->resourceItems = NULL;
    mdlGenEffect(ctx, -1, MDL_CONTEXT_WIDE_MOTION_SLOT);
    return ctx;
}


extern void mdlFreeEffect(dds3ModelEffectCtrlr_t *);


/* Destroy motions, resources, device slots and the context itself. sdfMotionCtrlr_t
 * destruction must unlink inner->motionList; save each resource's next link before
 * destroying it. ctx and inner are required, not checked here. */
void mdlFree(dds3ModelHandle_t *ctx) {
    sdfModelRootNode_t *inner = ctx->inner;
    dds3ModelEffectCtrlr_t *resourceNode;
    dds3ModelEffectCtrlr_t *nextResource;

    while (inner->motionList != NULL) {
        sdfMotionCtrlrFree(inner->motionList);
    }
    sdfTexListFree(inner->assetData, 1);
    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = nextResource) {
        nextResource = resourceNode->next;
        mdlFreeEffect(resourceNode);
    }
    mdlFreeBlur(ctx);
    sdfModelRootFree(inner, 1, 1);
    mdlUnlinkResrc(ctx);
    sdfChipFreeDirect(ctx);
}

struct SdfPoolNode;
extern void mdlLinkBlur(dds3ModelHandle_t *, struct SdfPoolNode **);
extern void mdlLinkEffect(dds3ModelHandle_t *, dds3ModelEffectCtrlr_t *);

/* Per-frame update: step the active slot nodes, refresh the transforms, dispatch anchor records. */
void mdlLink(dds3ModelHandle_t *ctx, struct SdfPoolNode **surfaces) {
    sdfMotionCtrlr_t **slot = ctx->slots;
    sdfModelRootNode_t *inner;
    dds3ModelEffectCtrlr_t *rec;
    s32 i;

    for (i = 0; i != 4; i++) {
        if (*slot != NULL) {
            if ((*slot)->state != SDF_MOTION_STATE_UNINITIALIZED) {
                sdfMotionNext(*slot);
            }
        }
        slot++;
    }
    if (ctx->flags & MDL_SKIP_TRANSFORMS) {
        return;
    }
    inner = ctx->inner;
    sdfModelEvalTm(inner);
    sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, surfaces, inner);
    if (ctx->flags & MDL_SKIP_ANCHORS) {
        return;
    }
    if (ctx->flags & MDL_REQUIRE_ANCHOR_ENABLE) {
        if ((inner->flags & SDF_MODEL_HAS_ANCHOR_RECORDS) == 0) {
            return;
        }
    }
    for (rec = ctx->resourceItems; rec != NULL; rec = rec->next) {
        mdlLinkEffect(ctx, rec);
    }
    if (ctx->devList == NULL) {
        return;
    }
    mdlLinkBlur(ctx, surfaces);
}

extern void LoadIdentity(void);
extern void RotateMatrixX(f32 angle);
extern void RotateMatrixY(f32 angle);

#define MDL_MOTION_SLOT_COUNT 4
#define MDL_NO_BLEND_ENTRY (-1)
#define MDL_RADIANS_PER_DEGREE 0.017453293f
#define MDL_FULL_BLEND_PITCH 25.0f
#define MDL_ENTRY_ENABLED 1
#define MDL_PRIMARY_MOTION_SLOT 0
#define MDL_MOTION_LOOP_ENABLED 1
#define MDL_MOTION_LOOP_DISABLED 0

/* Blend the selected basis towards pitch/yaw (degrees), then update transforms
 * and anchors. For abs(pitch)<25, weight is abs(pitch)/25; otherwise one;
 * -1 skips basis blending. Slot motions/blending precede the skip flags.
 * surfaces is forwarded unchanged to the remaining update routines. */
void mdlBlendEntryPitchYawAndUpdate(dds3ModelHandle_t *ctx, struct SdfPoolNode **surfaces, s32 entryIndex, f32 pitch, f32 yaw) {
    sdfModelRootNode_t *inner;
    sdfModelNode_t *entry = NULL;
    f32 targetRows[4][4];
    f32 pitchMagnitude;
    f32 targetWeight;
    f32 existingWeight;
    dds3ModelEffectCtrlr_t *resourceNode;
    s32 slotIndex;

    inner = ctx->inner;
    if (entryIndex != MDL_NO_BLEND_ENTRY) {
        entry = ((sdfModelNode_t **)inner->list->buffer)[entryIndex];
        LoadIdentity();
        RotateMatrixX(pitch * MDL_RADIANS_PER_DEGREE);
        RotateMatrixY(yaw * MDL_RADIANS_PER_DEGREE);
        VU0_STORE_MATRIX(targetRows);
    }
    for (slotIndex = 0; slotIndex != MDL_MOTION_SLOT_COUNT; slotIndex++) {
        if (ctx->slots[slotIndex] != NULL) {
            if (ctx->slots[slotIndex]->state != SDF_MOTION_STATE_UNINITIALIZED) {
                sdfMotionNext(ctx->slots[slotIndex]);
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
    sdfModelEvalTm(inner);
    sdfBuildModelPacketsVia(sdfModelBuildNodeResourcePackets, surfaces, inner);
    if (ctx->flags & MDL_SKIP_ANCHORS) {
        return;
    }
    if (ctx->flags & MDL_REQUIRE_ANCHOR_ENABLE) {
        if ((inner->flags & SDF_MODEL_HAS_ANCHOR_RECORDS) == 0) {
            return;
        }
    }
    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
        mdlLinkEffect(ctx, resourceNode);
    }
    if (ctx->devList == NULL) {
        return;
    }
    mdlLinkBlur(ctx, surfaces);
}

/* Enable each table entry. The signed table count governs iteration; entry
 * pointers and the context's inner/table pointers are required. */
void mdlEnableAllEntries(dds3ModelHandle_t *ctx) {
    DevRequest *table = ctx->inner->list;
    s32 entryCount = table->usedCount;
    sdfModelNode_t **entries = table->buffer;
    s32 entryIndex;

    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        entries[entryIndex]->flags = MDL_ENTRY_ENABLED;
    }
}

extern sdfMotionCtrlr_t *mdlCreateMotion(dds3ModelHandle_t *owner, s32 searchId);
/* Select the first matching searchId, or create it, and make it current in its
 * signed slot index. Allocation success/slot bounds are assumed. Slot zero
 * also becomes ctx->first; id and motionIndex narrow into the current pair. */
void mdlMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, s32 loopEnabled,
                                     f32 blendLeadFrames, f32 blendDurationFrames) {
    sdfMotionCtrlr_t *node;
    s16 slotIndex;

    for (node = ctx->inner->motionList; node != NULL; node = node->next) {
        if (node->searchId == searchId) {
            break;
        }
    }
    if (node == NULL) {
        node = mdlCreateMotion(ctx, searchId);
    }
    slotIndex = node->slotIndex;
    ctx->slots[slotIndex] = node;
    if (slotIndex == MDL_PRIMARY_MOTION_SLOT) {
        ctx->first = node;
    }
    sdfMotionChangeBlend(node, motionIndex, loopEnabled, blendLeadFrames, blendDurationFrames);
    ctx->current.h.id = searchId;
    ctx->current.h.motionIndex = motionIndex;
    mdlStopEffect(ctx, slotIndex);
    mdlGenEffect(ctx, motionIndex, slotIndex);
}

/* Select a looping motion with both blend-frame parameters zero. */
void mdlLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex) {
    mdlMotion(ctx, searchId, motionIndex, MDL_MOTION_LOOP_ENABLED, 0.0f, 0.0f);
}

/* Select a nonlooping motion with both blend-frame parameters zero. */
void mdlSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex) {
    mdlMotion(ctx, searchId, motionIndex, MDL_MOTION_LOOP_DISABLED, 0.0f, 0.0f);
}

/* Select a looping motion with the supplied blend-frame parameters. */
void mdlSmoothLoopMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                          f32 blendDurationFrames) {
    mdlMotion(
        ctx, searchId, motionIndex, MDL_MOTION_LOOP_ENABLED, blendLeadFrames, blendDurationFrames);
}

/* Select a nonlooping motion with the supplied blend-frame parameters. */
void mdlSmoothSingleMotion(dds3ModelHandle_t *ctx, s32 searchId, s32 motionIndex, f32 blendLeadFrames,
                        f32 blendDurationFrames) {
    mdlMotion(
        ctx, searchId, motionIndex, MDL_MOTION_LOOP_DISABLED, blendLeadFrames, blendDurationFrames);
}

sdfMotionCtrlr_t *mdlGetMotionCtrlr(dds3ModelHandle_t *ctx, s32 searchId) {
    sdfMotionCtrlr_t *node;
    for (node = ctx->inner->motionList; node != NULL; node = node->next) {
        if (node->searchId == searchId) {
            return node;
        }
    }
    return NULL;
}

static inline const sdfMotionCtrlr_t *mdlFindNodeByIdReadOnly(const dds3ModelHandle_t *ctx, s32 searchId) {
    /* The mutable lookup remains available to writers; this path only reads. */
    return mdlGetMotionCtrlr((dds3ModelHandle_t *)ctx, searchId);
}

#define MDL_NODE_FIELD_MISSING (-1)

/* Read the motion selector, widened to s32. Missing nodes return -1,
 * distinct from a present selector of 0xFFFF. */
s32 mdlGetMotionNum(const dds3ModelHandle_t *ctx, s32 searchId) {
    const sdfMotionCtrlr_t *matchedNode = mdlFindNodeByIdReadOnly(ctx, searchId);
    if (matchedNode == NULL) {
        return MDL_NODE_FIELD_MISSING;
    }
    return matchedNode->motionIndex;
}

/* Read the selected motion's frame count; return zero for a missing node. */
u16 mdlGetMotionLengthEx(const dds3ModelHandle_t *ctx, s32 searchId) {
    const sdfMotionCtrlr_t *matchedNode = mdlFindNodeByIdReadOnly(ctx, searchId);
    if (matchedNode == NULL) {
        return 0;
    }
    return matchedNode->frameCount;
}

/* Numerically convert the stored float to s32, not a bit reinterpretation.
 * Return zero when the searched node is absent. */
s32 mdlGetMotionFrameEx(const dds3ModelHandle_t *ctx, s32 searchId) {
    const sdfMotionCtrlr_t *matchedNode = mdlFindNodeByIdReadOnly(ctx, searchId);
    if (matchedNode == NULL) {
        return 0;
    }
    return (s32)matchedNode->currentFrame;
}

/* Report its terminal state, or the shared missing-node status. */
MdlMotionEndStatus mdlChkMotionEndEx(const dds3ModelHandle_t *ctx, s32 searchId) {
    const sdfMotionCtrlr_t *matchedNode = mdlFindNodeByIdReadOnly(ctx, searchId);
    if (matchedNode == NULL) {
        return MDL_MOTION_END_NODE_MISSING;
    }
    return matchedNode->state == SDF_MOTION_STATE_TERMINAL;
}

/* Read the stored float, defaulting to zero when the searched node is absent. */
f32 mdlGetMotionSpeedEx(const dds3ModelHandle_t *ctx, s32 searchId) {
    const sdfMotionCtrlr_t *matchedNode = mdlFindNodeByIdReadOnly(ctx, searchId);
    if (matchedNode == NULL) {
        return 0.0f;
    }
    return matchedNode->frameStep;
}

/* Replace the stored float only when the searched node exists. */
void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, s32 searchId, f32 frameStep) {
    sdfMotionCtrlr_t *matchedNode = mdlGetMotionCtrlr(ctx, searchId);
    if (matchedNode != NULL) {
        matchedNode->frameStep = frameStep;
    }
}

/* vu0 routine: load the primary vector into vf10, not a C return value. */
void mdlGetPosition(dds3ModelHandle_t *ctx) {
    void *primaryVector = ctx->inner->matrix[3];
    SDF_VU0_LOADVECTOR_BARRIER(primaryVector);
}

/* vu0 routine: force vf10.w to one and store it as the primary vector. */
void mdlSetPosition(dds3ModelHandle_t *ctx) {
    void *primaryVector;
    SDF_VU0_VECTOR_W_SET_ONE();
    primaryVector = ctx->inner->matrix[3];
    SDF_VU0_STOREVECTOR(primaryVector);
}

/* vu0 routine: load the stored rotation quaternion into vf10. */
void mdlGetRotate(dds3ModelHandle_t *ctx) {
    void *rotationQuaternion = ctx->inner->rotationQuaternion;
    SDF_VU0_LOADVECTOR_BARRIER(rotationQuaternion);
}


/* vu0 routine: consume vf10 as a quaternion, preserve it in the model,
 * then store the resulting basis rows from vf28-vf30. No C argument supplies vf10. */
void mdlSetRotate(dds3ModelHandle_t *ctx) {
    void *rotationQuaternion;
    void *basisRow0;
    void *basisRow1;
    void *basisRow2;

    rotationQuaternion = ctx->inner->rotationQuaternion;
    SDF_VU0_STOREVECTOR(rotationQuaternion);
    effMiscQuaternionToMatrixVU();
    basisRow0 = ctx->inner->matrix[0];
    SDF_VU0_STORE_ROW0(basisRow0);
    basisRow1 = ctx->inner->matrix[1];
    SDF_VU0_STORE_ROW1(basisRow1);
    basisRow2 = ctx->inner->matrix[2];
    SDF_VU0_STORE_ROW2(basisRow2);
}

/* vu0 routine: load the tertiary vector into vf10; projection uses its row scale. */
void mdlGetScale(dds3ModelHandle_t *ctx) {
    void *tertiaryVector = ctx->inner->scaleVector;
    SDF_VU0_LOADVECTOR_BARRIER(tertiaryVector);
}

/* vu0 routine: store all vf10 components as the tertiary vector, without forcing w. */
void mdlSetScale(dds3ModelHandle_t *ctx) {
    void *tertiaryVector = ctx->inner->scaleVector;
    SDF_VU0_STOREVECTOR(tertiaryVector);
}

/* Return the last original value stored by either broadcast path. */
u32 mdlGetColor(dds3ModelHandle_t *ctx) {
    return ctx->inner->color;
}

/* Forward value to each next-linked resource; an empty resource list is a no-op. */
void mdlSetEffectColor(dds3ModelHandle_t *ctx, u32 value) {
    dds3ModelEffectCtrlr_t *resourceNode;

    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
        mdlSetEffectColorSub(ctx, resourceNode, value);
    }
}

void mdlSetColor(dds3ModelHandle_t *ctx, u32 value) {
    ctx->inner->color = value;
    mdlSetEffectColor(ctx, (value & 0xFF000000) | 0x808080);
}

/* Cache the original value and send that same value to every resource. */
void mdlSetColorWithEffect(dds3ModelHandle_t *ctx, u32 value) {
    ctx->inner->color = value;
    mdlSetEffectColor(ctx, value);
}

/* Forward the floating amount to each resource without changing color. */
void mdlSetEffectScale(dds3ModelHandle_t *ctx, f32 amount) {
    dds3ModelEffectCtrlr_t *resourceNode;

    for (resourceNode = ctx->resourceItems; resourceNode != NULL; resourceNode = resourceNode->next) {
        mdlSetEffectScaleSub(ctx, resourceNode, amount);
    }
}

/* vu0 routine: project point with the model/camera matrices and viewport vectors.
 * Row scaling uses live vf10 before loading point; the result is left in vf10.
 * Preserve the distinct matrix-composition order of this single-point path. */
void mdlGetScreenOffset(dds3ModelHandle_t *ctx, const f32 point[4])
{
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_MOVE_VF(vf24, vf28);
    VU0_MOVE_VF(vf25, vf29);
    VU0_MOVE_VF(vf26, vf30);
    VU0_MOVE_VF(vf27, vf31);
    VU0_LOAD_MATRIX(ctx->inner->matrix);
    SDF_VU0_SCALE_XYZ();
    MultMatrixRevNoArg();
    SDF_VU0_LOADVECTOR(point);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F650);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(D_0037F660);
    SDF_VU0_ADDVECTOR();
}

#define MDL_PROJECTION_SCALE_OFFSET 0x40
#define MDL_PROJECTION_BIAS_OFFSET 0x50

/* vu0 routine: project pointCount four-float vectors using the stored tertiary
 * row scale. Matrix setup still runs for nonpositive counts. Output stores all
 * four components; perspective division is not guarded against zero w. */
void mdlProjectPoints(dds3ModelHandle_t *ctx, f32 (*inputPoints)[4], f32 (*outputPoints)[4], s32 pointCount)
{
    s32 pointIndex;

    VU0_LOAD_MATRIX(ctx->inner->matrix);
    SDF_VU0_LOADVECTOR(ctx->inner->scaleVector);
    SDF_VU0_SCALE_XYZ();
    sdfPostmultiplyVuMatrixFromMemory(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    for (pointIndex = 0; pointIndex < pointCount; pointIndex++) {
        SDF_VU0_LOADVECTOR(inputPoints[pointIndex]);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_PERSVECTOR();
        SDF_VU0_LOADVECTOR_B(sdfProjectionMatrix + MDL_PROJECTION_SCALE_OFFSET);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(sdfProjectionMatrix + MDL_PROJECTION_BIAS_OFFSET);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(outputPoints[pointIndex]);
    }
}

/* Suspend every node in the inner motion list, not just the active slots. */
void mdlMotionPause(dds3ModelHandle_t *ctx) {
    sdfMotionCtrlr_t *motionNode;

    for (motionNode = ctx->inner->motionList; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionPause(motionNode);
    }
}

/* Resume every node in the inner motion list, not just the active slots. */
void mdlMotionPauseOff(dds3ModelHandle_t *ctx) {
    sdfMotionCtrlr_t *motionNode;

    for (motionNode = ctx->inner->motionList; motionNode != NULL; motionNode = motionNode->next) {
        sdfMotionPauseOff(motionNode);
    }
}

/* Return whether the searched node exists; do not expose the lookup result. */
u8 mdlHasMotionEx(dds3ModelHandle_t *ctx, s32 searchId) {
    sdfMotionCtrlr_t *lookupResult;

    lookupResult = mdlGetMotionCtrlr(ctx, searchId);
    return lookupResult != NULL;
}

/* Read the context resource-group halfword; ctx/sub are required. */
u16 mdlGetMajorNum(dds3ModelHandle_t *ctx) {
    return ctx->sub->group;
}

/* Read the context resource-id halfword; ctx/sub are required. */
u16 mdlGetMinorNum(dds3ModelHandle_t *ctx) {
    return ctx->sub->type;
}

/* Number of resource-group tables offered by the viewer's asset selector. */
u32 mdlGetNumOfMajor(void) {
    return 8;
}

/* Return the selected resource table's count, without bounds checks. */
u32 mdlGetNumOfMinor(s32 tableIndex) {
    return D_003C86B0[tableIndex].count;
}

/* Read the referenced halfword or return zero for a missing node. A present
 * node's motion-table reference is dereferenced without a separate NULL check. */
s32 mdlGetNumOfCut(dds3ModelHandle_t *ctx, s32 searchId) {
    sdfMotionCtrlr_t *matchedNode = mdlGetMotionCtrlr(ctx, searchId);
    if (matchedNode == NULL) {
        return 0;
    }
    return matchedNode->motionTable->entryCount;
}

extern void sdfTexListSetClutColor(DevRequest *list, s32 tint, f32 amount);

/* Update the inner resource list with the packed model-color word and scalar. */
void mdlSetClutColor(dds3ModelHandle_t *ctx, s32 tint, f32 amount) {
    sdfTexListSetClutColor(ctx->inner->assetData, tint, amount);
}


/* Copy a resource path's basename without its extension into destination. */
void mdlGetFilebase(s32 selectionListIndex, s32 selectionIndex, char *destination, s32 capacity) {
    MdlResourceSelection *selection;
    MdlResourcePath *pathEntry;
    const char *path;
    const char *basename;
    s32 pathLength;
    s32 startIndex;
    s32 endIndex;
    s32 copyLength;

    selection = (MdlResourceSelection *)D_003C86B0[selectionListIndex].entries;
    selection += selectionIndex;
    pathEntry = (MdlResourcePath *)D_003C6588[selection->pathTable].entries;
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


#define MDL_PART_PACKET_COMMAND 1
#define MDL_PART_INFO_TAG 0x30424950

/* A part-info packet has command 1 and the PIB0 tag in its inline packet header. */
s32 mdlPackCheck(PacWork *packetWork) {
    s32 isSentinel = 0;

    if (((SdfPacModelPacketHeader *)packetWork->packet)->command == MDL_PART_PACKET_COMMAND) {
        isSentinel = ((SdfPacModelPacketHeader *)packetWork->packet)->tag == MDL_PART_INFO_TAG;
    }
    return isSentinel;
}



extern PacWork *mdlPackSetOld(PacWork *work, s32 group, s32 id, s32 mode);
struct EffNodeDescriptor;
extern u16 mdlPibGetEffectCount(MdlRecord *record);
extern DevRequest *mdlEffectResrcListNew(u32 count);
extern void mdlCreateEffectResrc_P2A(DevRequest *list, void *descriptor);
extern void mdlCreateEffectResrc_D3P(DevRequest *list, struct EffNodeDescriptor *descriptor);


/* Generic transports preserve the queue/payload alias relationship; see
 * docs/idioms.md, "Pointer transport aliasing: queued model packets". */
PacWork *mdlPackSetOld(PacWork *work, s32 group, s32 id, s32 mode) {
    MdlLoadPayload setup;
    DevRequest *parts;
    s32 count;

    memset(&setup, 0, sizeof(setup));
    parts = NULL;
    while (work != NULL) {
        switch (((SdfPacModelPacketHeader *)work->packet)->command) {
        case 1:
            switch (((SdfPacModelPacketHeader *)work->packet)->tag) {
            case 0x30424950:
                setup.partInfo = work->dataCursor;
                setup.resourceHandle = PAC_WORK_ALLOCATION(work);
                count = mdlPibGetEffectCount((MdlRecord *)work->dataCursor);
                if (count > 0) {
                    parts = mdlEffectResrcListNew(count);
                    setup.partList = parts;
                }
                break;
            case 0x413250:
                mdlCreateEffectResrc_P2A(parts, work->dataCursor);
                sdfMemFreeDirect(PAC_WORK_ALLOCATION(work));
                break;
            case 0x503344:
                mdlCreateEffectResrc_D3P(parts, (struct EffNodeDescriptor *)work->dataCursor);
                sdfMemFreeDirect(PAC_WORK_ALLOCATION(work));
                break;
            }
            break;
        case 9:
            setup.resourceList = PAC_WORK_TEXTURE_LIST(work);
            break;
        case 6:
            if (setup.requestAllocation == NULL) {
                setup.requestAllocation = PAC_WORK_ALLOCATION(work);
                setup.itemList = (struct sdfModelDef_t *)work->dataCursor;
            }
            break;
        case 8:
            if (setup.motionResource == NULL) {
                setup.motionResource = PAC_WORK_ALLOCATION(work);
                setup.motionTable = (MotionTable *)work->dataCursor;
            }
            break;
        }
        work = sdfPacFreeResrc(work);
    }
    mdlRegistModelSet(group, id, mode, &setup);
    return work;
}


extern u32 mdlPibGetVersion(MdlRecord *record);
extern void mdlCreateEffectResrc_IPU(DevRequest *list, u8 *sourceAddress, struct sdfMemHandle_t *allocation, s32 size);

/* Apply the group's counted parts and entity slots; return the next packet. */
PacWork *mdlPackSet(PacWork *packetWork, s32 group, s32 id, s32 mode) {
    MdlLoadPayload groupSetup;
    MdlPartTagRecord partTagRecord;
    SdfPacModelPacketHeader *packetHeader;
    MdlRecord *record;
    MdlRecord *slotRecord;
    s32 partCount;
    DevRequest *partList;
    s32 slotCount;
    s32 objectSize;
    s32 remainingParts;
    s32 payloadValue;

    if (mdlPackCheck(packetWork) == 0) {
        return mdlPackSetOld(packetWork, group, id, mode);
    }
    record = (MdlRecord *)packetWork->dataCursor;
    payloadValue = mdlPibGetVersion(record);
    if (payloadValue <= 0) {
        return mdlPackSetOld(packetWork, group, id, mode);
    }

    memset(&groupSetup, 0, sizeof(groupSetup));
    groupSetup.partInfo = record;
    groupSetup.resourceHandle = PAC_WORK_ALLOCATION(packetWork);
    packetWork = sdfPacFreeResrc(packetWork);
    partCount = mdlPibGetEffectCount(record);
    remainingParts = partCount;
    if (partCount > 0) {
        partList = mdlEffectResrcListNew(partCount);
        groupSetup.partList = partList;
    }
    if (remainingParts > 0) {
        do {
            switch (((SdfPacModelPacketHeader *)packetWork->packet)->tag) {
            case 0x413250:
                mdlCreateEffectResrc_P2A(partList, packetWork->dataCursor);
                sdfMemFreeDirect(PAC_WORK_ALLOCATION(packetWork));
                break;
            case 0x503344:
                mdlCreateEffectResrc_D3P(partList, (struct EffNodeDescriptor *)packetWork->dataCursor);
                sdfMemFreeDirect(PAC_WORK_ALLOCATION(packetWork));
                break;
            case 0x555049:
                objectSize = ((SdfPacModelPacketHeader *)packetWork->packet)->decodedSize;
                if (objectSize == 0) {
                    objectSize = ((SdfPacModelPacketHeader *)packetWork->packet)->payloadSize
                               + (((SdfPacModelPacketHeader *)packetWork->packet)->flags & 0xF0) - 0x10;
                }
                mdlCreateEffectResrc_IPU(partList, packetWork->dataCursor,
                                    PAC_WORK_ALLOCATION(packetWork), objectSize);
                break;
            default:
                /* Retail retains these metadata stores: DDS1 2188E0/2188E4,
                 * DDS2 2333F8/2333FC. */
                packetHeader = (SdfPacModelPacketHeader *)packetWork->packet;
                partTagRecord.tag = packetHeader->tag;
                partTagRecord.state = 0;
                break;
            }
            packetWork = sdfPacFreeResrc(packetWork);
            remainingParts--;
        } while (remainingParts != 0);
    }

    if (((SdfPacModelPacketHeader *)packetWork->packet)->command == 9) {
        groupSetup.resourceList = PAC_WORK_TEXTURE_LIST(packetWork);
        packetWork = sdfPacFreeResrc(packetWork);
    }
    groupSetup.requestAllocation = PAC_WORK_ALLOCATION(packetWork);
    groupSetup.itemList = (struct sdfModelDef_t *)packetWork->dataCursor;
    packetWork = sdfPacFreeResrc(packetWork);
    mdlRegistModelSet(group, id, mode, &groupSetup);

    slotCount = mdlPibGetMotionCount(record);
    while (slotCount > 0) {
        slotRecord = (MdlRecord *)(packetWork->dataCursor - (u32)&((MdlRecord *)0)->tail);
        mdlRegistMotion(group, id, mode, (s16)slotRecord->parameter.word,
                                      slotRecord->payload.stream.selectorA,
                                      slotRecord->payload.stream.selectorB,
                                      (MotionTable *)packetWork->dataCursor,
                                      PAC_WORK_ALLOCATION(packetWork));
        packetWork = sdfPacFreeResrc(packetWork);
        slotCount--;
    }
    return packetWork;
}


/* Clean up the retained dispatch request, then release its completion job. */
void mdlFileCbFree(MdlDoneJob *ownerBlock) {
    pacFileHandleFree_(ownerBlock->owner);
    sdfChipFreeDirect(ownerBlock);
}


extern u32 mdlManagerWork;
/* Apply the completed load and remove its group id under the semaphore.
 * A non-NULL callback is invoked before owner/job cleanup; without a callback,
 * this function leaves cleanup to the request path. */
void mdlFileLoadCallback(FilePacRequest *requestOwner, MdlDoneJob *completionJob) {
    completionJob->owner = requestOwner;
    mdlPackSet(requestOwner->packet.queueHead, completionJob->group, completionJob->id, completionJob->arg);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(completionJob->group, completionJob->id);
    SignalSema(mdlManagerWork);
    if (completionJob->done != NULL) {
        completionJob->done(completionJob->doneArg);
        mdlFileCbFree(completionJob);
    }
}




extern void mdlFileLoadCallback(FilePacRequest *, MdlDoneJob *);

/* Allocate a completion job and dispatch the request. Group/id narrow to u16.
 * Without onComplete, run the existing no-callback completion path and clean up
 * here; otherwise the completion callback path owns cleanup. Always return zero.
 * Preserve the generic callback and user-data word parameters. */
s32 mdlFileLoad(s32 group, s32 id, s32 jobArg, const char *requestPath, void (*onComplete)(u32), u32 callbackArg) {
    MdlDoneJob *completionJob = sdfAllocAndClearQuadwords(MDL_DONE_JOB_BYTES);
    FilePacRequest *requestSlot;

    completionJob->group = group;
    completionJob->id = id;
    completionJob->arg = jobArg;
    completionJob->doneArg = callbackArg;
    completionJob->done = onComplete;
    requestSlot = fileCreatePacLoadWork(requestPath, 0, 0,
                                        (s32)mdlFileLoadCallback, (s32)completionJob);
    completionJob->owner = requestSlot;
    if (onComplete == NULL) {
        pacSync((struct FileRequest *)requestSlot);
        mdlFileCbFree(completionJob);
    }
    return 0;
}

INCLUDE_SDATA(const s32, "model/mdlManager", D_00436FA0);

INCLUDE_SDATA(const s32, "model/mdlManager", D_00436FA8);
