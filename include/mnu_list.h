#ifndef MNU_LIST_H
#define MNU_LIST_H

#include "mnu.h"

/* Native list owner, shared by both games' allocator and window consumers. */
struct MenuListNode {
    s32 index;
    s32 value;
    struct {
        u32 sprite;
        u32 effect;
    } sprites[8];       /* 0x08: four normal and four selected-row sprites */
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
            u8 pad6C[4];
        };
        CampWindowParams camp;
    };
    /* +0x70: terminal recovery panel at 267050; title at 2674C8.
     * Mantra lists retain a DatPartyRecord address in the same word. */
    union {
        u32 unk70;
        s32 childPanel;
        char *title;
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
    void (*drawCallback)(); /* 0x2C: native callers supply list-specific arguments. */
    /* 0x30 is generic list user data: DDS1 25F408 interprets a countdown
     * pointer, 2443F8 stores a buffer, and 272D50 stores staff draw context. */
    void *context;
    s32 categoryIndex; /* 0x34: selects one of the skill-window category layouts. */
    u32 categoryMarkerEnabled; /* 0x38: draw the active category marker. */
    s32 scale;          /* 0x3C: 8.8 fixed-point default */
};

#endif
