#include "common.h"
#include "dds3obj.h"
#include "evt_world.h"

struct NodeB;

extern u32 func_00128780(u32, u32, u32, u32, u32, u32);

extern u32 func_001281E0(u32);




extern EffWorldNode *dds3ActiveWorld;


INCLUDE_ASM(const s32, "game/code_001102C8", dds3GrowWorldValueChain);

u16 dds3GetWorldValueCount(WorldValueIndices *object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = object->entryCount;
    }
    return value;
}

u32 dds3WriteIndexedWorldObjectWord(WorldValueIndices *object, u32 value) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    ((WorldInfo *)dds3ActiveWorld->data)->unk14[object->cursorIndex].unk0 = value;
    return 1;
}

u32 dds3ReadIndexedWorldObjectWord(WorldValueIndices *object) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    return ((WorldInfo *)dds3ActiveWorld->data)->unk14[object->cursorIndex].unk0;
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 dds3ResetObjectValueCursor(WorldValueIndices *object) {
    object->cursorIndex = object->headIndex;
    return (u32)~(s32)object->headIndex >> 0x1f;
}

u32 dds3AdvanceObjectValueCursor(WorldValueIndices *object) {
    if (object->cursorIndex < 0) {
        return 0;
    }
    object->cursorIndex = ((WorldInfo *)dds3ActiveWorld->data)->unk14[object->cursorIndex].unk4;
    return (u32)~(s32)object->cursorIndex >> 0x1f;
}

/* Call `callback` with every value of the object's list in order; stop and return 0 as soon as one callback returns 0. Returns 1 when the object has no values, no callback is given, or all callbacks succeeded. */
s32 dds3VisitWorldObjectValues(WorldValueIndices *object, s32 (*callback)(u32)) {
    u32 more;

    if (callback == NULL) {
        return 1;
    }
    if (dds3GetWorldValueCount(object) == 0) {
        return 1;
    }
    for (more = dds3ResetObjectValueCursor(object); more != 0; more = dds3AdvanceObjectValueCursor(object)) {
        if (callback(dds3ReadIndexedWorldObjectWord(object)) == 0) {
            return 0;
        }
    }
    return 1;
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern struct SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern void sdfReleaseChipBlock(void *block);
extern u32 sdfResourceRetainAddress(struct SdfMemBlock *resource);

/* Allocate the object's data and 18 empty per-kind lists; return success. */
u32 dds3CreateWorldObjectData(EffWorldNode *object) {
    EvtWorldTable *data = sdfAllocSizeClassBlock(0x40);
    EvtWorldSlot *lists;
    s32 listIndex;

    if (data != NULL) {
        data->unk00 = -1;
        data->drawEnabled = 1;
        data->resource = 0;
        data->slots = NULL;
        data->cameraObject = 0;
        data->playerObject = 0;
        data->indexedHandle = 0;
        data->unk18 = 0;
        data->unk1C = 0;
        data->resource = sdfAllocGeneralBlock(0xD8);
        if (data->resource == 0) {
            sdfReleaseChipBlock(data);
            return 0;
        }
        lists = (EvtWorldSlot *)sdfResourceRetainAddress(data->resource);
        data->slots = lists;
        for (listIndex = 0x11; listIndex >= 0; listIndex--) {
            lists->count = 0;
            lists->head = NULL;
            lists->tail = NULL;
            lists++;
        }
        object->data = data;
        return 1;
    }
    return 0;
}

extern void dds3ClearSceneObjectState(EffWorldNode *object);
extern void evtReleaseSceneResource(EffWorldNode *object);
extern void sdfReleaseResourceAllocation(struct SdfMemBlock *resource);
void dds3RemoveWorldObjectNode(EffWorldNode *node);

/* Destroy every node of every list, then release the data block and scene state. */
void dds3DestroyWorldObjectData(EffWorldNode *object) {
    EvtWorldTable *data = object->data;
    s32 listIndex;

    if (data != NULL) {
        for (listIndex = 0; listIndex <= 0x11; listIndex++) {
            while (data->slots[listIndex].head != NULL) {
                dds3RemoveWorldObjectNode(data->slots[listIndex].head);
            }
        }
        if (data->resource != 0) {
            sdfReleaseResourceAllocation(data->resource);
        }
        dds3ClearSceneObjectState(object);
        evtReleaseSceneResource(object);
        sdfReleaseChipBlock(data);
    }
}

/* Cache the next link before calling an update that may remove the current node. */
u32 dds3UpdateWorldObjectLists(EffWorldNode *object) {
    EvtWorldTable *data = object->data;
    EffWorldNode *node;
    EffWorldNode *next;
    s32 listIndex;

    if (data == NULL) {
        return 0;
    }
    for (listIndex = 0; listIndex <= 0x11; listIndex++) {
        if (data->slots[listIndex].count != 0) {
            node = data->slots[listIndex].head;
            if (node->ops != NULL && node->ops->update != NULL) {
                do {
                    next = node->next;
                    node->ops->update(node);
                    node = next;
                } while (next != NULL);
            }
        }
    }
    return 1;
}

extern void fldSubmitVisibleWorldBackground(void);

/* Only kind 2 is drawn here; the remaining kinds participate in update traversal. */
u32 dds3DrawWorldObjectList(EffWorldNode *object) {
    EvtWorldTable *data = object->data;
    EffWorldNode *node;
    EffWorldNode *next;

    if (data == NULL) {
        return 0;
    }
    if (data->drawEnabled == 0) {
        return 1;
    }
    if (data->slots[2].count == 0) {
        return 1;
    }
    node = data->slots[2].head;
    if (node->ops == NULL) {
        return 1;
    }
    if (node->ops->draw == NULL) {
        return 1;
    }
    fldSubmitVisibleWorldBackground();
    do {
        next = node->next;
        node->ops->draw(node);
        node = next;
    } while (next != NULL);
    return 1;
}

void dds3SetWorldObjectDataValue(EffWorldNode *object, s8 value) {
    if (((EvtWorldTable *)object->data) != NULL) {
        ((EvtWorldTable *)object->data)->drawEnabled = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001102C8", dds3AppendWorldObjectNode);

extern void effObjNodeDestroy(EffWorldNode *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void dds3RemoveWorldObjectNode(EffWorldNode *node) {
    EvtWorldSlot *list;

    if (node != NULL && (u32)((node->kindTag >> 24) - 2) < 0x10) {
        list = &((EvtWorldTable *)node->owner->data)->slots[(node->kindTag >> 24)];
        if (list->head == node) {
            list->head = node->next;
        }
        if (list->tail == node) {
            list->tail = node->previous;
        }
        effObjNodeDestroy(node);
        list->count--;
    }
}

EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *object);

EffWorldNode *dds3SetWorldCameraObject(EffWorldNode *object, EffWorldNode *value) {
    EvtWorldTable *data;
    EffWorldNode *previous;

    data = ((EvtWorldTable *)object->data);
    previous = dds3GetWorldCameraObject(object);
    data->cameraObject = value;
    return previous;
}

EffWorldNode *dds3GetWorldCameraObject(EffWorldNode *object) {
    return ((EvtWorldTable *)object->data)->cameraObject;
}

EffWorldNode *dds3GetWorldPlayerObject(EffWorldNode *object);

void dds3SetWorldPlayerObject(EffWorldNode *object, EffWorldNode *value) {
    EvtWorldTable *data;

    data = ((EvtWorldTable *)object->data);
    dds3GetWorldPlayerObject(object);
    data->playerObject = value;
}

EffWorldNode *dds3GetWorldPlayerObject(EffWorldNode *object) {
    return ((EvtWorldTable *)object->data)->playerObject;
}

/* Find the node with `key` in list `kind` of the object, or NULL. */
EffWorldNode *dds3FindWorldObjectNodeByKey(EffWorldNode *object, u32 key, s32 kind) {
    EffWorldNode *node;

    if (((EvtWorldTable *)object->data)->slots[kind].count == 0) {
        return NULL;
    }
    node = ((EvtWorldTable *)object->data)->slots[kind].head;
    do {
        if (key == node->key) {
            return node;
        }
        node = node->next;
    } while (node != NULL);
    return NULL;
}

extern struct NodeB *dds3AppendWorldIndexNode(s32 initialCount);
extern void dds3GrowWorldValueChain(WorldValueIndices *object, s32 count);

struct NodeB *dds3CopyWorldListToValueChain(EffWorldNode *object, s32 kind) {
    EvtWorldTable *data = object->data;
    struct NodeB *indexObject;
    EffWorldNode *node;

    if (data->slots[kind].count == 0) {
        return NULL;
    }
    indexObject = dds3AppendWorldIndexNode(0);
    node = data->slots[kind].head;
    do {
        dds3GrowWorldValueChain((WorldValueIndices *)indexObject, 1);
        dds3WriteIndexedWorldObjectWord((WorldValueIndices *)indexObject, (u32)node);
        node = node->next;
    } while (node != NULL);
    return indexObject;
}

void dds3AttachResourceHandleToWorldObject(EffWorldNode *object, u32 resourceId) {
    EvtWorldTable *data;
    u32 handle;

    data = ((EvtWorldTable *)object->data);
    handle = func_001281E0(resourceId);
    data->indexedHandle = handle;
}

void dds3AttachConstructedResourceToWorldObject(EffWorldNode *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    EvtWorldTable *data;
    u32 handle;

    data = ((EvtWorldTable *)object->data);
    handle = func_00128780(arg1, arg2, arg3, arg4, arg5, arg6);
    data->indexedHandle = handle;
}
