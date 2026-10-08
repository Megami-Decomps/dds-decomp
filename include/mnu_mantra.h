#ifndef MNU_MANTRA_H
#define MNU_MANTRA_H

#include "mnu.h"
#include "dat_state.h"

struct ItfMesSub;
struct MenuList;
struct MenuListNode;
typedef struct MantraPanelPool MantraPanelPool;

typedef struct MtrSelectionState {
    s16 state;
    s16 outcome;
    s32 timer;
    s32 alpha;
    f32 scale;
    f32 highlightAlpha;
} MtrSelectionState;

typedef struct MtrSelectionFlags {
    u32 unk00 : 1;
    u32 visible : 1;
    u32 fadeProgress : 1;
    u32 profileReady : 1;
    u32 unk04 : 1;
    u32 unk05 : 27;
} MtrSelectionFlags;

typedef struct MtrResourceLoadState {
    u32 fileEntry;
    u32 unk04;
    s16 state;
    u16 entryIndex;
    u32 partyMask;
} MtrResourceLoadState;

typedef struct MtrEquipState {
    s32 state;
    s32 timer;
} MtrEquipState;

typedef struct MtrUnitMenuEntry {
    u16 unk00 : 13;
    u16 marked : 1;
    u16 unk14 : 2;
    u8 pad02[6];
} MtrUnitMenuEntry;

typedef struct MantraMenuSrc {
    union {
        u16 bits;
        struct {
            u16 unitIndex : 8;
            u16 kind : 4;
            u16 flag12 : 1;
            u16 queued : 1;
            u16 unk14 : 2;
        } fields;
    };
    u16 unk02;
    u32 unk04;
} MantraMenuSrc;

typedef struct MantraFlagResource {
    u32 allocation;
    u32 capacity;
    u16 *flags;
    u16 data[176];
} MantraFlagResource;

typedef union MenuPanelSelector {
    u32 packed;
    struct {
        u16 flags;
        s16 index;
    } fields;
} MenuPanelSelector;

/* The existing unit-entry flags and source-record tags are separate complete
 * 8-byte records. Native unit flags are accessed as halfwords. */

/* Both node-position tables use this complete 0x20-byte record. */
typedef struct MantraNodePos {
    union {
        MenuPanelSelector selector;
        struct {
            u32 kind : 4;
            s32 modelFlagState : 4;
            u32 reserved : 8;
            s16 id;
        };
    };
    s16 x;
    s16 y;
    struct MantraNodePos *neighbors[6];
} MantraNodePos;

typedef struct MenuMantraRecord {
    union {
        u16 flags;
        struct {
            u8 mantraId;
            u8 flagsHigh;
        };
    };
    u8 pad02[2];
    s8 element;
    u8 pad05[3];
} MenuMantraRecord;

/* The single menu work area starts at owner+0x240. Allocation and call
 * chains share it between selection, navigation and panel animation. */
typedef struct MantraMenuWork {
    s16 frame;
    s8 waitFrames;
    u8 nextMode;
    u8 pad004[2];
    s8 tutorialWaitFrames;
    u8 tutorialNextState;
    DatPartyRecord tutorialParty[3];
    union {
        u32 drawFlags;
        struct {
            u32 iconFade : 1;
            u32 hasSource : 1;
            u32 mode : 8;
            u32 unk10 : 4;
            u32 drawEnabled : 1;
            u32 unk15 : 1;
            u32 hasQueuedMastery : 1;
            u32 showOverlay : 1;
            u32 sourceKind : 8;
            u32 unk26 : 6;
        } drawBits;
    };
    s16 scrollX;
    s16 scrollY;
    u8 pad55C[4];
    MantraNodePos *defaultSelector;
    MantraNodePos *alternateSelector;
    u8 pad568[4];
    MantraFlagResource *slots[18];
    u32 collectedValues[8];
    s32 savedSelection;
    s32 collectedCount;
    MantraNodePos *savedSelector;
    s8 selectionIndex;
    s8 panelTransitionMode;
    u16 panelTransitionTimer;
    MenuMantraRecord records[112];
    MantraMenuSrc *src;
    u16 sourceMode;
    u16 sourceFlag;
    u8 pad96C[4];
    MtrUnitMenuEntry unitEntries[5];
    MantraMenuSrc *currentSlot;
    s32 masteryFrames;
    s32 delayFrames;
    s32 navigationState;
    u32 navigationMask;
    MantraPanelPool *resource;
    union {
        u32 flags;
        u8 flagBytes[4];
    };
    u8 unk9B4;
    u8 unk9B5;
    u16 unk9B6;
    MtrEquipState equip;
    u32 selectionController;
} MantraMenuWork;

/* func_00286E98 allocates and clears exactly 0xC08 bytes. The task keeps
 * this whole pointer while menu consumers address its work subobject. */
typedef struct MnuStatusResource {
    u32 allocationHandle;
    struct MenuList *list;
    u8 pad08[0x30];
    u32 resourceIdA;
    u32 resourceIdB;
    struct ItfMesSub *messageWindow;
    struct ItfMesSub *messageDefinition;
    MenuProgressHost *progressHost;
    u8 pad4C[8];
    DatPartyRecord snapshot;
    MtrSelectionFlags flags;
    MtrResourceLoadState resourceLoad;
    MtrSelectionState selection;
    MantraMenuWork menu;
    u8 padC04[4];
} MnuStatusResource;

typedef char MnuStatusResource_size[(sizeof(MnuStatusResource) == 0xC08) ? 1 : -1];
typedef char MantraMenuWork_size[(sizeof(MantraMenuWork) == 0x9C4) ? 1 : -1];
typedef char MantraNodePos_size[(sizeof(MantraNodePos) == 0x20) ? 1 : -1];
typedef char MantraFlagResource_size[(sizeof(MantraFlagResource) == 0x16C) ? 1 : -1];

typedef char MnuStatusResourceMenuOffsetCheck[((u32)&((MnuStatusResource *)0)->menu == 0x240) ? 1 : -1];
typedef char MnuStatusResourcePanelPoolOffsetCheck[((u32)&((MnuStatusResource *)0)->menu.resource == 0xBEC) ? 1 : -1];
typedef char MnuStatusResourceSnapshotOffsetCheck[((u32)&((MnuStatusResource *)0)->snapshot == 0x54) ? 1 : -1];
typedef char MnuStatusResourceUnitEntriesOffsetCheck[((u32)&((MnuStatusResource *)0)->menu.unitEntries == 0xBB0) ? 1 : -1];

typedef char MantraTutorialPartyOffsetCheck[((u32)&((MantraMenuWork *)0)->tutorialParty == 8) ? 1 : -1];

#endif
