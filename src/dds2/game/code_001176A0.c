#include "common.h"
#include "dds3_path.h"
#include "eff_transform.h"
#include "sdf.h"
#include "pcp_vu0.h"
#include "btl_action.h"
#include "dat_state.h"
#include "dat_command.h"

extern void *D_00435E80;


extern u32 scrGetWorkTaskHandle(void);
extern void dds3WorkInit(void *header);
extern s32 sdfDispatchPrimaryUnitScript(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode);
extern char sdfRuntimeTaskName[]; /* "GBWK" */
extern void kwlnTaskDestroyWithHierarchyByName(char *name, s32 flag);
extern void func_00117A80(void);
extern void sdfDecrementAllocationReferenceCount(u32 allocation);
extern s32 sdfReleaseResourceAllocation(u32 allocation);
extern struct EffWorldNode *dds3AppendWorldObjectNode();

extern void *sdfAllocGeneralBlock(s32 size);

extern void *sdfResourceRetainAddress(void *resource);

extern s32 kwlnTaskCreate(char *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);

extern void scrClearProcessGlobals(void);
extern void mdlResetViewerFlagsAndSolarOverlay(void);
extern void ptyClearProfileRecords(void);
extern void ptyRebuildAllProfiles(void);
extern void evtUpdateFlaggedEntries(void);
extern void dds3ForEachEntry(void);
extern void ptyAssignPartyRosterItemsAndMarkOwned(void);
extern void mdlFlagClear(s32 flag);
extern void mdlFlagSet(s32 flag);
extern void sdfSaveResetSnapshot(void);
extern void func_00118008(void);

s32 sdfBumpTickCounters(void);

void evtResetWorldAndProfileRuntime(void);

typedef struct SdfChannel {
    u8 pad00[0xA4];
} SdfChannel;

extern SdfChannel D_00385A90[8];
extern void func_00118798(SdfChannel *channel);


typedef struct EvtScaledValue {
    u32 value;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} EvtScaledValue;


typedef struct SdfPackedValue {
    u8 pad00[6];
    u16 hp; /* 0x06 */
    u8 pad08[6];
    u16 flagsAndValue;
} SdfPackedValue;

#define SDF_PACKED_FLAG 0x8000
#define SDF_PACKED_VALUE_MASK 0x7FFF


/* The 0x20 flag selects base enemy vitals instead of the party script path. */
#define SDF_UNIT_ENEMY 0x20


typedef struct SdfEnemyVitals {
    u32 flags;          /* 0x00 */
    u8 pad04[4];
    u16 maxHp;          /* 0x08 */
    u8 pad0A[2];
    u16 maxMp;          /* 0x0C */
} SdfEnemyVitals;

extern s32 datEnemyRecords;
/* Party-unit header: flags, record index into the enemy table (stride 76), hp / max hp. */
typedef struct SdfPartyUnit {
    u16 flags;          /* 0x00 */
    u8 pad02[2];
    u16 unitId;         /* 0x04 */
    u16 hp;             /* 0x06 */
    u16 maxHp;          /* 0x08 */
    u8 pad0A[0xA];
    u16 level;          /* 0x14 */
    u8 pad16[6];
    u16 hpBonus;        /* 0x1C */
    u16 mpBonus;        /* 0x1E */
} SdfPartyUnit;


struct DatUnitStatus;
extern s32 datGetStatWithStatusOverride(struct DatUnitStatus *, s32 statIndex);

extern u32 sdfRollActionHit(s32 channel, s32 arg1, SdfPackedValue *item);

extern void scrDestroyWorkTask(void);

extern void dds3SamplePathKeyframeInterval(u32 *index, f32 *fraction, Dds3PathKeyframes *table, f32 time);

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 sdfSampleActiveLinearCurve(Dds3PathCurveWork *user) {
    u32 index;
    f32 fraction;
    Dds3PathKeyframes *curve;
    f32 *values;

    if (user->flags & DDS3_PATH_SCALAR_CHANNEL) {
        curve = user->scalarKeys;
        dds3SamplePathKeyframeInterval(&index, &fraction, curve, user->time);
        values = curve->data;
        return values[index] * (1.0f - fraction) + values[index + 1] * fraction;
    }
    return 0.0f;
}

typedef struct SdfCounter {
    s32 direction;   /* 0x00: 0 counts up, 1 counts down */
    u32 flags;       /* 0x04: 8 = frozen, 0x20 = wrap */
    f32 limit;       /* 0x08 */
    f32 value;       /* 0x0C */
} SdfCounter;

/* Step the counter by one; returns 0 when it ran out and does not wrap. */
s32 sdfStepWrappingFloatCounter(SdfCounter *counter) {
    s32 result = 1;

    if (counter->flags & 8) {
        return 1;
    }
    if (counter->direction == 0) {
        if (counter->limit > counter->value) {
            counter->value = counter->value + 1.0f;
        } else if (counter->flags & 0x20) {
            counter->value = 0.0f;
        } else {
            result = 0;
        }
    } else if (counter->direction == 1) {
        if (counter->value > 0.0f) {
            counter->value = counter->value - 1.0f;
        } else if (counter->flags & 0x20) {
            counter->value = counter->limit;
        } else {
            result = 0;
        }
    }
    return result;
}

void sdfSetFloatCounterDirection(u32 *destination, u32 value) {
    *destination = value;
}

u32 sdfGetFloatCounterDirection(u32 *source) {
    return *source;
}

void evtScaleValueByMultiplier(float multiplier, EvtScaledValue *value) {
    value->scaled = multiplier * value->base;
}

float evtGetValueScaleFactor(EvtScaledValue *value) {
    return value->scaled / value->base;
}

void sdfFreezeFloatCounter(EvtScaledValue *value) {
    value->flags = value->flags | 8;
}

void sdfUnfreezeFloatCounter(EvtScaledValue *value) {
    value->flags = value->flags & 0xfffffff7;
}

void sdfEnableFloatCounterWrap(EvtScaledValue *value) {
    value->flags = value->flags | 0x20;
}

void sdfDisableFloatCounterWrap(EvtScaledValue *value) {
    value->flags = value->flags & 0xffffffdf;
}

EffWorldNode *evtSpawnActionObj11(s32 key, void *data, s32 value) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(0x11);

    obj->data = data;
    obj->key = key;
    obj->value = value;
    return obj;
}

void *func_001178B0(EffWorldNode *node) {
    return node->data;
}

void *dds3GetWorldNodeData(EffWorldNode *node) {
    return node->data;
}

/* Load the world node's borrowed data vector into VF10. */
void evtLoadValueVectorIntoVu(EffWorldNode *node) {
    void *vec = dds3GetWorldNodeData(node);

    VU0_LOAD_VF_MEMORY(vf10, vec);
}

/* Same, from the second quadword in the borrowed vector. */
void evtLoadValueSecondaryVectorIntoVu(EffWorldNode *node) {
    void *vec = (void *)((u8 *)dds3GetWorldNodeData(node) + 0x10);

    VU0_LOAD_VF_MEMORY(vf10, vec);
}

/* Allocate the runtime state block, zero it and register the "GBWK" tick task. */
void sdfCreateRuntimeTask(void) {
    void *mem = sdfAllocGeneralBlock(0x1E840);
    DatGameState *state = sdfResourceRetainAddress(mem);

    memset(state, 0, 0x1E840);
    state->header.backingAllocation = (s32)mem;
    state->header.firstTick = 0;
    state->header.secondTick = 0;
    kwlnTaskCreate(sdfRuntimeTaskName, 1, 0, 0, (void *)sdfBumpTickCounters, 0, state);
    datGameState = state;
    evtResetWorldAndProfileRuntime();
}

void sdfDestroyRuntimeTask(void) {
    u32 allocation;

    kwlnTaskDestroyWithHierarchyByName(sdfRuntimeTaskName, 0);
    func_00117A80();
    allocation = datGameState->header.backingAllocation;
    sdfDecrementAllocationReferenceCount(allocation);
    sdfReleaseResourceAllocation(allocation);
    datGameState = 0;
}

s32 sdfBumpTickCounters(void) {
    DatGameState *runtime;

    runtime = datGameState;
    runtime->header.firstTick += 1;
    runtime->header.secondTick += 1;
    return 0;
}

void evtResetWorldAndProfileRuntime(void) {
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    datGameState->world.updateMode = 8;
    ptyInitRuntime();
    func_00122B58(0);
    ptyClearProfileRecords();
    func_0026CE90();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    ptyAssignPartyRosterItemsAndMarkOwned();
    btlClearSharedBattleStateWords();
}

void func_00117A80(void) {
}

extern u8 D_0043E5B0[16];
extern u32 D_00385218[3];
extern s8 D_00435DB5;
extern s8 D_00435DB6;
extern u16 D_00435DB8;
extern s32 D_00435DBC;
extern s32 D_00435DC0;
extern s8 D_00435DC4;
extern s8 D_00435DC5;
extern s32 D_00438E90;
extern void *D_00438E94;
extern void *D_00438E98;
extern void *D_00438E9C;
extern void *D_00438EA0;
extern void *D_00438EA4;
extern s32 D_00438EA8;
extern u32 D_00438EAC;
extern s32 mdlFlagTest(s32 flag);
extern s32 mnuCreateFlagEntries(void);
extern void *sdfMemoryGetBlockAddress(void *handle);
extern void func_0011D130(void);
extern s32 mtrMantraEventBitPush(void);

/* Snapshot the progress that survives a full runtime reset: the three carried flags, header words, battle flags,
 * mantra bitmaps, profile records, party templates, high item counts and blocked-item flags. */
void sdfSaveResetSnapshot(void) {
    void *copy;

    D_00435DB5 = 0;
    D_00435DB6 = 0;
    D_00435DC4 = 0;
    D_00435DC5 = 0;
    memset(D_0043E5B0, 0, sizeof(D_0043E5B0));
    D_00435DB8 = 0;
    D_00435DBC = 0;
    if (mdlFlagTest(0xB8F)) {
        D_00435DB5 = 1;
    }
    if (mdlFlagTest(0xBA0)) {
        D_00435DB6 = 1;
    }
    if (mdlFlagTest(0xC0E)) {
        D_00435DC4 = 1;
    }
    memcpy(D_0043E5B0, datGameState->battleFlags, sizeof(datGameState->battleFlags));
    D_00435DB8 = datGameState->header.unk0C;
    D_00435DBC = datGameState->header.secondTick;
    D_00435DC0 = datGameState->header.unk20;
    D_00385218[0] = datGameState->header.unk24;
    D_00385218[1] = datGameState->header.unk28;
    D_00385218[2] = datGameState->header.unk2C;
    D_00438E90 = mnuCreateFlagEntries();
    D_00438E94 = sdfAllocGeneralBlock(sizeof(datGameState->mantraBits));
    copy = sdfMemoryGetBlockAddress(D_00438E94);
    memcpy(copy, datGameState->mantraBits, sizeof(datGameState->mantraBits));
    D_00438E98 = sdfAllocGeneralBlock(sizeof(datGameState->profileBanks));
    copy = sdfMemoryGetBlockAddress(D_00438E98);
    memcpy(copy, datGameState->profileBanks, sizeof(datGameState->profileBanks));
    func_0011D130();
    D_00438E9C = sdfAllocGeneralBlock(sizeof(datGameState->templates));
    copy = sdfMemoryGetBlockAddress(D_00438E9C);
    memcpy(copy, datGameState->templates, sizeof(datGameState->templates));
    D_00438EA0 = sdfAllocGeneralBlock(0x40);
    copy = sdfMemoryGetBlockAddress(D_00438EA0);
    memcpy(copy, &datGameState->inventory.counts[0xC0], 0x40);
    D_00438EA4 = sdfAllocGeneralBlock(sizeof(datGameState->itemBlockedFlags));
    copy = sdfMemoryGetBlockAddress(D_00438EA4);
    memcpy(copy, datGameState->itemBlockedFlags, sizeof(datGameState->itemBlockedFlags));
    D_00438EA8 = mtrMantraEventBitPush();
    D_00438EAC = datGameState->world.slotFlags;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118008);

/* A full reset preserves the scene allocation and restores the saved runtime data. */
void sdfResetGameRuntime(s32 fullReset) {
    s32 backingAllocation;

    if (fullReset == 1) {
        sdfSaveResetSnapshot();
        backingAllocation = datGameState->header.backingAllocation;
        memset(datGameState, 0, 0x1E840);
        datGameState->header.backingAllocation = backingAllocation;
    }
    datGameState->header.firstTick = 0;
    datGameState->header.secondTick = 0;
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    datGameState->world.updateMode = 8;
    ptyInitRuntime();
    mdlFlagClear(0xC0E);
    mdlFlagSet(0x801);
    func_00122B58(0);
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    ptyAssignPartyRosterItemsAndMarkOwned();
    if (fullReset == 1) {
        func_00118008();
    }
}

void func_00118680(void) {
    dds3WorkInit(D_00435E80);
}

void sdfFirePendingCallback(void) {
    if (D_00435E80 == 0) {
        return;
    }
    scrDestroyWorkTask();
}

u8 scrIsCurrentWorkTask(u32 expected) {
    u32 current;

    current = scrGetWorkTaskHandle();
    return current == expected;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_001186F8);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118798);

void sdfResetChannels(void) {
    u32 i;

    sdfFirePendingCallback();
    for (i = 0; i < 8; i++) {
        func_00118798(&D_00385A90[i]);
    }
    func_00118680();
}

/* Enemy vitals use their base record; party vitals use level growth and bonuses. */
s32 ptyComputeMaxHp(s32 unit) {
    s32 level;
    s32 stat;
    s32 result;

    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) != 0) {
        return ((SdfEnemyVitals *)(datEnemyRecords +
                ((SdfPartyUnit *)unit)->unitId * 76))->maxHp;
    }
    level = ((SdfPartyUnit *)unit)->level;
    stat = datGetStatWithStatusOverride((struct DatUnitStatus *)unit, 1);
    result = level * 4.0f +
             stat * datBattleParameters->maxHpGrowth[level - 1] + 10.0f;
    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) == 0) {
        result += ((SdfPartyUnit *)unit)->hpBonus;
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

s32 ptyComputeMaxMp(s32 unit) {
    s32 level;
    s32 stat;
    s32 result;

    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) != 0) {
        return ((SdfEnemyVitals *)(datEnemyRecords +
                ((SdfPartyUnit *)unit)->unitId * 76))->maxMp;
    }
    level = ((SdfPartyUnit *)unit)->level;
    stat = datGetStatWithStatusOverride((struct DatUnitStatus *)unit, 2);
    result = level * 4.0f +
             stat * datBattleParameters->maxMpGrowth[level - 1] + 8.0f;
    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) == 0) {
        result += ((SdfPartyUnit *)unit)->mpBonus;
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

s32 sdfDispatchPrimaryUnitScript(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (datCommandSelectors[unitIndex].kind == 5) {
        result = evtRunContext(0x19, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = datCommandRecords[unitIndex].flags;
        if (flags & 0x40) {
            if (unitIndex != 0x1E0) {
                result = evtRunContext(0x1F, scriptArg, contextArg, unitIndex, mode);
            } else {
                result = evtRunContext(0x1C, scriptArg, contextArg, 0x1E0, mode);
            }
        } else if (flags & 0x80) {
            result = evtRunContext(0x1D, scriptArg, contextArg, unitIndex, mode);
        } else {
            result = evtRunContext(5, scriptArg, contextArg, unitIndex, mode);
        }
    }
    return result;
}

void sdfDispatchCmd(u32 unitIndex, u32 scriptArg, u32 contextArg, u32 mode) {
    sdfDispatchPrimaryUnitScript(unitIndex, scriptArg, contextArg, (u8)mode);
}

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (datCommandSelectors[unitIndex].kind == 5) {
        result = evtRunContext(0x19, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = datCommandRecords[unitIndex].flags;
        if (flags & 0x40) {
            result = evtRunContext(0x1C, scriptArg, contextArg, unitIndex, mode);
        } else if (flags & 0x80) {
            result = evtRunContext(0x1D, scriptArg, contextArg, unitIndex, mode);
        } else {
            result = evtRunContext(9, scriptArg, contextArg, unitIndex, mode);
        }
    }
    return result;
}

void func_00118C58(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    evtRunContext(10, scriptArg, contextArg, unitIndex, mode);
}

void func_00118C80(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    evtRunContext(7, scriptArg, contextArg, unitIndex, mode);
}

void sdfDispatchSubCmd(u32 unitIndex, u32 scriptArg, u32 contextArg, u32 mode) {
    func_00118C80(unitIndex, scriptArg, contextArg, (u8)mode);
}


f32 sdfGetHpBracketScale(SdfPartyUnit *unit) {
    s32 hp = unit->hp;
    s32 band;

    if (hp % 10 != 0) {
        hp = hp - hp % 10 + 10;
    }
    band = hp * 10 / unit->maxHp - 1;
    if (band < 0) {
        band = 0;
    }
    if (band >= 10) {
        band = 9;
    }
    if (unit->flags & 0x20) {
        return datBattleParameters->enemyHpScale[band];
    }
    return datBattleParameters->partyHpScale[band];
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118D60);

extern s32 datFlagToElementIndex(u32);
extern s32 datGetEffectiveAffinity(struct DatUnitStatus *, s32);
extern s32 datUnitHasSkill(SdfPackedValue *, s32);
extern u32 effMiscRandMod(void *state, u32 modulus);
extern void func_0035B6E0(const char *fmt, ...);
extern char D_00412B08[]; /* "btl:bad ratio = %d%%[%d][%X]\n" */

/* Rolls whether the action hits: returns the surviving channel mask, or 0 on a miss. */
u32 sdfRollActionHit(s32 index, s32 queryArg, SdfPackedValue *packed) {
    u16 list[16];
    u16 count;
    u16 bit;
    u16 mask;
    s32 ratio = 100;
    u32 kind;
    s32 scaled;
    s32 roll;
    s32 hit;
    u16 flag;

    mask = datCommandRecords[index].attribute.parts.flagMask;
    if (datCommandRecords[index].attribute.parts.kind == 3) {
        count = 0;
        for (bit = 0; bit < 16; bit++) {
            if ((mask >> bit) & 1) {
                list[count] = bit;
                count++;
            }
        }
        mask = 1 << list[effMiscRandMod(0, count)];
    }
    if (mask != 0 && (datCommandRecords[index].attribute.parts.kind == 1 || datCommandRecords[index].attribute.parts.kind == 3)) {
        kind = datFlagToElementIndex(mask);
        if (!((u32)datCommandRecords[index].unk30 == 4 && (packed->flagsAndValue & 0x7FFF) == 8)) {
            if (datGetEffectiveAffinity((struct DatUnitStatus *)packed, kind) & 0x170000) {
                return 0;
            }
        }
        switch (kind) {
        case 3:
            if (datUnitHasSkill(packed, 0x25E)) {
                ratio = (u32)(datAbilityParameters[0x25E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
            }
            break;
        case 4:
            if (datUnitHasSkill(packed, 0x25F)) {
                ratio = (u32)(datAbilityParameters[0x25F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
            }
            break;
        case 9:
            if (datUnitHasSkill(packed, 0x263)) {
                ratio = (u32)(datAbilityParameters[0x263 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
            }
            break;
        }
    }
    flag = mask & 1;
    if (flag) {
        SdfPartyUnit *actor = (SdfPartyUnit *)packed;

        if (actor->hp * 100 / actor->maxHp >= 25) {
            mask &= 0xFFFE;
        } else if (!(*(u16 *)queryArg & 4)) {
            mask &= 0xFFFE;
        } else if ((actor->flags & SDF_UNIT_ENEMY) == 0 ||
                   (((SdfEnemyVitals *)(datEnemyRecords + actor->unitId * 76))->flags & 0x440) != 0) {
            mask &= 0xFFFE;
        }
    }
    if (mask & 0x8000) {
        if (packed->flagsAndValue & 0x1804) {
            mask &= 0x7FFF;
        }
    }
    if (mask == 0) {
        return 0;
    }
    hit = 1;
    if (datCommandRecords[index].attribute.parts.hitChance < 100) {
        scaled = evtRunContext(0xC, queryArg, packed, index, mask) * ((f32)ratio / 100.0f);
        func_0035B6E0(D_00412B08, scaled, ratio, mask);
        roll = effMiscRandMod(0, 100);
        hit = roll < scaled;
    }
    return hit ? mask : 0;
}

u32 sdfQueryChannelValue(s32 channel, s32 queryArg, SdfPackedValue *item) {
    u32 result;
    u32 mode = datCommandRecords[channel].attribute.parts.kind;

    if (mode != 1 && mode != 3) {
        return 0;
    }
    result = sdfRollActionHit(channel, queryArg, item);
    if (!((item->flagsAndValue & SDF_PACKED_VALUE_MASK) < result)) {
        result = 0;
    }
    return result;
}

u32 sdfQueryChannelBits(s32 channel, s32 queryArg, SdfPackedValue *item) {
    u32 result;

    if (datCommandRecords[channel].attribute.parts.kind != 2) {
        return 0;
    }
    result = sdfRollActionHit(channel, queryArg, item);
    if ((result & (item->flagsAndValue & SDF_PACKED_VALUE_MASK)) == 0) {
        result = 0;
    }
    return result;
}

extern s32 func_00118D60(s32 channel, s32 queryArg, SdfPackedValue *item, u32 mode, u8 vital);
extern void datClearUnitStatusBits(void *item, s32 mask);
extern s32 datAdjustCurrentHp(void *item, s32 delta);
extern s32 datAdjustCurrentMp(void *item, s32 delta);
void sdfRaisePackedChannelValue(SdfPackedValue *item, u32 value);

s32 sdfApplyCommandResults(s32 channel, s32 queryArg, SdfPackedValue *item) {
    s32 hpDelta;
    s32 mpDelta;
    u32 value;
    u32 bits;

    if (queryArg != 0 && item != NULL) {
        hpDelta = func_00118D60(channel, queryArg, item, 1, 1);
        mpDelta = func_00118D60(channel, queryArg, item, 1, 2);
        value = sdfQueryChannelValue(channel, queryArg, item);
        bits = sdfQueryChannelBits(channel, queryArg, item);
        if (hpDelta == 0 && mpDelta == 0 && value == 0 && bits == 0) {
            return 0;
        }
        sdfRaisePackedChannelValue(item, value);
        datClearUnitStatusBits(item, bits);
        if (mpDelta != 0) {
            datAdjustCurrentMp(item, mpDelta);
        }
        if (hpDelta != 0) {
            datAdjustCurrentHp(item, hpDelta);
            if (item->hp == 0) {
                sdfRaisePackedChannelValue(item, 0x4000);
            }
        }
    }
    switch ((u32)datCommandRecords[channel].unk30) {
    case 5:
        datGameState->world.fieldFlags |= 1;
        break;
    case 6:
        datGameState->world.fieldFlags |= 2;
        break;
    case 7:
        datGameState->world.fieldFlags |= 4;
        break;
    case 8:
        datGameState->world.fieldFlags |= 8;
        break;
    }
    return 1;
}

void sdfSetPackedValuePreservingFlag(SdfPackedValue *item, u16 value) {
    item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_FLAG) | (value & SDF_PACKED_VALUE_MASK);
}

extern void evtRandomizeEntryValue();

/* Raise the packed value to `value` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(SdfPackedValue *item, u32 value) {
    s32 channel;

    if ((item->flagsAndValue & SDF_PACKED_VALUE_MASK) < value) {
        item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_FLAG) | (value & SDF_PACKED_VALUE_MASK);
        channel = item->flagsAndValue & SDF_PACKED_VALUE_MASK;
        switch (channel) {
        case 0x40:
        case 0x80:
        case 0x400:
            evtRandomizeEntryValue(item);
            break;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001176A0", D_00412B08);

INCLUDE_SDATA(const s32, "game/code_001176A0", sdfRuntimeTaskName);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DB8);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DBC);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC4);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC5);

INCLUDE_SDATA(const s32, "game/code_001176A0", datGameState);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DD4);

