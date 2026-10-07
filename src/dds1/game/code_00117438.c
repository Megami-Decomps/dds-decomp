#include "common.h"
#include "eff_transform.h"
#include "pcp_vu0.h"
#include "btl_action.h"
#include "dat_state.h"
#include "sdf.h"

extern u32 scrGetWorkTaskHandle(void);
extern void scrDestroyWorkTask(void);

extern u32 D_003BAAAC;

extern s32 datEnemyRecords;
extern s32 datCommandSelectors;
extern u8 *datCommandRecords;
typedef struct EvtScaledValue {
    u32 unk0;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u32 value18;
} EvtScaledValue;


typedef struct SdfPackedValue {
    u8 pad00[6];
    u16 hp; /* 0x06 */
    u8 pad08[6];
    u16 flagsAndValue;
} SdfPackedValue;

/* Channel mask occupies the low 15 bits; the high status bit survives updates. */
#define SDF_PACKED_STATUS_BIT 0x8000
#define SDF_PACKED_CHANNEL_MASK 0x7fff

/* The 0x20 flag selects base enemy vitals instead of the party script path. */
#define SDF_UNIT_ENEMY 0x20

/* Party-unit header used for HP/MP script dispatch. Full stride: 0x1A4. */
typedef struct SdfPartyUnit {
    u16 flags;          /* 0x00 */
    u8 pad02[2];
    u16 unitId;         /* 0x04 */
    u16 hp;             /* 0x06 */
    u16 maxHp;          /* 0x08 */
    u8 pad0A[0x12];
    u16 hpBonus;        /* 0x1C */
    u16 mpBonus;        /* 0x1E */
} SdfPartyUnit;

typedef struct SdfEnemyVitals {
    u32 flags;          /* 0x00 */
    u8 pad04[4];
    u16 maxHp;          /* 0x08 */
    u8 pad0A[2];
    u16 maxMp;          /* 0x0C */
} SdfEnemyVitals;

/* Two-byte runtime-mode entries; the signed second byte selects script 0x18. */
typedef struct SdfUnitMode {
    s8 code; /* 0x00: used as a resource index by sound/UI consumers */
    s8 kind; /* 0x01: 5 selects the alternate unit script */
} SdfUnitMode;

extern u32 sdfRollActionHit(s32 index, s32 queryArg, SdfPackedValue *packed);
extern char sdfRuntimeTaskName[];
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 mode);
extern s32 sdfDecrementAllocationReferenceCount(s32 allocation);
extern s32 sdfReleaseResourceAllocation(s32 allocation);
void func_00117808(void);
s32 sdfBumpTickCounters(void);
void evtResetWorldAndProfileRuntime(void);
extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);

extern void scrClearProcessGlobals(void);
extern void mdlResetViewerFlagsAndSolarOverlay(void);
extern void ptyClearProfileRecords(void);
extern void ptyRebuildAllProfiles(void);
extern void evtUpdateFlaggedEntries(void);
extern void dds3ForEachEntry(void);
extern void mdlFlagClear(s32 flag);
extern void func_00120C08(s32 mode);
extern void sdfSaveResetSnapshot(void);
extern void func_00117C48(void);

s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode);
typedef struct SdfCurveTable {
    u8 pad00[4];
    f32 *values;          /* 0x04 */
} SdfCurveTable;

typedef struct SdfCurveUser {
    u8 pad00[4];
    u32 flags;            /* 0x04: bit 2 = curve active */
    u8 pad08[4];
    f32 time;             /* 0x0C */
    u8 pad10[0xC];
    SdfCurveTable *curve; /* 0x1C */
} SdfCurveUser;

extern void func_00116B80(s32 *index, f32 *fraction, void *table, f32 time);

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 sdfSampleActiveLinearCurve(SdfCurveUser *user) {
    s32 index;
    f32 fraction;
    SdfCurveTable *curve;
    f32 *values;

    if (user->flags & 4) {
        curve = user->curve;
        func_00116B80(&index, &fraction, curve, user->time);
        values = curve->values;
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

void func_00117568(u32 *destination, u32 value) {
    *destination = value;
}

u32 func_00117570(u32 *source) {
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


extern EffWorldNode *dds3AppendWorldObjectNode();

EffWorldNode *evtSpawnActionObj11(s32 a, s32 b, s32 c) {
    EffWorldNode *obj = dds3AppendWorldObjectNode(0x11);

    obj->data = (void *)b;
    obj->key = a;
    obj->value = c;
    return obj;
}

u32 func_00117648(EvtScaledValue *value) {
    return value->value18;
}

u32 func_00117650(EvtScaledValue *value) {
    return value->value18;
}

/* Load the vector at the scaled value's +0x18 pointer into VF10. */
void evtLoadValueVectorIntoVu(EvtScaledValue *value) {
    void *vec = (void *)func_00117650(value);

    VU0_LOAD_VF_MEMORY(vf10, vec);

}

/* Same, from the second quadword at +0x18. */
void evtLoadValueSecondaryVectorIntoVu(EvtScaledValue *value) {
    void *vec = (void *)((u8 *)func_00117650(value) + 0x10);

    VU0_LOAD_VF_MEMORY(vf10, vec);

}

/* Allocate the 0x33600 game-state block, retain its scene allocation, zero it
 * and register the "GBWK" tick task that owns it. */
void sdfCreateRuntimeTask(void) {
    void *mem = sdfAllocGeneralBlock(0x33600);
    DatGameState *state = sdfResourceRetainAddress(mem);

    memset(state, 0, 0x33600);
    state->header.backingAllocation = (s32)mem;
    state->header.firstTick = 0;
    state->header.secondTick = 0;
    kwlnTaskCreate(sdfRuntimeTaskName, 1, 0, 0, (void *)sdfBumpTickCounters, 0, state);
    datGameState = state;
    evtResetWorldAndProfileRuntime();
}

/* Tear down the "GBWK" task hierarchy, clear the backing scene allocation
 * owned by the game state, and release the global state handle. */
void sdfDestroyRuntimeTask(void) {
    s32 handle;

    kwlnTaskDestroyWithHierarchyByName(sdfRuntimeTaskName, 0);
    func_00117808();
    handle = datGameState->header.backingAllocation;
    sdfDecrementAllocationReferenceCount(handle);
    sdfReleaseResourceAllocation(handle);
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
    func_00120C08(0);
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    btlClearSharedBattleStateWords();
}

void func_00117808(void) {
}

extern s8 D_003BA9E5;
extern s8 D_003BA9E6;
extern s8 D_003BA9E7;
extern s8 D_003BA9E8;
extern s8 D_003BA9E9;
extern s8 D_003BA9EA;
extern u16 D_003BA9EC;
extern u32 D_003BA9F0;
extern u32 D_003BA9F4;
extern s8 D_003BA9F8;
extern s8 D_003BA9F9;
extern u32 D_003C2E30[3];
extern u32 D_0032A200[3];
extern u8 D_003C2E40[41];
extern s32 D_003BD790;
extern SdfMemBlock *D_003BD794;
extern SdfMemBlock *D_003BD798;
extern SdfMemBlock *D_003BD79C;
extern u32 D_003BD7A0;
extern s32 mdlFlagTest(s32 flag);
extern s32 mnuCreateFlagEntries(void);
extern u32 sdfMemoryGetBlockAddress(SdfMemBlock *block);
extern void ptySaveActiveUnitsToStock(void);
extern void mnuCollectFlagArray(u8 *flags);

/* Preserve carried flags and progress before the full runtime reset. */
void sdfSaveResetSnapshot(void) {
    void *copy;

    D_003BA9E5 = 0;
    D_003BA9E6 = 0;
    D_003BA9E7 = 0;
    D_003BA9E8 = 0;
    D_003BA9E9 = 0;
    D_003BA9EA = 0;
    D_003BA9F8 = 0;
    D_003BA9F9 = 0;
    memset(D_003C2E30, 0, sizeof(D_003C2E30));
    D_003BA9EC = 0;
    D_003BA9F0 = 0;
    if (mdlFlagTest(0xB90)) {
        D_003BA9E5 = 1;
    }
    if (mdlFlagTest(0xB91)) {
        D_003BA9E6 = 1;
    }
    if (mdlFlagTest(0xB92)) {
        D_003BA9E7 = 1;
    }
    if (mdlFlagTest(0xB93)) {
        D_003BA9E8 = 1;
    }
    if (mdlFlagTest(0xB94)) {
        D_003BA9E9 = 1;
    }
    if (mdlFlagTest(0xB95)) {
        D_003BA9EA = 1;
    }
    if (mdlFlagTest(0xC0E)) {
        D_003BA9F8 = 1;
    }
    memcpy(D_003C2E30, datGameState->battleFlags, sizeof(datGameState->battleFlags));
    D_003BA9EC = datGameState->header.unk0C;
    D_003BA9F0 = datGameState->header.secondTick;
    D_003BA9F4 = datGameState->header.unk20;
    D_0032A200[0] = datGameState->header.unk24;
    D_0032A200[1] = datGameState->header.unk28;
    D_0032A200[2] = datGameState->header.unk2C;
    D_003BD790 = mnuCreateFlagEntries();
    D_003BD794 = sdfAllocGeneralBlock(sizeof(datGameState->mantraBits));
    copy = (void *)sdfMemoryGetBlockAddress(D_003BD794);
    memcpy(copy, datGameState->mantraBits, sizeof(datGameState->mantraBits));
    D_003BD798 = sdfAllocGeneralBlock(sizeof(datGameState->profileRecords));
    copy = (void *)sdfMemoryGetBlockAddress(D_003BD798);
    memcpy(copy, datGameState->profileRecords, sizeof(datGameState->profileRecords));
    ptySaveActiveUnitsToStock();
    D_003BD79C = sdfAllocGeneralBlock(sizeof(datGameState->templates));
    copy = (void *)sdfMemoryGetBlockAddress(D_003BD79C);
    memcpy(copy, datGameState->templates, sizeof(datGameState->templates));
    memset(D_003C2E40, 0, sizeof(D_003C2E40));
    mnuCollectFlagArray(D_003C2E40);
    D_003BD7A0 = datGameState->world.slotFlags;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00117C48);

/* A full reset preserves the scene allocation and restores the saved runtime data. */
void sdfResetGameRuntime(s32 fullReset) {
    s32 backingAllocation;

    if (fullReset == 1) {
        sdfSaveResetSnapshot();
        backingAllocation = datGameState->header.backingAllocation;
        memset(datGameState, 0, 0x33600);
        datGameState->header.backingAllocation = backingAllocation;
    }
    datGameState->header.firstTick = 0;
    datGameState->header.secondTick = 0;
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    datGameState->world.updateMode = 8;
    ptyInitRuntime();
    mdlFlagClear(0xC0E);
    func_00120C08(0);
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    if (fullReset == 1) {
        func_00117C48();
    }
}

void func_001180F8(void) {
    dds3WorkInit(D_003BAAAC);
}

void sdfFirePendingCallback(void) {
    if (D_003BAAAC == 0) {
        return;
    }
    scrDestroyWorkTask();
}

u8 scrIsCurrentWorkTask(u32 expectedValue) {
    u32 currentValue;

    currentValue = scrGetWorkTaskHandle();
    return currentValue == expectedValue;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118170);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118210);

extern u8 D_0032A6F0[];
extern void func_00118210();

void sdfResetChannels(void) {
    u8 *entry;
    u32 i;

    sdfFirePendingCallback();
    entry = D_0032A6F0;
    for (i = 0; i < 8; i++) {
        func_00118210(entry);
        entry += 0xA4;
    }
    func_001180F8();
}

/* Enemy vitals use a 76-byte base table; party vitals come from a script,
 * a unit-specific bonus, and a maximum of 999. */
s32 ptyComputeMaxHp(s32 unit) {
    s32 result;

    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) != 0) {
        return ((SdfEnemyVitals *)(datEnemyRecords + ((SdfPartyUnit *)unit)->unitId * 76))->maxHp;
    }
    result = evtRunContext(1, unit, 0, 0, 0);
    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) == 0) {
        result += ((SdfPartyUnit *)unit)->hpBonus;
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

/* Compute the matching MP value from the base table or the MP script. */
s32 ptyComputeMaxMp(s32 unit) {
    s32 result;

    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) != 0) {
        return ((SdfEnemyVitals *)(datEnemyRecords + ((SdfPartyUnit *)unit)->unitId * 76))->maxMp;
    }
    result = evtRunContext(2, unit, 0, 0, 0);
    if ((((SdfPartyUnit *)unit)->flags & SDF_UNIT_ENEMY) == 0) {
        result += ((SdfPartyUnit *)unit)->mpBonus;
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

/* Sdf dispatch: pick the script id from the unit's runtime flag byte and run
 * it against the given entry index. 0x40/0x80 select the alternate scripts. */
s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (((SdfUnitMode *)datCommandSelectors)[unitIndex].kind == 5) {
        result = evtRunContext(0x18, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = datCommandRecords[unitIndex * 0x38];
        if (flags & 0x40) {
            result = evtRunContext(0x1B, scriptArg, contextArg, unitIndex, mode);
        } else if (flags & 0x80) {
            result = evtRunContext(0x1C, scriptArg, contextArg, unitIndex, mode);
        } else {
            result = evtRunContext(5, scriptArg, contextArg, unitIndex, mode);
        }
    }
    return result;
}

void sdfDispatchCmd(u32 unitIndex, u32 scriptArg, u32 contextArg, u32 mode) {
    sdfDispatchUnitScriptDefault5(unitIndex, scriptArg, contextArg, (u8)mode);
}

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (((SdfUnitMode *)datCommandSelectors)[unitIndex].kind == 5) {
        result = evtRunContext(0x18, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = datCommandRecords[unitIndex * 0x38];
        if (flags & 0x40) {
            result = evtRunContext(0x1B, scriptArg, contextArg, unitIndex, mode);
        } else if (flags & 0x80) {
            result = evtRunContext(0x1C, scriptArg, contextArg, unitIndex, mode);
        } else {
            result = evtRunContext(9, scriptArg, contextArg, unitIndex, mode);
        }
    }
    return result;
}

void func_00118620(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    evtRunContext(10, scriptArg, contextArg, unitIndex, mode);
}

void func_00118648(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    evtRunContext(7, scriptArg, contextArg, unitIndex, mode);
}

void sdfDispatchSubCmd(u32 unitIndex, u32 scriptArg, u32 contextArg, u32 mode) {
    func_00118648(unitIndex, scriptArg, contextArg, (u8)mode);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118688);

extern s32 datFlagToElementIndex(u32);
struct DatUnitStatus;
extern s32 datGetEffectiveAffinity(struct DatUnitStatus *, s32);
extern s32 datUnitHasSkill(SdfPackedValue *, s32);
extern s32 effMiscRandMod(s32, s32);
extern void func_003003F0(const char *, ...);
extern char D_0039F980[]; /* "btl:bad ratio = %d%%[%d][%X]\n" */

/* Per-slot battle record (0x38 bytes) in datCommandRecords. */
typedef struct SdfBattleSlot {
    u8 pad00[0x24];
    u8 type;        /* 0x24: 1/3 = skill-scaled hit roll, 2 = bit query */
    u8 chance;      /* 0x25: hit chance in percent, >= 100 always hits */
    u16 mask;       /* 0x26: candidate channel bits */
    u8 pad28[8];
    u32 mode;       /* 0x30 */
    u8 pad34[4];
} SdfBattleSlot;

#define SDF_BATTLE_SLOT(i) ((SdfBattleSlot *)(datCommandRecords + (i) * 0x38))

/* Rolls whether the action hits: returns the surviving channel mask, or 0 on a miss. */
u32 sdfRollActionHit(s32 index, s32 queryArg, SdfPackedValue *packed) {
    u16 list[16];
    u16 count;
    u16 bit;
    u16 mask;
    s32 ratio = 100;
    u32 kind;
    s32 scaled;
    s32 hit;
    u16 flag;

    mask = SDF_BATTLE_SLOT(index)->mask;
    if (SDF_BATTLE_SLOT(index)->type == 3) {
        count = 0;
        for (bit = 0; bit < 16; bit++) {
            if ((mask >> bit) & 1) {
                list[count] = bit;
                count++;
            }
        }
        mask = 1 << list[effMiscRandMod(0, count)];
    }
    if (mask != 0 && (SDF_BATTLE_SLOT(index)->type == 1 || SDF_BATTLE_SLOT(index)->type == 3)) {
        kind = datFlagToElementIndex(mask);
        if (!(SDF_BATTLE_SLOT(index)->mode == 4 && (packed->flagsAndValue & 0x7FFF) == 8)) {
            if (datGetEffectiveAffinity((struct DatUnitStatus *)packed, kind) & 0x170000) {
                return 0;
            }
        }
        switch (kind) {
        case 3:
            if (datUnitHasSkill(packed, 0x23E)) {
                ratio = (u32)(datAbilityParameters[0x23E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
            }
            break;
        case 4:
            if (datUnitHasSkill(packed, 0x23F)) {
                ratio = (u32)(datAbilityParameters[0x23F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
            }
            break;
        case 9:
            if (datUnitHasSkill(packed, 0x243)) {
                ratio = (u32)(datAbilityParameters[0x243 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)ratio);
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
    if (SDF_BATTLE_SLOT(index)->chance < 100) {
        scaled = evtRunContext(0xC, queryArg, packed, index, mask) * ((f32)ratio / 100.0f);
        func_003003F0(D_0039F980, scaled, ratio, mask);
        hit = effMiscRandMod(0, 100) < scaled;
    }
    return hit ? mask : 0;
}

u32 sdfQueryChannelValue(s32 channel, s32 queryArg, SdfPackedValue *item) {
    u32 result;
    u32 mode = datCommandRecords[channel * 0x38 + 0x24];

    if (mode != 1 && mode != 3) {
        return 0;
    }
    result = sdfRollActionHit(channel, queryArg, item);
    if (!((item->flagsAndValue & SDF_PACKED_CHANNEL_MASK) < result)) {
        result = 0;
    }
    return result;
}

u32 sdfQueryChannelBits(s32 channel, s32 queryArg, SdfPackedValue *item) {
    u32 result;

    if (datCommandRecords[channel * 0x38 + 0x24] != 2) {
        return 0;
    }
    result = sdfRollActionHit(channel, queryArg, item);
    if ((result & (item->flagsAndValue & SDF_PACKED_CHANNEL_MASK)) == 0) {
        result = 0;
    }
    return result;
}

extern s32 func_00118688(s32 channel, s32 queryArg, SdfPackedValue *item, u32 mode, u8 vital);
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
        hpDelta = func_00118688(channel, queryArg, item, 1, 1);
        mpDelta = func_00118688(channel, queryArg, item, 1, 2);
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
    switch (SDF_BATTLE_SLOT(channel)->mode) {
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
    item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_STATUS_BIT) | (value & SDF_PACKED_CHANNEL_MASK);
}

extern void evtRandomizeEntryValue();

/* Raise the packed value to `value` (never lower it); reaching 0x40, 0x80 or 0x400 re-randomizes the entry. */
void sdfRaisePackedChannelValue(SdfPackedValue *item, u32 value) {
    s32 channel;

    if ((item->flagsAndValue & SDF_PACKED_CHANNEL_MASK) < value) {
        item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_STATUS_BIT) | (value & SDF_PACKED_CHANNEL_MASK);
        channel = item->flagsAndValue & SDF_PACKED_CHANNEL_MASK;
        switch (channel) {
        case 0x40:
        case 0x80:
        case 0x400:
            evtRandomizeEntryValue(item);
            break;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00117438", D_0039F980);

INCLUDE_SDATA(const s32, "game/code_00117438", sdfRuntimeTaskName);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E9);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EA);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EC);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F0);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F4);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F9);

INCLUDE_SDATA(const s32, "game/code_00117438", datGameState);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BAA04);

