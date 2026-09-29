#include "common.h"
#include "pcp_vu0.h"

extern u32 D_004371EC;

/* Event work layout overlaps event/evtUnitManager's EventUnit at
 * value6C, flags, and valueBC. */
typedef struct EvtUnit {
    u8 pad00[0x04];     /* 0x00 */
    s32 unk04;          /* 0x04 */
    u8 pad08[0x64];     /* 0x08 */
    u32 value6C;        /* 0x6C */
    s128 vector;         /* 0x70: 16-byte vector copied by the setup helpers */
    u8 pad80[0x0C];     /* 0x80 */
    u32 *modelNode;      /* 0x8C: node flag word updated by script opcodes */
    void *sourceUnit;    /* 0x90: matching secondary-world unit */
    s32 unk94;          /* 0x94 */
    s32 unk98;          /* 0x98 */
    s32 unk9C;          /* 0x9C */
    s32 unkA0;          /* 0xA0 */
    f32 unkA4;          /* 0xA4 */
    u32 flags;          /* 0xA8 */
    s16 mode;            /* 0xAC */
    s16 unkAE;          /* 0xAE */
    s16 unkB0;          /* 0xB0 */
    s16 unkB2;          /* 0xB2 */
    s16 unkB4;          /* 0xB4 */
    s16 unkB6;          /* 0xB6 */
    u8 padB8[0x04];     /* 0xB8 */
    u16 valueBC;        /* 0xBC */
    s16 unkBE;          /* 0xBE */
    s16 unkC0;          /* 0xC0 */
    u8 padC2[0x2E];     /* 0xC2 */
    s16 unkF0[1];       /* 0xF0 */
} EvtUnit;

extern void *func_00110C70(void *arg0, s32 arg1, s32 arg2);

extern void *dds3GetWorldSecondaryObject(void);

extern void func_0023D708(EvtUnit *work, s32 arg1, s128 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

extern void *dds3GetWorldObject(void);

extern void *func_00110C60(void *arg0);

extern s32 func_0010D818(s32 arg0);

extern s32 scrReadIntParameter(s32 idx);

extern void dds3SetObjectFlags(void *arg0, s32 arg1);

extern void dds3ClearObjectFlags(void *arg0, s32 arg1);

extern void func_00113660(void *arg0, s32 arg1);

extern void func_001136A0(void *arg0);

extern void *memset(void *dst, s32 c, u32 n);

extern void effObjSetInnerFirstVec(void *arg0, void *arg1);

extern f32 bfWaitReadArgFloat(s32 idx);

extern void effObjSetInnerThirdVec(void *arg0, void *arg1);

extern EvtUnit *func_0023CC00(s32 idx);

extern s32 func_0023CE30(EvtUnit *unit);

extern s32 mdlCheckNodeByte30(u32 *arg0, s32 arg1);

extern void func_00110B50(void *arg0);

extern void func_0023CF70(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void func_0035B6E0();

extern u8 D_004219F0[];

extern u8 D_00421AC0[];

extern s32 scrReadStringParameter(s32 idx);

extern void effObjSetFlags(void *arg0, s32 arg1);

extern void *func_00115208(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421A90[];

extern void *func_00115518(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421B20[];

extern void *func_00115AC0(s32 arg0, s32 arg1);

extern u8 D_00421B70[];

extern s32 func_0023A7A0(s32 arg0, s32 arg1);

extern void evtUnitSetStateBits(EvtUnit *unit);

extern void func_0023CB68(EvtUnit *unit, s32 arg1);

extern u8 evtUnitHasStateBits(EvtUnit *unit);

extern void evtSetUnitValueTransition(EvtUnit *unit, void *arg1, s32 arg2);

extern void evtEndUnitValueTransition(EvtUnit *unit, s32 arg1);

extern void func_0010AE38(const char *fmt, ...);

extern void func_00115580(void *arg0, u32 arg1);

extern char D_00421AF8[];

extern void func_00115BD8(void *arg0);

extern s32 func_0010D8C8(void);

extern s32 func_0025D230(s32 arg0, s32 arg1);

extern s32 evtCreateMotionSeTask(s32 arg0, s32 arg1, s32 arg2);

extern s32 evtFindTaskById(s32 arg0);

extern void func_00101968(s32 arg0, s32 arg1);

extern void func_0023CED8(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

extern void func_0023CED8(EvtUnit *, s32, s32, s32, s32, s32);

extern s32 scrGetWindow(void);

extern void func_001A4BB8(s32 arg0, void (*arg1)(void));

extern void func_0023CE98(EvtUnit *unit, s32 arg1, s32 arg2);

extern void func_0023CEA8(u32 arg0);

extern void func_0023D360(EvtUnit *unit);

extern void func_0023D800(EvtUnit *, s32, s32, s32, s32, s32, s32);

extern f32 func_00240640(s32);

extern void func_00197F40(void *, f32);

/* Start a bounded vector transition; detach any previous secondary-world source. */
void func_0023D658(EvtUnit *eventUnit, s128 *sourceVector, s32 stepCount) {
    s128 *destination = &eventUnit->vector;

    if ((u32)(stepCount - 1) < 100) {
        eventUnit->sourceUnit = NULL;
        eventUnit->mode = 3;
        PCP_COPY_VECTOR(destination, sourceVector);
        eventUnit->unkB4 = stepCount;
        eventUnit->unkB6 = 0;
        eventUnit->unk94 = 0;
        eventUnit->unkB2 = 0;
    }
}

/* Track a secondary-world unit and copy the vector in its subobject at +0x10. */
void evtAttachSecondaryWorldUnit(EvtUnit *eventUnit, s32 objectId, s32 stepCount) {
    void *sourceUnit;

    sourceUnit = func_00110C70(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (sourceUnit != NULL) {
        func_0023D658(eventUnit, (s128 *)(*(u32 *)((u8 *)sourceUnit + 0x18) + 0x10), stepCount);
        eventUnit->sourceUnit = sourceUnit;
    }
}

/* Configure a mode-one vector transition, without retaining a world source. */
void func_0023D708(EvtUnit *eventUnit, s32 arg1, s128 *sourceVector, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    s128 *destination = &eventUnit->vector;

    eventUnit->unkB0 = arg1;
    eventUnit->mode = 1;
    eventUnit->unkAE = 0;
    eventUnit->sourceUnit = NULL;
    PCP_COPY_VECTOR(destination, sourceVector);
    eventUnit->unkB4 = arg4;
    eventUnit->unkB6 = arg5;
    eventUnit->unk94 = arg6;
    eventUnit->unkB2 = 0;
}

/* Configure the same transition from a secondary-world object's vector. */
void func_0023D740(EvtUnit *eventUnit, s32 arg1, s32 objectId, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    void *sourceUnit;

    sourceUnit = func_00110C70(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (sourceUnit != NULL) {
        func_0023D708(eventUnit, arg1, (s128 *)(*(u32 *)((u8 *)sourceUnit + 0x18)), arg3, arg4, arg5, arg6, arg7);
        eventUnit->unkAE = 1;
        eventUnit->sourceUnit = sourceUnit;
    }
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023D800);

s32 func_0023DA48(EvtUnit *eventUnit, s32 value) {
    s32 result = 0;

    if (value != 0) {
        eventUnit->unk94 = value;
        eventUnit->unkB2 = 0;
        eventUnit->mode = 4;
        result = 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023DA70);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E178);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E220);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E320);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E350);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E460);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E648);

void *evtFindWorldObjectByIdAndKind(s32 kind, s32 id) {
    void *world;

    world = dds3GetWorldObject();
    func_00110C70(world, id, kind);
}

u32 evtCommandGetSelectedUnitValue(void) {
    void *world;
    EvtUnit *object;
    s32 id;

    world = dds3GetWorldObject();
    object = func_00110C60(world);
    id = -1;
    if (object != NULL) {
        id = object->unk04;
    }
    func_0010D818(id);
    return 1;
}

u32 func_0023E758(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = scrReadIntParameter(0);
    param1 = scrReadIntParameter(1);
    value = func_0023A7A0(param0, param1);
    func_0010D818(value);
    return 1;
}

u32 func_0023E7A0(void) {
    s32 param0;
    s32 rid;
    s32 model;
    s32 ret;

    if (func_0010D8C8() == 0) {
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    model = func_0025D230(param0, rid);
    if (model < 0) {
        func_0010AE38("MODEL_BE not fount RID = %d!\n", scrReadIntParameter(1));
        return 1;
    }
    param0 = scrReadIntParameter(0);
    rid = scrReadIntParameter(1);
    ret = evtCreateMotionSeTask(model, param0, rid);
    if (ret != 0) {
        func_00101968(evtFindTaskById(scrReadIntParameter(0)), ret);
    }
    return func_0010D818(model);
}

u32 func_0023E898(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = scrReadIntParameter(0);
    temp_v1 = scrReadIntParameter(1);
    func_0023A8C0(temp_v0, temp_v1);
    return 1;
}

u32 func_0023E8D8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3SetObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023E948(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3SetObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023E9B8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_0023EA28(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113660(unit, scrReadIntParameter(1));
    return 1;
}

u32 func_0023EA90(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_001136A0(unit);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EAE8);

u32 evtSetWorldUnitFirstVector(void) {
    f32 vector[4];
    void *world;
    s32 id;
    void *unit;

    memset(vector, 0, 0x10);
    world = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(world, id, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    effObjSetInnerFirstVec(unit, vector);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EC80);

u32 evtSetWorldUnitThirdVector(void) {
    f32 vector[4];
    void *world;
    s32 id;
    void *unit;

    memset(vector, 0, 0x10);
    vector[3] = 1.0f;
    world = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(world, id, 5);
    if (unit == NULL) {
        return 1;
    }
    vector[0] = bfWaitReadArgFloat(1);
    vector[1] = bfWaitReadArgFloat(2);
    vector[2] = bfWaitReadArgFloat(3);
    effObjSetInnerThirdVec(unit, vector);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EE08);

u32 evtOpSetUnitParams5(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_0023CF70(unit, arg1, arg2, arg3, arg4);
    }
    return 1;
}

u32 evtOpSetUnitParams6(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        s32 arg5 = scrReadIntParameter(5);
        func_0023CED8(unit, arg1, arg2, arg3, arg4, arg5);
    }
    return 1;
}

void func_0023EFF8(void) {
    func_0023CEA8(D_004371EC);
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421810);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F010);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F168);

u32 evtOpBeginWindowCallback(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_0023CE98(unit, arg1, arg2);
        D_004371EC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_001A4BB8(window, func_0023EFF8);
    }
    return 1;
}

u32 func_0023F308(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(2);
        func_0023CE98(unit, arg1, arg2);
        func_0023CEA8((u32)unit);
        D_004371EC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_001A4BB8(window, func_0023EFF8);
    }
    return 1;
}

u32 func_0023F3A8(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return ret;
    }
    return func_0023CE30(unit) != 0;
}

u32 func_0023F3E8(void) {
    s32 id;
    EvtUnit *unit;
    s32 off;
    u32 ret = 1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return ret;
    }
    off = scrReadIntParameter(1);
    if (((((u8 *)(off + (s32)unit))[0xE0] & 1) & 0xFF) == 0) {
        return ret;
    }
    return mdlCheckNodeByte30(unit->modelNode, scrReadIntParameter(1)) != 0;
}

u32 func_0023F460(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = scrReadIntParameter(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00110B50(unit);
    return 1;
}

u32 func_0023F4B8(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = scrReadIntParameter(2);
        s32 arg2 = scrReadIntParameter(1);
        s32 arg3 = scrReadIntParameter(3);
        s32 arg4 = scrReadIntParameter(4);
        func_0023D740(unit, arg1, arg2, -1, arg3, arg4, 0, 0);
    }
    if (scrReadIntParameter(2) == 1) {
        func_0023D360(unit);
    }
    return 1;
}

u32 func_0023F580(void) {
    EvtUnit *unit;

    unit = func_0023CC00(scrReadIntParameter(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = scrReadIntParameter(1);
        s32 arg2 = scrReadIntParameter(5);
        s32 arg3 = scrReadIntParameter(6);
        s32 arg4 = scrReadIntParameter(2);
        s32 arg5 = scrReadIntParameter(4);
        s32 arg6 = scrReadIntParameter(3);
        func_0023D800(unit, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F650);

u32 evtCommandSetUnitValue(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->valueBC = scrReadIntParameter(1);
    return 1;
}

u32 func_0023F730(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = scrReadIntParameter(1);
    unit->unkC0 = scrReadIntParameter(2);
    return 1;
}

u32 func_0023F788(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    func_0023CF70(unit, scrReadIntParameter(1), 5, 7, 1);
    func_0035B6E0(D_004219F0);
    return 1;
}

u32 func_0023F7F8(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;
    s32 param2;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    param1 = scrReadIntParameter(1);
    param2 = scrReadIntParameter(2);
    evtAttachSecondaryWorldUnit(unit, param1, param2);
    return 1;
}

f32 evtGetShortestAngleDelta(f32 a, f32 b) {
    f32 diff;

    if (a < 0.0f || b < 0.0f) {
        a += 360.0f;
        b += 360.0f;
    }
    a = (s32)a % 360;
    b = (s32)b % 360;
    diff = a - b;
    if (diff > 180.0f || diff < -180.0f) {
        if (a < b) {
            a += 360.0f;
        } else {
            b += 360.0f;
        }
    }
    return b - a;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F938);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FBA8);

u32 evtUnitClearFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->modelNode &= ~1;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->modelNode |= 1;
    }
    return 1;
}

u32 func_0023FDC0(void) {
    s32 id;
    EvtUnit *unit;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    evtUnitSetStateBits(unit);
    return 1;
}

u32 func_0023FDF0(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    param1 = scrReadIntParameter(1);
    func_0023CB68(unit, param1);
    return 1;
}

u8 evtUnitHasNoStatusFlags(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    active = evtUnitHasStateBits(unit);
    return active == 0;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FE68);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_004219F0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FF90);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240068);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240128);

u32 func_002401C0(void) {
    s32 id;
    EvtUnit *unit;
    void *target;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    target = evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(2));
    if (target == NULL) {
        return 1;
    }
    evtSetUnitValueTransition(unit, target, scrReadIntParameter(1));
    return 1;
}

u32 func_00240238(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = scrReadIntParameter(0);
    unit = func_0023CC00(id);
    param1 = scrReadIntParameter(1);
    evtEndUnitValueTransition(unit, param1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240280);

u32 func_00240368(void) {
    u8 buf1[16];
    u8 buf2[16];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    param0 = scrReadStringParameter(0);
    unit = func_00115208(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

u32 func_00240420(void) {
    f32 buf1[4];
    f32 buf2[4];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = scrReadStringParameter(0);
    unit = func_00115518(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421A78);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421A90);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421AC0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_002404E0);

u32 func_002405B8(void) {
    return 1;
}

u32 evtUnitCheckModelCut(void) {
    s32 id;
    void *unit;
    u32 param1;

    id = scrReadIntParameter(0);
    unit = evtFindWorldObjectByIdAndKind(7, id);
    if (unit != NULL) {
        param1 = scrReadIntParameter(1);
        if (param1 >= 3U) {
            func_0010AE38(D_00421AF8, param1);
            return 1;
        } else {
            func_00115580(unit, param1);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240640);

u32 func_002406C0(void) {
    void *unit;

    unit = evtFindWorldObjectByIdAndKind(7, scrReadIntParameter(0));
    if (unit != NULL) {
        s32 mode = scrReadIntParameter(1);
        s32 index;
        if ((u32)mode >= 3) {
            func_0010AE38(D_00421AF8, mode);
            return 1;
        }
        func_00115580(unit, mode);
        index = scrReadIntParameter(2);
        if (index >= 0) {
            f32 value = func_00240640(index);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                func_00197F40(target, value);
            }
        }
    }
    return 1;
}

u32 func_00240770(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115AC0(1, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B20, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421AF8);

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421B20);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240800);

u32 func_002408A8(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = scrReadStringParameter(0);
    unit = func_00115AC0(2, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B70, 1);
        func_0035B6E0(D_00421AC0, scrReadStringParameter(0));
        func_0010D818(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421B70);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240938);

u32 func_002409E0(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A20(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A60(void) {
    s32 id;
    void *obj;

    id = scrReadIntParameter(0);
    obj = evtFindWorldObjectByIdAndKind(7, id);
    if (obj != NULL) {
        func_00110B50(obj);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240AA0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240B68);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240C48);

INCLUDE_SDATA(const s32, "game/code_0023D658", D_004371EC);

INCLUDE_SDATA(const s32, "game/code_0023D658", D_004371F0);

