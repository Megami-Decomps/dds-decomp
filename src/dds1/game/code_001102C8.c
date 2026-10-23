#include "common.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "sdf_texture_offset_list.h"

struct WorldIndexNode;

extern u32 fldLoadAreaRoomResourcesFromRequest(u32, u32, u32, u32,
                        const SdfTextureOffsetListHeader *, u32);

extern struct sdfMemHandle_t *fldLoadAreaRoomResources(const char *);




extern dds3Basic_t *dds3TheWorld;


u32 dds3AddBasicArray(dds3BasicArrayHandle_t *object, s32 count) {
    WorldInfo *world;
    s16 firstIndex;
    s16 lastIndex;
    s32 i;

    if (count == 0) {
        return 1;
    }
    if (dds3TheWorld == NULL) {
        return 0;
    }
    world = dds3TheWorld->data;
    if (world->freeEntryCount < count || count < 0) {
        return 0;
    }
    if (object == NULL) {
        return 0;
    }
    firstIndex = world->freeHeadIndex;
    for (i = 0; i < count - 1; i++) {
        world->freeHeadIndex = world->entries[world->freeHeadIndex].nextIndex;
    }
    lastIndex = world->freeHeadIndex;
    world->freeHeadIndex = world->entries[lastIndex].nextIndex;
    if (world->freeHeadIndex == -1) {
        world->freeTailIndex = -1;
    } else {
        world->entries[lastIndex].nextIndex = -1;
        world->entries[world->freeHeadIndex].previousIndex = -1;
    }
    world->freeEntryCount -= count;
    if (object->tailIndex < 0) {
        object->tailIndex = lastIndex;
        object->headIndex = firstIndex;
    } else {
        world->entries[object->tailIndex].nextIndex = firstIndex;
        world->entries[firstIndex].previousIndex = object->tailIndex;
        object->tailIndex = lastIndex;
    }
    object->cursorIndex = firstIndex;
    object->entryCount += count;
    return 1;
}

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = object->entryCount;
    }
    return value;
}

u32 dds3SetBasicArray(dds3BasicArrayHandle_t *object, u32 wordValue) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    ((WorldInfo *)dds3TheWorld->data)->entries[object->cursorIndex].value = wordValue;
    return 1;
}

u32 dds3GetBasicArray(const dds3BasicArrayHandle_t *object) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    return ((WorldInfo *)dds3TheWorld->data)->entries[object->cursorIndex].value;
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object) {
    object->cursorIndex = object->headIndex;
    return (u32)~(s32)object->headIndex >> 0x1f;
}

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *object) {
    if (object->cursorIndex < 0) {
        return 0;
    }
    object->cursorIndex = ((WorldInfo *)dds3TheWorld->data)->entries[object->cursorIndex].nextIndex;
    return (u32)~(s32)object->cursorIndex >> 0x1f;
}

/* Call `callback` with every value of the object's list in order; stop and return 0 as soon as one callback returns 0. Returns 1 when the object has no values, no callback is given, or all callbacks succeeded. */
s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *object, s32 (*callback)(void *)) {
    u32 more;

    if (callback == NULL) {
        return 1;
    }
    if (dds3GetBasicArrayCount(object) == 0) {
        return 1;
    }
    for (more = dds3SetFirstBasicArray(object); more != 0; more = dds3SetNextBasicArray(object)) {
        if (callback((void *)dds3GetBasicArray(object)) == 0) {
            return 0;
        }
    }
    return 1;
}


/* Allocate the object's data and 18 empty per-kind lists; return success. */
u32 dds3CreateWorldObjectData(dds3Basic_t *object) {
    EvtWorldTable *data = sdfChipAllocDirect(sizeof(EvtWorldTable));
    EvtWorldSlot *lists;
    s32 listIndex;

    if (data != NULL) {
        data->areaRoomKey = -1;
        data->drawEnabled = 1;
        data->resource = 0;
        data->slots = NULL;
        data->cameraObject = 0;
        data->playerObject = 0;
        data->indexedHandle = 0;
        data->sceneResourceHandle = 0;
        data->sceneScript = NULL;
        data->resource = sdfMemAllocDirect(0xD8);
        if (data->resource == 0) {
            sdfChipFreeDirect(data);
            return 0;
        }
        lists = (EvtWorldSlot *)sdfMemLock(data->resource);
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

extern void dds3SceneBasicFreeScene(dds3Basic_t *object);

/* Destroy every node of every list, then release the data block and scene state. */
void dds3DestroyWorldObjectData(dds3Basic_t *object) {
    EvtWorldTable *data = object->data;
    s32 listIndex;

    if (data != NULL) {
        for (listIndex = 0; listIndex <= 0x11; listIndex++) {
            while (data->slots[listIndex].head != NULL) {
                dds3DeleteSceneBasicData(data->slots[listIndex].head);
            }
        }
        if (data->resource != 0) {
            sdfMemFreeDirect(data->resource);
        }
        dds3SceneBasicFreeScene(object);
        dds3SceneBasicReleaseScriptFile(object);
        sdfChipFreeDirect(data);
    }
}

/* Cache the next link before calling an update that may remove the current node. */
u32 scenemainprocuder(dds3Basic_t *object) {
    EvtWorldTable *data = object->data;
    dds3Basic_t *node;
    dds3Basic_t *next;
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

extern void drawSceneBasicList(void);

/* Only kind 2 is drawn here; the remaining kinds participate in update traversal. */
u32 scenedrawprocuder(dds3Basic_t *object) {
    EvtWorldTable *data = object->data;
    dds3Basic_t *node;
    dds3Basic_t *next;

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
    drawSceneBasicList();
    do {
        next = node->next;
        node->ops->draw(node);
        node = next;
    } while (next != NULL);
    return 1;
}

void dds3VisibleSceneBasic(dds3Basic_t *object, s8 enabled) {
    if (((EvtWorldTable *)object->data) != NULL) {
        ((EvtWorldTable *)object->data)->drawEnabled = (s32)enabled;
    }
}
#include "dds3_world_secondary.h"
extern dds3Basic_t *dds3AllocBasic(u32 kind);

/* Create a member in the current secondary world's list for this kind. */
dds3Basic_t *dds3AddSceneBasicData(s32 kind) {
    dds3Basic_t *world;
    dds3Basic_t *node;
    EvtWorldSlot *list;

    if ((u32)(kind - 2) >= 16) {
        return NULL;
    }
    world = dds3GetControlScene();
    if (world == NULL) {
        return NULL;
    }
    list = &((EvtWorldTable *)world->data)->slots[kind];
    node = dds3AllocBasic(kind);
    if (node == NULL) {
        return NULL;
    }
    node->owner = world;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
    }
    list->count++;
    return node;
}

extern void dds3DeleteBasic(dds3Basic_t *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void dds3DeleteSceneBasicData(dds3Basic_t *node) {
    EvtWorldSlot *list;

    if (node != NULL && (u32)((node->kindTag >> 24) - 2) < 0x10) {
        list = &((EvtWorldTable *)node->owner->data)->slots[(node->kindTag >> 24)];
        if (list->head == node) {
            list->head = node->next;
        }
        if (list->tail == node) {
            list->tail = node->previous;
        }
        dds3DeleteBasic(node);
        list->count--;
    }
}


dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *object, dds3Basic_t *cameraObject) {
    EvtWorldTable *data;
    dds3Basic_t *previous;

    data = ((EvtWorldTable *)object->data);
    previous = dds3GetActiveCameraSceneBasic(object);
    data->cameraObject = cameraObject;
    return previous;
}

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *object) {
    return ((EvtWorldTable *)object->data)->cameraObject;
}


dds3Basic_t *dds3SetActiveUnitSceneBasic(dds3Basic_t *object, dds3Basic_t *playerObject) {
    EvtWorldTable *data;
    dds3Basic_t *previous;

    data = ((EvtWorldTable *)object->data);
    previous = dds3GetActiveUnitSceneBasic(object);
    data->playerObject = playerObject;
    return previous;
}

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *object) {
    return ((EvtWorldTable *)object->data)->playerObject;
}

/* Find the node with `key` in list `kind` of the object, or NULL. */
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *object, u32 key, s32 kind) {
    dds3Basic_t *node;

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


struct WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind) {
    EvtWorldTable *data = object->data;
    struct WorldIndexNode *indexObject;
    dds3Basic_t *node;

    if (data->slots[kind].count == 0) {
        return NULL;
    }
    indexObject = dds3GetBasicArrayHandle(0);
    node = data->slots[kind].head;
    do {
        dds3AddBasicArray((dds3BasicArrayHandle_t *)indexObject, 1);
        dds3SetBasicArray((dds3BasicArrayHandle_t *)indexObject, (u32)node);
        node = node->next;
    } while (node != NULL);
    return indexObject;
}

void dds3SceneBasicMakeScene(dds3Basic_t *object, const char *resourcePath) {
    EvtWorldTable *data;
    struct sdfMemHandle_t *handle;

    data = ((EvtWorldTable *)object->data);
    handle = fldLoadAreaRoomResources(resourcePath);
    data->indexedHandle = (u32)handle;
}

void dds3SceneBasicMakeScene_FromMemory(dds3Basic_t *object, u32 area, u32 room,
                                    u8 *resource0Data, u8 *resource1Data,
                                    const SdfTextureOffsetListHeader *textureOffsets, u32 flags) {
    EvtWorldTable *data;
    u32 handle;

    data = ((EvtWorldTable *)object->data);
    handle = fldLoadAreaRoomResourcesFromRequest(area, room, (u32)resource0Data, (u32)resource1Data,
                                                 textureOffsets, flags);
    data->indexedHandle = handle;
}
