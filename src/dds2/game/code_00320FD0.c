#include "common.h"

extern u32 mnuResourceRecords;

extern u32 D_004390C4;

extern void (*sdfTickCallback)(void);

extern u8 D_0045C860[];

extern u32 D_0043899C;

extern void mnuFreeOptionalBlock(u32);

extern void *memcpy(void *, const void *, u32);

extern void *memset(void *, s32, u32);

/* Byte cursor state at mnuStepCounterState (8 bytes). */
typedef struct CursorState {
    u8 unk0[2];
    u16 total;
    u8 index;
    u8 limit;
    u8 step;
    u8 unk7;
} CursorState;

extern CursorState mnuStepCounterState;

extern u16 mnuStepCounterThreshold[];

typedef struct ResourceNode {
    u32 id;
    u32 value;
    struct ResourceNode *next;
    u32 unk_C;
    u32 handle;
} ResourceNode;

typedef struct ResourceList {
    u32 count;
    ResourceNode *first;
    ResourceNode *last;
    u32 unk_C;
    void (*onRemove)(u32, u32); /* 0x10: called with each node's id and handle */
} ResourceList;

extern void func_0035A880(ResourceNode *);

u32 dds3RemoveListNodeAndNotify(u32 list, u32 node);

/* Retain the one-argument call to the old-style lookup declaration: it matches retail. */
u32 mnuRemoveResourceNodeById(u32 list) {
    u32 selectedNode = mnuFindResourceNodeById(list);
    if (selectedNode != 0) {
        return dds3RemoveListNodeAndNotify(list, selectedNode);
    }
    return 0;
}

/* Notify and free every node, then reset the list. */
void mnuClearResourceList(ResourceList *list) {
    ResourceNode *node;

    if (list != NULL) {
        node = list->first;
        if (node != NULL) {
            do {
                ResourceNode *current = node;
                node = node->next;
                list->onRemove(current->id, current->handle);
                func_0035A880(current);
            } while (node != NULL);
        }
        list->last = NULL;
        list->first = NULL;
        list->count = 0;
    }
}

typedef struct SdfLink {
    u8 pad00[8];
    struct SdfLink *prev;
    struct SdfLink *next;
} SdfLink;

typedef struct SdfLinkList {
    u32 pad00;
    SdfLink *prev;
    SdfLink *next;
} SdfLinkList;

void sdfLinkListExchangeNodes(SdfLinkList *list, SdfLink *a, SdfLink *b) {
    SdfLink *tmpNext;
    SdfLink *tmpPrev;

    if (a != NULL && b != NULL) {
        if (a->prev != NULL) {
            a->prev->next = b;
        }
        if (a->next != NULL) {
            a->next->prev = b;
        }
        if (b->prev != NULL) {
            b->prev->next = a;
        }
        if (b->next != NULL) {
            b->next->prev = a;
        }
        tmpNext = a->next;
        a->next = b->next;
        tmpPrev = a->prev;
        a->prev = b->prev;
        b->next = tmpNext;
        b->prev = tmpPrev;
        if (a->next == NULL) {
            list->prev = a;
        }
        if (a->prev == NULL) {
            list->next = a;
        }
        if (b->next == NULL) {
            list->prev = b;
        }
        if (b->prev == NULL) {
            list->next = b;
        }
    }
}

ResourceNode *mnuFindResourceNodeByValue(list, value)
    ResourceList *list;
    u32 value;
{
    ResourceNode *node = list->first;
    if (node == NULL) {
        return NULL;
    }
    do {
        if (node->value == value) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

ResourceNode *mnuFindResourceNodeById(list, id)
    ResourceList *list;
    u32 id;
{
    ResourceNode *node = list->first;
    if (node == NULL) {
        return NULL;
    }
    do {
        if (node->id == id) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

ResourceNode *mnuFindResourceNodeByHandle(list, handle)
    ResourceList *list;
    u32 handle;
{
    ResourceNode *node = list->first;
    if (node == NULL) {
        return NULL;
    }
    do {
        if (node->handle == handle) {
            break;
        }
        node = node->next;
    } while (node != NULL);
    return node;
}

void func_003211F0(void) {
}

u8 *mnuGetResourceProgressParameters(void) {
    return D_0045C860;
}

void mnuCopyResourceProgressParameters(u8 *src) {
    memcpy(D_0045C860, src, 16);
}

u8 * mnuGetResourceProgressStepState(void) {
    return (u8 *)&mnuStepCounterState;
}

void mnuSetResourceProgressCadence(u8 limit, u8 step) {
    mnuStepCounterState.limit = limit;
    mnuStepCounterState.step = step;
}

void mnuResetProgressLimitAndStep(u8 limit, u8 step) {
    memset(&mnuStepCounterState, 0, 8);
    mnuStepCounterState.limit = limit;
    mnuStepCounterState.step = step;
}

s32 mnuAdvanceCursorStepUntilThreshold(void) {
    if (++mnuStepCounterState.index >= mnuStepCounterState.limit) {
        mnuStepCounterState.index = 0;
        mnuStepCounterState.total += mnuStepCounterState.step;
        if (mnuStepCounterState.total >= mnuStepCounterThreshold[0]) {
            return 1;
        }
    }
    return 0;
}

void mnuResetResourceProgressCounters(void) {
    mnuStepCounterState.index = 0;
    mnuStepCounterState.total = 0;
}

void mnuBindResourceRecordTable(u32 records, u32 count) {
    mnuResourceRecords = records;
    D_004390C4 = count;
}

/* The externally owned table stores 28-byte records. */
u8 *mnuGetResourceRecordByIndex(s32 recordIndex) {
    return (u8 *)mnuResourceRecords + recordIndex * 28;
}

INCLUDE_ASM(const s32, "game/code_00320FD0", func_00321340);

void func_003214C0(void) {
}

void func_003214C8(u32 value) {
    D_0043899C = value;
}

void func_003214D0(u32 unused, s32 resource) {
    if (resource != 0) {
        mnuFreeOptionalBlock(resource);
        return;
    }
}

INCLUDE_SDATA(const s32, "game/code_00320FD0", D_0043899C);

