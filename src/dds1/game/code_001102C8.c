#include "common.h"

extern u32 func_00128780(u32, u32, u32, u32, u32, u32);

extern u32 func_001281E0(u32);

struct WorldListNode;

typedef struct WorldNodeVtbl {
    u8 pad00[8];
    void (*update)(struct WorldListNode *); /* 0x08 */
    void (*draw)(struct WorldListNode *);   /* 0x0C */
} WorldNodeVtbl;

typedef struct WorldListNode {
    u8 pad00[4];
    u32 key; /* 0x04: compared when searching a list */
    u8 pad08[7];
    u8 kind;                    /* 0x0F */
    WorldNodeVtbl *vtbl;        /* 0x10 */
    u8 pad14[0xC];
    struct WorldListNode *next; /* 0x20 */
    struct WorldListNode *prev; /* 0x24 */
    u8 pad28[8];
    struct WorldObject *owner;  /* 0x30 */
} WorldListNode;

typedef struct {
    s32 count;           /* 0x0 */
    WorldListNode *head; /* 0x4 */
    WorldListNode *tail; /* 0x8 */
} WorldList;

/* Global world-info entries: payload at +0, next index at +4. */
typedef struct {
    u32 value;
    u16 next;
    u8 pad06[2];
} WorldIndexedEntry;

typedef struct {
    s32 value00; /* 0x00 */
    u32 resource; /* 0x04: allocation holding the lists */
    WorldList *lists;    /* 0x08 */
    u32 cameraObject; /* 0x0C: selected camera/pose source */
    u32 playerObject; /* 0x10: player unit attached by field creation */
    u32 indexedHandle; /* 0x14: object's attached resource handle */
    u32 value18; /* 0x18 */
    u32 value1C; /* 0x1C */
    s32 drawEnabled; /* 0x20: enables rendering of list kind 2 */
} WorldObjectData;

typedef struct WorldObject {
    s16 headIndex;   /* 0x00: first entry in the indexed value chain */
    s16 tailIndex;   /* 0x02 */
    s16 cursorIndex; /* 0x04: current entry for reads and writes */
    u16 entryCount;  /* 0x06 */
    u8 pad08[0x10];
    WorldObjectData *data; /* 0x18 */
} WorldObject;

/* The global world's +0x18 pointer is world info, not WorldObjectData. */
typedef struct {
    u8 pad00[0x14];
    WorldIndexedEntry *entries; /* 0x14 */
} WorldIndexState;

typedef struct {
    u8 pad00[0x18];
    WorldIndexState *state; /* 0x18 */
} WorldHandle;

extern WorldHandle *dds3ActiveWorld;


INCLUDE_ASM(const s32, "game/code_001102C8", dds3GrowWorldValueChain);

u16 dds3GetWorldValueCount(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((WorldObject *)object)->entryCount;
    }
    return value;
}

u32 dds3WriteIndexedWorldObjectWord(WorldObject *object, u32 value) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    dds3ActiveWorld->state->entries[object->cursorIndex].value = value;
    return 1;
}

u32 dds3ReadIndexedWorldObjectWord(WorldObject *object) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    return dds3ActiveWorld->state->entries[object->cursorIndex].value;
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 dds3ResetObjectValueCursor(WorldObject *object) {
    object->cursorIndex = object->headIndex;
    return (u32)~(s32)object->headIndex >> 0x1f;
}

u32 dds3AdvanceObjectValueCursor(WorldObject *object) {
    if (object->cursorIndex < 0) {
        return 0;
    }
    object->cursorIndex = dds3ActiveWorld->state->entries[object->cursorIndex].next;
    return (u32)~(s32)object->cursorIndex >> 0x1f;
}

/* Call `callback` with every value of the object's list in order; stop and return 0 as soon as one callback returns 0. Returns 1 when the object has no values, no callback is given, or all callbacks succeeded. */
s32 dds3VisitWorldObjectValues(WorldObject *object, s32 (*callback)(u32)) {
    u32 more;

    if (callback == NULL) {
        return 1;
    }
    if (dds3GetWorldValueCount((s32)object) == 0) {
        return 1;
    }
    for (more = dds3ResetObjectValueCursor(object); more != 0; more = dds3AdvanceObjectValueCursor(object)) {
        if (callback(dds3ReadIndexedWorldObjectWord(object)) == 0) {
            return 0;
        }
    }
    return 1;
}

extern void *func_002CFEB8(s32 size);
extern u32 sdfAllocGeneralBlock(s32 size);
extern void sdfReleaseChipBlock(void *block);
extern WorldList *sdfResourceRetainAddress(u32 resource);

/* Allocate the object's data and 18 empty per-kind lists; return success. */
u32 dds3CreateWorldObjectData(WorldObject *object) {
    WorldObjectData *data = func_002CFEB8(0x40);
    WorldList *lists;
    s32 listIndex;

    if (data != NULL) {
        data->value00 = -1;
        data->drawEnabled = 1;
        data->resource = 0;
        data->lists = NULL;
        data->cameraObject = 0;
        data->playerObject = 0;
        data->indexedHandle = 0;
        data->value18 = 0;
        data->value1C = 0;
        data->resource = sdfAllocGeneralBlock(0xD8);
        if (data->resource == 0) {
            sdfReleaseChipBlock(data);
            return 0;
        }
        lists = sdfResourceRetainAddress(data->resource);
        data->lists = lists;
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

extern void dds3ClearSceneObjectState();
extern void evtReleaseSceneResource();
extern void sdfReleaseResourceAllocation(u32 resource);
void dds3RemoveWorldObjectNode(WorldListNode *node);

/* Destroy every node of every list, then release the data block and scene state. */
void dds3DestroyWorldObjectData(WorldObject *object) {
    WorldObjectData *data = object->data;
    s32 listIndex;

    if (data != NULL) {
        for (listIndex = 0; listIndex <= 0x11; listIndex++) {
            while (data->lists[listIndex].head != NULL) {
                dds3RemoveWorldObjectNode(data->lists[listIndex].head);
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
u32 dds3UpdateWorldObjectLists(WorldObject *object) {
    WorldObjectData *data = object->data;
    WorldListNode *node;
    WorldListNode *next;
    s32 listIndex;

    if (data == NULL) {
        return 0;
    }
    for (listIndex = 0; listIndex <= 0x11; listIndex++) {
        if (data->lists[listIndex].count != 0) {
            node = data->lists[listIndex].head;
            if (node->vtbl != NULL && node->vtbl->update != NULL) {
                do {
                    next = node->next;
                    node->vtbl->update(node);
                    node = next;
                } while (next != NULL);
            }
        }
    }
    return 1;
}

extern void fldSubmitVisibleWorldBackground(void);

/* Only kind 2 is drawn here; the remaining kinds participate in update traversal. */
u32 dds3DrawWorldObjectList(WorldObject *object) {
    WorldObjectData *data = object->data;
    WorldListNode *node;
    WorldListNode *next;

    if (data == NULL) {
        return 0;
    }
    if (data->drawEnabled == 0) {
        return 1;
    }
    if (data->lists[2].count == 0) {
        return 1;
    }
    node = data->lists[2].head;
    if (node->vtbl == NULL) {
        return 1;
    }
    if (node->vtbl->draw == NULL) {
        return 1;
    }
    fldSubmitVisibleWorldBackground();
    do {
        next = node->next;
        node->vtbl->draw(node);
        node = next;
    } while (next != NULL);
    return 1;
}

void dds3SetWorldObjectDataValue(WorldObject *object, s8 value) {
    if (object->data != NULL) {
        object->data->drawEnabled = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001102C8", dds3AppendWorldObjectNode);

extern void effObjNodeDestroy(void *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void dds3RemoveWorldObjectNode(WorldListNode *node) {
    WorldList *list;

    if (node != NULL && (u32)(node->kind - 2) < 0x10) {
        list = &node->owner->data->lists[node->kind];
        if (list->head == node) {
            list->head = node->next;
        }
        if (list->tail == node) {
            list->tail = node->prev;
        }
        effObjNodeDestroy(node);
        list->count--;
    }
}

void dds3SetWorldCameraObject(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    dds3GetWorldCameraObject();
    data->cameraObject = value;
}

u32 dds3GetWorldCameraObject(WorldObject *object) {
    return object->data->cameraObject;
}

void dds3SetWorldPlayerObject(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    dds3GetWorldPlayerObject();
    data->playerObject = value;
}

u32 dds3GetWorldPlayerObject(WorldObject *object) {
    return object->data->playerObject;
}

/* Find the node with `key` in list `kind` of the object, or NULL. */
WorldListNode *dds3FindWorldObjectNodeByKey(WorldObject *object, u32 key, s32 kind) {
    WorldListNode *node;

    if (object->data->lists[kind].count == 0) {
        return NULL;
    }
    node = object->data->lists[kind].head;
    do {
        if (key == node->key) {
            return node;
        }
        node = node->next;
    } while (node != NULL);
    return NULL;
}

extern void *dds3AppendWorldIndexNode(s32 index);
extern void dds3GrowWorldValueChain();

void *dds3CopyWorldListToValueChain(WorldObject *object, s32 kind) {
    WorldObjectData *data = object->data;
    void *indexObject;
    WorldListNode *node;

    if (data->lists[kind].count == 0) {
        return NULL;
    }
    indexObject = dds3AppendWorldIndexNode(0);
    node = data->lists[kind].head;
    do {
        dds3GrowWorldValueChain(indexObject, 1);
        dds3WriteIndexedWorldObjectWord(indexObject, node);
        node = node->next;
    } while (node != NULL);
    return indexObject;
}

void dds3AttachResourceHandleToWorldObject(WorldObject *object, u32 resourceId) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_001281E0(resourceId);
    data->indexedHandle = handle;
}

void dds3AttachConstructedResourceToWorldObject(WorldObject *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_00128780(arg1, arg2, arg3, arg4, arg5, arg6);
    data->indexedHandle = handle;
}
