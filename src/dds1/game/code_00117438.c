#include "common.h"

extern s64 func_0011B938(void);
extern void func_0011B940(void);

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
    u8 pad00[0x34];
    u32 firstTick;
    u32 secondTick;
    u8 pad3C[0xA20];
    u32 updateMode;
} SdfRuntime;

typedef struct SdfPackedValue {
    u8 pad00[0xE];
    u16 flagsAndValue;
} SdfPackedValue;

extern u32 func_001189A0(s32 index, s32 arg1, SdfPackedValue *packed);
extern char D_003BA9E0[];
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern s32 func_002D0A60(s32 arg0);
extern s32 func_002D0918(s32 arg0);
void func_00117808(void);
s32 sdfBumpTickCounters(void);
void func_001177A8(void);
extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *resource);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

s32 func_001184A8(u32 arg0, u32 arg1, u32 arg2, u8 arg3);
INCLUDE_ASM(const s32, "game/code_00117438", func_00117438);

INCLUDE_ASM(const s32, "game/code_00117438", func_001174C0);

void func_00117568(u32 *value, u32 newValue) {
    *value = newValue;
}

u32 func_00117570(u32 *value) {
    return *value;
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
void func_001176A0(void) {
    void *mem = func_002D03F8(0x33600);
    u8 *state = (u8 *)sdfResourceRetainAddress(mem);

    memset(state, 0, 0x33600);
    *(s32 *)(state + 0x30) = (s32)mem;
    *(s32 *)(state + 0x34) = 0;
    *(s32 *)(state + 0x38) = 0;
    kwlnTaskCreate(D_003BA9E0, 1, 0, 0, (void *)sdfBumpTickCounters, 0, state);
    D_003BAA00 = (s32)state;
    func_001177A8();
}

/* Tear down the "GBWK" task hierarchy, clear the backing scene allocation
 * owned by the game state, and release the global state handle. */
void func_00117730(void) {
    s32 handle;

    kwlnTaskDestroyWithHierarchyByName(D_003BA9E0, 0);
    func_00117808();
    handle = *(s32 *)(D_003BAA00 + 0x30);
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
    func_0021F4B8();
    ((SdfRuntime *)D_003BAA00)->updateMode = 8;
    ptyInitRuntime();
    func_00120C08(0);
    func_002CC7D8();
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
    func_0011B940();
}

u8 func_00118140(s64 expectedValue) {
    s64 currentValue;

    currentValue = func_0011B938();
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

/* EnemyData base table has a 76-byte stride; fields 0x08/0x0C are the base
 * max HP / max MP, and unit offsets 0x1C/0x1E are the matching growth values. */
s32 func_00118368(s32 unit) {
    s32 result;

    if ((*(u16 *)unit & 0x20) != 0) {
        return *(u16 *)(D_003BAA1C + *(u16 *)(unit + 4) * 76 + 8);
    }
    result = evtRunContext(1, unit, 0, 0, 0);
    if ((*(u16 *)unit & 0x20) == 0) {
        result += *(u16 *)(unit + 0x1C);
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

s32 func_00118408(s32 unit) {
    s32 result;

    if ((*(u16 *)unit & 0x20) != 0) {
        return *(u16 *)(D_003BAA1C + *(u16 *)(unit + 4) * 76 + 0xC);
    }
    result = evtRunContext(2, unit, 0, 0, 0);
    if ((*(u16 *)unit & 0x20) == 0) {
        result += *(u16 *)(unit + 0x1E);
        if (result >= 1000) {
            result = 999;
        }
    }
    return result;
}

/* Sdf dispatch: pick the script id from the unit's runtime flag byte and run
 * it against the given entry index. 0x40/0x80 select the alternate scripts. */
s32 func_001184A8(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    s32 result;
    u8 flags;

    if (*(s8 *)(D_003BAA4C + arg0 * 2 + 1) == 5) {
        result = evtRunContext(0x18, arg1, arg2, arg0, arg3);
    } else {
        flags = D_003BAA50[arg0 * 0x38];
        if (flags & 0x40) {
            result = evtRunContext(0x1B, arg1, arg2, arg0, arg3);
        } else if (flags & 0x80) {
            result = evtRunContext(0x1C, arg1, arg2, arg0, arg3);
        } else {
            result = evtRunContext(5, arg1, arg2, arg0, arg3);
        }
    }
    return result;
}

void sdfDispatchCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_001184A8(arg0, arg1, arg2, (u8)arg3);
}

s32 func_00118570(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    s32 result;
    u8 flags;

    if (*(s8 *)(D_003BAA4C + arg0 * 2 + 1) == 5) {
        result = evtRunContext(0x18, arg1, arg2, arg0, arg3);
    } else {
        flags = D_003BAA50[arg0 * 0x38];
        if (flags & 0x40) {
            result = evtRunContext(0x1B, arg1, arg2, arg0, arg3);
        } else if (flags & 0x80) {
            result = evtRunContext(0x1C, arg1, arg2, arg0, arg3);
        } else {
            result = evtRunContext(9, arg1, arg2, arg0, arg3);
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

INCLUDE_ASM(const s32, "game/code_00117438", func_00118D70);

u32 sdfQueryChannelBits(s32 index, s32 arg1, SdfPackedValue *packed) {
    u32 result;

    if (D_003BAA50[index * 0x38 + 0x24] != 2) {
        return 0;
    }
    result = func_001189A0(index, arg1, packed);
    if ((result & (packed->flagsAndValue & 0x7FFF)) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

void sdfSetPackedValuePreservingFlag(SdfPackedValue *item, u16 value) {
    item->flagsAndValue = (item->flagsAndValue & 0x8000) | (value & 0x7fff);
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

