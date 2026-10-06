#ifndef MNU_LIST_H
#define MNU_LIST_H

#include "mnu.h"

/* Native DDS1 list owner, shared by the allocator and its window consumers. */
struct MenuListNode {
    s32 index;
    s32 value;
    u8 pad8[0x40];
    u32 flags48;        /* 0x48 */
    u8 pad4C[4];
    s32 animationTimer; /* 0x50: stepped down to zero while a list is visible */
    u8 selectionByte54; /* 0x54: cleared on moving the list selection */
    u8 pad55[3];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    /* The allocator reserves 0x74 bytes. Camp builders fill four payload
     * words at 0x60; other lists use the same storage as sorting keys. */
    union {
        struct {
            u32 sortKeyPrimary;   /* 0x60 */
            u32 sortKeySecondary; /* 0x64 */
            u32 sortKeyTertiary;  /* 0x68 */
            u8 pad6C[8];
        };
        CampWindowParams camp;
    };
};
struct MenuList {
    u32 stateFlags;     /* 0x00: cursor and selection-control bits */
    u32 flags;
    s32 id;             /* 0x08: owner/list identifier */
    s32 visibleCount;
    struct MenuListNode *first;
    struct MenuListNode *last;
    struct MenuListNode *head;
    struct MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowStep;        /* 0x28: constructor argument scaled by eight */
    u8 pad2C[0x10];
    s32 scale;          /* 0x3C: 8.8 fixed-point default */
};

#endif
