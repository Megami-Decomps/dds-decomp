#include "mnu_mantra.h"
#include "common.h"
#include "mnu.h"
#include "mnu_list.h"

extern s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *);
extern s32 evtGetMessageWindowControlState(void);
extern void evtFinishMessageWindowAndNotify(void);
extern s32 dspStartEntry(s32);
extern s32 dspCloseChannel(void);
extern void mnuDisableMantraBackground(u32);
extern void mnuEnableMantraBackground(u32);
extern void mnuToggleMantraTitleVariant(u32);
extern void mnuBeginMantraUnitPanelExit(u32);
extern u32 mnuRegisterMantraUnitPanelDraw(u32, u32);
extern u32 mnuGetSelectedNodeValue(MnuStatusResource *);
extern s32 mnuMoveNodeCursorToTargetIndex(MnuStatusResource *, s8);
extern void func_0028D070(MnuStatusResource *, s32, s32);
extern void func_00291590(MnuStatusResource *, s16, s32, s8, s8, s32);
extern void func_002932B0(MnuStatusResource *);
extern void func_00294060(MnuStatusResource *);


typedef struct MenuPanelTransition {
    u16 frame;
    u16 panelId;
    u16 mode;
    u16 action;
} MenuPanelTransition;

extern s32 func_00292478(void *, s32, s32);


extern u32 *mnuPanelSoundEntryPool;

typedef struct MantraPanelPool MantraPanelPool;

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
    u32 spriteHandle;
    u32 burstPool;
    s16 id;
    s16 transitionDelay;
} MantraPanelAnimation;

extern u32 mnuQueuePanelAnimationTransition(MantraPanelAnimation *, u32, s16);
extern void mnuOffsetPanelAndSetVisualParams(MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern void func_00278FA8(s32);
extern void func_00279148(s32);
extern void mnuStorePanelEntry(s32, s32);

extern MantraPanelAnimation *mnuSpawnPanelSlotB(MantraPanelPool *, s32, s8, s16, s16, u32);

extern MantraPanelAnimation *mnuFindPanelSlotById(MantraPanelPool *, s32, s8);


extern u32 func_002890A8(MnuStatusResource *);





/* The selector also carries word-wide flags in its packed representation. */



typedef struct MenuPanelEntry {
    s32 soundHandle;
    s32 framesRemaining;
} MenuPanelEntry;

typedef struct MenuPanelEntryPool {
    u32 allocation;
    MenuPanelEntry *entries;
    s32 count;
} MenuPanelEntryPool;

/* The eight-byte acquisition record uses its low byte as the mantra id. */





extern void func_00291338(void);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427560);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0028FD30", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004275E8);

s32 func_00290240(MnuStatusResource *object, s8 direction) {
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


s32 func_00290328(MnuStatusResource *object, MantraNodePos *list, s8 position) {
    u16 selected;
    MantraNodePos *entry;
    u16 value;
    s32 result;

    if (list == NULL || position == -1) {
        return -1;
    }
    selected = func_002890A8((MnuStatusResource *)object);
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
extern s32 func_00290240(MnuStatusResource *, s8);
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

    selected = (u16)func_002890A8((MnuStatusResource *)object);
    if (state->navigationState == 0 || state->navigationState == 7) {
        node = state->defaultSelector;
        position = func_00290240(object, flags);
        for (;;) {
            result = func_00290328(object, node, position);
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

u32 mnuGetDefaultPanelSelector(MnuStatusResource *object) {
    return (u32)object->menu.defaultSelector;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290A78);

extern void func_0026D168(void *, s32, s32);
extern void *evtAllocateMantraSelectionWork(s32, s32);

s32 mnuUpdateSelectedPanelSlot(MnuStatusResource *object) {
    s32 index;
    s32 source;

    index = func_002890A8((MnuStatusResource *)object);
    source = object->list->cursor->unk70;
    if (object->menu.slots[index] != 0) {
        func_0026D168(object->menu.slots[index], source, 0);
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

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00290C20);


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

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291038);

extern void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags);
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

/* Write each panel slot's 0xB0 saved flag words back to its list node's script entries and log the slot number. */
INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427668);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427680);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276A0);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_004276C0);

void mnuStoreMantraPanelFlagsToScript(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        u32 context = node->unk70;
        u16 *values = object->menu.slots[slotIndex]->flags;
        s32 i;

        for (i = 0; i < 0xB0; i++) {
            scrSetEntryLowFlags(context, i, *values++);
        }
        evtPrintDeveloperConsoleMessage("[%d]Mantra Data Saved!!!!!!!!!!!!!!!!!\n", slotIndex++);
    }
}

extern u32 scrGetEntryLowFlags(s32 arg0, u16 index);

/* Read each list node's 0xB0 script entry flags into its panel slot and log the slot number. */
void mnuLoadMantraPanelFlagsFromScript(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 slotIndex = 0;

    for (; node != NULL; node = node->next) {
        s32 context = node->unk70;
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
extern u32 ptyGetProfileRecordValue(u32 work, u16 scriptId);

s32 mnuValidateProfileEntry(MantraFlagResource *slot, s32 arg1) {
    u16 target = scrGetSelectedScriptEntryId((DatPartyRecord *)arg1) & 0xFFFF;
    u16 *dst = slot->flags;
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        u32 flags = scrGetEntryLowFlags(arg1, i);

        *dst++ = flags;
        if (i == target) {
            if (((flags >> 8) & 1) != 0) {
                target = 0;
            }
        }
    }
    if (target != 0 && ptyGetProfileRecordCap(target) == ptyGetProfileRecordValue(arg1, target)) {
        return target;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291338);

/* Updated as a halfword here, and read as signed high-byte flags elsewhere. */
typedef struct MenuPanelPositionRecord {
    u16 id;
    union {
        u16 stateFlags;
        struct {
            u8 state;
            s8 flags;
        };
    };
} MenuPanelPositionRecord;

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291400);
extern MenuPanelPositionRecord *func_00291400(s32 selector, u16 id);


void mnuActivatePanelSelection(MnuStatusResource *object, s8 selection) {
    MantraMenuWork *state = &object->menu;
    if (mnuQueueUnitPanelSelection(object->menu.selectionController, selection) != 0) {
        u32 flags;
        u32 option;
        mnuMoveNodeCursorToTargetIndex(object, selection);
        mnuTransitionActivePanelAnimations(state->resource, 0);
        flags = state->flags;
        func_00291590(object, 5, 1, selection, (flags >> 28) & 1, 0);
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

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291590);

extern MantraNodePos *mnuGetMantraPanelPositionRecord(s16);

void func_002917C0(MnuStatusResource *object, s32 selector, u16 id) {
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
    record = func_00291400(selector, id);
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
                record = func_00291400(selector, (*neighbor)->selector.fields.index);
                if ((record->stateFlags & 15) == 2) {
                    changed |= 2;
                    animation = mnuFindPanelSlotById(resource, (*neighbor)->selector.fields.index, 0);
                    mnuQueuePanelAnimationTransition(animation, 1, 10);
                    animation = mnuSpawnPanelSlotB(resource, (*neighbor)->selector.fields.index, 1, 50, 0, 0);
                    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 128, 83, 0, 0);
                    record->stateFlags = (record->stateFlags & 0xFFF0) | 1;
                }
            }
        }
    }
    func_00278FA8(object->menu.selectionController);
    func_00279148(object->menu.selectionController);
    if (changed & 1) {
        mnuStorePanelEntry(0x20004, 5);
    }
    if (changed & 2) {
        mnuStorePanelEntry(0x20005, 38);
    }
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291A20);


INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291C68);
INCLUDE_ASM(const s32, "game/code_0028FD30", func_00291DD0);

void itfClearSelectionFlags(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;
    state->flags &= 0xff0000ff;
}

extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, u32);
extern void mnuSpawnMantraIconAtPosition(s32, s32, u32);
extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, u32);
extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, u32);
extern void func_00278F60(u32);
extern void func_00278EA8(u32);
extern void func_00278EE0(u32);
extern void func_00279080(u32);
extern void func_002790B8(u32);
extern void func_002790F0(s32, s32, u32);
extern void func_00291C68(MnuStatusResource *, s32, u16);
extern void func_00291A20(MnuStatusResource *, s32, u16, s32);
extern void func_00291DD0(MnuStatusResource *, s32);
extern void mnuHideMantraInfo(u32);
extern void mnuShowMantraInfo(u32);
extern void mnuHideMantraTitle(u32);
extern void mnuShowMantraTitle(u32);
extern void mnuSetMantraBackgroundVariant(u32, s8);
extern void mnuBeginMantraBackgroundMaskFadeOut(u32);
extern void mnuBeginMantraBackgroundMaskFadeIn(u32);
extern u32 mnuRegisterMantraIconListCDraw(u32, u32);
extern void mnuBeginMantraIconListExit(u32);
extern void sndSetSequenceVolumePan(s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028FD30", D_00427740);

s32 func_00292478(void *context, s32 panelId, s32 action) {
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
        func_00278F60(object->menu.selectionController);
        func_002790F0((s32)((f32)position->x / 10.0f * 40.0f),
                     (s32)((f32)position->y / 10.0f * 39.0f),
                     object->menu.selectionController);
        return 0;
    case 2:
        func_00278EA8(object->menu.selectionController);
        func_00279080(object->menu.selectionController);
        return 0;
    case 3:
        func_00278EE0(object->menu.selectionController);
        func_002790B8(object->menu.selectionController);
        return 0;
    case 4:
        func_002917C0(object, state->flagBytes[3] & 15, id);
        return 0;
    case 5:
        func_00291C68(object, state->flagBytes[3] & 15, id);
        return 0;
    case 6:
        mnuActivatePanelSelection(object, state->flagBytes[3] & 15);
        goto play_selection_sound;
    case 7:
        mnuTransitionActivePanelAnimations(state->resource, 1);
        func_00291A20(object, state->flagBytes[3] & 15, id, 5);
        return 0;
    case 8:
        mnuTransitionActivePanelAnimations(state->resource, 1);
        flags = state->flags;
        func_00291590(object, 5, 1, (flags >> 24) & 15,
                     (flags >> 28) & 1, 0);
        return 0;
    case 9:
        mnuHideMantraInfo(object->menu.selectionController);
        mnuHideMantraTitle(object->menu.selectionController);
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 3);
        mnuBeginMantraBackgroundMaskFadeOut(object->menu.selectionController);
        func_00278EE0(object->menu.selectionController);
        func_002790B8(object->menu.selectionController);
        mnuTransitionActivePanelAnimations(state->resource, 1);
        {
            u32 drawFlags = state->drawFlags;
            u32 controller = object->menu.selectionController;

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
        func_00278EA8(object->menu.selectionController);
        func_00279080(object->menu.selectionController);
        flags = state->flags;
        func_00291590(object, 5, 1, (flags >> 24) & 15,
                     (flags >> 28) & 1, 0);
        {
            u32 drawFlags = state->drawFlags;
            u32 controller = object->menu.selectionController;

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
        func_00291DD0(object, 0);
        return 0;
    case 15:
        return 1;
    default:
        return 0;
    }
    return 0;
}

void func_00292998(MnuStatusResource *object) {
    MantraPanelPool *resource;
    MenuPanelPositionRecord *record;
    MantraPanelAnimation *animation;

    resource = object->menu.resource;
    mnuGetMantraPanelPositionRecord(0);
    record = func_00291400(0, 8);
    animation = mnuFindPanelSlotById(resource, 8, 0);
    mnuQueuePanelAnimationTransition(animation, 7, 0);
    animation = mnuSpawnPanelSlotB(resource, 8, 1, 0, 0, 0);
    mnuOffsetPanelAndSetVisualParams(animation, 0, 0, 0, 0x80, 0x53, 0, 0);
    mnuQueuePanelAnimationTransition(animation, 8, 0);
    record->stateFlags = (record->stateFlags & 0xfff0) | 1;
}

extern MantraNodePos *mnuGetMantraPanelPositionRecord(s16);
extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, u32);
extern void mnuSpawnMantraIconAtPosition(s32, s32, u32);
extern void func_00278EA8(u32);
extern void func_00279080(u32);

void func_00292A60(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;

    state->defaultSelector = mnuGetMantraPanelPositionRecord(8);
    mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->menu.selectionController);
    mnuSpawnMantraIconAtPosition((s32)((f32)state->defaultSelector->x / 10.0f * 40.0f),
        (s32)((f32)state->defaultSelector->y / 10.0f * 39.0f), object->menu.selectionController);
    func_00278EA8(object->menu.selectionController);
    func_00279080(object->menu.selectionController);
    state->flags &= ~0x20000000;
}

void func_00292B90(MnuStatusResource *object) {
    func_002917C0(object, 0, 8);
}


extern void func_00278F60(u32);
extern void func_002790F0(s32, s32, u32);
void itfPositionMantraSelectionController(MnuStatusResource *object) {
    MantraNodePos *record;

    record = mnuGetMantraPanelPositionRecord(8);
    func_00278F60(object->menu.selectionController);
    func_002790F0((s32)((f32)record->x / 10.0f * 40.0f), (s32)((f32)record->y / 10.0f * 39.0f), object->menu.selectionController);
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

s32 func_00292CF0(MnuStatusResource *object) {
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
            if (func_00292478(object, steps[i].panelId, steps[i].action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}



s32 func_00292EA8(MnuStatusResource *object) {
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
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 func_00292FF0(MnuStatusResource *object) {
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
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

s32 func_00293148(MnuStatusResource *object) {
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
            if (func_00292478(object, step->panelId, step->action)) {
                return 1;
            }
        }
    }
    state->flags = (state->flags & 0xFF0000FF) | ((u16)((state->flags >> 8) + 1) << 8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_002932B0);

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

void func_002933A8(MnuStatusResource *object) {
    object->menu.flagBytes[0] = 3;
    object->menu.unk9B5 = 0;
    object->menu.unk9B6 = 0;
    object->menu.flags = (object->menu.flags & 0xF0FFFFFF) | 0x20000000;
    func_00291338();
}



s32 func_002933F0(MnuStatusResource *object) {
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
        func_00278EE0(object->menu.selectionController);
        func_002790B8(object->menu.selectionController);
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
        func_0028D070(object, 2, 0);
        position = mnuGetMantraNodePositionRecord(scrGetSelectedScriptEntryId((DatPartyRecord *)mnuGetSelectedNodeValue(object)));
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
        func_00278EE0(object->menu.selectionController);
        func_002790B8(object->menu.selectionController);
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
        func_00291590(object, 0, 0, 0, 0, 0);
        state->tutorialNextState = 7;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_MODE_CHANGE_TUTORIAL_IN ...->MTR_MSL_TUTORIAL_000_RUN\n");
        break;
    case 5:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_INIT\n");
        func_002932B0(object);
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
        func_00292998(object);
        state->tutorialNextState = 11;
        state->tutorialWaitFrames = 30;
        state->flagBytes[0] = 37;
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN ...->MTR_MSL_TUTORIAL_001_RUN2\n");
        break;
    case 11:
        evtPrintDeveloperConsoleMessage("MTR_MSL_TUTORIAL_001_RUN2\n");
        func_00292A60(object);
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
        func_00292B90(object);
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
        if (func_00292CF0(object)) {
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
        if (func_00292EA8(object)) {
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
        if (func_00292FF0(object)) {
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
        if (func_00293148(object)) {
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
    func_00294060(object);
    return finished;
}


INCLUDE_ASM(const s32, "game/code_0028FD30", func_00293DB0);

s32 func_00293FD0(MnuStatusResource *object, u16 id) {
    MantraMenuWork *state = &object->menu;
    const MenuPanelPositionRecord *record;
    s32 selector;
    s32 result = -1;

    for (selector = 0; selector < 3; selector++) {
        if (((state->flags >> 24) & 0xF) == selector) {
            continue;
        }
        record = func_00291400(selector, id);
        if ((record->flags & 1) != 0) {
            result = selector;
            break;
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0028FD30", func_00294060);

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
/* Allocate twenty countdown entries for sounds attached to this panel. */
void mnuInitPanelSoundEntries(void) {
    s32 handle;
    MenuPanelEntryPool *pool;

    if (mnuPanelSoundEntryPool == 0) {
        handle = sdfAllocGeneralBlock(0xAC);
        mnuPanelSoundEntryPool = sdfMemoryGetBlockAddress(handle);
        memset(mnuPanelSoundEntryPool, 0, 0xAC);
        pool = (MenuPanelEntryPool *)mnuPanelSoundEntryPool;
        pool->entries = (MenuPanelEntry *)(pool + 1);
        pool->count = 0x14;
        pool->allocation = handle;
    }
}

void mnuReleasePanelEntryPool(void) {
    if (mnuPanelSoundEntryPool != (u32 *)0x0) {
        sdfReleaseResourceAllocation(((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->allocation);
    }
    mnuPanelSoundEntryPool = (u32 *)0x0;
}

extern void sndSetSequenceVolumePan(s32, s32, s32);
void mnuTickPanelSoundEntries(void) {
    s32 i;
    MenuPanelEntry *entry = ((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->entries;

    for (i = 0; i < ((MenuPanelEntryPool *)mnuPanelSoundEntryPool)->count; i++, entry++) {
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
    MenuPanelEntryPool *pool = (MenuPanelEntryPool *)mnuPanelSoundEntryPool;
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

void func_002945B8(MenuTerminalContext *value) {
    D_00438FC8 = value;
}
INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028FD30", D_00437958);

INCLUDE_SDATA(const s32, "game/code_0028FD30", mnuPanelSoundEntryPool);

