#include "common.h"

extern u32 func_0012A6F0(u32);

extern u32 func_0012AC90(u32, u32, u32, u32, u32, u32);

struct WorldListNode;

typedef struct WorldNodeVtbl {
    u8 pad00[8];
    void (*update)(struct WorldListNode *); /* 0x08 */
    void (*draw)(struct WorldListNode *);   /* 0x0C */
} WorldNodeVtbl;

typedef struct WorldListNode {
    u8 pad00[0xF];
    u8 kind;                    /* 0x0F */
    WorldNodeVtbl *vtbl;        /* 0x10 */
    u8 pad14[0xC];
    struct WorldListNode *next; /* 0x20 */
    struct WorldListNode *prev; /* 0x24 */
    u8 pad28[8];
    struct WorldObject *owner;  /* 0x30 */
} WorldListNode;

typedef struct WorldList {
    s32 count;           /* 0x0 */
    WorldListNode *head; /* 0x4 */
    WorldListNode *tail; /* 0x8 */
} WorldList;

typedef struct WorldObjectData {
    s32 value00;
    u32 resource;        /* 0x04: allocation holding the lists */
    WorldList *lists;    /* 0x08 */
    u32 value0C;
    u32 value10;
    u32 handle14;
    u32 value18;
    u32 value1C;
    s32 value20;
} WorldObjectData;

typedef struct WorldObject {
    u8 pad00[4];
    s16 index;   /* 0x04 */
    u16 value06; /* 0x06 */
    u8 pad08[0x10];
    WorldObjectData *data; /* 0x18 */
} WorldObject;

extern WorldObject *D_00435D8C;

INCLUDE_ASM(const s32, "game/code_001104F0", func_001104F0);

u16 func_00110628(s32 object) {
    u16 value;

    value = 0;
    if (object != 0) {
        value = ((WorldObject *)object)->value06;
    }
    return value;
}

u32 dds3WriteIndexedWorldObjectWord(WorldObject *object, u32 value) {
    if (object->value06 == 0) {
        return 0;
    }
    if (object->index < 0) {
        return 0;
    }
    ((u32 *)(D_00435D8C->data->handle14))[object->index * 2] = value;
    return 1;
}

u32 dds3ReadIndexedWorldObjectWord(WorldObject *object) {
    if (object->value06 == 0) {
        return 0;
    }
    if (object->index < 0) {
        return 0;
    }
    return ((u32 *)(D_00435D8C->data->handle14))[object->index * 2];
}

/* Signed comparison via complement-and-shift: zero counts as nonnegative. */
u32 dds3ResetObjectValueCursor(s16 *values) {
    values[2] = *values;
    return (u32)~(s32)*values >> 0x1f;
}

u32 dds3AdvanceObjectValueCursor(s16 *values) {
    if (values[2] < 0) {
        return 0;
    }
    values[2] = *(u16 *)((u8 *)D_00435D8C->data->handle14 + values[2] * 8 + 4);
    return (u32)~(s32)values[2] >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110720);

extern void *func_00328D68(s32 size);
extern u32 func_003292A8(s32 size);
extern void sdfReleaseChipBlock(void *block);
extern WorldList *sdfResourceRetainAddress(u32 resource);

u32 func_001107A0(WorldObject *object) {
    WorldObjectData *data = func_00328D68(0x40);
    WorldList *lists;
    s32 i;

    if (data != NULL) {
        data->value00 = -1;
        data->value20 = 1;
        data->resource = 0;
        data->lists = NULL;
        data->value0C = 0;
        data->value10 = 0;
        data->handle14 = 0;
        data->value18 = 0;
        data->value1C = 0;
        data->resource = func_003292A8(0xD8);
        if (data->resource == 0) {
            sdfReleaseChipBlock(data);
            return 0;
        }
        lists = sdfResourceRetainAddress(data->resource);
        data->lists = lists;
        for (i = 0x11; i >= 0; i--) {
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

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110860);

u32 func_00110938(WorldObject *object) {
    WorldObjectData *data = object->data;
    WorldListNode *node;
    WorldListNode *next;
    s32 i;

    if (data == NULL) {
        return 0;
    }
    for (i = 0; i <= 0x11; i++) {
        if (data->lists[i].count != 0) {
            node = data->lists[i].head;
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

u32 func_001109F0(WorldObject *object) {
    WorldObjectData *data = object->data;
    WorldListNode *node;
    WorldListNode *next;

    if (data == NULL) {
        return 0;
    }
    if (data->value20 == 0) {
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
        object->data->value20 = (s32)value;
    }
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110AA8);

extern void effObjNodeDestroy(void *node);

/* Unlink a world list node from its kind's list in the owner's data and destroy it. */
void func_00110B50(WorldListNode *node) {
    WorldList *list;

    if (node != NULL) {
        if ((u32)(node->kind - 2) < 0x10) {
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
}

void func_00110BE0(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_00110C18();
    data->value0C = value;
}

u32 func_00110C18(WorldObject *object) {
    return object->data->value0C;
}

void func_00110C28(WorldObject *object, u32 value) {
    WorldObjectData *data;

    data = object->data;
    func_00110C60();
    data->value10 = value;
}

u32 func_00110C60(WorldObject *object) {
    return object->data->value10;
}

INCLUDE_ASM(const s32, "game/code_001104F0", func_00110C70);

extern void *dds3AppendWorldIndexNode(s32 index);
extern void func_001104F0();

void *func_00110CD8(WorldObject *object, s32 kind) {
    WorldObjectData *data = object->data;
    void *index;
    WorldListNode *node;

    if (data->lists[kind].count == 0) {
        return NULL;
    }
    index = dds3AppendWorldIndexNode(0);
    node = data->lists[kind].head;
    do {
        func_001104F0(index, 1);
        dds3WriteIndexedWorldObjectWord(index, node);
        node = node->next;
    } while (node != NULL);
    return index;
}

void dds3AttachResourceHandleToWorldObject(WorldObject *object, u32 resourceId) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_0012A6F0(resourceId);
    data->handle14 = handle;
}

void dds3AttachConstructedResourceToWorldObject(WorldObject *object, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    WorldObjectData *data;
    u32 handle;

    data = object->data;
    handle = func_0012AC90(arg1, arg2, arg3, arg4, arg5, arg6);
    data->handle14 = handle;
}
