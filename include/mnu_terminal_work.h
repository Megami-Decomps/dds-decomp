#ifndef MNU_TERMINAL_WORK_H
#define MNU_TERMINAL_WORK_H

#include "common.h"
#include "mnu_transition.h"
#include "evt_world.h"
#include "eff.h"
#include "eff_resource_records.h"
#include "mnu_list.h"

/* The Karma Terminal's menu scene: its resources, list and panel state. */
typedef struct MenuTerminalWork {
    struct SdfMemBlock *allocation; /* 0x00: retained scene allocation */
    u32 initFlags;          /* 0x04: entry initialization bits */
    MenuPopupState transitionWork; /* 0x08 */
    s32 popupState;         /* 0x54 */
    u32 dispatchTable;       /* 0x58 */
    EvtResourcePair messageResources; /* 0x5C: loader handle and message data address */
    EffectSlotSet *resourceSets[2]; /* 0x64: the two loaded terminal sprite sets. */
    EffectSlotSet *alternateBatch; /* 0x6C */
    struct MenuList *commandLists[1]; /* 0x70: owned command-list array */
    struct MenuList *list; /* 0x74 */
    struct MenuList *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
    u8 pad84[4];
    s32 panelMode;         /* 0x88 */
    u8 pad8C[0xC];
    s32 resourcePhase;      /* 0x98 */
    s32 panelFade;          /* 0x9C: 0..0x100 color blend weight */
    EffMappedResource *effect[7];           /* 0xA0: effect batches; [4] and [5] are the pair selected via cursor */
    s32 cursor[2];           /* 0xBC: current and previous node, -1 until selected */
    void (*callbacks[2])(s32, struct MenuTerminalWork *); /* 0xC4 */
    s32 exitPending;         /* 0xCC */
    s32 transitionPending;   /* 0xD0 */
    s32 transitionStage;     /* 0xD4 */
    s32 selectedSlot;        /* 0xD8 */
    s32 reduced;             /* 0xDC */
    u32 imageHandles[7];     /* 0xE0: copied by mnuResolveStaffImageHandles */
    u8 padFC[0x5C];
    u32 effectHandle; /* 0x158: effect-resource address word. */
    s32 effectStage; /* 0x15C */
    u32 bgmHandle;           /* 0x160: encoded bank/track handle */
} MenuTerminalWork; /* 0x164 allocation (mnuTerminalCreateScene) */
typedef char MenuTerminalWork_size[(sizeof(MenuTerminalWork) == 0x164) ? 1 : -1];

void mnuLoadResourceHandles(MenuTerminalWork *work);
void mnuReleaseResourceHandles(MenuTerminalWork *work);

#endif
