#include "mnu.h"


extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

typedef struct EventCallbackContext {
    u8 pad00[0xC];
    MenuPopupState transitionWork; /* +0x0C: native saved-entry transition state */
    s32 dispatch; /* +0x58: state passed to the callback dispatcher */
} EventCallbackContext;

typedef struct FlagEntry {
    s32 firstFlag;
    s32 firstOn;
    s32 secondFlag;
    s32 secondOn;
} FlagEntry;

extern s32 sdfResourceRetainAddress(s32);

extern s32 mdlFlagTest(s32);

extern void mdlFlagClear(s32);

extern void mdlFlagSet(s32);

extern s32 sdfAllocGeneralBlock(s32);

typedef struct FlagPair {
    s32 first;
    s32 second;
} FlagPair;

typedef struct FlagSource {
    s32 first;
    s32 second;
    s32 pad[2];
} FlagSource;

extern FlagSource mnuSceneFlagEventEntries[];

extern FlagPair mnuPartyFlagEventEntries[];

extern s32 kwlnFadeIsActive(void);

extern u8 D_003CE6AC[];


typedef struct EventMenuSelection {
    u8 pad00[0x60];
    s32 entryIndex; /* 0x60 */
} EventMenuSelection;

typedef struct EventMenuOwner {
    u8 pad00[0x1C];
    EventMenuSelection *selection; /* 0x1C */
    s32 state; /* 0x20 */
} EventMenuOwner;

typedef struct EventVisualState {
    u8 pad00[0x3C];
    u32 statusFlag; /* 0x3C */
} EventVisualState;

typedef struct EventDispatchState {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus; /* 0x54 */
    u32 dispatchValue; /* 0x58 */
    u8 pad5C[0x1C];
    EventVisualState *visualState; /* 0x78 */
    u8 pad7C[4];
    EventMenuOwner *menuOwner; /* 0x80 */
    s32 menuMode; /* 0x84 */
    u8 pad88[8];
    s32 displayMode; /* 0x90 */
    u8 pad94[0x0C];
    s32 fadeStarted; /* 0xA0 */
    u8 padA4[0x28];
    u32 callback; /* 0xCC */
    u32 previousCallback; /* 0xD0 */
    s32 exitState; /* 0xD4 */
    u8 padD8[0x74];
    s32 stage; /* 0x14C */
} EventDispatchState;

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265AD8);

s32 evtAdvancePopupWithOptionalPreparation(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    if (*(s32 *)(context + 0xE0) != 0) {
        func_0025FD78((MenuTerminalContext *)context);
    }
    if (*(s32 *)(context + 0xB8) != 0) {
        func_00294758(context, *(s32 *)(context + 0xB8));
    }
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}
/* Route the supplied callback through dispatch mode 2 after menu setup. */
s32 func_00265EE8(s32 callback) {
    s32 eventContext = kwlnTaskGetUserValue();
    func_0026C900();
    return evtMenuSetHandler((void *)eventContext, 2, (void *)callback);
}

u32 func_00265F30(void) {
    dspSetActive(1);
    dspStartEntry(0xd);
    return 1;
}

u32 func_00265F58(void) {
    return 1;
}

extern u8 D_003CE690[];

s32 mnuPollMessageWindowBeforeClosing(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = (s32 *)(context + 0x58);
    s32 state = func_002C4038(&((EventCallbackContext *)context)->transitionWork, window, 0, (void *)callback);

    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                evtFinishMessageWindowAndNotify();
                mnuSetPopupEntryFlagged(window, D_003CE690);
            }
        }
        return 0;
    }
    return state;
}

s32 func_00265FE8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0025FD78((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_00266038(s32 callback) {
    s32 eventContext = kwlnTaskGetUserValue();
    func_0026C900();
    return evtMenuSetHandler((void *)eventContext, 2, (void *)callback);
}

u32 mnuStartReturnFadeAndRestoreDisplay(void) {
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 1);
    evtSetBoundedDisplayValue(1, 0);
    kwlnFadeInStart(0, 0, 0, 0xF);
    func_00342580(0x300000);
    return 1;
}

u32 evtStartFadeOut(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

s32 mnuWaitForFadeBeforePopup(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = (s32 *)(context + 0x58);
    s32 state = func_002C4038(&((EventCallbackContext *)context)->transitionWork, window, 0, (void *)callback);
    if (state == 0) {
        if (*window == 0) {
            if (kwlnFadeIsActive() == 0) {
                mnuSetPopupEntryFlagged(window, D_003CE6AC);
            }
        }
        return 0;
    }
    return state;
}

s32 func_00266188(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0025FD78((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 evtDispatchSync(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    return evtMenuSetHandler((void *)context, 2, (void *)callback);
}

u32 func_00266210(void) {
    return 1;
}

u32 func_00266218(void) {
    return 1;
}

u32 func_00266220(void) {
    return 0;
}

u32 func_00266228(void) {
    return 0;
}

u32 func_00266230(void) {
    return 0;
}

/* Snapshot four primary flag pairs and sixteen extra pairs for restoration. */
s32 mnuCreateFlagEntries(void) {
    s32 handle = sdfAllocGeneralBlock(0x140);
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        entries[i].firstFlag = mnuSceneFlagEventEntries[i].first;
        entries[i].firstOn = mdlFlagTest(entries[i].firstFlag);
        entries[i].secondFlag = mnuSceneFlagEventEntries[i].second;
        entries[i].secondOn = mdlFlagTest(entries[i].secondFlag);
    }
    for (i = 0; i < 16; i++) {
        entries[4 + i].firstFlag = mnuPartyFlagEventEntries[i].first;
        entries[4 + i].firstOn = mdlFlagTest(entries[4 + i].firstFlag);
        entries[4 + i].secondFlag = mnuPartyFlagEventEntries[i].second;
        entries[4 + i].secondOn = mdlFlagTest(entries[4 + i].secondFlag);
    }
    return handle;
}

void mnuApplyCampResourceFlagEntries(s32 handle) {
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        if (entries[i].firstOn != 0) {
            mdlFlagSet(entries[i].firstFlag);
        }
        if (entries[i].secondOn != 0) {
            mdlFlagSet(entries[i].secondFlag);
        }
    }
    for (i = 0; i < 16; i++) {
        if (entries[4 + i].firstOn != 0) {
            mdlFlagSet(entries[4 + i].firstFlag);
        }
        if (entries[4 + i].secondOn != 0) {
            mdlFlagSet(entries[4 + i].secondFlag);
        }
    }
}

void mnuClearCampResourceFlagEntries(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        mdlFlagClear(mnuSceneFlagEventEntries[i].first);
        mdlFlagClear(mnuSceneFlagEventEntries[i].second);
    }
    for (i = 0; i < 16; i++) {
        mdlFlagClear(mnuPartyFlagEventEntries[i].first);
        mdlFlagClear(mnuPartyFlagEventEntries[i].second);
    }
}


extern const CampMapArguments D_00424DE0;
extern const CampEffectRows D_00424E10;
extern const char D_00424E30[];
extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);
extern u32 effCreateMappedResource(u32);
extern void sdfReleaseResourceAllocation();
extern void mnuInitializeMapPacket(u32, u32 *, s32, MapPacket *);
extern void mnuSetCampEffectResourceHandles(u32, u32, MenuEffectResources *);
extern void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);

void func_00266460(u32 object, MenuEffectResources *resources) {
    CampMapArguments mapArguments = D_00424DE0;
    CampEffectRows rows = D_00424E10;
    u32 dataAddress;
    u64 allocation;
    u32 mappedResource;

    allocation = sdfReadNamedResource(D_00424E30, &dataAddress, 0);
    mappedResource = effCreateMappedResource(dataAddress);
    sdfReleaseResourceAllocation(allocation);
    mnuInitializeMapPacket(1, mapArguments.values, 11, &resources->packet);
    mnuSetCampEffectResourceHandles(object, mappedResource, resources);
    mnuCopyCampEffectRowData(&rows, resources);
}

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D50);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D60);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D70);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D80);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D90);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DA0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DB0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DC0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DD0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DE0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E10);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E30);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E48);

