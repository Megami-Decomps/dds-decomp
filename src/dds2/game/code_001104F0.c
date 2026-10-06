#include "common.h"
#include "evt_world.h"

extern u32 func_0012A6F0(u32);

extern u32 func_0012AC90(u32, u32, u32, u32, u32, u32);

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
    struct EvtWorldObject *owner;  /* 0x30 */
} WorldListNode;


/* Global world-info entries: payload at +0, next index at +4. */
typedef struct {
    u32 value;
    u16 next;
    u8 pad06[2];
} WorldIndexedEntry;


/* The global world's +0x18 pointer is world info, not EvtWorldTable. */
typedef struct {
    u8 pad00[0x14];
    WorldIndexedEntry *entries; /* 0x14 */
} WorldIndexState;

typedef struct {
    u8 pad00[0x18];
    WorldIndexState *state; /* 0x18 */
} WorldHandle;

extern WorldHandle *dds3ActiveWorld;

INCLUDE_ASM(const s32, "game/code_001104F0", dds3GrowWorldValueChain);

u16 dds3GetWorldValueCount(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((EvtWorldObject *)object)->entryCount;
    }
    return value;
}

u32 dds3WriteIndexedWorldObjectWord(EvtWorldObject *object, u32 value) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    dds3ActiveWorld->state->entries[object->cursorIndex].value = value;
    return 1;
}

u32 dds3ReadIndexedWorldObjectWord(EvtWorldObject *object) {
    if (object->entryCount == 0) {
        return 0;
    }
    if (object->cursorIndex < 0) {
        return 0;
    }
    return dds3ActiveWorld->state->entries[object->cursorIndex].value;
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 dds3ResetObjectValueCursor(EvtWorldObject *object) {
    object->cursorIndex = object->headIndex;
    return (u32)~(s32)object->headIndex >> 0x1f;
}

u32 dds3AdvanceObjectValueCursor(EvtWorldObject *object) {
    if (object->cursorIndex < 0) {
        return 0;
    }
    object->cursorIndex = dds3ActiveWorld->state->entries[object->cursorIndex].next;
    return (u32)~(s32)object->cursorIndex >> 0x1f;
}

/* Call `callback` with every value of the object's list in order; stop and return 0 as soon as one callback returns 0. Returns 1 when the object has no values, no callback is given, or all callbacks succeeded. */
s32 dds3VisitWorldObjectValues(EvtWorldObject *object, s32 (*callback)(u32)) {
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

extern void *sdfAllocSizeClassBlock(s32 size);
extern struct SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern void sdfReleaseChipBlock(void *block);
extern u32 sdfResourceRetainAddress(struct SdfMemBlock *resource);

/* Allocate the object's data and 18 empty per-kind lists; return success. */
u32 dds3CreateWorldObjectData(EvtWorldObject *object) {
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
        object->table = data;
        return 1;
    }
    return 0;
}

extern void dds3ClearSceneObjectState();
extern void evtReleaseSceneResource();
extern void sdfReleaseResourceAllocation(struct SdfMemBlock *resource);
void dds3RemoveWorldObjectNode(WorldListNode *node);

/* Destroy every node of every list, then release the data block and scene state. */
void dds3DestroyWorldObjectData(EvtWorldObject *object) {
    EvtWorldTable *data = object->table;
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
u32 dds3UpdateWorldObjectLists(EvtWorldObject *object) {
    EvtWorldTable *data = object->table;
    WorldListNode *node;
    WorldListNode *next;
    s32 listIndex;

    if (data == NULL) {
        return 0;
    }
    for (listIndex = 0; listIndex <= 0x11; listIndex++) {
        if (data->slots[listIndex].count != 0) {
            node = data->slots[listIndex].head;
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
u32 dds3DrawWorldObjectList(EvtWorldObject *object) {
    EvtWorldTable *data = object->table;
    WorldListNode *node;
    WorldListNode *next;

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

void dds3SetWorldObjectDataValue(EvtWorldObject *object, s8 value) {
    if (object->table != NULL) {
        object->table->drawEnabled = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001104F0", dds3AppendWorldObjectNode);

extern void effObjNodeDestroy(void *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void dds3RemoveWorldObjectNode(WorldListNode *node) {
    EvtWorldSlot *list;

    if (node != NULL && (u32)(node->kind - 2) < 0x10) {
        list = &node->owner->table->slots[node->kind];
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

void dds3SetWorldCameraObject(EvtWorldObject *object, u32 value) {
    EvtWorldTable *data;

    data = object->table;
    dds3GetWorldCameraObject();
    data->cameraObject = value;
}

u32 dds3GetWorldCameraObject(EvtWorldObject *object) {
    return object->table->cameraObject;
}

void dds3SetWorldPlayerObject(EvtWorldObject *object, u32 value) {
    EvtWorldTable *data;

    data = object->table;
    dds3GetWorldPlayerObject();
    data->playerObject = value;
}

u32 dds3GetWorldPlayerObject(EvtWorldObject *object) {
    return object->table->playerObject;
}

/* Find the node with `key` in list `kind` of the object, or NULL. */
WorldListNode *dds3FindWorldObjectNodeByKey(EvtWorldObject *object, u32 key, s32 kind) {
    WorldListNode *node;

    if (object->table->slots[kind].count == 0) {
        return NULL;
    }
    node = object->table->slots[kind].head;
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

void *dds3CopyWorldListToValueChain(EvtWorldObject *object, s32 kind) {
    EvtWorldTable *data = object->table;
    void *indexObject;
    WorldListNode *node;

    if (data->slots[kind].count == 0) {
        return NULL;
    }
    indexObject = dds3AppendWorldIndexNode(0);
    node = data->slots[kind].head;
    do {
        dds3GrowWorldValueChain(indexObject, 1);
        dds3WriteIndexedWorldObjectWord(indexObject, node);
        node = node->next;
    } while (node != NULL);
    return indexObject;
}

void dds3AttachResourceHandleToWorldObject(EvtWorldObject *object, u32 resourceId) {
    EvtWorldTable *data;
    u32 handle;

    data = object->table;
    handle = func_0012A6F0(resourceId);
    data->indexedHandle = handle;
}

void dds3AttachConstructedResourceToWorldObject(EvtWorldObject *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    EvtWorldTable *data;
    u32 handle;

    data = object->table;
    handle = func_0012AC90(arg1, arg2, arg3, arg4, arg5, arg6);
    data->indexedHandle = handle;
}
