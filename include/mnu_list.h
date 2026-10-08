#ifndef MNU_LIST_H
#define MNU_LIST_H

#include "mnu.h"

/* DDS1 248D40 writes the two terminal payload words; 24BDB8 consumes them
 * as a party index and its signed recovery cost. The surrounding node storage
 * is also the camp/sort payload, not a second progress-node struct view. */
typedef struct MenuThresholdEntry {
    s32 entryId;
    s32 requiredAmount;
} MenuThresholdEntry;

struct DatPartyRecord;

/* Native list owner, shared by both games' allocator and window consumers. */
struct MenuListNode {
    s32 index;
    const void *value; /* 0x04: DDS1 245A40 and DDS2 2B0FA0 pass caption/table-entry pointers. */
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
        MenuThresholdEntry terminal;
    };
    /* +0x70: terminal recovery panel at 267050; title at 2674C8.
     * Mantra lists retain a DatPartyRecord address in the same word. */
    union {
        u32 unk70;
        s32 childPanel;
        char *title;
        struct DatPartyRecord *partyRecord;
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

struct MenuWindowContainer;
struct MenuListNode *mnuListAppendNode(struct MenuList *list, const void *value);
struct MenuListNode *mnuAppendWindowListNode(struct MenuWindowContainer *window, const void *value);

/* Insert the same opaque entry value while preserving the list cursor/window. */
#ifdef VERSION_DDS1
struct MenuListNode *func_0027B540(struct MenuList *list, struct MenuListNode *anchor,
                                const void *value, s32 mode, u32 options);
struct MenuListNode *func_0027C688(struct MenuWindowContainer *window, struct MenuListNode *anchor,
                                const void *value, s32 mode, u32 options);
#else
struct MenuListNode *func_002B83A0(struct MenuList *list, struct MenuListNode *anchor,
                                const void *value, s32 mode, u32 options);
struct MenuListNode *func_002B9708(struct MenuWindowContainer *window, struct MenuListNode *anchor,
                                const void *value, s32 mode, u32 options);
#endif

#ifdef VERSION_DDS1
struct MenuListNode *mnuAdvanceListCursorDefault(struct MenuList *list);
struct MenuListNode *mnuRetreatListCursorDefault(struct MenuList *list);
#endif

#endif
