#include "sdk_ps2.h"
#include "pcp_vu0.h"
#include "mnu_mantra.h"
#include "mnu_mantra_position_api.h"
#include "mdl_flag_api.h"

#include "common.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "prf_profile_cap.h"


extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void mnuArmMantraLimitLineFlags(struct MantraDrawPool *pool, u32);



typedef struct MantraProfileRequirement {
    u16 id;
    u16 reserved;
} MantraProfileRequirement;




extern s32 mnuGetActiveMantraModelFlagState(void);

extern const MantraProfileRequirement mnuMantraState0ProfileRequirements[16];
extern const MantraProfileRequirement mnuMantraState1ProfileRequirements[12];
extern const MantraProfileRequirement mnuMantraOtherStateProfileRequirements[18];
extern const char mnuMantraActiveStateDebugFormat[];
extern const char mnuMantraMemberEventDebugFormat[];
extern const char mnuMantraNoNeighborCandidateDebugText[];
extern const char mnuMantraProfileCandidateDebugFormat[];

extern datUnitWork_t *mnuGetNodeValueByIndex(MnuStatusResource *, s32);

extern s32 ptyAnyActivePartyMemberAtProfileCap(u16, u16);

extern u32 ptyGetProfileRecordValue(const datUnitWork_t *, u16);


struct MantraPanelPool;
struct MantraPanelAnimation;
extern u32 mnuGetSelectedMantraUnitIndex(struct MnuStatusResource *);
extern struct MantraPanelAnimation *mnuSpawnPanelSlotA(struct MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(struct MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern u32 mnuQueuePanelAnimationTransition(struct MantraPanelAnimation *, u32, s16);
extern void mnuStorePanelEntry(s32 soundHandle, s32 framesRemaining);

/* Mark the selected mantra node and start its panel animation. */
void mnuActivateMantraNode(MnuStatusResource *menu, u16 nodeId) {
    u32 selected;
    MantraFlagResource *slot;
    u16 *entry;
    struct MantraPanelAnimation *panel;

    mnuGetMantraNodePositionRecord((s16)nodeId);
    selected = mnuGetSelectedMantraUnitIndex(menu);
    slot = menu->menu.slots[selected];
    entry = slot->flags + nodeId;
    *entry = (*entry & 0xFFF0) | 1;
    panel = mnuSpawnPanelSlotA(menu->menu.resource, nodeId, 8, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(panel, 3, 0);
    mnuStorePanelEntry(0x20009, 0x14);
}

/* Set a mantra node's state for every listed unit (1 when a neighbour has
 * flag bit 8 set, otherwise 2), then play its panel animation and, when the
 * selected unit's state is 1, the second panel. `slot` is reassigned for the
 * selected unit so the pointer is allocated globally. */
void mnuUpdateMantraNodeState(MnuStatusResource *menu, u16 nodeId) {
    MantraNodePos *node;
    struct MenuListNode *unit;
    s32 index;
    struct MantraPanelAnimation *panel;
    u32 selected;
    u16 *entry;
    MantraFlagResource *slot;

    node = mnuGetMantraNodePositionRecord((s16)nodeId);
    index = 0;
    for (unit = menu->list->first; unit != NULL; unit = unit->next, index++) {
        MantraNodePos **neighbor;
        s32 i;

        slot = menu->menu.slots[index];
        entry = &slot->flags[node->id];
        *entry = (*entry & 0xFFF0) | 2;
        i = 0;
        neighbor = node->neighbors;
        do {
            MantraNodePos *next = *neighbor++;

            if (next != NULL) {
                u16 *other = &slot->flags[next->id];

                if ((u16)((*other >> 8) & 1) != 0) {
                    *entry = (*entry & 0xFFF0) | 1;
                    break;
                }
            }
        } while (++i < 6);
    }
    panel = mnuSpawnPanelSlotA(menu->menu.resource, nodeId, 10, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(panel, 1, 0);
    selected = mnuGetSelectedMantraUnitIndex(menu);
    slot = menu->menu.slots[selected];
    entry = &slot->flags[node->id];
    if ((*entry & 0xF) == 1) {
        mnuQueuePanelAnimationTransition(panel, 3, 0x28);
        panel = mnuSpawnPanelSlotA(menu->menu.resource, nodeId, 11, 100, 0, 0);
        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
        mnuQueuePanelAnimationTransition(panel, 0, 0);
        mnuStorePanelEntry(0x2000B, 0x2D);
    }
    mnuStorePanelEntry(0x2000A, 5);
}

extern const u16 D_00427218[4][8];
extern const u16 D_00427258[4][2];
extern const char D_00427268[];
s32 mnuInitMantraMapScroll(MnuStatusResource *object) {
    u16 layouts[4][8];
    u16 requirements[4][2];
    s32 enabled = 0;
    s32 i;
    s16 rankIndex;
    s16 rank;
    MantraMenuWork *work;

    memcpy(layouts, D_00427218, sizeof(layouts));
    memcpy(requirements, D_00427258, sizeof(requirements));
    for (i = 0; i < 4; i++) {
        if (evtBitCheck(requirements[i][1])) {
            enabled |= 1 << i;
        }
    }
    work = &object->menu;
    evtPrintDeveloperConsoleMessage(D_00427268, work->drawBits.drawRank);
    rankIndex = work->drawBits.drawRank == 0 ? 0 : work->drawBits.drawRank - 1;
    rank = work->drawBits.drawRank;
    switch (rank) {
    case 0:
    case 1:
        if (enabled) {
            MantraNodePos *node = work->defaultSelector;
            work->scrollX = (s32)((f32)node->x / 10.0f * 40.0f - 256.0f);
            work->scrollY = (s32)((f32)node->y / 10.0f * 39.0f - 191.0f);
        } else {
            work->scrollX = 124;
            work->scrollY = 160;
        }
        break;
    case 2:
    case 3:
        {
            MantraNodePos *node = work->defaultSelector;
            s16 nodeY;
            work->scrollX = (s32)((f32)node->x / 10.0f * 40.0f - 256.0f);
            nodeY = node->y;
            work->scrollEndX = work->scrollX + layouts[rankIndex][4];
            work->scrollY = (s32)((f32)nodeY / 10.0f * 39.0f - 191.0f);
            work->scrollEndY = work->scrollY + layouts[rankIndex][5];
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427218);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427258);

INCLUDE_RODATA(const s32, "game/code_0028E568", D_00427268);

s32 mnuAdvanceMantraSelectionCursor(MnuStatusResource *object, s32 centerOnNode) {
    s32 target[2];
    f32 movement[4] __attribute__((aligned(16)));
    s16 projected[2];
    u16 layouts[4][10] = {
        {145, 170, 118, 118, 142, 128, 240, 128, 20, 19},
        {106, 212, 98, 136, 142, 128, 240, 128, 20, 19},
        {-42, 360, -50, 368, 142, 128, 240, 128, 20, 19},
        {63, 180, 75, 160, 52, 72, 420, 300, 20, 19}
    };
    u16 requirements[4][2] = {
        {97, 0x977}, {98, 0x978}, {99, 0x979}, {100, 0x97A}
    };
    MantraMenuWork *work;
    MantraNodePos *node;
    s16 layout;
    s32 i;
    f32 distance;
    f32 step;

    work = &object->menu;
    for (i = 0; i < 4; i++) {
        evtBitCheck(requirements[i][1]);
    }
    layout = 3;
    if (work->drawBits.drawRank >= 2) {
        layout = work->drawBits.drawRank - 1;
    }
    node = work->defaultSelector;
    target[0] = work->scrollX;
    target[1] = work->scrollY;
    projected[0] = (s32)((f32)node->x / 10.0f * 40.0f);
    projected[1] = (s32)((f32)node->y / 10.0f * 39.0f);
    if (centerOnNode == 0) {
        switch (layout) {
        case 0:
        case 1:
        case 2:
            target[0] = projected[0] - 256;
            target[1] = projected[1] - 191;
            break;
        case 3:
            {
                const s16 *idField = &node->id;
                s32 nodeId;

                target[0] = 124;
                target[1] = 160;
                nodeId = *idField;
                for (i = 0; i < 4; i++) {
                    if (requirements[i][0] == nodeId) {
                        target[0] = projected[0] - 256;
                        target[1] = projected[1] - 191;
                    }
                }
            }
            break;
        }
    } else {
        target[0] = projected[0] - 256;
        target[1] = projected[1] - 191;
    }
    memset(movement, 0, sizeof(movement));
    movement[0] = target[0] - work->scrollX;
    movement[1] = target[1] - work->scrollY;
    VU0_LOAD_VF_PTR(vf10, movement);
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    step = distance - distance * 0.9f;
    if (step < 12.0f) {
        step = 12.0f;
    }
    if (distance < step) {
        step = distance;
    }
    VU0_SCALAR_OP_CLOBBER(step, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_STORE_VF_PTR(vf10, movement);
    work->scrollX += movement[0];
    work->scrollY += movement[1];
    work->scrollEndX = work->scrollX + layouts[layout][4];
    work->scrollEndY = work->scrollY + layouts[layout][5];
    if (step < distance) {
        return 1;
    }
    return 0;
}

extern MenuMantraRecord *mnuMantraPublishedSourceRecords;

/* Build the mantra source list after the status work: a header record whose
 * level comes from event flags 0x920-0x922, then one record per kind-2/4 node
 * and six per kind-3 node, each claiming the next event flag from 0x923. The
 * final store recomputes the list address so `record` stays the live cursor. */





void mnuBuildMantraSourceList(MnuStatusResource *status) {
    MenuMantraRecord *list = status->menu.records;
    MenuMantraRecord *record = list;
    MantraNodePos *node;
    s32 used;
    s32 i;

    memset(list, 0, 0x70 * sizeof(MenuMantraRecord));
    node = mnuGetMantraNodePositionRecord(0);
    list->flags |= 0x8000;
    if (evtBitCheck(0x922)) {
        list->flags = (list->flags & 0xF0FF) | 0x4300;
    } else if (evtBitCheck(0x921)) {
        list->flags = (list->flags & 0xF0FF) | 0x200;
    } else if (evtBitCheck(0x920)) {
        list->flags = (list->flags & 0xF0FF) | 0x100;
    }
    record++;
    used = 0;
    for (i = 0; i < 0xB0; i++, node++) {
        if (node->id != 0) {
            u32 kind = node->selector.packed & 0xF;

            if (kind == 2 || kind == 4) {
                record->mantraId = node->id;
                {
                    u32 packed = node->selector.packed;
                    u32 kindBits = (packed & 0xF) << 8;

                    record->flags = (record->flags & 0xE0FF) | kindBits | ((packed << 4) & 0x1000);
                }
                record->eventFlag = 0x923 + used++;
                record++;
            } else if (kind == 3) {
                s32 element;

                for (element = 0; element < 6; element++) {
                    record->mantraId = node->id;
                    {
                        u32 packed = node->selector.packed;

                        record->flags = (record->flags & 0xE0FF) | ((packed & 0xF) << 8) | ((packed << 4) & 0x1000);
                    }
                    record->element = element;
                    record->eventFlag = 0x923 + used++;
                    record++;
                }
            }
        }
        if (used >= 0x70) {
            break;
        }
    }
    evtPrintDeveloperConsoleMessage("Used EventFlag [0x%x] - [0x%x]\n", 0x920, 0x923 + used);
    mnuMantraPublishedSourceRecords = status->menu.records;
}

extern const u16 mnuMantraMemberFlagBasePairs[14][2];
extern const u16 mnuMantraNode97To100FlagPairs[4][2];
extern const u16 mnuMantraNode113To120FlagPairs[8][2];
extern const u16 mnuMantraNode145To152FlagPairs[8][2];
extern const s8 mnuMantraMemberFlagOffsets[9];

s32 mnuIsMantraNodeModelFlagSetForMember(u16 nodeId, s8 member) {
    u16 memberFlags[14][2];
    u16 flagsA[4][2];
    u16 flagsB[8][2];
    u16 flagsC[8][2];
    s8 memberOffsets[9];
    u32 i;

    memcpy(memberFlags, mnuMantraMemberFlagBasePairs, sizeof(memberFlags));
    memcpy(flagsA, mnuMantraNode97To100FlagPairs, sizeof(flagsA));
    memcpy(flagsB, mnuMantraNode113To120FlagPairs, sizeof(flagsB));
    memcpy(flagsC, mnuMantraNode145To152FlagPairs, sizeof(flagsC));
    memcpy(memberOffsets, mnuMantraMemberFlagOffsets, sizeof(memberOffsets));
    for (i = 0; i < 14; i++) {
        if (memberFlags[i][0] == nodeId) {
            return evtBitCheck(memberFlags[i][1] + memberOffsets[member]);
        }
    }
    for (i = 0; i < 4; i++) {
        if (flagsA[i][0] == nodeId) {
            return evtBitCheck(flagsA[i][1]);
        }
    }
    for (i = 0; i < 8; i++) {
        if (flagsB[i][0] == nodeId) {
            return evtBitCheck(flagsB[i][1]);
        }
    }
    for (i = 0; i < 8; i++) {
        if (flagsC[i][0] == nodeId) {
            return evtBitCheck(flagsC[i][1]);
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
    datUnitWork_t *party;
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
    evtPrintDeveloperConsoleMessage(mnuMantraActiveStateDebugFormat, modelFlagState);

    for (; slotIndex < 5; slotIndex++, slot++) {
        if (slot->nodeId != 0) {
            entryIndex = 0;
            record = mnuGetMantraNodePositionRecord(slot->nodeId);
            for (; entryIndex < 6; entryIndex++) {
                entry = record->neighbors[entryIndex];
                if (entry != 0 && modelFlagState < entry->modelFlagState) {
                    evtPrintDeveloperConsoleMessage(mnuMantraMemberEventDebugFormat, slotIndex);
                    state->fields.unitIndex = record->id;
                    state->selectedIndex = slotIndex;
                    return 1;
                }
            }
        }
    }

    evtPrintDeveloperConsoleMessage(mnuMantraNoNeighborCandidateDebugText);
    if (modelFlagState == 0) {
        requirements = mnuMantraState0ProfileRequirements;
        requirementCount = 16;
    } else if (modelFlagState == 1) {
        requirements = mnuMantraState1ProfileRequirements;
        requirementCount = 12;
    } else {
        requirements = mnuMantraOtherStateProfileRequirements;
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
                    evtPrintDeveloperConsoleMessage(mnuMantraProfileCandidateDebugFormat, slotIndex,
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
    datUnitWork_t *value;
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
    s32 listIndex = 0;
    while (current != 0) {
        if (current->partyRecord->unitId == state->targetId) {
            state->selectedIndex = listIndex;
            return 0;
        }
        current = current->next;
        listIndex++;
    }
    return 0;
}

const u16 mnuMantraMemberFlagBasePairs[14][2] __attribute__((aligned(8))) = {
    { 7, 0x923 }, { 14, 0x929 }, { 21, 0x92F }, { 28, 0x935 },
    { 35, 0x93B }, { 59, 0x941 }, { 64, 0x947 }, { 68, 0x94B },
    { 83, 0x953 }, { 84, 0x959 }, { 90, 0x95F }, { 91, 0x965 },
    { 92, 0x96B }, { 94, 0x971 }
};

const u16 mnuMantraNode97To100FlagPairs[4][2] __attribute__((aligned(8))) = {
    { 97, 0x977 }, { 98, 0x978 }, { 99, 0x979 }, { 100, 0x97A }
};

const u16 mnuMantraNode113To120FlagPairs[8][2] __attribute__((aligned(8))) = {
    { 113, 0x97B }, { 114, 0x97C }, { 115, 0x97D }, { 116, 0x97E },
    { 117, 0x97F }, { 118, 0x980 }, { 119, 0x981 }, { 120, 0x982 }
};

const u16 mnuMantraNode145To152FlagPairs[8][2] __attribute__((aligned(8))) = {
    { 145, 0x983 }, { 146, 0x984 }, { 147, 0x985 }, { 148, 0x986 },
    { 149, 0x987 }, { 150, 0x988 }, { 151, 0x989 }, { 152, 0x98A }
};

const s8 mnuMantraMemberFlagOffsets[9] __attribute__((aligned(8))) = { 0, 0, 1, 5, 2, 3, 4, 5, 1 };

const char mnuMantraActiveStateDebugFormat[] __attribute__((aligned(8))) = "mtrChkMantraEventTarget_DrawRankUp!![%d]\n";

const char mnuMantraMemberEventDebugFormat[] __attribute__((aligned(8))) = "EventFlag PlayerIndex[%d]\n";

const char mnuMantraNoNeighborCandidateDebugText[] __attribute__((aligned(8))) = "None...\n\n";

const char mnuMantraProfileCandidateDebugFormat[] __attribute__((aligned(8))) = "EventFlag PlayerIndex[%d:0x%02x] MantraID[0x%02x]\n";

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



extern u32 mnuGetSelectedMantraUnitIndex(struct MnuStatusResource *);

extern void mnuQueueMantraLimitLineFlags(struct MantraDrawPool *pool, u32 flags);

extern void mnuSetMantraBackgroundSelection(struct MantraDrawPool *pool, u32 value);

void mnuUpdateSelectedMantraLimitLineState(MnuStatusResource *object) {
    u16 limitIds[18] = {
        0x61, 0x3B, 0x5B, 0x23, 0x40, 0x62,
        0x53, 0x5E, 0x1C, 0x5A, 0x63, 0x54,
        0x15, 0x44, 0x64, 0x07, 0x5C, 0x0E
    };
    MnuStatusResource *menu = object;
    u32 selected = mnuGetSelectedMantraUnitIndex(object);
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

extern s32 prfIsMantraCategoryEnabled(u32 categoryIndex);
extern s32 evtTestMantraNodePartyRequirements(u16, s32);
extern s32 prfReqEvaluateRules(u32, u32, datUnitWork_t *, u32, u32 *);
extern s32 prfReqCheckWithFallback(datUnitWork_t *, u16);

s32 mnuScanMantraSourceCandidates(MnuStatusResource *object) {
    s8 memberOffsets[9] = { 0, 0, 1, 5, 2, 3, 4, 5, 1 };
    MenuMantraRecord *record;
    struct MenuListNode *node;
    datUnitWork_t *selectedParty;
    datUnitWork_t **selectedPartySlot;
    datUnitWork_t *party;
    s32 changed = 0;
    s32 rank;
    s32 modelRank;
    s32 remaining;

    selectedPartySlot = &object->list->cursor->partyRecord;
    selectedParty = *selectedPartySlot;
    record = object->menu.records;
    if (!record->fields.announced) {
        for (rank = 0; prfIsMantraCategoryEnabled(rank); rank++) {}
        rank--;
        modelRank = mnuGetActiveMantraModelFlagState();
        if (modelRank < rank) changed = 1;
        evtPrintDeveloperConsoleMessage("DrawRank!!!!!!!!!!!!!!!!!!![%d][%d]\n", modelRank, rank);
        if (rank < modelRank) rank = modelRank;
        object->menu.drawBits.drawRank = rank;
        if (record->fields.kind < rank) {
            record->fields.kind = rank;
            record->fields.queued = 1;
            if ((record->flags & 0xF00) == 0x100) {
                record->eventFlag = 0x920;
                object->menu.drawBits.drawRank = 1;
            } else if ((record->flags & 0xF00) == 0x200) {
                record->eventFlag = 0x921;
                object->menu.drawBits.drawRank = 2;
            } else if ((record->flags & 0xF00) == 0x300) {
                record->eventFlag = 0x922;
                record->fields.announced = 1;
                object->menu.drawBits.drawRank = 3;
            }
        }
    } else {
        object->menu.drawBits.drawRank = 3;
        object->menu.drawBits.showOverlay = 1;
    }
    record++;
    for (remaining = 110; remaining >= 0; remaining--, record++) {
        if (record->mantraId != 0 && !record->fields.announced) {
            switch (record->fields.kind) {
            case 2:
                if (!evtBitCheck(record->eventFlag)) {
                    if (evtTestMantraNodePartyRequirements(record->mantraId, 0)) {
                        record->fields.queued = 1;
                        changed = 1;
                        evtPrintDeveloperConsoleMessage("Event Mantra Hiding[%d]!!!!!!!!!!\n", record->mantraId);
                    }
                }
                break;
            case 3:
                if (!evtBitCheck(record->eventFlag)) {
                    for (node = object->list->first; node != NULL; node = node->next) {
                        party = node->partyRecord;
                        if (record->element == memberOffsets[party->unitId]) break;
                    }
                    if (node != NULL && prfReqEvaluateRules(0, 4, party, record->mantraId, NULL)) {
                        record->fields.queued = 1;
                        changed = 1;
                        record->targetId = party->unitId;
                        evtPrintDeveloperConsoleMessage("Event Mantra High Rank[%d]!!!!!!!!!!\n", record->mantraId);
                    }
                }
                break;
            case 4:
                if (!evtBitCheck(record->eventFlag)) {
                    if (prfReqCheckWithFallback(selectedParty, record->mantraId)) {
                        record->fields.queued = 1;
                        changed = 1;
                        evtPrintDeveloperConsoleMessage("Event Mantra Rare[%d]!!!!!!!!!!\n", record->mantraId);
                    }
                }
                break;
            }
        }
    }
    return changed;
}

u32 func_0028FD08(void) {
    return 0;
}
