#include "common.h"
#include "pcp_vu0.h"

extern u32 D_00435E80;

extern s32 D_00435DD0;

extern s64 scrGetWorkTaskHandle(void);
extern void func_00118AB0();
extern char D_00435DB0[]; /* "GBWK" */
extern void kwlnTaskDestroyWithHierarchyByName(char *name, s32 flag);
extern void func_00117A80(void);
extern void sdfDecrementAllocationReferenceCount(u32 allocation);
extern s32 func_003297C8(u32 allocation);
extern struct ActionObj *dds3AppendWorldObjectNode();

extern void *func_003292A8(s32 size);

extern void *sdfResourceRetainAddress(void *resource);

extern s32 kwlnTaskCreate(char *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);

s32 sdfBumpTickCounters(void);

void evtResetWorldAndProfileRuntime(void);

typedef struct SdfChannel {
    u8 pad00[0xA4];
} SdfChannel;

extern SdfChannel D_00385A90[8];
extern void func_00118798(SdfChannel *channel);

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 firstPayload; /* 0x4: first scalar passed to evtSpawnActionObj11 */
    s32 thirdPayload; /* 0x8: third scalar passed to evtSpawnActionObj11 */
    u8 unkC[0xC]; /* 0xC */
    s32 secondPayload; /* 0x18: second payload passed to evtSpawnActionObj11 */
} ActionObj;

typedef struct EvtScaledValue {
    u32 value;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u32 value18;
} EvtScaledValue;

typedef struct SdfRuntime {
    u8 pad00[0x30];
    s32 backingAllocation; /* 0x30: handle returned by scene allocator */
    u32 firstTick;
    u32 secondTick;
    u8 pad3C[0xA20];
    u32 updateMode;
} SdfRuntime;

typedef struct SdfPackedValue {
    u8 pad00[0xE];
    u16 flagsAndValue;
} SdfPackedValue;

#define SDF_PACKED_FLAG 0x8000
#define SDF_PACKED_VALUE_MASK 0x7FFF

typedef struct SdfChannelState {
    u8 scriptFlags; /* 0x00: 0x40/0x80 select alternate dispatch scripts */
    u8 pad01[0x23];
    u8 mode; /* 0x24 */
    u8 pad25[0x13];
} SdfChannelState;

/* Byte 0 supplies an entry/resource code; byte 1 selects the script kind. */
typedef struct SdfUnitMode {
    s8 code; /* 0x00: used as a resource index by sound/UI consumers */
    s8 kind; /* 0x01: 5 selects the alternate unit script */
} SdfUnitMode;

extern SdfUnitMode *D_00435E1C;

extern SdfChannelState *D_00435E20;
extern u32 func_001190B0(s32 channel, s32 arg1, SdfPackedValue *item);

extern void scrDestroyWorkTask(void);

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

extern void func_00116DE8(s32 *index, f32 *fraction, void *table, f32 time);

/* Linearly interpolated curve sample at `time`; 0 when no curve is active. */
f32 sdfSampleActiveLinearCurve(SdfCurveUser *user) {
    s32 index;
    f32 fraction;
    SdfCurveTable *curve;
    f32 *values;

    if (user->flags & 4) {
        curve = user->curve;
        func_00116DE8(&index, &fraction, curve, user->time);
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

void func_001177D0(u32 *destination, u32 value) {
    *destination = value;
}

u32 func_001177D8(u32 *source) {
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

ActionObj *evtSpawnActionObj11(s32 a, s32 b, s32 c) {
    ActionObj *obj = dds3AppendWorldObjectNode(0x11);

    obj->secondPayload = b;
    obj->firstPayload = a;
    obj->thirdPayload = c;
    return obj;
}

u32 func_001178B0(EvtScaledValue *value) {
    return value->value18;
}

u32 func_001178B8(EvtScaledValue *value) {
    return value->value18;
}

/* Load the vector at the slot's +0x18 pointer into VF10. */
void evtLoadValueVectorIntoVu(EvtScaledValue *value) {
    void *vec = (void *)func_001178B8(value);

    VU0_LOAD_VF_MEMORY(vf10, vec);
}

/* Same, from the second quadword at +0x18. */
void evtLoadValueSecondaryVectorIntoVu(EvtScaledValue *value) {
    void *vec = (void *)((u8 *)func_001178B8(value) + 0x10);

    VU0_LOAD_VF_MEMORY(vf10, vec);
}

/* Allocate the runtime state block, zero it and register the "GBWK" tick task. */
void sdfCreateRuntimeTask(void) {
    void *mem = func_003292A8(0x1E840);
    u8 *state = (u8 *)sdfResourceRetainAddress(mem);

    memset(state, 0, 0x1E840);
    ((SdfRuntime *)state)->backingAllocation = (s32)mem;
    ((SdfRuntime *)state)->firstTick = 0;
    ((SdfRuntime *)state)->secondTick = 0;
    kwlnTaskCreate(D_00435DB0, 1, 0, 0, (void *)sdfBumpTickCounters, 0, state);
    D_00435DD0 = (s32)state;
    evtResetWorldAndProfileRuntime();
}

void sdfDestroyRuntimeTask(void) {
    u32 allocation;

    kwlnTaskDestroyWithHierarchyByName(D_00435DB0, 0);
    func_00117A80();
    allocation = ((SdfRuntime *)D_00435DD0)->backingAllocation;
    sdfDecrementAllocationReferenceCount(allocation);
    func_003297C8(allocation);
    D_00435DD0 = 0;
}

s32 sdfBumpTickCounters(void) {
    SdfRuntime *runtime;

    runtime = (SdfRuntime *)D_00435DD0;
    runtime->firstTick += 1;
    runtime->secondTick += 1;
    return 0;
}

void evtResetWorldAndProfileRuntime(void) {
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    ((SdfRuntime *)D_00435DD0)->updateMode = 8;
    func_0011AB38();
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

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117A88);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118008);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118598);

void func_00118680(void) {
    dds3WorkInit(D_00435E80);
}

void sdfFirePendingCallback(void) {
    if (D_00435E80 == 0) {
        return;
    }
    scrDestroyWorkTask();
}

u8 scrIsCurrentWorkTask(s64 expected) {
    s64 current;

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

INCLUDE_ASM(const s32, "game/code_001176A0", func_001188F0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001189D0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118AB0);

void sdfDispatchCmd(u32 context, u32 first, u32 second, u32 flags) {
    func_00118AB0(context, first, second, (u8)flags);
}

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (D_00435E1C[unitIndex].kind == 5) {
        result = evtRunContext(0x19, scriptArg, contextArg, unitIndex, mode);
    } else {
        flags = D_00435E20[unitIndex].scriptFlags;
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

void func_00118C58(u32 context, u32 first, u32 second, u8 flags) {
    evtRunContext(10, first, second, context, flags);
}

void func_00118C80(u32 context, u32 first, u32 second, u8 flags) {
    evtRunContext(7, first, second, context, flags);
}

void sdfDispatchSubCmd(u32 context, u32 first, u32 second, u32 flags) {
    func_00118C80(context, first, second, (u8)flags);
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118CC0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118D60);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001190B0);

u32 sdfQueryChannelValue(s32 channel, s32 arg1, SdfPackedValue *item) {
    u32 result;
    u32 mode = D_00435E20[channel].mode;

    if (mode != 1 && mode != 3) {
        return 0;
    }
    result = func_001190B0(channel, arg1, item);
    if (!((item->flagsAndValue & SDF_PACKED_VALUE_MASK) < result)) {
        result = 0;
    }
    return result;
}

u32 sdfQueryChannelBits(s32 channel, s32 arg1, SdfPackedValue *item) {
    u32 result;

    if (D_00435E20[channel].mode != 2) {
        return 0;
    }
    result = func_001190B0(channel, arg1, item);
    if ((result & (item->flagsAndValue & SDF_PACKED_VALUE_MASK)) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119548);

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

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DB0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DB8);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DBC);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC4);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC5);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DD0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DD4);

