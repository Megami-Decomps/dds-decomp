#include "common.h"

/* Event unit/work object shared by the setup helpers below and the
 * script opcodes. Field layout matches event/evtUnitManager's EvtUnit
 * where they overlap (unk6C, flags, unkBC). */
typedef struct EvtUnit {
    u8 pad00[0x04];     /* 0x00 */
    s32 unk04;          /* 0x04 */
    u8 pad08[0x64];     /* 0x08 */
    u32 unk6C;          /* 0x6C */
    s128 unk70;          /* 0x70: 16-byte vector copied by the setup helpers */
    u8 pad80[0x0C];     /* 0x80 */
    u32 *flagWord;       /* 0x8C: status opcodes update its first bit */
    void *linkedUnit;    /* 0x90: world unit attached by setup helpers */
    s32 unk94;          /* 0x94 */
    s32 unk98;          /* 0x98 */
    s32 unk9C;          /* 0x9C */
    s32 unkA0;          /* 0xA0 */
    f32 unkA4;          /* 0xA4 */
    u32 flags;          /* 0xA8 */
    s16 unkAC;          /* 0xAC */
    s16 unkAE;          /* 0xAE */
    s16 unkB0;          /* 0xB0 */
    s16 unkB2;          /* 0xB2 */
    s16 unkB4;          /* 0xB4 */
    s16 unkB6;          /* 0xB6 */
    u8 padB8[0x04];     /* 0xB8 */
    u16 unkBC;          /* 0xBC */
    s16 unkBE;          /* 0xBE */
    s16 unkC0;          /* 0xC0 */
    u8 padC2[0x2E];     /* 0xC2 */
    s16 unkF0[1];       /* 0xF0 */
} EvtUnit;

typedef struct {
    s32 unk00;          /* 0x00 */
    s32 unk04;          /* 0x04 */
    f32 unk08;          /* 0x08 */
    f32 unk0C;          /* 0x0C */
    f32 unk10;          /* 0x10 */
    f32 unk14;          /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    f32 unk20;          /* 0x20 */
    f32 unk24;          /* 0x24 */
    f32 unk28;          /* 0x28 */
    f32 unk2C;          /* 0x2C */
    f32 unk30[4];       /* 0x30 */
} EvtSlot;

extern EvtSlot D_003D7BD8[7];

typedef struct {
    u8 pad00[0x10];     /* 0x00 */
    f32 unk10;          /* 0x10 */
    u8 pad14[0x04];     /* 0x14 */
    f32 unk18;          /* 0x18 */
    f32 unk1C;          /* 0x1C */
    u8 pad20[0x250];    /* 0x20 */
} Entry270;

extern Entry270 *D_003BAA20;

extern void *dds3GetWorldObject(void);
extern void effObjSetInnerThirdVec(void *arg0, void *arg1);

extern u8 evtTestUnitStatusFlags(EvtUnit *unit);

extern EvtUnit *func_00222090(s32 idx);

extern u32 D_003BBDAC;

extern s32 func_0010D428(s32 idx);
extern s32 func_0021FC30(s32 arg0, s32 arg1);


extern void *func_00110A48(void *arg0, s32 arg1, s32 arg2);
extern void *func_00110A38(void *arg0);
extern s32 func_00222298(EvtUnit *unit);
extern void func_00115970(void *arg0);
extern void func_00110928(void *arg0);
extern void *dds3GetWorldSecondaryObject(void);
extern void func_00222B70(EvtUnit *work, s32 arg1, s128 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

extern void dds3SetObjectFlags(void *arg0, s32 arg1);
extern void dds3ClearObjectFlags(void *arg0, s32 arg1);
extern void func_00113478(void *arg0);
extern void func_00113438(void *arg0, s32 arg1);
extern s32 func_0010D6A0(void);
extern void func_0010AC10(const char *fmt, ...);
extern s32 func_00241E18(s32 arg0, s32 arg1);
extern s32 func_00242298(s32 arg0, s32 arg1, s32 arg2);
extern s32 evtFindTaskById(s32 arg0);
extern void func_00101A80(s32 arg0, s32 arg1);
extern void func_002223D8(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_00222340(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_003003F0();
extern u8 D_003AC480[];
extern void func_00221BE0(EvtUnit *unit, void *arg1, s32 arg2);
extern s32 mdlCheckNodeByte30(u32 *arg0, s32 arg1);
extern void *memset(void *dst, s32 c, u32 n);
extern void func_00115318(void *arg0, u32 arg1);
extern void effObjSetInnerFirstVec(void *arg0, void *arg1);
extern f32 func_0010D4F0(s32 idx);
extern char D_003AC588[];
extern u8 D_003AC2A0[];
extern u8 D_003AC550[];
extern u8 D_003AC5B0[];
extern u8 D_003AC600[];
extern s32 scrGetWindow(void);
extern void func_0019CB98(s32 arg0, void (*arg1)(void));
extern void func_00222300(EvtUnit *unit, s32 arg1, s32 arg2);
extern s32 func_0010D5A8(s32 idx);
extern void *func_00115858(s32 arg0, s32 arg1);
extern void effObjSetFlags(void *arg0, s32 arg1);
extern void *func_00114FA0(s32 arg0, void *arg1, void *arg2);
extern u8 D_003AC520[];
extern void *func_001152B0(s32 arg0, void *arg1, void *arg2);
extern s32 func_0010D5F0(s32 arg0);
extern void func_0021FD50(s32 arg0, s32 arg1);
extern void evtSetUnitStatusFlags(EvtUnit *unit);
extern void func_00221FF8(EvtUnit *unit, s32 arg1);
extern void func_00221C50(EvtUnit *unit, s32 arg1);
extern void func_00222310(u32 arg0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222AC0);

void func_00222B00(EvtUnit *work, s32 objectId, s32 arg2) {
    void *worldUnit;

    worldUnit = func_00110A48(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        func_00222AC0(work, (s128 *)(*(u32 *)((u8 *)worldUnit + 0x18) + 0x10), arg2);
        work->linkedUnit = worldUnit;
    }
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222B70);

void func_00222BA8(EvtUnit *work, s32 arg1, s32 objectId, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    void *worldUnit;

    worldUnit = func_00110A48(dds3GetWorldSecondaryObject(), objectId, 0x11);
    if (worldUnit != NULL) {
        func_00222B70(work, arg1, (s128 *)(*(u32 *)((u8 *)worldUnit + 0x18)), arg3, arg4, arg5, arg6, arg7);
        work->unkAE = 1;
        work->linkedUnit = worldUnit;
    }
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222C68);

s32 func_00222EB0(EvtUnit *work, s32 arg1) {
    s32 ret = 0;

    if (arg1 != 0) {
        work->unk94 = arg1;
        work->unkB2 = 0;
        work->unkAC = 4;
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00222ED8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223540);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002235E8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002236E8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223718);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223828);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223A10);

void *func_00223AA0(s32 type, s32 id) {
    void *ctx;

    ctx = dds3GetWorldObject();
    func_00110A48(ctx, id, type);
}

u32 func_00223AE0(void) {
    void *ctx;
    EvtUnit *obj;
    s32 value;

    ctx = dds3GetWorldObject();
    obj = func_00110A38(ctx);
    value = -1;
    if (obj != NULL) {
        value = obj->unk04;
    }
    func_0010D5F0(value);
    return 1;
}

u32 func_00223B20(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = func_0010D428(0);
    param1 = func_0010D428(1);
    value = func_0021FC30(param0, param1);
    func_0010D5F0(value);
    return 1;
}

u32 func_00223B68(void) {
    s32 param0;
    s32 rid;
    s32 model;
    s32 ret;

    if (func_0010D6A0() == 0) {
        return 1;
    }
    param0 = func_0010D428(0);
    rid = func_0010D428(1);
    model = func_00241E18(param0, rid);
    if (model < 0) {
        func_0010AC10("MODEL_BE not fount RID = %d!\n", func_0010D428(1));
        return 1;
    }
    param0 = func_0010D428(0);
    rid = func_0010D428(1);
    ret = func_00242298(model, param0, rid);
    if (ret != 0) {
        func_00101A80(evtFindTaskById(func_0010D428(0)), ret);
    }
    return func_0010D5F0(model);
}

u32 func_00223C60(void) {
    s32 param0;
    s32 param1;

    param0 = func_0010D428(0);
    param1 = func_0010D428(1);
    func_0021FD50(param0, param1);
    return 1;
}

u32 func_00223CA0(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3SetObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223D10(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3SetObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223D80(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    dds3ClearObjectFlags(unit, 0x400);
    dds3ClearObjectFlags(unit, 0x200);
    return 1;
}

u32 func_00223DF0(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113438(unit, func_0010D428(1));
    return 1;
}

u32 func_00223E58(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113478(unit);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00223EB0);

u32 func_00223FB0(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = func_0010D4F0(1);
    v[1] = func_0010D4F0(2);
    v[2] = func_0010D4F0(3);
    effObjSetInnerFirstVec(unit, v);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224048);

u32 func_00224130(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = func_0010D4F0(1);
    v[1] = func_0010D4F0(2);
    v[2] = func_0010D4F0(3);
    effObjSetInnerThirdVec(unit, v);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002241D0);

u32 func_00224268(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = func_0010D428(1);
        s32 arg2 = func_0010D428(2);
        s32 arg3 = func_0010D428(3);
        s32 arg4 = func_0010D428(4);
        func_002223D8(unit, arg1, arg2, arg3, arg4);
    }
    return 1;
}

extern void func_00222340(EvtUnit *, s32, s32, s32, s32, s32);

u32 func_00224308(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = func_0010D428(1);
        s32 arg2 = func_0010D428(2);
        s32 arg3 = func_0010D428(3);
        s32 arg4 = func_0010D428(4);
        s32 arg5 = func_0010D428(5);
        func_00222340(unit, arg1, arg2, arg3, arg4, arg5);
    }
    return 1;
}

void func_002243C0(void) {
    func_00222310(D_003BBDAC);
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC2A0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002243D8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224530);

u32 func_00224638(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = func_0010D428(1);
        s32 arg2 = func_0010D428(2);
        func_00222300(unit, arg1, arg2);
        D_003BBDAC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_0019CB98(window, func_002243C0);
    }
    return 1;
}

u32 func_002246D0(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 arg1 = func_0010D428(1);
        s32 arg2 = func_0010D428(2);
        func_00222300(unit, arg1, arg2);
        func_00222310((u32)unit);
        D_003BBDAC = (u32)unit;
    }
    {
        s32 window = scrGetWindow();
        if (window < 0) {
            return 1;
        }
        func_0019CB98(window, func_002243C0);
    }
    return 1;
}

u32 func_00224770(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return ret;
    }
    return func_00222298(unit) != 0;
}

u32 func_002247B0(void) {
    s32 id;
    EvtUnit *unit;
    s32 off;
    u32 ret = 1;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return ret;
    }
    off = func_0010D428(1);
    if (((((u8 *)(off + (s32)unit))[0xE0] & 1) & 0xFF) == 0) {
        return ret;
    }
    return mdlCheckNodeByte30(unit->flagWord, func_0010D428(1)) != 0;
}

u32 func_00224828(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = dds3GetWorldObject();
    id = func_0010D428(0);
    unit = func_00110A48(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00110928(unit);
    return 1;
}

extern void func_002227C8(EvtUnit *unit);

u32 func_00224880(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = func_0010D428(2);
        s32 arg2 = func_0010D428(1);
        s32 arg3 = func_0010D428(3);
        s32 arg4 = func_0010D428(4);
        func_00222BA8(unit, arg1, arg2, -1, arg3, arg4, 0, 0);
    }
    if (func_0010D428(2) == 1) {
        func_002227C8(unit);
    }
    return 1;
}

extern void func_00222C68(EvtUnit *, s32, s32, s32, s32, s32, s32);

u32 func_00224948(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    {
        s32 arg1 = func_0010D428(1);
        s32 arg2 = func_0010D428(5);
        s32 arg3 = func_0010D428(6);
        s32 arg4 = func_0010D428(2);
        s32 arg5 = func_0010D428(4);
        s32 arg6 = func_0010D428(3);
        func_00222C68(unit, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    return 1;
}

u32 func_00224A18(void) {
    EvtUnit *unit;

    unit = func_00222090(func_0010D428(0));
    if (unit == NULL) {
        return 1;
    }
    {
        s32 index = func_0010D428(1);
        s32 value = func_0010D428(2);
        unit->unkF0[index] = value;
    }
    return 1;
}

u32 func_00224A80(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBC = func_0010D428(1);
    return 1;
}

u32 func_00224AD0(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = func_0010D428(1);
    unit->unkC0 = func_0010D428(2);
    return 1;
}

u32 func_00224B28(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    func_002223D8(unit, func_0010D428(1), 5, 7, 1);
    func_003003F0(D_003AC480);
    return 1;
}

u32 func_00224B98(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;
    s32 param2;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    param1 = func_0010D428(1);
    param2 = func_0010D428(2);
    func_00222B00(unit, param1, param2);
    return 1;
}

f32 func_00224C08(f32 a, f32 b) {
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

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224CD8);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00224F48);

u32 evtUnitClearFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit != NULL) {
        *unit->flagWord &= ~1;
    }
    return 1;
}

u32 evtUnitSetFlagBit(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    if (unit != NULL) {
        *unit->flagWord |= 1;
    }
    return 1;
}

u32 func_00225160(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D428(0);
    unit = func_00222090(id);
    evtSetUnitStatusFlags(unit);
    return 1;
}

u32 func_00225190(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = func_0010D428(0);
    unit = func_00222090(id);
    param1 = func_0010D428(1);
    func_00221FF8(unit, param1);
    return 1;
}

u8 func_002251D8(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = func_0010D428(0);
    unit = func_00222090(id);
    active = evtTestUnitStatusFlags(unit);
    return active == 0;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225208);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC480);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225330);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225408);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002254C8);

u32 func_00225560(void) {
    s32 id;
    EvtUnit *unit;
    void *target;

    id = func_0010D428(0);
    unit = func_00222090(id);
    target = func_00223AA0(9, func_0010D428(2));
    if (target == NULL) {
        return 1;
    }
    func_00221BE0(unit, target, func_0010D428(1));
    return 1;
}

u32 func_002255D8(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = func_0010D428(0);
    unit = func_00222090(id);
    param1 = func_0010D428(1);
    func_00221C50(unit, param1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225620);

u32 func_00225708(void) {
    u8 buf1[16];
    u8 buf2[16];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    param0 = func_0010D5A8(0);
    unit = func_00114FA0(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, func_0010D5A8(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->unk04);
    }
    return 1;
}

u32 func_002257C0(void) {
    f32 buf1[4];
    f32 buf2[4];
    s32 param0;
    EvtUnit *unit;

    memset(buf1, 0, 0x10);
    memset(buf2, 0, 0x10);
    buf2[3] = 1.0f;
    param0 = func_0010D5A8(0);
    unit = func_001152B0(param0, buf1, buf2);
    if (unit == NULL) {
        func_003003F0(D_003AC520, 1);
        func_003003F0(D_003AC550, func_0010D5A8(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC508);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC520);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC550);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225880);

u32 func_00225958(void) {
    return 1;
}

u32 evtUnitCheckModelCut(void) {
    s32 id;
    void *unit;
    u32 param1;

    id = func_0010D428(0);
    unit = func_00223AA0(7, id);
    if (unit != NULL) {
        param1 = func_0010D428(1);
        if (param1 >= 3U) {
            func_0010AC10(D_003AC588, param1);
            return 1;
        } else {
            func_00115318(unit, param1);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_002259E0);

extern f32 func_002259E0(s32);
extern void func_00190308(void *, f32);

u32 func_00225A60(void) {
    void *unit;

    unit = func_00223AA0(7, func_0010D428(0));
    if (unit != NULL) {
        s32 mode = func_0010D428(1);
        s32 index;
        if ((u32)mode >= 3) {
            func_0010AC10(D_003AC588, mode);
            return 1;
        }
        func_00115318(unit, mode);
        index = func_0010D428(2);
        if (index >= 0) {
            f32 value = func_002259E0(index);
            void *target = *(void **)(*(u8 **)((u8 *)unit + 0x18) + 0x2C);
            if (target != NULL) {
                func_00190308(target, value);
            }
        }
    }
    return 1;
}

u32 func_00225B10(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = func_0010D5A8(0);
    unit = func_00115858(1, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC5B0, 1);
        func_003003F0(D_003AC550, func_0010D5A8(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC588);

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC5B0);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225BA0);

u32 func_00225C48(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = func_0010D5A8(0);
    unit = func_00115858(2, param0);
    if (unit == NULL) {
        func_003003F0(D_003AC600, 1);
        func_003003F0(D_003AC550, func_0010D5A8(0));
        func_0010D5F0(0);
    } else {
        effObjSetFlags(unit, 1);
        func_0010D5F0(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00222AC0", D_003AC600);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225CD8);

u32 func_00225D80(void) {
    s32 id;
    void *obj;

    id = func_0010D428(0);
    obj = func_00223AA0(7, id);
    if (obj != NULL) {
        func_00115970(obj);
    }
    return 1;
}

u32 func_00225DC0(void) {
    s32 id;
    void *obj;

    id = func_0010D428(0);
    obj = func_00223AA0(7, id);
    if (obj != NULL) {
        func_00115970(obj);
    }
    return 1;
}

u32 func_00225E00(void) {
    s32 id;
    void *obj;

    id = func_0010D428(0);
    obj = func_00223AA0(7, id);
    if (obj != NULL) {
        func_00110928(obj);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225E40);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225F08);

INCLUDE_ASM(const s32, "game/code_00222AC0", func_00225FE8);

INCLUDE_SDATA(const s32, "game/code_00222AC0", D_003BBDAC);

INCLUDE_SDATA(const s32, "game/code_00222AC0", D_003BBDB0);

