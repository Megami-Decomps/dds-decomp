#include "common.h"

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
    u8 pad06[0x16];
    u16 hpBonus;        /* 0x1C */
    u16 mpBonus;        /* 0x1E */
} SdfPartyUnit;

typedef struct SdfEnemyVitals {
    u8 pad00[8];
    u16 maxHp;          /* 0x08 */
    u8 pad0A[2];
    u16 maxMp;          /* 0x0C */
} SdfEnemyVitals;

/* Two-byte runtime-mode entries; the signed second byte selects script 0x18. */
typedef struct SdfUnitMode {
    u8 pad00;           /* 0x00 */
    s8 mode;            /* 0x01 */
} SdfUnitMode;

extern u32 func_001189A0(s32 index, s32 queryArg, SdfPackedValue *packed);
extern char D_003BA9E0[];
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 mode);
extern s32 func_002D0A60(s32 allocation);
extern s32 func_002D0918(s32 allocation);
void func_00117808(void);
s32 sdfBumpTickCounters(void);
void func_001177A8(void);
extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern s32 kwlnTaskCreate(void *name, s32 priority, s32 group, s32 flags, void *update, void *destroy, void *data);

s32 sdfDispatchUnitScriptDefault5(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode);
INCLUDE_ASM(const s32, "game/code_00117438", func_00117438);

INCLUDE_ASM(const s32, "game/code_00117438", func_001174C0);

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

void func_001175A8(EvtScaledValue *value) {
    value->flags = value->flags | 8;
}

void func_001175B8(EvtScaledValue *value) {
    value->flags = value->flags & 0xfffffff7;
}

void func_001175D0(EvtScaledValue *value) {
    value->flags = value->flags | 0x20;
}

void func_001175E0(EvtScaledValue *value) {
    value->flags = value->flags & 0xffffffdf;
}

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

extern ActionObj *func_00110880();

ActionObj *evtSpawnActionObj11(s32 a, s32 b, s32 c) {
    ActionObj *obj = func_00110880(0x11);

    obj->unk18 = (void *)b;
    obj->unk4 = a;
    obj->unk8 = c;
    return obj;
}

u32 func_00117648(EvtScaledValue *value) {
    return value->value18;
}

u32 func_00117650(EvtScaledValue *value) {
    return value->value18;
}

/* Load the vector at the scaled value's +0x18 pointer into VF10. */
void func_00117658(EvtScaledValue *value) {
    void *vec = (void *)func_00117650(value);

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        :
        : "r"(vec)
        : "memory"
    );
}

/* Same, from the second quadword at +0x18. */
void func_00117678(EvtScaledValue *value) {
    void *vec = (void *)((u8 *)func_00117650(value) + 0x10);

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        :
        : "r"(vec)
        : "memory"
    );
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
    func_001177A8();
}

/* Tear down the "GBWK" task hierarchy, clear the backing scene allocation
 * owned by the game state, and release the global state handle. */
void sdfDestroyRuntimeTask(void) {
    s32 handle;

    kwlnTaskDestroyWithHierarchyByName(D_003BA9E0, 0);
    func_00117808();
    handle = ((SdfRuntime *)D_003BAA00)->backingAllocation;
    func_002D0A60(handle);
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

void func_001177A8(void) {
    scrClearProcessGlobals();
    mdlResetViewerFlagsAndSolarOverlay();
    ((SdfRuntime *)D_003BAA00)->updateMode = 8;
    ptyInitRuntime();
    func_00120C08(0);
    ptyClearProfileRecords();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    func_001ACCF0();
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

u8 func_00118140(s64 expectedValue) {
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

    if (((SdfUnitMode *)D_003BAA4C)[unitIndex].mode == 5) {
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

void sdfDispatchCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    sdfDispatchUnitScriptDefault5(arg0, arg1, arg2, (u8)arg3);
}

s32 sdfDispatchUnitScriptDefault9(u32 unitIndex, u32 scriptArg, u32 contextArg, u8 mode) {
    s32 result;
    u8 flags;

    if (((SdfUnitMode *)D_003BAA4C)[unitIndex].mode == 5) {
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

void func_00118620(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(10, arg1, arg2, arg0, arg3);
}

void func_00118648(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(7, arg1, arg2, arg0, arg3);
}

void sdfDispatchSubCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_00118648(arg0, arg1, arg2, (u8)arg3);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118688);

INCLUDE_ASM(const s32, "game/code_00117438", func_001189A0);

u32 sdfQueryChannelValue(s32 channel, s32 arg1, SdfPackedValue *item) {
    u32 result;
    u32 mode = D_003BAA50[channel * 0x38 + 0x24];

    if (mode != 1 && mode != 3) {
        return 0;
    }
    result = func_001189A0(channel, arg1, item);
    if (!((item->flagsAndValue & SDF_PACKED_CHANNEL_MASK) < result)) {
        result = 0;
    }
    return result;
}

u32 sdfQueryChannelBits(s32 index, s32 arg1, SdfPackedValue *packed) {
    u32 result;

    if (D_003BAA50[index * 0x38 + 0x24] != 2) {
        return 0;
    }
    result = func_001189A0(index, arg1, packed);
    if ((result & (packed->flagsAndValue & SDF_PACKED_CHANNEL_MASK)) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

void sdfSetPackedValuePreservingFlag(SdfPackedValue *item, u16 value) {
    item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_STATUS_BIT) | (value & SDF_PACKED_CHANNEL_MASK);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00119018);

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

