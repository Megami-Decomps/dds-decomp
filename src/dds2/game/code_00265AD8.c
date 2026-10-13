#include "dsp_entry_api.h"
#include "evt_message_window_api.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "kwln_fade.h"
#include "mnu.h"
#include "sdf_resource.h"
#include "mnu_flag_snapshot.h"
#include "mdl_flag_api.h"





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

extern const char D_00424D08[];
extern u8 D_003CE674[];
extern u8 D_003CE498[];
extern ShopRankPriceRow D_003CBB70[];
extern const u8 (*D_00435E5C)[25];
extern s32 evtIsActiveFlagSet(s32);
extern void mnuShopLoadSpriteAssets(MenuTerminalContext *);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern u32 evtMenuSetProgressFlag(MenuTerminalContext *);
extern s32 func_00265A60(MenuTerminalContext *);
extern void dspSetActive(s32);
extern s32 evtGetMessageWindowControlState(void);
extern u32 func_00260460(void);
extern s32 mnuCampFindActiveSlot(void);
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern void mnuAwardCampProgressCurrency(void);
extern void dspStartActivePresetEntry(void);

/* DDS2's twelve-state shop sequence. The stock scan and dispatcher pattern
 * share the exact DDS1 donor, but fade, reward and preset paths are DDS2's. */
s32 func_00265AD8(KwlnTask *task) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 *dispatchSlot = &state->popupState;
    s32 allowed;
    s32 result;

    result = func_002C4038(&state->transitionWork, dispatchSlot,
                          0, task);
    if (result == 0) {
        if (*dispatchSlot == 0) {
            allowed = 0;
            if (kwlnTaskGetTaskByName(D_00424D08) == NULL) {
                allowed = 1;
            }
            if (evtIsActiveFlagSet(0) != 0) {
                allowed = 1;
            }
            if (allowed) {
                switch (state->phase) {
                case 0:
                    mnuShopLoadSpriteAssets(state);
                    kwlnFadeOutStart(0, 0, 0, 0xF);
                    state->prepared = 1;
                    state->phase = 1;
                    break;

                case 1:
                    if (state->delayFrames > 0) {
                        state->delayFrames--;
                    } else if (evtMenuSetProgressFlag(state) != 0) {
                        state->phase = 8;
                    } else if (func_00265A60(state) != 0) {
                        state->phase = 10;
                    } else if (mdlFlagTest(4) == 0) {
                        mnuSetPopupEntryFlagged(dispatchSlot, D_003CE674);
                    } else if (state->options != 0) {
                        dspSetActive(1);
                        dspStartEntry(state->options + 0xF);
                        state->phase = 2;
                    } else {
                        state->phase = 5;
                    }
                    break;

                case 2:
                    if (evtGetMessageWindowControlState() == 0) {
                        evtFinishMessageWindowAndNotify();
                        state->options = func_00260460();
                        state->phase = 3;
                        state->rewardCursor = 0;
                    }
                    break;

                case 3: {
                    s32 activeSlot = mnuCampFindActiveSlot();
                    u32 index;

                    if (activeSlot == -1) {
                        state->phase = 5;
                    }
                    index = state->rewardCursor;
                    if (index < 32) {
                        u32 stockOffset = index * sizeof(ShopRankPriceEntry) +
                                          activeSlot * (s32)sizeof(ShopRankPriceRow);
                        u32 flagsAddress = (u32)&D_003CBB70[0].entries[0].flags + stockOffset;

                        do {
                            ShopRankPriceEntry *entry = (ShopRankPriceEntry *)(flagsAddress - sizeof(u32));
                            u16 itemId = entry->itemId;

                            if ((*(u32 *)flagsAddress & 1) != 0) {
                                state->rewardCursor = index + 1;
                                dspSetActive(1);
                                evtCopyEntryStringToActiveWindow(0, D_00435E5C[itemId]);
                                dspStartEntry(15);
                                break;
                            }
                            index++;
                            flagsAddress += sizeof(ShopRankPriceEntry);
                        } while (index < 32);
                    }
                    if (index == 32) {
                        state->rewardCursor = -1;
                    }
                    state->phase = 4;
                    break;
                }

                case 4:
                    if (evtGetMessageWindowControlState() == 0) {
                        evtFinishMessageWindowAndNotify();
                        if (state->rewardCursor == -1) {
                            if (state->options == 2) {
                                state->phase = 6;
                            } else {
                                state->phase = 1;
                            }
                        } else {
                            state->phase = 3;
                        }
                    }
                    break;

                case 6:
                    dspSetActive(1);
                    dspStartEntry(0x18);
                    state->phase = 7;
                    break;

                case 8:
                    mnuAwardCampProgressCurrency();
                    state->phase = 9;
                    break;

                case 10:
                    dspStartActivePresetEntry();
                    state->phase = 11;
                    break;

                case 7:
                case 9:
                case 11:
                    if (evtGetMessageWindowControlState() == 0) {
                        evtFinishMessageWindowAndNotify();
                        state->phase = 1;
                    }
                    break;

                case 5:
                    if (state->retryFrames < 20) {
                        state->retryFrames++;
                    } else {
                        state->phase = 0;
                        mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
                    }
                    break;
                }
            }
        }
        result = 0;
    }
    return result;
}

s32 evtAdvancePopupWithOptionalPreparation(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);

    if (*(s32 *)(context + 0xE0) != 0) {
        mnuDrawShopPanelDecorations((MenuTerminalContext *)context);
    }
    if (*(s32 *)(context + 0xB8) != 0) {
        mnuDrawTerminalPanelTransition(context, *(s32 *)(context + 0xB8));
    }
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}
/* Route the supplied callback through dispatch mode 2 after menu setup. */
s32 func_00265EE8(KwlnTask *callback) {
    s32 eventContext = kwlnTaskGetUserValue(callback);
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

s32 mnuPollMessageWindowBeforeClosing(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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

s32 func_00265FE8(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);

    mnuDrawShopPanelDecorations((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_00266038(KwlnTask *callback) {
    s32 eventContext = kwlnTaskGetUserValue(callback);
    func_0026C900();
    return evtMenuSetHandler((void *)eventContext, 2, (void *)callback);
}

u32 mnuStartReturnFadeAndRestoreDisplay(void) {
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 1);
    evtSetBoundedDisplayValue(1, 0);
    kwlnFadeInStart(0, 0, 0, 0xF);
    sndReleaseTrackHandle(0x300000);
    return 1;
}

u32 evtStartFadeOut(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

s32 mnuWaitForFadeBeforePopup(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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

s32 func_00266188(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);

    mnuDrawShopPanelDecorations((MenuTerminalContext *)context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 evtDispatchSync(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);

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
struct SdfMemBlock *mnuCreateFlagEntries(void) {
    struct SdfMemBlock *handle = sdfAllocGeneralBlock(0x140);
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

void mnuApplyCampResourceFlagEntries(struct SdfMemBlock *handle) {
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
extern void mnuInitializeMapPacket(u32, u32 *, s32, MapPacket *);
extern void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);

void func_00266460(struct EffectSlotSet *object, MenuEffectResources *resources) {
    CampMapArguments mapArguments = D_00424DE0;
    CampEffectRows rows = D_00424E10;
    u32 dataAddress;
    struct SdfMemBlock *allocation;
    struct EffMappedResource *mappedResource;

    allocation = sdfReadNamedResource(D_00424E30, &dataAddress, 0);
    mappedResource = effCreateMappedResource((const u8 *)dataAddress);
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

