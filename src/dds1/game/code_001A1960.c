#include "common.h"
#include "pcp_vu0.h"

#define VU_LOAD10(p) __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

#define VU_STORE10(p) __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

typedef struct ActorEntrySlot {
    s16 code;
    s16 unk02;
    s16 countdown;
} ActorEntrySlot;

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u32 actionFlags;
    u8 unk_118[8];
    u16 entryMask;
    u8 entryDataTail[2];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
    u8 unk_130[6];
    u8 unk_136[0x18E];
    u8 kind;
    u8 pad_2C5;
    ActorEntrySlot entrySlots[7];
    s32 selectedEntryIndex;
    u32 marker;
    u8 pad_2F8[0x4C];
    struct UiObject *next;
} UiObject;

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

typedef struct SceneTask {
    s32 state;
    u8 pad04[4];
    u32 flags;
    u8 pad0C[0xC];
    UiObject *actor;
} SceneTask;

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x30];
    UiObject *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x28];
    s32 mode;
    u8 pad_280[0x1C];
    s32 taskParent;
    u8 pad_2A0[0xC];
    s32 spriteObject;
    u8 pad_2B0[0x24];
    SceneSlot slots[8];
    SceneTask *groupPrimary[20];
    SceneTask *groupSecondary[45];
    SceneTask *groupTertiary[15];
    u8 pad_42C[0x184];
    s32 (*sceneCallback)();
} BattleController;

typedef struct BtlEntry {
    u16 flags;
    u8 pad2[4];
    u16 hp;
    u8 pad8[2];
    u16 mp;
    u8 padC[2];
    u16 status;
    u8 pad10[4];
    u16 unk14;
    u8 unk16[5];
    u8 pad1B[0x171];
    u16 unk18C;
    u16 unk18E;
    u16 unk190;
} BtlEntry;

typedef struct EntryPair {
    s16 first;
    s16 second;
    u8 pad4[4];
} EntryPair;

extern EntryPair D_003583D0[];

extern u8 D_0035F100[];

extern s32 D_0035D9F0[];

extern s32 D_0035DA08[];

extern void func_001E9DE0(s32, s32, s32);

extern void func_001EB368(s32, s32);

extern s32 btlCountTasksForOwner(s64);

typedef struct BtlSlotRecord {
    u8 pad_00[0x84];
    u32 word[7];
} BtlSlotRecord;

typedef struct BtlSlotOwner {
    u8 pad_00[0x18];
    BtlSlotRecord *records;
} BtlSlotOwner;

typedef struct SoundBankEntry {
    u32 unk_00;
    u32 resource;
    u32 unk_08;
} SoundBankEntry;

extern SoundBankEntry D_0035F748[];

typedef struct SoundEffectNode {
    u32 flags;
    u8 pad4[0xC];
    u32 handle;
} SoundEffectNode;

typedef struct EffectLoadArgs {
    SoundEffectNode *effect;
    void *loadHandle;
    const char *name;
} EffectLoadArgs;

extern s32 btlUpdateUnitPositionInterpolationTask(u32 *);

extern void btlUnitGetMuzzlePosVU(void *);

extern s32 btlStepUnitRotationNlerp(u32 *);

extern s32 func_001190B0();

extern s32 btlGetEffectActive(void);

extern s32 func_001191B0();

extern void func_001BCB88(s32, s32);

extern s32 func_00119368(s32, s32);

extern u64 btlAdvanceRuntimeSequenceCounter();

extern u8 *btlAllocateIndexedUnitEffectTask(u8 *, s32, s32, f32);

extern void btlUpdateScene(void);

typedef struct BtlUnit {
    u8 pad_00[0x108];
    s64 owner;
    u32 flags;
    u32 stateFlags;
    u32 gunResourceFlags;
    s8 lookupId;
    u8 pad_11D[0x1A7];
    s8 unk2C4;
    u8 pad_2C5[0x2B];
    s32 unk2F0;
    u8 pad_2F4[4];
    s32 resourceNode;
    s32 resourceLink;
    s32 link;
    s32 listNode;
    u8 pad_308[4];
    void *gunResource;
    u8 pad_310[4];
    s32 unk314;
    u8 pad_318[4];
    s32 effectObject;
    s32 ext;
    u8 pad_324[8];
    s32 unk32C;
    s32 unk330;
    u8 pad_334[8];
    u32 handle;
    struct BtlUnit *previousActor;
    struct BtlUnit *nextActor;
} BtlUnit;

typedef struct BtlActorWork {
    u8 pad_00[0x228];
    BtlUnit *actorList;
} BtlActorWork;

typedef struct SoundTask {
    u8 enabled;
    u8 unk_01[0xF];
    u8 status;
    u8 unk_11[0xF];
    u16 taskId;
    u8 unk_22[2];
    u16 flags;
    u8 unk_26[0xA];
    u32 unk_30;
    u32 unk_34;
    u64 unk_38;
    u64 owner;
    void (*onStart)(u32);
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*process)(void);
        u32 (*playCustomSound)(u8 *);
        s32 (*releaseSound)(u16 *);
        s32 (*acquireSound)(u32 *);
    } callback;
    void (*onFinish)(u32 *);
    void *args;
    struct SoundTask *next;
    struct SoundTask *nextActive;
    struct SoundTask *deferNext;
    struct SoundTask *deferPrev;
} SoundTask;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    s32 fadeCountdown;
    u32 resourceHandle;
    u32 unk_14;
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

typedef struct SoundLink {
    void *owner;
    void *sound;
    void *task;
    u16 variant;
    u16 unk_0E;
} SoundLink;

typedef struct SoundResourceLink {
    void *owner;
    void *sound;
    void *task;
    u32 variant;
    u32 unk_10;
} SoundResourceLink;

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
} SndPad;

extern SndPad D_00324510;

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern char D_003A5158[]; /* "%sMIDI%04X.SMG" */

extern s32 sndFindPackedTrackLoadStatus(u32);

extern s64 func_001F0998(void);

extern s32 func_001F06E0();

extern s64 func_001F0B90(void);

extern s32 sndPlaySkillSeTask(u32 *);

extern void sndFormatResourceNameFromUnitMode(s32, s32);

extern u32 sndFinishEarringPlayback(void);

extern s32 sndTickFadeCounter();

extern s32 btlQueueTintTransitionWhenEnabled(u32 *);

extern void *sdfAllocAndClearQuadwords(s32);

extern s32 btlCreateMoveOtherUnitsTask();

extern s32 mnuPollTitleStreamStateLocked(void);

extern SoundResourceNode *sndAllocResourceNode(void);

extern u32 kwlnDrawControlFlags;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_003BB694;

extern u32 btlTintTransitionHoldCount;

extern s8 D_00324530[];

extern u8 D_003583A0[];

extern void *D_00358408[];

extern void *D_00358450[];

extern s32 D_00358510[];

extern s32 D_00359A78[];

extern s32 D_00359A90[];

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern u64 dds3AdvanceWorldCounter(void);

extern u32 evtSpawnActionObj9(u64);

extern s32 btlSetActorEffectParameter();

extern s32 mdlFlagTest(u32);

extern void func_00215FE0(s32);

extern SoundTask *btlDeferredTaskTail;

extern SoundTask *btlDeferredTaskHead;

extern s32 func_00214868(void);

u8 *fldCreateSceneGroupAction(u8 *, u32, s32);

extern s32 datEnemyRecords;

extern u32 fldGetSceneScriptTaskUserData(void);

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern s32 btlTrackedTaskHandles;

extern u32 D_003BB3DC;

extern u32 D_003BD834;

extern u32 D_003BD838;

extern u32 btlLinkedSelectionTaskBuffer;

extern u32 D_003BB3A8;

extern u32 btlCommandPanelTaskNameRef;

extern u32 D_003BB3B0;

extern u32 D_003BB3AC;

extern u32 D_003BB3BC;

extern u32 D_003BB3C4;

extern u32 btlAnalyzPanelTaskNameRef;

extern u32 kwlnTaskGetUserValue(s64);

extern u32 btlMahenPanelTaskNameRef;

extern s64 kwlnTaskGetTaskByName(u32);

extern s64 kwlnTaskIsRegistered(s64);

extern s32 btlGetTrackedTaskHandle(s32);

extern u32 btlCommandPanelWork;

extern s32 func_001A8DD8(s32, s32 *);

extern s32 btlCheckSpecialAbility(s32, s32);

extern void func_001DEFE0(s32, s32, f32);

extern void func_001B83D8(s32, s32, s32);

extern void btlApplyScaledUnitEffectParameter(u8 *, s32, s32, f32);

extern s32 btlGetRuntime(void);

extern u8 *datBattleSceneRecords;

extern s32 func_00197760(s32, s32, s32, s32, s32, s32);

extern char D_003BB450[];

extern s32 datItemSkillRecords;

extern s32 D_003BAA14;

extern s32 D_003BAA28;

extern s32 D_003BAA10;

extern s32 D_003BAA20;

extern s32 D_003BAA30;

extern s32 datCommandSelectors;

extern s32 datCommandRecords;

extern s32 datGameState;

extern s32 datAffinityRecords;

extern s32 datActionAnimationRecords;

extern s32 dds3FindEntryIndex();

extern s32 btlGetIndexedPartyEntryRecord(s32);

extern s32 func_001F5028(s32 arg0);

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern s8 effSharedRandomState[];

extern s32 func_002D03F8(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern void sndResetTransition(void);

extern void func_001F4430(void);

extern void btlResetDeferredTaskQueue(void);

extern void btlResetToInitialScene(void);

extern void fldClearSceneSlotsAndGroups(void);

extern void btlResetFieldColorAndSweepFlags(void);

extern void btlRefreshSoundEntries(void);

extern void func_001C45F0(void);

extern void itfMesDestroyWindowIfPresent(s32);

void func_001A1868(u8 *object, s32 value);

void func_001A1880(u8 *object, s32 value);

u16 btlRefreshUnitMaximumHpAndClampCurrentHp(s32 object);

u16 btlRefreshUnitMaximumMpAndClampCurrentMp(s32 object);

void func_001A1948();

void func_001A1960() {
    datClearUnitStatusBits();
}

void btlSetActorSelectedEntryIndex(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2f0) = arg1;
}

void btlClearActorSelectedEntryIndex(s32 arg0) {
    *(u32 *)(arg0 + 0x2f0) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A1990);

s32 btlGetActorEntryData(s32 arg0) {
    s32 temp_v0;

    if ((*(u32 *)(arg0 + 0x110) & 0x400) == 0) {
        temp_v0 = btlGetIndexedPartyEntryRecord(*(u8 *)(arg0 + 0x2c4));
        return temp_v0;
    }
    return arg0 + 0x120;
}

s32 btlGetCurrentPartyEntryRecord(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return datGameState + temp_v0 * 0x1a4 + 0xa60;
}

s32 btlGetIndexedPartyEntryRecord(s32 arg0) {
    return datGameState + arg0 * 0x1a4 + 0xa60;
}

void btlSyncPlayerWork(UiObject *actor) {
    BtlEntry *src = (BtlEntry *)&actor->entryMask;
    BtlEntry *dst = (BtlEntry *)btlGetIndexedPartyEntryRecord(actor->kind);
    s32 maxHp;
    s32 maxMp;
    if (src->flags & 0x1000) {
        dst->flags |= 0x1000;
    } else {
        dst->flags &= ~0x1000;
    }
    if (src->flags & 0x4000) {
        dst->flags |= 0x4000;
    } else {
        dst->flags &= ~0x4000;
    }
    dst->unk14 = src->unk14;
    maxHp = func_001190B0(dst);
    maxMp = func_001191B0(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->unk16, src->unk16, 5);
    dst->status = src->status & 0x7FFF;
    dst->unk18C = src->unk18C;
    dst->unk18E = src->unk18E;
    dst->unk190 = src->unk190;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(s32 arg0) {
    return dds3FindEntryIndex(*(u16 *)(arg0 + 0x124));
}

void func_001A1CD0(void) {
}

s32 btlFindActiveActorByKind(s32 index) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (index == *(u8 *)(node + 0x2C4)) {
                    return node;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A1D48);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2258);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2608);

s32 btlGetEntryFlagsUnlessDisabled(s32 entry) {
    if ((*(u16 *)entry & 4) != 0) {
        return 0;
    }
    return *(s32 *)(datEnemyRecords + *(u16 *)(entry + 4) * 76);
}

void func_001A29B8(void) {
    func_00119300();
}

u32 func_001A29D0(s32 arg0, s32 arg1) {
    return func_00119368(arg0, arg1);
}

extern s32 datAbilityParameters;

s32 btlApplyCommandAbilityMultiplier(s32 arg0, s32 arg1) {
    u32 value = func_0011A158(arg0, arg1);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (*(u8 *)(datCommandRecords + arg1 * 56 + 3)) {
    case 1:
        if (btlCheckSpecialAbility(arg0, 0x234)) {
            scale = *(f32 *)(datAbilityParameters + 0x1A0);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(arg0, 0x235)) {
            scale = *(f32 *)(datAbilityParameters + 0x1A8);
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s8 func_001A2AE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = datAffinityRecords + arg0 * 0x10;
    return *(s8 *)(temp_v0 - 0x1aa4);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2B00);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2CC0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A2DE8);

s8 btlGetActorIndexedSignedValue(s32 object, s32 index) {
    if (index == 0 && (*(u32 *)(object + 0x110) & 0x400) != 0) {
        return *(s8 *)(datEnemyRecords + *(u16 *)(object + 0x124) * 76 + 0x46);
    }
    return *(s8 *)(datCommandSelectors + index * 2);
}

void func_001A2F50(s32 arg0) {
    btlResolveUnitValueWithOverride(arg0 + 0x120);
}

s32 btlResolveUnitValueWithOverride(s32 object, s32 value) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x670);
    if (handler != 0) {
        s32 result = handler(object, value);
        if (result != -1) {
            return result;
        }
    }
    return func_00119520(object, value);
}

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA10 + arg1 * 0x270;
    }
    return D_003BAA20 + arg1 * 0x270;
}

s32 btlSelectSharedOrIndexedTransformParameters(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003583A0;
    }
    return D_003BAA30 + arg1 * 24;
}

s32 btlSelectSideIndexedActorParameterTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA14 + arg1 * 0x74;
    }
    return D_003BAA28 + arg1 * 0x74;
}

extern char D_003A1788[];

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = *(u16 *)(datItemSkillRecords + index * 8 + 2);
    btlBossDebugPrintf(D_003A1788, index, item);
    return item;
}

s32 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return *(u16 *)(arg0 * 8 + datItemSkillRecords + 2);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1788);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A30F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3360);

s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, s32 arg1) {
    u32 count;
    u32 i;
    s32 cmd;
    s32 index;

    if (arg0 == 0) {
        goto fail;
    }
    if (arg1 == 0) {
        goto fail;
    }
    count = btlGetIndexListCount(arg1);
    if (count < 2) {
        return 0;
    }
    cmd = *(s32 *)(arg0 + 0x20);
    if (cmd < 2) {
        goto fail;
    }
    if (cmd >= 5) {
        if (cmd > 8) {
            goto fail;
        }
        if (cmd < 7) {
            goto fail;
        }
    }
    if (cmd == 4) {
        index = btlGetLoggedIndexedCommandItem(*(s32 *)(arg0 + 0x28));
    } else {
        index = *(s32 *)(arg0 + 0x24);
    }
    if (*(u8 *)(datCommandRecords + index * 56 + 8) != 0) {
        goto fail;
    }
    if (*(u8 *)(datCommandRecords + index * 56 + 0x24) != 2) {
        goto fail;
    }
    if (*(u16 *)(datCommandRecords + index * 56 + 0x26) == 0) {
        goto fail;
    }
    for (i = 0; i < count; i++) {
        if ((*(u16 *)(datCommandRecords + index * 56 + 0x26) &
             *(u16 *)(btlGetIndexListEntry(arg1, i) + 0x12E)) != 0) {
            return i;
        }
    }
fail:
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3638);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3740);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A3CE0);

u32 btlEncodeActorIndexAsSelectionMask(u32 id) {
    u32 mask;
    switch (id) {
    case 0xFFFFFFFF: mask = 0x1; break;
    case 0: mask = 0x2; break;
    case 1: mask = 0x4; break;
    case 2: mask = 0x8; break;
    case 3: mask = 0x10; break;
    case 4: mask = 0x20; break;
    case 5: mask = 0x40; break;
    case 6: mask = 0x80; break;
    case 7: mask = 0x100; break;
    case 8: mask = 0x200; break;
    case 9: mask = 0x400; break;
    case 10: mask = 0x800; break;
    case 11: mask = 0x1000; break;
    case 12: mask = 0x2000; break;
    case 13: mask = 0x4000; break;
    case 14: mask = 0x8000; break;
    case 15: mask = 0x10000; break;
    case 16: mask = 0x20000; break;
    case 17: mask = 0x40000; break;
    case 18: mask = 0x80000; break;
    default: mask = 0; break;
    }
    return mask;
}

void func_001A4060(void) {
    func_00119750();
}

s32 btlCheckSpecialAbility(s32 object, s32 flag) {
    if (datUnitHasSkill(object, flag) == 0) {
        return 0;
    }
    switch (flag) {
    case 0x207:
        return evtGetMirroredSolarPhase() == 8;
    case 0x208:
        return evtGetMirroredSolarPhase() == 0;
    default:
        return 1;
    }
}

extern s8 effSharedRandomState[];

s32 btlSelectActorAction(s32 object) {
    s32 result = func_001A4130(object, 1);
    if (result == 0) {
        result = (effMiscRand(effSharedRandomState) & 1) != 0 ? 2 : 7;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4130);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4240);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4328);

s32 btlAllActiveUnitsReady(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 0xC0) != 0) {
                    return 0;
                }
                if ((flags & 0x20) != 0) {
                    if ((*(u32 *)(node + 0x114) & 1) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

extern s32 D_003BAA3C;

extern s32 datBattleParameters;

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4598);

u32 func_001A4630(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x254);
}

s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    s32 node = *(s32 *)(btlGetRuntime() + 0x224);
    for (; node != 0; node = *(s32 *)(node + 0x16C)) {
        if ((*(u32 *)(node + 8) & 8) != 0) {
            s32 actor = *(s32 *)(node + 0x18);
            u32 flags = *(u32 *)(actor + 0x110);
            if ((flags & 1) != 0) {
                if ((flags & 0x200) != 0) {
                    if ((flags & 2) != 0) {
                        if ((flags & 0xE0) == 0) {
                            candidates[count++] = node;
                        }
                    }
                }
            }
        }
    }
    if (count != 0) {
        return candidates[effMiscRandMod(0, count)];
    }
    return 0;
}

s32 btlCountAvailableUnits(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 btlCountAvailableParticipants(void) {
    s32 count = 0;
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    {
        u8 *entry = (u8 *)(datGameState + 0xA60);
        s32 i;
        for (i = 4; i >= 0; i--, entry += 0x1A4) {
            u16 flags = *(u16 *)entry;
            if ((flags & 1) != 0) {
                if ((flags & 2) == 0) {
                    if ((*(u16 *)(entry + 0xE) & 0x4000) == 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A47F0);

void btlClearAllActorEntrySlots(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

s32 btlActorEntryIsExpired(UiObject *unit, s32 index) {
    if (unit->entrySlots[index].code == 0) {
        return 0;
    }
    return unit->entrySlots[index].countdown < 1;
}

s32 btlMatchActorEntryCode(UiObject *unit, s32 index) {
    s16 value = unit->entrySlots[index].code;
    if (D_003583D0[index].first != 0) {
        if (D_003583D0[index].first == value) {
            return 1;
        }
    }
    if (D_003583D0[index].second != 0) {
        if (D_003583D0[index].second == value) {
            return 2;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4948);

void btlSetActorEntryCode(s32 arg0, s32 arg1, u16 arg2) {
    *(u16 *)(arg1 * 6 + arg0 + 0x2c6) = arg2;
}

void btlClearActorEntrySlot(UiObject *unit, s32 index) {
    unit->entrySlots[index].code = 0;
    unit->entrySlots[index].unk02 = -1;
    unit->entrySlots[index].countdown = -1;
}

s16 btlGetActorEntryCode(s32 arg0, s32 arg1) {
    return *(s16 *)(arg1 * 6 + arg0 + 0x2c6);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4A30);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A4C68);

INCLUDE_ASM(const s32, "game/code_001A1960", btlLowestSetPairIndex);

void btlTickActorEntryCountdowns(u8 *scene) {
    u32 index;
    s16 *timer = (s16 *)(scene + 0x2CA);
    for (index = 0; index < 7; index++, timer += 3) {
        if (*timer >= 0) {
            if (*timer == 0) {
                *timer = -1;
            }
            --*timer;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5030);

extern s32 datAbilityParameters;

s32 btlGetAbilityAttributeMultiplierPercent(u8 *actor, s32 attr) {
    u32 value = 100;

    switch (attr) {
    case 0:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23C)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x1E0) * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23D)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x1E8) * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23E)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x1F0) * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x23F)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x1F8) * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x240)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x200) * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x241)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x208) * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x242)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x210) * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x243)) {
            value = (u32)(*(f32 *)(datAbilityParameters + 0x218) * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (slot == 0 && btlCheckSpecialAbility(unit + 0x120, 0x24F) != 0) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x251) != 0) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x252) != 0) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(unit + 0x120, 0x244) != 0) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(unit + 0x120, 0x245) != 0) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(unit + 0x120, 0x246) != 0) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(unit + 0x120, 0x247) != 0) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(unit + 0x120, 0x248) != 0) {
        return 1;
    }
    if (slot < 0xF) {
        if (slot >= 0xA && btlCheckSpecialAbility(unit + 0x120, 0x253) != 0) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x257) != 0) {
            return 1;
        }
    }
    if (slot != 7 && btlCheckSpecialAbility(unit + 0x120, 0x249) != 0) {
        return 1;
    }
    return 0;
}

u32 btlHasSpecialAbilityWhenArgumentUnset(s32 arg0, s64 arg1) {
    if (arg1 == 0 && btlCheckSpecialAbility(arg0 + 0x120, 0x254) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(s32 unit, u32 slot) {
    if (slot == 8 && btlCheckSpecialAbility(unit + 0x120, 0x255)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(unit + 0x120, 0x256)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(unit + 0x120, 0x258)) {
            return 1;
        }
    }
    return 0;
}

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(datCommandSelectors + index * 2);
    if (resource < 0) {
        return 0;
    }
    return (s32)D_00358408[resource];
}

void *btlGetIndexedUiResource(s32 arg0) {
    return D_00358450[*(u16 *)(arg0 + 0x124)];
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1918);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5690);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A57A0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5958);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A5C40);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6118);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6570);

s32 btlTestActorStatusPredicate(s32 object) {
    s32 (*predicate)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x650);
    if (predicate != 0 && predicate(object) != 0) {
        return 1;
    }
    return (*(u16 *)(object + 0x12E) & 0x806) != 0;
}

s32 btlComputeStatusPenaltyFifth(s32 object) {
    s32 result = 0;
    u16 category = *(u16 *)(object + 0x12E) & 0x7FFF;
    switch (category) {
    case 0x80:
    case 0x400:
    case 0x2000:
        result = -*(u16 *)(object + 0x128) / 5;
        break;
    }
    return result;
}

s32 btlRollFearChance(s32 unused, u8 *actor, u32 flags, u32 options) {
    s32 ratio;
    if ((*(u32 *)(btlGetRuntime() + 0x1FC) & 0x80) != 0) return 0;
    if ((*(u32 *)(actor + 0x114) & 8) != 0) return 0;
    if ((flags & 1) == 0) return 0;
    if ((*(u16 *)(actor + 0x12E) & 1) != 0) return 0;
    ratio = 0;
    if (options & 2) {
        ratio = 30;
    } else if (options & 4) {
        ratio = 40;
    }
    btlBossDebugPrintf("btl:fear ratio[%d]\n", ratio);
    return btlRollAiBucket() < ratio;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6828);

f32 func_001A6958(void) {
    return 1.5f;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6968);

u8 btlGetActorDisplayByteWithDefault(s32 object, s32 index) {
    if (index == 0) {
        if ((*(u32 *)(object + 0x110) & 0x400) != 0) {
            return *(u8 *)(datEnemyRecords + *(u16 *)(object + 0x124) * 76 + 0x48);
        }
        return 12;
    }
    return 12;
}

extern u8 D_00358490[];

extern char D_003A1A58[]; /* "btl:delay=%d\n" */

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6AA0);

s32 btlResolveSkillCategory(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x17C:
    case 0x17F:
        return 0x37;
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    default:
        return *(s8 *)(datCommandSelectors + id * 2 + 1) == 2 ? 0x2D : 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6BE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6EC0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A6F98);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7180);

s32 btlQueryUnitChannelFlags(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, first + 0x120, second + 0x120);
    if ((*(u16 *)(second + 0x12E) & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7410);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7548);

void func_001A7AD0(void) {
}

extern f32 func_001A7C20(u8 *, u8 *, s32);

s32 btlCalculateEnemyExperienceReward(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    u8 *entry;
    f32 ratio;
    u32 ep;

    if (!(*(u32 *)(enemy + 0x110) & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(*(u32 *)(acquirer + 0x110) & 0x200)) {
        return result;
    }
    entry = (u8 *)(datEnemyRecords + *(u16 *)(enemy + 0x124) * 76);
    ratio = func_001A7C20(acquirer, enemy, 1);
    ep = (u32)((f32)*(u16 *)(entry + 0x2E) * ratio);
    if (*(u32 *)entry & 0x2000) {
        ep *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f]\n", ep, *(u16 *)(entry + 0x2E), ratio);
    } else {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f](acquisition)\n", ep, *(u16 *)(entry + 0x2E), ratio);
    }
    return ep;
}

extern u8 *datBattleSceneRecords;

extern char D_003A1B90[];

extern char D_003A1BA8[];

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A7C20);

s32 btlGetEnemyMoney(u8 *acquirer, u8 *enemy) {
    s32 result = 0;
    s32 money;
    u8 *entry;
    if (!(*(u32 *)(enemy + 0x110) & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(*(u32 *)(acquirer + 0x110) & 0x200)) {
        return result;
    }
    entry = (u8 *)(datEnemyRecords + *(u16 *)(enemy + 0x124) * 0x4C);
    money = *(s32 *)(entry + 0x28);
    if (*(u32 *)entry & 0x2000) {
        money *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:money=%d\n", money, acquirer);
    } else {
        btlBossDebugPrintf("btl:money=%d(acquisition)\n", money);
    }
    return money;
}

extern f32 func_001A7C20(u8 *, u8 *, s32);

s32 btlCalculateHuntEpReward(u8 *arg0, u8 *arg1) {
    u8 *entry = (u8 *)(datEnemyRecords + *(u16 *)(arg1 + 0x124) * 76);
    f32 ratio = func_001A7C20(arg0, arg1, 0);
    u32 ep = (u32)((f32)*(u16 *)(entry + 0x30) * ratio);
    if (*(u32 *)entry & 0x2000) {
        ep *= 100;
    }
    btlBossDebugPrintf("btl:ep=%d[%d,%.3f](hunt)\n", ep, *(u16 *)(entry + 0x30), ratio);
    return ep;
}

u32 func_001A7ED8(void) {
    return 0;
}

extern char D_003A1C10[]; /* "btl:hunt mp rec[%d]\n" */

extern s32 datAbilityParameters;

s32 btlCalculateAbilityRecoveryAmount(u8 *actor) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x24E)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * *(f32 *)(datAbilityParameters + 0x270));
    } else if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x229)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * *(f32 *)(datAbilityParameters + 0x148));
    }
    btlBossDebugPrintf(D_003A1C10, recovery);
    return recovery;
}

s32 btlIsUnitDefeatTriggeredByValueDelta(u8 *actor, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled((s32)(actor + 0x120)) & 4) return 0;
    if (btlHasEnemyRecordDefeatExemptionFlag((s32)actor)) return 0;
    if ((*(u16 *)(actor + 0x12E) & 0x7FFF) == 0x4000) return 1;
    if ((*(u32 *)(btlGetRuntime() + 0x1F4) & 0x80) == 0) return 0;
    return *(u16 *)(actor + 0x126) + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(UiObject *object) {
    return object->currentValue * 100 / object->maximumValue < 25;
}

s32 btlWouldUiValueFallBelowQuarter(UiObject *object, s32 delta) {
    s32 value = object->currentValue + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->maximumValue < 25;
}

s32 btlBothSidesActive(UiObject *unit) {
    UiObject *actor;
    s32 a;
    s32 b;
    if (btlIsUnitDefeatTriggeredByValueDelta((u8 *)unit, 0) != 0) {
        return 0;
    }
    if (unit->flags & 0x60) {
        return 0;
    }
    a = 0;
    b = 0;
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->next) {
        if (actor->flags & 1) {
            if (!(actor->flags & 0xE0)) {
                if (actor->flags & 0x200) {
                    a++;
                }
                if (actor->flags & 0x400) {
                    b++;
                }
            }
        }
    }
    if (a != 0 && b != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsUiObjectIndexAllowed(UiObject *object) {
    if ((object->flags & 0x400) != 0) {
        if (object->index >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 btlIsUnitStatusFlagClear(UiObject *object) {
    if ((object->statusFlags & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1C10);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8188);

extern s32 D_00358514[];

extern s32 D_00358518[];

s32 btlSelectedEntryHitsElement(s32 arg0, UiObject *unit, s32 arg2) {
    u32 kind;
    s32 mask;
    u32 power;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    kind = btlGetActorIndexedSignedValue(arg0, arg2);
    mask = btlEncodeActorIndexAsSelectionMask(kind);
    power = *(u16 *)(unit->selectedEntryIndex * 0x38 + datCommandRecords + 0x2E);
    if (power == 0) {
        return 0;
    }
    if (kind >= 0x10 && (kind < 0x12 || kind == -1)) {
        return 0;
    }
    if (power >= 0x20) {
        return 0;
    }
    return (D_00358518[power * 3] & mask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(datCommandRecords + arg0 * 56 + 0x2e);
    return D_00358510[temp_v0 * 3];
}

s32 btlTestSelectedItemCategoryMask(s32 object, s32 mask) {
    s32 index = *(s32 *)(object + 0x2F0);
    u16 item;
    if (index == -1) {
        return 0;
    }
    item = *(u16 *)(datCommandRecords + index * 56 + 0x2E);
    return (D_00358518[item * 3] & btlEncodeActorIndexAsSelectionMask(mask)) != 0;
}

s32 fldGetSelectedUnitStat(s32 object) {
    s32 item = *(s32 *)(object + 0x2F0);
    if (item == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(item);
}

s32 btlGetSelectedUnitProperty(s32 object) {
    s32 index = *(s32 *)(object + 0x2F0);
    if (index == -1) {
        return 0;
    }
    return D_00358514[*(u16 *)(datCommandRecords + index * 56 + 0x2E) * 3];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8538);

extern u8 *datBattleSceneRecords;

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8640);

extern u8 *datBattleSceneRecords;

extern u8 *datEnemyAiRecords;

s32 btlIsSelectedActorStatusAndRecordClear(s32 object) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(s8 *)(datBattleSceneRecords + index * 40) != 0) {
        return 0;
    }
    if ((*(u16 *)(object + 0x12E) & 0x2A0F) != 0) {
        return 0;
    }
    return datEnemyAiRecords[*(u16 *)(object + 0x124) * 0x15C] == 0;
}

s32 btlAreUnitStatusAndEntryFlagsClear(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(arg0 + 0x120) & 0x40) < 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8850);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8A30);

s32 btlHasAdjacentActorRecordStatus(s32 object) {
    u8 *status = (u8 *)(datEnemyRecords + *(u16 *)(object + 0x124) * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

u32 btlIsActorHighStateFlagClear(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8CE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A8DD8);

u8 btlHasAvailableOption(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001A8DD8(arg0, 0);
    return temp_v0 != 0;
}

s32 btlChooseRandomAvailableOption(s32 object) {
    s32 choices[24];
    s32 count = func_001A8DD8(object, choices);
    if (count != 0) {
        return choices[effMiscRandMod(0, count)];
    }
    return -1;
}

s32 btlChooseEligibleSkill(s32 object) {
    s32 choices[24];
    s32 count = 0;
    u32 i;
    u16 *ids = (u16 *)(object + 0x142);
    for (i = 0; i < 24; i++) {
        u32 id = *ids++;
        if (id != 0) {
            if (id < 0x260) {
                s32 category = *(s8 *)(datCommandSelectors + id * 2 + 1);
                if (category != 2) {
                    if (category != 4) {
                        if ((*(u8 *)(datCommandRecords + id * 56 + 1) & 2) != 0) {
                            if (id < 0xAB || (id >= 0xAD && id != 0xBF)) {
                                choices[count++] = id;
                            }
                        }
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    return choices[effMiscRandMod(0, count)];
}

extern s32 datAbilityParameters;

f32 btlGetActionCategoryMultiplier(s32 object, s32 unused, s32 index) {
    s32 category = *(u16 *)(datCommandRecords + index * 56 + 0x16);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(object + 0x120, 0x21D) != 0) {
        return *(f32 *)(datAbilityParameters + 0xE8);
    }
    return 0.0f;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(datCommandRecords + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(datCommandRecords + index * 56 + 0x22) / 100.0f;
}

u32 btlAdjustPointsForCombatFlags(u32 flags, u32 secondary, u32 points, s32 index) {
    if (flags & 0x20000) return 0x1194;
    if (flags & 0x40000) return 0x1194;
    if (flags & 0x10000) return points + 0x64;
    if (flags & 2) return points + 0x64;
    if (secondary & 4) return points >> 1;
    if (secondary & 2) return points >> 1;
    if (flags & 4) {
        if (*(u16 *)(datCommandRecords + index * 56 + 0x16) == 8 ||
            *(u16 *)(datCommandRecords + index * 56 + 0x16) == 10) {
            return points;
        }
        if (*(u8 *)(datCommandRecords + index * 56 + 2) == 2) return points;
        return points + 0x64;
    }
    return points;
}

s32 btlGetCommandResultKindFromFlags(u32 flags, u32 secondary) {
    if (flags & 0x20000) return 1;
    if (flags & 0x40000) return 1;
    if (flags & 0x10000) return 1;
    if (flags & 2) return 1;
    if (secondary & 4) return 3;
    return secondary & 2 ? 2 : 1;
}

s32 btlAverageMaximumValueForMask(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x128);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void btlAverageFilteredMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 1);
}

void btlAverageAllMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 0);
}

s32 btlAverageCurrentValueForMask(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x126);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void btlAverageFilteredCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 1);
}

void btlAverageAllCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 0);
}

s32 btlAverageMaskedActorStat(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x134);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void func_001A9488(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 1);
}

void func_001A94A0(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 0);
}

s32 btlSumOrAverageActorAttribute(u32 mask, s32 attribute, s8 allowDisabled) {
    s32 sum = 0;
    s32 count = 0;
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    s32 value = func_00119368(node + 0x120, attribute);
                    count++;
                    sum += value;
                }
            }
        }
    }
    if (count >= 2) {
        sum /= count;
    }
    return sum;
}

void btlAverageFilteredActorAttribute(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A95C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9780);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1D28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A99B0);

void btlClearUnitStatusMask(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                *(u32 *)(node + 0x110) = flags & ~0x1000;
                *(u16 *)(node + 0x120) &= ~0x1000;
            }
        }
    }
}

extern char D_003A1D78[];

extern s32 evtRunContext(s32, u8 *, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9D38);

extern char D_003A1DA0[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

s32 btlRollActorEligibilityWithAbilityOverride(u8 *actor) {
    u8 *battle = (u8 *)btlGetRuntime();
    s32 (*predicate)(u8 *) = *(s32 (**)(u8 *))(battle + 0x65C);
    if (predicate != 0 && predicate(actor) == 0) return 0;
    if (*(u32 *)(actor + 0x114) & 0x2000) return 0;
    if ((*(u16 *)(actor + 0x12E) & 0x7FFF) == 0x4000) return 0;
    if (btlCheckSpecialAbility((s32)(actor + 0x120), 0x231)) return 1;
    btlBossDebugPrintf(D_003A1DA0, 5, 1.0);
    return btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(s32 object) {
    if ((*(u32 *)(object + 0x110) & 0x400) == 0) {
        return 0;
    }
    return (*(s32 *)(datEnemyRecords + *(u16 *)(object + 0x124) * 76) & 0x100) > 0;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1DA0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001A9F40);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA030);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA130);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA548);

u32 func_001AA848(void) {
    btlGetRuntime();
    return 0xffffffff;
}

void btlRestoreUnitMinimumValueAndClearStatus(s32 object, s32 status) {
    *(u16 *)(status + 0x26) &= ~3;
    *(u32 *)(object + 0x110) &= ~0x20;
    *(u16 *)(object + 0x12E) &= ~0x4080;
    if (*(u16 *)(object + 0x126) == 0) {
        *(u16 *)(object + 0x126) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AA8B0);

s32 btlIsBattleRecordEligible(u8 *actor, u8 *target, s32 recordIndex, s32 speciesIndex) {
    s32 (*callback)(u8 *, u8 *, s32) =
        *(s32 (**)(u8 *, u8 *, s32))(btlGetRuntime() + 0x680);
    u8 *resource;
    if (callback != 0 && callback(actor, target, speciesIndex) == 0) {
        return 0;
    }
    resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    if (*(s16 *)(resource + recordIndex * 20 + 0x2C) != 2) {
        return 0;
    }
    if (speciesIndex != 0 &&
        *(u8 *)(datCommandRecords + speciesIndex * 56 + 8) != 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorModeActionCodeAllowed(u8 *actor) {
    if (*(u32 *)(actor + 0xDC) != 1) {
        return 1;
    }
    switch (*(u32 *)(actor + 0xE0)) {
    case 0x26:
    case 0x54:
    case 0x153:
        return 0;
    default:
        return 1;
    }
}

s32 btlHasHighPriorityState(void) {
    u8 *resource;
    s8 index;
    s32 count;
    if (*(s8 *)btlCommandPanelWork == 2) {
        resource = (u8 *)btlLinkedSelectionTaskBuffer;
        index = *(s8 *)resource;
        count = *(s16 *)(resource + index * 2);
        if (count >= 0x80) {
            if (*(s32 *)(resource + index * 8 + 4) >= 11) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    u8 *actor = *(u8 **)(btlGetRuntime() + 0x228);
    s32 count = 0;
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x321) == 0x301 &&
            (*(u16 *)(actor + 0x12E) & 0x800) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}

typedef struct BattleCmdPanelSlot {
    u8 pad_00[5];
    u8 flag;
    u16 value;
    u8 pad_08[8];
} BattleCmdPanelSlot;

typedef struct BattleCmdPanelHead {
    u8 kind;
    s8 index;
    u16 mask;
    u8 pad_04[8];
    u32 first;
} BattleCmdPanelHead;

typedef struct BattleCmdPanel {
    BattleCmdPanelHead head;
    BattleCmdPanelSlot slotsA[5];
    BattleCmdPanelSlot slotsB[5];
} BattleCmdPanel;

typedef struct BattleCmdPanelSlotTable {
    u32 values[5];
} BattleCmdPanelSlotTable;

extern const BattleCmdPanelSlotTable D_003A1FA8;

extern const BattleCmdPanelSlotTable D_003A1FC0;

void btlInitializeCommandPanelSlotTables(void) {
    BattleCmdPanelSlotTable tableA = D_003A1FA8;
    BattleCmdPanelSlotTable tableB = D_003A1FC0;
    s32 i;

    btlCommandPanelWork = (u32)sdfAllocAndClearQuadwords(0xCC);
    ((BattleCmdPanelHead *)btlCommandPanelWork)->kind = 1;
    ((BattleCmdPanelHead *)btlCommandPanelWork)->index = 0;
    ((BattleCmdPanelHead *)btlCommandPanelWork)->mask = btlGetActorIdForClass(((BattleCmdPanelHead *)btlCommandPanelWork)->index);
    btlGetActorClassPair(((BattleCmdPanelHead *)btlCommandPanelWork)->index,
                  &((BattleCmdPanelHead *)btlCommandPanelWork)->first,
                  (u32 *)(btlCommandPanelWork + 0x10));
    for (i = 0; i < 5; i++) {
        ((BattleCmdPanel *)btlCommandPanelWork)->slotsA[i].flag = 0;
        ((BattleCmdPanel *)btlCommandPanelWork)->slotsA[i].value = tableB.values[i];
        ((BattleCmdPanel *)btlCommandPanelWork)->slotsB[i].flag = 1;
        ((BattleCmdPanel *)btlCommandPanelWork)->slotsB[i].value = tableA.values[i];
    }
}

typedef struct ActorClassIds {
    s16 values[8];
} ActorClassIds;

extern const ActorClassIds btlActorIdsByClass;

s16 btlGetActorIdForClass(s8 classId) {
    ActorClassIds table = btlActorIdsByClass;
    return table.values[classId];
}

typedef struct ActorClassPairTable {
    u32 values[14];
} ActorClassPairTable;

extern const ActorClassPairTable btlActorClassPairs;

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    ActorClassPairTable pairs = btlActorClassPairs;
    *first = pairs.values[classId * 2];
    *second = pairs.values[classId * 2 + 1];
}

extern SndPad D_00324510;

extern u32 btlCommandPanelWork;

extern u32 btlLinkedSelectionTaskBuffer;

extern u8 D_00359160[];

extern void func_001B83D8(s32, s32, s32);

extern void sndSetStationedSeVolume(u32);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1FA8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", btlActorIdsByClass);

INCLUDE_RODATA(const s32, "game/code_001A1960", btlActorClassPairs);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AADF8);

typedef struct BattleCmdPanelGridTable {
    u8 values[30];
} BattleCmdPanelGridTable;

extern const BattleCmdPanelGridTable D_003A2050;

extern const BattleCmdPanelGridTable D_003A2070;

void btlPopulateCommandPanelGrid(void) {
    BattleCmdPanelGridTable table1 = D_003A2050;
    BattleCmdPanelGridTable table2 = D_003A2070;
    s32 i;

    for (i = 0; i < 5; i++) {
        *(u8 *)(btlCommandPanelWork + 0x14 + i * 0x10) =
            table1.values[*(s8 *)(btlCommandPanelWork + 1) * 5 + i];
        *(u8 *)(btlCommandPanelWork + 0x64 + i * 0x10) =
            table2.values[*(s8 *)(btlCommandPanelWork + 1) * 5 + i];
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AB270);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AB558);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AB810);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ABDF8);

void btlReleaseAndClearChipBlock(void) {
    sdfReleaseChipBlock(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC080);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC398);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC4A8);

void btlInitializeActionRecordWithScale(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

typedef struct BtlResBlock {
    s32 unk0;
    s32 nameA;
    s32 nameB;
    s32 nameC;
    s32 resA;
    s32 resB;
    s32 resC;
    s32 unk1C;
} BtlResBlock;

extern u8 D_003BB3E4;

extern u8 btlResourceBlockLoaded;

extern BtlResBlock *btlResourceBlock;

extern char D_003A21C8[]; /* "/battle/panel/batle_01.spr" */

extern char D_003A21E8[]; /* "/battle/panel/batle_02.spr" */

extern char D_003A2208[]; /* "/battle/panel/battle_03.spr" */

extern s32 func_002D03F8(s32);

extern u32 *sdfResourceRetainAddress(s32);

extern s32 func_002EB028(char *, void *, s32);

void btlPanelResourcesLoad(void) {
    u8 params[16];
    s32 handle;
    BtlResBlock *block;
    if (D_003BB3E4 == 0) {
        handle = func_002D03F8(0x28);
        block = (BtlResBlock *)sdfResourceRetainAddress(handle);
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->nameA = func_002EB028(D_003A21C8, params, 0);
        btlResourceBlock->nameB = func_002EB028(D_003A21E8, params, 0);
        btlResourceBlock->nameC = func_002EB028(D_003A2208, params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_003BB3E4 = 1;
}

typedef struct BtlWorkRes {
    u8 pad[0x4A4];
    s32 resA;
    s32 resB;
    s32 resC;
} BtlWorkRes;

extern s32 func_002BD9C0();

void btlLoadResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
    if (btlResourceBlockLoaded == 0) {
        btlResourceBlock->resA = func_002BD9C0(btlResourceBlock->nameA, 0);
        btlResourceBlock->resB = func_002BD9C0(btlResourceBlock->nameB, 0);
        btlResourceBlock->resC = func_002BD9C0(btlResourceBlock->nameC, 0);
        work->resA = btlResourceBlock->resA;
        work->resB = btlResourceBlock->resB;
        btlResourceBlockLoaded = 1;
    }
}

extern s32 effDestroyResourceSlotSet();

void btlReleaseResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
    if (btlResourceBlockLoaded != 0) {
        effDestroyResourceSlotSet(btlResourceBlock->resA);
        btlResourceBlock->resA = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resB);
        btlResourceBlock->resB = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resC);
        btlResourceBlock->resC = 0;
        work->resA = 0;
        work->resB = 0;
        work->resC = 0;
        btlResourceBlockLoaded = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC7D8);

void btlClearTaskActorSlots(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u8 *node = *(u8 **)(context + 0x224);
    while (node != 0) {
        s32 i;
        u8 *actor = *(u8 **)(node + 0x18);
        if (actor != 0) {
            if (*(u16 *)(context + 0x24C) == 1) {
                if (*(u32 *)(actor + 0x110) & 0x200) {
                    u32 *entries;
                    i = 0;
                    entries = (u32 *)(node + 0x148);
                    for (; i < 8; i++) {
                        *entries++ = 0;
                    }
                }
            } else if (*(u32 *)(actor + 0x110) & 0x400) {
                u32 *entries;
                i = 7;
                entries = (u32 *)(node + 0x164);
                for (; i >= 0; i--) {
                    *entries-- = 0;
                }
            }
        }
        node = *(u8 **)(node + 0x16C);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AC9E8);

void func_001ACAE0(void) {
    s32 temp_v0;
    s32 buf[4];

    temp_v0 = btlGetRuntime();
    fldCountSceneFadeKinds(temp_v0, buf);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACB08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACC20);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 2;
    puVar1 = (u32 *)(datGameState + 0x2e9dc);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACD30);

void func_001ACDF0(void) {
    s32 *temp_v0;
    s32 temp_v1;

    temp_v0 = D_00359A78;
    temp_v0 += 2;
    temp_v1 = 2;
    do {
        temp_v1 = temp_v1 - 1;
        *temp_v0 = 0;
        temp_v0 = temp_v0 - 1;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACE28);

extern u32 D_003BB3A0;

s64 btlSetTaskPhase2(void) {
    s64 task = kwlnTaskGetTaskByName(D_003BB3A0);
    if (task != 0) {
        *(s32 *)kwlnTaskGetUserValue(task) = 2;
        return 1;
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACF10);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ACFE0);

u32 btlIsNamedBattleTaskRegistered(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(10);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001AD1F8(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD230);

u32 func_001AD3A8(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0xb);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlGetRegisteredTaskValueOrDefault(void) {
    if (func_001AD3A8() == 0) {
        return 0x80;
    }
    return *(s8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef));
}

u32 func_001AD428(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD468);

void func_001AD5A0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

u32 btlHasRegisteredGuidePanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(9);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

typedef struct BtlPhaseTask {
    s32 phase;
} BtlPhaseTask;

void btlSetTaskPhase5(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        ((BtlPhaseTask *)kwlnTaskGetUserValue(task))->phase = 5;
        *(u8 *)btlCommandPanelWork = 3;
    }
}

void btlSetTrackedTaskDisplayMode(s32 mode) {
    s32 task = btlGetTrackedTaskHandle(7);
    if (task != 0) {
        s32 *data = (s32 *)kwlnTaskGetUserValue(task);
        data[2] = mode;
        if (mode == 0) {
            data[1] = 1;
            data[5] = 0x80;
        } else {
            data[5] = 0xFF;
            data[1] = 4;
            data[6] = 0xFF;
        }
    }
}

void func_001AD6D8(s32 unused) {
    btlGetRuntime();
    kwlnTaskGetUserValue(btlGetTrackedTaskHandle(7));
    *(u8 *)(btlTrackedTaskHandles + 0x48) = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(6);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3BC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD758);

u32 func_001AD928(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(1);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3AC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AD970);

u32 func_001ADB30(void) {
    s64 temp_v0;

    temp_v0 = btlGetTrackedTaskHandle(0);
    if (temp_v0 != 0) {
        temp_v0 = kwlnTaskIsRegistered(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3A8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

typedef struct MsgQueueTaskData {
    s32 task;
    s32 id;
    s32 unk08;
    s32 counter;
    s32 value;
} MsgQueueTaskData;

extern s32 func_001B4A70(s64);

extern void btlReleaseDialogTaskData(s64);

extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlReplaceDialogTasksAndQueueMessage(s32 arg0, s32 arg1) {
    s32 context = btlGetRuntime();
    s32 task = btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (func_001ADB30() != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
    data = (MsgQueueTaskData *)sdfAllocAndClearQuadwords(0x40);
    if (func_001AD928() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(10), 0);
    }
    data->id = arg0;
    data->unk08 = arg1;
    data->value = 0x2D;
    task = kwlnTaskCreate(D_003BB3A8, 0x2B0E, 1, 1, func_001B4A70, btlReleaseDialogTaskData, (u32)data);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    data->task = task;
    btlSetTrackedTaskHandle(0, task);
    return 1;
}

s32 btlGetTrackedTaskHandle(s32 arg0) {
    s32 temp_v0;

    temp_v0 = btlTrackedTaskHandles + arg0 * 4;
    return *(s32 *)temp_v0;
}

void btlSetTrackedTaskHandle(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = btlTrackedTaskHandles + arg0 * 4;
    *(s32 *)temp_v0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ADCE8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ADE68);

typedef struct { u16 flag; u16 unk_02; } MesWindowState;

typedef struct { u32 unk_00; s32 window[9]; u8 unk_28[0x20]; MesWindowState state[9]; } MesWindowList;

void itfMesCloseAllWindows(s32 handle) {
    MesWindowList *list;
    s32 i;
    btlGetRuntime();
    list = (MesWindowList *)kwlnTaskGetUserValue(handle);
    for (i = 0; i < 9; i++) {
        if (list->state[i].flag != 0) {
            itfMesCleanupWindow(list->window[i], 0);
            itfMesDestroyWindowIfPresent(list->window[i]);
        }
    }
    sdfReleaseChipBlock(list);
    btlSetTrackedTaskHandle(10, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2050);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2070);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2090);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A20A0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A20D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2100);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2128);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2158);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2168);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21A8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21B8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21C8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A21E8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2208);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2228);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2258);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2270);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2298);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A22C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AE250);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AE540);

void btlReleaseTaskAndRefreshCursorIfFlagged(s32 handle) {
    u8 *context = (u8 *)btlGetRuntime();
    u8 *data = (u8 *)kwlnTaskGetUserValue(handle);
    sdfReleaseChipBlock(data);
    btlSetTrackedTaskHandle(11, 0);
    if (*(u16 *)(*(u8 **)(*(u8 **)(context + 0x164) + 0x18) + 0x12E) & 0x80) {
        btlInitCursorAndApplyAction(context + 0x70, context + 0x70);
    }
    *(u32 *)(context + 0x1F4) |= 0x100000;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AEE78);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2460);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AF058);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AF5D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2490);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A24A0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A24D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2500);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2530);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001AFF78);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B0460);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A25C0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A25D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A25F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2608);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A27C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B05D0);

u32 btlSetSlotLowByteClamped(BtlSlotOwner *owner, s32 group, s32 slot, s32 delta) {
    u32 word = owner->records[group].word[slot];
    u32 limit;
    u32 value;
    if (delta > 0) {
        limit = value = word & 0xFF;
        if ((u32)delta < value) {
            value = delta;
        }
    } else {
        limit = word & 0xFF;
        value = 0;
    }
    delta = value;
    if (limit > 0x80) {
        if (delta >= 0x80) {
            delta = limit;
        }
    }
    return (word & ~0xFF) | delta;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B09A8);

void btlReleaseMessageWindowTask(s64 task) {
    s32 *entry = (s32 *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(entry[9], 0);
    sdfReleaseChipBlock(entry);
    btlSetTrackedTaskHandle(9, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B0CB8);

u8 btlHasRequiredActorStatusBits(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

void func_001B0D70(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x2c) + 0x18);
    *(s16 *)(temp_v0 + 0x2ac) = arg1;
    *(s16 *)(temp_v0 + 0x2ae) = arg2;
    *(s16 *)(temp_v0 + 0x2b0) = arg3;
}

s32 btlCountEligibleLinkedActors(s32 context) {
    s32 node = *(s32 *)(context + 0x228);
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        if ((*(u64 *)(node + 0x110) & 0x201) == 0x201) {
            if ((*(u16 *)(node + 0x120) & 2) != 0) {
                count++;
            }
        }
    }
    return count;
}

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern void func_00101A80(u32, u32);

extern u32 D_003BB3C0;

extern s32 func_001B0E68(s64);

extern void btlReleaseRegisteredChildTaskWork(s64);

void btlStartRegisteredChildTask(void) {
    s32 context = btlGetRuntime();
    u32 data = (u32)sdfAllocAndClearQuadwords(0x20);
    u32 task = kwlnTaskCreate(D_003BB3C0, 0x2B0E, 1, 1, func_001B0E68, btlReleaseRegisteredChildTaskWork, data);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    btlSetTrackedTaskHandle(7, task);
}

void func_001B0E48(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B0E68);

void btlReleaseRegisteredChildTaskWork(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    btlSetTrackedTaskHandle(7, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A28C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B1518);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B19F8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2938);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B1C88);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2968);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2390);

s32 btlUpdatePhaseGatedTaskUntilTimeout(s64 task) {
    s32 *state = (s32 *)kwlnTaskGetUserValue(task);
    s32 phase = btlGetNamedTaskPairStatusOrUnavailable();
    if ((u8)(phase - 1) < 2) {
        return 0;
    }
    func_001B1518(state);
    if (*(s8 *)(btlTrackedTaskHandles + 0x54) == 0) {
        func_001B19F8(state);
    }
    func_001B1C88(state);
    if (*(s8 *)(btlTrackedTaskHandles + 0x54) == 0) {
        func_001B2390(state);
    }
    ++state[0];
    return state[0] < 50 ? 0 : -1;
}

void btlReleasePsechgPanelWork(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    btlSetTrackedTaskHandle(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2998);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2AC8);

void btlReleaseBattleScratchBlocks(void) {
    sdfReleaseChipBlock(D_003BD834);
    sdfReleaseChipBlock(D_003BD838);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A29F0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B2D80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3300);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3DC8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B3FB0);

void btlReleaseCmsleffPanelWork(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    btlSetTrackedTaskHandle(5, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4308);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B47A8);

void btlFreeRegisteredTaskData(s64 task) {
    btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(1, 0);
}

void btlToggleModelFlagOnInput(void) {
    if (D_00324530[13] < 0) {
        if (mdlFlagTest(0xC0E)) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4A70);

void btlReleaseDialogTaskData(s64 task) {
    s32 *entry;
    btlGetRuntime();
    entry = (s32 *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(entry[1], 0);
    sdfReleaseChipBlock(entry);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)sdfAllocAndClearQuadwords(0x40);
    D_003BB3DC = (u32)window;
    *(s32 *)(window + 0x10) = 0x14;
    *(s32 *)(window + 0x18) = 0x1800080;
    *(s32 *)(window + 0x1C) = 0x40800080;
    *(s32 *)(window + 0x20) = 0x40800080;
    *(s32 *)(window + 0x24) = 0x60808080;
    *(s32 *)(window + 0x38) = 0xBB;
    *(s32 *)(window + 0x3C) = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4D58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2AF0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B4F10);

void btlReleaseRegisteredTaskBuffer(void) {
    btlGetRuntime();
    sdfReleaseChipBlock(D_003BB3DC);
    D_003BB3DC = 0;
    btlSetTrackedTaskHandle(4, 0);
}

s32 sndAreSlotsEmpty(void) {
    s32 *slot = (s32 *)(datGameState + 0x2E9DC);
    s32 i;
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            return 0;
        }
    }
    return 1;
}

extern u8 D_00359160[];

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B53E8);

s64 btlGetTaskState6(void) {
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        s32 *state = *(s32 **)(kwlnTaskGetUserValue(task) + 0x2C);
        return func_001B53E8(state[6]);
    }
    return task;
}

typedef struct FlagEntry {
    u32 unk0;
    u16 id;
    u8 pad6[6];
} FlagEntry;

extern u8 *func_001BD708(u8 *, u16 *);

s64 btlClearFlagEntries(void) {
    u16 count;
    s64 task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    FlagEntry *entries;
    s32 i;
    if (task != 0) {
        entries = (FlagEntry *)func_001BD708((u8 *)kwlnTaskGetUserValue(task), &count);
        for (i = 0; i < count; i++) {
            func_001ACD30(entries[i].id, 0);
        }
        return 1;
    }
    return task;
}

extern u32 D_003BB3D0;

extern u32 D_003BB3D4;

s64 btlDestroyTaskC(void) {
    s64 result = kwlnTaskGetTaskByName(D_003BB3D0);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xC), 0);
        }
        result = 1;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2B20);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B55A8);

void btlFinishTrackedBattleTaskAndCloseWindow(s64 task) {
    s32 context = btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(12, 0);
    *(u32 *)(context + 0x1F4) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2B50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B5970);

s64 btlDestroyTaskD(void) {
    s64 result = kwlnTaskGetTaskByName(D_003BB3D4);
    if (result != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xD), 0);
        }
        result = 1;
    }
    return result;
}

void btlReleaseWindowTask(s64 task) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    sdfReleaseChipBlock(kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(13, 0);
    battle->flags |= 0x100000;
    *(u8 *)(btlTrackedTaskHandles + 0x3D) = 0;
    evtFinishMessageWindowAndNotify();
}

extern u8 btlSoundSlotDefaults[];

void btlInitSoundSlotTable(void) {
    u8 initial[0x20];
    u8 *allocated;
    u32 *source;
    u32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, btlSoundSlotDefaults, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    btlLinkedSelectionTaskBuffer = (u32)allocated;
    state = (s16 *)(allocated + 2);
    destination = (u32 *)(allocated + 0x10);
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    btlSetTrackedTaskHandle(2, 1);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", btlSoundSlotDefaults);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B5CD8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B60E8);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfReleaseChipBlock(btlLinkedSelectionTaskBuffer);
    }
    btlSetTrackedTaskHandle(2, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6308);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6498);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6850);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B6CF8);

void btlReleaseStwrPanelResource(s64 arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    func_002D0918(*(s32 *)temp_v0);
    btlSetTrackedTaskHandle(3, 0);
}

s32 btlAreLinkedSceneCountersAtThreshold(void) {
    s32 base;
    s32 i;
    if (btlGetTrackedTaskHandle(8) == 0) {
        if (btlGetTrackedTaskHandle(2) != 0) {
            base = btlLinkedSelectionTaskBuffer;
            for (i = 0; i < 4; i++) {
                if (*(s16 *)(base + 2 + i * 2) < 0x80) {
                    return 0;
                }
                if (*(s32 *)(base + 0xC + i * 8) < 11) {
                    return 0;
                }
            }
            if (*(s32 *)(base + 0x18) == *(s32 *)(base + 0x10) + 23) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7238);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B74C8);

typedef struct BtlSlot {
    u8 pad0[4];
    u8 state;
    u8 pad5[0x28B];
} BtlSlot;

typedef struct BtlSlotBank {
    u8 pad0[8];
    s32 count;
    u8 padC[0x7C4];
    BtlSlot slots[1];
} BtlSlotBank;

void btlSlotBankPromoteStates(BtlSlotBank *bank) {
    s32 i;
    for (i = 0; i < bank->count; i++) {
        BtlSlot *slot = &bank->slots[i];
        s32 state = slot->state;
        if (state == 1 || state == 2) {
            slot->state = 4;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7880);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7C90);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B7F50);

void btlReleaseTrackedTaskResource(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3B0);
    temp_v1 = kwlnTaskGetUserValue(temp_v0);
    func_002D0918(*(s32 *)(temp_v1 + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B83D8);

typedef struct BtlSlotRow {
    u8 pad_00[0x10];
    u8 state;
    u8 value;
} BtlSlotRow;

void btlUpdateActorSlotPresentationState(BtlUnit *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlUnit *node = ((BtlActorWork *)btlGetRuntime())->actorList;
    u8 *entry;
    BtlSlotRow *slotEntry;
    s32 offset;
    for (; node != 0; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits((s32)node) != 0) {
            slot = *(u8 *)((u8 *)node + 0x11C);
            if (object->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        s64 task = kwlnTaskGetTaskByName(D_003BB3B0);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                func_001B8838(entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (BtlSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8650);

void btlResetActorSlotPresentationValue(u8 *object) {
    s32 count = 0;
    u8 slot = 0;
    u8 *node = *(u8 **)(btlGetRuntime() + 0x228);
    u8 *entry;
    s32 offset;
    for (; node != 0; node = *(u8 **)(node + 0x344)) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = *(u8 *)(object + 0x11C);
            if (*(s64 *)(object + 0x108) == *(s64 *)(node + 0x108)) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3B0));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8838);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B89B0);

void btlUpdateActorSlotStates(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

typedef struct UiSlotEntry {
    u8 pad00[0x18];
    s8 state;
    u8 pad19[0x277];
} UiSlotEntry;

void btlAdvancePendingSceneSlotStates(u8 *scene) {
    UiSlotEntry *entry = (UiSlotEntry *)(scene + 0xE0);
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->state == 1) {
            entry->state = 5;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8BB0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B8CB8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B91F0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2C90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2CE8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D00);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9318);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B96F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9A50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001B9E98);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA198);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA408);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BA660);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BAB08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BAE08);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB118);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB440);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2D68);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB6C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BB990);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BBE18);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BC540);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BC978);

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    s64 first;
    s64 second;
    if (btlTrackedTaskHandles != 0) {
        first = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        second = kwlnTaskGetTaskByName(D_003BB3B0);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (*(s8 *)(btlTrackedTaskHandles + 0x3D) == 0) {
            return *(s8 *)(btlTrackedTaskHandles + 0x3C);
        }
        return 0;
    }
    return -128;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BCB88);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BCCE0);

void fldInitializeBattleSceneFlow(void) {
    s32 context = btlGetRuntime();
    btlNextScaledRandom(7);
    if ((*(u32 *)(context + 0x1F4) & 0x400) != 0) {
        if (*(u16 *)(context + 0x24C) == 1) {
            func_001AD6D8(0);
            btlClearNodeFlags();
        } else {
            func_001AD6D8(1);
            btlClearNodeFlags();
        }
    }
    btlToggleModelFlagOnInput();
    func_001BCCE0();
}

void btlDebugPrintf(const char *fmt, ...) {
}

void func_001BCE90(void) {
}

void func_001BCE98(void) {
}

void fldSubmitSceneObjectAtCoordinates(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    itfSetTextDrawLimit(0x13);
    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_001958A0(temp_v0, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(temp_v0);
    itfSetTextDrawLimit(0xffffffffffffffff);
}

extern u32 D_003BAA8C;

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = func_00197760(x << 4, y << 3, z, w, D_003BAA8C + index * 17, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

extern u32 D_003BAA84;

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = func_00197760(x << 4, y << 3, z, w, D_003BAA84 + index * 25, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

extern u8 *datBattleSceneRecords;

s32 btlIsSceneActorLimitSatisfied(s32 unused, u32 limit) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(u16 *)(datBattleSceneRecords + index * 40 + 0x20) & 0x800) {
        return 1;
    }
    if (limit < (u32)btlCountFlaggedSceneActors()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD0D0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2DD8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD190);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD2C0);

extern u16 D_00359960[];

extern u16 D_00358B20[];

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD4F0);

extern u8 D_00358FE0[];

void btlBuildEligibleActorList(s32 unused, s16 *count) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    s32 total = 0;
    if ((*(u16 *)(datBattleSceneRecords + index * 40 + 0x20) & 0x800) == 0) {
        u8 *selected = (u8 *)(datGameState + 0x12A0);
        u8 *flags = (u8 *)datItemSkillRecords;
        u8 *out = D_00358FE0;
        s32 i;
        for (i = 0; i < 0xC0; i++, flags += 8, selected++) {
            if (*selected != 0 && (*flags & 2)) {
                out[0] = i;
                out[1] = *selected;
                out += 2;
                total++;
            }
        }
    }
    *count = total;
}

extern u8 D_00359160[];

u8 *func_001BD708(u8 *object, u16 *value) {
    s32 result = func_001A3740(*(s32 *)(*(u8 **)(object + 0x2C) + 0x18), D_00359160);
    *value = result;
    return D_00359160;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BD750);

extern void btlDrawRetreatCommandLabel(s32);

void fldDispatchSceneKindHandler(s32 arg0) {
    switch (func_001BD0D0(arg0, *(s8 *)(btlCommandPanelWork + 1))) {
    case 0:
        func_001BDF60(arg0, 0, 2, 3);
        return;
    case 4:
        func_001BEB58(arg0);
        return;
    case 2:
        func_001BE8A0(arg0);
        return;
    case 3:
        func_001BE590(arg0);
        return;
    case 6:
        btlDrawRetreatCommandLabel(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BDF60);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BE590);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BE8A0);

void btlDrawRetreatCommandLabel(s32 unused) {
    char text[8];
    BattleController *battle;
    s8 *modeFlags;
    s32 color;
    s32 handle;

    memcpy(text, D_003BB450, sizeof(text));
    battle = (BattleController *)btlGetRuntime();
    modeFlags = (s8 *)datBattleSceneRecords;
    if (modeFlags[battle->mode * 0x28] != 0) {
        color = *(s16 *)(btlLinkedSelectionTaskBuffer + 2) | 0x504F6100;
    } else {
        color = *(s16 *)(btlLinkedSelectionTaskBuffer + 2) | 0x89FEFF00;
    }
    itfSetTextDrawLimit(0x13);
    handle = func_00197760(0x1A0, 0xA60, 0xFF0010, color, (s32)text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
    itfSetTextDrawLimit(-1);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BEB58);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF040);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2EA0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF0F8);

void fldClearBattleSceneObject(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(temp_v0);
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 0x2a8) = 0;
    btlReleaseBattleScratchBlocks();
}

void fldInitializeSceneObject(s32 object, s32 owner) {
    memset((void *)object, 0, 0x30);
    *(s32 *)object = 1;
    *(s32 *)(object + 0x28) = owner + 0x20;
    *(s32 *)(object + 0x2C) = owner;
}

INCLUDE_ASM(const s32, "game/code_001A1960", fldGetSceneObjectTaskUserData);

s64 fldGetSceneObjectState(void) {
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)fldGetSceneObjectTaskUserData();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF4C0);

void fldSetSceneObjectAndGroupStates(void) {
    s32 *task = (s32 *)fldGetSceneObjectTaskUserData();
    if (task != 0) {
        u8 *state = (u8 *)btlCommandPanelWork;
        *task = 5;
        *state = 3;
    }
}

void fldGetSceneDirectionStepOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2] = {
        {{-7, 3}, {-6, 4}, {-5, 4}, {-4, 5}, {-3, 5}, {-2, 6}, {0, 0}, {0, 0}},
        {{0x22, 3}, {0x21, 4}, {0x20, 4}, {0x1F, 5}, {0x1E, 5}, {0x1D, 6}, {0, 0}, {0, 0}},
        {{0x11, 0x29}, {0x11, 0x28}, {0x11, 0x27}, {0x11, 0x26}, {0x11, 0x25}, {0x11, 0x24}, {0, 0}, {0, 0}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BF8B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BFAD0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2FB8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A2FE8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001BFDE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C0650);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3020);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3030);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C08B8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C0DF8);

INCLUDE_ASM(const s32, "game/code_001A1960", btlDrawCenteredPanelSegments);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3078);

s32 fldStepSceneStateMachine(s64 handle) {
    BattleController *work = (BattleController *)btlGetRuntime();
    s32 *state;
    s32 mode;
    s32 i;
    s32 count;
    s32 owner;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (s32 *)kwlnTaskGetUserValue(handle);
    switch (*state) {
    case 1:
        *state = 2;
        break;
    case 2:
        owner = state[3];
        mode = 1;
        switch (work->mode) {
        case 0x108:
            if (btlGetEffectActive() == 1) {
                mode = 5;
            }
            break;
        case 0x10E:
            count = btlGetIndexListCount(owner);
            for (i = 0; i < count; i++) {
                if (*(u32 *)((u8 *)btlGetIndexListEntry(state[3], i) + 0x110) & 0x200) {
                    break;
                }
            }
            if (i >= count) {
                mode = 5;
            }
            break;
        }
        func_001C08B8(work, state, mode);
        func_001C0DF8(state);
        break;
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C13E8);

void fldReleaseSceneSpriteWork(u32 arg0) {
    btlFreeIndexList(*(u32 *)((s32)arg0 + 0x10));
    btlFreeIndexList(*(u32 *)((s32)arg0 + 0xc));
    sdfReleaseChipBlock(arg0);
}

void fldReleaseSceneSprite(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    fldReleaseSceneSpriteWork(temp_v0);
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", fldGetSceneScriptTaskUserData);

u32 fldGetSceneScriptState(void) {
    u32 *puVar1;

    puVar1 = (u32 *)fldGetSceneScriptTaskUserData();
    return *puVar1;
}

u32 fldGetSceneScriptValue(void) {
    u32 *puVar1;

    puVar1 = (u32 *)(fldGetSceneScriptTaskUserData() + 0x10);
    return *puVar1;
}

extern s32 fldStepSceneStateMachine(s64);

extern u32 func_001C13E8(s32);

void fldCreateSceneSpriteTask(s32 arg0) {
    BattleController *scene;
    s32 task;
    kwlnTaskGetTaskByName(D_003BB3A0);
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0xA), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(9), 0);
    }
    if (func_001AD928() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(1), 0);
    }
    if (func_001ADB30() != 0) {
        kwlnTaskDestroyWithHierarchy(btlGetTrackedTaskHandle(0), 0);
    }
    scene = (BattleController *)btlGetRuntime();
    task = kwlnTaskCreate(D_003BB3A0, 0x2B0E, 1, 1, fldStepSceneStateMachine, fldReleaseSceneSprite,
                          func_001C13E8(arg0));
    func_00101A80(scene->taskParent, task);
    scene->spriteObject = task;
}

void fldMarkActiveSceneScriptState(void) {
    u32 *temp_v0;

    temp_v0 = (u32 *)fldGetSceneScriptTaskUserData();
    if (temp_v0 != 0) {
        *temp_v0 = 6;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C1850);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A30D8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C1988);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A31A0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2158);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C27B0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3220);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2938);

void fldScaleSceneCoordinateRecord(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C2E90);

extern s32 D_003BD83C;

void fldSetSceneSlotRange(s32 index) {
    u8 *scene = (u8 *)D_003BD83C;
    if (*(s8 *)(scene + 0x20) < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            s32 offset = i * 2;
            scene = (u8 *)D_003BD83C;
            *(s32 *)(scene + (offset + *(s32 *)(scene + 4)) * 4 + 0x38) = 0x80;
            *(u8 *)((*(s32 *)(scene + 4) + offset) + (s32)scene + 0x22) = 3;
        }
        ((u8 *)D_003BD83C)[0x21] = index;
    }
    ((u8 *)D_003BD83C)[0x20] = index;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C3040);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C32B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C3B28);

typedef struct SceneFadingRecord {
    SceneSlot slot;
    u8 alpha;
    s32 target;
} SceneFadingRecord;

void fldInitSceneFadeRecords(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    SceneSlot *slot = (SceneSlot *)((u8 *)scene + 0x2D4);
    SceneFadingRecord *record = (SceneFadingRecord *)((u8 *)scene + 0x44C);
    u32 i = 0;

    while (i < 8 && slot->a != 0) {
        memcpy(&record->slot, slot, sizeof(SceneSlot));
        record->alpha = 0x80;
        record->target = -1;
        i++;
        slot++;
        record++;
    }
    while (i < 8) {
        memset(record, 0, sizeof(*record));
        record->target = -1;
        i++;
        record++;
    }
}

s32 btlFindSceneSlotById(u8 *entry) {
    s32 context = btlGetRuntime();
    u8 *slot = (u8 *)(context + 0x2D4);
    u32 i;
    for (i = 0; i < 8; i++, slot += 3) {
        if (slot[2] == entry[2]) {
            return i;
        }
    }
    return -1;
}

u8 *fldFindSceneSlotRecord(u8 *entry) {
    s32 context = btlGetRuntime();
    s32 index = btlFindSceneSlotById(entry);
    u8 *record = 0;

    if (index != -1) {
        record = (u8 *)(context + index * 3 + 0x2D4);
    }
    return record;
}

s32 btlFadeStaleSceneSlots(void) {
    s32 context = btlGetRuntime();
    u8 *scene = (u8 *)(context + 0x44C);
    u8 *slot = (u8 *)(context + 0x2D4);
    u32 i;
    s32 changed = 0;
    for (i = 0; i < 8; i++, slot += 3, scene += 8) {
        if (scene[2] != slot[2] && fldFindSceneSlotRecord(scene) == 0) {
            if (scene[3] != 0) {
                scene[3] -= 8;
                changed = 1;
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BattleController *scene, s32 *outFadeCount) {
    SceneFadingRecord *record = (SceneFadingRecord *)((u8 *)scene + 0x44C);
    SceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    s8 *global;

    while (fadeCount < 8 && record[fadeCount].slot.a != 0) {
        if (record[fadeCount].slot.a == 1) {
            countA++;
        }
        if (record[fadeCount].slot.a == 2) {
            countB++;
        }
        fadeCount++;
    }
    global = (s8 *)btlTrackedTaskHandles;
    if (global[0x48] == 0) {
        if (countA != 0 || countB != 0) {
            *(s32 *)(global + 0x4C) = countA;
            *(s32 *)(global + 0x50) = countB;
            global[0x48] = 1;
        }
    }
    fadeCount--;
    slot = (SceneSlot *)((u8 *)scene + 0x2D4);
    slotCount = 0;
    while (slotCount < 8 && slot->a != 0) {
        slotCount++;
        slot++;
    }
    *outFadeCount = fadeCount;
    return slotCount;
}
INCLUDE_ASM(const s32, "game/code_001A1960", func_001C4278);

s32 fldSceneCleanupTask(s64 task) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (*(u32 *)(btlTrackedTaskHandles + 0x38) == 1 && *(s8 *)(btlTrackedTaskHandles + 0x3D) == 0) {
        return 0;
    }
    func_001C4278();
    return 0;
}

void fldResetSceneStatus(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x2b0) = 0;
}

void fldBeginSceneTransition(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    func_001BCB88(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    btlGetRuntime();
    func_001BCB88(0, 8);
}

void func_001C44F8(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 arg0) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

extern u32 D_003BB488;

extern s32 fldSceneCleanupTask(s64);

void fldCreateSceneCleanupTask(void) {
    s64 oldTask = kwlnTaskGetTaskByName(D_003BB488);
    u8 *context;
    u32 task;
    if (oldTask == 0) {
        btlGetRuntime();
    }
    context = (u8 *)btlGetRuntime();
    task = kwlnTaskCreate(D_003BB488, 0x2B0E, 1, 1, fldSceneCleanupTask, fldResetSceneStatus, 0);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    *(u32 *)(context + 0x2B0) = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    func_001B6308();
    func_001B6498(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C45F0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C4658);

void func_001C48A8(void) {
}

u32 func_001C48B0(void) {
    return 0;
}

void fldBattleSceneEnterInit(u8 *arg0) {
    u32 id = *(u32 *)(arg0 + 0x27C);
    if (id < 0x400 && (*(u16 *)(datBattleSceneRecords + id * 40 + 0x20) & 0x8000) != 0) {
        *(u32 *)(arg0 + 0x1F8) |= 8;
        kwlnFadeInStart(0xFF, 0xFF, 0xFF, 0);
    }
    if (func_001A99B0() != 0) {
        *(u8 *)(arg0 + 0x24A) = 2;
    } else {
        *(u8 *)(arg0 + 0x24A) = 0;
    }
    func_0020FF50();
    func_0020ED90(*(u32 *)(arg0 + 0x27C));
    func_001F3278(*(u32 *)(arg0 + 0x270), *(u32 *)(arg0 + 0x27C));
    __asm__ volatile(".set noreorder\n\tsqc2 vf0, 0(%0)\n\t.set reorder" : : "r"(arg0));
}

s32 btlLoadBankWhenTasksIdle(void) {
    if (sndIsStreamStatusTwoOrThree() != 0 &&
        btlIsEventSequenceTaskReady() != 0 &&
        func_00213B60() != 0) {
        sndLoadBattleBank();
        func_002101C8();
        return 3;
    }
    return 0;
}

void fldMarkGridTiles(u8 *context) {
    s32 *node = (s32 *)fldCreateSceneTileTask(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A));
    *(u64 *)((u8 *)node + 0x40) = 0x8000000000000001ULL;
    btlStartTask(node);
    btlStartTask(btlCreateFloorLoadTask(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A)));
}

extern void btlResetTitleStreamOnBattleFlag(void);

extern void func_001DC0E8(void);

extern void btlAdvanceWorldCounterAndSpawnActionObject(void);

extern void btlCreateRainEffect(s16, s16);

s32 btlStartOwnerTaskIfClear(u8 *object) {
    if (btlCountTasksForOwner(0x8000000000000001ULL) == 0) {
        btlResetTitleStreamOnBattleFlag();
        func_001DC0E8();
        btlAdvanceWorldCounterAndSpawnActionObject();
        btlCreateRainEffect(*(s16 *)(object + 0x288), *(s16 *)(object + 0x28A));
        return 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C4A80);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void func_001A57A0(void);

extern void kwlnFadeBackgroundStartOut(s32);

extern void kwlnDrawSetOffsetTransition(s32, s32, s32);

extern void kwlnDrawEnableD88(s32);

extern void kwlnDrawEnableDc8(s32);

extern void kwlnDrawEnableE08(s32);

extern void kwlnDrawSetupC70B(s32);

extern void kwlnDrawEnableCd0(s32);

extern void kwlnDrawEnableD30(s32);

extern void btlMarkRuntimeUpdatePending(void);

extern void kwlnFadeStartIn(s32);

extern s32 fldGetEncounterRuntimeResult(void);

extern void fldSetEncounterPendingValue(s32);

extern void evtSetSolarOverlayFullyVisible(void);

INCLUDE_ASM(const s32, "game/code_001A1960", fldSceneStateRestoreDisplay);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C50C0);

extern s32 func_00213B90(void);

extern s32 btlAreWorkBuffersReady(void);

s32 btlInitializeSceneAfterTasksAndBuffersReady(u8 *arg0) {
    u32 i;

    if (btlCountTasksForOwner(0x8000000000000003ULL) == 0 &&
        btlAreWorkBuffersReady() != 0 &&
        func_00213B90() != 0) {
        fldInitializeSceneGroups();
        func_001AC7D8();
        *(u8 *)(arg0 + 0x258) = 0;
        func_001A9780();
        if (*(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 1) != 0) {
            for (i = 0; i < *(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 2); i++) {
                func_001A4240(*(u8 *)(datBattleSceneRecords + *(s32 *)(arg0 + 0x27C) * 40 + 1));
            }
        }
        return 8;
    }
    return 0;
}

void btlConsumeSceneAdvanceFlags(void) {
}

s32 fldConsumeSceneInputFlags(s32 scene) {
    u32 flags = *(u32 *)(scene + 0x1F4);
    s32 result;

    if (flags & 0x800) {
        fldClearSceneAdvanceFlag();
        result = 8;
    } else if (flags & 0x400) {
        fldClearSceneAdvanceFlag();
        result = 7;
    } else {
        return 0;
    }
    *(u32 *)(scene + 0x1F4) |= 0x20;
    return result;
}

extern void func_001AC7D8();

void fldAdvanceSceneVariant(BattleController *scene) {
    s32 notFirst = scene->variant != 1;
    scene->variant = 2 - notFirst;
    if (scene->sceneCallback != 0) {
        s32 variant = scene->sceneCallback();
        if (variant != -1) {
            scene->variant = variant;
        }
    }
    scene->step = scene->step + 1;
    fldInitializeSceneGroups();
    func_001AC7D8();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C5910);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C5B90);

s32 fldSceneStateWaitScriptRelease(u8 *arg0) {
    s32 finished = 1;
    s32 state = *(s32 *)(arg0 + 0x21C);

    switch (state) {
    case 0xF000002:
        if (btlReleaseScriptResourceA() == 0) {
            finished = 0;
        } else {
            btlStartTask(btlCreateSoundUpdateTask(0xC));
            btlStartTask(btlCreateSoundReleaseTask(0xC));
            btlStartTask(btlCreateWaitUnitListIdleTask(0xC));
            btlStartTask(btlCreateApplyToActiveActorsTask(0xC));
            btlStartTask(btlCreateFadeStateResetTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
        }
        break;
    case 0xF000000:
        finished = 0;
        if (*(s32 *)(arg0 + 0x210) == 0x14) {
            func_001AD468(*(s32 *)(arg0 + 0x4A0), *(s32 *)(arg0 + 0x220));
        } else if (*(s32 *)(arg0 + 0x210) >= 0x2D) {
            if (btlHasRegisteredGuidePanelTask() != 0) {
                if (D_00324530[1] < 0) {
                    func_001AD5A0();
                    func_001BCB88(1, 0x14);
                }
            } else {
                finished = 1;
            }
        }
        break;
    case 0xF000003:
        break;
    default:
        if (state != -1) {
            finished = btlReleaseScriptResource() != 0;
        }
        break;
    }

    if (finished != 0) {
        if ((*(u32 *)(arg0 + 0x1F4) & 0x800) == 0) {
            fldEnableSceneGroupAdvancement();
            *(u32 *)(arg0 + 0x1F4) &= ~0x400;
            *(u32 *)(arg0 + 0x1F4) &= ~0x1000;
            *(u32 *)(arg0 + 0x1F4) &= ~0x20;
            return 6;
        }
        return 9;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3248);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3258);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3268);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3278);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A32F0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C5F80);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C6888);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C6D48);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C6E88);

void fldMarkLinkedSceneActors(s32 context) {
    s32 *entry;
    btlAdvanceTitleStateWithAudioCleanup(context);
    entry = *(s32 **)(context + 0x224);
    while (entry != 0) {
        if (entry[0] != 30) {
            btlDispatchStateHandler(entry, 30);
        }
        entry = *(s32 **)((u8 *)entry + 0x16C);
    }
    btlFlagTasksForUpdate();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C7028);

void fldMarkSceneRefresh(s32 arg0) {
    func_00215FE0(arg0);
    *(u32 *)(arg0 + 0x1fc) = *(u32 *)(arg0 + 0x1fc) | 4;
}

extern s32 func_00215FF8(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

u32 fldBeginFadeWhenSceneReady(void) {
    if (func_00215FF8() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

extern SceneInitializer D_00359A88[];

void btlSetScene(s32 scene) {
    s32 work = btlGetRuntime();

    *(s32 *)(work + 0x208) = scene;
    *(s32 *)(work + 0x210) = 0;
    *(s32 *)(work + 0x214) = 0;
    D_00359A88[scene].initialize(work);
}

void btlQueueScene(u32 arg0) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x20c) = arg0;
}

extern void btlSetScene(s32);

extern s32 D_00359A8C[];

void btlUpdateScene(void) {
    s32 context = btlGetRuntime();
    s32 next = *(s32 *)(context + 0x20C);
    s32 result;
    if (next != 0) {
        btlSetScene(next);
        *(s32 *)(context + 0x20C) = 0;
    }
    result = ((s32 (*)(s32))D_00359A8C[*(s32 *)(context + 0x208) * 3])(context);
    if (result != 0) {
        btlQueueScene(result);
    }
    *(s32 *)(context + 0x210) += 1;
}

void btlResetToInitialScene(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    btlSetScene(1);
    *(u32 *)(temp_v0 + 0x20c) = 0;
}

void btlGetCurrentSceneRecordValue(void) {
}

s32 fldGetSceneDescriptorProperty(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(btlGetRuntime() + 0x208);
    return D_00359A90[temp_v0 * 3];
}

s32 *fldGetActorSceneGroupResource(s32 *object) {
    s32 context = btlGetRuntime();
    s32 *result = (s32 *)(context + 0x33C);
    u32 flags;
    if (object[2] & 0x40) {
        return (s32 *)(context + 0x42C);
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200:
        result = (s32 *)(context + 0x2EC);
        break;
    case 0x400:
        break;
    case 0x800:
        result = (s32 *)(context + 0x3F0);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

s32 fldGetSceneGroupResource(u8 group) {
    s32 context = btlGetRuntime();
    s32 resource;

    switch (group) {
    case 1:
        resource = context + 0x2EC;
        break;
    case 2:
        resource = context + 0x33C;
        break;
    case 3:
        resource = context + 0x3F0;
        break;
    default:
        resource = 0;
        break;
    }
    return resource;
}

s32 fldClassifyActorSceneGroup(s32 *object) {
    u32 flags;
    btlGetRuntime();
    if (object[2] & 0x40) {
        return 8;
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

void btlSortSceneGroupByPriorityDesc(SceneTask **list, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = list;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                func_001A29D0((s32)first->actor + 0x120, 3) < func_001A29D0((s32)second->actor + 0x120, 3)) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

void fldSortGroupByPriority(SceneTask **group, s32 count) {
    s32 swapped;
    do {
        SceneTask **entry = group;
        u32 i = 0;
        swapped = 0;
        for (; i < count - 1; i++, entry++) {
            SceneTask *first = entry[0];
            SceneTask *second = entry[1];
            if (first != 0 && second != 0 &&
                *(u8 *)((s32)first->actor + 0x11C) > *(u8 *)((s32)second->actor + 0x11C)) {
                entry[0] = second;
                swapped = 1;
                entry[1] = first;
            }
        }
    } while (swapped != 0);
}

s32 fldGetSceneGroupIndexByActorFlags(u8 *object) {
    u32 flags = *(u32 *)(*(u8 **)(object + 0x18) + 0x110) & 0xE00;
    s32 result;
    switch (flags) {
    case 0x200:
        result = 1;
        break;
    case 0x400:
        result = 2;
        break;
    case 0x800:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C7690);

void fldCompactSceneSlots(void) {
    SceneSlot *slot = ((BattleController *)btlGetRuntime())->slots;
    u32 i;
    if (slot->b == 0) {
        for (i = 0; i < 7; i++) {
            slot[0].a = slot[1].a;
            slot[0].b = slot[1].b;
            slot[0].id = slot[1].id;
            slot++;
        }
        slot->a = 0;
        slot->b = 0;
        slot->id = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C79E8);

INCLUDE_ASM(const s32, "game/code_001A1960", fldInsertSceneSlots);

void fldSwapSceneSlots(s32 index) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    if (scene->flags & 0x100) {
        u8 firstId = scene->slots[0].a;
        u8 count = scene->slots[0].b;
        func_001C79E8(index, 1);
        if (index < count) {
            if (scene->slots[1].a != 0 && scene->slots[1].a != firstId) {
                u8 a = scene->slots[0].a;
                u8 b = scene->slots[0].b;
                u8 id = scene->slots[0].id;
                scene->slots[0].a = scene->slots[1].a;
                scene->slots[0].b = scene->slots[1].b;
                scene->slots[0].id = scene->slots[1].id;
                scene->slots[1].a = a;
                scene->slots[1].b = b;
                scene->slots[1].id = id;
            }
        }
    }
}

s32 fldCountSceneSlots(void) {
    SceneSlot *slot = ((BattleController *)btlGetRuntime())->slots;
    s32 count = 0;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->a != 0 && slot->b != 0) {
            count++;
        }
    }
    return count;
}

s32 fldAreSceneSlotsFinished(void) {
    s32 context = btlGetRuntime();
    u8 *slot;
    u32 i;
    if (*(u32 *)(context + 0x1F4) & 0x1000) {
        return 1;
    }
    slot = (u8 *)(context + 0x2D4);
    for (i = 0; i < 8; i++, slot += 3) {
        if (*slot != 0) {
            return 0;
        }
    }
    return 1;
}

void fldInitializeSceneGroups(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    fldSortGroupByPriority(temp_v0 + 0x2ec, 0x14);
    btlSortSceneGroupByPriorityDesc(temp_v0 + 0x33c, 0x2d);
    btlSortSceneGroupByPriorityDesc(temp_v0 + 0x3f0, 0xf);
    func_001C7690();
}

void btlMoveTaskToGroupTail(SceneTask *task) {
    SceneTask **group = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 count = fldClassifyActorSceneGroup((s32 *)task);
    u32 last;
    u32 i;
    for (i = 0; i < count; i++, group++) {
        if (*group == task) {
            break;
        }
    }
    last = count - 1;
    for (; i < last && group[1] != 0; i++, group++) {
        *group = group[1];
    }
    *group = task;
}

void btlRotateGroupUntilTaskFirst(SceneTask *task) {
    BattleController *scene;
    u32 i;

    if (task != 0 && (task->flags & 8) != 0 && task->actor != 0 &&
        (task->flags & 0x40) == 0 && (task->actor->flags & 0x200) != 0) {
        i = 0;
        scene = (BattleController *)btlGetRuntime();
        fldSortGroupByPriority(scene->groupPrimary, 0x14);
        for (; i < 0x14 && scene->groupPrimary[0] != task; i++) {
            btlMoveTaskToGroupTail(scene->groupPrimary[0]);
        }
    }
}

void fldAppendTaskToGroup(SceneTask *task) {
    SceneTask **slot = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    SceneTask *head;
    fldClassifyActorSceneGroup((s32 *)task);
    head = *slot;
    while (*slot != 0) {
        slot++;
    }
    *slot = task;
    btlRotateGroupUntilTaskFirst(head);
}

void fldAppendSceneGroupHandle(s32 value) {
    s32 *slot = (s32 *)(btlGetRuntime() + 0x42C);
    while (*slot != 0) {
        slot++;
    }
    *slot = value;
}

INCLUDE_ASM(const s32, "game/code_001A1960", fldUpdateSceneGroupTask);

void btlRemoveTaskFromSceneGroup(SceneTask *task) {
    SceneTask **group = (SceneTask **)fldGetActorSceneGroupResource((s32 *)task);
    u32 count = fldClassifyActorSceneGroup((s32 *)task);
    u32 i = 0;
    u32 last;
    for (; i < count; i++) {
        if (group[i] == task) {
            group[i] = 0;
            break;
        }
    }
    last = count - 1;
    for (; i < last; i++) {
        SceneTask *current = group[i];
        SceneTask *next = group[i + 1];
        group[i + 1] = current;
        group[i] = next;
    }
}

void fldEnableSceneGroupAdvancement(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0xc;
}

void fldClearSceneAdvanceFlag(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

s32 fldGetActiveSceneGroupValue(void) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = btlGetRuntime();
    temp_v1 = *(s32 *)(temp_v0 + 0x42c);
    if (temp_v1 != 0) {
        return temp_v1;
    }
    temp_v2 = fldGetSceneGroupResource(*(u8 *)(temp_v0 + 0x2d4));
    return *(s32 *)temp_v2;
}

s32 fldGetSceneGroupEntry(s32 index) {
    s32 context = btlGetRuntime();

    if (*(u8 *)(context + 0x2D4) == 0) {
        return 0;
    }
    return *(s32 *)(fldGetSceneGroupResource(*(u8 *)(context + 0x2D4)) + index * 4);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C8330);

INCLUDE_ASM(const s32, "game/code_001A1960", fldClearSceneSlotsAndGroups);

u32 fldDispatchSceneGroupRequestWhenAllowed(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001C79E8(arg0[1], temp_v0);
    return 1;
}

u8 *fldCreateSceneGroupAction(u8 *actor, u32 owner, s32 groupIndex) {
    u8 group = groupIndex;
    u8 *object = (u8 *)btlAllocTask(0xC);
    u8 *fields;
    object[0] = 1;
    *(s16 *)(object + 0x20) = 0x5C;
    object[0x10] = 0;
    if (actor != 0) {
        *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(u32 *)(object + 0x4C) = (u32)fldDispatchSceneGroupRequestWhenAllowed;
    *(u32 *)(object + 0x48) = 0;
    fields = (u8 *)btlGetTaskArguments(object);
    *(u32 *)(fields + 0) = (u32)actor;
    *(u32 *)(fields + 4) = owner;
    fields[8] = group;
    return object;
}

void fldStopSceneActorActionUpdate(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

u32 fldActivateRequestedSceneActor(s32 *request) {
    s32 context = btlGetRuntime();
    s32 actor = request[0];

    *(u32 *)(context + 0x1F4) |= 4;
    if (actor != 0 && (*(u32 *)(actor + 8) & 0x40) != 0) {
        return 1;
    }
    fldInsertSceneSlots(request[1]);
    return 1;
}

extern u32 fldActivateRequestedSceneActor(s32 *);

u8 *fldCreateSceneActorAction(u8 *owner, s32 parameter) {
    u8 *task = (u8 *)btlAllocTask(8);
    u8 *data;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x5D;
    task[0x10] = 0;
    if (owner != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(owner + 0x18) + 0x108);
    }
    *(u32 *)(task + 0x48) = (u32)fldStopSceneActorActionUpdate;
    *(u32 *)(task + 0x4C) = (u32)fldActivateRequestedSceneActor;
    data = (u8 *)btlGetTaskArguments(task);
    *(u32 *)data = (u32)owner;
    *(s32 *)(data + 4) = parameter;
    return task;
}

u32 fldApplySceneSlotSwapRequest(u32 *arg0) {
    fldSwapSceneSlots(*arg0);
    return 1;
}

s32 *fldCreateActorAction(s32 owner) {
    u8 *task = (u8 *)btlAllocTask(4);
    *task = 1;
    *(u16 *)(task + 0x20) = 0x5E;
    *(u32 *)(task + 0x4C) = (u32)fldApplySceneSlotSwapRequest;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(s32 *)btlGetTaskArguments(task) = owner;
    return (s32 *)task;
}

void btlSetSceneTaskActiveFlag(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void btlClearSceneTaskActiveFlag(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void btlBindActorTaskAndSelectActionNumber(s32 arg0, s32 arg1) {
    u32 flags;

    flags = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((flags & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            flags = *(u32 *)(arg0 + 8);
            goto store;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + datEnemyRecords + 0x15);
    }
    flags = *(u32 *)(arg0 + 8);
store:
    *(u32 *)(arg0 + 8) = flags | 8;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A35A8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A35B8);

void btlActionSeqStateSelect(u8 *task) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *unit = *(u8 **)(task + 0x18);
    s32 (*hook)(u8 *);
    s32 next;
    u32 flags;
    *(u32 *)(task + 8) &= ~0x20;
    if (*(u16 *)(task + 4) == 0) {
        btlDispatchStateHandler(task, 0x1A);
        btlBossDebugPrintf("btl:actnum 0 [%p]\n", task);
        return;
    }
    hook = *(s32 (**)(u8 *))(work + 0x5FC);
    if (hook != 0) {
        next = hook(task);
        if (next != -1) {
            btlDispatchStateHandler(task, next);
            return;
        }
    }
    if (*(u32 *)(task + 8) & 0x40) {
        btlDispatchStateHandler(task, 0xA);
    } else {
        flags = *(u32 *)(unit + 0x110);
        if (flags & 0x200) {
            if (*(u32 *)(work + 0x1F4) & 0x8000) {
                btlDispatchStateHandler(task, 9);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        } else if (flags & 0x400) {
            if (!(*(u32 *)(work + 0x1FC) & 1)) {
                btlDispatchStateHandler(task, 8);
            } else {
                btlDispatchStateHandler(task, 6);
            }
        }
    }
}

extern s32 effOffsetIfOwnerFlagClear();

void btlUnitTurnEndStateSelect(u8 *task) {
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags = *(u32 *)(unit + 0x110);
    if (flags & 0x200) {
        if (flags & 0x1000) {
            if ((*(u32 *)(unit + 0x114) & 0x40) && !(*(u16 *)(unit + 0x12E) & 0x7C0E) &&
                !(*(u32 *)(task + 8) & 0x100)) {
                *(u16 *)(task + 0x50) = 4;
                *(s32 *)(task + 0x54) = effOffsetIfOwnerFlagClear(unit, 0xA4);
                *(u32 *)(unit + 0x110) = (*(u32 *)(unit + 0x110) & ~0x20) | 0x400000;
                *(u16 *)(unit + 0x120) |= 0x4000;
                *(u32 *)(unit + 0x114) |= 0x2000;
                btlDispatchStateHandler(task, 0x10);
            } else {
                btlDispatchStateHandler(task, 0x1D);
            }
            *(u32 *)(unit + 0x114) &= ~0x40;
        } else {
            btlDispatchStateHandler(task, 0x1D);
        }
    } else {
        btlDispatchStateHandler(task, 0x1D);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C8B00);

typedef struct SceneActor {
    u8 pad_00[0x108];
    s64 ownerId;
    u8 pad_110[0x1E8];
    s32 resourceNode;
    u8 pad_2FC[8];
    s32 listNode;
    u8 pad_308[0xC];
    s32 pendingResource;
} SceneActor;

s32 fldReleaseIdleSceneActorResources(SceneActor *actor) {
    if (actor->resourceNode != 0) {
        if (sndIsResourceNodeReferencedOrActive(actor->resourceNode) != 0) {
            return 0;
        }
        sndFreeResourceNode(actor->resourceNode);
        actor->resourceNode = 0;
    }
    if (actor->listNode != 0) {
        if (sndHasResourceFlagsOneOrEight(actor->listNode) != 0) {
            return 0;
        }
        sndFreeListNode(actor->listNode);
        actor->listNode = 0;
    }
    if (actor->pendingResource != 0) {
        return 0;
    }
    if (btlIsUnitInActiveList((s32)actor) != 0) {
        btlResetActiveUnitList();
        return 0;
    }
    if (btlCountTasksForOwner(actor->ownerId) != 0) {
        return 0;
    }
    return btlCountTasksByKind(0x2B) == 0;
}

void func_001C8D38(void) {
}

void func_001C8D40(void) {
}

void func_001C8D48(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

void btlReleaseIdleUnitSoundAndAdvanceTask(u8 *task) {
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags;
    u32 masked;
    s32 resource = *(s32 *)(unit + 0x2F8);

    *(u32 *)(task + 8) &= ~0x100;
    if (resource != 0) {
        if (sndIsResourceNodeReferencedOrActive(resource) == 0) {
            sndFreeResourceNode(*(s32 *)(unit + 0x2F8));
            *(s32 *)(unit + 0x2F8) = 0;
        }
    }
    flags = *(u32 *)(unit + 0x110);
    if ((flags & 0x400) == 0 && (*(u16 *)(unit + 0x12E) & 0x4000) == 0) {
        if (*(s32 *)(unit + 0xC8) == 0x1F) {
            func_001A1948(unit + 0x120, 0x1000);
            flags = *(u32 *)(unit + 0x110);
        }
        masked = flags & ~0x20;
        masked &= ~0x08000000;
        *(u32 *)(unit + 0x110) = masked;
        btlFlagUnitDefeatCandidate(unit);
        func_001D5990(unit);
        btlStartTask(btlAllocateIndexedUnitEffectTask(unit, 0xE, 0, 1.0f));
        fldAppendTaskToGroup((SceneTask *)task);
        btlDispatchStateHandler(task, 2);
    }
    return;
}

void func_001C8E60(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

void btlTaskUpdateFlags(u8 *task) {
    u8 *unit = *(u8 **)(task + 0x18);
    u32 flags;
    if (!(*(u32 *)(unit + 0x110) & 0x20)) {
        *(u32 *)(task + 8) &= ~0x100;
    }
    if (*(s32 *)(unit + 0x2F8) != 0 && sndIsResourceNodeReferencedOrActive(*(s32 *)(unit + 0x2F8)) == 0) {
        sndFreeResourceNode(*(s32 *)(unit + 0x2F8));
        *(s32 *)(unit + 0x2F8) = 0;
    }
    flags = *(u32 *)(unit + 0x110);
    if (flags & 0x20000000) {
        btlDispatchStateHandler(task, 0x11);
    } else if (flags & 0x400000) {
        btlDispatchStateHandler(task, 0x10);
    } else if (flags & 0x10000000) {
        btlDispatchStateHandler(task, 0x12);
    } else if (flags & 0x20) {
        btlUnitTurnEndStateSelect(task);
    }
}

void func_001C8F88(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

void btlActionSeqCheckDispatch(u8 *task) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    u32 flags = scene->flags;
    s32 unit = *(s32 *)(task + 0x18);
    UiObject *actor;
    if (!(flags & 0x20)) {
        for (actor = scene->actors; actor != 0; actor = actor->next) {
            u32 actorFlags = actor->flags;
            if (actorFlags & 0x4000) {
                return;
            }
            if (actorFlags & 0x30400000) {
                return;
            }
        }
        if (!(flags & 0x8000) || func_001C8B00(unit) != 0) {
            if (btlBothSidesActive((UiObject *)unit) == 0) {
                btlDispatchStateHandler(task, 0x1C);
                return;
            }
            if (func_001FCAC0(task) != 0) {
                btlDispatchStateHandler(task, 5);
            } else {
                btlActionSeqStateSelect(task);
            }
        }
    }
}

void func_001C9090(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C9098);

void func_001C93A0(void) {
}

void btlStartCommandSoundAndEffectTasks(u8 *arg0) {
    u8 *ctx;
    u8 *task;
    u64 value;
    s32 count;

    if (sndHasActiveActor() != 0) {
        return;
    }
    if (btlCountTasksByKind(0x45) != 0) {
        return;
    }
    ctx = *(u8 **)(arg0 + 0x18);
    value = btlAdvanceRuntimeSequenceCounter();
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    if (*(u32 *)(ctx + 0x110) & 0x200) {
        btlStartTask(btlCreateCommandSoundTask(arg0, 9));
    } else {
        btlStartTask(btlCreateCommandSoundTask(arg0, 3));
    }
    switch (*(u16 *)(ctx + 0x12E) & 0x7FFF) {
    case 0x200:
        func_001FF0C8(arg0, 0);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x2000:
        func_001FF0C8(arg0, 1);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x20:
        if (*(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) & 0x200) {
            func_001FF0C8(arg0, 2);
        } else {
            func_001FF0C8(arg0, 3);
        }
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 0x40:
        func_001FF0C8(arg0, 4);
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 1:
        *(s32 *)(arg0 + 0x20) = 0xD;
        btlDispatchStateHandler(arg0, 0xC);
        break;
    case 8:
        task = fldCreateSceneGroupAction(arg0, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = *(u64 *)(ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    case 0x800:
        task = fldCreateSceneGroupAction(arg0, 0x64, 1);
        *task = 7;
        *(u64 *)(task + 8) = value;
        *(u64 *)(task + 0x40) = *(u64 *)(ctx + 0x108);
        btlStartTask(task);
        btlDispatchStateHandler(arg0, 0x1A);
        break;
    }
    count = func_001FD170(arg0);
    if (count > 0) {
        task = btlCreateEffObjB(ctx, count);
        *(u64 *)(task + 0x40) = value;
        btlStartTask(task);
    }
    *(u32 *)(arg0 + 8) |= 0x200;
}

void func_001C9628(u32 arg0) {
    btlGetRuntime();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001BF4C0(arg0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C9660);

void btlCommandResultEffectSelect(u8 *task) {
    s32 sel;
    s32 reason;
    s32 state;

    *(u32 *)(task + 8) &= ~4;
    state = *(s32 *)(task + 0x20);
    if (state <= 0) {
        goto spawn;
    }
    if (state >= 5) {
        if (state > 8) {
            goto spawn;
        }
        if (state < 7) {
            goto spawn;
        }
    }
    if (state == 4) {
        sel = btlGetLoggedIndexedCommandItem(*(s32 *)(task + 0x28));
    } else {
        sel = *(s32 *)(task + 0x24);
    }
    reason = btlGetCommandBlockReason(task, sel);
    switch (reason) {
    case 2:
        btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0x82));
        sndSetStationedSeVolume(0xD);
        btlDispatchStateHandler(task, 6);
        return;
    case 6:
        btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xB0));
        sndSetStationedSeVolume(0xD);
        btlDispatchStateHandler(task, 6);
        return;
    case 7:
        btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xB2));
        sndSetStationedSeVolume(0xD);
        btlDispatchStateHandler(task, 6);
        return;
    case 9:
        btlStartTask(btlCreateEffObjB(*(s32 *)(task + 0x18), 0xD0));
        sndSetStationedSeVolume(0xD);
        btlDispatchStateHandler(task, 6);
        return;
    }
spawn:
    fldCreateSceneSpriteTask((s32)task);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C9960);

void func_001C9C20(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001C9C38);

void btlMarkSceneTaskAfterReset(s32 arg0) {
    func_001B83D8(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
}

s32 btlCommandStateSelectB(s32 arg0) {
    if (sndHasActiveActor() == 0) {
        func_001D3FE8(arg0, arg0 + 0x20);
        if (btlAiCheckStatusRollEligibility(arg0) != 0) {
            btlDispatchStateHandler(arg0, 0xB);
        } else {
            btlDispatchStateHandler(arg0, 0xC);
        }
    }
}

void func_001C9EC0(void) {
}

void btlChooseActorStateFromFirstLinkedUnit(u8 *actor) {
    u8 *model;
    if (sndHasActiveActor() != 0) {
        return;
    }
    model = (u8 *)btlGetIndexListEntry(*(u32 *)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (btlIsActiveActor((s32)model) == 0 ||
         (*(u64 *)(model + 0x110) & 0xE1) != 1)) {
        btlDispatchStateHandler(actor, 26);
    } else {
        btlDispatchStateHandler(actor, 12);
    }
}

void btlResetCommandIndexWork(s32 arg0) {
    func_001ACC20();
    btlResetIndexWork(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x2f4) = 0xffffffff;
}

void btlCommandStartSoundTasks(u8 *task) {
    u8 *unit;
    s64 ownerId;
    s32 effect;
    u8 *object;
    if (sndHasActiveActor() == 0 && btlCountTasksByKind(0x45) == 0) {
        unit = *(u8 **)(task + 0x18);
        ownerId = btlAdvanceRuntimeSequenceCounter();
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        if (*(u32 *)(unit + 0x110) & 0x200) {
            btlStartTask(btlCreateCommandSoundTask(task, 9));
        } else {
            btlStartTask(btlCreateCommandSoundTask(task, 3));
        }
        if ((*(u16 *)(unit + 0x12E) & 0x7FFF) == 0x20) {
            if (*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) {
                func_001FF0C8(task, 2);
            } else {
                func_001FF0C8(task, 3);
            }
            btlDispatchStateHandler(task, 0xC);
        }
        effect = func_001FD170(task);
        if (effect > 0) {
            object = (u8 *)btlCreateEffObjB(unit, effect);
            *(s64 *)(object + 0x40) = ownerId;
            btlStartTask(object);
        }
        *(u32 *)(task + 8) |= 0x200;
    }
}

extern char D_003A3648[]; /* "btl:command=%d\n" */

extern char D_003A3648[]; /* "btl:command=%d\n" */

void btlCommandPrintAndFetchOwner(u8 *task) {
    s32 *commandPtr;
    s32 command;
    btlBossDebugPrintf(D_003A3648, *(s32 *)(task + 0x20));
    commandPtr = (s32 *)(task + 0x20);
    func_001D12A0(task, commandPtr);
    command = *commandPtr;
    if (command <= 0) {
        return;
    }
    if (command >= 4) {
        if (command >= 9) {
            return;
        }
        if (command < 7) {
            return;
        }
    }
    if (btlGetIndexListCount(*(s32 *)(task + 0x60)) == 1) {
        *(s64 *)(task + 0x68) = *(s64 *)((u8 *)btlGetIndexListEntry(*(s32 *)(task + 0x60), 0) + 0x108);
    }
}

void btlDispatchEffectCommandWhenActorReady(u8 *command) {
    u8 *actor = *(u8 **)(command + 0x18);
    if (func_001C8B00(actor) == 0) {
        return;
    }
    if (*(u16 *)(command + 0x50) == 2) {
        btlStartTask(btlCreateEffObjB(actor, *(u32 *)(command + 0x54)));
    }
    func_001F0CA0(command, command + 0x20);
    btlDispatchCommandViaHookOrDefault(command, command + 0x20);
}

void func_001CA1F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CA1F8);

void func_001CB408(void) {
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3648);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3670);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CB410);

void func_001CCD10(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CCD18);

void btlMarkLinkedActorStatusFlag(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CE0D8);

void func_001CE5D8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CE5F0);

void func_001CEA58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CEA70);

void func_001CEB78(void) {
}

void btlCommandTaskStartEffects(u8 *task) {
    s32 countdown;
    s32 effectId;

    if ((*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) != 0 &&
        (*(u32 *)(task + 8) & 0x20) != 0) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
    }
    switch (*(s32 *)(task + 0x20)) {
    case 14:
        *(u32 *)(*(u8 **)(task + 0x18) + 0x110) |= 0x2000000;
        effectId = 0xF;
        countdown = 0x64;
        break;
    case 10:
        if ((*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x400) != 0) {
            btlStartTask(btlCreateCommandSoundUpdateTask());
            btlStartTask(btlCreateSecondaryCommandSoundTask());
            btlStartTask(btlCreateCommandSoundTask((s32)task, 3));
        }
        effectId = (*(u32 *)(*(u8 **)(task + 0x18) + 0x110) & 0x200) ? 0xF : 0x1E;
        countdown = 0x32;
        break;
    case 13:
        effectId = 0xF;
        countdown = 0x64;
        break;
    default:
        effectId = 0;
        countdown = 0;
        break;
    }

    btlStartTask(btlCreateEffObjA(0, *(s32 *)(task + 0x20)));
    {
        u8 *object = fldCreateSceneGroupAction(task, countdown, 1);
        *(s32 *)(object + 0x28) = effectId;
        *(s64 *)(object + 0x40) = *(s64 *)(*(u8 **)(task + 0x18) + 0x108);
        btlStartTask(object);
    }
    if ((*(u16 *)(*(u8 **)(task + 0x18) + 0x12E) & 0x480) != 0) {
        btlDispatchStateHandler(task, 0x18);
    } else {
        btlDispatchStateHandler(task, 0x1A);
    }
}

extern u64 btlStartTask(s32);

u64 btlCommandTaskReturnStart(u8 *task) {
    u8 *linked = *(u8 **)(task + 0x34);
    u8 *actor;
    u8 *object;

    if (linked != 0) {
        actor = linked;
    } else {
        actor = *(u8 **)(task + 0x18);
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlBossDebugPrintf("return:player=%X[%X]\n", actor[0x2C4], *(u16 *)(actor + 0x124));
    } else {
        btlBossDebugPrintf("return:enemy=%X\n", *(u16 *)(actor + 0x124));
    }
    if (*(u32 *)(actor + 0x110) & 0x400) {
        btlStartTask(btlCreateCommandSoundUpdateTask());
        btlStartTask(btlCreateSecondaryCommandSoundTask());
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 9));
        btlStartTask(btlCreateEffObjA(actor, *(u32 *)(task + 0x20)));
    } else if ((*(u32 *)(task + 8) & 0x200) == 0) {
        btlStartTask(btlCreateCommandSoundTask((s32)task, 0x10));
        btlStartTask(btlCreateEffObjB(actor, 0xF));
    }
    if (*(u32 *)(actor + 0x110) & 0x200) {
        btlSyncPlayerWork((UiObject *)actor);
    }
    object = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(object + 0x28) = 0xF;
    *(u64 *)(object + 0x40) = *(u64 *)(*(u8 **)(task + 0x18) + 0x108);
    btlStartTask((s32)object);
    object = func_001D9468(actor, 1);
    *(s32 *)(object + 0x28) = 0xF;
    return btlStartTask((s32)object);
}

extern s32 btlCountTasksByKind(u16 kind);

extern void btlRepositionPartyAroundBattleCenter(void);

extern s32 func_001A3638(void);

extern void fldUpdateSceneGroupTask(s32 task);

extern void btlRemoveTaskFromSceneGroup(SceneTask *task);

extern void btlDispatchStateHandler(s32 object, s32 state);

void btlCommandTaskReturnUpdate(s32 task) {
    s32 unit = *(s32 *)(task + 0x18);
    u32 flags = *(u32 *)(unit + 0x110);

    *(u32 *)(unit + 0x110) = flags & ~1;
    if (flags & 0x200) {
        btlRepositionPartyAroundBattleCenter();
        func_001A3638();
    }
    if (btlCountTasksByKind(0x3C) != 0) {
        return;
    }
    if (btlCountTasksByKind(0x42) != 0) {
        return;
    }
    if ((*(u32 *)(unit + 0x110) & 0x40) == 0) {
        return;
    }
    if (fldReleaseIdleSceneActorResources((SceneActor *)*(s32 *)(task + 0x18)) != 0) {
        if (*(u32 *)(unit + 0x110) & 0x200) {
            func_001A1960(unit + 0x120, 8);
            func_001A2258(unit);
        }
        fldUpdateSceneGroupTask(task);
        btlRemoveTaskFromSceneGroup((SceneTask *)task);
        btlDispatchStateHandler(task, 0x1E);
    }
}

void btlStartLinkedActorEffectTask(s32 task) {
    s32 unit = *(s32 *)(task + 0x18);
    s32 entry;
    if ((*(u32 *)(unit + 0x110) & 0x200) == 0 && func_001A8CE0(unit) == 0) {
        entry = (s32)btlCreateEffObjB(*(s32 *)(task + 0x18), 0x67);
        *(u8 *)(entry + 0) = 0xA;
        *(u16 *)(entry + 8) = 0x40;
        btlStartTask(entry);
        if ((*(u16 *)(*(s32 *)(task + 0x18) + 0x12E) & 0x480) != 0) {
            btlDispatchStateHandler(task, 0x18);
        } else {
            btlDispatchStateHandler(task, 0x1A);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CF050);

void btlStartOwnerEffectTasks(s32 *arguments) {
    s32 owner = arguments[0x34 / 4];
    s32 value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = func_001D9468(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    btlStartTask(value);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CF7A0);

void btlRecordLinkedActorOutcome(s32 object) {
    s32 context = btlGetRuntime();
    s32 target = *(s32 *)(object + 0x18);
    *(s32 *)(context + 0x254) += 1;
    if (func_001A8640(target)) {
        *(u32 *)(context + 0x1F4) |= 0x2000;
    } else {
        *(u32 *)(context + 0x1F4) |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CFB10);

void func_001CFD70(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001CFD78);

void btlSpawnSceneActionAndSwitchState(void) {
}

void func_001D0048(u8 *arg0) {
    u8 *task = fldCreateSceneGroupAction(arg0, 0x1194, 1);

    btlStartTask(task);
    btlDispatchStateHandler(arg0, 0x1a);
}

void func_001D0088(void) {
}

extern s32 btlCountTasksForOwner(s64);

extern void btlDispatchStateHandler(s32, s32);

void btlAdvanceStateWhenLinkedTasksFinish(s32 object) {
    s32 owner = *(s32 *)(object + 0x18);
    if (btlCountTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        btlDispatchStateHandler(object, 0x1C);
    }
}

void btlAdvanceLinkedUnitWhenOwnerIdle(void) {
}

void func_001D00E0(s32 object) {
    s32 owner = *(s32 *)(object + 0x18);
    if (btlCountTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        *(u32 *)(owner + 0x110) &= ~0x4000;
        btlDispatchStateHandler(object, 2);
    }
}

void btlFinalizeLinkedActionAndAdvanceHistory(void) {
}

extern void btlAdvanceHistoryCounter(s32 task);

extern void btlResetIndexWork();

void btlUnitTurnEndCommit(s32 task) {
    void (*hook)(s32) = *(void (**)(s32))(btlGetRuntime() + 0x600);
    s32 owner = *(s32 *)(task + 0x18);

    if (hook != 0) {
        hook(task);
    }
    btlAdvanceHistoryCounter(task);
    btlResetIndexWork((u8 *)task + 0x20);
    *(s32 *)(owner + 0x2F4) = -1;
    *(u32 *)(task + 0xC) &= ~1;
    *(s32 *)(task + 0x14) += 1;
    *(u32 *)(owner + 0x110) &= ~0x4000;
    fldUpdateSceneGroupTask(task);
    if (*(u32 *)(*(s32 *)(task + 0x18) + 0x110) & 0x20) {
        btlUnitTurnEndStateSelect((u8 *)task);
    } else {
        btlDispatchStateHandler(task, 2);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D0210);

extern void btlResetIndexWork();

void btlRemoveEligibleActorSceneTask(s32 arg0) {
    UiObject *actor = *(UiObject **)(arg0 + 0x18);
    s32 entryFlags;

    if (actor->flags & 0x200) {
        if (fldReleaseIdleSceneActorResources((SceneActor *)actor) == 0) {
            return;
        }
        btlResetIndexWork(arg0 + 0x20);
        actor->marker = -1;
        btlClearAllActorEntrySlots((u32)actor);
        fldUpdateSceneGroupTask(arg0);
        btlRemoveTaskFromSceneGroup((SceneTask *)arg0);
        btlDispatchStateHandler(arg0, 1);
    } else {
        if ((actor->flags & 0x400) == 0) {
            return;
        }
        entryFlags = btlGetEntryFlagsUnlessDisabled((s32)((u8 *)actor + 0x120));
        if ((actor->flags & 0x40) == 0 && !(entryFlags & 0x200)) {
            return;
        }
        fldUpdateSceneGroupTask(arg0);
        btlRemoveTaskFromSceneGroup((SceneTask *)arg0);
        if ((entryFlags & 0x200) && (actor->flags & 0x40) == 0) {
            btlDispatchStateHandler(arg0, 1);
        } else {
            btlDispatchStateHandler(arg0, 0x1E);
        }
    }
}

void func_001D0590(void) {
}

void btlCommandTaskReleaseActor(s32 arg0) {
    if ((*(u32 *)(arg0 + 8) & 8) != 0) {
        if (fldReleaseIdleSceneActorResources((SceneActor *)*(s32 *)(arg0 + 0x18)) == 0) {
            return;
        }
        if (*(s32 *)(arg0 + 0x18) != 0) {
            btlReleaseUnitResources((BtlUnit *)*(s32 *)(arg0 + 0x18));
            {
                u8 *fx = (u8 *)*(s32 *)(arg0 + 0x18);
                *(u32 *)(fx + 0x54) = 0x80808080;
                *(u32 *)(fx + 0x84) = 0x80808080;
                *(u32 *)(fx + 0x110) = *(u32 *)(fx + 0x110) & 0x700;
                btlInitUnitFxDefaults(fx);
            }
            {
                u8 *model = (u8 *)*(s32 *)(arg0 + 0x18);
                PCP_COPY_VECTOR(model + 0x60, model + 0x30);
                PCP_COPY_VECTOR(model + 0x70, model + 0x40);
                *(s32 *)(model + 0x2F0) = -1;
            }
            *(u32 *)(arg0 + 0x18) = 0;
            *(u32 *)(arg0 + 8) &= ~8;
        }
    }
    btlClearSceneTaskActiveFlag(arg0);
    *(u32 *)(arg0 + 8) |= 2;
}

void btlFlagLinkedActorActionInProgress(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

extern s32 btlGetRuntime(void);

extern void btlDispatchStateHandler(s32, s32);

void btlRunHookAndAdvanceUnitState(s32 object) {
    void (*callback)(s32) = *(void (**)(s32))(btlGetRuntime() + 0x660);
    if (callback != 0) {
        callback(object);
    }
    btlDispatchStateHandler(object, 0x1A);
}

void func_001D06C0(void) {
}

void func_001D06C8(void) {
}

void func_001D06D0(void) {
    btlInitDrawTables();
}

void btlAdvanceUnitWhenActionGateClears(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00214868();
    if (temp_v0 == 0) {
        btlDispatchStateHandler(arg0, 6);
        return;
    }
}

typedef struct BattleActionState {
    void (*start)(s32);
    void (*update)(s32);
    void (*finish)(s32);
} BattleActionState;

extern BattleActionState D_00359B28[];

INCLUDE_ASM(const s32, "game/code_001A1960", btlDispatchStateHandler);

extern char D_003A3788[]; /* "btl:action seq create[%p]\n" */

u8 *btlCreateActionSeq(void) {
    u8 *sequence = sdfAllocAndClearQuadwords(0x170);
    u8 *context;
    u8 *head;

    *(u16 *)(sequence + 4) = 1;
    btlInitBattleIndexWork(sequence + 0x20);
    context = (u8 *)btlGetRuntime();
    *(u8 **)(sequence + 0x168) = 0;
    head = *(u8 **)(context + 0x224);
    if (head != 0) {
        *(u8 **)(head + 0x168) = sequence;
        *(u8 **)(sequence + 0x16C) = *(u8 **)(context + 0x224);
    } else {
        *(u8 **)(sequence + 0x16C) = 0;
    }
    *(u8 **)(context + 0x224) = sequence;
    btlDispatchStateHandler((s32)sequence, 0);
    btlBossDebugPrintf(D_003A3788, sequence);
    return sequence;
}

extern char D_003A37A8[]; /* "btl:action seq delete[%p]\n" */

void btlDestroyActionSeq(s32 object) {
    btlBossDebugPrintf(D_003A37A8, object);
    btlReleaseObjectBuffers(object + 0x20);
    if (*(s32 *)(object + 0x16C) != 0) {
        *(s32 *)(*(s32 *)(object + 0x16C) + 0x168) = *(s32 *)(object + 0x168);
    }
    if (*(s32 *)(object + 0x168) != 0) {
        *(s32 *)(*(s32 *)(object + 0x168) + 0x16C) = *(s32 *)(object + 0x16C);
    } else {
        s32 context = btlGetRuntime();
        *(s32 *)(context + 0x224) = *(s32 *)(object + 0x16C);
    }
    sdfReleaseChipBlock(object);
}

void btlUpdateActionSeqs(void) {
    s32 action = *(s32 *)(btlGetRuntime() + 0x224);
    while (action != 0) {
        s32 flags = *(s32 *)(action + 8);
        s32 next = *(s32 *)(action + 0x16C);
        if (flags & 1) {
            D_00359B28[*(s32 *)action].update(action);
            *(s32 *)(action + 0x10) += 1;
        } else if (flags & 2) {
            btlDestroyActionSeq(action);
        }
        action = next;
    }
}

void btlDestroyAllActionSeqs(void) {
    u8 *node = *(u8 **)(btlGetRuntime() + 0x224);
    while (node != 0) {
        u8 *next = *(u8 **)(node + 0x16C);
        btlDestroyActionSeq((s32)node);
        node = next;
    }
}

s32 btlFindUnitByActor(s32 target) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x224);
    while (node != 0) {
        if (*(s32 *)(node + 0x18) == target) {
            return node;
        }
        node = *(s32 *)(node + 0x16C);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3778);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3788);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A37A8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D09B8);

void btlDispatchCommandViaHookOrDefault(u8 *command, u8 *argument) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x604);

    if (handler != 0) {
        s32 result = handler((s32)command, (s32)argument);
        if (result != -1) {
            btlDispatchStateHandler((s32)command, result);
            return;
        }
    }
    switch (*(s32 *)argument) {
    case 1:
        btlDispatchStateHandler((s32)command, 0xD);
        break;
    case 4:
        *(s32 *)(argument + 4) = btlGetLoggedIndexedCommandItem(*(s32 *)(argument + 8));
        /* fallthrough */
    case 2:
    case 3:
    case 7:
    case 8:
        if (*(s8 *)(datCommandSelectors + *(s32 *)(argument + 4) * 2 + 1) != 1) {
            btlDispatchStateHandler((s32)command, 0xE);
        } else {
            if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x200) {
                scrSetGlobalSeenBit(*(u16 *)(argument + 4));
            }
            btlDispatchStateHandler((s32)command, 0xF);
        }
        break;
    case 5:
        if (*(u32 *)(*(s32 *)(command + 0x18) + 0x110) & 0x1000) {
            btlDispatchStateHandler((s32)command, 0x10);
        } else {
            btlDispatchStateHandler((s32)command, 0x11);
        }
        break;
    case 10:
    case 13:
    case 14:
        btlDispatchStateHandler((s32)command, 0x13);
        break;
    case 9:
        if (*(u32 *)(*(s32 *)(command + 0x34) + 0x110) & 1) {
            btlDispatchStateHandler((s32)command, 0x16);
        } else {
            btlDispatchStateHandler((s32)command, 0x15);
        }
        break;
    case 12:
        btlDispatchStateHandler((s32)command, 0x15);
        break;
    case 6: {
        u32 flags = *(u32 *)(*(s32 *)(command + 0x18) + 0x110);
        if (flags & 0x200) {
            btlDispatchStateHandler((s32)command, 0x17);
        } else if (flags & 0x400) {
            btlDispatchStateHandler((s32)command, 0x14);
        }
        break;
    }
    case 11:
        btlDispatchStateHandler((s32)command, 0x14);
        break;
    case 15:
        btlDispatchStateHandler((s32)command, 0x18);
        break;
    case 16:
        btlDispatchStateHandler((s32)command, 0x19);
        break;
    case 17:
        btlDispatchStateHandler((s32)command, 0x1F);
        break;
    }
}

s32 btlIsSupportedCommandKind(s32 *state) {
    switch (*state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

s32 btlResolveActionOperand(u8 *actor, s32 *argument) {
    switch (argument[0]) {
    case 1:
        if ((*(u64 *)(actor + 0x110) & 0x1200) == 0x200) {
            return btlGetActorBedAssetIdFromIndex(*(u16 *)(actor + 0x172));
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        return 0;
    case 4:
        return btlGetLoggedIndexedCommandItem(argument[2]);
    case 2:
    case 3:
    case 7:
    case 8:
        return argument[1];
    default:
        return -1;
    }
}

u32 btlClassifyActionOperand(u8 *actor, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = btlGetIndexListCount(*(s32 *)(argument + 0x40));
        if ((*(u64 *)(actor + 0x110) & 0x1200) == 0x1200 &&
            (*(u16 *)(actor + 0x12E) & 0x1000) == 0 &&
            count == 1) {
            u8 *option = *(u8 **)(argument + 0x60);
            if (*(s32 *)(option + 0xC) == 2 && option[0x14] == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (*(u32 *)(actor + 0x110) & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8:
        return *(u8 *)(datActionAnimationRecords + *(s32 *)(argument + 4) * 0x20);
    default:
        return 0;
    }
}

s32 btlClassifyActionResult(u8 *arg0, u32 arg1, s32 arg2, u32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 code;

    btlGetEntryFlagsUnlessDisabled((s32)(arg0 + 0x120));
    if (arg6 >= 0) {
        switch (*(u32 *)(datCommandRecords + arg6 * 56 + 0x30)) {
        case 1:
        case 2:
        case 9:
        case 10:
            return -1;
        }
    }
    if ((*(u32 *)(datCommandRecords + arg6 * 56 + 0x24) & 0x400000FF) == 0x40000002) {
        return -1;
    }
    if (arg1 & 0x50004) {
        return -1;
    }
    if (arg3 & 0xE0001) {
        code = -1;
    } else if ((*(u32 *)(arg0 + 0x110) & 0x200) != 0 && arg2 == 2 && arg4 == 1 && arg5 == 0) {
        code = 0x12;
    } else {
        code = 1;
    }
    if (arg5 != 0 && (*(u64 *)(arg0 + 0x110) & 0x4000000200) == 0x200) {
        code = 0xB;
    }
    if ((arg1 & 0x20001) == 0) {
        code = -1;
    }
    return code;
}

s32 btlActionEntryIsEmpty(s32 index, u8 *slot, u8 *entry) {
    s32 kind;

    if (index >= 0) {
        switch (*(s32 *)(datCommandRecords + index * 56 + 0x30)) {
        case 1:
        case 2:
        case 9:
        case 10:
        case 12:
        case 13:
        case 14:
        case 15:
            return 0;
        }
    }
    if (slot != 0) {
        kind = *(s32 *)(slot + 8);
        if (kind == 2 || kind == 0x10000) {
            return 0;
        }
    }
    if ((*(u16 *)(entry + 0x26) & 1) != 0) {
        return 0;
    }
    if ((*(u16 *)(entry + 0x26) & 2) != 0) {
        return 0;
    }
    if (*(s32 *)(entry + 0x00) == 0) {
        if (*(s32 *)(entry + 0x04) == 0) {
            if (*(s32 *)(entry + 0x08) == 0) {
                if (*(s32 *)(entry + 0x0C) == 0) {
                    if (*(s32 *)(entry + 0x1C) == 0) {
                        if (*(s32 *)(entry + 0x20) == 0) {
                            if (*(s32 *)(entry + 0x10) == 0) {
                                if (*(s32 *)(entry + 0x18) == 0) {
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D1218);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D12A0);

typedef struct BattleIndexEntry {
    u8 unk0;
    u8 pad1[7];
    s32 unk8;
    u8 padC[4];
    u8 unk10;
    u8 pad11[3];
    u8 unk14;
} BattleIndexEntry;

typedef struct BattleIndexWork {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24[9];
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u16 unk30;
    u8 pad32[2];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 indices;
    u8 pad44[0xC];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 unk5C;
    u8 unk5E;
    u8 pad5F;
    u32 device;
} BattleIndexWork;

void btlResetIndexWork(BattleIndexWork *work) {
    u32 i;
    s32 offset;
    BattleIndexEntry *entry;
    BattleIndexEntry *next;
    work->unk00 = -1;
    work->unk04 = -1;
    work->unk08 = -1;
    work->unk0C = 0;
    work->unk10 = 0;
    work->unk14 = 0;
    work->unk18 = -1;
    work->unk1C = 0;
    work->unk20 = 8;
    work->unk30 = 0;
    work->unk34 = 0;
    work->unk38 = -1;
    work->unk3C = 0;
    work->unk2D = 0;
    work->unk2E = 0;
    work->unk50 = 0;
    work->unk54 = 0;
    work->unk58 = 0;
    work->unk5C = 0;
    work->unk5E = 0;
    for (i = 0, offset = 0; i < 13; i++) {
        *(u8 *)(offset + work->device) = 0;
        entry = (BattleIndexEntry *)(offset + work->device);
        entry->unk8 = 0;
        entry->unk14 = 0;
        next = (BattleIndexEntry *)(offset + work->device);
        offset += 0xA1C;
        next->unk10 = 0;
    }
    btlClearIndexList(work->indices);
}

void btlInitBattleIndexWork(u8 *object) {
    u32 handle;
    u32 value;
    *(u32 *)(object + 0x40) = (u32)btlAllocateIndexList(13);
    handle = func_002D03F8(0x836C);
    value = sdfResourceRetainAddress(handle);
    *(u32 *)(object + 0x64) = handle;
    *(u32 *)(object + 0x60) = value;
    *(u64 *)(object + 0x48) = 0;
    btlResetIndexWork(object);
}

extern void btlFreeIndexList();

void btlReleaseObjectBuffers(u8 *object) {
    u32 handle = *(u32 *)(object + 0x64);
    if (handle != 0) {
        func_002D0918(handle);
        *(u32 *)(object + 0x64) = 0;
    }
    if (*(u32 *)(object + 0x40) != 0) {
        btlFreeIndexList(*(u32 *)(object + 0x40));
        *(u32 *)(object + 0x40) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D2C78);

typedef struct BattleDeltaSpec {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u32 unk_24;
} BattleDeltaSpec;

extern u32 func_001D2C78(void *);

extern void *btlAllocTask(s32);

extern u32 btlGetTaskArguments(s32);

void *btlCreateActorParameterDeltaTask(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x45;
    *(void **)(task + 0x4C) = func_001D2C78;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}

u32 btlApplyDeferredActorStats(u8 *arguments) {
    s32 context = btlGetRuntime();
    u8 *actor = *(u8 **)arguments;
    s32 primary;
    u8 *resource;
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    primary = *(s32 *)(arguments + 0x20);
    if (primary == 0 && *(s32 *)(arguments + 0x24) == 0) {
        return 1;
    }
    if (*(u32 *)(actor + 0x110) & 0x60) {
        return 1;
    }
    resource = actor + 0x120;
    func_001A1868(resource, primary);
    func_001A1880(resource, *(s32 *)(arguments + 0x24));
    func_001D5990(actor);
    btlIsUnitDefeatTriggeredByValueDelta(actor, 0);
    return 1;
}

void *btlCreateDeferredActorStatsTask(u8 *owner, BattleDeltaSpec *spec) {
    u8 *task = btlAllocTask(0x2C);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x46;
    *(void **)(task + 0x4C) = btlApplyDeferredActorStats;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(BattleDeltaSpec *)(arguments + 1) = *spec;
    return task;
}

u32 btlApplyDeferredUnitStatus(void *argument) {
    u32 *args = (u32 *)argument;
    s32 context = btlGetRuntime();
    u8 *owner = (u8 *)args[0];
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    func_001A1948(owner + 0x120, args[1]);
    func_001D5990(owner);
    btlIsUnitDefeatTriggeredByValueDelta(owner, 0);
    return 1;
}

extern void *btlAllocTask(s32);

extern u32 btlGetTaskArguments(s32);

extern u32 btlApplyDeferredUnitStatus(void *);

void *btlCreateDeferredUnitStatusTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x47;
    *(void **)(task + 0x4C) = btlApplyDeferredUnitStatus;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlApplyCategoryStatDamage);

extern u32 btlApplyCategoryStatDamage(void *);

void *btlCreateCategoryStatDamageTask(u8 *owner, u32 value, u32 extra) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApplyCategoryStatDamage;
    *(u16 *)(task + 0x20) = 0x48;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = value;
    arguments[1] = extra;
    return task;
}

s32 func_001D33D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    func_001A4C68(*(s32 *)temp_v0, *(s32 *)(temp_v0 + 0x14), *(s16 *)(temp_v0 + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D3400);

u32 btlApplyQueuedActorEntrySelection(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        btlSetActorSelectedEntryIndex(*arg0, arg0[7]);
        func_001D5990(*arg0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D3510);

u32 func_001D35E0(u32 *arg0) {
    btlClearActorSelectedEntryIndex(*arg0);
    func_001D5990(*arg0);
    return 1;
}

void *func_001D3618(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D35E0;
    *(u16 *)(task + 0x20) = 0x4B;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D3688);

extern void func_001D3688();

u8 *btlCreateActorSoundOptionTask(u8 *arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4C;
    *(void **)(task + 0x4C) = func_001D3688;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 func_001D3A20(s32 arg0) {
    if ((*(u8 *)(*(s32 *)(arg0 + 4) * 8 + datItemSkillRecords + 1) & 4) != 0) {
        func_00119900(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
    }
    return 1;
}

u8 *btlCreatePermittedBattleVoiceTask(s32 arg0, s32 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4D;
    *(void **)(task + 0x4C) = func_001D3A20;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 btlPlayQueuedBattleVoice(s32 arg0) {
    func_00119900(*(u16 *)(arg0 + 4), 1);
    return 1;
}

u8 *btlCreateQueuedBattleVoiceTask(u8 *arg0, u16 arg1) {
    u8 *task = btlAllocTask(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4E;
    *(void **)(task + 0x4C) = btlPlayQueuedBattleVoice;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    ((u16 *)data)[2] = arg1;
    return task;
}

u32 btlAddEpFromPacket(s32 arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if (*(u32 *)(((s32 *)arg0)[0] + 0x110) & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2CC) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:epall=%d[%d](packet)\n", *(s32 *)(context + 0x2CC), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

u8 *btlScheduleEpPacketTask(u8 *owner, s32 value) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4F;
    *(void **)(task + 0x4C) = btlAddEpFromPacket;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlAddMoneyFromPacket(void *arg0) {
    s32 context = btlGetRuntime();

    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if (*(u32 *)(((s32 *)arg0)[0] + 0x110) & 0x400) {
        return 1;
    }
    *(s32 *)(context + 0x2C0) += ((s32 *)arg0)[1];
    btlBossDebugPrintf("btl:money=%d[%d](packet)\n", *(s32 *)(context + 0x2C0), ((s32 *)arg0)[1]);
    return 1;
}

extern u32 btlAddMoneyFromPacket(void *);

void *btlScheduleMoneyPacketTask(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x50;
    *(void **)(task + 0x4C) = btlAddMoneyFromPacket;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 btlRefreshEligibleActors(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u8 *actor = *(u8 **)(context + 0x228);
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 &&
                    (u16)(*(u16 *)(actor + 0x124) - 1) < 0x17F) {
                    u32 entry = *(u32 *)(datEnemyRecords + *(u16 *)(actor + 0x124) * 76);
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((*(u32 *)(actor + 0x114) & 8) == 0) {
                                u16 prior = *(u16 *)(actor + 0x12E);
                                func_001A1948(actor + 0x120, 1);
                                func_001D5990(actor);
                                if (*(u16 *)(actor + 0x12E) == 1 &&
                                    prior != *(u16 *)(actor + 0x12E)) {
                                    *(u32 *)(actor + 0x114) |= 4;
                                    *(u32 *)(context + 0x1F8) |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return 1;
}

void *btlCreateRefreshEligibleActorsTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = btlRefreshEligibleActors;
    *(u16 *)(task + 0x20) = 0x51;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}

extern char D_003A3A40[];

extern char D_003A3A50[];

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)btlGetRuntime();
    u32 flags = *(u32 *)(context + 0x1F4);
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if (D_00324510.edge22 < 0 || D_00324510.edge23 < 0) {
            *(u32 *)(context + 0x1F4) = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            btlSetTrackedTaskDisplayMode(0);
            btlBossDebugPrintf(D_003A3A40);
        }
    } else if (D_00324510.edge22 < 0) {
        *(u32 *)(context + 0x1F4) = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        btlSetTrackedTaskDisplayMode(1);
        btlBossDebugPrintf(D_003A3A50);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D3FE8);

void btlFindSoundTaskByWorkValue(void) {
}

s32 btlFindTaskByHandle(s64 owner) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x230);
    while (node != 0) {
        if (*(s64 *)(node + 0x38) == owner) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 btlFindTaskByOwner(s64 owner) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x230);
    while (node != 0) {
        if (*(s64 *)(node + 0x40) == owner) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 btlFindTaskByKind(u16 kind) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x230);
    while (node != 0) {
        if (*(u16 *)(node + 0x20) == kind) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 btlCountRegisteredTasks(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = btlGetRuntime();
    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(temp_v0 + 0x230); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

s32 btlCountTasksForOwner(s64 key) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x230);
    s32 count = 0;
    while (node != 0) {
        s64 owner = *(s64 *)(node + 0x40);
        node = *(s32 *)(node + 0x58);
        if (owner == key) {
            count++;
        }
    }
    return count;
}

s32 btlCountTasksByKind(u16 kind) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x230);
    s32 count = 0;
    while (node != 0) {
        u16 nodeKind = *(u16 *)(node + 0x20);
        node = *(s32 *)(node + 0x58);
        if (nodeKind == kind) {
            count++;
        }
    }
    return count;
}

void btlFlagTasksForUpdate(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x22C);
    while (node != 0) {
        u16 flags = *(u16 *)(node + 0x24);
        s32 next = *(s32 *)(node + 0x5C);
        if ((flags & 1) != 0) {
            *(u16 *)(node + 0x24) = flags | 4;
        }
        node = next;
    }
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3A40);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3A50);

/* A battle task start condition: `kind` selects what is looked up and how its presence or state is tested. */
typedef struct TaskCondition {
    u8 kind;           /* 0x00 */
    u8 pad01[7];
    union {
        s32 count;     /* kind 2: threshold the value must reach */
        s64 handle;    /* kinds 3-5: task handle */
        s64 owner;     /* kinds 6-8: task owner */
        u16 taskKind;  /* kinds 9-10: task kind */
    } value;           /* 0x08 */
} TaskCondition;

s32 btlEvalTaskCondition(TaskCondition *condition, s32 value) {
    s32 result = 0;
    s32 task;

    switch (condition->kind) {
    case 0:
        break;
    case 1:
        result = 1;
        break;
    case 2:
        if (!(value < condition->value.count)) {
            result = 1;
        }
        break;
    case 3:
        if (btlFindTaskByHandle(condition->value.handle) != 0) {
            result = 1;
        }
        break;
    case 4:
        if (btlFindTaskByHandle(condition->value.handle) == 0) {
            result = 1;
        }
        break;
    case 5:
        task = btlFindTaskByHandle(condition->value.handle);
        if (task != 0) {
            if (*(u16 *)(task + 0x22) == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 6:
        if (btlFindTaskByOwner(condition->value.owner) != 0) {
            result = 1;
        }
        break;
    case 7:
        if (btlFindTaskByOwner(condition->value.owner) == 0) {
            result = 1;
        }
        break;
    case 8:
        task = btlFindTaskByOwner(condition->value.owner);
        if (task != 0) {
            if (*(u16 *)(task + 0x22) == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 9:
        if (btlFindTaskByKind(condition->value.taskKind) != 0) {
            result = 1;
        }
        break;
    case 10:
        result = btlFindTaskByKind(condition->value.taskKind) == 0;
        break;
    }
    return result;
}

void *btlAllocTask(s32 size) {
    u8 *task = sdfAllocAndClearQuadwords(size + 0x70);
    u8 *context;
    u8 *head;

    if (size > 0) {
        *(u8 **)(task + 0x54) = task + 0x70;
    } else {
        *(u8 **)(task + 0x54) = 0;
    }
    context = (u8 *)btlGetRuntime();
    *(u8 **)(task + 0x58) = 0;
    head = *(u8 **)(context + 0x22C);
    if (head != 0) {
        *(u8 **)(head + 0x58) = task;
        *(u8 **)(task + 0x5C) = *(u8 **)(context + 0x22C);
    } else {
        *(u8 **)(context + 0x230) = task;
        *(u8 **)(task + 0x5C) = 0;
    }
    *(u8 **)(context + 0x22C) = task;
    *(u16 *)(task + 0x24) |= 1;
    return task;
}

u32 btlGetTaskArguments(s32 arg0) {
    return *(u32 *)(arg0 + 0x54);
}

void btlFreeTask(s32 task) {
    s32 context;
    s32 next;
    s32 previous;
    void (*cleanup)(s32);
    cleanup = *(void (**)(s32))(task + 0x50);
    if (cleanup != 0) {
        cleanup(*(s32 *)(task + 0x54));
    }
    context = btlGetRuntime();
    previous = *(s32 *)(task + 0x5C);
    if (previous != 0) {
        *(s32 *)(previous + 0x58) = *(s32 *)(task + 0x58);
    } else {
        *(s32 *)(context + 0x230) = *(s32 *)(task + 0x58);
    }
    next = *(s32 *)(task + 0x58);
    if (next != 0) {
        *(s32 *)(next + 0x5C) = *(s32 *)(task + 0x5C);
    } else {
        *(s32 *)(context + 0x22C) = *(s32 *)(task + 0x5C);
    }
    sdfReleaseChipBlock(task);
}

u64 btlStartTask(s32 task) {
    u64 value = btlAdvanceRuntimeSequenceCounter();
    s32 callback = *(s32 *)(task + 0x48);
    *(u16 *)(task + 0x24) |= 8;
    *(u64 *)(task + 0x38) = value;
    *(u32 *)(task + 0x30) = 0;
    *(u32 *)(task + 0x34) = 0;
    *(u16 *)(task + 0x22) = 0;
    *(u32 *)(task + 0x60) = 0;
    *(u32 *)(task + 0x64) = 0;
    if (callback != 0) {
        ((void (*)(s32))callback)(*(s32 *)(task + 0x54));
    }
    return *(u64 *)(task + 0x38);
}

void btlResetDeferredTaskQueue(void) {
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlRunTask);

extern void btlRunTask(s32);

void btlSweepFinishedTasks(void) {
    SoundTask *task = *(SoundTask **)(btlGetRuntime() + 0x230);
    SoundTask *next;
    while (task != 0) {
        next = task->next;
        if (!(task->flags & 2)) {
            btlRunTask((s32)task);
        } else {
            task->deferNext = 0;
            if (btlDeferredTaskTail != 0) {
                btlDeferredTaskTail->deferNext = task;
                task->deferPrev = btlDeferredTaskTail;
            } else {
                btlDeferredTaskHead = task;
                task->deferPrev = 0;
            }
            btlDeferredTaskTail = task;
        }
        task = next;
    }
}

void btlClearDeferredTasks(void) {
    s32 node = btlDeferredTaskHead;
    while (node != 0) {
        s32 next = *(s32 *)(node + 0x60);
        btlRunTask(node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

void btlClearTaskLists(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x22C);
    while (node != 0) {
        s32 next = *(s32 *)(node + 0x5C);
        btlFreeTask(node);
        node = next;
    }
    btlDeferredTaskTail = 0;
    btlDeferredTaskHead = 0;
}

u32 func_001D4B28(void) {
    return 1;
}

void *btlCreateImmediateCompletionTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = func_001D4B28;
    *(u16 *)(task + 0x20) = 0x63;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}

extern s32 btlBossDebugPrintf(const char *, ...);

void btlDumpTaskQueue(void) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x230);
    while (node != 0) {
        btlBossDebugPrintf("btl:packet[%d]\n", *(u16 *)(node + 0x20));
        node = *(s32 *)(node + 0x58);
    }
    btlBossDebugPrintf("btl:packet head[%p]\n", *(void **)(context + 0x22C));
    btlBossDebugPrintf("btl:packet tail[%p]\n", *(void **)(context + 0x230));
}

extern u128 D_00359CC0;

void btlInitUnitFxDefaults(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x90, &D_00359CC0);
    *(f32 *)(fx + 0xB0) = 220.0f;
    *(f32 *)(fx + 0xB4) = 80.0f;
    *(f32 *)(fx + 0xC0) = 75.0f;
}

extern u128 D_00359CD0;

extern u128 D_00359CE0;

void btlInitFxLights(u8 *fx) {
    PCP_COPY_VECTOR(fx + 0x30, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x40, &D_00359CE0);
    *(u32 *)(fx + 0x58) = 0;
    *(f32 *)(fx + 0x50) = 1.0f;
    *(u32 *)(fx + 0x54) = 0x80808080;
    PCP_COPY_VECTOR(fx + 0x60, &D_00359CD0);
    PCP_COPY_VECTOR(fx + 0x70, &D_00359CE0);
    *(f32 *)(fx + 0x80) = 1.0f;
    *(u32 *)(fx + 0x84) = 0x80808080;
    *(u32 *)(fx + 0x88) = 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlInitializeEffectVectorsFromSourceRecords);

extern s32 func_002183D0(s32);

extern s32 func_002183E0(s32);

s32 btlHasMatchingModel(s32 effect, s32 model) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x228);
    while (node != 0) {
        if ((*(u32 *)(node + 0x110) & 2) != 0 &&
            *(s32 *)(node + 0x320) != 0 &&
            *(s32 *)(node + 0x308) != 0 &&
            func_002183D0(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == effect &&
            func_002183E0(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == model) {
            return 1;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D4E60);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D4E98);

extern const char D_003A3AD0[];

void btlReleaseActorModelResources(u8 *object) {
    s32 sound;
    s32 load;
    s32 model;
    u32 state;
    u32 flags;
    if (object[0xCC] == 0) {
        sound = *(s32 *)(object + 0x308);
        if (sound != 0) {
            sndReleaseSlotOwner(sound);
            *(s32 *)(object + 0x308) = 0;
        }
        load = *(s32 *)(object + 0x324);
        if (load != 0) {
            sdfReleaseDevSlot(load, 1, 1);
            *(s32 *)(object + 0x324) = 0;
            btlBossDebugPrintf(D_003A3AD0, object);
        }
        model = *(s32 *)(object + 0x31C);
        if (model != 0) {
            dds3RemoveWorldObjectNode(model);
            *(s32 *)(object + 0x31C) = 0;
            *(s32 *)(object + 0x320) = 0;
        }
    } else {
        *(s32 *)(object + 0x308) = 0;
        *(s32 *)(object + 0x324) = 0;
        *(s32 *)(object + 0x31C) = 0;
        *(s32 *)(object + 0x320) = 0;
    }
    state = *(u32 *)(object + 0x118) & ~1;
    flags = *(u32 *)(object + 0x110) & ~2;
    state &= ~2;
    *(u32 *)(object + 0x110) = flags;
    *(u32 *)(object + 0x118) = state;
}

void btlRequestModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_002118D8(arg1, arg2);
        return;
    }
    mdlRequestAsset(arg1, arg2, 0);
}

void btlReleaseModelAssetByMode(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        btlReleaseFoundModelEntry(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlCheckModelAssetByMode);

void btlFlagUnitDefeatCandidate(u8 *unit) {
    s32 (*hook)(u8 *) = *(s32 (**)(u8 *))((u8 *)btlGetRuntime() + 0x654);
    if (hook == 0 || hook(unit) != 0) {
        *(u32 *)(unit + 0x110) |= 4;
        if (!(*(u32 *)(unit + 0x110) & 0x8000000)) {
            *(u32 *)(unit + 0x110) |= 8;
            if (*(u32 *)(unit + 0x110) & 2) {
                **(u32 **)(*(u8 **)(unit + 0x320) + 0x8C) &= ~1;
            }
        }
    }
}

void btlClearUnitDefeatCandidate(u8 *object) {
    s32 (*callback)(u8 *);
    u32 flags;
    u32 masked;

    callback = *(s32 (**)(u8 *))(btlGetRuntime() + 0x658);
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = *(u32 *)(object + 0x110);
    masked = flags & ~4;
    masked &= ~8;
    *(u32 *)(object + 0x110) = masked;
    if ((flags & 2) != 0) {
        u32 *resource = *(u32 **)(*(u8 **)(object + 0x320) + 0x8C);
        *resource |= 1;
    }
}

u32 btlIsUnitInfoFlagOneEligible(u8 *object) {
    u32 flags = *(u32 *)(object + 0x110);
    if ((flags & 0x8000000) != 0) {
        return 0;
    }
    if ((flags & 1) == 0) {
        return 0;
    }
    if ((flags & 2) == 0) {
        return 0;
    }
    return **(u8 **)(*(u8 **)(object + 0x320) + 0x8C) & 1;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3AD0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D5578);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D5990);

s32 btlIsActorModeAcceptedByBattleHook(u8 *object) {
    s32 value;
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    {
        s32 (*callback)(s32) = *(s32 (**)(s32))(btlGetRuntime() + 0x5A4);
        if (callback != 0 && callback(0) == *(s32 *)(object + 0xEC)) {
            return 1;
        }
    }
    value = *(s32 *)(object + 0xEC);
    switch (value) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    }
    return 0;
}

extern void func_001D5578(u8 *, s32, s32, f32);

void btlApplyScaledUnitEffectParameter(u8 *object, s32 index, s32 argument, f32 scale) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    f32 value = *(f32 *)(resource + index * 20 + 0x34);
    func_001D5578(object, index, argument, value * scale);
}

s32 btlGetSlotRateKind(u8 *object, s32 index) {
    u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    s32 value = *(s16 *)(resource + index * 20 + 0x30);

    switch (value) {
    case 0:
        return 0;
    case 1:
    case 2:
    case 3:
        return 2;
    }
    return 0;
}

void btlUpdateUnitEffects(void) {
    s32 context = btlGetRuntime();
    u8 *object = *(u8 **)(context + 0x228);

    while (object != 0) {
        if (*(u32 *)(object + 0x110) & 2) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xC4),
                                                  *(s32 *)(object + 0xC8));
            s32 model = *(s32 *)(*(s32 *)(object + 0x320) + 0x8C);
            s32 node = mdlGetNodeField2C(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                btlIsActorModeAcceptedByBattleHook(object) == 0) {
                func_001D5990(object);
                func_001D5578(object, *(s32 *)(object + 0xFC),
                              *(s32 *)(object + 0x100),
                              *(f32 *)(object + 0x104));
            }
        }
        object = *(u8 **)(object + 0x344);
    }
}

void btlApplyUnitModelScaledValue(u8 *object) {
    s32 context;
    u8 *resource;
    f32 volume;
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    context = btlGetRuntime();
    *(u32 *)(object + 0xE8) &= ~1;
    volume = *(f32 *)(object + 0xF4);
    resource = *(u8 **)(object + 0x320);
    *(f32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) =
        volume * (30.0f / (f32)*(s8 *)(context + 0x490));
}

void btlResetUnitModelProgress(u8 *object) {
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        u8 *resource = *(u8 **)(object + 0x320);
        *(u32 *)(object + 0xE8) |= 1;
        *(u32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D6050);

f32 btlGetUnitModelValue1C(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0.0f;
    }
    return *(f32 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x1c);
}

void btlAdvanceUnitModelFrame(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_002DB538(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 800) + 0x8c) + 0x1c));
        return;
    }
}

s32 btlGetUnitModelFrameCount(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0;
    }
    return *(u16 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x2e);
}

void btlSeekUnitModelFrameZero(s32 arg0) {
    extern void func_002DB538(void *, f32);

    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return;
    }
    func_002DB538(*(void **)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c), 0.0f);
}

void btlSeekRandomModelFrame(u8 *object) {
    s32 duration;
    u32 randomFrame;
    f32 frame;
    u8 *resource;
    extern void func_002DB538(void *, f32);

    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    duration = btlGetUnitModelFrameCount((s32)object);
    if (duration > 0) {
        randomFrame = (u32)effMiscRandMod(0, duration);
        frame = (f32)randomFrame;
        resource = *(u8 **)(object + 0x320);
        func_002DB538(*(void **)(*(u8 **)(resource + 0x8C) + 0x1C),
                       frame);
    }
}

u32 btlIsUnitModelStateFive(s32 object) {
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 1;
    }
    if (*(s32 *)(object + 0xF0) != 2) {
        return 1;
    }
    return *(u8 *)(*(s32 *)(*(s32 *)(*(s32 *)(object + 0x320) + 0x8C) + 0x1C) + 0x30) == 5;
}

void btlSetUnitPosition(u8 *object, void *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((*(u32 *)(object + 0x114) & 0x80) != 0) {
        return;
    }
    context = btlGetRuntime();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(position));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x60) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder" : : "r"(context));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(world) : "memory");
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        world[2] += *(f32 *)(object + 0x88);
        effObjSetInnerFirstVec(*(s32 *)(object + 0x31C), world);
    }
}

void func_001D6300(u8 *object, void *position) {
    PCP_COPY_VECTOR(position, object + 0x60);
}

void btlGetUnitWorldPos(u8 *object, void *worldPosition) {
    s32 context = btlGetRuntime();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\tlqc2 vf11, 0(%1)\n\tvadd.xyzw vf10, vf10, vf11\n\tsqc2 vf10, 0(%2)\n\t.set reorder"
                     : : "r"(object + 0x60), "r"(context), "r"(worldPosition) : "memory");
}

extern void btlRefreshUnitFxVectors(u8 *);

s32 btlSetActorEffectParameter(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(btlGetRuntime() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)sdfLoadMapRecordPositionVector(*(s32 *)(effect + 0x18), value);
    }
}

void btlSetActorEffectParameterOrMuzzlePosition(u32 arg0, s32 arg1) {
    s64 temp_v0;

    temp_v0 = btlSetActorEffectParameter();
    if (temp_v0 == 0) {
        btlUnitGetMuzzlePosVU(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D6428);

extern s32 sdfLoadMapRecordLookAtBasis(s32, s32);

s32 btlSetActorAlternateEffectParameter(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(btlGetRuntime() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    btlRefreshUnitFxVectors(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)sdfLoadMapRecordLookAtBasis(*(s32 *)(effect + 0x18), value);
    }
}

void btlSetAlternateEffectParameterOrMuzzlePosition(void) {
    if (btlSetActorAlternateEffectParameter() != 0) {
        return;
    }
    __asm__ volatile(".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\tvmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder");
}

s32 btlIsUnitAtStoredPosition(u8 *object) {
    f32 position[3];
    func_001D6300(object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_003A3B70[];

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerSecondVec(s32, void *);

void btlSetUnitRotation(u8 *object, void *rotation) {
    u8 vector[16];
    if ((*(u32 *)(object + 0x114) & 0x100) != 0) {
        return;
    }
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(rotation));
    if ((*(u32 *)(object + 0x110) & 0x10) != 0) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_003A3B70));
        effMiscQuatMultiplyVU();
    }
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x70));
    __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_003A3B70));
    effMiscQuatMultiplyVU();
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(vector));
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        effObjSetInnerSecondVec(*(s32 *)(object + 0x31C), vector);
    }
}

void btlCopyUnitRotationQuaternion(u8 *object, void *position) {
    PCP_COPY_VECTOR(position, object + 0x70);
}

extern void func_00221E08(u32, s32, u32);

void btlSetUnitColor(u8 *unit, u32 color, s32 mode) {
    if (*(u32 *)(unit + 0x110) & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        *(u32 *)(unit + 0x54) = (*(u32 *)(unit + 0x54) & 0xFF000000) | (color & 0xFFFFFF);
        func_00221E08(*(u32 *)(unit + 0x320), mode, color);
    }
}

void btlBlendUnitColor(u8 *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (*(u32 *)(unit + 0x110) & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (*(u32 *)(unit + 0x54) & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        *(u32 *)(unit + 0x84) = (*(u32 *)(unit + 0x84) & 0xFF000000) | (color & 0xFFFFFF);
        func_00221E08(*(u32 *)(unit + 0x320), mode, blended);
    }
}

void btlReleaseUnitModelColorResource(s32 arg0, s32 arg1) {
    mdlReleaseInnerResourceHandle(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c), (arg1 & 0xffffff) | 0x80000000);
}

extern void effObjFetchInnerFirstVec(u32);

extern void effObjFetchInnerSecondVecNorm(u32);

extern void mdlStorePrimaryVectorVU(u32);

extern void mdlUpdateContextRotationBasisFromQuaternion(u32);

extern void sdfModelUpdateCurrentFrameTransforms(u32);

void btlRefreshUnitFxVectors(u8 *unit) {
    if (!(*(u32 *)(unit + 0x110) & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(*(u32 *)(unit + 0x31C));
    mdlStorePrimaryVectorVU(*(u32 *)(*(u8 **)(unit + 0x320) + 0x8C));
    effObjFetchInnerSecondVecNorm(*(u32 *)(unit + 0x31C));
    mdlUpdateContextRotationBasisFromQuaternion(*(u32 *)(*(u8 **)(unit + 0x320) + 0x8C));
    sdfModelUpdateCurrentFrameTransforms(*(u32 *)(*(u8 **)(*(u8 **)(unit + 0x320) + 0x8C) + 0x18));
}

extern void btlUnitGetBodyPosVU(u8 *);

extern s32 btlAimHorizontalDirectionVU(void *, void *);

void btlUnitFaceTarget(u8 *object, u8 *target) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        VU_STORE10(first);
        btlUnitGetBodyPosVU(target);
        VU_STORE10(second);
        if (btlAimHorizontalDirectionVU(first, second) != 0) {
            VU_STORE10(result);
            btlSetUnitRotation(object, result);
        }
    }
}

extern s32 func_001F7868(void *, void *, f32);

void btlUnitFaceTargetScaled(u8 *object, u8 *target, f32 scale) {
    u8 first[16];
    u8 second[16];
    u8 result[16];
    if ((*(u32 *)(object + 0x110) & 0x80000) != 0) {
        btlUnitGetBodyPosVU(object);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(first));
        btlUnitGetBodyPosVU(target);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(second));
        func_001F7868(first, second, scale);
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(result));
        btlSetUnitRotation(object, result);
    }
}

typedef struct BtlUnitStats {
    u32 word[0x69];
} BtlUnitStats;

void btlCopyUnitStats(s32 arg0, s32 arg1) {
    BtlUnitStats *stats = (BtlUnitStats *)(arg0 + 0x120);
    *stats = *(BtlUnitStats *)arg1;
    btlRefreshUnitMaximumHpAndClampCurrentHp((s32)stats);
    btlRefreshUnitMaximumMpAndClampCurrentMp((s32)stats);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D6A80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3B70);

void btlCreateUnitTransparency(u8 *unit) {
    u8 *shape;
    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return;
    }
    if (*(s32 *)(unit + 0x324) != 0) {
        return;
    }
    if (*(u8 *)(unit + 0xCC) != 0) {
        return;
    }
    shape = *(u8 **)(*(u8 **)(*(u8 **)(unit + 0x320) + 0x8C) + 0xC);
    *(s32 *)(unit + 0x324) = sdfModelCreateWithItems(*(s32 *)(shape + 0x14), *(s32 *)(shape + 0x18));
    dds3SetObjectFlags(*(s32 *)(unit + 0x31C), 1);
    btlBossDebugPrintf("btl:unit transparency create[%p]\n", unit);
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUpdateUnitTransparency);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D6FB0);

extern char D_003A3BA8[];

extern char D_003BB5F8[];

s32 btlFormatUnitBedName(u8 *actor, char *filename) {
    u32 flags;
    btlGetRuntime();
    flags = *(u32 *)(actor + 0x110);
    if (!(flags & 0x200)) {
        return 0;
    }
    if (flags & 0x1000) {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8, 0,
                      *(u16 *)(actor + 0x124));
    } else {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8,
                      btlGetActorBedAssetIdFromIndex(*(u16 *)(actor + 0x172)),
                      *(u16 *)(actor + 0x124));
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3BA8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D7258);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D74B8);

u32 btlApplyIndexedUnitEffectTask(u8 *arguments) {
    s32 index = *(s32 *)(arguments + 4);
    if (index >= 0) {
        btlApplyScaledUnitEffectParameter(*(u8 **)arguments, index, *(s32 *)(arguments + 8),
                        *(f32 *)(arguments + 0xC));
    }
    return 1;
}

u8 *btlAllocateIndexedUnitEffectTask(u8 *owner, s32 index, s32 value, f32 scale) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 9;
    *(void **)(task + 0x4C) = btlApplyIndexedUnitEffectTask;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(f32 *)(arguments + 3) = scale;
    return task;
}

u32 btlApplyScaledUnitModelTask(u32 *arg0) {
    btlApplyUnitModelScaledValue(*arg0);
    return 1;
}

void *btlCreateScaledUnitModelTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApplyScaledUnitModelTask;
    *(u16 *)(task + 0x20) = 10;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)btlGetUnitModelValue1C(arguments[0]) >= arguments[1]) {
        if ((*(u32 *)(arguments[0] + 0xE8) & 1) == 0) {
            btlResetUnitModelProgress((u8 *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

void *btlScheduleThresholdTask(u8 *owner, u32 threshold) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0xB;
    *(void **)(task + 0x4C) = btlPollThresholdTask;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = threshold;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlApproachTargetTask);

extern u32 btlApproachTargetTask(u32 *);

u8 *btlAllocateApproachTargetTask(u8 *owner, s32 index, f32 scale) {
    u8 *task = btlAllocTask(24);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlApproachTargetTask;
    *(u16 *)(task + 0x20) = 0xE;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    *(f32 *)(arguments + 3) = scale;
    arguments[2] = 0;
    arguments[5] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUpdateUnitPositionInterpolationTask);

SoundTask *btlCreateUnitPositionLerpTowardTargetTask(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = (SoundTask *)btlAllocTask(0x30);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xC;
    task->owner = unit->owner;
    *(void **)((u8 *)task + 0x4C) = btlUpdateUnitPositionInterpolationTask;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x60);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlStepUnitRotationNlerp);

SoundTask *btlCreateUnitRotationInterpolationTask(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = (SoundTask *)btlAllocTask(0x30);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xD;
    task->owner = unit->owner;
    *(void **)((u8 *)task + 0x4C) = btlStepUnitRotationNlerp;
    *(u32 *)((u8 *)task + 0x48) = 0;
    args = (u8 *)btlGetTaskArguments((s32)task);
    *(f32 *)(args + 0x20) = scale;
    *(u32 *)(args + 0x2C) = (u32)unit;
    *(u32 *)(args + 0x24) = 0;
    *(u32 *)(args + 0x28) = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x70);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

extern char D_003A3BC8[];

extern char D_003A3BE8[];

extern void func_001D4E98(u8 *, u32, u32);

void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        func_001D4E98(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            btlClearUnitDefeatCandidate(object);
            func_00221EF0(*(u32 *)(object + 0x320), 0, 0);
            *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
        }
        btlBossDebugPrintf(D_003A3BC8, effect, model);
    } else {
        btlRequestModelAssetByMode(object, effect, model);
        *(u32 *)(object + 0x118) |= 1;
        btlBossDebugPrintf(D_003A3BE8, effect, model);
    }
}

extern char D_003A3C08[];

extern s32 btlCheckModelAssetByMode(u8 *, u32, u32);

u32 btlPollModelLoadCompletion(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        if (!btlCheckModelAssetByMode(object, effect, model)) {
            return 0;
        }
        func_001D4E98(object, effect, model);
        btlReleaseModelAssetByMode(object, effect, model);
        btlBossDebugPrintf(D_003A3C08, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        btlClearUnitDefeatCandidate(object);
        func_00221EF0(*(u32 *)(object + 0x320), 0, 0);
        *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
    }
    *(u32 *)(object + 0x118) = (*(u32 *)(object + 0x118) & ~1) | 2;
    return 1;
}

u8 *btlCreateModelLoadPollTask(u8 *owner, u32 index, u32 value, s8 mode) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x18;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlRequestModelOrReuse;
    *(void **)(task + 0x4C) = btlPollModelLoadCompletion;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(s8 *)(arguments + 3) = mode;
    return task;
}

u32 btlReleaseUnitModelTask(u32 *arg0) {
    btlClearUnitDefeatCandidate(*arg0);
    btlReleaseActorModelResources(*arg0);
    return 1;
}

void *btlScheduleRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlReleaseUnitModelTask;
    *(u16 *)(task + 0x20) = 0x19;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3C08);

s32 btlBeginModelChange(u32 *arguments) {
    s32 owner = arguments[0];
    u32 model = arguments[1];
    u32 variant = arguments[2];
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        btlRequestModelAssetByMode(owner, model, variant);
        *(u32 *)(owner + 0x118) = (*(u32 *)(owner + 0x118) | 1) & ~2;
        return btlBossDebugPrintf("btl:model change start[%X,%X]\n", model, variant);
    }
    return status;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D8190);

extern u32 func_001D8190(u32 *);

u8 *btlCreateModelChangeTask(u8 *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    u8 *task = btlAllocTask(0x1C);
    u32 *args;
    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1A;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlBeginModelChange;
    *(void **)(task + 0x4C) = func_001D8190;
    args = (u32 *)btlGetTaskArguments((s32)task);
    args[0] = (u32)unit;
    args[1] = arg1;
    args[2] = arg2;
    args[3] = arg3;
    args[4] = arg4;
    *((u8 *)args + 0x19) = arg5;
    *((u8 *)args + 0x18) = 0;
    args[5] = 0;
    return task;
}

void btlApplyLinkedUnitStatusWhenActorActive(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(*(u32 *)(*(s32 *)(arg0 + 0xc) + 800));
        return;
    }
}

u32 btlApplyUnitFxWhenLoaded(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_00221D00(*(u32 *)(arg0[3] + 800), arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

u8 *btlCreateUnitTask0F(u8 *unit, s32 arg1, s32 arg2, s32 arg3) {
    u8 *task = btlAllocTask(16);
    u32 *args;
    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0xF;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlApplyLinkedUnitStatusWhenActorActive;
    *(void **)(task + 0x4C) = btlApplyUnitFxWhenLoaded;
    args = (u32 *)btlGetTaskArguments((s32)task);
    args[3] = (u32)unit;
    args[0] = arg1;
    args[1] = arg2;
    args[2] = arg3;
    return task;
}

void btlPrepareUnitStatusFxOnStart(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        evtSetUnitStatusFlags(*(u32 *)(*(s32 *)(arg0 + 0x14) + 800));
        return;
    }
}

u32 btlApplyUnitVectorFxWhenLoaded(u8 *arguments) {
    u8 *object = *(u8 **)(arguments + 0x14);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(arguments));
        evtSetUnitNormalizedDirection(*(s32 *)(object + 0x320), *(s32 *)(arguments + 0x10));
    }
    return 1;
}

u8 *btlCreateUnitTask10(u8 *unit, f32 *spawnPosition, s32 value) {
    u8 *task = btlAllocTask(0x18);
    u32 *args;
    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x10;
    *(u64 *)(task + 0x40) = *(u64 *)(unit + 0x108);
    *(void **)(task + 0x48) = btlPrepareUnitStatusFxOnStart;
    *(void **)(task + 0x4C) = btlApplyUnitVectorFxWhenLoaded;
    args = (u32 *)btlGetTaskArguments((s32)task);
    args[5] = (u32)unit;
    args[4] = value;
    PCP_COPY_VECTOR(args, spawnPosition);
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUnitFadeInTask);

extern u32 btlUnitFadeInTask(u32 *);

u8 *btlCreateUnitFadeInTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitFadeInTask;
    *(u16 *)(task + 0x20) = 0x11;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[4] = 0x80808080;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUnitFadeOutTask);

extern u32 btlUnitFadeOutTask(u32 *);

u8 *btlCreateUnitFadeOutTask(u8 *owner, u32 value, u32 variant) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitFadeOutTask;
    *(u16 *)(task + 0x20) = 0x12;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D8E80);

extern u32 func_001D8E80(u32 *);

u8 *func_001D9038(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x13;
    *(void **)(task + 0x4C) = func_001D8E80;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D90C0);

extern u32 func_001D90C0(u32 *);

u8 *func_001D91E0(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x14;
    *(void **)(task + 0x4C) = func_001D90C0;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D9268);

extern u32 func_001D9268(u32 *);

u8 *func_001D9468(u8 *owner, u32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x15;
    *(void **)(task + 0x4C) = func_001D9268;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

extern s32 sndMixerClone(s32);

extern s32 func_00160958(s32, s32, u8 *, s32);

extern void effBattleUpdateSelectedValue(u8 *, s32);

extern void func_00160D88(u8 *);

u32 btlUpdateSelectedUnitEffect(u32 *arguments) {
    u8 *unit = (u8 *)arguments[0];
    u8 *context;
    u8 *work;
    s32 handle;

    if ((*(u32 *)(unit + 0x110) & 2) == 0) {
        return 1;
    }
    context = (u8 *)btlGetRuntime();
    if (arguments[2] == 0) {
        work = *(u8 **)(context + 0x550);
        handle = *(s32 *)(work + 0x14);
        *(u32 *)(unit + 0x110) |= 0x80;
        arguments[1] = sndMixerClone(handle);
        arguments[2] = func_00160958(arguments[1], 2, unit, 0);
        arguments[3] = 0xE;
        *(u16 *)(arguments[2] + 0xC) &= 0xFFF9;
        effBattleUpdateSelectedValue((u8 *)arguments[2], 0xE);
        *(u32 *)(unit + 0x110) &= ~8;
        if (*(u32 *)(unit + 0x110) & 2) {
            u8 *ext = *(u8 **)(unit + 0x320);
            *(u32 *)(*(u8 **)(ext + 0x8C)) |= 1;
        }
    }
    arguments[4] = arguments[4] + 1;
    if ((s32)arguments[4] >= (s32)arguments[3]) {
        *(u32 *)(unit + 0x110) = (*(u32 *)(unit + 0x110) & ~0x80) | 0x40;
        return 1;
    }
    func_00160D88((u8 *)arguments[2]);
    return 0;
}

extern u32 btlUpdateSelectedUnitEffect(u32 *);

void btlFinishSelectedUnitEffect(u32 *arguments) {
    u32 value = arguments[2];
    if (value != 0) {
        effReleaseBattleVoiceOwner(value);
    }
    if (arguments[1] != 0) {
        sndReleaseAllVoices(arguments[1]);
    }
    btlClearUnitDefeatCandidate(arguments[0]);
    *(u32 *)(arguments[0] + 0x110) |= 0x40;
}

u8 *btlCreateSelectedEffectUpdateTask(u8 *owner) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x16;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x4C) = btlUpdateSelectedUnitEffect;
    *(void **)(task + 0x50) = btlFinishSelectedUnitEffect;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = 0;
    arguments[4] = 0;
    arguments[3] = 0;
    return task;
}

u32 btlUpdateCommandSoundTask(void) {
    btlUpdateUnitActors();
    return 1;
}

SoundTask *btlCreateCommandSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlUpdateCommandSoundTask;
    task->taskId = 0x1B;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 btlUpdateCommandSoundTaskSecondary(void) {
    btlRefreshUnitEffects();
    return 1;
}

u8 *btlCreateSecondaryCommandSoundTask(void) {
    u8 *task = btlAllocTask(0);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x1C;
    *(u16 *)(task + 0x24) |= 2;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(void **)(task + 0x4C) = btlUpdateCommandSoundTaskSecondary;
    return task;
}

u32 func_001D97D0(void) {
    return 1;
}

SoundTask *func_001D97D8(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = func_001D97D0;
    task->taskId = 0x20;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUnitBaseLightTask);

extern u32 btlUnitBaseLightTask(u32 *);

void *btlCreateUnitBaseLightTask(u8 *owner) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlUnitBaseLightTask;
    *(u16 *)(task + 0x20) = 0x21;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

extern char D_003A3CA0[];

extern f32 effMiscRandUnitFloat(void *);

u32 btlStiffenDamageShakeStep(u32 *task) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 scale;
    s32 node;

    if (!(*(u32 *)((u8 *)*task + 0x110) & 2)) {
        return 1;
    }
    if (*(s32 *)(task + 2) == 0) {
        node = mdlGetNodeField2C(*(s32 *)(*(s32 *)((u8 *)*task + 0x320) + 0x8C), 0);
        if (node < 0x1D) {
            u8 *resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)((u8 *)*task + 0xC4),
                                                 *(s32 *)((u8 *)*task + 0xC8));
            if (*(s16 *)(resource + node * 20 + 0x30) == 2) {
                func_001D74B8(*(s32 *)task);
                btlBossDebugPrintf(D_003A3CA0);
            }
        }
    }
    if (0.5f < *(f32 *)(task + 1)) {
        scale = *(f32 *)(task + 1) * (effMiscRandUnitFloat(effSharedRandomState) * 0.5f + 0.5f);
        if (*(s32 *)(task + 2) & 1) {
            scale = -scale;
        }
        if (*(u64 *)((u8 *)*task + 0x110) & 0x808000000000) {
            effObjFetchInnerFirstVec(*(u32 *)((u8 *)*task + 0x31C));
            VU_STORE10(pos);
            pos[0] += scale;
        } else {
            func_001D6300((u8 *)*task, pos);
            pos[0] += scale;
            pos[2] += *(f32 *)((u8 *)*task + 0x88);
        }
        effObjSetInnerFirstVec(*(s32 *)((u8 *)*task + 0x31C), pos);
        *(f32 *)(task + 1) *= 0.85f;
    } else {
        if (*(u64 *)((u8 *)*task + 0x110) & 0x808000000000) {
            effObjFetchInnerFirstVec(*(u32 *)((u8 *)*task + 0x31C));
            VU_STORE10(pos);
        } else {
            func_001D6300((u8 *)*task, pos);
            pos[2] += *(f32 *)((u8 *)*task + 0x88);
        }
        effObjSetInnerFirstVec(*(s32 *)((u8 *)*task + 0x31C), pos);
        return 1;
    }
    *(s32 *)(task + 2) += 1;
    return 0;
}

extern u32 btlStiffenDamageShakeStep(u32 *);

u8 *btlCreateStiffenDamageShakeTask(u8 *owner, f32 value) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1D;
    *(void **)(task + 0x4C) = btlStiffenDamageShakeStep;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    *(f32 *)(arguments + 1) = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001D9C28);

extern void func_001D9C28();

u8 *func_001D9E48(u8 *arg0) {
    u8 *task = btlAllocTask(0x10);
    u32 *data;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9C28;
    *(u16 *)(task + 0x20) = 0x1E;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = (u32)arg0;
    data[1] = 0;
    return task;
}

u32 func_001D9EC0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    btlSetUnitRotation(temp_v0, temp_v0 + 0x40);
    return 1;
}

void *btlScheduleActorUpdate(u8 *owner) {
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9EC0;
    *(u16 *)(task + 0x20) = 0x1F;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

u32 btlRefreshUnitFxVectorTask(u32 *arg0) {
    btlRefreshUnitFxVectors(*arg0);
    return 1;
}

void *btlCreateUnitFxVectorRefreshTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = btlRefreshUnitFxVectorTask;
    *(u16 *)(task + 0x20) = 0x22;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
    return task;
}

extern void sdfFreeMemoryFromEitherHeap(s32);

extern s32 func_00288B68(char *);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3CA0);

void btlStartGunFinishLoad(s32 task) {
    char filename[0x70];
    u8 *actor = *(u8 **)task;
    if ((*(u32 *)(actor + 0x110) & 0x400) != 0) {
        return;
    }
    if (*(s32 *)(actor + 0x30C) != 0) {
        sdfFreeMemoryFromEitherHeap(*(s32 *)(actor + 0x30C));
        *(s32 *)(actor + 0x30C) = 0;
    }
    if (btlFormatUnitBedName(actor, filename)) {
        s32 handle = func_00288B68(filename);
        *(s32 *)(task + 4) = handle;
        btlBossDebugPrintf("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    *(u32 *)(actor + 0x118) = (*(u32 *)(actor + 0x118) | 4) & ~8;
}

typedef struct GunLoadUnit {
    u8 pad_00[0x118];
    u32 resourceFlags;
    u8 pad_11C[0x1F0];
    void *gunResource;
} GunLoadUnit;

typedef struct GunLoadArgs {
    GunLoadUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(u32 *arg) {
    GunLoadArgs *args = (GunLoadArgs *)arg;
    GunLoadUnit *unit = args->unit;
    if (args->handle == 0) {
        return 1;
    }
    if (fileIsRequestReadyInCurrentMode(args->handle) == 0) {
        return 0;
    }
    btlBossDebugPrintf("btl:gun & finish load end[%p]\n", args->handle);
    unit->gunResource = sdfResourceRetainAddress(fileGetResourceHandle(args->handle));
    filePollEntryCleanup(args->handle);
    unit->resourceFlags = (unit->resourceFlags & ~4) | 8;
    return 1;
}

extern void btlStartGunFinishLoad(s32);

extern u32 btlPollGunLoad(u32 *);

u8 *btlCreateGunLoadPollTask(u8 *owner) {
    u8 *task = btlAllocTask(8);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x23;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlStartGunFinishLoad;
    *(void **)(task + 0x4C) = btlPollGunLoad;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

u32 btlUpdateUnitEffectsTask(void) {
    btlUpdateUnitEffects();
    return 1;
}

SoundTask *btlCreateUpdateUnitEffectsTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlUpdateUnitEffectsTask;
    task->taskId = 0x24;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

extern void btlCreateUnitTransparency(u8 *);

extern s32 btlCreateActorTransparency(u32 *);

s32 btlCreateActorTransparency(u32 *arguments) {
    u8 *actor = (u8 *)arguments[0];
    if ((*(u32 *)(actor + 0x110) & 2) == 0) {
        return 0;
    }
    btlCreateUnitTransparency(actor);
    *(u32 *)((u8 *)arguments[0] + 0x110) |= 0x20000;
    return 1;
}

void *btlCreateActorTransparencyTask(u32 actor) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x25;
    *(void **)(task + 0x4C) = btlCreateActorTransparency;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = actor;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DA2E0);

extern s32 func_001DA2E0(u32 *);

u8 *btlCreateActorModelBlendTask(u8 *actor, u32 target, u32 index, u32 value, f32 scale) {
    u8 *task = btlAllocTask(0x1C);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001DA2E0;
    *(u16 *)(task + 0x20) = 0x26;
    *(u64 *)(task + 0x40) = *(u64 *)(actor + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)actor;
    arguments[1] = target;
    arguments[2] = index;
    arguments[4] = value;
    *(f32 *)(arguments + 5) = scale;
    arguments[3] = -1;
    arguments[6] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DA468);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DA780);

void btlResetUnitLinks(u8 *actor) {
    *(s32 *)(actor + 0x2F0) = -1;
    *(s32 *)(actor + 0x2F4) = -1;
    *(u32 *)(actor + 0x110) = 0;
    *(u32 *)(actor + 0x114) = 0;
    *(u32 *)(actor + 0x118) = 0;
    *(u16 *)(actor + 0x310) = 0;
    btlClearAllActorEntrySlots((u32)actor);
    *(u32 *)(actor + 0x2FC) = (u32)sndAllocResourceLink(actor);
    *(u32 *)(actor + 0x300) = (u32)sndAllocLink(actor);
}

BtlUnit *btlCreateUnit(void) {
    u32 handle = func_002D03F8(0x348);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlActorWork *work;
    memset(unit, 0, 0x348);
    unit->handle = handle;
    unit->owner = btlAdvanceRuntimeSequenceCounter();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->unk2F0 = -1;
    unit->unk2C4 = 6;
    unit->gunResourceFlags = 0;
    unit->unk314 = 0;
    unit->resourceNode = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults((u8 *)unit);
    btlInitFxLights((u8 *)unit);
    btlResetUnitLinks((u8 *)unit);
    work = (BtlActorWork *)btlGetRuntime();
    unit->previousActor = 0;
    if (work->actorList != 0) {
        work->actorList->previousActor = unit;
        unit->nextActor = work->actorList;
    } else {
        unit->nextActor = 0;
    }
    work->actorList = unit;
    btlBossDebugPrintf("btl:unit create[%p]\n", unit);
    return unit;
}

void btlReleaseUnitResources(BtlUnit *unit) {
    btlBossDebugPrintf("btl:unit data free[%p]\n", unit);
    if (unit->resourceNode != 0) {
        sndFreeResourceNode(unit->resourceNode);
        unit->resourceNode = 0;
    }
    if (unit->resourceLink != 0) {
        sndFreeResourceLink(unit->resourceLink);
        unit->resourceLink = 0;
    }
    if (unit->link != 0) {
        sndFreeLink(unit->link);
        unit->link = 0;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->listNode != 0) {
        sndFreeListNode(unit->listNode);
        unit->listNode = 0;
    }
    btlReleaseActorModelResources((u8 *)unit);
    if (unit->unk330 != 0) {
        sdfQueueNonzeroResourceId(unit->unk330);
        unit->unk330 = 0;
        unit->unk32C = 0;
    }
}

extern char D_003A3D38[]; /* "btl:unit delete[%p]\n" */

void btlDestroyUnit(u8 *actor) {
    u8 *next;
    u8 *previous;

    btlBossDebugPrintf(D_003A3D38, actor);
    btlReleaseUnitResources(actor);
    next = *(u8 **)(actor + 0x344);
    if (next != 0) {
        *(u8 **)(next + 0x340) = *(u8 **)(actor + 0x340);
    }
    previous = *(u8 **)(actor + 0x340);
    if (previous != 0) {
        *(u8 **)(previous + 0x344) = *(u8 **)(actor + 0x344);
    } else {
        *(u8 **)(btlGetRuntime() + 0x228) = *(u8 **)(actor + 0x344);
    }
    func_002D0918(*(s32 *)(actor + 0x33C));
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlActorWork *)btlGetRuntime())->actorList; unit != 0; unit = next) {
        next = unit->nextActor;
        btlDestroyUnit((u8 *)unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        s32 next = *(s32 *)(actor + 0x344);
        if (*(u32 *)(actor + 0x110) & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

s32 btlFindActorForOwner(s64 target) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (*(s64 *)(actor + 0x108) == target) {
            return actor;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 btlIsActiveActor(s32 candidate) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (actor == candidate) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeClear(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) == 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

s32 btlFindUnitByModeFlagged(s32 arg0) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) != 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

void *btlAllocateIndexList(s32 capacity) {
    u8 *list = sdfAllocAndClearQuadwords(capacity * 4 + 12);
    *(s32 *)list = capacity;
    *(u32 **)(list + 8) = (u32 *)(list + 12);
    *(s32 *)(list + 4) = 0;
    return list;
}

void btlFreeIndexList(u32 ptr) {
    sdfReleaseChipBlock(ptr);
}

void btlAppendIndexListEntry(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 4);
    *(s32 *)(arg0 + 4) = temp_v0 + 1;
    *(u32 *)(temp_v0 * 4 + *(s32 *)(arg0 + 8)) = arg1;
}

void btlClearIndexList(s32 arg0) {
    *(u32 *)(arg0 + 4) = 0;
}

u32 btlGetIndexListCount(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 btlGetIndexListEntry(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 8));
}

void btlCopyIndexList(s32 destination, s32 source) {
    u32 count;
    u32 index;

    btlClearIndexList(destination);
    count = btlGetIndexListCount(source);
    for (index = 0; index < count; index++) {
        btlAppendIndexListEntry(destination, btlGetIndexListEntry(source, index));
    }
}

void btlSwapIndexListEntries(s32 arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    if (arg1 == arg2) {
        return;
    }
    temp_v0 = *(s32 **)(arg0 + 8);
    temp_v1 = temp_v0[arg1];
    temp_v2 = temp_v0[arg2];
    temp_v0[arg1] = temp_v2;
    temp_v0[arg2] = temp_v1;
}

u32 btlFindListIndex(s32 arg0, s32 arg1) {
    u32 count = btlGetIndexListCount(arg0);
    u32 index;

    for (index = 0; index < count; index++) {
        if (arg1 == btlGetIndexListEntry(arg0, index)) {
            return index;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlApplyUnitEffectScale);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DB048);

void btlNormalizeActionCameraKeyScales(s32 action) {
    f32 *key = (f32 *)action;
    u32 flags = *(u32 *)(action + 0xF0);
    u32 i;
    if (flags & 2) {
        for (i = 0; i < 4; i++, key += 12) {
            f32 *pos = key + 12;
            VU0_LOAD_VF(vf10, key + 16);
            VU0_NEGATE_XYZ(vf10);
            VU0_LOAD_VF(vf11, pos);
            VU0_SCALAR_OP(key[20] - 1.0f, "vmulx.xyzw vf10, vf10, vf2x");
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, pos);
            key[20] = 1.0f;
        }
    } else if (flags & 4) {
        f32 *src = key + 12;
        f32 *dst = key + 24;
        for (i = 1; i < 4; i++, dst += 12) {
            f32 delta = dst[8] - src[8];
            dst[0] -= dst[4] * delta;
            dst[1] -= dst[5] * delta;
            dst[2] -= dst[6] * delta;
            dst[8] = src[8];
        }
    }
}

void btlInterpolateVectorStep(f32 *src) {
    f32 vec[4];
    f32 step = -src[8];
    vec[3] = 0.0f;
    vec[0] = src[4] * step + src[0];
    vec[1] = src[5] * step + src[1];
    vec[2] = src[6] * step + src[2];
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(vec) : "memory");
}

u32 func_001DB358(void) {
    return 1;
}

u32 func_001DB360(void) {
    return 1;
}

u32 func_001DB368(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DB370);

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_001F7D80(u8 *, f32);

extern void func_001DB370(u8 *, u8 *, u8 *, f32);

s32 btlStepPoseBlendHalf(u8 *object) {
    f32 blend;
    if (*(u32 *)(object + 0x110) == 0) {
        *(f32 *)(object + 0x128) = 0.0f;
        btlScalarRangeInitQuadratic(object + 0x13C, (f32)(*(s32 *)(object + 0x12C) * 2));
        btlCopyMotionTransform(object, object + 0x30);
        return 0;
    }
    blend = func_001F7D80(object + 0x13C, 1.0f);
    if (blend > 0.5f) {
        blend = 0.5f;
    }
    func_001DB370(object, object + 0x30, object + 0xC0, 2.0f * blend);
    *(f32 *)(object + 0x128) = blend;
    if (blend >= 0.5f) {
        return 1;
    }
    return 0;
}

extern void btlScalarRangeSetStartClearEnd(u8 *, f32);

extern f32 func_001F7CD8(u8 *);

extern void btlCopyMotionTransform(u8 *, u8 *);

extern void func_001DB370(u8 *, u8 *, u8 *, f32);

s32 btlStepPoseBlend(u8 *actor) {
    u8 *motion = actor + 0x134;
    u8 *position = actor + 0x30;
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        btlScalarRangeSetStartClearEnd(motion, *(f32 *)(actor + 0x130));
        btlCopyMotionTransform(actor, position);
    }
    value = func_001F7CD8(motion);
    func_001DB370(actor, position, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

extern void btlScalarRangeInitQuadratic(u8 *, f32);

extern f32 func_001F7D80(u8 *, f32);

s32 btlStepPoseBlendFrame(u8 *actor) {
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        *(u32 *)(actor + 0x128) = 0;
        btlScalarRangeInitQuadratic(actor + 0x13C, (f32)*(s32 *)(actor + 0x12C));
        btlCopyMotionTransform(actor, actor + 0x30);
        return 0;
    }
    value = func_001F7D80(actor + 0x13C, 1.0f);
    func_001DB370(actor, actor + 0x30, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

s32 btlStepPoseBlendRatio(u8 *actor) {
    f32 ratio = (f32)*(s32 *)(actor + 0x110) / (f32)*(s32 *)(actor + 0x12C);
    if (ratio <= 1.0f) {
        func_001DB370(actor, actor + 0x30, actor + 0xC0, ratio);
        return 0;
    }
    btlCopyMotionTransform(actor, actor + 0xC0);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DB698);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DB7D0);

u32 btlExecuteCommandSoundTask(u32 *arg0) {
    func_001DB048(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

void *btlCreateCommandSoundTask(s32 owner, s32 variant) {
    SoundTask *task = (SoundTask *)btlAllocTask(20);
    u32 *arguments;
    task->enabled = 1;
    task->taskId = 0x27;
    task->status = 0;
    if (owner != 0 && *(s32 *)(owner + 0x18) != 0) {
        *(u64 *)((u8 *)task + 0x40) = *(u64 *)(*(s32 *)(owner + 0x18) + 0x108);
    }
    task->callback.update = (void (*)(void))btlExecuteCommandSoundTask;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = owner;
    arguments[3] = variant;
    arguments[1] = 0;
    arguments[2] = 0;
    arguments[4] = 0;
    return task;
}

void *btlCreateTargetedCommandSoundTask(s32 owner, s32 variant, u32 target) {
    void *task = btlCreateCommandSoundTask(owner, variant);
    u32 *arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[4] = target;
    return task;
}

void *btlCreateCommandSoundWithArguments(s32 owner, s32 variant, u32 first, u32 second, u32 third) {
    void *task = btlCreateCommandSoundTask(owner, second);
    u32 *arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[4] = third;
    arguments[1] = variant;
    arguments[2] = first;
    return task;
}

extern void btlInitMotionTransformFromComponents(u8 *, f32, f32, f32, f32, f32, f32, f32, f32);

u32 btlInitializeMotionTransformFromTaskArguments(u8 *arguments) {
    u8 *context = (u8 *)btlGetRuntime();
    func_001DB048(1, *(u32 *)arguments, 0, 0, 0);
    btlInitMotionTransformFromComponents(context + 0x70, *(f32 *)(arguments + 4), *(f32 *)(arguments + 8),
                    *(f32 *)(arguments + 0xC), *(f32 *)(arguments + 0x10),
                    *(f32 *)(arguments + 0x14), *(f32 *)(arguments + 0x18),
                    *(f32 *)(arguments + 0x1C), *(f32 *)(arguments + 0x20));
    return 1;
}

u8 *btlCreateFloatTask28(u8 *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    u8 *task = btlAllocTask(0x24);
    f32 *args;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x28;
    task[0x10] = 0;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlInitializeMotionTransformFromTaskArguments;
    *(u32 *)(task + 0x48) = 0;
    args = (f32 *)btlGetTaskArguments((s32)task);
    *(u8 **)args = actor;
    args[1] = a;
    args[2] = b;
    args[3] = c;
    args[4] = d;
    args[5] = e;
    args[6] = f;
    args[7] = g;
    args[8] = h;
    return task;
}

extern void func_001DB048(s32, s32, s32, s32, s32);

extern void btlSetEffectCameraKeys(u8 *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 btlApplyEffectCameraKeyframes(f32 *args) {
    s32 context = btlGetRuntime();
    func_001DB048(1, *(s32 *)args, 0, 0, 0);
    btlSetEffectCameraKeys((u8 *)(context + 0x70), args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

u8 *btlCreateFloatTask29(u8 *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    u8 *task = btlAllocTask(0x44);
    f32 *args;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x29;
    task[0x10] = 0;
    if (actor != 0 && *(u8 **)(actor + 0x18) != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(actor + 0x18) + 0x108);
    }
    *(void **)(task + 0x4C) = btlApplyEffectCameraKeyframes;
    *(u32 *)(task + 0x48) = 0;
    args = (f32 *)btlGetTaskArguments((s32)task);
    *(u8 **)args = actor;
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;
    args[5] = a5;
    args[6] = a6;
    args[7] = a7;
    args[8] = a8;
    args[9] = a9;
    args[10] = a10;
    args[11] = a11;
    args[12] = a12;
    args[13] = a13;
    args[14] = a14;
    args[15] = a15;
    args[16] = a16;
    return task;
}

u32 btlRunCameraMotionResetTask(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    btlResetCameraMotion(temp_v0 + 0x70);
    return 1;
}

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlRunCameraMotionResetTask;
    task->taskId = 0x2A;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DBE68);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DC0E8);

void btlClearPendingSoundList(void) {
    s32 context = btlGetRuntime();
    s32 actor = *(s32 *)(context + 0x188);
    if (actor != 0) {
        btlFreeIndexList(actor);
        *(s32 *)(context + 0x188) = 0;
    }
    *(u32 *)(context + 0x1F4) &= ~0x10;
}

void btlCopyMotionTransform(u8 *dst, u8 *src) {
    PCP_COPY_VECTOR(dst, src);
    PCP_COPY_VECTOR(dst + 0x10, src + 0x10);
    *(f32 *)(dst + 0x20) = *(f32 *)(src + 0x20);
    *(f32 *)(dst + 0x24) = *(f32 *)(src + 0x24);
}

void func_001DC2A0(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0x24) = arg1;
}

extern void effMiscQuaternionToMatrixVU(void);

extern void btlClearRuntimeFlag2000(void);

extern u8 D_0037E110[];

void btlInitMotionTransformFromVectors(u8 *object, f32 *origin, f32 *direction) {
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(direction));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037E110));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddz.xyzw vf10, vf30, vf10z\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(1.0f));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(origin));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object) : "memory");
    *(f32 *)(object + 0x20) = 1.0f;
    *(f32 *)(object + 0x24) = 0.6981317f;
    btlClearRuntimeFlag2000();
}

extern void btlInitMotionTransformFromVectors(u8 *, f32 *, f32 *);

void btlInitMotionTransformFromComponents(u8 *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
                    f32 vz, f32 vw, f32 scale) {
    f32 origin[4];
    f32 direction[4];
    origin[0] = x;
    origin[1] = y;
    origin[2] = z;
    direction[0] = vx;
    direction[1] = vy;
    direction[2] = vz;
    direction[3] = vw;
    origin[3] = 0.0f;
    btlInitMotionTransformFromVectors(object, origin, direction);
    *(f32 *)(object + 0x24) = scale * 0.017453293f;
}

void btlSetEffectCameraKeys(u8 *fx, f32 x0, f32 y0, f32 z0, f32 vx0, f32 vy0, f32 vz0, f32 vw0,
                            f32 x1, f32 y1, f32 z1, f32 vx1, f32 vy1, f32 vz1, f32 vw1,
                            f32 scale, f32 f154) {
    btlInitMotionTransformFromComponents(fx + 0x30, x0, y0, z0, vx0, vy0, vz0, vw0, scale);
    btlInitMotionTransformFromComponents(fx + 0xC0, x1, y1, z1, vx1, vy1, vz1, vw1, scale);
    *(f32 *)(fx + 0x130) = f154;
    *(u32 *)(fx + 0xF0) |= 0x41;
}

u32 btlGetActiveUnitId(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 btlGetPoseBlendProgress(s32 arg0) {
    return *(f32 *)(arg0 + 0x128);
}

s32 btlIsUnitInActiveList(s32 unit) {
    u8 *work = (u8 *)btlGetRuntime();
    u8 *slot = *(u8 **)(work + 0x164);
    u32 count;
    u32 i;
    if (slot != 0 && *(s32 *)(slot + 0x18) == unit) {
        return 1;
    }
    count = btlGetIndexListCount(*(s32 *)(work + 0x188));
    for (i = 0; i < count; i++) {
        if (btlGetIndexListEntry(*(s32 *)(work + 0x188), i) == unit) {
            return 1;
        }
    }
    return 0;
}

void btlResetActiveUnitList(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x164) = 0;
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x400;
    btlClearIndexList(*(u32 *)(temp_v0 + 0x188));
}

void btlClearRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void btlSetRuntimeFlag2000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 btlIsRuntimeFlag2000Clear(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
}

typedef struct WorldMotionData {
    u8 pad0[0x20];
    s32 unk20;
} WorldMotionData;

typedef struct WorldObjectSub {
    u8 pad0[0x34];
    s32 handle;
} WorldObjectSub;

typedef struct WorldObjectHead {
    u8 pad0[8];
    WorldObjectSub *sub;
} WorldObjectHead;

typedef struct WorldObj {
    u8 pad0[0x18];
    WorldObjectHead *head;
} WorldObj;

extern void *dds3GetWorldObject(void);

extern WorldMotionData *dds3GetWorldCameraObject(void *);

extern void dds3SetWorldCameraObject(void *, s32);

extern f32 dds3GetCameraValue(s32);

extern void func_00106488(f32);

void btlRefreshWorldCameraHandle(void) {
    WorldObj *object;
    s32 handle;
    if (((BattleController *)btlGetRuntime())->flags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = (s32)dds3GetWorldCameraObject(object);
            if (handle != 0) {
                if (((WorldMotionData *)handle)->unk20 != 0) {
                    handle = ((WorldMotionData *)handle)->unk20;
                } else {
                    handle = object->head->sub->handle;
                }
                dds3SetWorldCameraObject(object, handle);
                func_00106488(dds3GetCameraValue(handle));
            }
        }
    }
}

extern s32 D_003BB668;
extern s32 D_003BB664;

s32 btlGetWorldObjectDefault(void) {
    WorldMotionData *data;
    s32 *value;
    if (!(((BattleController *)btlGetRuntime())->flags & 2)) {
        return D_003BB668;
    }
    data = dds3GetWorldCameraObject(dds3GetWorldObject());
    if (data == 0) {
        return D_003BB668;
    }
    value = (s32 *)((u8 *)data + 8);
    if (*value == 0) {
        return D_003BB664;
    }
    return *value;
}

s32 btlIsWorldMotionIdle(void) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        s32 state = dds3GetWorldCameraObject(dds3GetWorldObject());
        if (state == 0) {
            return 0;
        }
        return *(s32 *)(state + 8) == 0;
    }
}

s32 btlGetCameraVectorWork(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return temp_v0 + 0x70;
}

void btlFlagAllUnitDefeatCandidatesTask(void) {
    btlFlagAllUnitsDefeatCandidate();
}

void btlClearAllUnitDefeatCandidatesTask(void) {
    btlClearAllUnitDefeatCandidates();
}

void btlFlagLinkedGroupDefeatCandidatesTask(s32 arg0) {
    btlFlagMatchingUnitsDefeatCandidate(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0xf4) + 0x18) + 0x110) & 0x600);
}

void btlApplyCombinedActorFlags(u8 *resource) {
    u32 flags = 0;
    u32 count = btlGetIndexListCount(*(s32 *)(resource + 0x118));
    u32 index;
    for (index = 0; index < count; index++) {
        u8 *actor = (u8 *)btlGetIndexListEntry(*(s32 *)(resource + 0x118), index);
        flags |= *(u32 *)(actor + 0x110) & 0x600;
    }
    if (flags != 0) {
        btlFlagMatchingUnitsDefeatCandidate(flags);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3D90);

INCLUDE_ASM(const s32, "game/code_001A1960", btlResetCameraMotion);

INCLUDE_ASM(const s32, "game/code_001A1960", btlDebugPrintWorldTransform);

INCLUDE_ASM(const s32, "game/code_001A1960", btlFaceActionParticipantsTowardLinkedTarget);

INCLUDE_ASM(const s32, "game/code_001A1960", btlAimLinkedUnitAtMuzzle);

INCLUDE_ASM(const s32, "game/code_001A1960", btlMatchFirstLinkedActorFlags);

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = *(u32 *)(actor + 0x104);
    s32 linked;
    s32 category;
    u32 count;
    u8 *entry;
    u32 i;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = *(s32 *)(actor + 0xF4);
    if (linked == 0) {
        return 1;
    }
    count = btlGetIndexListCount(*(s32 *)(linked + 0x60));
    entry = *(u8 **)(linked + 0x80);
    for (i = 0; i < count; i++, entry += 0xA1C) {
        if (entry[0x14] != 0) {
            return 0;
        }
    }
    if (*(u16 *)(*(s32 *)(linked + 0x18) + 0x12E) & 0x480) {
        return 0;
    }
    category = *(s32 *)(actor + 0x114);
    if (category != 0 && (*(u16 *)(datActionAnimationRecords + category * 32 + 0x1C) & 1)) {
        return 0;
    }
    return 1;
}

u32 btlHasMarkedEntry10(u8 *object) {
    u8 *resource = *(u8 **)(object + 0xF4);
    u32 count;
    u32 index;
    u8 *entry;
    if (resource == 0) {
        return 0;
    }
    count = btlGetIndexListCount(*(s32 *)(resource + 0x60));
    entry = *(u8 **)(resource + 0x80);
    for (index = 0; index < count; index++, entry += 0xA1C) {
        if (entry[0x10] != 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 btlGetRuntime(void);

extern f32 btlUnitGetTopY(s32);

s32 btlCheckActorDistanceLimit(void) {
    s32 actor = *(s32 *)(btlGetRuntime() + 0x228);

    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 1;
}

extern f32 func_001F66D8(s32, s32, s32);

s32 btlIsEntryHeightWithinLimit(void) {
    if (func_001F66D8(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

s32 btlHasIdleLinkedSlotKindTwo(u8 *actor) {
    u8 *linked = *(u8 **)(actor + 0xF4);
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(*(s32 *)(linked + 0x60));
    entry = *(u8 **)(linked + 0x80);
    for (; i < count; i++, entry += 0xA1C) {
        if (entry[0x10] == 0 && *(s32 *)(entry + 8) == 1 && *(s32 *)(entry + 0xC) == 2 &&
            !(*(u32 *)(btlGetIndexListEntry(*(s32 *)(linked + 0x60), i) + 0x110) & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasEligibleLinkedEntryTypeTwo(u8 *actor) {
    u8 *linked = *(u8 **)(actor + 0xF4);
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = btlGetIndexListCount(*(s32 *)(linked + 0x60));
    entry = *(u8 **)(linked + 0x80);
    for (; i < count; i++, entry += 0xA1C) {
        if (*(s32 *)(entry + 8) == 2 &&
            !(*(u32 *)(btlGetIndexListEntry(*(s32 *)(linked + 0x60), i) + 0x110) & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 btlHasLinkedEffectNodeTrigger(u8 *fx) {
    u8 *task;
    u8 *owner;
    s32 index;
    u8 *table;
    if (*(s32 *)(fx + 0xF4) == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource() == 0) {
        return 0;
    }
    task = *(u8 **)(fx + 0xF4);
    owner = *(u8 **)(task + 0x18);
    index = *(s32 *)(task + 0x44);
    table = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(owner + 0xC4), *(s32 *)(owner + 0xC8));
    return *(s16 *)(table + index * 0x14 + 0x2C) == 2;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 0x100) == 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorCategoryTypeTwo(s32 arg0) {
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x114);
    if (temp_v1 == 0) {
        return 0;
    }
    return ((*(s32 *)(datCommandRecords + temp_v1 * 56 + 0x30) ^ 2) < 1U);
}

u32 btlCanUseActorCategoryFlag2(s32 actor) {
    s32 index;
    if (btlIsActorCategoryMarked(actor) != 0) {
        return 1;
    }
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 2) != 0) {
        return 1;
    }
    return 0;
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index != 0 && *(u8 *)(datCommandRecords + index * 56 + 8) != 0) {
        return 0;
    }
    return btlGetIndexListCount(*(s32 *)(actor + 0x118)) == 1;
}

u32 func_001DD2C0(s32 actor) {
    s32 index;
    if (btlCanUseLinkedActor(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 4) != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    return *(s32 *)(datCommandRecords + index * 56 + 0x30) == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(datActionAnimationRecords + index * 0x20 + 0x1C) & 0x40) == 0) {
        return 0;
    }
    return 1;
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    s32 entry;

    switch (*(u32 *)(actor + 0x104)) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = *(s32 *)(actor + 0xF4);
    if (linked == 0) {
        return 0;
    }
    if (btlGetIndexListCount(*(s32 *)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = btlGetIndexListEntry(*(s32 *)(linked + 0x60), 0);
    return ((*(u32 *)(*(s32 *)(linked + 0x18) + 0x110) ^ *(u32 *)(entry + 0x110)) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(u8 *node) {
    u8 *resource = *(u8 **)(node + 0xF4);
    u8 *actor;
    u32 id;
    if (resource == 0) return 0;
    if (btlGetIndexListCount(*(s32 *)(resource + 0x60)) >= 2) return 0;
    actor = (u8 *)btlGetIndexListEntry(*(s32 *)(resource + 0x60), 0);
    if ((*(u32 *)(actor + 0x110) & 0x400) == 0) return 0;
    id = *(u32 *)(actor + 0xC8);
    if (id >= 0x180) return 0;
    if (*(u32 *)(datEnemyRecords + id * 76) & 0x1000) return 1;
    return 0;
}

u8 func_001DD488(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x5f;
}

u8 func_001DD498(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DD4B8);

void func_001DD678(void) {
}

void func_001DD680(u32 arg0) {
    func_001DF358(arg0, arg0);
}

void func_001DD698(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DD6A0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DD7E8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DD890);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DDE28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DDF20);

void btlAdvanceTargetCursorUnlessHookHandles(u8 *actor) {
    s32 (*callback)(u8 *) = *(s32 (**)(u8 *))(btlGetRuntime() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (*(u16 *)(actor + 0x10C) == 12) {
        btlAdvanceTargetCursorAnimation(actor, actor);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlUpdateActionPoseForLinkedTarget);

void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DE5F8);

void func_001DE958(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DE960);

void func_001DEA68(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlStartLinkedActionPoseBlendIfEligible);

void btlAdvanceUnblockedPlayerCursorAnimation(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    btlAdvancePlayerCursorAnimation(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DEBE0);

void func_001DEDB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E6260(arg0, temp_v0);
}

void func_001DEDE8(u32 arg0) {
    func_001E6368(arg0, arg0);
}

void btlAdvanceCommandCursorTask(u32 arg0) {
    btlAdvanceCommandCursor(arg0, arg0);
}

void func_001DEE18(u32 arg0) {
    func_001E4AC0(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001DEE38(void) {
    func_001E4E50();
}

void btlStartLinkedDefeatCandidateAction(u8 *actor) {
    u8 *resource = *(u8 **)(actor + 0xF4);
    btlAppendIndexListEntry(*(s32 *)(actor + 0x118), *(s32 *)(resource + 0x18));
    func_001E5198(actor, actor + 0x30, actor + 0xC0);
    btlClearAllUnitDefeatCandidates();
    resource = *(u8 **)(actor + 0xF4);
    btlFlagUnitDefeatCandidate(*(s32 *)(resource + 0x18));
    *(f32 *)(actor + 0x130) = 50.0f;
    *(u32 *)(actor + 0xF0) |= 0x41;
}

void func_001DEEC0(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0xf4) + 0x18) + 0x110) & 0x200) != 0) {
        func_001E4960(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x108) != 0x10) {
        btlResetUnitEffectVector(arg0, arg0);
        return;
    }
}

void func_001DEF20(void) {
}

void btlInitializeCursorForLinkedAction(s32 arg0) {
    if (*(s32 *)(arg0 + 0xf4) != 0) {
        btlInitLinkedUnitActionCursor(*(s32 *)(arg0 + 0xf4));
        return;
    }
}

void func_001DEF58(void) {
}

s32 func_001DEF60(s32 actor) {
    s32 context = btlGetRuntime();
    s32 (*callback)(s32) = *(void **)(context + 0x618);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

s32 func_001DEFA0(s32 actor) {
    s32 context = btlGetRuntime();
    s32 (*callback)(s32) = *(void **)(context + 0x620);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DEFE0);

void func_001DF358(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 27.5f);
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlPrepareUnitPoseWithTiltRotation);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DF410);

INCLUDE_ASM(const s32, "game/code_001A1960", btlPrepareRandomizedActionCameraPose);

extern void func_001DB698(u8 *);

void btlAimEffectPoseAtUnit(u8 *actor) {
    u8 *object = *(u8 **)(*(u8 **)(actor + 0xF4) + 0x18);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        if (btlSetActorEffectParameter(object, 1) == 0) {
            btlUnitGetMuzzlePosVU(object);
        }
        __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(actor + 0xC0));
        func_001DB698(actor + 0xC0);
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DFAE0);

INCLUDE_ASM(const s32, "game/code_001A1960", btlRefreshActionPoseBlendSnapshot);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001DFE28);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E0100);

INCLUDE_ASM(const s32, "game/code_001A1960", btlSetupCameraPoseAimUnit);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E0398);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E0718);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E0B68);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E0DA0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E1288);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E16C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E1CF8);

INCLUDE_ASM(const s32, "game/code_001A1960", btlActionAimUserAtTargets);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E20C0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E2578);

void btlFlagUserAndTargetDefeat(u8 *command, u8 *unused) {
    BtlUnit *user;
    BtlUnit *target;
    f32 userPos[4];
    f32 targetPos[4];
    user = *(BtlUnit **)(*(s32 *)(command + 0xF4) + 0x18);
    target = (BtlUnit *)btlGetIndexListEntry(*(u32 *)(command + 0x118), 0);
    if (!(user->flags & target->flags & 0x600)) {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate((u8 *)user);
        btlFlagMatchingUnitsDefeatCandidate(target->flags & 0x600);
    } else {
        btlClearAllUnitDefeatCandidates();
        btlFlagUnitDefeatCandidate((u8 *)user);
        btlFlagUnitDefeatCandidate((u8 *)target);
    }
    btlUnitGetMuzzlePosVU(user);
    VU_STORE10(userPos);
    btlUnitGetMuzzlePosVU(target);
    VU_STORE10(targetPos);
    btlUnitFaceTarget((u8 *)target, (u8 *)user);
    if (userPos[0] < targetPos[0]) {
        *(u32 *)(command + 0xF0) |= 0x200;
    } else {
        *(u32 *)(command + 0xF0) &= ~0x200;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E2970);

INCLUDE_ASM(const s32, "game/code_001A1960", btlSetupActionCameraPair);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E2D20);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E3310);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E37B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E3920);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E3E58);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4180);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4578);

void func_001E4708(void) {
    btlPrepareUnitPoseWithTiltRotation();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4720);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4960);

void btlResetUnitEffectVector(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E5198);

void func_001E5460(u32 arg0) {
    btlFlagUserAndTargetDefeat(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E5478);

void func_001E5700(u32 arg0) {
    func_001E5460(arg0);
}

void func_001E5718(void) {
    func_001E5478();
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlChooseActionPoseBlendFromActorCount);

void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3E40);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3F00);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A3FC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A43F0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4400);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4408);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4468);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E5800);

INCLUDE_ASM(const s32, "game/code_001A1960", btlAdvancePlayerCursorAnimation);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6180);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6260);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6368);

typedef struct SoundCursor {
    u16 unk_00;
    u16 frame;
    s16 mode;
    s16 index;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
} SoundCursor;

#define CURSOR ((SoundCursor *)D_0035F100)

void btlAdvanceCommandCursor(s32 arg0, s32 arg1) {
    if (CURSOR->mode == 0) {
        func_001E9DE0(arg0, arg1, D_0035D9F0[CURSOR->index]);
    } else {
        func_001E9DE0(arg0, arg1, D_0035DA08[CURSOR->index]);
    }
    func_001EB368(arg0, arg1);
    CURSOR->frame++;
}

s32 btlFindLinkedActorById(s32 owner, s32 id) {
    s32 first = *(s32 *)(*(s32 *)(owner + 0xF4) + 0x18);
    s32 second;
    s32 third;

    if (*(u8 *)(first + 0x11C) == id) {
        return first;
    }
    second = *(s32 *)(owner + 0xF8);
    if (*(u8 *)(second + 0x11C) == id) {
        return second;
    }
    third = *(s32 *)(owner + 0xFC);
    if (third != 0 && *(u8 *)(third + 0x11C) == id) {
        return third;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6668);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6AC8);

void btlUnitGetPosVU(u32 unit, u8 mode) {
    u8 pos[16];
    switch (mode) {
    case 1:
        btlSetActorEffectParameterOrMuzzlePosition(unit, 1);
        VU_STORE10(pos);
        break;
    case 0:
    default:
        btlUnitGetMuzzlePosVU(unit);
        VU_STORE10(pos);
        break;
    }
    VU_LOAD10(pos);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4668);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A46F8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E6BB0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001E9DE0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EB1B0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EB368);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EBE88);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ECCA8);

s32 btlFindActiveActorById(s32 id) {
    s32 node;

    for (node = *(s32 *)(btlGetRuntime() + 0x228); node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);

        if (flags & 1) {
            if ((flags & 0xC0) == 0) {
                if (flags & 0x200) {
                    if (*(u8 *)(node + 0x11C) == id) {
                        return node;
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001ED5C8);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EDB20);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EE160);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EE658);

u32 btlCountUnitsByFlags(u32 mask) {
    s32 context = btlGetRuntime();
    u8 *actor = *(u8 **)(context + 0x228);
    u32 count = 0;
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if ((flags & 1) != 0 && (flags & mask) != 0 && (flags & 0x20) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EEAE0);

INCLUDE_ASM(const s32, "game/code_001A1960", btlAdvanceCursorForUnmarkedUnit);

extern u8 D_0035F100[];

void btlClearCommandCursorAndRunAction(s32 actor) {
    memset(D_0035F100, 0, 0x130);
    btlFlagUserAndTargetDefeat(actor, actor);
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlAdvanceCommandCursorOrAction);

INCLUDE_ASM(const s32, "game/code_001A1960", btlInitCommandCursorForCategory);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EEED8);

void btlInitCommandCursorForFirstActor(s32 arg0, s32 arg1) {
    BattleController *work = (BattleController *)btlGetRuntime();
    s32 first = btlGetIndexListEntry(*(s32 *)(arg0 + 0x118), 0);
    memset(CURSOR, 0, 0x130);
    func_001D74B8(first);
    if (btlHasFirstLinkedCategoryFlag1000((u8 *)arg0) != 0) {
        func_001E6BB0(arg0, arg1, 5, 1);
    } else {
        func_001E6BB0(arg0, arg1, 5, 0);
    }
    if (work->mode == 0x10E) {
        CURSOR->unk_0C = 3;
    } else {
        CURSOR->unk_0C = 0;
    }
    func_001E6668(arg0, arg1, 0, 0x11);
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlAdvanceTargetCursorAnimation);

extern void func_001D74B8(s32);

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

void btlInitLinkedUnitActionCursor(u8 *arg0) {
    s32 context = btlGetRuntime() + 0x70;
    *(u8 **)(context + 0xF4) = arg0;
    memset(CURSOR, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    func_001D74B8(*(s32 *)(arg0 + 0x18));
    func_001E6BB0(context, context, 6, 0);
    btlClearAllUnitDefeatCandidatesTask();
    btlFlagUnitDefeatCandidate(*(s32 *)(arg0 + 0x18));
    CURSOR->unk_0C = 0;
    func_001E6668(context, context, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EF1F8);

extern void func_001E6BB0(s32, s32, s32, s32);

extern void func_001E6668(s32, s32, s32, s32);

extern s32 func_001EB1B0(s32, s32, s32, s32);

void btlInitCursorAndApplyAction(s32 actor, s32 target) {
    memset(D_0035F100, 0, 0x130);
    func_001E6BB0(actor, target, 2, 6);
    func_001E6668(actor, target, 0, 0);
    func_001EB1B0(actor, target, 0, 1);
    *(u16 *)(D_0035F100 + 0xC) = 2;
}

void btlAdvanceWorldCounterAndSpawnActionObject(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = btlGetRuntime();
    temp_v0 = dds3AdvanceWorldCounter();
    temp_v2 = evtSpawnActionObj9(temp_v0);
    *(u32 *)(temp_v1 + 0x204) = temp_v2;
    D_003BB694 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x57C) != 0) {
        if (*(u32 *)(context + 0x580) != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    s32 context = btlGetRuntime();
    u32 pointer = *(u32 *)(context + 0x580);
    if (pointer != 0) {
        sdfReleaseChipOrRetainedResource(pointer);
        *(u32 *)(context + 0x580) = 0;
    }
    if (*(u32 *)(context + 0x57C) != 0) {
        sdfReleaseChipOrRetainedResource(*(u32 *)(context + 0x57C));
        *(u32 *)(context + 0x57C) = 0;
    }
}

void btlWaitForPendingWorkAndReleaseBuffers(void) {
    s64 temp_v0;
    s32 temp_v1;

    func_0021FE70();
    do {
        temp_v0 = sdfCheckPendingWorkWithInterrupts();
    } while (temp_v0 != 0);
    evtDestroySecondaryWorldNode();
    do {
        temp_v0 = sdfCheckPendingWorkWithInterrupts();
    } while (temp_v0 != 0);
    btlReleaseWorkBuffers();
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 500) = *(u32 *)(temp_v1 + 500) & 0xfffffffd;
}

extern char D_003A4AD8[];

extern char D_003A4AF0[]; /* "btl:free field F2\n" */

extern char D_003A4B08[]; /* "btl:free field F1\n" */

extern void sdfQueueNonzeroResourceId(s32);

typedef struct BattleFieldBlocks {
    u8 unk_00[0x290];
    s32 fieldF1;
    s32 fieldF2;
    s32 fieldF3;
} BattleFieldBlocks;

void btlFreeFieldBlocks(void) {
    BattleFieldBlocks *context = (BattleFieldBlocks *)btlGetRuntime();
    btlWaitForPendingWorkAndReleaseBuffers();
    if (context->fieldF3 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF3);
        context->fieldF3 = 0;
        btlBossDebugPrintf(D_003A4AD8);
    }
    if (context->fieldF2 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF2);
        context->fieldF2 = 0;
        btlBossDebugPrintf(D_003A4AF0);
    }
    if (context->fieldF1 != 0) {
        sdfQueueNonzeroResourceId(context->fieldF1);
        context->fieldF1 = 0;
        btlBossDebugPrintf(D_003A4B08);
    }
    context = (BattleFieldBlocks *)btlGetRuntime();
    *(u32 *)((u8 *)context + 0x1F4) &= ~2;
}

extern f32 *D_00324770[];

extern u8 kwlnDefaultColorVector[];

extern void fldApplyLightSetCurrent(void);

void btlInitializeSceneLightingAndTint(void) {
    u8 *context = (u8 *)btlGetRuntime();
    fldApplyLightSetCurrent();
    btlInitTintTransitionResource(0x80, 0);
    VU_LOAD10((u8 *)D_00324770[0] + 0x10);
    VU_STORE10(context + 0x10);
    VU_STORE10(context + 0x40);
    VU_LOAD10(D_00324770[0]);
    VU_STORE10(context + 0x20);
    VU_STORE10(context + 0x50);
    VU_LOAD10(kwlnDefaultColorVector);
    VU_STORE10(context + 0x30);
    VU_STORE10(context + 0x60);
    *(u32 *)(context + 0x698) = 0x807E5C5E;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EF6B8);

s32 btlGetActionDefaultOrOverride(s32 index) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x1F4) & 0x30000000) {
        return *(s32 *)(context + 0x6A0);
    }
    return *(s32 *)(datActionAnimationRecords + index * 32 + 0x18);
}

u32 btlCameraVectorHasNaN(void) {
    u8 *context = (u8 *)btlGetRuntime();
    if (*(f32 *)(context + 0x50) != *(f32 *)(context + 0x50) ||
        *(f32 *)(context + 0x54) != *(f32 *)(context + 0x54) ||
        *(f32 *)(context + 0x58) != *(f32 *)(context + 0x58) ||
        *(f32 *)(context + 0x60) != *(f32 *)(context + 0x60) ||
        *(f32 *)(context + 0x64) != *(f32 *)(context + 0x64) ||
        *(f32 *)(context + 0x68) != *(f32 *)(context + 0x68)) {
        return 1;
    }
    return 0;
}

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_0035F5C0;

void btlInitTintTransitionResource(u32 resource, u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_0035F5C0.resource = resource;
    D_0035F5C0.handle = handle;
}

void btlInitTintTransitionDefault(u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = fldGetSkyDrawState();
    D_0035F5C0.handle = handle;
    D_0035F5C0.resource = 0x80;
}

void btlStepTintTransition(void) {
    SoundCommand *cmd = &D_0035F5C0;
    u32 value;
    u32 start;
    if (cmd->currentId != 0) {
        cmd->currentId += 0xFFFF;
        start = cmd->resource;
        value = (f32)(s32)(cmd->handle - start) * ((f32)cmd->currentId / (f32)cmd->nextId);
        fldSetSkyDrawState(value + D_0035F5C0.resource);
    } else {
        fldSetSkyDrawState(cmd->resource);
    }
}

typedef struct SoundTransition {
    u32 currentResource;
    u8 unk_04[0x14];
    u32 previousResource;
    u32 queuedResource;
    u16 soundId;
    u16 queuedId;
} SoundTransition;

void btlQueueTintTransition(u32 resource, u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = resource;
        transition->queuedResource = resource;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = resource;
}

void btlQueueTintTransitionToZero(u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = 0;
        transition->queuedResource = 0;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = 0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
}

extern s32 btlBlendColor(s32, s32, f32);

void btlStepBlendColor(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;

    if (transition->soundId != 0) {
        transition->currentResource = btlBlendColor(transition->queuedResource, transition->previousResource, (f32)transition->soundId / (f32)transition->queuedId);
        transition->soundId--;
    } else {
        transition->currentResource = transition->queuedResource;
    }
    btlStepTintTransition();
}

void btlDrawTintIfVisible(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;

    if ((transition->currentResource & 0xFF000000) != 0) {
        func_00187C08(transition);
    }
}

void sndResetTransition(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;
    D_003BB694 = 0;
    btlTintTransitionHoldCount = 0;
    transition->soundId = 0;
    transition->currentResource = 0;
}

void btlClearTintAndEnableCamera(void) {
    btlQueueTintTransitionToZero(0);
    func_00113E40(1);
}

extern f32 *D_00324770[];

void btlUpdateTintAndWorldLight(void) {
    u8 *context = (u8 *)btlGetRuntime();
    f32 *position = D_00324770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        func_00113E40(0);
    } else {
        func_00113E40(1);
    }
    if (*(u32 *)(context + 0x1F8) & 0x20) {
        func_00113E40(0);
    }
    btlStepBlendColor();
}

void btlTickFieldSwayAndTint(void) {
    s32 temp_v0 = btlGetRuntime();

    if ((((*(u32 *)(temp_v0 + 500) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x1f8) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x1fc) & 0x4000000) == 0)) {
        func_00132BD0();
        fldUpdateSwayOffset();
        func_00132010();
    }
    btlDrawTintIfVisible();
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EFD58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4AD8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4AF0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4B08);

INCLUDE_ASM(const s32, "game/code_001A1960", btlCreateRainEffect);

void btlDispatchLinkedEffectWhenBattleGatesClear(void) {
    if (mdlFlagTest(0x32)) {
        return;
    }
    {
        s32 context = btlGetRuntime();
        if (*(u32 *)(context + 0x1F8) & 0x20) {
            return;
        }
        if (*(u32 *)(context + 0x58C) != 0) {
            effUpdateAndDrawSelectionEntries(*(u32 *)(context + 0x58C));
        }
    }
}

extern char D_003A4B40[]; /* "btl:rain exit\n" */

void btlStopRainSoundTransition(void) {
    s32 context = btlGetRuntime();
    if (*(u32 *)(context + 0x58C) != 0) {
        btlBossDebugPrintf(D_003A4B40);
        effReleaseSelectionFlagList(*(u32 *)(context + 0x58C));
        *(u32 *)(context + 0x58C) = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4B40);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001EFFA8);

extern u32 func_001EFFA8(u32 *);

s32 fldCreateSceneTileTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(40);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = func_001EFFA8;
    task[0x10] = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    memset(arguments, 0, 40);
    arguments[0] = soundId;
    arguments[1] = variant;
    arguments[9] = 0;
    return (s32)task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlPollFloorLoadTask);

extern u32 btlPollFloorLoadTask(u32 *);

u8 *btlCreateFloorLoadTask(u32 soundId, u32 variant) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 2;
    *(void **)(task + 0x4C) = btlPollFloorLoadTask;
    task[0x10] = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[2] = soundId;
    arguments[3] = variant;
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F0430);

extern u32 func_001F0430(u32 *);

void *btlCreateEffectTaskWithSourceParams(u8 *source, u32 value) {
    u8 *task = btlAllocTask(0x34);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 3;
    *(u16 *)(task + 0x24) |= 2;
    *(void **)(task + 0x4C) = func_001F0430;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u8 *)btlGetTaskArguments((s32)task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, (u8 *)btlGetRuntime() + 0x10, 0x30);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F06E0);

void *func_001F0920(owner)
    u32 owner;
{
    u8 *task = btlAllocTask(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 4;
    *(u16 *)(task + 0x24) |= 2;
    *(void **)(task + 0x4C) = func_001F06E0;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = owner;
    return task;
}

s64 func_001F0998(void) {
    D_003BB694 = 1;
    return func_001F06E0();
}

SoundTask *btlCreateSoundUpdateTask(void) {
    SoundTask *task = (SoundTask *)func_001F0920();
    task->taskId = 7;
    task->callback.update = func_001F0998;
    return task;
}

s32 btlQueueTintTransitionWhenEnabled(u32 *arguments) {
    s32 context = btlGetRuntime();
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) == 0) {
        btlQueueTintTransition(arguments[0], *(u16 *)(arguments + 1));
    }
    btlTintTransitionHoldCount++;
    return 1;
}

SoundTask *sndCreateAcquireTask(u32 soundId, u32 flags) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u32 *data;
    task->enabled = 1;
    task->taskId = 5;
    task->callback.acquireSound = btlQueueTintTransitionWhenEnabled;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = soundId;
    data[1] = flags;
    return task;
}

s32 sndTickFadeCounter(soundId)
    u16 *soundId;
{
    s32 context = btlGetRuntime();

    if (btlTintTransitionHoldCount == 0) {
        return 1;
    }
    btlTintTransitionHoldCount--;
    if (btlTintTransitionHoldCount != 0) {
        return 1;
    }
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) != 0) {
        return 1;
    }
    btlQueueTintTransitionToZero(*soundId);
    return 1;
}

SoundTask *sndCreateReleaseTask(sound)
    u32 *sound;

{
    SoundTask *task = (SoundTask *)btlAllocTask(4);
    task->enabled = 1;
    task->taskId = 6;
    task->callback.releaseSound = sndTickFadeCounter;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)sound;
    return task;
}

s64 func_001F0B90(void) {
    btlTintTransitionHoldCount = 1;
    return sndTickFadeCounter();
}

SoundTask *btlCreateSoundReleaseTask(void) {
    SoundTask *task = (SoundTask *)sndCreateReleaseTask();
    task->taskId = 8;
    task->callback.update = func_001F0B90;
    return task;
}

void btlExtendTaskFrameLimit(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

u32 sndGetResourceStatus(u32 *sound) {
    u32 flags;
    if (!sound[1]) {
        return 0;
    }
    flags = sound[0];
    if (flags & 1) {
        return 0xFFFFFFF;
    }
    if (flags & 2) {
        return sound[3];
    }
    return 0;
}

s32 sndLookupResourceType(s32 sound, s32 index) {
    s32 (*lookup)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x644);
    if (lookup != 0) {
        s32 value = lookup(sound, index);
        if (value != -1) {
            return value;
        }
    }
    return *(u8 *)(datActionAnimationRecords + index * 0x20 + 3);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F0CA0);

void sndCreateSystemEffect(u32 *effect) {
    s32 handle;
    if (!(effect[0] & 8) || effect[4] || effect[1]) {
        return;
    }
    handle = sndMixerClone(effect[5]);
    effect[4] = handle;
    btlBossDebugPrintf("btl:system effect create[%p]\n", handle);
}

void sndDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        btlBossDebugPrintf("btl:system effect delete[%p]\n", effect[4]);
        sndReleaseAllVoices(effect[4]);
        effect[4] = 0;
    }
}

void sndAddEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    task[2] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    ++effect[1];
    ++source[0x314 / 4];
    ++target[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F1110);

extern u32 func_001F1110(u32 *);

void sndReleaseEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    if (task[2]) {
        effReleaseBattleVoiceOwner(task[2]);
    }
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    --effect[1];
    --source[0x314 / 4];
    --target[0x314 / 4];
    sndDeleteSystemEffect(effect);
}

s32 func_001F12E8(u32 effect, u32 soundId, u8 *owner, u16 variant) {
    u8 *task = btlAllocTask(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2B;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndAddEffectReferences;
    *(void **)(task + 0x4C) = func_001F1110;
    *(void **)(task + 0x50) = sndReleaseEffectReferences;
    arguments = btlGetTaskArguments((s32)task);
    *(u32 *)arguments = effect;
    *(u32 *)(arguments + 0xC) = soundId;
    *(u32 *)(arguments + 0x10) = soundId;
    *(u32 *)(arguments + 0x14) = soundId;
    *(u8 **)(arguments + 0x18) = owner;
    *(u16 *)(arguments + 4) = variant;
    *(u32 *)(arguments + 8) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return (s32)task;
}

SoundTask *sndCreateEffectWithTargets(s32 sound, s32 flags, u32 *source, u32 *target, s32 mode, u16 variant) {
    SoundTask *task = (SoundTask *)func_001F12E8(sound, flags, mode, variant);
    u32 *data = (u32 *)btlGetTaskArguments((s32)task);
    data[4] = (u32)source;
    data[5] = (u32)target;
    return task;
}

void sndStartEffectTask(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F1470);

extern u32 func_001F1470(u32 *);

void sndFinishActorEffectTask(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        effReleaseBattleVoiceOwner(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    sndDeleteSystemEffect(temp_v0);
}

u8 *sndCreateActorEffectTask(u32 effect, u8 *owner, u32 channel) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2C;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndStartEffectTask;
    *(void **)(task + 0x4C) = func_001F1470;
    *(void **)(task + 0x50) = sndFinishActorEffectTask;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = effect;
    arguments[2] = (u32)owner;
    arguments[3] = channel;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

void sndIncrementEffectActiveCount(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

extern s32 sndGetEffectNodeParameter(s32, u16);

u32 sndWaitEffectFramesAndApplyUnitParameter(u32 *args) {
    u32 *effect = (u32 *)args[0];
    s32 frames;

    if ((effect[0] & 2) == 0) {
        return 0;
    }
    frames = sndGetEffectNodeParameter((s32)effect, (u16)args[1]);
    if ((s32)args[5] >= frames) {
        *(s32 *)(args[0] + 8) -= 1;
        if ((s32)args[3] >= 0) {
            btlApplyScaledUnitEffectParameter((u8 *)args[2], args[3], args[4], 1.0f);
        }
        return 1;
    }
    args[5]++;
    return 0;
}

void *sndCreateTimedUnitEffectTask(u32 effect, u8 *actor, u16 frames, u32 channel, u32 volume) {
    u8 *task = btlAllocTask(24);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2E;
    *(u64 *)(task + 0x40) = *(u64 *)(actor + 0x108);
    *(void **)(task + 0x48) = sndIncrementEffectActiveCount;
    *(void **)(task + 0x4C) = sndWaitEffectFramesAndApplyUnitParameter;
    *(u32 *)(task + 0x50) = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = effect;
    arguments[2] = (u32)actor;
    *(u16 *)(arguments + 1) = frames;
    arguments[3] = channel;
    arguments[4] = volume;
    arguments[5] = 0;
    return task;
}

void sndBeginEffectLoad(EffectLoadArgs *args) {
    SoundEffectNode *effect = args->effect;
    if (effect->flags & 2) {
        if (effect->handle != 0) {
            sndReleaseAllVoices(effect->handle);
            effect->handle = 0;
        }
        effect->flags &= ~2;
    }
    args->loadHandle = (void *)func_00288B48(args->name);
    effect->flags |= 1;
    btlBossDebugPrintf("btl:effect load start[%s]\n", args->name);
}

extern char D_003A4C88[];

u32 sndPollEffectLoad(u32 *args) {
    u8 *effect = (u8 *)args[0];
    s32 resource;

    if (*(u32 *)effect & 2) {
        return 1;
    }
    if (!fileIsRequestReadyInCurrentMode(args[1])) {
        return 0;
    }
    btlBossDebugPrintf(D_003A4C88, args[2]);
    resource = fileGetResourceHandle(args[1]);
    *(u32 *)(effect + 0x10) =
        sndMixerClone(sdfResourceRetainAddress(resource));
    func_002D0918(resource);
    filePollEntryCleanup(args[1]);
    *(u32 *)effect = (*(u32 *)effect & ~1) | 2;
    return 0;
}

extern void sndBeginEffectLoad(EffectLoadArgs *);

extern u32 sndPollEffectLoad(u32 *);

u8 *sndCreateEffectLoadTask(u32 soundId, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 12);
    u8 *arguments;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x2F;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = sndBeginEffectLoad;
    *(void **)(task + 0x4C) = sndPollEffectLoad;
    task[0x10] = 0;
    arguments = btlGetTaskArguments((s32)task);
    name = (char *)(arguments + 12);
    *(u32 *)arguments = soundId;
    *(char **)(arguments + 8) = name;
    strcpy(name, filename);
    return task;
}

extern u32 btlWaitUnitListIdle(void);

u32 btlWaitUnitListIdle(void) {
    s32 context = btlGetRuntime();
    s32 unit;

    if (*(u32 *)(context + 0x1F4) & 0x40000000) {
        return 1;
    }
    for (unit = *(s32 *)(context + 0x228); unit != 0; unit = *(s32 *)(unit + 0x344)) {
    }
    return 1;
}

void *btlCreateWaitUnitListIdleTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x30;
    *(void **)(task + 0x4C) = btlWaitUnitListIdle;
    task[0x10] = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
    return task;
}

u32 sndApplyToActiveActors(u32 *soundId) {
    u8 *object = *(u8 **)(btlGetRuntime() + 0x228);
    while (object != 0) {
        u32 flags = *(u32 *)(object + 0x110);
        if (flags & 1) {
            if (flags & 2) {
                if (*(u32 *)(object + 0x320) != 0 &&
                    (flags & 0xE0) == 0) {
                    btlBlendUnitColor(object, *(u32 *)(object + 0x54), *soundId);
                }
            }
        }
        object = *(u8 **)(object + 0x344);
    }
    return 1;
}

void *btlCreateApplyToActiveActorsTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x31;
    *(void **)(task + 0x4C) = sndApplyToActiveActors;
    task[0x10] = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
    return task;
}

u32 func_001F1C18(void) {
    func_00105888();
    return 1;
}

SoundTask *btlCreateFadeStateResetTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = func_001F1C18;
    task->taskId = 0x32;
    task->status = 0;
    return task;
}

void sndAddSourceReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    sndCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F1CC8);

extern u32 func_001F1CC8(u32 *);

void sndFinishEffectSourceTask(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        effReleaseBattleVoiceOwner(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    sndDeleteSystemEffect(temp_v0);
}

u8 *sndCreateEffectSourceTask(u32 effect, u8 *owner, u64 resource) {
    u8 *task = btlAllocTask(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2D;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = sndAddSourceReferences;
    *(void **)(task + 0x4C) = func_001F1CC8;
    *(void **)(task + 0x50) = sndFinishEffectSourceTask;
    arguments = btlGetTaskArguments((s32)task);
    *(u32 *)arguments = effect;
    *(u8 **)(arguments + 8) = owner;
    *(u64 *)(arguments + 0x10) = resource;
    *(u32 *)(arguments + 4) = 0;
    *(u32 *)(arguments + 0x18) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return task;
}

u32 btlTaskStartFadeIn(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

void *btlCreateFadeInTask(u32 sound) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x35;
    *(void **)(task + 0x4C) = btlTaskStartFadeIn;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = sound;
    return task;
}

u32 btlTaskStartCustomFadeIn(u8 *arg0) {
    kwlnFadeInStart(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

SoundTask *sndCreateCustomTask(u32 soundId, u32 options) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u32 *data;
    task->enabled = 1;
    task->taskId = 0x36;
    task->callback.playCustomSound = btlTaskStartCustomFadeIn;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)btlGetTaskArguments((s32)task);
    data[0] = soundId;
    data[1] = options;
    return task;
}

u32 btlTaskSetBattleFlag40000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0x40000;
    mdlClearListedObjectFlag();
    return 1;
}

SoundTask *sndCreateSetBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlTaskSetBattleFlag40000;
    task->taskId = 0x37;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 btlTaskClearBattleFlag40000(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffbffff;
    mdlSetListedObjectFlag();
    return 1;
}

SoundTask *sndCreateClearBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlTaskClearBattleFlag40000;
    task->taskId = 0x38;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4C88);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F2218);

void sndSetEffectNodeParameter(s32 arg0, u16 arg1) {
    sndReadSelectedMixerBankValue(*(u32 *)(arg0 + 0x10), arg1);
}

s32 sndGetEffectNodeParameter(s32 arg0, u16 arg1) {
    return func_00160858(*(u32 *)(arg0 + 0x10), arg1);
}

s32 sndIsResourceNodeReferencedOrActive(s32 arg0) {
    if (*(s32 *)(arg0 + 4) != 0) {
        return 1;
    }
    return *(u32 *)(arg0 + 8) != 0;
}

s32 sndHasActiveActor(void) {
    s32 actor = *(s32 *)(btlGetRuntime() + 0x228);
    while (actor != 0) {
        s32 sound = *(s32 *)(actor + 0x2F8);
        if (sound != 0 && sndIsResourceNodeReferencedOrActive(sound) != 0) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceNode));
    u8 *state;
    SoundResourceNode *first;
    node->unk_04 = 0;
    node->unk_08 = 0;
    node->fadeCountdown = 0;
    node->resourceHandle = 0;
    state = (u8 *)btlGetRuntime();
    node->previous = 0;
    first = *(SoundResourceNode **)(state + 0x234);
    if (first) {
        first->previous = node;
        node->next = *(SoundResourceNode **)(state + 0x234);
    } else {
        node->next = 0;
    }
    *(SoundResourceNode **)(state + 0x234) = node;
    return node;
}

SoundResourceNode *sndCreateResourceNode(u32 soundId) {
    SoundResourceNode *node = (SoundResourceNode *)sndAllocResourceNode();
    node->resourceHandle = sndMixerClone(soundId);
    node->flags |= 2;
    return node;
}

void sndFreeResourceNode(SoundResourceNode *node) {
    if (node->resourceHandle) {
        sndReleaseAllVoices(node->resourceHandle);
    }
    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node->previous) {
        node->previous->next = node->next;
    } else {
        *(SoundResourceNode **)(btlGetRuntime() + 0x234) = node->next;
    }
    sdfReleaseChipBlock(node);
}

void btlUpdateFadeColor(void) {
    s32 context = btlGetRuntime();
    SoundResourceNode *node;

    for (node = *(SoundResourceNode **)(context + 0x234); node != 0; node = node->next) {
        if (node->unk_04 == 0) {
            node->fadeCountdown = 0;
        } else if (node->fadeCountdown > 0) {
            node->fadeCountdown = node->fadeCountdown - 1;
        }
    }
    if ((u32)(btlGetActiveUnitId() - 9) < 2 || *(s32 *)(context + 0x2A4) != 0 || *(s32 *)(context + 0x208) == 8) {
        *(u8 *)(context + 0x584) = 0;
    } else {
        *(u8 *)(context + 0x584) = 1;
    }
    switch (*(u8 *)(context + 0x584)) {
    case 0: {
        u32 packedColor = *(u32 *)(context + 0x588);

        /* The high byte rises by 0x10 per frame, capped at the opaque gray tint. */
        if (packedColor <= 0x8080807F) {
            *(u32 *)(context + 0x588) = packedColor + 0x10000000;
        } else {
            *(u32 *)(context + 0x588) = 0x80808080;
        }
        break;
    }
    case 1: {
        u32 packedColor = *(u32 *)(context + 0x588);

        if (packedColor > 0x808080) {
            *(u32 *)(context + 0x588) = packedColor - 0x10000000;
        } else {
            *(u32 *)(context + 0x588) = 0x808080;
        }
        break;
    }
    }
    effTickSlotVolumeFade();
}

void btlSweepFloorModelLists(void) {
    effSweepFloorModelList();
}

void btlResetFieldColorAndSweepFlags(void) {
    effBTLFieldColorResetFlags();
    fileResetRenderFlags();
    kwlnDrawControlFlags = kwlnDrawControlFlags & 0xdfffffff;
}

void btlClearSoundAndModelResources(void) {
    sndClearResourceNodes();
    mdlMarkAndProcessObjectNodes();
    effBTLFieldColorResetFlags();
    fileResetRenderFlags();
}

void sndClearResourceNodes(void) {
    SoundResourceNode *node = *(SoundResourceNode **)(btlGetRuntime() + 0x234);
    while (node) {
        SoundResourceNode *next = node->next;
        sndFreeResourceNode(node);
        node = next;
    }
}

s32 btlButtonMaskToIndex(u32 mask) {
    switch (mask & 0x7FFF) {
    case 0:
        return 0;
    case 0x0001:
        return 0xE;
    case 0x0002:
        return 0xB;
    case 0x0004:
        return 0xA;
    case 0x0008:
        return 8;
    case 0x0010:
        return 6;
    case 0x0020:
        return 7;
    case 0x0040:
        return 9;
    case 0x0080:
        return 5;
    case 0x0100:
        return 0xD;
    case 0x0200:
        return 4;
    case 0x0400:
        return 3;
    case 0x0800:
        return 0xC;
    case 0x1000:
        return 2;
    case 0x2000:
        return 1;
    case 0x4000:
        return 0x10;
    default:
        return 0;
    }
}

SoundResourceLink *sndAllocResourceLink(void *owner) {
    SoundResourceLink *node = sdfAllocAndClearQuadwords(sizeof(SoundResourceLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void sndFreeResourceLink(SoundResourceLink *node) {
    if (node->sound) {
        effReleaseBattleVoiceOwner(node->sound);
        --*(s32 *)((u8 *)node->task + 4);
        sndDeleteSystemEffect(node->task);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F2818);

void btlMarkTaskReady(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

SoundLink *sndAllocLink(void *owner) {
    SoundLink *node = sdfAllocAndClearQuadwords(sizeof(SoundLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void sndFreeLink(SoundLink *node) {
    if (node->sound) {
        effReleaseBattleVoiceOwner(node->sound);
        --*(s32 *)((u8 *)node->task + 4);
        sndDeleteSystemEffect(node->task);
    }
    sdfReleaseChipBlock(node);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F2C00);

u32 btlDisableBattleFade(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u8 *)(temp_v0 + 0x584) = 0;
    return 1;
}

SoundTask *sndCreateClearStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlDisableBattleFade;
    task->taskId = 0x33;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 btlEnableBattleFade(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(u8 *)(temp_v0 + 0x584) = 1;
    return 1;
}

SoundTask *sndCreateSetStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlEnableBattleFade;
    task->taskId = 0x34;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

extern s32 func_00288A80(const char *path);

extern void func_00288C50(s32 archive);

extern void func_00288788(s32 archive);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A4FF0);

INCLUDE_ASM(const s32, "game/code_001A1960", sndLoadSysEffLb);

void btlRefreshSoundEntries(void) {
    u32 i;

    btlGetRuntime();
    for (i = 0; i < 0x31; i++) {
        if (D_0035F748[i].resource != 0) {
            btlCreateIndexedSoundResourceNode(i, D_0035F748[i].resource);
        }
    }
}

void sndFreeBattleSoundEntries(void) {
    s32 context = btlGetRuntime();
    SoundResourceNode **node = (SoundResourceNode **)(context + 0x4B8);
    u32 i;

    for (i = 0; i < 0x31; i++, node++) {
        if (D_0035F748[i].resource != 0) {
            sndFreeResourceNode(*node);
            *node = 0;
        }
    }
}

void btlCreateIndexedSoundResourceNode(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = btlGetRuntime();
    puVar3 = (u32 *)sndAllocResourceNode();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4b8) = puVar3;
    *puVar3 = temp_v0 | 10;
}

typedef struct SoundHandleNode {
    u32 handle;
    void *actor;
} SoundHandleNode;

extern u32 func_002940D0(s32);

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = sdfAllocAndClearQuadwords(8);
    SoundBankEntry *entry = &D_0035F748[index];
    node->actor = actor;
    node->handle = func_002940D0(entry->resource);
    return node;
}

void btlUpdateJobPositionFromModel(s32 *args) {
    f32 pos[4];

    if (sdfLoadMapRecordPositionVector(*(s32 *)(args[1] + 0x18), 1) == 0) {
        mdlLoadPrimaryVectorVU(args[1]);
        VU_STORE10(pos);
        pos[1] -= 150.0f;
    } else {
        VU_STORE10(pos);
    }
    fileQueueSetPosition(args[0], pos);
    fileQueueUpdate(args[0]);
}

void sndDestroyFileQueueWrapper(u32 arg0) {
    fileQueueDestroy(*(u32 *)arg0);
    sdfReleaseChipBlock(arg0);
}

void func_001F3230(void) {
}

void sndSetStationedSeVolume(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x58, 0x3f);
}

void sndSetStationedSeHighVolume(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

extern void func_0026A5F0(s32);

extern void mnuResetTitleStreamLocked(void);

extern s32 D_0035F998[];

extern u32 D_003BB6A8;

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F3278);

u8 sndIsStreamStatusTwoOrThree(void) {
    s32 temp_v0;

    temp_v0 = mnuPollTitleStreamStateLocked();
    return temp_v0 - 2U < 2;
}

void btlResetTitleStreamOnBattleFlag(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    if ((*(u32 *)(temp_v0 + 500) & 0x10000) != 0) {
        mnuRunTitleStreamTransitionAndLogBgm();
        return;
    }
}

void btlAdvanceTitleState(void) {
    mnuAdvanceTitleStateUnderSemaphore();
}

void btlAdvanceTitleStateWithAudioCleanup(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    func_002E8E28();
    func_002E8E00();
}

void btlAdvanceTitleStateTask(void) {
    btlAdvanceTitleState();
}

void btlAdvanceTitleStateWithAudioCleanupTask(void) {
    btlAdvanceTitleStateWithAudioCleanup();
}

s32 sndLoadAndPlayStationedSe(u32 soundId) {
    s32 loaded = sndFindPackedTrackLoadStatus(soundId);
    if (loaded != 0) {
        sndSetStationedSeVolume(soundId);
        return 1;
    }
    return loaded;
}

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (sndLoadAndPlayStationedSe(soundId)) {
        btlBossDebugPrintf("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

SoundTask *sndCreateStationedSeTask(u32 soundId) {
    SoundTask *task = (SoundTask *)btlAllocTask(4);
    task->enabled = 1;
    task->taskId = 0x55;
    task->status = 0;
    task->callback.playSound = sndPlayStationedSe;
    *(u32 *)btlGetTaskArguments((s32)task) = soundId;
    return task;
}

s32 sndHasFlaggedActiveNode(void) {
    ActiveSoundNode *node = *(ActiveSoundNode **)(btlGetRuntime() + 0x238);
    while (node != 0) {
        if (node->flags & 8) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

s32 sndPlaySkillSeTask(u32 *arg0) {
    u8 *task = (u8 *)arg0;
    s32 context = btlGetRuntime();
    u32 value;

    if (*(u16 *)(task + 4) == 2 && *(u32 *)(context + 0x268) < 6) {
        btlBossDebugPrintf("btl:skill SE ignore[frame:%d]\n", *(u32 *)(context + 0x268));
        return 1;
    }
    *(u32 *)(context + 0x268) = 0;
    if (sndFindPackedTrackLoadStatus(*(u32 *)task) != 0) {
        switch (*(u16 *)(task + 4)) {
        case 1:
            value = *(u32 *)task;
            break;
        case 2:
            value = *(u32 *)task | 1;
            break;
        case 3:
            value = *(u32 *)task | 1;
            break;
        default:
            value = 0;
            break;
        }
        sndSetStationedSeVolume(value);
        btlBossDebugPrintf("btl:sound skillSE play[%X-%X]\n", value >> 16, value & 0xFFFF);
        return 1;
    }
    btlBossDebugPrintf("btl:sound load wait[%X]\n", *(u16 *)(task + 2));
    return 0;
}

SoundTask *sndCreateSkillSeTask(s32 skill, u16 variant) {
    SoundTask *task = (SoundTask *)btlAllocTask(8);
    u8 *data;

    task->enabled = 1;
    task->taskId = 0x52;
    task->status = 0;
    task->callback.playSound = sndPlaySkillSeTask;
    data = (u8 *)btlGetTaskArguments((s32)task);
    *(u32 *)data = *(u32 *)(skill + 8);
    *(u16 *)(data + 4) = variant;
    return task;
}

typedef struct SoundLoadNode {
    u32 flags;
    u8 state;
    u8 unk_05[3];
    u32 position;
} SoundLoadNode;

typedef struct SoundFileRequest {
    SoundLoadNode *node;
    void *handle;
    u32 resourceHandle;
    u32 blockIndex;
    const char *name;
} SoundFileRequest;

void sndStartFileLoad(SoundFileRequest *request) {
    SoundLoadNode *node = request->node;

    request->handle = (void *)func_00288B48(request->name);
    node->flags |= 1;
    node->position = (request->blockIndex + 0x200) << 16;
    node->state = 2;
    btlBossDebugPrintf("btl:sound file load start[%s]\n", request->name);
}

extern char D_003A50D8[];

extern char D_003A50F0[];

extern char D_003A5110[];

extern char D_003A5138[];

extern void func_002E9450(s32, s32);

u32 sndPollMotSeFileAndSpu(SoundFileRequest *request) {
    SoundLoadNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        btlBossDebugPrintf(D_003A50D8);
        return 0;
    }
    if ((node->flags & 2) == 0) {
        if (fileIsRequestReadyInCurrentMode(request->handle)) {
            s32 size;
            s32 data;
            btlBossDebugPrintf(D_003A50F0, request->name);
            request->resourceHandle = fileGetResourceHandle(request->handle);
            size = fileGetResourceSize(request->handle);
            data = sdfResourceRetainAddress(request->resourceHandle);
            if (sndFindPackedTrackLoadStatus(node->position) == 0) {
                func_002E9450(data, size);
                node->flags |= 8;
                /* The packed position stores the sound block number in its upper halfword. */
                btlBossDebugPrintf(D_003A5110, *(u16 *)((u8 *)node + 0xA), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (sndFindPackedTrackLoadStatus(node->position) != 0) {
        btlBossDebugPrintf(D_003A5138, *(u16 *)((u8 *)node + 0xA));
        func_002D0918(request->resourceHandle);
        filePollEntryCleanup(request->handle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

u8 *sndCreateFileLoadTask(SoundLoadNode *node, u32 variant, const char *filename) {
    u8 *task = btlAllocTask(strlen(filename) + 20);
    SoundFileRequest *request;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x53;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = sndStartFileLoad;
    *(void **)(task + 0x4C) = sndPollMotSeFileAndSpu;
    task[0x10] = 0;
    request = (SoundFileRequest *)btlGetTaskArguments((s32)task);
    name = (char *)(request + 1);
    request->node = node;
    request->blockIndex = variant;
    request->name = name;
    strcpy(name, filename);
    return task;
}

s32 sndLoadDataFile(s32 *data) {
    char filename[0x70];
    if (sndIsCommandBusySigned()) {
        return 1;
    }
    sndFormatResourceNameFromUnitMode(data[0], (s32)filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

void *sndCreateDataFileLoadTask(u8 *owner) {
    u8 *task = btlAllocTask(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = sndLoadDataFile;
    *(u16 *)(task + 0x20) = 0x56;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)btlGetTaskArguments((s32)task) = (u32)owner;
    return task;
}

s32 sndIsCommandBusySigned(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 sndHasResourceFlagsOneOrEight(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void sndFormatResourceNameFromIndex(s32 arg0, s32 arg1) {
    func_003014F0(arg1, D_003A5158, D_003BB6B0, (arg0 + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A50D8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A50F0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5110);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5138);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5158);

void sndFormatResourceNameFromUnitMode(s32 arg0, s32 arg1) {
    func_003014F0(arg1, "MDD_%03X.ADB", *(u16 *)(arg0 + 0x124));
}

s32 sndMapResourceType(s32 sound, s32 index) {
    s32 type = sndLookupResourceType(sound, index);
    if (type < 0x1A && type != 0) {
        if (type >= 0xB) {
            return type + *(s32 *)(btlGetRuntime() + 0x1E4) - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = sdfAllocAndClearQuadwords(0x14);
    u8 *state = (u8 *)btlGetRuntime();
    ActiveSoundNode *first;

    node->previous = 0;
    first = *(ActiveSoundNode **)(state + 0x238);
    if (first != 0) {
        first->previous = node;
        node->next = *(ActiveSoundNode **)(state + 0x238);
    } else {
        node->next = 0;
    }
    *(ActiveSoundNode **)(state + 0x238) = node;
    return node;
}

void sndFreeListNode(ActiveSoundNode *node) {
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        *(ActiveSoundNode **)(btlGetRuntime() + 0x238) = node->next;
    }
    sdfReleaseChipBlock(node);
}

void sndClearList(void) {
    ActiveSoundNode *node = *(ActiveSoundNode **)(btlGetRuntime() + 0x238);
    while (node != 0) {
        ActiveSoundNode *next = node->next;
        sndFreeListNode(node);
        node = next;
    }
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F3C38);

extern char D_003A5178[];

extern char D_003A5188[];

extern char D_003A5198[];

extern char D_003A51A8[];

extern u32 func_001F3C38(u32 *, u32);

void sndLoadMotSeFiles(u32 *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = func_001F3C38(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_003014F0(filename, D_003A5158, D_003BB6B0, id >> 16);
            } else if (sound[1] == 0) {
                func_003014F0(filename, D_003A5178, D_003A5188, sound[2]);
            } else {
                func_003014F0(filename, D_003A5198, D_003A5188, sound[2]);
            }
            *(u32 *)(handleTable + offset) = func_00288B48(filename);
            btlBossDebugPrintf(D_003A51A8, slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

s32 sndFindListNodeForChannel(s32 soundId, s32 channel) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x23C);
    while (node != 0) {
        if (*(s32 *)(node + 4) == soundId && *(s32 *)(node + 8) == channel) {
            return node;
        }
        node = *(s32 *)(node + 0x104);
    }
    return 0;
}

extern char D_003A51D0[];

u8 *sndAcquireSlotOwner(s32 soundId, s32 channel) {
    u8 *node = (u8 *)sndFindListNodeForChannel(soundId, channel);
    u8 *context;
    u8 *head;

    if (node != 0) {
        btlBossDebugPrintf(D_003A51D0, node);
        (*(u32 *)(node + 0xC))++;
        return node;
    }
    node = sdfAllocAndClearQuadwords(0x108);
    *(s32 *)(node + 4) = soundId;
    *(s32 *)(node + 8) = channel;
    *(u32 *)(node + 0xC) = 1;
    context = (u8 *)btlGetRuntime();
    *(u8 **)(node + 0x100) = 0;
    head = *(u8 **)(context + 0x23C);
    if (head != 0) {
        *(u8 **)(head + 0x100) = node;
        *(u8 **)(node + 0x104) = *(u8 **)(context + 0x23C);
    } else {
        *(u8 **)(node + 0x104) = 0;
    }
    *(u8 **)(context + 0x23C) = node;
    if (mdlFlagTest(0xC0F) == 0) {
        sndLoadMotSeFiles(node);
    }
    return node;
}

void sndReleaseSlotOwner(u8 *node) {
    u32 count = *(u32 *)(node + 0xC) - 1;
    *(u32 *)(node + 0xC) = count;
    if (count == 0) {
        u32 i = 0;
        u32 *resources = (u32 *)(node + 0x8C);
        u32 *handles = (u32 *)(node + 0x18);
        for (; i < 0x1D; i++, handles++, resources++) {
            if (*handles != 0) {
                filePollEntryCleanup(*handles);
            }
            if (*resources != 0) {
                func_002D0918(*resources);
            }
        }
        if (*(u8 **)(node + 0x104) != 0) {
            *(u8 **)(*(u8 **)(node + 0x104) + 0x100) =
                *(u8 **)(node + 0x100);
        }
        if (*(u8 **)(node + 0x100) != 0) {
            *(u8 **)(*(u8 **)(node + 0x100) + 0x104) =
                *(u8 **)(node + 0x104);
        } else {
            *(u8 **)(btlGetRuntime() + 0x23C) =
                *(u8 **)(node + 0x104);
        }
        sdfReleaseChipBlock(node);
    }
}

void sndReleaseAllSlotOwners(void) {
    s32 node = *(s32 *)(btlGetRuntime() + 0x23C);

    while (node != 0) {
        s32 next = *(s32 *)(node + 0x104);

        sndReleaseSlotOwner(node);
        node = next;
    }
}

void btlStartMoveOtherUnitsTask(void) {
    s32 task = btlCreateMoveOtherUnitsTask();
    btlStartTask(task);
}

s32 sndHasActiveFileLoad(void) {
    u8 *node = *(u8 **)(btlGetRuntime() + 0x23C);
    while (node) {
        if (*(u32 *)node & 8) {
            return 1;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", btlQueueUnitSoundSlotFileLoad);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A51D0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F41C0);

extern void btlQueueUnitSoundSlotFileLoad(s32);

extern u32 func_001F41C0(u32 *);

s32 btlCreateMoveOtherUnitsTask(u8 *owner, u32 soundId) {
    u8 *task = btlAllocTask(16);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x54;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = btlQueueUnitSoundSlotFileLoad;
    *(void **)(task + 0x4C) = func_001F41C0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = soundId;
    arguments[1] = 0;
    arguments[3] = 0;
    return (s32)task;
}

void func_001F4430(void) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    *(s32 *)(temp_v0 + 0x264) = -1;
    *(s32 *)(temp_v0 + 0x268) = -1;
}

void sndLoadBattleBank(void) {
    if (sndFindPackedTrackLoadStatus(0x10000) == 0) {
        sndEnsureMidiBankResident(0x10000);
        btlBossDebugPrintf("btl:sound load BSE SMG\n");
    }
}

u8 sndIsBattleBankLoaded(void) {
    s64 temp_v0;

    temp_v0 = sndFindPackedTrackLoadStatus(0x10000);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F44C0);

s32 sndHasOccupiedNodeSlots(void) {
    s32 context = btlGetRuntime();
    s32 node = *(s32 *)(context + 0x23C);
    while (node != 0) {
        if ((*(u32 *)node & 2) == 0) {
            u32 index = 0;
            u32 *slot = (u32 *)(node + 0x18);
            for (; index < 0x1D; index++) {
                if (*slot != 0) {
                    return 1;
                }
                slot++;
            }
        }
        node = *(s32 *)(node + 0x104);
    }
    return 0;
}

extern s32 mnuGetSoundBufferStateLocked(void);

extern void mnuPrintTitleDebugBanner(void);

u32 sndFinishEarringPlayback(void) {
    s32 status = mnuGetSoundBufferStateLocked();
    if (status == 0) {
        return 1;
    }
    if (status == 2) {
        func_003003F0("%%%%%%%%%%%%%%%% EARRING(2)\n");
        mnuPrintTitleDebugBanner();
    }
    return 0;
}

SoundTask *sndCreateEarringTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = sndFinishEarringPlayback;
    task->taskId = 0x57;
    task->status = 0;
    return task;
}

typedef struct BattleVoiceLoad {
    u32 request;
    s32 state;
    s32 index;
} BattleVoiceLoad;

extern u8 D_00377650[];

extern u8 D_00377654[];

INCLUDE_ASM(const s32, "game/code_001A1960", sndPollAtrac3SELoadTask);

extern u32 sndPollAtrac3SELoadTask(void *);

void *sndCreateAtracEffectLoadTask(u32 owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x58;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x4C) = sndPollAtrac3SELoadTask;
    task[0x10] = 0;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[2] = owner;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F49D0);

extern char D_003A5390[];

extern char D_003A53B0[];

extern char D_003A53D0[];

u32 sndUpdateEarringDeadPlayback(u32 *args) {
    u32 resource;
    u32 data;
    u32 size;

    if (args[1] == 0) {
        return 1;
    }
    if (args[2] == 0) {
        if (fileIsRequestReadyInCurrentMode(args[1]) != 0) {
            resource = fileGetResourceHandle(args[1]);
            args[2] = resource;
            data = sdfResourceRetainAddress(resource);
            size = fileGetResourceSize(args[1]);
            filePollEntryCleanup(args[1]);
            func_0026ABA8(data, size, 2);
            mnuPrintTitleDebugBanner();
            func_003003F0(D_003A5390);
            btlBossDebugPrintf(D_003A53B0);
        }
        return 0;
    }
    if (mnuGetSoundBufferStateLocked() == 0) {
        btlBossDebugPrintf(D_003A53D0);
        return 1;
    }
    return 0;
}

void sndFinishEarringPlaybackTask(u32 *sound) {
    u8 *state = (u8 *)btlGetRuntime();
    if (sound[2]) {
        func_002D0918(sound[2]);
    }
    --*(u16 *)(state + 0x260);
}

extern u32 func_001F49D0(u32 *);

void *sndCreateEarringPlaybackTask(u8 *owner) {
    u8 *task = btlAllocTask(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x59;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = func_001F49D0;
    *(void **)(task + 0x4C) = sndUpdateEarringDeadPlayback;
    *(void **)(task + 0x50) = sndFinishEarringPlaybackTask;
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    arguments[2] = 0;
    return task;
}

u32 btlPlayStationedSe1C(void) {
    sndLoadAndPlayStationedSe(0x1c);
    return 1;
}

SoundTask *btlCreateStationedSe1CTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlPlayStationedSe1C;
    task->taskId = 0x5A;
    task->status = 0;
    return task;
}

u32 btlAdvanceTitleStateAfterSound(void) {
    btlAdvanceTitleState();
    return 1;
}

SoundTask *btlCreateAdvanceTitleStateTask(void) {
    SoundTask *task = (SoundTask *)btlAllocTask(0);
    task->enabled = 1;
    task->callback.process = btlAdvanceTitleStateAfterSound;
    task->taskId = 0x5B;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F4D50);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F5028);

extern void func_001F4D50(void *);

void btlRepositionPartyAroundBattleCenter(void) {
    f32 vector[4];
    u8 *context = (u8 *)btlGetRuntime();
    PCP_COPY_VECTOR(vector, context);
    func_001F4D50(vector);
}

s64 func_001F53F0(void) {
    return func_001F5028(0x400);
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F5410);

INCLUDE_ASM(const s32, "game/code_001A1960", btlPlaceTripleFormationAroundTarget);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F57D0);

INCLUDE_ASM(const s32, "game/code_001A1960", btlOrientFrontAndBackUnitsTowardTargets);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F5B50);

void func_001F5D00(void) {
}

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F5D08);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A53D0);

INCLUDE_ASM(const s32, "game/code_001A1960", func_001F5ED8);

extern u32 func_001F5ED8(u32 *);

u8 *btlCreateSoundPlaybackTask(u8 *owner, u32 soundId, u32 variant, u32 channel, u32 flags) {
    u8 *task = btlAllocTask(20);
    u32 *arguments;
    u8 *sound;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001F5ED8;
    sound = *(u8 **)(owner + 0x18);
    *(u16 *)(task + 0x20) = 0x5F;
    *(u32 *)(task + 0x48) = 0;
    *(u64 *)(task + 0x40) = *(u64 *)(sound + 0x108);
    arguments = (u32 *)btlGetTaskArguments((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = soundId;
    arguments[2] = variant;
    arguments[3] = channel;
    arguments[4] = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001A1960", D_003A5410);

