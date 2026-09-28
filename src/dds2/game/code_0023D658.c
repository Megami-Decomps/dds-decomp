#include "common.h"

extern u32 D_004371EC;

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
    u32 *unk8C;         /* 0x8C: flag word updated by the status opcodes */
    void *unk90;        /* 0x90 */
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

extern void *func_00110C70(void *arg0, s32 arg1, s32 arg2);

extern void *func_0010FFE8(void);

extern void func_0023D708(EvtUnit *work, s32 arg1, s128 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

extern void *func_0010FFA8(void);

extern void *func_00110C60(void *arg0);

extern s32 func_0010D818(s32 arg0);

extern s32 func_0010D650(s32 idx);

extern void func_00111B30(void *arg0, s32 arg1);

extern void func_00111B60(void *arg0, s32 arg1);

extern void func_00113660(void *arg0, s32 arg1);

extern void func_001136A0(void *arg0);

extern void *memset(void *dst, s32 c, u32 n);

extern void func_0010F908(void *arg0, void *arg1);

extern f32 func_0010D718(s32 idx);

extern void func_0010F968(void *arg0, void *arg1);

extern EvtUnit *func_0023CC00(s32 idx);

extern s32 func_0023CE30(EvtUnit *unit);

extern s32 func_002329F8(u32 *arg0, s32 arg1);

extern void func_00110B50(void *arg0);

extern void func_0023CF70(EvtUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void func_0035B6E0();

extern u8 D_004219F0[];

extern u8 D_00421AC0[];

extern s32 func_0010D7D0(s32 idx);

extern void func_00115DF0(void *arg0, s32 arg1);

extern void *func_00115208(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421A90[];

extern void *func_00115518(s32 arg0, void *arg1, void *arg2);

extern u8 D_00421B20[];

extern void *func_00115AC0(s32 arg0, s32 arg1);

extern u8 D_00421B70[];

extern s32 func_0023A7A0(s32 arg0, s32 arg1);

extern void func_0023CB58(EvtUnit *unit);

extern void func_0023CB68(EvtUnit *unit, s32 arg1);

extern u8 func_0023CB48(EvtUnit *unit);

extern void func_0023C750(EvtUnit *unit, void *arg1, s32 arg2);

extern void func_0023C7C0(EvtUnit *unit, s32 arg1);

extern void func_0010AE38(const char *fmt, ...);

extern void func_00115580(void *arg0, u32 arg1);

extern char D_00421AF8[];

extern void func_00115BD8(void *arg0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023D658);

void func_0023D698(EvtUnit *work, s32 arg1, s32 arg2) {
    void *unit;

    unit = func_00110C70(func_0010FFE8(), arg1, 0x11);
    if (unit != NULL) {
        func_0023D658(work, (s128 *)(*(u32 *)((u8 *)unit + 0x18) + 0x10), arg2);
        work->unk90 = unit;
    }
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023D708);

void func_0023D740(EvtUnit *work, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    void *unit;

    unit = func_00110C70(func_0010FFE8(), arg2, 0x11);
    if (unit != NULL) {
        func_0023D708(work, arg1, (s128 *)(*(u32 *)((u8 *)unit + 0x18)), arg3, arg4, arg5, arg6, arg7);
        work->unkAE = 1;
        work->unk90 = unit;
    }
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023D800);

s32 func_0023DA48(EvtUnit *work, s32 arg1) {
    s32 ret = 0;

    if (arg1 != 0) {
        work->unk94 = arg1;
        work->unkB2 = 0;
        work->unkAC = 4;
        ret = 1;
    }
    return ret;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023DA70);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E178);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E220);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E320);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E350);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E460);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E648);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E6D8);

u32 func_0023E718(void) {
    void *ctx;
    EvtUnit *obj;
    s32 value;

    ctx = func_0010FFA8();
    obj = func_00110C60(ctx);
    value = -1;
    if (obj != NULL) {
        value = obj->unk04;
    }
    func_0010D818(value);
    return 1;
}

u32 func_0023E758(void) {
    s32 param0;
    s32 param1;
    s32 value;

    param0 = func_0010D650(0);
    param1 = func_0010D650(1);
    value = func_0023A7A0(param0, param1);
    func_0010D818(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023E7A0);

u32 func_0023E898(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0023A8C0(temp_v0, temp_v1);
    return 1;
}

u32 func_0023E8D8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00111B30(unit, 0x400);
    func_00111B60(unit, 0x200);
    return 1;
}

u32 func_0023E948(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00111B60(unit, 0x400);
    func_00111B30(unit, 0x200);
    return 1;
}

u32 func_0023E9B8(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00111B60(unit, 0x400);
    func_00111B60(unit, 0x200);
    return 1;
}

u32 func_0023EA28(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00113660(unit, func_0010D650(1));
    return 1;
}

u32 func_0023EA90(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_001136A0(unit);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EAE8);

u32 func_0023EBE8(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = func_0010D718(1);
    v[1] = func_0010D718(2);
    v[2] = func_0010D718(3);
    func_0010F908(unit, v);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EC80);

u32 func_0023ED68(void) {
    f32 v[4];
    void *ctx;
    s32 id;
    void *unit;

    memset(v, 0, 0x10);
    v[3] = 1.0f;
    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return 1;
    }
    v[0] = func_0010D718(1);
    v[1] = func_0010D718(2);
    v[2] = func_0010D718(3);
    func_0010F968(unit, v);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EE08);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EEA0);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023EF40);

void func_0023EFF8(void) {
    func_0023CEA8(D_004371EC);
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421810);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F010);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F168);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F270);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F308);

u32 func_0023F3A8(void) {
    s32 id;
    EvtUnit *unit;
    u32 ret = 1;

    id = func_0010D650(0);
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

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return ret;
    }
    off = func_0010D650(1);
    if (((((u8 *)(off + (s32)unit))[0xE0] & 1) & 0xFF) == 0) {
        return ret;
    }
    return func_002329F8(unit->unk8C, func_0010D650(1)) != 0;
}

u32 func_0023F460(void) {
    void *ctx;
    s32 id;
    void *unit;
    u32 ret = 1;

    ctx = func_0010FFA8();
    id = func_0010D650(0);
    unit = func_00110C70(ctx, id, 5);
    if (unit == NULL) {
        return ret;
    }
    func_00110B50(unit);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F4B8);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F580);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F650);

u32 func_0023F6E0(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBC = func_0010D650(1);
    return 1;
}

u32 func_0023F730(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkBE = func_0010D650(1);
    unit->unkC0 = func_0010D650(2);
    return 1;
}

u32 func_0023F788(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    func_0023CF70(unit, func_0010D650(1), 5, 7, 1);
    func_0035B6E0(D_004219F0);
    return 1;
}

u32 func_0023F7F8(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;
    s32 param2;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit == NULL) {
        return 1;
    }
    unit->unkB2 = 0;
    param1 = func_0010D650(1);
    param2 = func_0010D650(2);
    func_0023D698(unit, param1, param2);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F868);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023F938);

INCLUDE_ASM(const s32, "game/code_0023D658", func_0023FBA8);

u32 func_0023FD38(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->unk8C &= ~1;
    }
    return 1;
}

u32 func_0023FD80(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    if (unit != NULL) {
        *unit->unk8C |= 1;
    }
    return 1;
}

u32 func_0023FDC0(void) {
    s32 id;
    EvtUnit *unit;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    func_0023CB58(unit);
    return 1;
}

u32 func_0023FDF0(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    param1 = func_0010D650(1);
    func_0023CB68(unit, param1);
    return 1;
}

u8 func_0023FE38(void) {
    s32 id;
    EvtUnit *unit;
    u8 active;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    active = func_0023CB48(unit);
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

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    target = func_0023E6D8(9, func_0010D650(2));
    if (target == NULL) {
        return 1;
    }
    func_0023C750(unit, target, func_0010D650(1));
    return 1;
}

u32 func_00240238(void) {
    s32 id;
    EvtUnit *unit;
    s32 param1;

    id = func_0010D650(0);
    unit = func_0023CC00(id);
    param1 = func_0010D650(1);
    func_0023C7C0(unit, param1);
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
    param0 = func_0010D7D0(0);
    unit = func_00115208(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, func_0010D7D0(0));
        func_0010D818(0);
    } else {
        func_00115DF0(unit, 1);
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
    param0 = func_0010D7D0(0);
    unit = func_00115518(param0, buf1, buf2);
    if (unit == NULL) {
        func_0035B6E0(D_00421A90, 1);
        func_0035B6E0(D_00421AC0, func_0010D7D0(0));
        func_0010D818(0);
    } else {
        func_00115DF0(unit, 1);
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

u32 func_002405C0(void) {
    s32 id;
    void *unit;
    u32 param1;

    id = func_0010D650(0);
    unit = func_0023E6D8(7, id);
    if (unit != NULL) {
        param1 = func_0010D650(1);
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

INCLUDE_ASM(const s32, "game/code_0023D658", func_002406C0);

u32 func_00240770(void) {
    s32 param0;
    EvtUnit *unit;

    param0 = func_0010D7D0(0);
    unit = func_00115AC0(1, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B20, 1);
        func_0035B6E0(D_00421AC0, func_0010D7D0(0));
        func_0010D818(0);
    } else {
        func_00115DF0(unit, 1);
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

    param0 = func_0010D7D0(0);
    unit = func_00115AC0(2, param0);
    if (unit == NULL) {
        func_0035B6E0(D_00421B70, 1);
        func_0035B6E0(D_00421AC0, func_0010D7D0(0));
        func_0010D818(0);
    } else {
        func_00115DF0(unit, 1);
        func_0010D818(unit->unk04);
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0023D658", D_00421B70);

INCLUDE_ASM(const s32, "game/code_0023D658", func_00240938);

u32 func_002409E0(void) {
    s32 id;
    void *obj;

    id = func_0010D650(0);
    obj = func_0023E6D8(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A20(void) {
    s32 id;
    void *obj;

    id = func_0010D650(0);
    obj = func_0023E6D8(7, id);
    if (obj != NULL) {
        func_00115BD8(obj);
    }
    return 1;
}

u32 func_00240A60(void) {
    s32 id;
    void *obj;

    id = func_0010D650(0);
    obj = func_0023E6D8(7, id);
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

