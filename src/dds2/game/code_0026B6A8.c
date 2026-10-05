#include "mnu.h"

extern u32 kwlnTaskGetUserValue();

extern void mnuDrawTerminalBackdrop(s32);

extern void func_0026C900(void);

extern s32 evtGetMessageWindowControlState(void);

extern s32 datGameState;

extern char D_003CE848[];

typedef struct SceneFlagEntry {
    s32 needFlag;    /* 0x00 */
    s32 doneFlag;    /* 0x04 */
    u16 areaIndex;   /* 0x08 */
    u8 nameIndex;    /* 0x0A */
    u8 pad0B;
    u16 dialogIndex; /* 0x0C */
    u16 pad0E;
} SceneFlagEntry;

typedef struct PartyFlagPair {
    s32 needFlag;
    s32 doneFlag;
} PartyFlagPair;

typedef struct PartySlotHeader {
    u16 flags;
    u16 pad02;
    u16 unitId;
} PartySlotHeader;

/* Encoded name-table rows; keep byte storage and the retail table strides. */
typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

typedef struct DspMantraName {
    u8 encodedText[19];
} DspMantraName;

extern SceneFlagEntry mnuSceneFlagEventEntries[4];
extern PartyFlagPair mnuPartyFlagEventEntries[];
extern s32 D_00435E5C;
extern DspMantraName *D_00435E50;
extern s32 D_00435E4C;
extern DspUnitName *D_00435E48;

extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);
extern void evtCloseDisplayChannelAndEnsureMessageWindow(s32);
extern void evtCopyEntryStringToActiveWindow(s32, void *);
extern void dspSetActive();

s32 dspStartFlagEvent(s32 context) {
    s32 kind = *(s32 *)(context + 0x84);
    s32 i;
    PartySlotHeader *slot;

    if (kind < 2) {
        if (kind >= 0) {
            if (mdlFlagTest(0x1B1) != 0 && mdlFlagTest(0x916) == 0) {
                dspSetActive(1);
                dspStartEntry(5);
                mdlFlagSet(0x916);
                return 1;
            }
            for (i = 0; i < sizeof(mnuSceneFlagEventEntries) / sizeof(mnuSceneFlagEventEntries[0]); i++) {
                if (mdlFlagTest(mnuSceneFlagEventEntries[i].needFlag) != 0 && mdlFlagTest(mnuSceneFlagEventEntries[i].doneFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, (void *)(D_00435E5C + mnuSceneFlagEventEntries[i].areaIndex * 0x19));
                    evtCopyEntryStringToActiveWindow(1, D_00435E50[mnuSceneFlagEventEntries[i].nameIndex].encodedText);
                    evtCopyEntryStringToActiveWindow(2, (void *)(D_00435E4C + mnuSceneFlagEventEntries[i].dialogIndex * 0x11));
                    dspStartEntry(3);
                    mdlFlagSet(mnuSceneFlagEventEntries[i].doneFlag);
                    return 1;
                }
            }
            for (i = 0; i < 5; i++) {
                slot = (PartySlotHeader *)(datGameState + i * 0x1C4 + 0xA60);
                if ((slot->flags & 1) != 0 && mdlFlagTest(mnuPartyFlagEventEntries[slot->unitId].needFlag) != 0
                    && mdlFlagTest(mnuPartyFlagEventEntries[slot->unitId].doneFlag) == 0) {
                    evtCloseDisplayChannelAndEnsureMessageWindow(context);
                    dspSetActive(1);
                    evtCopyEntryStringToActiveWindow(0, D_00435E48[slot->unitId].encodedText);
                    dspStartEntry(4);
                    mdlFlagSet(mnuPartyFlagEventEntries[slot->unitId].doneFlag);
                    return 1;
                }
            }
        }
    }
    return 0;
}


s32 mnuPrepareTerminalPanelState(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    mnuTerminalSelectResourceBank(state);
    mnuApplyFadeTrackMode(0, state);
    return 1;
}

u32 func_0026B930(void) {
    return 1;
}

s32 dspUpdateFlagEvent(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s32 result = func_002C4038(state + 8, panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (dspStartFlagEvent(state) == 0) {
                mnuSetPopupEntryFlagged(panel, D_003CE848);
            }
        }
    }
    return 0;
}

s32 mnuDispatchTerminalPanel(s32 request) {
    s32 state = kwlnTaskGetUserValue();

    mnuDrawTerminalBackdrop(state);
    return menuSetHandler(state, 1, request);
}

s32 mnuDispatchTerminalPanelExit(s32 request) {
    s32 state = kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler(state, 2, request);
}

u32 mnuStartTaskFadeIn(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    /* Both arms are identical in retail; kept as written. */
    if (*(s32 *)(state + 0xe4) == 0) {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    else {
        kwlnFadeInStart(0, 0, 0, 0xf);
    }
    return 1;
}

s32 mnuStartTerminalPanelFadeOut(void) {
    s32 state = kwlnTaskGetUserValue();

    mnuApplyFadeTrackMode(1, state);
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

extern s32 kwlnFadeIsActive(void);

extern char D_003CE998[];

/* When the current panel is idle and no fade is running, start its queued step. */
s32 func_0026BAF8(u64 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panelState = (s32 *)(state + 0x54);
    s32 result = func_002C4038(state + 8, panelState, 0, request);
    if (result == 0) {
        if (*panelState == 0 && kwlnFadeIsActive() == 0) {
            mnuSetPopupEntryFlagged(panelState, D_003CE998);
        }
        result = 0;
    }
    return result;
}

s32 func_0026BB78(s32 request) {
    s32 state = kwlnTaskGetUserValue();

    mnuDrawTerminalBackdrop(state);
    return menuSetHandler(state, 1, request);
}

s32 func_0026BBC8(s32 request) {
    s32 state = kwlnTaskGetUserValue();

    return menuSetHandler(state, 2, request);
}

u32 mnuClearTerminalPanelSelection(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    *(u32 *)(state + 0x98) = 0;
    return 1;
}

s32 mnuRebuildTerminalMenuResources(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    mnuReleaseWorkResources(state);
    mnuTerminalBuildMenus(state);
    return 1;
}

u32 func_0026BC68(void) {
    return 0;
}

u32 func_0026BC70(void) {
    return 0;
}

u32 func_0026BC78(void) {
    return 0;
}

/* Index (0-3) of the lowest main character present in the party, as a bit mask over unit ids 1, 2, 5 and 8; 0 when none. */
s32 mnuFirstPresentMainCharacterIndex(void) {
    PartySlotHeader *slot;
    u32 present = 0;
    s32 i;

    for (i = 0; i < 5; i++) {
        slot = (PartySlotHeader *)(datGameState + i * 0x1C4 + 0xA60);
        if (slot->flags & 1) {
            switch (slot->unitId) {
            case 1:
                present |= 1;
                break;
            case 2:
                present |= 2;
                break;
            case 5:
                present |= 4;
                break;
            case 8:
                present |= 8;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (present & 1) {
            return i;
        }
        present >>= 1;
    }
    return 0;
}

typedef struct DspScrollingStrip {
    void *resource;
    s32 frameIndex;
    s32 horizontalOffset;
    s32 verticalOffset;
    s32 scrollSpeed;
} DspScrollingStrip;

typedef struct DspScrollingStripState {
    s32 unk0;
    s32 layout;
    s32 unk8;
    void *resource;
    s32 layer;
    s32 unk14;
    s32 unk18;
    DspScrollingStrip strips[6];
} DspScrollingStripState;

s32 func_0026BD38(s32 index) {
    return index == 1 ? 4 : 6;
}

void mnuInitScrollingStripState(DspScrollingStripState *state, s32 layout, void *resource, s32 firstFrame, s32 layer) {
    s32 count;
    s32 i;

    memset(state, 0, sizeof(*state));
    state->layout = layout;
    count = func_0026BD38(layout);
    state->resource = resource;
    state->layer = layer;
    state->unk8 = 0;
    for (i = 0; i < count; i++) {
        state->strips[i].resource = resource;
        state->strips[i].frameIndex = firstFrame + i;
        state->strips[i].horizontalOffset = 0;
        state->strips[i].verticalOffset = 0;
        state->strips[i].scrollSpeed = 0;
    }
}

extern u32 effMiscRand(void *state);

/* Give each scrolling strip a random speed in the requested range. */
void func_0026BE28(DspScrollingStripState *state, s32 negate, s32 minimum, s32 maximum) {
    /* scrollSpeed is the final word in each five-word strip record. */
    s32 *offsets = &state->strips[0].scrollSpeed;
    s32 range = maximum - minimum;
    s32 i = 5;

    for (; i >= 0; i--) {
        s32 offset = effMiscRand(NULL) % range + minimum;
        if (negate == 0) {
            *offsets = offset;
        } else {
            *offsets = -offset;
        }
        offsets += 5;
    }
}

void func_0026BEB0(DspScrollingStripState *state, s32 vertical, s32 horizontal, s32 unused) {
    state->unk14 = vertical;
    state->unk18 = horizontal;
}

INCLUDE_ASM(const s32, "game/code_0026B6A8", func_0026BEC0);

u32 func_0026C168(void) {
    return 0xffffffff;
}

s32 evtCopyWorldObjectEntryValue(s32 id, s32 dst) {
    s32 src = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 9, id);
    if (src != 0) {
        *(s32 *)(*(s32 *)(dst + 0x18) + 0x80) = *(s32 *)(*(s32 *)(src + 0x18) + 0x78);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026B6A8", D_00425018);

