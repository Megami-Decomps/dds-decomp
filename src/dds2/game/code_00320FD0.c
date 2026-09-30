#include "common.h"

extern u32 D_004390C0;

extern u32 D_004390C4;

extern void (*D_004389C4)(void);

extern u8 D_0045C860[];

extern u32 D_0043899C;

extern void func_00321908(u32);

extern void *memcpy(void *, const void *, u32);

extern void *memset(void *, s32, u32);

/* Byte cursor state at D_004390B8 (8 bytes). */
typedef struct CursorState {
    u8 unk0[2];
    u16 total;
    u8 index;
    u8 limit;
    u8 step;
    u8 unk7;
} CursorState;

extern CursorState D_004390B8;

extern u16 D_0045C866[];

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
} ResourceList;

u32 func_00320F68(u32 list, u32 node);

/* Retain the one-argument call to the old-style lookup declaration: it matches retail. */
u32 func_00320FD0(u32 list) {
    u32 selectedNode = mnuFindResourceNodeById(list);
    if (selectedNode != 0) {
        return func_00320F68(list, selectedNode);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00320FD0", func_00321018);

INCLUDE_ASM(const s32, "game/code_00320FD0", func_00321090);

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

u8 *func_003211F8(void) {
    return D_0045C860;
}

void func_00321208(u8 *src) {
    memcpy(D_0045C860, src, 16);
}

u8 * func_00321238(void) {
    return (u8 *)&D_004390B8;
}

void func_00321248(u8 arg0, u8 arg1) {
    D_004390B8.limit = arg0;
    D_004390B8.step = arg1;
}

void func_00321258(u8 arg0, u8 arg1) {
    memset(&D_004390B8, 0, 8);
    D_004390B8.limit = arg0;
    D_004390B8.step = arg1;
}

s32 func_003212A8(void) {
    if (++D_004390B8.index >= D_004390B8.limit) {
        D_004390B8.index = 0;
        D_004390B8.total += D_004390B8.step;
        if (D_004390B8.total >= D_0045C866[0]) {
            return 1;
        }
    }
    return 0;
}

void func_00321308(void) {
    D_004390B8.index = 0;
    D_004390B8.total = 0;
}

void func_00321318(u32 arg0, u32 arg1) {
    D_004390C0 = arg0;
    D_004390C4 = arg1;
}

/* The externally owned table stores 28-byte records. */
u8 *mnuGetResourceRecordByIndex(s32 recordIndex) {
    return (u8 *)D_004390C0 + recordIndex * 28;
}

INCLUDE_ASM(const s32, "game/code_00320FD0", func_00321340);

void func_003214C0(void) {
}

void func_003214C8(u32 value) {
    D_0043899C = value;
}

void func_003214D0(u32 unused, s32 resource) {
    if (resource != 0) {
        func_00321908(resource);
        return;
    }
}

INCLUDE_SDATA(const s32, "game/code_00320FD0", D_0043899C);

