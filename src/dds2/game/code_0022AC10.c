#include "common.h"
#include "file_pac.h"
#include "btl_state.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "mdl.h"
#include "evt_unit.h"
#include "scr.h"
#include "dat_state.h"
#include "dat_command.h"

#define BTL_COMMAND_RECORD_BYTES 0x38
#define BTL_LIST_FLAG_MASK 0x7FFF
#define BTL_LIST_MARKED_FLAG 0x4000
#define BTL_ENTRY_CODE_COUNT 5
#define BTL_EXPIRED_POSITIVE_ENTRY_RULE 0x800
#define BTL_EXPIRED_NEGATIVE_ENTRY_RULE 0x1000
#define BTL_COUNTER_ELIGIBILITY_UNSET (-1)
#define BTL_COUNTER_ELIGIBILITY_MET 3
#define BTL_COUNTER_ELIGIBILITY_NOT_MET 0
#define BTL_BLOCK_EMPTY_OR_ALL_FLAGGED 0xA

#define MNU_LIST_INPUT_FLAG 2
#define MNU_LIST_ROW_HEIGHT 0x18
#define MNU_LIST_FRAME_INSET 4
#define MNU_LIST_FIXED_ROW_HEIGHT 0xC0
#define MNU_LIST_LABEL_OFFSET 0x2C
#define MNU_LIST_SELECTED_COLOR 0x89FEFF80
#define MNU_LIST_NORMAL_COLOR 0xA09DC380
#define MNU_LIST_TEXT_DEPTH 0xFF0000
#define MNU_LIST_SELECTED_TEXT_FLAG 4

#define BTL_GROUP_COUNT 8
#define BTL_GROUP_RESOURCE_SLOT_COUNT 8
#define BTL_GROUP_OWNS_RESOURCES_FLAG 1

#define BTL_EVENT_TASK_NONE (-1)
#define BTL_EVENT_ACTION_NONE (-1)
#define BTL_EVENT_SCENE_MODE_LIMIT 0x400
#define BTL_EVENT_SOUND_RESOURCE_COUNT 15
#define BTL_EVENT_SOUND_KEY_FIRST 0x32
#define BTL_EVENT_SOUND_SLOT_FIRST 0xB
#define BTL_EVENT_DATA_LIFETIME_FLAG 2
#define BTL_EVENT_SEQUENCE_BASE 0x01E00000
#define BTL_EVENT_SEQUENCE_ID_BIAS 0x384
#define BTL_EVENT_SEQUENCE_VOLUME 0x7F
#define BTL_EVENT_SEQUENCE_PAN 0x3F

/* Each group also has an independent, singly-linked list of IDs. */
typedef struct BattleGroupIdEntry {
    struct BattleGroupIdEntry *next;
    s32 id;
} BattleGroupIdEntry;


extern BattleGroupNode *btlFindGroupedEntity(s32, s32);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);

extern u8 D_00436F5D;

extern s32 mdlFlagTest(s32);

typedef struct BtlLightParams {
    f32 r;
    f32 g;
    f32 b;
    u8 pad0C[4];
    f32 unk10;
    f32 unk14;
    f32 unk18;
} BtlLightParams;

extern BtlLightParams D_0037FA50;

extern BtlLightParams D_0037FA90;

extern BtlLightParams D_0037FAD0;

extern BtlLightParams D_0037FB20;

extern s32 btlGetRuntime(void);

extern s32 btlCountTasksByKind(u32);

extern void *sdfAllocAndClearQuadwords(s32);

extern void func_002C7CE8(void *);

extern void sndReleaseSlotOwner(void *);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 func_001AC360(u64, BtlIndexList *, u64);



extern s32 btlMatchActorEntryCode(void *, s32);


extern s8 D_00453068[];

typedef struct BattleRuntimeState {
    s32 counter; /* Active encounter updates since initialization. */
    u16 state;
    u8 fadeMode; /* selects the initial overlay alpha in btlInitFadeColors */
    s8 pending;
    s8 active;
    u8 pad09[3];
    u32 options;
    u32 color10;
    u32 color14;
    u8 unk_18[0x20];
    void *ownedData;
    void *resource;
    void *handle;
} BattleRuntimeState;

extern BattleRuntimeState btlRuntimeState;

typedef struct PadButtons {
    s8 unk_0;
    s8 stick;
    u8 pad02[2];
    u8 pageUp, pageDown, up, down;
} PadButtons;

extern PadButtons sdfPadButtonStates[];

extern s32 kwlnFadeIsBackgroundOverlayActive();

extern void btlReleaseOwnedData(void);

extern void *memset(void *, s32, u32);

extern SdfPoolNode D_00380608;

extern u8 D_00380860[];

extern u8 D_00380870[];

extern s32 kwlnHeldTextureReference;

extern void sdfFreeMemoryFromEitherHeap(void *);

extern void *sdfAllocGeneralBlockHigh(s32);

extern void *sdfResourceRetainAddress(void *);

extern s32 sdfAllocatePacketList(s32 (*allocator)(s32));

extern void sdfClearLinkedPacketList(SdfLinkedPacketList *);

extern void sdfAppendPacketChainNode(SdfPacketChain *, SdfLinkedPacketList *);

extern void sdfCreateDescriptorPacket(SdfListHead *list, s32 descriptorAddress,
                                      s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                                      s32 imageAddress, s32 (*allocatePacket)(s32));

extern void kwlnCreateHeldTextureBuffer(s32, s32, f32);

extern s32 kwlnTextureSetReferenceFlagIfPresent(void);

extern void func_0022D040(void);

extern u8 D_003BF961[];

extern u16 D_003BF962[];

extern s32 sdfCreateSemaphore(s32, s32, s32);

extern u32 mdlGroupJobSemaphore;

extern BattleGroupNode *btlGroupNodeHeads[];

extern BattleGroupIdEntry *btlGroupIdHeads[];

extern void scrSetIntegerReturnValue();

extern void ptyAdjustItemQuantity(u32, u32);

extern s32 func_0022D2F8(u32, u32);

extern u32 btlAllocTask(u32);

extern u32 btlGetTaskArguments(u32);

extern void func_0022BA08(void);

extern void btlStartSkillEventTask(u32);


extern s32 btlReleaseScriptResource(void);

extern s32 btlFindModelEntry();

struct BattleScriptTaskData;

/* 0x70-byte queued-task header, distinct from the unit's 0x170-byte BtlTask.
 * The callback union preserves each consumer's actual function prototype. */
typedef struct BattleTask {
    u8 enabled;
    u8 pad01[0xF];
    u8 status;
    u8 pad11[0xF];
    u16 taskId;
    u16 state;
    u16 flags;
    u8 pad26[2];
    s32 startDelay;
    s32 endDelay;
    u32 unk30;
    u32 unk34;
    u64 sequence;
    u64 owner;
    void (*onStart)(u32);
    union {
        void (*update)(void);
        s32 (*run)(void *);
        u32 (*processScript)(struct BattleScriptTaskData *);
    } callback;
    void (*onFinish)(u32 *);
    void *args;
    struct BattleTask *next;
    struct BattleTask *nextActive;
    struct BattleTask *deferNext;
    struct BattleTask *deferPrev;
    u8 pad68[8];
} BattleTask;

typedef struct BattleScriptTaskData {
    u32 object;
    u32 group;
    u32 frames;
} BattleScriptTaskData;

extern void func_0035C860(char *, const char *, ...);

extern char D_0041B650[];

extern char btlPrimaryScriptResourceName[];

extern char btlSecondaryScriptResourceName[];

extern s32 evtFindTaskResourceEntryByKey(s16, s32);

extern char D_0041B7E0[];

typedef struct BattleListEntry {
    u8 pad0[6];
    u16 primaryCount;
    u16 primaryLimit;
    u16 secondaryCount;
    u16 secondaryLimit;
    u16 flags;
} BattleListEntry;

extern s32 mdlRequestAsset(s32, s32, s32);

extern s32 fileRequestIsReady(void *);

extern void sdfBuildPacketE(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern u64 btlStartTask(void *);

extern s32 scrReadIntParameter(s32);

extern void btlApplyScaledUnitEffectParameter(BtlUnit *, s32, s32, f32);

extern s32 btlGetSlotRateKind(BtlUnit *, s32);

extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern u8 *btlFindUnitByModeClear(s32);

extern u8 *btlFindUnitByModeFlagged(s32);

extern void *btlCreateModelChangeTask(void *, s32, s32, s32, s32, s32);

extern void *sdfAllocSizeClassBlock(s32);


extern char D_00436CF8[];

extern char D_0041B628[];

extern char D_0041B640[];

extern char D_0041B768[];

extern char D_0041B780[];

extern char D_0041B7A8[];

extern char D_0041B7D0[];

extern void sdfReleaseResourceAllocation(s32);

extern void *func_0019F448(s32, s32, u32, u32, s32, s32);

extern void func_0019D550(void *, s32, s32);

extern s32 frFontQueueGlyphInSelectedSlot(void *);

extern void func_0020D1C0(u8 *, u8 *, s32, s32, u32, u32);

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, s32);

extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);

extern u8 D_00436EF8[];

extern SdfPoolNode kwlnPositionedTextSurface;

extern void sdfReleaseChipBlock(void *);

extern void sdfQueueNonzeroResourceId(void *);

extern void mdlDestroyContext(MdlCtx *);

extern void mdlDestroyPartList(DevRequest *);

extern void sdfResourceListRelease(DevRequest *, s32);

extern void sdfReleaseChipBlock(void *);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022AC10);

extern char D_0041B6A8[]; /* "/event/e%03d/e%03d/scr/e%03d.bf" */
extern char D_0041B6C8[]; /* "btl:event[%s]\n" */
extern char D_0041B6D8[]; /* "btl:event BE load[e%03d]\n" */
extern char D_0041B6F8[]; /* "btl:event SMG free[%X]\n" */
extern char D_0041B710[]; /* "btl:BSE free\n" */
extern void *sdfReadNamedResource(const char *, void *, s32);
extern s32 mnuCampCreateTask(s32);
extern void func_00101968(s32, s32);
extern s32 sndFindPackedTrackLoadStatus(s32);
extern void sndReleaseMidiTrack(s32);

/* Reset event state and request the scene's script/task resources.
 * The raw unsigned event word is deliberately converted to signed 16-bit.
 * DDS2 releases resident event/0x10000 tracks and also sets event flag bit 4. */
void func_0022AF90(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    u32 battleMode = battleState->battleMode;
    u16 rawEventId;
    s16 eventId;
    s32 eventTask;
    char scriptPath[0x80];

    battleState->eventTaskId = BTL_EVENT_TASK_NONE;
    battleState->eventAction = BTL_EVENT_ACTION_NONE;
    battleState->eventActive = 0;
    battleState->scriptFlags = 0;
    battleState->eventFlags = 0;
    battleState->scriptHandle = 0;
    battleState->eventAssets = 0;
    battleState->eventRequest = 0;
    battleState->eventData = 0;
    if (battleMode >= BTL_EVENT_SCENE_MODE_LIMIT) {
        return;
    }
    rawEventId = datBattleSceneRecords[battleMode].eventId;
    if (rawEventId == 0) {
        return;
    }
    eventId = (s16)rawEventId;
    battleState->eventTaskId = eventId;
    func_0035C860(scriptPath, D_0041B6A8, eventId - eventId % 10, eventId, eventId);
    battleState->eventAssets = sdfReadNamedResource(scriptPath, &battleState->scriptHandle, 0);
    btlBossDebugPrintf(D_0041B6C8, scriptPath);
    eventTask = mnuCampCreateTask(battleState->eventTaskId);
    btlBossDebugPrintf(D_0041B6D8, battleState->eventTaskId);
    func_00101968((s32)battleState->scriptOwner, eventTask);
    battleState->sequenceHandle = BTL_EVENT_SEQUENCE_BASE + ((battleState->eventTaskId - BTL_EVENT_SEQUENCE_ID_BIAS) << 16);
    if (sndFindPackedTrackLoadStatus(battleState->sequenceHandle) != 0) {
        sndReleaseMidiTrack(battleState->sequenceHandle);
        btlBossDebugPrintf(D_0041B6F8, battleState->sequenceHandle);
    }
    if (sndFindPackedTrackLoadStatus(0x10000) != 0) {
        sndReleaseMidiTrack(0x10000);
        btlBossDebugPrintf(D_0041B710);
    }
    battleState->eventFlags |= 6;
}

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B6A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B6C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B6D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B6F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B710);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022B108);

/* Release both optional voice resources once and reset the action state.
 * The shared temporary holds either resource; other event flag bits survive. */
void btlReleaseEventData(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    void *resource;
    if ((battleState->eventFlags & BTL_EVENT_DATA_LIFETIME_FLAG) == 0) {
        return;
    }
    resource = battleState->eventData;
    if (resource != 0) {
        effReleaseBattleVoiceOwner(resource);
        battleState->eventData = 0;
    }
    resource = battleState->eventRequest;
    if (resource != 0) {
        sndReleaseAllVoices(resource);
        battleState->eventRequest = 0;
    }
    battleState->eventActive = 0;
    battleState->eventAction = BTL_EVENT_ACTION_NONE;
    battleState->eventFlags &= ~BTL_EVENT_DATA_LIFETIME_FLAG;
    btlBossDebugPrintf(D_0041B768);
}

extern void btlCreateIndexedSoundResourceNode(s32 slotIndex, u32 handle);

/* Populate sound slots 11..25 from event keys 50..64; skip missing bindings. */
void func_0022B288(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    s32 eventTaskId = battleState->eventTaskId;
    s32 resourceIndex;

    if (eventTaskId == BTL_EVENT_TASK_NONE) {
        return;
    }

    for (resourceIndex = 0; resourceIndex < BTL_EVENT_SOUND_RESOURCE_COUNT; resourceIndex++) {
        s32 resourceKey = resourceIndex + BTL_EVENT_SOUND_KEY_FIRST;
        s32 slotIndex = resourceIndex + BTL_EVENT_SOUND_SLOT_FIRST;
        s32 resourceValue = evtFindTaskResourceEntryByKey(battleState->eventTaskId, resourceKey);

        if (resourceValue == 0) {
            btlBossDebugPrintf(D_0041B780, slotIndex, resourceKey);
        } else {
            btlCreateIndexedSoundResourceNode(slotIndex, resourceValue);
            btlBossDebugPrintf(D_0041B7A8, slotIndex, resourceKey);
        }
    }
}

/* Release the voice resources before the optional event asset allocation. */
void btlReleaseEventAssets(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    void *eventAssets;
    btlReleaseEventData();
    eventAssets = battleState->eventAssets;
    if (eventAssets != 0) {
        sdfReleaseResourceAllocation(eventAssets);
        battleState->eventAssets = 0;
    }
    btlBossDebugPrintf(D_0041B7D0);
}

/* Select a unit and event-resource key from script parameters.
 * Missing/inactive units or missing resources leave state untouched;
 * the command still returns 1 on every path. */
s32 btlCommandSelectEventAction(void) {
    s32 modeSelector = scrReadIntParameter(0);
    s32 unitId = scrReadIntParameter(1);
    s32 actionKey = scrReadIntParameter(2);
    BtlUnit *selectedUnit;
    BtlState *battleState;
    s32 actionResource;
    if (modeSelector == 0) {
        selectedUnit = (BtlUnit *)btlFindUnitByModeClear(unitId);
    } else {
        selectedUnit = (BtlUnit *)btlFindUnitByModeFlagged(unitId);
    }
    if (selectedUnit == 0) {
        return 1;
    }
    if ((selectedUnit->flags & 2) == 0) {
        return 1;
    }
    battleState = (BtlState *)btlGetRuntime();
    actionResource = evtFindTaskResourceEntryByKey(battleState->eventTaskId, actionKey);
    if (actionResource == 0) {
        return 1;
    }
    battleState->eventResult = actionResource;
    battleState->eventUnit = selectedUnit;
    battleState->eventAction = actionKey;
    battleState->eventActive = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022B460);

s32 btlOpStartSelectedUnitModelChange(void) {
    s32 choice = scrReadIntParameter(0);
    s32 unitIndex = scrReadIntParameter(1);
    s32 first = scrReadIntParameter(2);
    s32 second = scrReadIntParameter(3);
    u8 *unit;
    if ((u32)unitIndex >= 0x180) {
        return 1;
    }
    if ((u32)first >= 0x180) {
        return 1;
    }
    if (choice == 0) {
        unit = btlFindUnitByModeClear(unitIndex);
    } else {
        unit = btlFindUnitByModeFlagged(unitIndex);
    }
    if (unit == NULL) {
        return 1;
    }
    btlStartTask(btlCreateModelChangeTask(unit, choice != 0, first, second, 0x18, 0));
    return 1;
}

u8 btlAreModelChangeTasksFinished(void) {
    s64 taskCount;

    taskCount = btlCountTasksByKind(0x1a);
    return taskCount == 0;
}

s32 btlCommandPlayUnitMotion(void) {
    s32 choice = scrReadIntParameter(0);
    s32 unitIndex = scrReadIntParameter(1);
    s32 index = scrReadIntParameter(2);
    BtlUnit *unit;

    if (choice == 0) {
        unit = (BtlUnit *)btlFindUnitByModeClear(unitIndex);
    } else {
        unit = (BtlUnit *)btlFindUnitByModeFlagged(unitIndex);
    }
    if (unit == 0) {
        return 1;
    }
    if ((unit->flags & 2) == 0) {
        return 1;
    }
    if (index >= 0) {
        if (index < 0x1D) {
            btlApplyScaledUnitEffectParameter(unit, index, btlGetSlotRateKind(unit, index), 1.0f);
        } else {
            mdlAddEntryPlainEx((s32)unit->ext->owner, 0, index, 0.0f, 0.0f);
        }
    }
    return 1;
}

/* Apply the fixed volume/pan to a loaded sequence plus the script's offset.
 * Returns 1 even when the sequence is not loaded. */
s32 btlCommandSetSequenceVolumePan(void) {
    BtlState *battleState = (BtlState *)btlGetRuntime();
    s32 sequenceOffset = scrReadIntParameter(0);
    if (sndFindPackedTrackLoadStatus(battleState->sequenceHandle) != 0) {
        sndSetSequenceVolumePan(battleState->sequenceHandle + sequenceOffset, BTL_EVENT_SEQUENCE_VOLUME, BTL_EVENT_SEQUENCE_PAN);
        btlBossDebugPrintf(D_0041B7E0, battleState->sequenceHandle + sequenceOffset);
    }
    return 1;
}

/* Optional hook: it reads its argument (100) straight from $a0 and returns the value passed to scrSetIntegerReturnValue. */
s32 func_0022B760(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    s32 value = 100;

    if (state->scriptReturnHook != NULL) {
        value = state->scriptReturnHook();
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

typedef struct BtlFlagSlot {
    u8 key;
    u8 pad[3];
} BtlFlagSlot;

extern BtlFlagSlot D_003BF960[];

extern u32 effMiscRandMod(void *state, u32 modulus);

/* Script command: picks a random slot of the argument's group whose flag 0x8FF - slot is clear; passes its index (or -1 / 0) to scrSetIntegerReturnValue. */
s32 func_0022B7A0(void) {
    u32 value = scrReadIntParameter(0);
    s32 count;
    s32 pick;
    u32 group;
    u32 i;

    if (value < 100) {
        group = D_003BF960[value].key;
        count = 0;
        for (i = 0; i < 100; i++) {
            if (group == D_003BF960[i].key) {
                if (mdlFlagTest(0x8FF - i) == 0) {
                    count++;
                }
            }
        }
        if (count > 0) {
            pick = effMiscRandMod(0, count);
            for (i = 0; i < 100; i++) {
                if (group == D_003BF960[i].key) {
                    if (mdlFlagTest(0x8FF - i) == 0) {
                        if (--pick == -1) {
                            break;
                        }
                    }
                }
            }
            value = i < 100 ? i : -1;
        } else {
            value = -1;
        }
    } else {
        value = 0;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 btlCmdGetIndexedBattleStateValue(void) {
    u32 index = scrReadIntParameter(0);
    u32 value = 0;
    if (index < 100) {
        value = D_003BF961[index * 4];
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 btlCommandCountModelFlags(void) {
    s64 isSet;
    s32 flag;
    u32 index;
    s32 count;

    index = 0;
    count = 0;
    do {
        flag = 0x8ff - index;
        index = index + 1;
        isSet = mdlFlagTest(flag);
        if (isSet != 0) {
            count = count + 1;
        }
    } while (index < 100);
    scrSetIntegerReturnValue(count);
    return 1;
}

u32 func_0022B988(void) {
    u32 index = scrReadIntParameter(0);
    u32 value = 1;
    if (index < 100) {
        value = D_003BF962[index * 2];
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

u32 func_0022B9D0(void) {
    s32 index = scrReadIntParameter(0);
    if (index < 0x100) {
        ptyAdjustItemQuantity(index, 1);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022BA08);

/* Queue script-resource work for object/group; return its 32-bit task address. */
u32 btlCreateScriptResourceTask(u32 object, u32 group) {
    BattleTask *task = (BattleTask *)btlAllocTask(12);
    BattleScriptTaskData *data;
    task->enabled = 1;
    task->taskId = 0x68;
    task->callback.update = func_0022BA08;
    task->status = 0;
    data = (BattleScriptTaskData *)btlGetTaskArguments((u32)task);
    data->object = object;
    data->group = group;
    data->frames = 0;
    return (u32)task;
}

/* Start the event on frame zero, bind its actor, and return 1 on completion. */
u32 btlUpdateScriptResourceTask(BattleScriptTaskData *record) {
    if (record->frames == 0) {
        BtlState *battle = (BtlState *)btlGetRuntime();
        u32 object;
        btlStartSkillEventTask(record->group);
        object = battle->scriptTask;
        if (object != 0) {
            scrSetCurrentActor((KwlnTask *)object, (void *)record->object);
        }
    }
    if (btlReleaseScriptResource()) {
        return 1;
    }
    ++record->frames;
    return 0;
}



/* Queue object/group event processing with zero elapsed frames; return the task. */
void *btlCreateActionTask(void *object, s32 group) {
    BattleTask *task = (BattleTask *)btlAllocTask(12);
    BattleScriptTaskData *data;

    task->enabled = 1;
    task->taskId = 0x69;
    task->callback.processScript = btlUpdateScriptResourceTask;
    task->status = 0;
    data = (BattleScriptTaskData *)btlGetTaskArguments((u32)task);
    data->object = (u32)object;
    data->group = group;
    data->frames = 0;
    return task;
}

/* Report whether a bit-0x200 unit meets either native restriction test. */
s32 btlHasRestrictedUnit(void) {
    BtlUnit *unitCursor = ((BtlState *)btlGetRuntime())->units;
    while (unitCursor != 0) {
        u32 unitFlags = unitCursor->flags;
        if (unitFlags & 0x200) {
            if (unitFlags & 0xe0) {
                return 1;
            }
            if (unitCursor->partyRecord.status & 0x4000) {
                return 1;
            }
        }
        unitCursor = unitCursor->nextActor;
    }
    return 0;
}

/* Match exactly the marked flag after discarding bit 0x8000. */
s32 btlListHasMarkedFlag(u8 **entryList, s32 entryCount) {
    s32 entryIndex;
    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        BattleListEntry *listEntry = (BattleListEntry *)entryList[entryIndex];
        if ((listEntry->flags & BTL_LIST_FLAG_MASK) == BTL_LIST_MARKED_FLAG) {
            return 1;
        }
    }
    return 0;
}

/* True only when every primary counter is >= its limit; an empty list passes. */
s32 btlListCountersWithinLimits(u8 **entryList, s32 entryCount) {
    s32 entryIndex;
    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        BattleListEntry *listEntry = (BattleListEntry *)entryList[entryIndex];
        if (listEntry->primaryCount < listEntry->primaryLimit) {
            return 0;
        }
    }
    return 1;
}

/* The secondary-counter counterpart also accepts equality and empty lists. */
s32 btlListSecondaryCountersWithinLimits(u8 **entryList, s32 entryCount) {
    s32 entryIndex;
    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        BattleListEntry *listEntry = (BattleListEntry *)entryList[entryIndex];
        if (listEntry->secondaryCount < listEntry->secondaryLimit) {
            return 0;
        }
    }
    return 1;
}

/* Test any requested low-15-bit flag, rather than the exact marked pattern. */
s32 btlListHasMatchingFlag(u8 **entryList, s32 entryCount, u32 requestedFlags) {
    s32 entryIndex;
    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        BattleListEntry *listEntry = (BattleListEntry *)entryList[entryIndex];
        if ((listEntry->flags & BTL_LIST_FLAG_MASK) & requestedFlags) {
            return 1;
        }
    }
    return 0;
}

/* Every entry must classify as 1 or 2 with a corresponding allowed mask bit.
 * Other classifications fail the all-entry check; an empty list passes. */
s32 btlIndexListMatchesEntryCodes(BtlIndexList *indexList, s32 entryCode, u32 allowedCodeBits) {
    u32 matchedCount = 0;
    u32 entryIndex;
    u32 entryCount = btlGetIndexListCount(indexList);

    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        switch (btlMatchActorEntryCode(btlGetIndexListEntry(indexList, entryIndex), entryCode)) {
        case 1:
            if (allowedCodeBits & 0x2555) {
                matchedCount++;
            }
            break;
        case 2:
            if (allowedCodeBits & 0x2AA) {
                matchedCount++;
            }
            break;
        }
    }
    return matchedCount == entryCount;
}

/* Reject expired positive or negative codes according to the command's rule.
 * A returned code value of zero is not rejected; other expiry rules pass. */
INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B768);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B780);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B7A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B7D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B7E0);

s32 btlIndexListNoExpiredEntryCodes(BtlIndexList *indexList, s32 commandId) {
    s32 entryCodes[BTL_ENTRY_CODE_COUNT] = {0, 1, 2, 3, 4};
    s32 entryCount = btlGetIndexListCount(indexList);
    s32 entryIndex;
    u32 codeIndex;
    void *actorEntry;
    s32 requirementBits;

    for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
        actorEntry = btlGetIndexListEntry(indexList, entryIndex);
        requirementBits = datCommandRecords[commandId].requirementBits;
        switch (requirementBits) {
        case BTL_EXPIRED_POSITIVE_ENTRY_RULE:
            for (codeIndex = 0; codeIndex < BTL_ENTRY_CODE_COUNT; codeIndex++) {
                if (btlActorEntryIsExpired(actorEntry, entryCodes[codeIndex]) != 0) {
                    if (btlGetActorEntryCode(actorEntry, entryCodes[codeIndex]) > 0) {
                        return 0;
                    }
                }
            }
            break;
        case BTL_EXPIRED_NEGATIVE_ENTRY_RULE:
            for (codeIndex = 0; codeIndex < BTL_ENTRY_CODE_COUNT; codeIndex++) {
                if (btlActorEntryIsExpired(actorEntry, entryCodes[codeIndex]) != 0) {
                    if (btlGetActorEntryCode(actorEntry, entryCodes[codeIndex]) < 0) {
                        return 0;
                    }
                }
            }
            break;
        }
    }
    return 1;
}

/* Return 3 for met requirements, or 0 for failed/inapplicable requirements.
 * A failed primary check suppresses later checks. Unlike DDS1, DDS2 requires
 * requirementBits == 0 before each check; preserve all three native rereads. */
u16 btlDetermineCommandCounterEligibility(u8 **entryList, s32 entryCount, BtlIndexList *indexList, s32 commandId) {
    s32 eligibility = BTL_COUNTER_ELIGIBILITY_UNSET;
    if (datCommandRecords[commandId].options & 1) {
        if (datCommandRecords[commandId].requirementBits == 0) {
            switch (datCommandRecords[commandId].primaryLimitKind) {
            case 2:
            case 5:
            case 7:
            case 9:
            case 11:
            case 15:
                eligibility = btlListCountersWithinLimits(entryList, entryCount) ? BTL_COUNTER_ELIGIBILITY_MET : BTL_COUNTER_ELIGIBILITY_NOT_MET;
                break;
            }
        }
        if (eligibility != BTL_COUNTER_ELIGIBILITY_NOT_MET) {
            if (datCommandRecords[commandId].requirementBits == 0) {
                switch (datCommandRecords[commandId].secondaryLimitKind) {
                case 2:
                case 5:
                case 7:
                case 9:
                case 11:
                case 15:
                    eligibility = btlListSecondaryCountersWithinLimits(entryList, entryCount) ? BTL_COUNTER_ELIGIBILITY_MET : BTL_COUNTER_ELIGIBILITY_NOT_MET;
                    break;
                }
            }
            if (eligibility != BTL_COUNTER_ELIGIBILITY_NOT_MET) {
                if (datCommandRecords[commandId].requirementBits == 0) {
                    if (datCommandRecords[commandId].attribute.parts.kind == 2) {
                        eligibility = btlListHasMatchingFlag(entryList, entryCount, datCommandRecords[commandId].attribute.parts.flagMask) == 0 ? BTL_COUNTER_ELIGIBILITY_MET : BTL_COUNTER_ELIGIBILITY_NOT_MET;
                    }
                }
            }
        }
    }
    return (eligibility < 0) ? BTL_COUNTER_ELIGIBILITY_NOT_MET : eligibility;
}

/* Query the command's block reason, freeing the temporary target list.
 * DDS2 omits DDS1's mode/flag gate. Its final empty/all-flagged-target reason
 * is 10, versus 9 in DDS1. Zero means no block. */
s32 btlGetCommandBlockReason(BtlTask *actionTask, s32 commandId) {
    DatCommandRecord *commandRecord;
    BtlIndexList *targetList;
    s32 targetCount;
    s32 flaggedTargetCount;
    s32 targetIndex;
    if (*(u32 *)(btlGetRuntime() + 0x220) & 0x800) {
        return 0;
    }
    if (commandId <= 0) {
        return 0;
    }
    commandRecord = (DatCommandRecord *)(commandId * BTL_COMMAND_RECORD_BYTES + (s32)datCommandRecords);
    if ((commandRecord->attribute.bits & 0x400000FF) == 0x40000002) {
        if ((~commandRecord->restriction & 0x7FFF) == 0x4000) {
            if (btlHasRestrictedUnit() == 0) {
                return 2;
            }
        }
    }
    flaggedTargetCount = 0;
    targetList = btlAllocateIndexList(0xD);
    func_001AC360((s32)actionTask, targetList, 0);
    targetCount = btlGetIndexListCount(targetList);
    if (datCommandRecords[commandId].flags & 8) {
        for (targetIndex = 0; targetIndex < targetCount; targetIndex++) {
            if (((BtlUnit *)btlGetIndexListEntry(targetList, targetIndex))->partyRecord.status & 0x800) {
                flaggedTargetCount++;
            }
        }
    }
    btlFreeIndexList(targetList);
    if (targetCount != 0) {
        if (targetCount != flaggedTargetCount) {
            return 0;
        }
    }
    return BTL_BLOCK_EMPTY_OR_ALL_FLAGGED;
}

extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);
extern s32 btlLowestSetPairIndex(u32);

s32 btlGetCommandTargetEligibility(BtlIndexList *indexList, s32 commandId) {
    u8 *entryList[26];
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 count;
    s32 i;
    u32 flags;
    s32 requirement;
    s32 code;

    if (battle->unk220 & 0x40) {
        return 0;
    }
    if (commandId <= 0) {
        return 0;
    }
    count = btlGetIndexListCount(indexList);
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        entryList[i] = (u8 *)btlGetIndexListEntry(indexList, i) + 0x120;
    }
    if (datCommandRecords[commandId].kind == 2 ||
        (datCommandRecords[commandId].flags & 0x20)) {
        for (i = 0; i < count; i++) {
            flags = btlGetEntryFlagsUnlessDisabled(&((BtlUnit *)btlGetIndexListEntry(indexList, i))->partyRecord);
            if (flags & 0x8000) {
                return 7;
            }
            if (flags & 0x40) {
                return 5;
            }
        }
    }
    if (datCommandRecords[commandId].unk30 == 1) {
        if (battle->commandRestrictFlags & 0x10) {
            return 9;
        }
        for (i = 0; i < count; i++) {
            if (btlGetEntryFlagsUnlessDisabled(&((BtlUnit *)btlGetIndexListEntry(indexList, i))->partyRecord) & 0x800) {
                return 9;
            }
        }
    }
    requirement = datCommandRecords[commandId].requirementBits;
    if (requirement == 0x400 || requirement == 0x2000) {
        code = btlLowestSetPairIndex(requirement);
        if (code >= 0) {
            if (btlIndexListMatchesEntryCodes(indexList, code,
                                             datCommandRecords[commandId].requirementBits)) {
                return 3;
            }
        }
    }
    return btlDetermineCommandCounterEligibility(entryList, count, indexList, commandId);
}

/* Checks a command row's required-entry flags against the index list: 0 when not satisfied, 3 when every flagged pair matches. */
s32 btlCheckCommandRequiredEntryMatches(BtlIndexList *list, s32 row) {
    s32 result = 0;
    u32 flags;
    u32 mask;
    s32 bit;
    s32 index;

    if (row <= 0) {
        return result;
    }
    flags = datCommandRecords[row].requirementBits;
    if (flags == 0) {
        return result;
    }
    if (flags == 0x800 || flags == 0x1000) {
        return btlIndexListNoExpiredEntryCodes(list, row) != 0 ? 3 : 0;
    }
    for (bit = 0; bit < 0x20; bit++) {
        mask = 1 << bit;
        if (flags & mask) {
            index = btlLowestSetPairIndex(mask);
            if (index < 7 && index != -1) {
                if (btlIndexListMatchesEntryCodes(list, index, mask) == 0) {
                    return 0;
                }
            }
        }
    }
    return 3;
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022C600);

/* 0x108-byte sound cache owner, with 29 file/handle slots; not a battler. */
typedef struct SoundSlotOwner {
    u32 flags;
    s32 category;
    s32 id;
    s32 refCount;
    s32 load[2];
    s32 slot[0x1D];
    s32 handle[0x1D];
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

/* 0x20-byte model cache entry owns a file/PAC request and a sound-cache reference. */
typedef struct BattleModelEntry {
    s32 kind;
    s32 id;
    s32 refCount;
    s8 state;
    u8 unk_0d[3];
    FilePacRequest *packRequest;
    SoundSlotOwner *soundOwner;
    struct BattleModelEntry *prev;
    struct BattleModelEntry *next;
} BattleModelEntry;

/* Allocate a cache entry with one reference and insert it at the list head. */
BattleModelEntry *btlCreateModelEntry(void) {
    BattleModelEntry *cacheEntry = sdfAllocAndClearQuadwords(sizeof(BattleModelEntry));
    u8 *battleState;
    BattleModelEntry *previousHead;
    cacheEntry->refCount = 1;
    cacheEntry->state = 0;
    battleState = (u8 *)btlGetRuntime();
    cacheEntry->prev = 0;
    previousHead = *(BattleModelEntry **)(battleState + 0x264);
    if (previousHead != 0) {
        previousHead->prev = cacheEntry;
        cacheEntry->next = *(BattleModelEntry **)(battleState + 0x264);
    } else {
        cacheEntry->next = 0;
    }
    *(BattleModelEntry **)(battleState + 0x264) = cacheEntry;
    return cacheEntry;
}

/* Only the final reference releases resources and unlinks the cache entry. */
void btlReleaseModelEntry(BattleModelEntry *cacheEntry) {
    if (--cacheEntry->refCount != 0) {
        return;
    }
    if (cacheEntry->packRequest != 0) {
        func_002C7CE8(cacheEntry->packRequest);
    }
    if (cacheEntry->soundOwner != 0) {
        sndReleaseSlotOwner(cacheEntry->soundOwner);
    }
    if (cacheEntry->next != 0) {
        cacheEntry->next->prev = cacheEntry->prev;
    }
    if (cacheEntry->prev != 0) {
        cacheEntry->prev->next = cacheEntry->next;
    } else {
        *(BattleModelEntry **)((u8 *)btlGetRuntime() + 0x264) = cacheEntry->next;
    }
    sdfReleaseChipBlock(cacheEntry);
    btlBossDebugPrintf("btl:pack free[%X,%X]\n", cacheEntry->kind, cacheEntry->id);
}

/* Release one reference from each entry, preserving any retained entries. */
void btlReleaseAllModelEntries(void) {
    BattleModelEntry *cacheCursor = *(BattleModelEntry **)((u8 *)btlGetRuntime() + 0x264);
    BattleModelEntry *nextEntry;
    while (cacheCursor != 0) {
        nextEntry = cacheCursor->next;
        btlReleaseModelEntry(cacheCursor);
        cacheCursor = nextEntry;
    }
}

/* Format the model path into the caller's buffer; nonzero selects devil data. */
void btlFormatModelResourcePath(s32 isDevil, s32 modelId, char *pathOut) {
    if (isDevil == 0) {
        func_0035C860(pathOut, "%spc%03X_ms.LB", "/model/human/", modelId);
    } else {
        func_0035C860(pathOut, "%s%03X_ms.LB", "/model/devil/", modelId);
    }
}

/* Nonzero cache state bypasses requests. Otherwise retain both native model
 * queries and the signed-byte readiness result. */
s8 btlIsModelPackEntryReady(BattleModelEntry *cacheEntry) {
    s32 requestReady;
    if (cacheEntry->state != 0) {
        return 1;
    }
    if (mdlRequestAsset(cacheEntry->kind, cacheEntry->id, 0) == 0 ||
        mdlRequestAsset(cacheEntry->kind, cacheEntry->id, 0) == -1) {
        return 0;
    }
    if (cacheEntry->packRequest == 0) {
        return 1;
    }
    requestReady = fileRequestIsReady(cacheEntry->packRequest);
    return requestReady;
}

/* Return the matching cache entry's legacy 32-bit address, or zero. */
s32 btlFindModelEntry(modelKind, modelId)
s32 modelKind;
s32 modelId;
{
    BattleModelEntry *cacheCursor = *(BattleModelEntry **)((u8 *)btlGetRuntime() + 0x264);
    while (cacheCursor != 0) {
        if (cacheCursor->kind == modelKind && cacheCursor->id == modelId) {
            return (s32)cacheCursor;
        }
        cacheCursor = cacheCursor->next;
    }
    return 0;
}

void func_0022CA40(void) {
}

void func_0022CA48(void) {
    btlReleaseAllModelEntries();
}

/* Reuse a cached kind/id pair or request its model pack and first reference. */
void btlLoadModelPack(s32 modelKind, s32 modelId) {
    char resourcePath[128];
    BattleModelEntry *cacheEntry = (BattleModelEntry *)btlFindModelEntry(modelKind, modelId);

    if (cacheEntry == 0) {
        cacheEntry = btlCreateModelEntry();
        cacheEntry->kind = modelKind;
        cacheEntry->id = modelId;
        mdlRequestAsset(modelKind, modelId, 0);
        if (sndFindListNodeForChannel(modelKind, modelId) == 0) {
            btlFormatModelResourcePath(modelKind, modelId, resourcePath);
            cacheEntry->packRequest = (FilePacRequest *)fileQueuePlainDispatchRequest(resourcePath);
            btlBossDebugPrintf("btl:pack load start[%s][%X,%X]\n", resourcePath, modelKind, modelId);
        } else {
            cacheEntry->packRequest = 0;
            cacheEntry->soundOwner = 0;
            btlBossDebugPrintf("btl:pack load start[same motSE find][%X,%X]\n", modelKind, modelId);
        }
    } else {
        btlBossDebugPrintf("btl:same pack find[%X,%X]\n", modelKind, modelId);
        cacheEntry->refCount++;
    }
}

/* Drop one reference from the matching kind/id cache entry, if it exists. */
void btlReleaseFoundModelEntry(s32 modelKind, s32 modelId) {
    s32 entryAddress;

    entryAddress = btlFindModelEntry(modelKind, modelId);
    if (entryAddress != 0) {
        btlReleaseModelEntry((BattleModelEntry *)entryAddress);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022CBA0);

/* Return the sign-extended cache state, or zero when no entry exists. */
s32 btlGetEntryState(s32 modelKind, s32 modelId) {
    BattleModelEntry *cacheEntry = (BattleModelEntry *)btlFindModelEntry(modelKind, modelId);
    if (cacheEntry != 0) {
        return cacheEntry->state;
    }
    return 0;
}

/* Despite its public name, this only queries readiness; it releases nothing. */
s32 btlReleaseEntryIfReady(s32 kind, s32 id) {
    s32 entry = btlFindModelEntry(kind, id);
    if (entry != 0) {
        return btlIsModelPackEntryReady((BattleModelEntry *)entry);
    }
    return entry;
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022CD60);

extern s32 sdfConsCreateDrawPacket(s32, s32, s32);
extern void sdfQueueTexturedQuad(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                                 s32, s32, s32, s32, s32, s32, s32, s32, s32, s32 (*)(s32));

void func_0022CE30(s32 list, s32 primitive, s32 color, s32 depth, f32 scale) {
    s32 halfWidth;
    s32 halfHeight;
    s32 xOffset;
    s32 yOffset;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 horizontal[4];
    s32 vertical[4];

    sdfConsCreateDrawPacket(list, kwlnHeldTextureReference, (primitive >> 9) & 1);
    halfWidth = 0x1000;
    halfHeight = 0x700;
    xOffset = (s32)((f32)halfWidth * scale);
    yOffset = (s32)((f32)halfHeight * scale);
    horizontal[0] = halfWidth - xOffset;
    horizontal[1] = halfWidth + xOffset;
    vertical[0] = halfHeight - yOffset;
    vertical[1] = halfHeight + yOffset;
    left = horizontal[0] + 0x7000;
    top = vertical[0] + 0x7900;
    right = horizontal[1] + 0x7000;
    bottom = vertical[1] + 0x7900;
    sdfQueueTexturedQuad(list, color, primitive,
                         left, top, 0, 0,
                         right, top, 0x2000, 0,
                         left, bottom, 0, 0xE00,
                         right, bottom, 0x2000, 0xE00,
                         depth, 0);
}

void btlBuildOverlayQuadPacket(s32 packet, s32 first, s32 second, s32 color) {
    sdfBuildPacketE(packet, second, first, 0x7000, 0x7900, 0x9000, 0x7900, 0x7000,
                  0x8700, 0x9000, 0x8700, color, 0);
}

void btlInitLightParams(void) {
    D_0037FA50.r = 1.75f;
    D_0037FA50.g = 1.75f;
    D_0037FA50.b = 1.75f;
    D_0037FA50.unk10 = 0.8660254f;
    D_0037FA50.unk14 = 0.5f;
    D_0037FA50.unk18 = 0;
    D_0037FA90.r = 0;
    D_0037FA90.g = 0;
    D_0037FA90.b = 0;
    D_0037FA90.unk10 = 0;
    D_0037FA90.unk14 = 0;
    D_0037FA90.unk18 = 0;
    D_0037FAD0.r = 0;
    D_0037FAD0.g = 0;
    D_0037FAD0.b = 0;
    D_0037FAD0.unk10 = 0;
    D_0037FAD0.unk14 = 0;
    D_0037FAD0.unk18 = 0;
    D_0037FB20.r = 1.15f;
    D_0037FB20.g = 1.15f;
    D_0037FB20.b = 1.15f;
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022D040);

void btlReleaseOwnedData(void) {
    void *data = btlRuntimeState.ownedData;
    if (data != 0) {
        sdfFreeMemoryFromEitherHeap(data);
        btlRuntimeState.ownedData = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022D2F8);

void btlInitFadeColors(void) {
    func_0022D040();
    btlRuntimeState.color10 = 0x80808080;
    if (btlRuntimeState.fadeMode < 2) {
        btlRuntimeState.color14 = 0x20FFFFFF;
    } else {
        btlRuntimeState.color14 = 0x00FFFFFF;
    }
}

s32 btlUpdateFadeIn(void) {
    func_0022D2F8(btlRuntimeState.color14, btlRuntimeState.color10);
    if ((btlRuntimeState.color14 & 0xFF000000) != 0) {
        btlRuntimeState.color14 -= 0x08000000;
        return 0;
    }
    return 1;
}

void btlFadeSelectionOverlayAlpha(void) {
}

s32 btlFadeSharedOverlayAlpha(void) {
    s32 result = func_0022D2F8(btlRuntimeState.color14, btlRuntimeState.color10);
    if ((btlRuntimeState.color10 & 0xFF000000) > 0x08000000) {
        btlRuntimeState.color10 -= 0x08000000;
    } else {
        btlRuntimeState.color10 &= 0x00FFFFFF;
    }
    return result;
}

void btlClearOverlayBuffers(void) {
    u8 *entry;
    s32 i;
    u64 clearValue;
    kwlnCreateHeldTextureBuffer(0x200, 0xe0, 0.0f);
    kwlnTextureSetReferenceFlagIfPresent();
    entry = D_00380870;
    i = 0;
    clearValue = 0x80008000ULL;
    entry += 0x1b70;
    do {
        i++;
        *(u64 *)(entry - 0x10) = clearValue;
        *(u64 *)entry = clearValue;
        entry += 0x1f40;
    } while (i != 2);
    btlRuntimeState.options |= 2;
}

void btlResetHeldTextureState(void) {
    kwlnTextureClearReferenceFlag();
    kwlnTextureReleaseHeldReference();
}

void btlInitializeGraphicsRuntime(void) {
    BattleRuntimeState *runtime = &btlRuntimeState;
    SdfListHead *packetList;
    SdfLinkedPacketList *context;
    runtime->handle = sdfAllocGeneralBlockHigh(0x70000);
    runtime->resource = sdfResourceRetainAddress(runtime->handle);
    packetList = (SdfListHead *)sdfAllocatePacketList(0);
    context = (SdfLinkedPacketList *)sdfAllocPacketAligned(16);
    sdfClearLinkedPacketList(context);
    sdfCreatePatchableResourcePacket(packetList, context, 0, 0, 0x200, 0xe0, (s32)runtime->resource, 0, 0, 0);
    sdfAppendPacketChainNode((SdfPacketChain *)D_00380860, context);
    D_00380608.append((SdfListHead *)&D_00380608, packetList);
}

void btlSubmitFrameAndQueueRuntimeHandle(void) {
    BattleRuntimeState *runtime = &btlRuntimeState;
    SdfListHead *packetList = (SdfListHead *)sdfAllocatePacketList(0);
    sdfCreateDescriptorPacket(packetList, (s32)((SdfTex *)kwlnHeldTextureReference)->primaryResource,
                              0, 0, 0x200, 0xe0, (s32)runtime->resource, 0);
    D_00380608.append((SdfListHead *)&D_00380608, packetList);
    sdfQueueNonzeroResourceId(runtime->handle);
    runtime->handle = 0;
    runtime->resource = 0;
    runtime->options |= 1;
}

void btlInitializeOverlayGraphics(void) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocatePacketList(0);
    SdfLinkedPacketList *context = (SdfLinkedPacketList *)sdfAllocPacketAligned(16);
    sdfClearLinkedPacketList(context);
    sdfCreateGraphBufferCopyPacket(packetList, context, ((SdfTex *)kwlnHeldTextureReference)->primaryResource, 0, 0, 0, 0, 0x200, 0xe0, 0, 0);
    sdfAppendPacketChainNode((SdfPacketChain *)D_00380860, context);
    D_00380608.append((SdfListHead *)&D_00380608, packetList);
    btlRuntimeState.options |= 1;
}

void btlResetRuntimeState(void);

extern u32 kwlnDrawControlFlags;
extern char D_0041B988[];
extern char D_0041B9A0[];
extern char D_0041B9B8[];
extern void func_00134A18(void);
extern void kwlnFadeBackgroundStartOut(s32);
extern s32 sdfCheckPendingWorkWithInterrupts(void);

s32 btlUpdateRuntimeFadeState(void) {
    s8 mode;

    if (btlRuntimeState.active == 0) {
        return 0;
    }
    D_0037F980.offsetX = 2048.0f;
    D_0037F980.offsetY = 2048.0f;
    switch (btlRuntimeState.state) {
    case 1:
        if (btlRuntimeState.fadeMode < 2) {
            func_00134A18();
            if (btlRuntimeState.counter == 0) {
                btlInitLightParams();
                btlBossDebugPrintf(D_0041B988);
                break;
            }
            btlRuntimeState.state = 2;
            btlInitializeGraphicsRuntime();
            btlInitFadeColors();
        } else {
            mode = btlRuntimeState.fadeMode;
            if (mode == 2) {
                kwlnFadeBackgroundStartOut(0);
                kwlnDrawControlFlags |= 0x10000000;
                if (btlRuntimeState.counter < 3) {
                    func_00134A18();
                    btlInitLightParams();
                    kwlnDrawControlFlags |= 0x02000000;
                    btlBossDebugPrintf(D_0041B9A0);
                    break;
                }
                btlRuntimeState.state = 2;
                while (sdfCheckPendingWorkWithInterrupts() != 0) {
                }
                btlClearOverlayBuffers();
                btlInitializeOverlayGraphics();
                btlInitFadeColors();
            }
        }
        /* Fall through after preparing the initial fade. */
    case 2:
        if (btlRuntimeState.counter == 2 && (btlRuntimeState.options & 1) == 0) {
            while (sdfCheckPendingWorkWithInterrupts() != 0) {
            }
            btlClearOverlayBuffers();
            btlSubmitFrameAndQueueRuntimeHandle();
        }
        if (btlUpdateFadeIn() != 0) {
            if (btlRuntimeState.pending != 0) {
                btlFadeSelectionOverlayAlpha();
                btlRuntimeState.state = 3;
            }
        }
        break;
    case 3:
        if (btlFadeSharedOverlayAlpha() != 0) {
            btlRuntimeState.state = 4;
        }
        break;
    case 4:
        btlResetHeldTextureState();
        btlResetRuntimeState();
        btlBossDebugPrintf(D_0041B9B8);
        break;
    }
    btlRuntimeState.counter++;
    return 1;
}

void btlClearRuntimeState(void) {
    BattleRuntimeState *state = &btlRuntimeState;
    memset(state, 0, sizeof(*state));
    state->counter = 0;
    state->state = 0;
    state->fadeMode = 0;
    state->pending = 0;
    state->active = 0;
    state->options = 0;
    state->handle = 0;
    state->resource = 0;
}


void btlResetAsyncState(void) {
    void *handle = btlRuntimeState.handle;
    if (handle != 0) {
        sdfQueueNonzeroResourceId(handle);
        btlRuntimeState.handle = 0;
        btlRuntimeState.resource = 0;
    }
    btlResetRuntimeState();
}

void btlActivateRuntime(u8 condition) {
    BattleRuntimeState *battle = &btlRuntimeState;
    battle->fadeMode = condition;
    battle->counter = 0;
    battle->state = 1;
    battle->active = 1;
    battle->pending = 0;
    battle->options = 0;
    if (kwlnFadeIsBackgroundOverlayActive() != 0) {
        battle->options |= 4;
    }
}

void btlResetRuntimeState(void) {
    btlRuntimeState.state = 0;
    btlRuntimeState.active = 0;
    btlReleaseOwnedData();
}

s32 func_0022E450(void) {
    return D_00453068[0];
}

s32 func_0022E460(void) {
    u16 state;

    if (btlRuntimeState.active == 0) {
        return 1;
    }
    state = btlRuntimeState.state;
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_0022E490(void) {
    u16 state;

    if (btlRuntimeState.active == 0) {
        return 1;
    }
    state = btlRuntimeState.state;
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void btlMarkRuntimeUpdatePending(void) {
    if (btlRuntimeState.active != 0) {
        btlRuntimeState.pending = 1;
    }
}

extern s32 *D_00435DD8;
extern s32 *D_00435DDC;
extern s32 *D_00435DFC;
extern char D_00436EE8[];
extern char D_00436EF0[];
extern s32 func_001ABF50(BtlUnit *, s32);
extern void btlBossDebugPrintfN(s32, s32, s32, const char *, ...);
extern void evtSetDrawSurfaceIndex(s32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, u32, u32, u32, u32);

typedef struct BtlAffinityRow {
    s32 value[19];
} BtlAffinityRow;

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B988);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B9A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B9B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B9D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B9E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041B9F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BA90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BAF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB38);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB68);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BB98);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BBB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BBC8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BBE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BBF8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BC10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BC20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041BC30);

void btlDrawUnitAffinityDebug(BtlUnit *unit, s32 x, s32 y) {
    BtlAffinityRow row;
    const char *elementNames[19] = {"BUTURI  ", "ZYUGEKI ", "KAEN    ", "HYOKETU ", "DENGEKI ", "SHOGEKI ", "TIHEN   ", "BANNO   ", "HAMA    ", "JYUSATU ", "CHARM   ", "POISON  ", "CLOSE   ", "PANIC   ", "SINKEI  ", "HUNT    ", "RECOVERY", "HOJO    ", "SPECIAL "};
    const char *labels[13] = {"DV_MUKOU ", "DV_HANSYA", "DV_KYUSYU", "DV_BADOFF", "DV_WK    ", "MUKOU|WK ", "HANSYA|WK", "KYUSYU|WK", "BADOFF|WK", "MUKOU|BOFF", "HANSYA|BOFF", "KYUSYU|BOFF", "DV_BUG!!!"};
    u32 color[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 i;
    s32 value;
    s32 label;

    if ((unit->flags & 1) != 0) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        color[3] = color[2] = color[1] = color[0] = 0x80A0A0A0;
        evtSubmitDefaultDepthGradientRect(x, y, 240, 2, color[0], color[1], color[2], color[3]);
        color[3] = color[2] = color[1] = color[0] = 0x80101010;
        evtSubmitDefaultDepthGradientRect(x, y, 240, 320, color[0], color[1], color[2], color[3]);
        color[3] = color[2] = color[1] = color[0] = 0x80A0A0A0;
        evtSubmitDefaultDepthGradientRect(x, y + 320, 240, 2, color[0], color[1], color[2], color[3]);
        btlBossDebugPrintfN(x, y, 0, D_00436EE8);
        if ((unit->flags & 0x200) != 0) {
            if ((unit->flags & 0x1000) != 0) {
                row = ((BtlAffinityRow *)D_00435DD8)[unit->partyRecord.unitId];
            } else {
                row = ((BtlAffinityRow *)D_00435DDC)[unit->partyRecord.unitId];
            }
        } else {
            row = ((BtlAffinityRow *)D_00435DFC)[unit->partyRecord.unitId];
        }
        for (i = 0; i < 16; i++) {
            row.value[i] = func_001ABF50(unit, i);
            value = row.value[i];
            if ((value & 0xFFFF0000) != 0) {
                switch (value & 0xFFFF0000) {
                case 0x10000: label = 0; break;
                case 0x20000: label = 1; break;
                case 0x40000: label = 2; break;
                case 0x100000: label = 3; break;
                case 0x80000000: label = 4; break;
                case 0x80010000: label = 5; break;
                case 0x80020000: label = 6; break;
                case 0x80040000: label = 7; break;
                case 0x80100000: label = 8; break;
                case 0x110000: label = 9; break;
                case 0x120000: label = 10; break;
                case 0x140000: label = 11; break;
                default: label = 12; break;
                }
                btlBossDebugPrintfN(x, y + (i + 1) * 18, 0, "%s:%3d|%s", elementNames[i], (u16)row.value[i], labels[label]);
            } else {
                btlBossDebugPrintfN(x, y + (i + 1) * 18, 0, D_00436EF0, elementNames[i], (u16)value);
            }
        }
    }
}

typedef struct MenuList {
    u32 count;
    u32 cursor;
    u32 top;
    u32 rows;
} MenuList;

/* Apply up/down/page-down/page-up precedence, including endpoint wrapping.
 * Returns 1 only for a negative stick byte when no direction branch ran. */
s32 mnuListMoveCursor(MenuList *menuList) {
    if (sdfPadButtonStates->up & MNU_LIST_INPUT_FLAG) {
        if (menuList->cursor != 0) {
            menuList->cursor--;
            if (menuList->cursor == menuList->top && menuList->cursor != 0) {
                menuList->top = menuList->cursor - 1;
            }
        } else {
            menuList->cursor = menuList->count - 1;
            menuList->top = menuList->count - menuList->rows;
        }
    } else if (sdfPadButtonStates->down & MNU_LIST_INPUT_FLAG) {
        if (menuList->cursor >= menuList->count - 1) {
            menuList->cursor = 0;
            menuList->top = 0;
        } else {
            menuList->cursor++;
            if (menuList->cursor == menuList->top + menuList->rows - 1 && menuList->top < menuList->count - menuList->rows) {
                menuList->top++;
            }
        }
    } else if (sdfPadButtonStates->pageDown & MNU_LIST_INPUT_FLAG) {
        if (menuList->top + menuList->rows * 2 < menuList->count) {
            menuList->top += menuList->rows;
            menuList->cursor += menuList->rows;
        } else {
            menuList->cursor = menuList->count - 1;
            menuList->top = menuList->count - menuList->rows;
        }
    } else if (sdfPadButtonStates->pageUp & MNU_LIST_INPUT_FLAG) {
        if (menuList->top >= menuList->rows) {
            menuList->top -= menuList->rows;
            menuList->cursor -= menuList->rows;
        } else {
            menuList->cursor = 0;
            menuList->top = 0;
        }
    } else if (sdfPadButtonStates->stick < 0) {
        return 1;
    }
    return 0;
}

/* Queue text at pixel coordinates, using the font's native X/Y scaling. */
s32 mnuQueueColoredGlyphAtPosition(s32 x, s32 y, s32 text) {
    void *textGlyph = func_0019F448(x << 4, y << 3, MNU_LIST_TEXT_DEPTH, MNU_LIST_NORMAL_COLOR, text, 0);
    func_0019D550(textGlyph, 0, 0x60);
    return frFontQueueGlyphInSelectedSlot(textGlyph);
}

/* Draw the visible text interval, highlighting the selected absolute index.
 * Keep pointer-typed coordinates and the legacy s32 fallthrough unchanged. */
s32 btlDrawSelectableListRows(u8 *x, u8 *y, s32 unusedMode, u8 *selectionState, s32 *rowTexts) {
    u32 firstIndex;
    u32 visibleRowCount;
    u32 itemIndex;
    u32 endIndex;
    u32 selectedIndex;
    s32 rowY;
    firstIndex = ((MenuList *)selectionState)->top;
    visibleRowCount = ((MenuList *)selectionState)->rows;
    endIndex = firstIndex + visibleRowCount;
    itemIndex = firstIndex;
    selectedIndex = ((MenuList *)selectionState)->cursor;
    rowY = (s32)y;
    for (; itemIndex < endIndex; itemIndex++) {
        void *textGlyph = func_0019F448((s32)x << 4, rowY << 3, MNU_LIST_TEXT_DEPTH, itemIndex == selectedIndex ? MNU_LIST_SELECTED_COLOR : MNU_LIST_NORMAL_COLOR, rowTexts[itemIndex], 0);
        func_0019D550(textGlyph, 0, 0x60);
        frFontQueueGlyphInSelectedSlot(textGlyph);
        rowY += MNU_LIST_ROW_HEIGHT;
    }
}

/* Size the frame from the visible rows, then forward the row-draw result. */
s32 mnuDrawMenuFrameSizedToRows(u8 *x, u8 *y, s32 mode, u8 *selectionState, s32 *rowTexts) {
    s32 frameHeight = ((MenuList *)selectionState)->rows * MNU_LIST_ROW_HEIGHT + MNU_LIST_FRAME_INSET;
    func_0020D1C0(x - MNU_LIST_FRAME_INSET, y - MNU_LIST_FRAME_INSET, mode, frameHeight, 0x80806020, 0x30000000);
    return btlDrawSelectableListRows(x, y, mode, selectionState, rowTexts);
}

/* Submit formatted indices in one packet list, then draw the text column.
 * Preserve the separate fixed-point and pixel-coordinate arithmetic. */
s32 mnuDrawSelectableMenuRows(u8 *x, u8 *y, s32 mode, u8 *selectionState, s32 *rowTexts) {
    void *indexPackets;
    u32 firstIndex;
    u32 visibleRowCount;
    u32 itemIndex;
    u32 endIndex;
    u32 selectedIndex;
    s32 rowY;
    func_0020D1C0(x - MNU_LIST_FRAME_INSET, y - MNU_LIST_FRAME_INSET, mode, ((MenuList *)selectionState)->rows * MNU_LIST_ROW_HEIGHT + MNU_LIST_FRAME_INSET, 0x80806020, 0x30000000);
    indexPackets = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(indexPackets);
    firstIndex = ((MenuList *)selectionState)->top;
    visibleRowCount = ((MenuList *)selectionState)->rows;
    endIndex = firstIndex + visibleRowCount;
    itemIndex = firstIndex;
    selectedIndex = ((MenuList *)selectionState)->cursor;
    if (itemIndex < endIndex) {
        rowY = (s32)y * 8 + 0x7900;
        for (; itemIndex < endIndex; itemIndex++) {
            s32 selectionFlags = MNU_LIST_SELECTED_TEXT_FLAG;
            if (itemIndex != selectedIndex) {
                selectionFlags = 0;
            }
            sdfAppendPacket(indexPackets, (u32)sdfCreateFormattedSifCommand((s32)x * 16 + 0x7000, rowY, MNU_LIST_TEXT_DEPTH, selectionFlags, (const char *)D_00436EF8, itemIndex));
            rowY += MNU_LIST_FIXED_ROW_HEIGHT;
        }
    }
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, (SdfListHead *)indexPackets);
    return btlDrawSelectableListRows(x + MNU_LIST_LABEL_OFFSET, y, mode, selectionState, rowTexts);
}

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022F068);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_0022F180);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_002303D0);

void func_00230960(void) {
    D_00436F5D = 0;
    dds3WorkClear();
}

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C118);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C128);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C138);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C148);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C158);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C168);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C178);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C188);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C198);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C1F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C208);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C218);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C228);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C238);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C248);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C258);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C268);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C278);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C288);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C298);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C2A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C2B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C2C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C2D8);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_00230978);

INCLUDE_ASM(const s32, "game/code_0022AC10", func_00230D90);

/* Create the model-job semaphore and clear both lists for all eight groups. */
void btlInitializeCommandSemaphoreSlots(void) {
    s32 i;

    mdlGroupJobSemaphore = sdfCreateSemaphore(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        btlGroupNodeHeads[i] = 0;
        btlGroupIdHeads[i] = 0;
    }
}

/* Return the first node of this type in group, or NULL. */
BattleGroupNode *btlFindGroupedEntity(s32 group, s32 type) {
    BattleGroupNode *entry = btlGroupNodeHeads[group];
    while (entry != 0) {
        if (entry->type == type) {
            break;
        }
        entry = entry->next;
    }
    return entry;
}

/* Return whether this group's separate ID list contains id. */
s32 btlGroupContainsId(s32 groupIndex, s32 wantedId) {
    BattleGroupIdEntry *idCursor = btlGroupIdHeads[groupIndex];
    while (idCursor != 0) {
        if (idCursor->id == wantedId) {
            return 1;
        }
        idCursor = idCursor->next;
    }
    return 0;
}

/* Prepend id to this group's ID list; duplicate IDs are allowed. */
void btlAddGroupId(s32 groupIndex, s32 newId) {
    BattleGroupIdEntry *idEntry = sdfAllocSizeClassBlock(sizeof(BattleGroupIdEntry));
    BattleGroupIdEntry **groupHead = &btlGroupIdHeads[groupIndex];
    idEntry->id = newId;
    idEntry->next = *groupHead;
    *groupHead = idEntry;
}

/* Remove the first matching ID using its incoming link, including the head. */
void btlRemoveGroupId(s32 groupIndex, s32 wantedId) {
    BattleGroupIdEntry **idLink;
    BattleGroupIdEntry *idCursor;

    idLink = &btlGroupIdHeads[groupIndex];
    idCursor = *idLink;
    if (idCursor == 0) {
        return;
    }
    do {
        if (idCursor->id == wantedId) {
            *idLink = idCursor->next;
            sdfReleaseChipBlock(idCursor);
            break;
        } else {
            idLink = &idCursor->next;
            idCursor = idCursor->next;
        }
    } while (idCursor != 0);
}

/* Replace the group/type node; only flags bit 0 selects resource ownership. */
void btlCreateGroupNode(s32 groupIndex, s32 entityType, s32 ownershipFlags, DevRequest *resourceList, void *itemList, s32 requestHandle) {
    BattleGroupNode *groupNode;
    BattleGroupNode *previousHead;
    s32 slotIndex;
    btlRemoveCurrentGroupedEntity(groupIndex, entityType);
    groupNode = sdfAllocSizeClassBlock(sizeof(BattleGroupNode));
    previousHead = btlGroupNodeHeads[groupIndex];
    if (previousHead != NULL) {
        previousHead->prev = groupNode;
    }
    btlGroupNodeHeads[groupIndex] = groupNode;
    groupNode->next = previousHead;
    groupNode->group = groupIndex;
    groupNode->type = entityType;
    groupNode->resourceList = resourceList;
    groupNode->itemList = itemList;
    groupNode->requestHandle = requestHandle;
    groupNode->prev = NULL;
    groupNode->modelContext = NULL;
    for (slotIndex = 0; slotIndex != BTL_GROUP_RESOURCE_SLOT_COUNT; slotIndex++) {
        groupNode->slots[slotIndex].flags = 0;
        groupNode->slots[slotIndex].data = NULL;
        groupNode->slots[slotIndex].resourceHandle = 0;
    }
    groupNode->ownsResources = ownershipFlags & BTL_GROUP_OWNS_RESOURCES_FLAG;
    groupNode->resourceHandle = 0;
    groupNode->partInfo = NULL;
    groupNode->partList = NULL;
    groupNode->unk_AC = 1.0f;
    groupNode->unk_B0 = 100.0f;
}

/* Capture ownership before clearing it for callbacks; keep the context-release
 * loop and resource-release order intact. NULL is allowed. */
void btlDestroyGroupNode(BattleGroupNode *groupNode) {
    BattleGroupNode *previousNode;
    BattleGroupNode *nextNode;
    u8 ownsResources;
    s32 slotIndex;
    if (groupNode == NULL) {
        return;
    }
    previousNode = groupNode->prev;
    nextNode = groupNode->next;
    if (previousNode == NULL) {
        btlGroupNodeHeads[groupNode->group] = nextNode;
    } else {
        previousNode->next = nextNode;
    }
    if (nextNode != NULL) {
        nextNode->prev = previousNode;
    }
    ownsResources = groupNode->ownsResources;
    groupNode->ownsResources = 0;
    if (groupNode->modelContext != NULL) {
        do {
            mdlDestroyContext(groupNode->modelContext);
        } while (groupNode->modelContext != NULL);
    }
    if (ownsResources != 0) {
        sdfResourceListRelease(groupNode->resourceList, 1);
        sdfQueueNonzeroResourceId((void *)groupNode->requestHandle);
        for (slotIndex = 0; slotIndex != BTL_GROUP_RESOURCE_SLOT_COUNT; slotIndex++) {
            if (groupNode->slots[slotIndex].resourceHandle != 0) {
                sdfReleaseResourceAllocation(groupNode->slots[slotIndex].resourceHandle);
            }
        }
    }
    mdlDestroyPartList(groupNode->partList);
    sdfReleaseResourceAllocation(groupNode->resourceHandle);
    sdfReleaseChipBlock(groupNode);
}

/* Forward the group/type pair explicitly, then destroy its node if present. */
void btlRemoveCurrentGroupedEntity(s32 groupIndex, s32 entityType) {
    BattleGroupNode *groupNode;

    groupNode = btlFindGroupedEntity(groupIndex, entityType);
    btlDestroyGroupNode(groupNode);
}

/* Destroy every group node, saving the next link before each unlink/free. */
void btlReleaseAllEntities(void) {
    u32 groupIndex = 0;
    BattleGroupNode **groupHead = btlGroupNodeHeads;
    do {
        BattleGroupNode *groupNode = *groupHead;
        while (groupNode != 0) {
            BattleGroupNode *nextNode = groupNode->next;
            btlDestroyGroupNode(groupNode);
            groupNode = nextNode;
        }
        groupIndex++;
        groupHead++;
    } while (groupIndex < BTL_GROUP_COUNT);
}


extern Motion *func_003340E0(SdfModel *, MotionTable *);

/* Create and attach the motion for the selected resource record. */
Motion *motionOwnerCreateObjectForRecord(MdlCtx *owner, s32 index) {
    MotionTable *resource = owner->sub->slots[index].data;
    s16 slot = owner->sub->slots[index].slot;
    Motion *object = func_003340E0(owner->inner, resource);

    object->searchId = index;
    owner->slots[slot] = object;
    object->slotIndex = slot;
    if (slot == 0) {
        owner->first = object;
    }
    return object;
}

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C300);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C310);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C320);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C330);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C340);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C350);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C360);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C370);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C380);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C390);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C3F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C400);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C410);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C420);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C430);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C440);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C450);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C460);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C470);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C480);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C490);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C4F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C500);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C510);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C520);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C530);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C540);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C550);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C560);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C570);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C580);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C590);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C5F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C600);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C610);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C620);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C630);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C640);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C650);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C660);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C670);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C680);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C690);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C6F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C700);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C710);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C720);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C730);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C740);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C750);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C760);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C770);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C780);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C790);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C7F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C800);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C810);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C820);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C830);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C840);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C850);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C860);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C870);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C880);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C890);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C8F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C900);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C910);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C920);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C930);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C940);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C950);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C960);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C970);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C980);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C990);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041C9F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CA90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CAF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CB90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CBF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CC90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CCF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CD90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CDF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CE90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CEA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CEB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CEC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CED0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CEE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CEF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CF90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041CFF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D000);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D010);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D020);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D030);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D040);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D050);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D060);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D070);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D080);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D090);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D0F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D100);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D110);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D120);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D130);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D140);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D150);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D160);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D170);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D180);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D190);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D1F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D200);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D210);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D220);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D230);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D240);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D250);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D260);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D270);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D280);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D290);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D2F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D300);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D310);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D320);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D330);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D340);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D350);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D360);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D370);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D380);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D390);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D3F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D400);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D410);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D420);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D430);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D440);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D450);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D460);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D470);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D480);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D490);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D4F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D500);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D510);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D520);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D530);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D540);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D550);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D560);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D570);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D580);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D590);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D5F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D600);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D610);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D620);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D630);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D640);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D650);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D660);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D670);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D680);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D690);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D6F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D700);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D710);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D720);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D730);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D740);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D750);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D760);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D770);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D780);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D798);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D7B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D7C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D7E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D7F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D810);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D828);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D840);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D858);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D870);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D888);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D8A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D8B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D8D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D8E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D900);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D918);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D930);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D948);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D960);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D978);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D990);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D9A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D9C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D9D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041D9F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA08);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA38);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA68);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DA90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DAF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DB90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DBF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DC90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DCF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DD90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DDF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DE90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DEA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DEB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DEC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DED0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DEE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DEF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DF90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041DFF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E000);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E010);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E020);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E030);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E040);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E050);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E060);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E070);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E080);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E090);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E0F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E100);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E110);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E120);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E130);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E140);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E150);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E160);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E170);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E180);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E190);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E1F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E200);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E210);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E220);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E230);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E240);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E250);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E260);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E270);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E280);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E290);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E2F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E300);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E310);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E320);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E330);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E340);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E350);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E360);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E370);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E380);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E390);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E3F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E400);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E410);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E420);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E430);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E440);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E450);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E460);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E470);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E480);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E490);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E4F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E500);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E510);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E520);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E530);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E540);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E550);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E560);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E570);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E580);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E590);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E5F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E600);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E610);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E620);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E630);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E640);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E650);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E660);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E670);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E680);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E690);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E6F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E700);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E710);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E720);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E730);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E740);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E750);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E760);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E770);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E780);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E790);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E7F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E800);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E810);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E820);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E830);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E840);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E850);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E860);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E870);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E880);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E890);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E8F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E900);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E910);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E920);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E930);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E940);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E950);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E960);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E970);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E980);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E990);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041E9F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EA90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EAF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EB90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EBF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EC90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ECF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041ED90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EDF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EE90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EEA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EEB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EEC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EED0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EEE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EEF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EF90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041EFF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F000);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F010);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F020);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F030);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F040);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F050);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F060);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F070);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F080);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F098);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F0B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F0C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F0D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F0F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F108);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F120);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F138);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F158);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F170);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F188);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F1A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F1B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F1D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F1E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F200);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F218);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F230);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F248);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F260);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F270);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F288);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F2A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F2C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F2D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F2F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F310);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F328);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F340);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F358);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F370);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F388);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F3A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F3B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F3D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F3E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F400);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F418);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F428);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F440);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F458);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F470);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F488);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F4A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F4B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F4D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F4E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F4F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F508);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F518);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F528);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F538);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F550);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F568);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F580);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F5A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F5C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F5F8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F620);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F648);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F670);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F698);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F6C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F6E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F710);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F738);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F760);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F788);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F7B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F7D8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F800);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F828);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F850);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F878);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F8A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F8C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F8F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F918);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F940);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F968);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F990);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F9B8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041F9E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FA08);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FA30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FA50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FA78);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FAA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FAC8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FAF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FB18);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FB40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FB68);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FB90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FBB8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FBE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FC08);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FC30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FC58);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FC80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FCA8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FCD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FCF8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FD20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FD48);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FD70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FD98);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FDC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FDE8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FE10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FE38);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FE60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FE88);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FEB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FED8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FF00);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FF28);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FF50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FF78);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FFA0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FFC8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_0041FFE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420000);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420020);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420040);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420060);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420080);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004200A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004200C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004200E0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420100);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420120);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420140);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420168);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420188);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004201A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004201C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004201E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420208);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420228);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420248);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420268);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420288);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004202A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004202C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004202E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420308);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420328);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420348);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420368);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420388);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004203A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004203C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004203E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420408);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420428);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420448);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420468);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420488);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004204A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004204C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004204E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420508);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420528);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420548);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420568);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420588);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004205A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004205C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004205E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420608);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420628);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420648);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420668);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420688);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004206A8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004206C8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004206E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420708);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420728);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420750);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420770);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420790);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004207B0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004207D0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004207F0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420810);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420830);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420850);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420870);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420898);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004208C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004208E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420910);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420930);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420958);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420978);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004209A0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004209C0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_004209E8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420A10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420A38);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420A58);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420A80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420AA8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420AD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420AF8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420B18);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420B40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420B68);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420B90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420BB8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420BE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420C08);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420C30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420C58);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420C80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420CA8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420CD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420CF8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420D18);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420D38);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420D60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420D80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420DA8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420DD0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420DF0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420E10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420E30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420E50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420E78);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420E98);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420EB0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420EC8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420EE0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420EF8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F10);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F20);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F30);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F40);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F50);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F60);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F70);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F80);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420F90);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420FA8);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420FC0);

INCLUDE_RODATA(const s32, "game/code_0022AC10", D_00420FD8);

