#include "common.h"
#include "pcp_vu0.h"

extern s64 scrGetWorkTaskHandle(void);
extern void scrDestroyWorkTask(void);

extern u32 D_003BAAAC;

extern s32 D_003BAA00;
extern s32 D_003BAA1C;
extern s32 D_003BAA4C;
extern u8 *D_003BAA50;
typedef struct EvtScaledValue {
    u32 unk0;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u32 value18;
} EvtScaledValue;

typedef struct SdfRuntime {
    u8 pad00[0x30];
    s32 backingAllocation; /* 0x30: handle returned by scene allocator */
    u32 firstTick;          /* 0x34 */
    u32 secondTick;         /* 0x38 */
    u8 pad3C[0xA20];
    u32 updateMode;         /* 0xA5C */
} SdfRuntime;

typedef struct SdfPackedValue {
    u8 pad00[0xE];
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

extern u32 func_001189A0(s32 index, s32 queryArg, SdfPackedValue *packed);
extern char D_003BA9E0[];
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 mode);
extern s32 sdfDecrementAllocationReferenceCount(s32 allocation);
extern s32 func_002D0918(s32 allocation);
void func_00117808(void);
s32 sdfBumpTickCounters(void);
void evtResetWorldAndProfileRuntime(void);
extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);

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

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 firstPayload; /* 0x4: first scalar passed to evtSpawnActionObj11 */
    s32 thirdPayload; /* 0x8: third scalar passed to evtSpawnActionObj11 */
    u8 unkC[0xC]; /* 0xC */
    void *secondPayload; /* 0x18: second payload passed to evtSpawnActionObj11 */
} ActionObj;

extern ActionObj *dds3AppendWorldObjectNode();

ActionObj *evtSpawnActionObj11(s32 a, s32 b, s32 c) {
    ActionObj *obj = dds3AppendWorldObjectNode(0x11);

    obj->secondPayload = (void *)b;
    obj->firstPayload = a;
    obj->thirdPayload = c;
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
    void *mem = func_002D03F8(0x33600);
    u8 *state = (u8 *)sdfResourceRetainAddress(mem);

    memset(state, 0, 0x33600);
    ((SdfRuntime *)state)->backingAllocation = (s32)mem;
    ((SdfRuntime *)state)->firstTick = 0;
    ((SdfRuntime *)state)->secondTick = 0;
    kwlnTaskCreate(D_003BA9E0, 1, 0, 0, (void *)sdfBumpTickCounters, 0, state);
    D_003BAA00 = (s32)state;
    evtResetWorldAndProfileRuntime();
}

/* Tear down the "GBWK" task hierarchy, clear the backing scene allocation
 * owned by the game state, and release the global state handle. */
void sdfDestroyRuntimeTask(void) {
    s32 handle;

    kwlnTaskDestroyWithHierarchyByName(D_003BA9E0, 0);
    func_00117808();
    handle = ((SdfRuntime *)D_003BAA00)->backingAllocation;
    sdfDecrementAllocationReferenceCount(handle);
    func_002D0918(handle);
    D_003BAA00 = 0;
}

s32 sdfBumpTickCounters(void) {
    SdfRuntime *runtime;

    runtime = (SdfRuntime *)D_003BAA00;
    runtime->firstTick += 1;
    runtime->secondTick += 1;
    return 0;
}

void evtResetWorldAndProfileRuntime(void) {
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    ((SdfRuntime *)D_003BAA00)->updateMode = 8;
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

INCLUDE_ASM(const s32, "game/code_00117438", func_00117810);

INCLUDE_ASM(const s32, "game/code_00117438", func_00117C48);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118020);

void func_001180F8(void) {
    dds3WorkInit(D_003BAAAC);
}

void sdfFirePendingCallback(void) {
    if (D_003BAAAC == 0) {
        return;
    }
    scrDestroyWorkTask();
}

u8 scrIsCurrentWorkTask(s64 expectedValue) {
    s64 currentValue;

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
        return ((SdfEnemyVitals *)(D_003BAA1C + ((SdfPartyUnit *)unit)->unitId * 76))->maxHp;
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
        return ((SdfEnemyVitals *)(D_003BAA1C + ((SdfPartyUnit *)unit)->unitId * 76))->maxMp;
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

    if (((SdfUnitMode *)D_003BAA4C)[unitIndex].kind == 5) {
        result = evtRunContext(0x18, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = D_003BAA50[unitIndex * 0x38];
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

    if (((SdfUnitMode *)D_003BAA4C)[unitIndex].kind == 5) {
        result = evtRunContext(0x18, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = D_003BAA50[unitIndex * 0x38];
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

extern s32 D_003BAA5C;
extern s32 func_00119750(u32);
extern u32 func_00119520(SdfPackedValue *, s32);
extern s32 datUnitHasSkill(SdfPackedValue *, s32);
extern s32 effMiscRandMod(s32, s32);
extern void func_003003F0(const char *, ...);
extern char D_0039F980[]; /* "btl:bad ratio = %d%%[%d][%X]\n" */

/* Per-slot battle record (0x38 bytes) in D_003BAA50. */
typedef struct SdfBattleSlot {
    u8 pad00[0x24];
    u8 type;        /* 0x24: 1/3 = skill-scaled hit roll, 2 = bit query */
    u8 chance;      /* 0x25: hit chance in percent, >= 100 always hits */
    u16 mask;       /* 0x26: candidate channel bits */
    u8 pad28[8];
    s32 mode;       /* 0x30 */
    u8 pad34[4];
} SdfBattleSlot;

#define SDF_BATTLE_SLOT(i) ((SdfBattleSlot *)(D_003BAA50 + (i) * 0x38))

/* Rolls whether the action hits: returns the surviving channel mask, or 0 on a miss. */
u32 func_001189A0(s32 index, s32 queryArg, SdfPackedValue *packed) {
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
        kind = func_00119750(mask);
        if (!(SDF_BATTLE_SLOT(index)->mode == 4 && (packed->flagsAndValue & 0x7FFF) == 8)) {
            if (func_00119520(packed, kind) & 0x170000) {
                return 0;
            }
        }
        switch (kind) {
        case 3:
            if (datUnitHasSkill(packed, 0x23E)) {
                ratio = (u32)(*(f32 *)(D_003BAA5C + 0x1F0) * (f32)ratio);
            }
            break;
        case 4:
            if (datUnitHasSkill(packed, 0x23F)) {
                ratio = (u32)(*(f32 *)(D_003BAA5C + 0x1F8) * (f32)ratio);
            }
            break;
        case 9:
            if (datUnitHasSkill(packed, 0x243)) {
                ratio = (u32)(*(f32 *)(D_003BAA5C + 0x218) * (f32)ratio);
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
                   (((SdfEnemyVitals *)(D_003BAA1C + actor->unitId * 76))->flags & 0x440) != 0) {
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
    u32 mode = D_003BAA50[channel * 0x38 + 0x24];

    if (mode != 1 && mode != 3) {
        return 0;
    }
    result = func_001189A0(channel, queryArg, item);
    if (!((item->flagsAndValue & SDF_PACKED_CHANNEL_MASK) < result)) {
        result = 0;
    }
    return result;
}

u32 sdfQueryChannelBits(s32 channel, s32 queryArg, SdfPackedValue *item) {
    u32 result;

    if (D_003BAA50[channel * 0x38 + 0x24] != 2) {
        return 0;
    }
    result = func_001189A0(channel, queryArg, item);
    if ((result & (item->flagsAndValue & SDF_PACKED_CHANNEL_MASK)) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

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

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E0);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E9);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EA);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EC);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F0);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F4);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F9);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BAA00);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BAA04);

