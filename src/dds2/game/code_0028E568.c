#include "mnu_mantra.h"
#include "mdl_flag_api.h"

#include "common.h"
#include "dat_state.h"
#include "mnu_list.h"


extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void mnuArmMantraLimitLineFlags(struct MantraDrawPool *pool, u32);



typedef struct MantraProfileRequirement {
    u16 id;
    u16 reserved;
} MantraProfileRequirement;




extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern s32 mnuGetActiveMantraModelFlagState(void);

extern const MantraProfileRequirement D_003D0078[16];
extern const MantraProfileRequirement D_003D00B8[12];
extern const MantraProfileRequirement D_003D00E8[18];
extern const char D_00427390[];
extern const char D_004273C0[];
extern const char D_004273E0[];
extern const char D_004273F0[];

extern DatPartyRecord *mnuGetNodeValueByIndex(MnuStatusResource *, s32);

extern s32 ptyAnyActivePartyMemberAtProfileCap(u16, u16);

extern u32 ptyGetProfileRecordCap(u16);

extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);


struct MantraPanelPool;
struct MantraPanelAnimation;
extern u32 func_002890A8(struct MnuStatusResource *);
extern struct MantraPanelAnimation *mnuSpawnPanelSlotA(struct MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(struct MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern u32 mnuQueuePanelAnimationTransition(struct MantraPanelAnimation *, u32, s16);
extern void mnuStorePanelEntry(s32, s32);

/* Mark the selected mantra node and start its panel animation. */
void mnuActivateMantraNode(MnuStatusResource *menu, u16 nodeId) {
    u32 selected;
    MantraFlagResource *slot;
    u16 *entry;
    struct MantraPanelAnimation *panel;

    mnuGetMantraNodePositionRecord((s16)nodeId);
    selected = func_002890A8(menu);
    slot = menu->menu.slots[selected];
    entry = slot->flags + nodeId;
    *entry = (*entry & 0xFFF0) | 1;
    panel = mnuSpawnPanelSlotA(menu->menu.resource, nodeId, 8, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(panel, 3, 0);
    mnuStorePanelEntry(0x20009, 0x14);
}

INCLUDE_ASM(const s32, "game/code_0028E568", mnuUpdateMantraNodeState);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427258);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E568", func_0028EB38);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427278);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004272C8);

INCLUDE_ASM(const s32, "game/code_0028E568", mnuBuildMantraSourceList);

extern void *memcpy(void *, const void *, u32);
extern const u16 D_004272F8[14][2];
extern const u16 D_00427330[4][2];
extern const u16 D_00427340[8][2];
extern const u16 D_00427360[8][2];
extern const s8 D_00427380[9];

s32 mnuIsMantraNodeModelFlagSetForMember(u16 nodeId, s8 member) {
    u16 memberFlags[14][2];
    u16 flagsA[4][2];
    u16 flagsB[8][2];
    u16 flagsC[8][2];
    s8 memberOffsets[9];
    u32 i;

    memcpy(memberFlags, D_004272F8, sizeof(memberFlags));
    memcpy(flagsA, D_00427330, sizeof(flagsA));
    memcpy(flagsB, D_00427340, sizeof(flagsB));
    memcpy(flagsC, D_00427360, sizeof(flagsC));
    memcpy(memberOffsets, D_00427380, sizeof(memberOffsets));
    for (i = 0; i < 14; i++) {
        if (memberFlags[i][0] == nodeId) {
            return mdlFlagTest(memberFlags[i][1] + memberOffsets[member]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (flagsA[i][0] == nodeId) {
            return mdlFlagTest(flagsA[i][1]);
        }
    }
    for (i = 0; i < 8; i++) {
        if (flagsB[i][0] == nodeId) {
            return mdlFlagTest(flagsB[i][1]);
        }
    }
    for (i = 0; i < 8; i++) {
        if (flagsC[i][0] == nodeId) {
            return mdlFlagTest(flagsC[i][1]);
        }
    }
    return 0;
}

/* Select a menu row whose mantra profile cannot advance at the current model
 * state; if none qualifies, search party profiles against the active rank set. */
s32 mnuFindMantraSourceCandidate(MnuStatusResource *object, MantraMenuSrc *state) {
    MantraNodePos *record;
    MantraNodePos *entry;
    MtrUnitMenuEntry *slot;
    struct MenuListNode *node;
    DatPartyRecord *party;
    const MantraProfileRequirement *requirements;
    u32 value;
    u32 cap;
    s32 modelFlagState;
    s32 slotIndex;
    s32 entryIndex;
    s32 requirementCount;

    slotIndex = 0;
    slot = &object->menu.unitEntries[0];
    modelFlagState = mnuGetActiveMantraModelFlagState();
    evtPrintDeveloperConsoleMessage(D_00427390, modelFlagState);

    for (; slotIndex < 5; slotIndex++, slot++) {
        if (slot->nodeId != 0) {
            entryIndex = 0;
            record = mnuGetMantraNodePositionRecord(slot->nodeId);
            for (; entryIndex < 6; entryIndex++) {
                entry = record->neighbors[entryIndex];
                if (entry != 0 && modelFlagState < entry->modelFlagState) {
                    evtPrintDeveloperConsoleMessage(D_004273C0, slotIndex);
                    state->fields.unitIndex = record->id;
                    state->selectedIndex = slotIndex;
                    return 1;
                }
            }
        }
    }

    evtPrintDeveloperConsoleMessage(D_004273E0);
    if (modelFlagState == 0) {
        requirements = D_003D0078;
        requirementCount = 16;
    } else if (modelFlagState == 1) {
        requirements = D_003D00B8;
        requirementCount = 12;
    } else {
        requirements = D_003D00E8;
        requirementCount = 18;
    }

    node = object->list->first;
    slotIndex = 0;
    for (; node != 0; slotIndex++, node = node->next) {
        party = node->partyRecord;
        for (entryIndex = 0; entryIndex < requirementCount; entryIndex++) {
            u16 id = requirements[entryIndex].id;

            if (id != 0) {
                value = ptyGetProfileRecordValue(party, id);
                cap = ptyGetProfileRecordCap(id);
                if (value == cap) {
                    evtPrintDeveloperConsoleMessage(D_004273F0, slotIndex,
                                                    party->unitId, id);
                    state->fields.unitIndex = id;
                    state->selectedIndex = slotIndex;
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 mnuSelectPreferredMantraNode(MnuStatusResource *object, MantraMenuSrc *state) {
    MantraMenuWork *work = &object->menu;
    MtrUnitMenuEntry *slot;
    MantraNodePos *record;
    MantraNodePos *entry;
    MantraNodePos **entries;
    struct MenuListNode *node;
    DatPartyRecord *value;
    s32 i, j;
    s32 cap;

    for (i = 0, slot = work->unitEntries; i < 5; i++, slot++) {
        if (slot->nodeId != 0) {
            record = mnuGetMantraNodePositionRecord(slot->nodeId);
            value = mnuGetNodeValueByIndex(object, i);
            if (ptyAnyActivePartyMemberAtProfileCap(record->id, value->unitId) == 0) {
                for (j = 0; j < 6; j++) {
                    entry = record->neighbors[j];
                    if (entry != 0 && entry->kind == 2 && entry->id == state->fields.unitIndex) {
                        state->selectedIndex = i;
                        return 1;
                    }
                }
            }
        }
    }
    for (i = 0, slot = work->unitEntries; i < 5; i++, slot++) {
        if (slot->nodeId != 0) {
            record = mnuGetMantraNodePositionRecord(slot->nodeId);
            mnuGetNodeValueByIndex(object, i);
            for (j = 0; j < 6; j++) {
                entry = record->neighbors[j];
                if (entry != 0 && entry->kind == 2 && entry->id == state->fields.unitIndex) {
                    state->selectedIndex = i;
                    return 1;
                }
            }
        }
    }
    node = object->list->first;
    for (i = 0; i < 5; i++, node = node->next) {
        value = node->partyRecord;
        record = mnuGetMantraNodePositionRecord(state->fields.unitIndex);
        for (j = 0, entries = record->neighbors; j < 6; j++, entries++) {
            if (*entries != 0) {
                cap = ptyGetProfileRecordCap((*entries)->id);
                if (cap == ptyGetProfileRecordValue(value, (*entries)->id)) {
                    state->selectedIndex = i;
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Select the first list item whose ID matches the requested ID.
 * If absent, retain the previous selection; the return value is always zero. */
s32 mnuSelectMatchingNode(MnuStatusResource *object, MantraMenuSrc *state) {
    struct MenuListNode *current = object->list->first;
    s32 index = 0;
    while (current != 0) {
        if (current->partyRecord->unitId == state->targetId) {
            state->selectedIndex = index;
            return 0;
        }
        current = current->next;
        index++;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004272F8);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427380);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427390);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273C0);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273E0);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004273F0);

void mnuSelectMantraLimitLine(MnuStatusResource *object, u16 id) {
    u32 flags = 0;
    u16 limitIds[18] = {
        0x61, 0x3B, 0x5B, 0x23, 0x40, 0x62,
        0x53, 0x5E, 0x1C, 0x5A, 0x63, 0x54,
        0x15, 0x44, 0x64, 0x07, 0x5C, 0x0E
    };
    s32 i;

    for (i = 0; i < 18; i++) {
        if (limitIds[i] == id) {
            flags = 1U << i;
            break;
        }
    }
    evtPrintDeveloperConsoleMessage(
        "-----------------------LimitLineSetting!!!!!!!!![%x]\n", flags);
    if (flags != 0) {
        mnuArmMantraLimitLineFlags(object->menu.selectionController, flags);
    }
}



extern u32 func_002890A8(struct MnuStatusResource *);

extern void mnuQueueMantraLimitLineFlags(struct MantraDrawPool *pool, u32 flags);

extern void mnuSetMantraBackgroundSelection(struct MantraDrawPool *pool, u32 value);

void func_0028F8A8(MnuStatusResource *object) {
    u16 limitIds[18] = {
        0x61, 0x3B, 0x5B, 0x23, 0x40, 0x62,
        0x53, 0x5E, 0x1C, 0x5A, 0x63, 0x54,
        0x15, 0x44, 0x64, 0x07, 0x5C, 0x0E
    };
    MnuStatusResource *menu = object;
    u32 selected = func_002890A8(object);
    MantraFlagResource *slot = menu->menu.slots[selected];
    u32 flags = 0;
    s32 i;

    for (i = 0; i < 18; i++) {
        u16 *value = slot->flags + limitIds[i];

        if ((*value & 0xF) != 3) {
            flags |= 1U << i;
        }
    }

    mnuQueueMantraLimitLineFlags(menu->menu.selectionController, flags);
    mnuSetMantraBackgroundSelection(menu->menu.selectionController, flags);
}

INCLUDE_RODATA(const s32, "game/code_0028E568", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E568", mnuScanMantraSourceCandidates);

u32 func_0028FD08(void) {
    return 0;
}
