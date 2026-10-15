#include "dsp_name.h"
#include "evt_message_window_api.h"
#include "mnu_mantra.h"
#include "mnu_mantra_position_api.h"
#include "mnu_panel_position_record.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "mnu_list.h"

extern s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *);
extern u32 evtFinishMessageWindowAndNotify(void);
extern s32 dspStartEntry(s32);
extern s32 dspCloseChannel(void);
extern void mnuDisableMantraBackground(struct MantraDrawPool *pool);
extern void mnuEnableMantraBackground(struct MantraDrawPool *pool);
extern void mnuToggleMantraTitleVariant(struct MantraDrawPool *pool);
extern void mnuBeginMantraUnitPanelExit(struct MantraDrawPool *pool);
extern struct MantraDrawItem *mnuRegisterMantraUnitPanelDraw(struct MantraDrawPool *pool, u32);
extern DatPartyRecord *mnuGetSelectedNodeValue(MnuStatusResource *);
extern s32 mnuMoveNodeCursorToTargetIndex(MnuStatusResource *, s8);
extern void mnuSpawnMantraPanelsFromSelectedUnitFlags(MnuStatusResource *, s32, s32);
extern void mnuUpdateMantraSelectionPanelSlots(MnuStatusResource *, s16, s32, s8, s8, s32);
extern void mnuInitializeMantraTutorialParty(MnuStatusResource *);
extern void mnuUpdateMantraTutorialPanelSprites(MnuStatusResource *);


typedef struct MenuPanelTransition {
    u16 frame;
    u16 panelId;
    u16 mode;
    u16 action;
} MenuPanelTransition;

extern s32 mnuDispatchMantraPanelCommand(void *, s32, s32);


typedef struct MantraPanelPool MantraPanelPool;
typedef struct MantraBurstPool MantraBurstPool;
typedef struct MantraSparkleEmitter MantraSparkleEmitter;

/* Shared 48-byte animation record used by the panel pool in code_0026DBF8. */
typedef struct MantraPanelAnimation {
    union {
        u32 flags;
        struct {
            u8 kind;
            u8 control[3];
        } tag;
    };
    s16 startDelay;
    s16 endDelay;
    s32 animationTicks;
    u16 x;
    u16 y;
    u32 visualParameters[3];
    u8 byte1C;
    u8 byte1D;
    s16 frame;
    u8 stateA;
    u8 stateB;
    u8 stateC;
    u8 pad23;
    MantraSparkleEmitter *spriteHandle;
    MantraBurstPool *burstPool;
    s16 id;
    s16 transitionDelay;
} MantraPanelAnimation;

extern u32 mnuQueuePanelAnimationTransition(MantraPanelAnimation *, u32, s16);
extern void mnuOffsetPanelAndSetVisualParams(MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern void mnuSpawnAlternateMantraSpecialNodeIcon(struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconWithShortLoop(struct MantraDrawPool *pool);
extern void mnuStorePanelEntry(s32, s32);

extern MantraPanelAnimation *mnuSpawnPanelSlotB(MantraPanelPool *, s32, s8, s16, s16, u32);

extern MantraPanelAnimation *mnuFindPanelSlotById(MantraPanelPool *, s32, s8);


extern u32 mnuGetSelectedMantraUnitIndex(MnuStatusResource *);





/* The selector also carries word-wide flags in its packed representation. */



typedef struct MenuPanelEntry {
    s32 soundHandle;
    s32 framesRemaining;
} MenuPanelEntry;

typedef struct MenuPanelEntryPool {
    struct SdfMemBlock *allocation;
    MenuPanelEntry *entries;
    s32 count;
} MenuPanelEntryPool;

extern MenuPanelEntryPool *mnuPanelSoundEntryPool;

/* The eight-byte acquisition record uses its low byte as the mantra id. */





extern void mnuPrepareMantraPanelEntriesForTutorial(void);



extern void evtPrintDeveloperConsoleMessage(const char *, ...);

/* Choose the pending mastery record, preferring queued kind-two entries. */
MenuMantraRecord *func_0028FD30(MnuStatusResource *resource, s16 kind, s16 element) {
    MenuMantraRecord *record = resource->menu.records;
    /* Capture record zero separately from the private element-map snapshot. */
    u16 *rankUpWord = &record->flags;
    u16 rankUpFlags = *rankUpWord;
    s8 elementMap[9] = {0, 0, 1, 5, 2, 3, 4, 5, 1};
    MenuMantraRecord *selected;
    s32 index;

    if ((rankUpFlags & 0x2000) && (kind == 0xFF || kind == -1)) {
        evtPrintDeveloperConsoleMessage("EventFlag DrawRankUp!!!\n");
        return record;
    }

    record++;
    selected = NULL;
    for (index = 1; index < 112; index++, record++) {
        if (record->mantraId != 0 && (record->flags & 0x6000) == 0x2000) {
            evtPrintDeveloperConsoleMessage("EventFlag MantraID [%d] i[%d]\n",
                                            record->mantraId, index);
            if (selected != NULL) {
                if (((record->flags >> 8) & 0xF) != kind && kind != -1) {
                    continue;
                }
                if ((record->flags & 0xF00) != 0x200) {
                    continue;
                }
                if (record->flags & 0x1000) {
                    selected = record;
                }
            } else {
                if (((record->flags >> 8) & 0xF) == kind || kind == -1) {
                    if ((record->flags & 0xF00) == 0x300) {
                        if (element == -1 || record->element == elementMap[element]) {
                            selected = record;
                        }
                    } else {
                        selected = record;
                    }
                    if ((record->flags & 0xF00) == 0x400) {
                        return selected;
                    }
                }
            }
        }
    }
    return selected;
}


typedef struct MantraAttributeNameTable {
    char text[6][9];
} MantraAttributeNameTable;

typedef struct MantraAcquisitionDialogues {
    s32 entries[6][2];
} MantraAcquisitionDialogues;

extern DspUnitName *D_00435E48;
extern DspMantraName *D_00435E50;
extern const MantraAttributeNameTable D_004275B0;
extern const MantraAcquisitionDialogues D_004275E8;
extern const char D_00437940[];
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern void prfSummarizeNonzeroEntryAttributes(s32, s32 *);
extern void *memset(void *, s32, u32);
extern s32 sprintf(char *, const char *, ...);
extern void ptyAdjustAllActiveRosterStats(s32, s32);

s32 mnuHandleMantraSourceDialogResult(MantraMenuSrc *source, MnuStatusResource *object) {
    u16 *sourceBits;
    s32 attributeSummary[2];
    MantraAttributeNameTable attributeNames = D_004275B0;
    char formatted[16];
    MantraAcquisitionDialogues messageEntries = D_004275E8;
    char *text = attributeNames.text[0];
    s32 activeVariant;
    s32 mode;
    s32 messageEntry;
    u32 currentFlags;
    u32 publishedFlags;
    DatPartyRecord *party;
    MantraMenuWork *state;

    if (source == NULL) {
        return 0;
    }

    /* This is the existing u16 member, disjoint from the local dialog copy. */
    sourceBits = &source->bits;
    state = &object->menu;
    party = object->list->cursor->partyRecord;
    activeVariant = state->drawBits.unk15 != 0;
    memset(attributeSummary, 0, sizeof(attributeSummary));
    if (*sourceBits & 0x8000) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48[party->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(1, D_00435E50[source->fields.unitIndex].encodedText);
        *sourceBits &= ~0x2000;
        dspStartEntry(messageEntries.entries[0][activeVariant]);
        return -1;
    }

    evtCopyEntryStringToActiveWindow(0, D_00435E48[party->unitId].encodedText);
    evtCopyEntryStringToActiveWindow(1, D_00435E50[source->fields.unitIndex].encodedText);

    currentFlags = *sourceBits;
    mode = currentFlags & 0x0F00;
    if (currentFlags & 0x1000) {
        prfSummarizeNonzeroEntryAttributes(source->fields.unitIndex, attributeSummary);
        evtCopyEntryStringToActiveWindow(2, text + attributeSummary[0] * 9);
        text = formatted;
        sprintf(text, D_00437940, attributeSummary[1]);
        evtCopyEntryStringToActiveWindow(3, text);
        currentFlags = *sourceBits;
        mode = currentFlags & 0x0F00;
    }
    if (mode == 0x0200) {
        if (currentFlags & 0x1000) {
            if (attributeSummary[0] != 5) {
                messageEntry = messageEntries.entries[1][activeVariant];
            } else {
                messageEntry = messageEntries.entries[2][activeVariant];
            }
        } else {
            messageEntry = messageEntries.entries[3][activeVariant];
        }
    } else if (mode == 0x0300) {
        messageEntry = messageEntries.entries[4][activeVariant];
    } else if (mode == 0x0400) {
        messageEntry = messageEntries.entries[5][activeVariant];
    } else {
        goto complete;
    }
    dspStartEntry(messageEntry);
    /* The dialog callback can update the caller-owned acquisition record. */
    currentFlags = *sourceBits;

complete:
    /* Without a dialog callback, the earlier flags snapshot is still current. */
    publishedFlags = (currentFlags & ~0x2000) | 0x4000;
    *sourceBits = publishedFlags;
    if ((publishedFlags & 0x0F00) == 0x0300) {
        *sourceBits = publishedFlags & ~0x4000;
    }

    if ((((state->drawFlags >> 15) & (*sourceBits >> 12)) & 1) != 0) {
        ptyAdjustAllActiveRosterStats(attributeSummary[0], attributeSummary[1]);
    }
    return source->fields.unitIndex;
}

/* The dialog copies all six fixed-width attribute names before selecting one. */
const MantraAttributeNameTable D_004275B0 __attribute__((aligned(8))) = {
    {"Strength", "Vitality", "Magic", "Agility", "Luck", "ALL"}
};

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275E8);

s32 mnuGetMantraNeighborIndexForDirection(MnuStatusResource *object, s8 direction) {
    MantraNodePos *position = object->menu.defaultSelector;
    s32 grid[2];
    s32 neighbor;

    grid[0] = position->x / 10 + (position->x & 1);
    grid[1] = position->y / 10;
    switch (direction) {
    /* Vertical moves depend on the staggered row; horizontal moves do not. */
    case 1: neighbor = (grid[1] & 1) ? 5 : 0; break;
    case 4: neighbor = (grid[1] & 1) ? 3 : 2; break;
    case 2: neighbor = 1; break;
    case 8: neighbor = 4; break;
    /* Upper diagonals, then lower diagonals. */
    case 9: neighbor = 5; break;
    case 3: neighbor = 0; break;
    case 12: neighbor = 3; break;
    case 6: neighbor = 2; break;
    default: return -1;
    }
    return neighbor;
}


s32 mnuClassifyMantraNeighborStatus(MnuStatusResource *object, MantraNodePos *list, s8 position) {
    u16 selected;
    MantraNodePos *entry;
    u16 value;
    s32 result;

    if (list == NULL || position == -1) {
        return -1;
    }
    selected = mnuGetSelectedMantraUnitIndex((MnuStatusResource *)object);
    entry = list->neighbors[position];
    result = 0;
    if (entry == NULL) {
        result = 1;
    } else {
        value = object->menu.slots[selected]->flags[entry->selector.fields.index];
        if ((value & 15) == 3) {
            result = 2;
        } else if ((entry->selector.packed & 0x100) != 0 && ((value >> 8) & 8) != 0) {
            switch (position) {
                case 1:
                case 4:
                    result = 3;
                    break;
                default:
                    result = 2;
                    break;
            }
        }
    }
    return result;
}

extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern s32 mnuGetMantraNeighborIndexForDirection(MnuStatusResource *, s8);
extern MantraNodePos *mnuResolveSpecialMantraNeighbor(MnuStatusResource *, u16, s8);
typedef struct MantraNeighborIds {
    u16 id;
    u16 neighbors[6];
} MantraNeighborIds;
extern MantraNeighborIds D_003D0130[50];

s32 mnuNavigateMantraSelector(MnuStatusResource *object, s8 flags) {
    MantraMenuWork *state = &object->menu;
    MantraNodePos *node;
    MantraNodePos *neighbor;
    s32 selected;
    s8 position;
    s8 attempts = 0;
    s8 result;
    u16 i;
    u16 id;

    selected = (u16)mnuGetSelectedMantraUnitIndex((MnuStatusResource *)object);
    if (state->navigationState == 0 || state->navigationState == 7) {
        node = state->defaultSelector;
        position = mnuGetMantraNeighborIndexForDirection(object, flags);
        for (;;) {
            result = mnuClassifyMantraNeighborStatus(object, node, position);
            switch (result) {
                case -1:
                    return 0;
                case 0:
                    state->defaultSelector = node->neighbors[position];
                    if (state->navigationState == 0) {
                        state->navigationState = 4;
                    }
                    return 1;
                case 1:
                    if ((flags & 0xa) != 0 || attempts == 2) {
                        for (i = 0; i < 50; i++) {
                            if (D_003D0130[i].id == node->selector.fields.index) {
                                id = D_003D0130[i].neighbors[position];
                                if (id != 0) {
                                    neighbor = mnuGetMantraNodePositionRecord(id);
                                    if ((state->slots[selected]->flags[neighbor->selector.fields.index] & 15) != 3) {
                                        state->defaultSelector = neighbor;
                                        if (state->navigationState == 0) {
                                            state->navigationState = 4;
                                        }
                                        return 1;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    if (attempts == 2) {
                        return 0;
                    }
                    /* Try the opposite direction when this neighbor is unavailable. */
                case 2:
                    if (position == 1 || position == 4) {
                        return 0;
                    }
                    if (position == 0) {
                        position = 5;
                    } else if (position == 5) {
                        position = 0;
                    } else if (position == 2) {
                        position = 3;
                    } else if (position == 3) {
                        position = 2;
                    }
                    attempts++;
                    if (attempts == 2) {
                        neighbor = mnuResolveSpecialMantraNeighbor(object, state->defaultSelector->selector.fields.index, position);
                        if (neighbor != NULL) {
                            state->defaultSelector = neighbor;
                            if (state->navigationState == 0) {
                                state->navigationState = 4;
                            }
                            return 1;
                        }
                    } else if (attempts >= 3) {
                        return 0;
                    }
                    break;
                case 3:
                    state->defaultSelector = node->neighbors[position]->neighbors[position];
                    if (state->navigationState == 0) {
                        state->navigationState = 4;
                    }
                    return 1;
            }
        }
    }
    return 0;
}

extern s32 mnuGetActiveMantraModelFlagState(void);
extern s32 mdlFlagTest(s32);

extern void *memcpy(void *, const void *, u32);
extern const s16 D_00427668[4][3];
extern const u16 D_00427680[3][5];
extern const u16 D_004276A0[3][5];

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437940);

MantraNodePos *mnuResolveSpecialMantraNeighbor(MnuStatusResource *object, u16 id, s8 direction) {
    s32 i;
    s8 activeFlags = 0;
    u16 bridgeIds[4] = {0x61, 0x62, 0x63, 0x64};
    s16 destinations[4][3];
    u16 horizontalNodes[3][5];
    u16 verticalNodes[3][5];
    s8 state;

    memcpy(destinations, D_00427668, sizeof(destinations));
    memcpy(horizontalNodes, D_00427680, sizeof(horizontalNodes));
    memcpy(verticalNodes, D_004276A0, sizeof(verticalNodes));
    state = mnuGetActiveMantraModelFlagState();

    if (state == 3) {
        return NULL;
    }
    for (i = 0; i < 4; i++) {
        if (mdlFlagTest(0x977 + i)) {
            activeFlags |= 1 << i;
        }
    }
    for (i = 0; i < sizeof(bridgeIds) / sizeof(bridgeIds[0]); i++) {
        if (bridgeIds[i] == id) {
            if (i >= 2) {
                if (direction == 5 || direction == 0) {
                    return mnuGetMantraNodePositionRecord(destinations[i][state]);
                }
            } else {
                if (direction == 2 || direction == 3) {
                    return mnuGetMantraNodePositionRecord(destinations[i][state]);
                }
            }
            return NULL;
        }
    }
    for (i = 0; i < 5; i++) {
        if (horizontalNodes[state][i] == id &&
            (direction == 5 || direction == 0)) {
            if (i < 2) {
                if (activeFlags & 1) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[0]);
                }
                if (activeFlags & 2) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[1]);
                }
            } else {
                if (activeFlags & 2) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[1]);
                }
                if (activeFlags & 1) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[0]);
                }
            }
        }
    }
    for (i = 0; i < 5; i++) {
        if (verticalNodes[state][i] == id &&
            (direction == 2 || direction == 3)) {
            if (i < 2) {
                if (activeFlags & 4) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[2]);
                }
                if (activeFlags & 8) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[3]);
                }
            } else {
                if (activeFlags & 8) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[3]);
                }
                if (activeFlags & 4) {
                    return mnuGetMantraNodePositionRecord(bridgeIds[2]);
                }
            }
        }
    }
    return NULL;
}

MantraNodePos *mnuGetDefaultPanelSelector(MnuStatusResource *object) {
    return object->menu.defaultSelector;
}

extern void mnuUpdateMantraRecordFlags(void *, s32, s32);
extern void *evtAllocateMantraSelectionWork(s32, s32);
extern void mnuMergePartyMantraProfileCapStates(void *);

s32 mnuRefreshMantraSelectionResources(MnuStatusResource *object) {
    MantraMenuWork *menu;
    struct MenuListNode *node;
    MantraFlagResource **slot;
    s32 source;

    menu = &object->menu;
    node = object->list->first;
    if (node != 0) {
        slot = menu->slots;
        while (node != 0) {
            source = node->unk70;
            if (*slot != 0) {
                mnuUpdateMantraRecordFlags(*slot, source, 0);
            } else {
                *slot = evtAllocateMantraSelectionWork(source, 0);
            }
            node = node->next;
            slot++;
        }
    }
    if (menu->slots[5] != 0) {
        mnuMergePartyMantraProfileCapStates(menu->slots);
    } else {
        menu->slots[5] = evtAllocateMantraSelectionWork(0, 0);
        mnuMergePartyMantraProfileCapStates(menu->slots);
    }
    return 1;
}

s32 mnuUpdateSelectedPanelSlot(MnuStatusResource *object) {
    s32 index;
    s32 source;

    index = mnuGetSelectedMantraUnitIndex((MnuStatusResource *)object);
    source = object->list->cursor->unk70;
    if (object->menu.slots[index] != 0) {
        mnuUpdateMantraRecordFlags(object->menu.slots[index], source, 0);
    } else {
        object->menu.slots[index] = evtAllocateMantraSelectionWork(source, 0);
    }
    return 1;
}

u16 mnuGetSelectedPanelValue(MnuStatusResource *object) {
    s32 values;

    values = (s32)object->menu.slots[object->list->cursor->index]->flags;
    if (object->menu.alternateSelector != 0) {
        return *(u16 *)(object->menu.alternateSelector->selector.fields.index * 2 + values);
    }
    return *(u16 *)(object->menu.defaultSelector->selector.fields.index * 2 + values);
}

u16 mnuGetPanelValueAt(MnuStatusResource *object, s32 index) {
    return *(u16 *)
                    (((index << 0x10) >> 0xf) +
                    (s32)object->menu.slots[object->list->cursor->index]->flags);
}

INCLUDE_ASM(const s32, "game/code_0028FD30", mnuDrawMantraNodeDetails);


typedef struct MantraModelFlag {
    u16 panelIndex : 8;
    u16 kind : 4;
    u16 state : 1;
    u16 unused : 3;
    u16 modelFlag;
    u8 variant;
    u8 pad05[3];
} MantraModelFlag;

extern void *memset(void *, s32, u32);
extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);

void mnuSynchronizeMantraModelFlags(s32 mode) {
    MantraModelFlag flags[112];
    MantraNodePos *node;
    s32 i = 175;
    s32 count;
    s32 variant;

    memset(flags, 0, sizeof(flags));
    node = mnuGetMantraNodePositionRecord(0);
    count = 0;
    for (; i >= 0; i--, node++) {
        if (node->selector.fields.index != 0) {
            s32 kind = node->selector.packed & 15;

            if (kind == 2 || kind == 4) {
                u32 packed;

                flags[count].panelIndex = node->selector.fields.index;
                packed = node->selector.packed;
                flags[count].kind = packed & 15;
                flags[count].state = (packed >> 8) & 1;
                flags[count].modelFlag = count + 0x923;
                count++;
            } else if (kind == 3) {
                for (variant = 0; variant < 6; variant++) {
                    u32 packed;

                    flags[count].panelIndex = node->selector.fields.index;
                    packed = node->selector.packed;
                    flags[count].kind = packed & 15;
                    flags[count].state = (packed >> 8) & 1;
                    flags[count].modelFlag = count + 0x923;
                    flags[count].variant = variant;
                    count++;
                }
            }
        }
    }
    for (i = 0; i < 112; i++) {
        if (flags[i].kind == 3) {
            switch (mode) {
            case 0:
                if (mdlFlagTest(flags[i].modelFlag)) {
                    mdlFlagSet(flags[i].modelFlag + 1);
                }
                i += 5;
                break;
            case 1:
                if (mdlFlagTest(flags[i].modelFlag + 1)) {
                    mdlFlagSet(flags[i].modelFlag);
                }
                i += 5;
                break;
            }
        }
    }
}

extern void ptyRecomputeMaxVitals(DatPartyRecord *, const s32 *);
extern s32 ptyAddClampedEntryValue(DatPartyRecord *entry, s32 statIndex, s32 amount);

/* Apply one stat, or all five base stats, to each occupied party slot. */
void ptyAdjustAllActiveRosterStats(s32 statIndex, s32 amount) {
    s32 index;

    for (index = 0; index < 5; index++) {
        if ((u16)(datGameState->party[index].flags & 1) != 0) {
            DatPartyRecord *entry = &datGameState->party[index];

            if (statIndex == DAT_BASE_STAT_COUNT) {
                s32 stat;

                for (stat = 0; stat < DAT_BASE_STAT_COUNT; stat++) {
                    ptyAddClampedEntryValue(entry, stat, amount);
                }
            } else {
                ptyAddClampedEntryValue(entry, statIndex, amount);
            }
            ptyRecomputeMaxVitals(entry, NULL);
        }
    }
}

extern void scrSetEntryLowFlags(DatPartyRecord *context, u16 entryId, u16 lowFlags);
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

const s16 D_00427668[4][3] __attribute__((section(".rodata"), aligned(8))) = {
    { 0x36, 0x93, 0x34 },
    { 0x28, 0x94, 0x2A },
    { 0x4C, 0x73, 0x51 },
    { 0x01, 0x74, 0x03 }
};

const u16 D_00427680[3][5] __attribute__((section(".rodata"), aligned(8))) = {
    { 0x36, 0, 0x72, 0x28, 0 },
    { 0x93, 0x37, 0x33, 0x94, 0 },
    { 0x38, 0x34, 0x98, 0x2A, 0x2B }
};

const u16 D_004276A0[3][5] __attribute__((section(".rodata"), aligned(8))) = {
    { 0x4C, 0, 0x71, 0x01, 0 },
    { 0x73, 0x4D, 0x02, 0x74, 0 },
    { 0x4E, 0x51, 0x97, 0x03, 0x04 }
};

/* Write each panel slot's 0xB0 saved flag words back to its list node's script entries and log the slot number. */
INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276C0);

void mnuStoreMantraPanelFlagsToScript(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        DatPartyRecord *context = node->partyRecord;
        u16 *values = object->menu.slots[slotIndex]->flags;
        s32 i;

        for (i = 0; i < 0xB0; i++) {
            scrSetEntryLowFlags(context, i, *values++);
        }
        evtPrintDeveloperConsoleMessage("[%d]Mantra Data Saved!!!!!!!!!!!!!!!!!\n", slotIndex++);
    }
}

extern u16 scrGetEntryLowFlags(DatPartyRecord *context, u16 entryId);

/* Read each list node's 0xB0 script entry flags into its panel slot and log the slot number. */
void mnuLoadMantraPanelFlagsFromScript(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        DatPartyRecord *context = node->partyRecord;
        u16 *values = object->menu.slots[slotIndex]->flags;
        s32 i;

        for (i = 0; i < 0xB0; i++) {
            *values++ = scrGetEntryLowFlags(context, i);
        }
        evtPrintDeveloperConsoleMessage("[%d]Mantra Data Load!!!!!!!!!!!!!!!!!\n", slotIndex++);
    }
}

extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16 scriptId);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *work, u16 scriptId);

s32 mnuValidateProfileEntry(MantraFlagResource *slot, DatPartyRecord *partyRecord) {
    u16 target = scrGetSelectedScriptEntryId(partyRecord) & 0xFFFF;
    u16 *dst = slot->flags;
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        u16 flags = scrGetEntryLowFlags(partyRecord, i);

        *dst++ = flags;
        if (i == target) {
            if (((flags >> 8) & 1) != 0) {
                target = 0;
            }
        }
    }
    if (target != 0 && ptyGetProfileRecordCap(target) == ptyGetProfileRecordValue(partyRecord, target)) {
        return target;
    }
    return 0;
}

/* Preserve the flags while changing the four-bit panel status. */
static inline s32 mnuPanelFlagsWithStatus(s32 flags, s32 status) {
    return (flags & ~15) | status;
}

extern MenuPanelPositionRecord D_003CFEB8[][37];

/* Reset all tutorial entries; mark the two tutorial-specific mantra ids.
 * cc1 keeps the consumed mask and shared status/group value across both
 * loops. Initializing group before them gives retail's sched1 LUID tie. */
void mnuPrepareMantraPanelEntriesForTutorial(void) {
    s32 group = 0;
    s32 statusMask = ~0xF;
    s32 defaultStatus = 2;
    u32 entry;

    for (; group < 3; group++) {
        for (entry = 0; entry < 37; entry++) {
            D_003CFEB8[group][entry].stateFlags = defaultStatus;
            if (group == 1 && D_003CFEB8[1][entry].id == 0x4B) {
                D_003CFEB8[1][entry].stateFlags =
                    (D_003CFEB8[1][entry].stateFlags & statusMask) | 1;
            }
            if (group == defaultStatus && D_003CFEB8[2][entry].id == 0x45) {
                D_003CFEB8[2][entry].stateFlags =
                    (D_003CFEB8[2][entry].stateFlags & statusMask) | 1;
            }
        }
    }
}


/* Find one panel record by its script ID within a selected 37-entry row. */
MenuPanelPositionRecord *mnuFindPanelPositionRecordByScriptId(s32 selector, u16 id) {
    s32 offset = (selector << 7) + ((selector * 5) << 2);
    MenuPanelPositionRecord *entry =
        (MenuPanelPositionRecord *)((u8 *)D_003CFEB8 + offset);
    u32 i = 0;

    for (i = 0; i < 37; i++, entry++) {
        if (id == entry->id) {
            return entry;
        }
    }
    return NULL;
}


void mnuActivatePanelSelection(MnuStatusResource *object, s8 selection) {
    MantraMenuWork *state = &object->menu;
    if (mnuQueueUnitPanelSelection(object->menu.selectionController, selection) != 0) {
        u32 flags;
        u32 option;
        mnuMoveNodeCursorToTargetIndex(object, selection);
        mnuTransitionActivePanelAnimations(state->resource, 0);
        flags = state->flags;
        mnuUpdateMantraSelectionPanelSlots(object, 5, 1, selection, (flags >> 28) & 1, 0);
        option = (selection & 0xf) << 24;
        state->flags = (state->flags & 0xf0ffffff) | option;
    }
}

void mnuSetPanelSelection(MnuStatusResource *object, s8 selection) {
    MantraMenuWork *state = &object->menu;
    if (mnuQueueUnitPanelSelection(object->menu.selectionController, selection) != 0) {
        mnuMoveNodeCursorToTargetIndex(object, selection);
        state->flags = (state->flags & 0xf0ffffff)
            | ((selection & 0xf) << 24);
    }
}

INCLUDE_ASM(const s32, "game/code_0028FD30", mnuUpdateMantraSelectionPanelSlots);

void mnuAcquireMantraAndUnlockNeighbors(MnuStatusResource *object, s32 selector, u16 id) {
    MantraMenuWork *state = &object->menu;
    MantraNodePos *position;
    MantraNodePos **neighbor;
    MenuPanelPositionRecord *record;
    MantraPanelAnimation *animation;
    MantraPanelPool *resource;
    u32 changed = 1;
    s32 i;

    resource = object->menu.resource;
    position = mnuGetMantraPanelPositionRecord(id);
    record = mnuFindPanelPositionRecordByScriptId(selector, id);
    animation = mnuFindPanelSlotById(resource, id, 1);
    mnuQueuePanelAnimationTransition(animation, 1, 0);
    animation->animationTicks = 60;
    animation = mnuSpawnPanelSlotB(resource, id, 2, 61, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 0, 0);
    animation->flags |= 0x10000000;
    record->stateFlags |= 0x100;
    neighbor = position->neighbors;
    for (i = 5; i >= 0; i--, neighbor++) {
        if (*neighbor != NULL) {
            u32 nodeFlags = (*neighbor)->selector.packed;
            s32 maxTier = (state->flags >> 28) & 1;

            if (maxTier >= ((s8)nodeFlags >> 4) && (nodeFlags & 15) == 1) {
                record = mnuFindPanelPositionRecordByScriptId(selector, (*neighbor)->selector.fields.index);
                if ((record->stateFlags & 15) == 2) {
                    changed |= 2;
                    animation = mnuFindPanelSlotById(resource, (*neighbor)->selector.fields.index, 0);
                    mnuQueuePanelAnimationTransition(animation, 1, 10);
                    animation = mnuSpawnPanelSlotB(resource, (*neighbor)->selector.fields.index, 1, 50, 0, 0);
                    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
                    record->stateFlags = mnuPanelFlagsWithStatus(record->stateFlags, 1);
                }
            }
        }
    }
    mnuSpawnAlternateMantraSpecialNodeIcon(object->menu.selectionController);
    mnuSpawnMantraIconWithShortLoop(object->menu.selectionController);
    if (changed & 1) {
        mnuStorePanelEntry(0x20004, 5);
    }
    if (changed & 2) {
        mnuStorePanelEntry(0x20005, 38);
    }
}

void mnuBuildMantraPanelSelectionView(MnuStatusResource *object, s32 selector, u16 id, s32 panelX) {
    MantraMenuWork *state = &object->menu;
    MantraNodePos *position;
    MenuPanelPositionRecord *record;
    MantraPanelAnimation *animation;
    u16 visibleIds[7];
    s32 i;
    s32 recordIndex;

    position = mnuGetMantraPanelPositionRecord((s16)id);
    mnuFindPanelPositionRecordByScriptId(selector, id);
    visibleIds[6] = id;
    for (recordIndex = 0; recordIndex < 6; recordIndex++) {
        visibleIds[recordIndex] = position->neighbors[recordIndex]->id;
    }
    animation = mnuSpawnPanelSlotB(object->menu.resource, id, 6, (s16)panelX, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    for (i = 0; i < 6; i++) {
        mnuGetMantraPanelPositionRecord((s16)visibleIds[i]);
        animation = mnuSpawnPanelSlotB(object->menu.resource, visibleIds[i], 2, (s16)panelX, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    }
    position = mnuGetMantraPanelPositionRecord(0);
    recordIndex = 175;
    do {
        if (position->id != 0) {
            if (position->modelFlagState <= (s32)((state->flags >> 28) & 1)) {
                s32 hidden = 1;

                for (i = 0; i < 7; i++) {
                    if (position->id == visibleIds[i]) {
                        hidden = 0;
                        break;
                    }
                }
                if (hidden) {
                    record = mnuFindPanelPositionRecordByScriptId(selector, id);
                    if ((record->stateFlags & 15) != 3) {
                        animation = mnuSpawnPanelSlotB(object->menu.resource, position->id, 13, (s16)panelX, 0, 0);
                        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
                    }
                }
            }
        }
        position++;
    } while (--recordIndex >= 0);
}

void mnuRefreshMantraPanelNeighborStates(MnuStatusResource *object, s32 selector, u16 id) {
    MantraNodePos *position;
    MantraNodePos **neighbor;
    MenuPanelPositionRecord *record;
    MantraPanelAnimation *animation;
    s32 i;
    s32 k;

    position = mnuGetMantraPanelPositionRecord((s16)id);
    animation = mnuFindPanelSlotById(object->menu.resource, id, 6);
    mnuQueuePanelAnimationTransition(animation, 1, 0);
    i = 0;
    while (i < 3) {
        record = mnuFindPanelPositionRecordByScriptId(i++, id);
        record->stateFlags |= 0x800;
    }
    for (k = 0; k < 3; k++) {
        record = mnuFindPanelPositionRecordByScriptId(k, id);
        record->stateFlags = mnuPanelFlagsWithStatus(record->stateFlags, 2);
        neighbor = position->neighbors;
        for (i = 5; i >= 0; i--, neighbor++) {
            if (*neighbor != NULL) {
                if ((mnuFindPanelPositionRecordByScriptId(k, (*neighbor)->id)->flags & 1) != 0) {
                    record->stateFlags = mnuPanelFlagsWithStatus(record->stateFlags, 1);
                }
            }
        }
    }
    mnuSpawnAlternateMantraSpecialNodeIcon(object->menu.selectionController);
    mnuSpawnMantraIconWithShortLoop(object->menu.selectionController);
    mnuStorePanelEntry(0x20007, 20);
}
/* Four-byte spawn records: signed lookup ID, signed slot kind, reserved byte. */
typedef struct MantraPanelSpawnRecord {
    s16 id;
    s8 kind;
    u8 pad03;
} MantraPanelSpawnRecord;

/* Keep the external bound unspecified: retail uses a full address here.
 * The local two-record snapshot still copies the proven eight bytes. */
extern const MantraPanelSpawnRecord D_00437958[];
extern const MantraPanelSpawnRecord D_00427720[8];
extern const MantraPanelSpawnRecord D_00427740[8];
extern MantraNodePos *mnuGetMantraPanelPositionRecord(s16 id);

void mnuSpawnMantraPanelSequence(MnuStatusResource *object, s32 selector) {
    MantraPanelSpawnRecord initialRows[2];
    MantraPanelSpawnRecord firstRows[8];
    MantraPanelSpawnRecord secondRows[8];
    MantraPanelAnimation *animation;
    s32 delay;
    u32 row;
    u16 panelId;
    const u8 *cursor;
    MantraPanelSpawnRecord *secondBank = secondRows;

    __builtin_memcpy(initialRows, D_00437958, sizeof(initialRows));
    __builtin_memcpy(firstRows, D_00427720, sizeof(firstRows));
    __builtin_memcpy(secondBank, D_00427740, sizeof(secondRows));

    panelId = (u16)initialRows[0].id;
    mnuGetMantraPanelPositionRecord(initialRows[0].id);
    mnuFindPanelPositionRecordByScriptId(selector, panelId);
    animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 1, 0);

    animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, 20, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 3, 0);

    animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, initialRows[0].kind, 80, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 8, 0);

    delay = 0;
    row = 0;
    /* Retail walks the kind byte; recover the containing row for each ID.
     * Start from the whole array representation and stop before advancing
     * beyond its last row. */
    cursor = (const u8 *)&firstRows + 2;
    for (;;) {
        const MantraPanelSpawnRecord *entry =
            (const MantraPanelSpawnRecord *)(cursor - 2);
        f32 oldDelay;
        s32 interpolatedX;

        panelId = (u16)entry->id;
        row++;

        mnuGetMantraPanelPositionRecord(entry->id);
        mnuFindPanelPositionRecordByScriptId(selector, panelId);
        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)delay, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 1, 0);

        oldDelay = (f32)delay;
        interpolatedX = (s32)(oldDelay * 5.0f / 3.0f + 20.0f);
        delay += 3;
        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)interpolatedX, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 3, 0);

        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, *(const s8 *)cursor, 80, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 8, 0);
        if (row >= 8U) {
            break;
        }
        cursor += sizeof(firstRows[0]);
    }

    delay = 0;
    row = 0;
    cursor = (const u8 *)secondBank + 2;
    for (;;) {
        const MantraPanelSpawnRecord *entry =
            (const MantraPanelSpawnRecord *)(cursor - 2);
        f32 oldDelay;
        s32 interpolatedX;

        panelId = (u16)entry->id;
        row++;

        mnuGetMantraPanelPositionRecord(entry->id);
        mnuFindPanelPositionRecordByScriptId(selector, panelId);
        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)delay, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 1, 0);

        oldDelay = (f32)delay;
        interpolatedX = (s32)(oldDelay * 5.0f / 3.0f + 20.0f);
        delay += 3;
        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)interpolatedX, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 3, 0);

        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, *(const s8 *)cursor, 80, 0, 0);
        mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
        mnuQueuePanelAnimationTransition(animation, 8, 0);
        if (row >= 8U) {
            break;
        }
        cursor += sizeof(secondRows[0]);
    }

    panelId = (u16)initialRows[1].id;
    mnuGetMantraPanelPositionRecord(initialRows[1].id);
    mnuFindPanelPositionRecordByScriptId(selector, panelId);
    animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)delay, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 1, 0);

    {
        f32 oldDelay = (f32)delay;
        s32 interpolatedX = (s32)(oldDelay * 5.0f / 3.0f + 20.0f);
        animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, 13, (s16)interpolatedX, 0, 0);
    }
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 3, 0);

    animation = mnuSpawnPanelSlotB(object->menu.resource, panelId, secondRows[1].kind, 80, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 8, 0);
    mnuStorePanelEntry(0x20006, 20);
}

void itfClearSelectionFlags(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;
    state->flags &= 0xff0000ff;
}

extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconAtPosition(s32, s32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSpawnAlternateMantraIconForPanelSoundEntry(struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconFadeIn(struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconFadeOut(struct MantraDrawPool *pool);
extern void mnuSpawnMantraShortLoopIconFadeIn(struct MantraDrawPool *pool);
extern void mnuSpawnMantraShortLoopIconFadeOut(struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconAtPositionWithShortLoop(s32, s32, struct MantraDrawPool *pool);
extern void mnuRefreshMantraPanelNeighborStates(MnuStatusResource *, s32, u16);
extern void mnuSpawnMantraPanelSequence(MnuStatusResource *, s32);
extern void mnuHideMantraInfo(struct MantraDrawPool *pool);
extern void mnuShowMantraInfo(struct MantraDrawPool *pool);
extern void mnuHideMantraTitle(struct MantraDrawPool *pool);
extern void mnuShowMantraTitle(struct MantraDrawPool *pool);
extern void mnuSetMantraBackgroundVariant(struct MantraDrawPool *pool, s8);
extern void mnuBeginMantraBackgroundMaskFadeOut(struct MantraDrawPool *pool);
extern void mnuBeginMantraBackgroundMaskFadeIn(struct MantraDrawPool *pool);
extern struct MantraDrawItem *mnuRegisterMantraIconListCDraw(struct MantraDrawPool *pool, u32);
extern void mnuBeginMantraIconListExit(struct MantraDrawPool *pool);
extern void sndSetSequenceVolumePan(s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427740);

s32 mnuDispatchMantraPanelCommand(void *context, s32 panelId, s32 action) {
    MnuStatusResource *object = context;
    MantraMenuWork *state = &object->menu;
    MantraNodePos *position;
    u16 id = panelId;
    u32 flags;

    switch (action) {
    case 0:
        state->defaultSelector = mnuGetMantraPanelPositionRecord((s16)id);
        mnuSpawnMantraShortLoopIconAtPosition(
            (s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
            (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f),
            object->menu.selectionController);
        mnuSpawnMantraIconAtPosition(
            (s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
            (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f),
            object->menu.selectionController);
        return 0;
    case 1:
        position = mnuGetMantraPanelPositionRecord((s16)id);
        mnuSpawnAlternateMantraIconForPanelSoundEntry(object->menu.selectionController);
        mnuSpawnMantraIconAtPositionWithShortLoop((s32)((f32)position->x / 10.0f * 40.0f),
                     (s32)((f32)position->y / 10.0f * 39.0f),
                     object->menu.selectionController);
        return 0;
    case 2:
        mnuSpawnMantraIconFadeIn(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeIn(object->menu.selectionController);
        return 0;
    case 3:
        mnuSpawnMantraIconFadeOut(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeOut(object->menu.selectionController);
        return 0;
    case 4:
        mnuAcquireMantraAndUnlockNeighbors(object, state->flagBytes[3] & 15, id);
        return 0;
    case 5:
        mnuRefreshMantraPanelNeighborStates(object, state->flagBytes[3] & 15, id);
        return 0;
    case 6:
        mnuActivatePanelSelection(object, state->flagBytes[3] & 15);
        goto play_selection_sound;
    case 7:
        mnuTransitionActivePanelAnimations(state->resource, 1);
        mnuBuildMantraPanelSelectionView(object, state->flagBytes[3] & 15, id, 5);
        return 0;
    case 8:
        mnuTransitionActivePanelAnimations(state->resource, 1);
        flags = state->flags;
        mnuUpdateMantraSelectionPanelSlots(object, 5, 1, (flags >> 24) & 15,
                     (flags >> 28) & 1, 0);
        return 0;
    case 9:
        mnuHideMantraInfo(object->menu.selectionController);
        mnuHideMantraTitle(object->menu.selectionController);
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 3);
        mnuBeginMantraBackgroundMaskFadeOut(object->menu.selectionController);
        mnuSpawnMantraIconFadeOut(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeOut(object->menu.selectionController);
        mnuTransitionActivePanelAnimations(state->resource, 1);
        {
            u32 drawFlags = state->drawFlags;
            struct MantraDrawPool *controller = object->menu.selectionController;

            state->drawFlags = (drawFlags & ~1) | ((drawFlags & 1) ^ 1);
            mnuRegisterMantraIconListCDraw(controller, (u32)object);
        }
        object->menu.unk9B5 = 1;
        object->menu.unk9B6 = 0;
        break;
    case 10:
        mnuShowMantraInfo(object->menu.selectionController);
        mnuShowMantraTitle(object->menu.selectionController);
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 0);
        mnuBeginMantraBackgroundMaskFadeIn(object->menu.selectionController);
        mnuSpawnMantraIconFadeIn(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeIn(object->menu.selectionController);
        flags = state->flags;
        mnuUpdateMantraSelectionPanelSlots(object, 5, 1, (flags >> 24) & 15,
                     (flags >> 28) & 1, 0);
        {
            u32 drawFlags = state->drawFlags;
            struct MantraDrawPool *controller = object->menu.selectionController;

            state->drawFlags = (drawFlags & ~1) | ((drawFlags & 1) ^ 1);
            mnuBeginMantraIconListExit(controller);
        }
        object->menu.unk9B5 = 2;
        object->menu.unk9B6 = 0;
        break;
    case 11:
        position = mnuGetMantraPanelPositionRecord((s16)id);
        mnuSpawnMantraVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
        return 0;
    case 12:
        position = mnuGetMantraPanelPositionRecord((s16)id);
        mnuSpawnMantraShortLoopVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
        return 0;
    case 13:
        mnuSetPanelSelection(object, state->flagBytes[3] & 15);
play_selection_sound:
        sndSetSequenceVolumePan(4, 127, 63);
        return 0;
    case 14:
        mnuSpawnMantraPanelSequence(object, 0);
        return 0;
    case 15:
        return 1;
    default:
        return 0;
    }
    return 0;
}

void mnuStartMantraTutorialPanelReveal(MnuStatusResource *object) {
    MantraPanelPool *resource;
    MenuPanelPositionRecord *record;
    MantraPanelAnimation *animation;

    resource = object->menu.resource;
    mnuGetMantraPanelPositionRecord(0);
    record = mnuFindPanelPositionRecordByScriptId(0, 8);
    animation = mnuFindPanelSlotById(resource, 8, 0);
    mnuQueuePanelAnimationTransition(animation, 7, 0);
    animation = mnuSpawnPanelSlotB(resource, 8, 1, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 8, 0);
    record->stateFlags = mnuPanelFlagsWithStatus(record->stateFlags, 1);
}

extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconAtPosition(s32, s32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconFadeIn(struct MantraDrawPool *pool);
extern void mnuSpawnMantraShortLoopIconFadeIn(struct MantraDrawPool *pool);

void mnuInitializeMantraTutorialSelector(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;

    state->defaultSelector = mnuGetMantraPanelPositionRecord(8);
    mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->menu.selectionController);
    mnuSpawnMantraIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->menu.selectionController);
    mnuSpawnMantraIconFadeIn(object->menu.selectionController);
    mnuSpawnMantraShortLoopIconFadeIn(object->menu.selectionController);
    state->flags &= ~0x20000000;
}

void mnuAcquireTutorialMantraNodeAndNeighbors(MnuStatusResource *object) {
    mnuAcquireMantraAndUnlockNeighbors(object, 0, 8);
}


extern void mnuSpawnAlternateMantraIconForPanelSoundEntry(struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconAtPositionWithShortLoop(s32, s32, struct MantraDrawPool *pool);
void itfPositionMantraSelectionController(MnuStatusResource *object) {
    MantraNodePos *record;

    record = mnuGetMantraPanelPositionRecord(8);
    mnuSpawnAlternateMantraIconForPanelSoundEntry(object->menu.selectionController);
    mnuSpawnMantraIconAtPositionWithShortLoop((s32)((f32)record->x / 10.0f * 40.0f), (s32)((f32)record->y / 10.0f * 39.0f), object->menu.selectionController);
}

extern void itfInstallDefaultMantraSelector(MnuStatusResource *);

void itfInstallDefaultMantraSelector(MnuStatusResource *object) {
    MantraNodePos *record;
    MantraMenuWork *state = &object->menu;

    record = mnuGetMantraPanelPositionRecord(0x71);
    state->defaultSelector = record;
    mnuSpawnMantraIconAtPosition((s32)((f32)record->x / 10.0f * 40.0f),
        (s32)((f32)record->y / 10.0f * 39.0f), object->menu.selectionController);
}

s32 mnuRunMantraTutorialFourthSequence(MnuStatusResource *object) {
    MenuPanelTransition steps[23] = {
        {0, 22, 0, 0},
        {30, 22, 0, 4},
        {90, 22, 0, 1},
        {120, 0, 1, 6},
        {120, 75, 1, 0},
        {150, 75, 1, 4},
        {210, 75, 1, 1},
        {240, 15, 1, 0},
        {270, 15, 1, 4},
        {330, 15, 1, 1},
        {360, 0, 2, 6},
        {360, 69, 2, 0},
        {390, 69, 2, 4},
        {450, 69, 2, 1},
        {480, 39, 2, 0},
        {510, 39, 2, 4},
        {570, 39, 2, 1},
        {600, 113, 2, 0},
        {630, 113, 2, 7},
        {630, 113, 2, 5},
        {770, 113, 2, 8},
        {800, 113, 2, 1},
        {800, 0, 2, 15},
    };
    MantraMenuWork *state = &object->menu;
    u32 i;

    for (i = 0; i < 23; i++) {
        if (steps[i].frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((steps[i].mode & 15) << 24);
            if (mnuDispatchMantraPanelCommand(object, steps[i].panelId, steps[i].action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}



s32 mnuRunMantraTutorialFifthSequence(MnuStatusResource *object) {
    MenuPanelTransition steps[6] = {
        {0, 0, 0, 6}, {0, 0x16, 0, 0},
        {30, 0x16, 0, 9}, {30, 0x16, 0, 11},
        {30, 0x16, 0, 12}, {60, 0x16, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MantraMenuWork *state = &object->menu;
    u32 i;

    for (i = 0; i < 6; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (mnuDispatchMantraPanelCommand(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 mnuRunMantraTutorialSixthSequence(MnuStatusResource *object) {
    MenuPanelTransition steps[7] = {
        {0, 0, 1, 13}, {0, 15, 1, 12},
        {30, 0, 2, 13}, {30, 0x71, 2, 12},
        {60, 0, 0, 13}, {60, 0x16, 0, 12},
        {100, 0x16, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MantraMenuWork *state = &object->menu;
    u32 i;

    for (i = 0; i < 7; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (mnuDispatchMantraPanelCommand(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 mnuRunMantraTutorialSeventhSequence(MnuStatusResource *object) {
    MenuPanelTransition steps[8] = {
        {0, 0, 0, 10}, {30, 1, 0, 0},
        {60, 1, 0, 4}, {150, 1, 0, 1},
        {180, 1, 0, 3}, {180, 1, 0, 14},
        {280, 1, 0, 2}, {280, 0, 0, 15}
    };
    MenuPanelTransition *step = steps;
    MantraMenuWork *state = &object->menu;
    u32 i;

    for (i = 0; i < 8; i++, step++) {
        if (step->frame == (u16)(state->flags >> 8)) {
            state->flags = (state->flags & 0xF0FFFFFF) | ((step->mode & 15) << 24);
            if (mnuDispatchMantraPanelCommand(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

/* Set up the three tutorial party records and their display order. */
void mnuInitializeMantraTutorialParty(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;
    s32 index;

    memset(state->tutorialParty, 0, sizeof(state->tutorialParty));
    state->tutorialParty[0].unitId = 1;
    state->tutorialParty[0].level = 1;
    state->tutorialParty[1].unitId = 4;
    state->tutorialParty[1].level = 1;
    state->tutorialParty[2].unitId = 5;
    state->tutorialParty[2].level = 1;

    for (index = 2; index >= 0; index--) {
        state->collectedValues[index] = (u32)&state->tutorialParty[index];
    }

    state->flags &= 0xF0FFFFFF;
    state->flags &= 0xEFFFFFFF;
    state->collectedCount = 3;
    state->savedSelection = 0;
}

void mnuCollectPanelNodeValues(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 count = 0;
    MantraMenuWork *state = &object->menu;
    if (node != 0) {
        u32 *slot = object->menu.collectedValues;
        do {
            u32 value = node->unk70;
            count++;
            node = node->next;
            *slot++ = value;
        } while (node != 0);
    }
    state->collectedCount = count;
    state->savedSelection = object->menu.selectionIndex;
}

void mnuBeginMantraTutorialFromNormalMode(MnuStatusResource *object) {
    object->menu.flagBytes[0] = 3;
    object->menu.unk9B5 = 0;
    object->menu.unk9B6 = 0;
    object->menu.flags = (object->menu.flags & 0xF0FFFFFF) | 0x20000000;
    mnuPrepareMantraPanelEntriesForTutorial();
}



s32 mnuUpdateMantraTutorial(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;
    MantraNodePos *position;
    s32 finished = 0;

    switch (state->flagBytes[0]) {
    case 3:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_NORMAL_OUT\n");
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 2);
        mnuDisableMantraBackground(object->menu.selectionController);
        if (state->drawBits.showOverlay) {
            mnuHideMantraLimitLine(object->menu.selectionController);
        }
        mnuBeginMantraBackgroundMaskFadeOut(object->menu.selectionController);
        mnuToggleMantraTitleVariant(object->menu.selectionController);
        mnuHideMantraInfo(object->menu.selectionController);
        mnuHideMantraScrollCursor(object->menu.selectionController);
        mnuBeginMantraUnitPanelExit(object->menu.selectionController);
        mnuSpawnMantraIconFadeOut(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeOut(object->menu.selectionController);
        mnuTransitionActivePanelAnimations(state->resource, 1);
        state->tutorialNextState = 5;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_NORMAL_OUT ...->MTR_MSL_TUTORIAL_INIT\n");
        break;
    case 4:
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 0);
        if (state->drawBits.showOverlay) {
            mnuShowMantraLimitLine(object->menu.selectionController);
            mnuEnableMantraBackground(object->menu.selectionController);
        }
        mnuBeginMantraBackgroundMaskFadeIn(object->menu.selectionController);
        mnuShowMantraInfo(object->menu.selectionController);
        mnuShowMantraScrollCursor(object->menu.selectionController);
        mnuRegisterMantraUnitPanelDraw(object->menu.selectionController, (u32)state->collectedValues);
        mnuSpawnMantraPanelsFromSelectedUnitFlags(object, 2, 0);
        position = mnuGetMantraNodePositionRecord(scrGetSelectedScriptEntryId(mnuGetSelectedNodeValue(object)));
        mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)position->x / 10.0f * 40.0f),
            (s32)((f32)position->y / 10.0f * 39.0f), object->menu.selectionController);
        state->defaultSelector = state->savedSelector;
        position = state->defaultSelector;
        mnuSpawnMantraIconAtPosition((s32)((f32)position->x / 10.0f * 40.0f),
            (s32)((f32)position->y / 10.0f * 39.0f), object->menu.selectionController);
        finished = 1;
        break;
    case 1:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_OUT\n");
        mnuBeginMantraBackgroundMaskFadeOut(object->menu.selectionController);
        mnuToggleMantraTitleVariant(object->menu.selectionController);
        mnuHideMantraInfo(object->menu.selectionController);
        mnuBeginMantraUnitPanelExit(object->menu.selectionController);
        mnuSpawnMantraIconFadeOut(object->menu.selectionController);
        mnuSpawnMantraShortLoopIconFadeOut(object->menu.selectionController);
        mnuTransitionActivePanelAnimations(state->resource, 1);
        state->tutorialNextState = 6;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_OUT ...->MTR_MSL_TUTORIAL_MODE_CHANGE_NORMAL_IN\n");
        break;
    case 2:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_IN\n");
        mnuBeginMantraBackgroundMaskFadeIn(object->menu.selectionController);
        mnuShowMantraInfo(object->menu.selectionController);
        mnuRegisterMantraUnitPanelDraw(object->menu.selectionController, (u32)state->collectedValues);
        mnuUpdateMantraSelectionPanelSlots(object, 0, 0, 0, 0, 0);
        state->tutorialNextState = 7;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_IN ...->MTR_MSL_TUTORIAL_000_RUN\n");
        break;
    case 5:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_INIT\n");
        mnuInitializeMantraTutorialParty(object);
        object->menu.scrollX = 124;
        object->menu.scrollY = 160;
        dspCloseChannel();
        evtCreateMessageWindowIfMissing(object->messageDefinition);
        state->tutorialNextState = 2;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_INIT ...->MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_IN\n");
        break;
    case 6:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_EXIT\n");
        mnuCollectPanelNodeValues(object);
        state->flags &= ~0x20000000;
        mnuMoveNodeCursorToTargetIndex(object, state->selectionIndex);
        state->tutorialNextState = 4;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_EXIT ...->MTR_MSL_TUTORIAL_MODE_CHANGE_NORMAL_IN\n");
        break;
    case 7:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_000_RUN\n");
        state->tutorialNextState = 8;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_000_RUN ...->MTR_MSL_TUTORIAL_000\n");
        break;
    case 8:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_000\n");
        dspStartEntry(6);
        state->tutorialNextState = 9;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_000 ...->MTR_MSL_TUTORIAL_000_WAIT\n");
        break;
    case 9:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 10;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_000_WAIT\n");
        }
        break;
    case 10:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN\n");
        mnuStartMantraTutorialPanelReveal(object);
        state->tutorialNextState = 11;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN ...->MTR_MSL_TUTORIAL_001_RUN2\n");
        break;
    case 11:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN2\n");
        mnuInitializeMantraTutorialSelector(object);
        state->tutorialNextState = 12;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN2 ...->MTR_MSL_TUTORIAL_001\n");
        break;
    case 12:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001\n");
        dspStartEntry(7);
        state->tutorialNextState = 13;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001 ...->MTR_MSL_TUTORIAL_001_WAIT\n");
        break;
    case 13:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 14;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_WAIT\n");
        }
        break;
    case 14:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002_RUN\n");
        mnuAcquireTutorialMantraNodeAndNeighbors(object);
        state->tutorialNextState = 15;
        state->tutorialWaitFrames = 60;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002_RUN ...->MTR_MSL_TUTORIAL_002_RUN2\n");
        break;
    case 15:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002_RUN2\n");
        itfPositionMantraSelectionController(object);
        state->tutorialNextState = 16;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002_RUN2 ...->MTR_MSL_TUTORIAL_002\n");
        break;
    case 16:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002\n");
        dspStartEntry(8);
        state->tutorialNextState = 17;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002 ...->MTR_MSL_TUTORIAL_003_WAIT\n");
        break;
    case 17:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 18;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_002_WAIT\n");
        }
        break;
    case 18:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_003_RUN\n");
        itfInstallDefaultMantraSelector(object);
        state->tutorialNextState = 19;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_003_RUN ...->MTR_MSL_TUTORIAL_003\n");
        break;
    case 19:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_003\n");
        dspStartEntry(9);
        state->tutorialNextState = 20;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_003 ...->MTR_MSL_TUTORIAL_004_WAIT\n");
        break;
    case 20:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 21;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_003_WAIT\n");
        }
        break;
    case 21:
        if (mnuRunMantraTutorialFourthSequence(object)) {
            state->tutorialNextState = 22;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_004_RUN ...->MTR_MSL_TUTORIAL_004\n");
        }
        break;
    case 22:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_004\n");
        dspStartEntry(10);
        state->tutorialNextState = 23;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_004 ...->MTR_MSL_TUTORIAL_004_WAIT\n");
        break;
    case 23:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 24;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_004_WAIT\n");
        }
        break;
    case 24:
        if (mnuRunMantraTutorialFifthSequence(object)) {
            state->tutorialNextState = 25;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_005_RUN ...->MTR_MSL_TUTORIAL_005\n");
        }
        break;
    case 25:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_005\n");
        dspStartEntry(11);
        state->tutorialNextState = 26;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_005 ...->MTR_MSL_TUTORIAL_005_WAIT\n");
        break;
    case 26:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 27;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_005_WAIT\n");
        }
        break;
    case 27:
        if (mnuRunMantraTutorialSixthSequence(object)) {
            state->tutorialNextState = 28;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_006_RUN ...->MTR_MSL_TUTORIAL_006\n");
        }
        break;
    case 28:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_006\n");
        dspStartEntry(12);
        state->tutorialNextState = 29;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_006 ...->MTR_MSL_TUTORIAL_006_WAIT\n");
        break;
    case 29:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 30;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_006_WAIT\n");
        }
        break;
    case 30:
        if (mnuRunMantraTutorialSeventhSequence(object)) {
            state->tutorialNextState = 31;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_007_RUN ...->MTR_MSL_TUTORIAL_007\n");
        }
        break;
    case 31:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_007\n");
        dspStartEntry(13);
        state->tutorialNextState = 32;
        state->tutorialWaitFrames = 0;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_007 ...->MTR_MSL_TUTORIAL_007_WAIT\n");
        break;
    case 32:
        if (evtGetMessageWindowControlState() == 0) {
            state->tutorialNextState = 1;
            state->tutorialWaitFrames = 0;
            state->flagBytes[0] = 37;
            itfClearSelectionFlags(object);
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_007_WAIT\n");
        }
        break;
    case 37:
        if (state->tutorialWaitFrames > 0) {
            state->tutorialWaitFrames--;
        }
        if (state->tutorialWaitFrames == 0) {
            state->flagBytes[0] = state->tutorialNextState;
        }
        break;
    }
    mnuUpdateMantraTutorialPanelSprites(object);
    return finished;
}


INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293DB0);

s32 mnuFindOtherPanelSelectorForScriptId(MnuStatusResource *object, u16 id) {
    MantraMenuWork *state = &object->menu;
    const MenuPanelPositionRecord *record;
    s32 selector;
    s32 result = -1;

    for (selector = 0; selector < 3; selector++) {
        if (((state->flags >> 24) & 0xF) == selector) {
            continue;
        }
        record = mnuFindPanelPositionRecordByScriptId(selector, id);
        if ((record->flags & 1) != 0) {
            result = selector;
            break;
        }
    }
    return result;
}

/* Draw the tutorial node map while its signed-byte transition opens/closes.
 * The packed state word contains an unsigned state byte and signed flags byte.
 * Widen the signed pair before masking/decoding kind 2's high flag byte. */
void mnuUpdateMantraTutorialPanelSprites(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    s32 transitionMode = work->unk9B5;
    u8 drawMode = (u8)work->unk9B5;
    s32 fade = 0;
    f32 progress;
    s32 selector;
    s32 remaining;
    MantraNodePos *position;

    switch (transitionMode) {
    case 0: /* Hidden: no transition timer update. */
    case 3: /* Visible: no transition timer update. */
        break;
    case 1:
        work->unk9B6++;
        if (work->unk9B6 < 6) {
            break;
        }
        work->unk9B6 = 0;
        work->unk9B5 = 3;
        drawMode = 3;
        break;
    case 2:
        work->unk9B6++;
        if (work->unk9B6 < 6) {
            break;
        }
        work->unk9B6 = 0;
        work->unk9B5 = 0;
        drawMode = 0;
        break;
    }

    progress = (f32)work->unk9B6 / 5.0f;
    switch ((s8)drawMode) {
    case 0:
        return;
    case 1:
        fade = (s32)(progress * 128.0f);
        break;
    case 2:
        fade = (s32)((1.0f - progress) * 128.0f);
        break;
    case 3:
        fade = 0x80;
        break;
    }

    position = mnuGetMantraPanelPositionRecord(0);
    remaining = 0xAF;
    for (; remaining >= 0; remaining--, position++) {
        s32 id = position->selector.fields.index;
        if (id != 0) {
            MenuPanelPositionRecord *record;
            s32 modelFlagState = position->modelFlagState;
            u32 menuFlags = work->flags;
            u16 *flagWord;

            if (modelFlagState > (s32)((menuFlags >> 28) & 1)) {
                continue;
            }
            selector = (menuFlags >> 24) & 0xF;
            record = mnuFindPanelPositionRecordByScriptId(
                selector, (u16)position->selector.fields.index);
            flagWord = &record->stateFlags;
            if ((*flagWord & 0xF) != 3) {
                s32 x = (s32)((((((f32)position->x / 10.0f) * 20.0f) - 180.0f) + 241.0f) - 10.0f);
                s32 y = (s32)((((f32)position->y / 10.0f) * 20.0f) - 180.0f + 221.0f);
                switch (position->kind) {
                case 1:
                case 3:
                case 4: {
                    s32 headerFlags = (s8)(*flagWord >> 8);
                    if (headerFlags & 1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x105, 0, 0x52);
                    } else if (mnuFindOtherPanelSelectorForScriptId(
                                   object, (u16)position->id) != -1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x106, 0, 0x52);
                    } else {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x107, 0, 0x52);
                    }
                    break;
                }
                case 2: {
                    s32 high = ((s16)*flagWord & 0xFF00) >> 8;
                    if (high & 8) {
                        if (position->selector.packed & 0x100) {
                            mnuDrawMantraSprite(x, y, 0, fade, 0x108, 0, 0x52);
                        } else {
                            mnuDrawMantraSprite(x, y, 0, fade, 0x10A, 0, 0x52);
                        }
                    } else if (high & 1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x105, 0, 0x52);
                    } else {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x109, 0, 0x52);
                    }
                    break;
                }
                }
            }
        }
    }
}

extern void *memset(void *, s32, u32);
/* Allocate twenty countdown entries for sounds attached to this panel. */
void mnuInitPanelSoundEntries(void) {
    struct SdfMemBlock *allocation;
    MenuPanelEntryPool *pool;

    if (mnuPanelSoundEntryPool == 0) {
        allocation = sdfAllocGeneralBlock(0xAC);
        mnuPanelSoundEntryPool =
            (MenuPanelEntryPool *)(u32)sdfMemoryGetBlockAddress(allocation);
        memset(mnuPanelSoundEntryPool, 0, 0xAC);
        pool = mnuPanelSoundEntryPool;
        pool->entries = (MenuPanelEntry *)(pool + 1);
        pool->count = 0x14;
        pool->allocation = allocation;
    }
}

void mnuReleasePanelEntryPool(void) {
    if (mnuPanelSoundEntryPool != 0) {
        sdfReleaseResourceAllocation(mnuPanelSoundEntryPool->allocation);
    }
    mnuPanelSoundEntryPool = 0;
}

extern void sndSetSequenceVolumePan(s32, s32, s32);
void mnuTickPanelSoundEntries(void) {
    s32 i;
    MenuPanelEntry *entry = mnuPanelSoundEntryPool->entries;

    for (i = 0; i < mnuPanelSoundEntryPool->count; i++, entry++) {
        if (entry->soundHandle != 0) {
            if (entry->framesRemaining == 0) {
                sndSetSequenceVolumePan(entry->soundHandle, 0x7F, 0x3F);
                entry->soundHandle = 0;
            }
            if (entry->framesRemaining > 0) {
                entry->framesRemaining--;
            }
        }
    }
}

MenuPanelEntry *mnuFindFreePanelEntry(void) {
    MenuPanelEntryPool *pool = mnuPanelSoundEntryPool;
    s32 count = pool->count;
    MenuPanelEntry *entry = pool->entries;
    s32 index;
    for (index = 0; index < count; index++, entry++) {
        if (entry->soundHandle == 0) {
            return entry;
        }
    }
    return 0;
}

void mnuStorePanelEntry(s32 soundHandle, s32 framesRemaining) {
    MenuPanelEntry *entry = mnuFindFreePanelEntry();
    entry->soundHandle = soundHandle;
    entry->framesRemaining = framesRemaining;
}

void mnuSetCurrentTerminalContext(MenuTerminalContext *value) {
    D_00438FC8 = value;
}
INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437958);

MenuPanelEntryPool *mnuPanelSoundEntryPool = 0;

