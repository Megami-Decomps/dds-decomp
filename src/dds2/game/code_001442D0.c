#include "common.h"
#include "pcp_vu0.h"

#include "fpu.h"

/* DDS2 saves store the area flag table earlier than the DDS1 save layout. */
typedef struct FldAreaFlagsView {
    u8 pad00[0xFCD0];
    u64 areaFlags[13][64];
} FldAreaFlagsView;

extern s32 D_004363C4;

extern u32 D_00435F0C;

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void func_002433B0(void);

extern s32 D_004362C8;

extern u32 D_00436330[];

extern s32 func_001578C0(u32);

extern s32 D_00436320;

extern s32 D_00436214;

extern f32 sdfAtan2(f32, f32);

extern f32 D_0038BAC0[];

extern s32 D_003899D8[];

extern s32 D_00436228;

extern s32 D_0039A0B8[];

extern s32 D_0039A028[];

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_00343ED0(char *, void *, s32);

extern u32 D_0044F7F0[];

extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern u32 D_00436380;

extern u32 D_00436384;

extern u32 D_00436388;

extern u32 D_0043638C;

extern u32 D_00436390;

extern u32 D_00436394;

extern u32 D_00436398;

extern u32 D_0043639C;

extern s32 D_004363AC;

extern s32 D_004363B0;

extern s32 D_004363B4;

extern u32 D_00436370;

extern s32 D_00436374;

extern s32 D_00436368;

extern s32 D_0043636C;

extern u32 D_00436324;

extern u32 D_00436328;

extern s32 D_0043632C;

extern s32 D_00435DD0;

extern u32 D_0043623C;

extern u32 D_00436200;

extern u64 func_00101958(void);

extern u32 D_00436210;

extern u32 D_00436234;

extern u32 D_00436238;

extern u32 D_00436270;

extern s32 D_00436278;

extern u32 D_0043626C;

extern u32 D_00436364;

extern u32 D_00436354;

extern u32 D_0043635C;

extern s32 D_0043637C;

extern u64 fldFindRoomByTask(u32);

extern s32 func_00120858(void);

extern void func_0010D818(s32 value);

extern s32 func_00140780(s32 value);

extern s32 func_0013FA98(s32 param0, s32 param1);

extern s32 func_0013FFF8(s32 param0, s32 param1);

extern void func_00140238(void);

extern void func_00144EE0(void);

extern void func_00144F60(void);

extern void func_00144F08(void);

extern void func_00144F88(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldLoadArchive(s32 param);

extern void func_001453D0(s32 param0, s32 param1);

extern void func_00145400(s32 param0, s32 param1);

extern void func_0014EC00(s32 param0, s32 param1, s32 param2);

extern s32 fldGetTaskRecordValue(u32 key);

extern void func_00140A58(void *entry);

/* Work object queried by func_0014F408; +0xE4 holds the key for func_0013BEE8. */
typedef struct {
    u8 unk00[0xE4]; /* 0x00 */
    u32 key;        /* 0xE4 */
} EffCmdWork;

extern void func_0014BF98(s32 handle);

extern s32 func_0023CC00(s32 param);

extern char *func_0010D7D0(s32 idx);

extern s32 fldFindEffectByName(char *str);

extern s32 func_0014E620(void);

extern s32 sdfSoundIsCommandBusy(void);

extern void *func_00328D68(s32 size);

extern void func_00101950(s32 arg0, void *arg1);

extern s32 func_00144400(void);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_00436208[];

extern s32 D_0043621C;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} FldClear18; /* 0x18 bytes */

extern FldClear18 D_0044F730[];

extern s32 D_00389770[];

extern void func_00342580(s32 arg0);

extern s32 D_00389798[];

extern void func_00341C78();

extern void func_00129E50(u32 arg0, s32 arg1);

extern s32 func_00129D60(s32 arg0);

extern s32 D_00389780[];

extern s32 D_00436244;

extern s32 D_00436248;

extern s32 D_00389834[];

extern s32 D_00389838[];

extern s32 D_00436340;

extern f32 D_003A8E90[];

extern s32 D_00436344;

extern f32 D_003A8EA0[];

extern char D_00413C10[]; /* "fldTitle" */

extern s32 func_00101740(void *name);

extern char D_00413C80[]; /* "fldTitleMini" */

extern void func_001576A0(u32 arg0);

extern void func_001132F0(s32 arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 D_00451BE0[];

extern s32 D_00389884[];

extern u8 D_003A9EB0[];

extern void func_0026C538(void *arg0);

extern void func_0026C5B8(s32 arg0);

extern void func_00126000(void);

extern void *D_00451B94[];

extern s32 D_003897C0[];

extern u64 dds3GetWorldSecondaryObject(void);

extern u32 *func_001111A8(u64 world, const char *name);

extern void func_0035B6E0(const char *fmt, ...);

extern s32 D_003897C8[];

extern s32 D_00389988[];

extern s32 func_0010D8C8(void);

extern s32 fldGetActorStat0(s32);

extern s32 func_001421C0(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldGetRowValue(s32);

extern u32 func_00140750(void);

extern void func_00125F58(void);

extern s32 D_003A8E70[];

extern s32 D_003A8E80[];

extern s32 D_0043633C;

extern s32 D_00451B9C[];

extern s32 D_00389784[];

extern s32 func_00123238(s32, s32, s32, s32);

extern s32 func_00123338(s32, s32, s32, s32);

extern void func_0014ABA8(s32, s32, s32);

extern void func_0014AB38(s32, s32, s32);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 func_0010D650(s32);

extern s32 D_0038984C[];

extern s32 func_00140B80(void);

extern void func_00123DE8(s32, s32, s32, s32, s32, s32);

extern s32 effMiscRandMod(s32, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 D_003AA720[];

extern void func_0032E348(void);

extern void func_00149CE0(void);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern s32 func_0013F1B8(void);

typedef struct {
    s32 *resource;
    u8 pad[0x4C];
} FieldResourceSlot;

extern FieldResourceSlot D_0044FFB0[];

extern void func_00145550(s32);

extern void func_00145698(void);

extern char D_004363E8[];

extern s32 func_0035D600(char *, const char *);

extern void fldSetFadeTarget(s32, s32, s32);

extern void func_00135568(s32);

extern s32 D_00399FF0[];

extern s32 mdlFlagTest();

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_00389A70[32];

extern s32 func_00144B50(s32);

extern s32 func_00144B50(s32);

extern s32 D_0043622C;

extern s32 D_00436230;

extern s32 func_00342168(s32);

extern void func_003421E8(s32);

extern s32 D_00436274;

typedef struct {
    u8 pad0[0x10];
    s32 value;
    u8 pad14[8];
} FldItem; /* 0x1C bytes */

typedef struct {
    u8 pad0[4];
    FldItem *items;
    u32 count;
    u8 padC[8];
} FldSceneRecord; /* 0x14 bytes */

extern s32 fldFindRecordItem(s32 scene, u32 index);

extern s32 D_003A8E30[];

extern s32 D_003A8E40[];

extern s32 D_003A8E50[];

extern s32 D_003A8E30[4];

extern s32 D_003A8E40[4];

extern s32 D_003A8E50[4];

typedef struct {
    s32 unk0, unk4, unk8, unkC;
    u8 pad10[0x10];
    s32 *unk20;
    u8 pad24[8];
    s32 unk2C, unk30, unk34, unk38;
    s16 unk3C, unk3E;
    char name[0x10];
} FldTblEnt50; /* 0x50 bytes */

extern FldTblEnt50 D_0044FF90[];

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0x14];
    s32 unk24;
    u8 pad28[8];
} FldObj30; /* 0x30 bytes */

extern FldObj30 D_00450990[32];

extern s32 D_00436300;

extern s32 D_0043630C;

extern s32 func_00123590();

extern void dds3SetObjectFlags();

extern void func_001576D8(s32 handle);

extern void func_00157790(s32 handle, f32 *pos);

typedef struct FieldPair48 {
    f32 pos[4];
    f32 vel[4];
    s32 unk20;
    s32 active;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
} FieldPair48; /* 0x30 bytes */

extern FieldPair48 D_00450F90[];

extern u32 effMiscRand();

extern s16 D_00389874[];

typedef struct FldVec4 {
    f32 v[4];
} FldVec4;

extern void effObjSetInnerFirstVec();

extern s32 *dds3GetUnk0C();

extern void dds3ClearObjectFlags();

extern FldVec4 D_00413DE8;

extern s32 D_00451B90[];

extern void func_0014F5F0();

extern void func_0014F980();

extern s32 D_00451B90[];

extern s32 D_00451B90[];

extern s32 D_00451B90[];

extern s32 dds3GetWorldObject(void);

extern void *func_00110C70(s32, s32, s32);

extern s32 func_00125F38(void);

extern s32 func_00110BE0(s32, s32);

extern void func_00113110(s32, f32 *, f32 *);

extern void func_0012F770(void);

extern f32 D_0038BAD0[];

extern f32 D_0038BAE0[];

extern s32 D_004360B4;

extern s32 D_004360B8;

extern void func_00123138(s32, s32, s32, s32);

extern int strcmp(const char *, const char *);

extern char D_004363F0[]; /* "BARIA" */

extern char *D_004361D8;

extern void func_001235E8(s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001442D0);

extern u8 func_00127398(void);

extern u8 func_001275D0(void);

extern s32 func_002CE928(void);

extern s32 D_00435EE0;

s32 func_00144400(void) {
    if (func_00127398() != 0) {
        return 0;
    }
    if (func_001275D0() != 0) {
        return 0;
    }
    if (func_002CE928() != 0) {
        return 0;
    }
    if (fldTitleIsActive() != 0) {
        return 0;
    }
    if (D_00389884[0] == 1) {
        return 0;
    }
    if (D_00435EE0 != -0x3e7) {
        func_0014D838(0x80, 0, 0);
    } else {
        func_0014B0F8(0);
    }
    return 0;
}

void *func_00144498(s32 arg0) {
    s32 *temp_v0;

    temp_v0 = func_00328D68(0x10);
    temp_v0[0] = 0;
    temp_v0[1] = 0;
    temp_v0[2] = 0;
    temp_v0[3] = 0;
    func_00101950(arg0, temp_v0);
    return func_00144400;
}

void func_001444E8(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00328E48(temp_v0);
    D_00436200 = 0;
}

void fldEnsureTask(void) {
    if (D_00436200 == 0) {
        D_00436200 = kwlnTaskCreate(D_00436208, 0x2B0B, 1, 1, func_00144498, func_001444E8, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", fldDestroyTask);

void func_00144598(void) {
    FldClear18 *temp_v0 = D_0044F730;
    s32 temp_v1 = 7;

    do {
        temp_v1 -= 1;
        temp_v0->unk0 = 0;
        temp_v0->unk8 = 0;
        temp_v0->unk4 = 0;
        temp_v0 += 1;
    } while (temp_v1 >= 0);
    D_0043621C = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001445D0);

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_00389780[0];
    if (stage == 11) {
        stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
    }
    for (i = 0; i < D_0043621C; i++) {
        if (D_0044F730[i].unk0 & 1) {
            D_0044F730[i].unk0 &= ~1;
            func_00341C78(D_00399FF0[stage] + D_0044F730[i].unk4);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001447A0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004136C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144B50);

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_00389A70[i].id == id && (D_00389A70[i].flag == 0 || mdlFlagTest(D_00389A70[i].flag) != 0)) {
            return D_00389A70[i].data[idx];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144CB8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144DB0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144DF8);

void func_00144EE0(void) {
    func_00341C00(D_00436210);
    D_00436210 = 0;
}

void func_00144F08(void) {
    func_00342580(D_00436210);
    if (D_00389770[10] >= 0x80) {
        D_00389770[10] = 1;
    }
    D_00436210 = 0;
}

void func_00144F48(void) {
    func_00341CA8();
}

void func_00144F60(void) {
    func_00341C78(D_00436210);
    D_00389798[0] = 0;
}

void func_00144F88(s32 id) {
    if (D_00389770[4] < 0x32) {
        if (id >= 0x10) {
            if (id == 0x80) {
                sndSetSequenceVolumePan(0x6d0060, 0x7f, 0x3f);
            } else if (id == 0x81) {
                sndSetSequenceVolumePan(0x6d0061, 0x7f, 0x3f);
            } else if (id == 0x82) {
                sndSetSequenceVolumePan(0x6d0064, 0x7f, 0x3f);
            } else if (id >= 0x200) {
                sndSetSequenceVolumePan(0x68fe00 + id, 0x7f, 0x3f);
            } else {
                sndSetSequenceVolumePan(D_00436214 + id, 0x7f, 0x3f);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", fldPlayFieldSe);

void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_00389780[0] < 50) {
        sndSetSequenceVolumePan(D_00436214 + category, volume, 0x3F);
    }
}

void fldSelectBgmMode(s32 arg0) {
    s32 volume;
    if (D_003899D8[0] == 0) {
        return;
    }
    switch (arg0) {
    case -1:
        if (D_00436228 != arg0) {
            func_00341C78(D_00436228);
            D_00436228 = arg0;
        }
        break;
    case 0:
        if (D_00436228 != -1) {
            func_00342580(D_00436228);
            D_00436228 = -1;
        }
        break;
    case 1:
        volume = D_0039A0B8[0] + 1;
        func_00341BB8(volume);
        D_00436228 = volume;
        break;
    case 2:
        volume = D_0039A028[0] + 1;
        func_00341BB8(volume);
        D_00436228 = volume;
        break;
    }
}

void func_00145228(void) {
    s32 handle = func_00144B50(D_00389770[10]);
    s32 stage;
    s32 idx;

    if (handle == -1) {
        if (D_00389770[4] >= 0x32) {
            return;
        }
        stage = D_00389770[4];
        if (stage == 11) {
            stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, D_00389770[5] + 1);
        D_00436214 = D_00399FF0[stage];
        D_00389770[10] = idx;
        handle = D_00436214 + idx;
    }
    D_0043622C = handle;
    D_00436230 = 0;
}

s32 func_001452D8(void) {
    switch (D_00436230) {
    case 0:
        if (func_00342168(D_0043622C) == 0) {
            func_003421E8(D_0043622C);
            D_00436230 = D_00436230 + 1;
        } else {
            D_00436230 = D_00436230 + 2;
        }
        break;
    case 1:
        if (func_00342168(D_0043622C) == 1) {
            D_00436230 = D_00436230 + 1;
        }
        break;
    default:
        D_00436230 = -1;
        return 1;
    }
    return 0;
}

u32 func_00145358(void) {
    return D_00436234;
}

s32 fldLoadArchive(s32 id) {
    s32 name = 0x30000000 + (id << 16);
    s32 result = func_00342168(name);
    if (result == 0) {
        func_003421E8(name);
        D_00436234 = 1;
        return 0;
    }
    if (result == 1) {
        D_00436234 = 0;
        return 1;
    }
    return 0;
}

void func_001453D0(s32 arg0, s32 arg1) {
    sndSetSequenceVolumePan(arg0 * 0x10000 + arg1 + 0x30000000, 0x7f, 0x3f);
}

void func_00145400(s32 arg0, s32 arg1) {
    func_00341C78(arg0 * 0x10000 + arg1 + 0x30000000);
}

void func_00145428(void) {
    D_00436238 = 0;
}

s32 func_00145430(void) {
    switch (D_00436238) {
    case 0:
        if (func_00342168(0x680000) == 0) {
            func_003421E8(0x680000);
            D_00436238 = D_00436238 + 1;
        } else {
            D_00436238 = D_00436238 + 2;
        }
        break;
    case 1:
        if (func_00342168(0x680000) == 1) {
            D_00436238 = D_00436238 + 1;
        }
        break;
    default:
        D_00436238 = -1;
        return 1;
    }
    return 0;
}

void func_001454B8(void) {
    D_0043623C = 0;
}

s32 func_001454C0(void) {
    switch (D_0043623C) {
    case 0:
        if (func_00342168(0x690000) == 0) {
            func_003421E8(0x690000);
            D_0043623C = D_0043623C + 1;
        } else {
            D_0043623C = D_0043623C + 2;
        }
        break;
    case 1:
        if (func_00342168(0x690000) == 1) {
            D_0043623C = D_0043623C + 1;
        }
        break;
    default:
        D_0043623C = -1;
        return 1;
    }
    return 0;
}

u32 func_00145548(void) {
    return D_00436210;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145550);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145698);

void func_00145730(s32 arg0, s32 arg1) {
    if (D_00389780[0] < 0xC8) {
        s32 temp_a0 = arg0;
        s32 temp_a1 = arg1;
        s32 temp_v0 = temp_a0 + 8;

        D_00436278 = temp_a1;
        func_00129E50(arg0, temp_v0);
        {
            s32 temp_v1 = func_00129D60(temp_v0);
            s32 temp_4 = *(s32 *)(temp_v1 + 4);
            s32 temp_8 = *(s32 *)(temp_v1 + 8);

            D_00436274 = temp_8;
            D_00436270 = temp_4;
        }
    }
}

void func_00145790(void) {
    if (D_00389780[0] < 200) {
        func_00145550(D_00389780[0] % 100);
        func_00145698();
    }
}

void func_001457E0(void) {
    if (D_00436278 != 0) {
        func_003298C0(D_00436278);
    }
    D_00436278 = 0;
    D_00436270 = 0;
    D_00436274 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145818);

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetEmitterPosition);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145948);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145D48);

s32 fldFindRecordItem(s32 scene, u32 index) {
    s32 result = 1;
    FldSceneRecord *rec = (FldSceneRecord *)D_00436270;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)D_00436274; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->value + 1;
            }
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00146150);

s32 fldGetMaxItemValue(void) {
    s32 max = 0;
    FldSceneRecord *rec = (FldSceneRecord *)D_00436270;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)D_00436274; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->value) {
                max = item->value;
            }
        }
    }
    return max + 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00146250);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148188);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148488);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148A98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149A00);

void fldReleaseResourceSlots(void) {
    if (D_0044F7F0[1] != 0) {
        func_0032BBB0(D_0044F7F0[1]);
    }
    if (D_0044F7F0[2] != 0) {
        func_0032BBB0(D_0044F7F0[2]);
    }
    if (D_0044F7F0[3] != 0) {
        func_0032BBB0(D_0044F7F0[3]);
    }
    if (D_0044F7F0[6] != 0) {
        func_0032BBB0(D_0044F7F0[6]);
    }
    D_0044F7F0[1] = 0;
    D_0044F7F0[2] = 0;
    D_0044F7F0[3] = 0;
    D_0044F7F0[6] = 0;
    while (func_0032CD98() != 0) {
        func_0032E348();
    }
    D_0043626C = 0;
    func_0019D1F8(0x54);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149CE0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149E08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149FE8);

void func_0014A228(void) {
    func_0032E348();
    func_0032E348();
    func_00149CE0();
}

u32 func_0014A250(void) {
    return D_0043626C;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A258);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A880);

void func_0014A9F0(s32 arg0, s32 arg1, s32 arg2) {
    D_00389770[4] = arg0;
    D_00389770[6] = arg2;
    D_00436248 = D_00389770[5] = arg1;
    D_00436244 = arg0 % 100;
}

extern s32 D_00387CE0[];

s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 index = D_00387CE0[area % 100];

    if (index == -1) {
        return 0;
    }
    return (((FldAreaFlagsView *)D_00435DD0)->areaFlags[index][floor] >> bit) & 1;
}

void fldSetCurrentFloorFlag(s32 bit) {
    s32 index;
    s32 floor;
    s32 bitIndex;

    if (bit <= 0) {
        return;
    }
    index = D_00387CE0[D_00436244 % 100];
    if (index == -1) {
        return;
    }
    floor = D_00389770[5];
    bitIndex = bit - 1;
    {
        s32 byteOffset = 0xFCD0 + (index * 64 + floor) * 8;
        u64 mask = (u64)1 << bitIndex;
        u64 *flags = (u64 *)(D_00435DD0 + byteOffset);
        D_00389770[6] = bitIndex;
        D_00389770[47] = bit;
        *flags |= mask;
    }
    D_00389770[48] = fldFindRecordItem(floor, bitIndex);
}

void func_0014AB38(s32 area, s32 floor, s32 bit) {
    s32 index = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (index != -1) {
        s32 byteOffset = 0xFCD0 + ((index * 64 + floor) * 8);
        u64 mask = (u64)1 << bit;
        u64 *flags = (u64 *)(D_00435DD0 + byteOffset);
        *flags |= mask;
    }
}

void func_0014ABA8(s32 area, s32 floor, s32 bit) {
    s32 index = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (index != -1) {
        s32 byteOffset = 0xFCD0 + ((index * 64 + floor) * 8);
        u64 mask = (u64)1 << bit;
        u64 *flags = (u64 *)(D_00435DD0 + byteOffset);
        *flags &= ~mask;
    }
}

void func_0014AC18(s32 arg0) {
    if (arg0 >= 0x40) {
        D_00389834[0] = -1;
    } else {
        D_00389834[0] = arg0;
    }
}

void func_0014AC40(s32 arg0) {
    if (arg0 >= 0x40) {
        D_00389838[0] = -1;
    } else {
        D_00389838[0] = arg0;
    }
}

s32 fldFindPreviousMarkedValue(s32 limit) {
    s32 group = D_00436270;
    s32 best = -1;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)D_00436274; groupIndex++, group += 0x14) {
        u32 entryCount = *(u32 *)(group + 8);
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(D_00436244, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldFindRecordItem(groupIndex, entryIndex);
                if (value < limit && value > best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = *(u32 *)(group + 8);
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldFindNextMarkedValue(s32 limit) {
    s32 group = D_00436270;
    s32 best = 999;
    s32 groupIndex = 0;

    for (groupIndex = 0; groupIndex < (s32)D_00436274; groupIndex++, group += 0x14) {
        u32 entryCount = *(u32 *)(group + 8);
        u32 entryIndex;

        for (entryIndex = 0; entryIndex < entryCount;) {
            s32 marked = fldGetFloorFlag(D_00436244, groupIndex, entryIndex);
            if (marked) {
                s32 value = fldFindRecordItem(groupIndex, entryIndex);
                if (value > limit && value < best) {
                    best = value;
                }
            }
            entryIndex++;
            entryCount = *(u32 *)(group + 8);
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}

void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z) {
    s32 i;
    s32 entry = D_00436270;
    for (i = 0; i < (s32)D_00436274; i++, entry += 0x14) {
        if (i == index) {
            f32 *position = *(f32 **)(entry + 0x10);
            *x = position[0];
            *z = position[2];
            return;
        }
    }
    *x = 0.0f;
    *z = 0.0f;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AE58);

INCLUDE_ASM(const s32, "game/code_001442D0", fldCheckSceneReady);

void func_0014B050(void) {
    if (D_0043626C == 1) {
        func_00148488();
        func_00146250();
    }
}

void func_0014B088(void) {
    func_0014A880();
    func_00148A98();
}

void func_0014B0A8(void) {
    u64 *puVar1;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = D_00435DD0;
    do {
        puVar1 = (u64 *)(temp_v1 + 0xfcd0);
        temp_v0 = 0x3f;
        do {
            temp_v0 = temp_v0 - 1;
            *puVar1 = 0;
            puVar1 = puVar1 + 1;
        } while (-1 < temp_v0);
        temp_v2 = temp_v2 + 1;
        temp_v1 = temp_v1 + 0x200;
    } while (temp_v2 < 10);
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B440);

void fldReleaseTextureSlots(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_003A8E50[i] != 0) {
            func_00157658(D_003A8E50[i]);
            D_003A8E50[i] = 0;
            func_003298C0(D_003A8E30[i]);
            D_003A8E30[i] = 0;
            D_003A8E40[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B5D8);

typedef struct FieldResourceIds {
    s32 entries[2];
} FieldResourceIds;

void fldLoadResourceByIndex(s32 index) {
    FieldResourceIds ids = *(FieldResourceIds *)D_00436330;
    D_00436324 = func_00343ED0(ids.entries[index], &D_00436328, 0);
    D_0043632C = func_001578C0(D_00436328);
}

void func_0014B748(void) {
    if (D_0043632C != 0) {
        func_00157658(D_0043632C);
        D_0043632C = 0;
        func_003298C0(D_00436324);
        D_00436324 = 0;
        D_00436328 = 0;
    }
}

typedef struct {
    u8 unk00[0x14C];
    f32 position[3];
} FieldPlacementState;

void func_0014B788(void) {
    f32 position[4];
    memset(position, 0, sizeof(position));
    position[3] = 1.0f;
    if (D_0043632C != 0) {
        FieldPlacementState *state = (FieldPlacementState *)D_00389770;
        position[0] = state->position[0];
        position[1] = state->position[1];
        position[2] = state->position[2];
        func_00157790(D_0043632C, position);
        func_001576A0(D_0043632C);
    }
}

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        D_003A8E70[i] = func_001579C8(D_0043633C);
        D_003A8E80[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B860);

void func_0014B940(f32 arg0, f32 arg1, f32 arg2) {
    D_00436340 = 1;
    D_003A8E90[0] = arg0;
    D_003A8E90[1] = arg1;
    D_003A8E90[2] = arg2;
}

void func_0014B960(f32 arg0, f32 arg1, f32 arg2) {
    D_00436344 = 1;
    D_003A8EA0[0] = arg0;
    D_003A8EA0[1] = arg1;
    D_003A8EA0[2] = arg2;
}

void mnuReleaseResourceEntries(void) {
    s32 i;
    D_00436340 = 0;
    D_00436344 = 0;
    for (i = 0; i < 4; i++) {
        if (D_003A8E70[i] != 0) {
            func_00157658(D_003A8E70[i]);
            D_003A8E70[i] = 0;
        }
    }
}

void func_0014B9E0(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (D_003A8E70[i] != 0 && D_003A8E80[i] != 0) {
            func_001576A0(D_003A8E70[i]);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BA60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BD20);

extern u32 D_004362DC;

extern u32 D_004362E8;

extern u32 D_004362F4;

extern u32 D_004362CC;

void func_0014BDE0(void) {
    if (D_00389770[0x1F0 / 4] != 0) {
        func_0032BBB0(D_00389770[0x1F0 / 4]);
        D_00389770[0x1F0 / 4] = 0;
    }
    if (D_00389770[0x1FC / 4] != 0) {
        func_0032BBB0(D_00389770[0x1FC / 4]);
        D_00389770[0x1FC / 4] = 0;
    }
    if (D_00389770[0x208 / 4] != 0) {
        func_0032BBB0(D_00389770[0x208 / 4]);
        D_00389770[0x208 / 4] = 0;
    }
    if (D_00389770[0x214 / 4] != 0) {
        func_0032BBB0(D_00389770[0x214 / 4]);
        D_00389770[0x214 / 4] = 0;
    }
    if (D_004362DC != 0) {
        func_00157658(D_004362DC);
        D_004362DC = 0;
    }
    if (D_004362E8 != 0) {
        func_00157658(D_004362E8);
        D_004362E8 = 0;
    }
    if (D_004362F4 != 0) {
        func_00157658(D_004362F4);
        D_004362F4 = 0;
    }
    if (D_004362CC != 0) {
        func_00157658(D_004362CC);
        D_004362CC = 0;
    }
}

void fldClearMenuEntries(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        D_0044FF90[i].unk0 = 0;
        D_0044FF90[i].unk4 = 0;
        D_0044FF90[i].unk8 = 0;
        D_0044FF90[i].unkC = 0;
        D_0044FF90[i].unk20 = 0;
        D_0044FF90[i].unk2C = 0;
        D_0044FF90[i].unk30 = 0;
        D_0044FF90[i].unk34 = 0;
        D_0044FF90[i].unk38 = 0;
        D_0044FF90[i].unk3C = 0;
        D_0044FF90[i].unk3E = 0;
    }
    D_004362C8 = 0;
}

void fldResetObjectSlots(void) {
    s32 i;

    D_00436320 = 0;
    for (i = 0; i < 32; i++) {
        D_00450990[i].unk4 = -1;
        D_00450990[i].unk0 = 0;
        D_00450990[i].unk8 = 0;
        D_00450990[i].unkC = 0;
        if (D_00450990[i].unk24 != 0) {
            func_00157658(D_00450990[i].unk24);
        }
        D_00450990[i].unk24 = 0;
    }
    D_00436300 = 0;
    D_0043630C = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BF98);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AF8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C2B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B38);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C9B0);

void fldActivateObjectById(s32 id) {
    s32 i;

    for (i = 0; i < D_00436320; i++) {
        if (D_00450990[i].unk4 == id && D_00450990[i].unk8 == 0) {
            D_00450990[i].unkC = 1;
        }
    }
}

void fldReleaseObjectSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_00450990[i].unk24 != 0) {
            func_00157658(D_00450990[i].unk24);
            D_00450990[i].unk24 = 0;
        }
    }
    D_00436320 = 0;
    for (i = 0; i < 32; i++) {
        D_00450990[i].unk4 = -1;
        D_00450990[i].unk0 = 0;
        D_00450990[i].unk8 = 0;
        D_00450990[i].unkC = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D0E8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D380);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D7B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D838);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DB50);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DDD8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DEA8);

void fldFireRoomEffects(void) {
    s32 i;

    for (i = 0; i < D_004362C8; i++) {
        s32 room = D_0044FF90[i].unk38;
        if (room != 0 && func_00123590(D_00389770[4], D_00389770[5] + 1, room) != 0) {
            if (D_0044FF90[i].unk20 != 0) {
                dds3SetObjectFlags(D_0044FF90[i].unk20, 1);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E3A8);

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetNpcPalette);

INCLUDE_ASM(const s32, "game/code_001442D0", fldFindEffectByName);

s32 func_0014E620(void) {
    s32 index = func_0013F1B8();
    if (index >= 0) {
        return D_0044FFB0[index].resource[1];
    }
    return 0;
}

void func_0014E668(u32 arg0) {
    D_00436364 = arg0;
}

s32 fldTitleIsActive(void) {
    return func_00101740(D_00413C10) != 0;
}

void func_0014E698(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E6A8);

void func_0014EBB8(void) {
    if (D_00436368 != 0) {
        func_0032BBB0(D_00436368);
        D_00436368 = 0;
    }
    if (D_0043636C != 0) {
        func_0032BBB0(D_0043636C);
        D_0043636C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C10);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EC00);

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive()) {
        kwlnTaskDestroyWithHierarchyByName(D_00413C10, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return func_00101740(D_00413C80) != 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EDB8);

void func_0014F138(void) {
    if (D_0043637C != 0) {
        func_0032BBB0(D_0043637C);
        D_0043637C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F168);

void func_0014F2F8(void) {
    if (D_00436374 == 0) {
        D_00436370 = 0;
        D_00436374 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F318);

void func_0014F408(void) {
    if (D_004363AC != 0) {
        func_0032BBB0(D_004363AC);
        D_004363AC = 0;
    }
    if (D_004363B0 != 0) {
        func_0032BBB0(D_004363B0);
        D_004363B0 = 0;
    }
    if (D_004363B4 != 0) {
        func_0032BBB0(D_004363B4);
        D_004363B4 = 0;
    }
    func_00157658(D_00436388);
    D_00436388 = 0;
    D_0043638C = 0;
    func_003298C0(D_00436380);
    D_00436380 = 0;
    D_00436384 = 0;
    func_00157658(D_00436398);
    D_00436398 = 0;
    D_0043639C = 0;
    func_003298C0(D_00436390);
    D_00436390 = 0;
    D_00436394 = 0;
}

void fldSetWeatherEffectPos(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (D_00436388 != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_001576D8(D_00436388);
        func_00157790(D_00436388, pos);
        D_0043638C = 1;
    }
    if (D_00436398 != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_001576D8(D_00436398);
        func_00157790(D_00436398, pos);
        D_0043639C = 1;
    }
}

void func_0014F560(void) {
    if (D_00436388 != 0 && D_0043638C != 0) {
        func_001576A0(D_00436388);
    }
    if (D_00436398 != 0 && D_0043639C != 0) {
        func_001576A0(D_00436398);
    }
}

typedef struct {
    s16 category;
    s16 id;
    u8 unk04[0xDC];
} FieldResourceRecord;

extern FieldResourceRecord *D_00435E18;

s32 func_0014F5B0(s32 category, s32 id) {
    FieldResourceRecord *record = D_00435E18;
    s32 index;
    for (index = 0; index < 8; index++, record++) {
        if (record->category == category && record->id == id) {
            return index;
        }
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DC8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5F0);

void func_0014F788(void) {
    FldEnt14 *entry = D_00451BE0;
    s32 i = 0x10;

    do {
        s32 temp = entry->unk0;

        i--;
        if (temp != 0) {
            func_001132F0(temp);
            entry->unk0 = 0;
        }
        entry++;
    } while (i >= 0);
}

void fldInitSparkTable(void) {
    s32 i;

    if (D_00389874[0] == 0) {
        for (i = 0; i < 64; i++) {
            D_00450F90[i].unk20 = 0;
            D_00450F90[i].active = 0;
            D_00450F90[i].unk28 = 0;
            D_00450F90[i].unk2C = -1;
            D_00450F90[i].unk2E = effMiscRand(0) % 60 + 15;
        }
    } else {
        for (i = 0; i < 64; i++) {
            D_00450F90[i].unk20 = 0;
            D_00450F90[i].unk2C = -1;
        }
    }
}

void fldResetSparkTable(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        D_00450F90[i].active = 0;
        D_00450F90[i].unk28 = 0;
        D_00450F90[i].unk2C = -1;
        D_00450F90[i].unk2E = effMiscRand(0) % 60 + 15;
    }
}

s32 fldSetSparkVectors(s32 index, const u128 *pos, const u128 *vel) {
    PCP_COPY_VECTOR(D_00450F90[index].pos, pos);
    PCP_COPY_VECTOR(D_00450F90[index].vel, vel);
    D_00450F90[index].unk20 = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F980);

void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_00413DE8;
    if (D_00450F90[index].unk20 == 1 && D_00450F90[index].active != 0 && D_00450F90[index].unk2C != -1) {
        vec.v[0] = D_00450F90[index].pos[0];
        vec.v[2] = D_00450F90[index].pos[2];
        effObjSetInnerFirstVec(D_00451BE0[D_00450F90[index].unk2C].unk0, &vec);
        obj = D_00451BE0[D_00450F90[index].unk2C].unk0;
        flags = dds3GetUnk0C(obj);
        *flags |= 1;
        dds3ClearObjectFlags(obj, 0x400);
        slot = D_00450F90[index].unk2C;
        D_00450F90[index].unk2C = -1;
        D_00451BE0[slot].unk4 = -1;
    }
}

void fldUpdateSparkSlots(void) {
    s32 i;

    func_0014F5F0();
    for (i = 0; i < 64 && i < D_00451B90[13]; i++) {
        if (D_00450F90[i].unk20 != 0 && D_00450F90[i].active != 0) {
            func_0014F980(i, D_00450F90[i].unk28);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014FD28);

s32 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < D_00451B90[13]; i++) {
        if (D_00450F90[i].active == 1 && D_00450F90[i].unk2C != -1) {
            f32 dx = x - D_00450F90[i].pos[0];
            f32 dy = y - D_00450F90[i].pos[1];
            f32 dz = z - D_00450F90[i].pos[2];

            if (fsqrtf(dx * dx + dy * dy + dz * dz) < 50.0f) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150138);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150800);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001509E0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DF8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E48);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150A60);

extern s32 D_00451BBC[];

s32 func_00150F10(void) {
    return D_00451BBC[0];
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150F20);

extern s32 D_00451B98[];

void func_001512D8(void) {
    D_00451B98[0] = -1;
}

extern s32 D_003899E0[];

extern void func_00130F98(s32);

void func_001512E8(void) {
    func_00130F98(0);
    D_003899E0[0] = 0;
    D_00389770[0x118 / 4] = 0;
    func_0014F788();
    func_0014F408();
    func_00125F58();
    D_00389770[0x114 / 4] = 1;
    *(s16 *)((u8 *)D_00389770 + 0x104) = 0;
    D_00451B9C[0] = 0;
    D_00389770[0x138 / 4] = 1;
}

void fldResetAfterEvent(void) {
    func_00341C78(0x680017);
    mnuAdvanceTitleStateUnderSemaphore();
    D_003899E0[0] = 0;
    D_00389770[0x118 / 4] = 0;
    func_0014F788();
    func_0014F408();
    func_00144DF8();
    D_00389770[0x114 / 4] = 0;
    *(s16 *)((u8 *)D_00389770 + 0x104) = 0;
    D_00451B9C[0] = 0;
    D_00389770[0x138 / 4] = 1;
    func_002433B0();
}

void func_001513C0(void) {
    func_0026C538(D_003A9EB0);
    func_0026C5B8(4);
    func_00126000();
    D_00389884[0] = 2;
}

void fldFinishDeferredExit(void) {
    if (D_00389770[0x45] == 2) {
        func_0026C900();
        if (!func_0026C768()) {
            func_0026C710();
            func_0026C728();
            func_00125F58();
            D_00389770[0x45] = 0;
        }
    }
}

s32 func_00151468(void) {
    if (D_00451B9C[0] < 2) {
        return 0;
    }
    return 1;
}

void func_00151480(void) {
    func_0014F560();
}

s32 func_00151498(void) {
    return *(s16 *)((u8 *)D_00451B94[0] + 6);
}

s32 func_001514A8(void) {
    return D_00451B98[0];
}

extern s16 D_00389876[];

s16 func_001514B8(void) {
    return D_00389876[0];
}

extern s32 D_00451D38[];

void fldResetViewState(void) {
    D_00451D38[0x6C / 4] = -1;
    D_00451D38[0x54 / 4] = 0;
    D_00451D38[0x50 / 4] = 0;
    D_00451D38[0x64 / 4] = 0;
    D_00451D38[0] = 0;
    D_00451D38[0x70 / 4] = 0;
    D_00451D38[0x34 / 4] = 0;
    D_00451D38[0x04 / 4] = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514F8);

void func_001515A0(void) {
    if (D_004363C4 != 0) {
        func_003298C0(D_004363C4);
        D_004363C4 = 0;
        D_004363C8 = 0;
        D_004363CC = 0;
        D_004363D0 = 0;
        D_004363D4 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001515E0);

extern s16 D_004363BC;

extern s16 D_004363BE;

extern s16 D_004363C0;

extern s16 D_004363C2;

void func_00151760(s16 x, s16 y, f32 *outX, f32 *outY) {
    f32 scaleX = (f32)D_004363C0;
    f32 originX = (f32)D_004363BC;
    f32 originY = (f32)D_004363BE;
    f32 sourceX = (f32)x;
    f32 sourceY = (f32)y;
    *outX = originX + scaleX * sourceX;
    *outY = originY + (f32)(-D_004363C2) * sourceY;
}

typedef struct {
    u8 x, y, z, w;
    u8 unk04[0xC];
} FieldCoordinateRecord;

typedef struct {
    s32 unk00;
    u32 count;
} FieldCoordinateList;

FieldCoordinateRecord *func_001517D0(s16 x, s16 y, s16 z, s16 w) {
    FieldCoordinateList *list = (FieldCoordinateList *)D_004363C8;
    FieldCoordinateRecord *record = (FieldCoordinateRecord *)D_004363CC;
    u32 index;
    for (index = 0; index < list->count; index++, record++) {
        if (record->x == (u8)x && record->y == (u8)y &&
            record->z == (u8)z && record->w == (u8)w) {
            return record;
        }
    }
    return NULL;
}

void fldCalcTargetDistanceYaw(f32 *distance, f32 *angle) {
    f32 *player = (f32 *)D_00389770;
    f32 *target = (f32 *)D_00451D38;
    f32 dx = player[0x14C / 4] - target[0x8 / 4];
    f32 dz = player[0x154 / 4] - target[0x10 / 4];
    f32 dist = fsqrtf(dx * dx + dz * dz);
    f32 yaw = 0.0f;
    if (!(dist < 1.0f)) {
        yaw = sdfAtan2(dx, dz) * 180.0f / 3.14f;
    }
    *distance = dist;
    *angle = yaw;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151918);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001519E8);

void func_00151DD8(void) {
}

void func_00151DE0(void) {
    s32 count = D_00451D38[0x40 / 4];
    if (count > 0) {
        D_00451D38[0x40 / 4] = count - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151E00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151FF8);

extern void func_001519E8(s32);

void func_001521F0(void) {
    s32 count = D_00451D38[0x40 / 4];
    s32 remaining = count - 1;
    if (count > 0) {
        D_00451D38[0x40 / 4] = remaining;
        count = remaining;
    }
    if (count == 0) {
        func_001519E8(1);
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152230);

void func_00152390(void) {
    s32 count = D_00451D38[0x40 / 4];
    if (count > 0) {
        D_00451D38[0x40 / 4] = count - 1;
    }
}

void func_001523B0(void) {
    s32 count = D_00451D38[0x40 / 4];
    if (count > 0) {
        D_00451D38[0x40 / 4] = count - 1;
    }
}

void func_001523D0(void) {
    s32 count = D_00451D38[0x40 / 4];
    if (count > 0) {
        D_00451D38[0x40 / 4] = count - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001523F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001525F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001526B8);

extern void func_001515A0(void);

extern void fldResetViewState(void);

extern void func_00341C78(s32);

void func_00152C18(void) {
    func_001515A0();
    fldResetViewState();
    func_00341C78(0x690061);
}

extern void fldCalcTargetDistanceYaw(f32 *, f32 *);

void fldUpdateViewAngle(void) {
    f32 distance;
    f32 angle;
    f32 *state;

    fldCalcTargetDistanceYaw(&distance, &angle);
    state = (f32 *)D_00451D38;
    state[0x44 / 4] = angle;
    state[0x14 / 4] = 180.0f - angle;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152C88);

static inline s32 scaleToVolume(s32 dist, s32 max, s32 range) {
    return (max - dist) * 127 / range;
}

s32 fldCalcDistanceVolume(f32 x, f32 y, f32 z) {
    f32 dx = D_0038BAC0[0] - x;
    f32 dy = D_0038BAC0[1] - y;
    f32 dz = D_0038BAC0[2] - z;
    f32 dist = fsqrtf(dx * dx + dy * dy + dz * dz) - 600.0f;
    s32 volume;
    if (4800.0f < dist) {
        dist = 4800.0f;
    }
    if (dist < 0.0f) {
        dist = 0.0f;
    }
    volume = scaleToVolume((s32)dist, 4800, 4800);
    if (volume > 127) {
        volume = 127;
    }
    if (volume < 0) {
        volume = 0;
    }
    return volume;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153068);

extern char D_00413F78[];

extern s32 D_00451D3C[];

INCLUDE_ASM(const s32, "game/code_001442D0", func_001533D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153410);

s32 func_00153520(void) {
    f32 distance;
    f32 angle;
    s32 inRange = 1;
    fldCalcTargetDistanceYaw(&distance, &angle);
    if (!(distance < 200.0f)) {
        inRange = 0;
    }
    return inRange;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153560);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001536B8);

void func_00153D40(s32 active) {
    if (active == 0) {
        D_00451D3C[0] = 1;
        return;
    }
    D_00451D3C[0] = 0;
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414000);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153D60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153FA0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414020);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414030);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001540E8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001542D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154528);

extern f32 D_004363E0;

extern f32 D_004363E4;

void func_00154540(f32 *x, f32 *y) {
    *x = D_004363E0;
    *y = D_004363E4;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154558);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154628);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001546F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154758);

extern void func_00133C18(void);

s32 func_001547F0(void) {
    func_00126000();
    if (D_00435F0C == 0) {
        return 1;
    }
    func_00133C18();
    return 1;
}

extern s32 D_003898B0[];

extern void dds3InvokeSlot1Handler(s32, s32);

s32 func_00154828(void) {
    if (D_00435F0C == 0) {
        return 1;
    }
    dds3InvokeSlot1Handler(D_00435F0C, 0);
    func_00133C18();
    func_00125F58();
    D_003898B0[0] = 0;
    return 1;
}

s32 func_00154870(void) {
    D_003898B0[0] = 1;
    return 1;
}

u32 func_00154880(void) {
    func_00128380(0x10);
    return 1;
}

u32 func_001548A0(void) {
    func_00128390(0x10);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001548C0);

INCLUDE_ASM(const s32, "game/code_001442D0", fldUpdateLookAtSegment);

s32 func_00154AE8(void) {
    D_003897C0[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154B00);

s32 func_00154CE0(void) {
    f32 pos[4];
    f32 rot[4];
    s32 handle;
    s32 object;
    s32 world;

    if (func_0010D650(0) == -1) {
        handle = func_00125F38();
        if (handle == 0) {
            return 1;
        }
        object = func_00110BE0(dds3GetWorldObject(), handle);
        if (object == 0) {
            return 1;
        }
        func_00113110(object, pos, rot);
        func_0012F770();
        D_0038BAD0[0] = pos[0];
        D_0038BAD0[1] = pos[1];
        D_0038BAD0[2] = pos[2];
        D_0038BAE0[0] = rot[0];
        D_0038BAE0[1] = rot[1];
        D_0038BAE0[2] = rot[2];
        D_004360B4 = 0;
        D_004360B8 = func_0010D650(1);
        D_003897C0[0] = 4;
    } else {
        world = dds3GetWorldObject();
        handle = (s32)func_00110C70(world, func_0010D650(0), 4);
        if (handle == 0) {
            return 1;
        }
        func_00110BE0(dds3GetWorldObject(), handle);
    }
    return 1;
}

s32 func_00154E08(void) {
    char *temp_v0;

    temp_v0 = func_0010D7D0(0);
    D_00389770[0x14] = 5;
    strcpy((char *)D_00389770 + 0x40, temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154E48);

void func_00154F18(void) {
}

u32 func_00154F20(void) {
    return 1;
}

u32 func_00154F28(void) {
    fldFreeDisplayObjects();
    return 1;
}

s32 fldFindSearchId(const char *name) {
    u32 *entry = func_001111A8(dds3GetWorldSecondaryObject(), name);
    if (entry != 0) {
        return entry[1];
    }
    func_0035B6E0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

s32 func_00154FA0(char *name) {
    s32 index;
    if (func_0035D600(name, D_004363E8) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (name[index] == '\0') {
            if (index < 4) return 0;
            return (name[index - 2] - '0') * 10 + (name[index - 1] - '0');
        }
    }
    return 0;
}

s32 func_00155020(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    D_003897C8[0] = temp_v0;
    return 1;
}

s32 func_00155048(void) {
    s32 temp_v0;

    D_00389988[11] = func_0010D650(0);
    temp_v0 = func_0010D650(1);
    func_00135A68(D_00389988[11], temp_v0);
    return 1;
}

s32 func_00155090(void) {
    s32 second;
    s32 third;
    D_00389988[13] = func_0010D650(0) & 0xFF;
    second = func_0010D650(1);
    third = func_0010D650(2);
    fldSetFadeTarget(D_00389988[13], second, third);
    func_00135568(func_0010D650(3));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155108);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001552E0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155498);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155690);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155848);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155BA8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155D48);

s32 fldCmdSetSceneBits(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = func_0010D650(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = func_0010D650(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = func_0010D7D0(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123138(world, stage, i, 0);
        }
    } else {
        func_00123138(world, stage, func_00154FA0(name), 0);
    }
    return 1;
}

s32 func_00155F90(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = func_0010D650(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = func_0010D650(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = func_0010D7D0(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D8;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123138(world, stage, i, 1);
        }
    } else {
        func_00123138(world, stage, func_00154FA0(name), 1);
    }
    return 1;
}

s32 func_00156070(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_00123238(area, floor, target, 0);
    return 1;
}

s32 func_001560F8(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_00123238(area, floor, target, 1);
    return 1;
}

s32 func_00156180(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_00123338(area, floor, target, 0);
    return 1;
}

s32 func_00156208(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_00123338(area, floor, target, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156290);

s32 func_00156330(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_0014AB38(area, floor, target);
    return 1;
}

s32 func_001563B8(void) {
    s32 area = func_0010D650(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = func_0010D650(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = func_0010D650(2);
    if (target == 0) return 1;
    func_0014ABA8(area, floor, target);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156440);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156538);

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = func_0010D650(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = func_0010D650(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    room = func_0010D650(2);
    if (room == 0) {
        for (; room < 16; room++) {
            func_001235E8(world, stage, room, func_0010D650(3));
        }
    } else {
        func_001235E8(world, stage, room, func_0010D650(3));
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetFlagFromWorld1);

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetFlagFromWorld3);

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetFlagFromWorld4);

INCLUDE_ASM(const s32, "game/code_001442D0", fldSetFlagFromWorld2);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414090);

INCLUDE_ASM(const s32, "game/code_001442D0", fldCmdPushSceneParam);

u32 func_001568E0(void) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D8C8();
    temp_v1 = fldFindRoomByTask(*(u32 *)(temp_v0 + 0xe4));
    func_001236E8(temp_v1);
    return 1;
}

u32 func_00156910(void) {
    if (func_00123788(fldFindRoomByTask(*(s32 *)(func_0010D8C8() + 0xE4))) != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

u32 func_00156960(void) {
    s32 scene;

    if (func_00140B80()) {
        func_00140BC8(0);
        return 1;
    }
    scene = fldGetTaskRecordValue(*(s32 *)(func_0010D8C8() + 0xE4));
    if (scene) {
        func_00140BC8(scene);
    }
    return 1;
}

u32 func_001569B8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_001411F8(temp_v0);
    return 1;
}

u32 func_001569E0(void) {
    return 1;
}

u32 func_001569E8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = fldGetActorStat0(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A18(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_001421C0(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A48(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = fldGetActorMotionEntry(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A78(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = fldGetRowValue(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156AA8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00142670(temp_v0);
    return 1;
}

u32 func_00156AD0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_001206D0(temp_v0, temp_v1);
    return 1;
}

u32 func_00156B10(void) {
    func_00120820();
    return 1;
}

s32 func_00156B30(void) {
    func_0010D818(func_00120858());
    return 1;
}

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
s32 func_00156B58(void) {
    func_0010D818(func_00140780(func_0010D650(0)));
    return 1;
}

u32 func_00156B88(void) {
    s32 scene;
    if (func_00140B80()) {
        scene = 0;
    } else {
        scene = func_0013EA18(*(s32 *)(func_0010D8C8() + 0xE4));
    }
    func_00140830(scene);
    return 1;
}

void func_00156BD0(void) {
    fldResetAfterEvent();
}

s32 func_00156BE8(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0010D818(func_0013FA98(param0, param1));
    return 1;
}

s32 func_00156C30(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0010D818(func_0013FFF8(param0, param1));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156C78);

s32 func_00156C98(void) {
    func_00140238();
    return 1;
}

s32 func_00156CB8(void) {
    D_00389798[0] = func_0010D650(0);
    func_00144CB8();
    return 1;
}

s32 func_00156CE8(void) {
    D_00389798[0] = func_0010D650(0);
    func_00144DF8();
    return 1;
}

s32 func_00156D18(void) {
    func_00144EE0();
    return 1;
}

s32 func_00156D38(void) {
    func_00144F60();
    return 1;
}

s32 func_00156D58(void) {
    func_00144F08();
    return 1;
}

s32 func_00156D78(void) {
    func_00144F88(func_0010D650(0));
    return 1;
}

s32 func_00156DA0(void) {
    fldPlayFieldSe(func_0010D650(0));
    return 1;
}

u8 func_00156DC8(void) {
    return fldLoadArchive(func_0010D650(0)) != 0;
}

s32 func_00156DF0(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_001453D0(param0, param1);
    return 1;
}

s32 func_00156E30(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_00145400(param0, param1);
    return 1;
}

s32 func_00156E70(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0014EC00(param0, param1, 0x3c);
    return 1;
}

s32 func_00156EB8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    D_0038984C[0] = temp_v0;
    return 1;
}

s32 func_00156EE0(void) {
    EffCmdWork *work = func_0010D8C8();
    void *entry = fldGetTaskRecordValue(work->key);

    if (entry != NULL) {
        func_00140A58(entry);
    }
    return 1;
}

s32 func_00156F18(void) {
    func_0010D818(func_00140750());
    return 1;
}

s32 func_00156F40(void) {
    func_0014BF98(func_0023CC00(func_0010D650(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 func_00156F70(void) {
    char *param = func_0010D7D0(0);

    func_0010D818(fldFindEffectByName(param));
    return 1;
}

s32 func_00156FA0(void) {
    func_0010D818(func_0014E620());
    return 1;
}

s32 func_00156FC8(void) {
    s32 changed = 0;
    switch (func_0010D650(0)) {
    case 0:
        if (D_00389770[3] & 1) {
            D_00389770[3] &= ~1;
            changed = 1;
        }
        break;
    case 1:
        if (D_00389770[3] & 2) {
            D_00389770[3] &= ~2;
            changed = 1;
        }
        break;
    case 2:
        if (D_00389770[3] & 4) {
            D_00389770[3] &= ~4;
            changed = 1;
        }
        break;
    case 3:
        if (D_00389770[3] & 8) {
            D_00389770[3] &= ~8;
            changed = 1;
        }
        break;
    }
    func_0010D818(changed);
    return 1;
}

s32 func_001570C0(void) {
    s32 threshold = D_003AA720[evtGetMirroredSolarPhase()];
    if (threshold >= effMiscRandMod(0, 100)) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

s32 func_00157130(void) {
    s32 value;
    value = func_0010D7D0(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 func_00157180(void) {
    func_00342690();
    return 1;
}

s32 func_001571A0(void) {
    func_0010D818(sdfSoundIsCommandBusy());
    return 1;
}

s32 func_001571C8(void) {
    s32 first = func_0010D650(0);
    s32 second = func_0010D650(1);
    s32 third = func_0010D650(2);
    s32 fourth = func_0010D650(3);
    func_00123DE8(D_00389780[0], first, second, third, fourth, func_0010D650(4));
    return 1;
}

extern s32 D_003898A4[];

s32 func_00157258(void) {
    D_003898A4[0] = 1;
    return 1;
}

extern u8 *D_00435F1C;

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157268);

s32 func_001572A0(void) {
    if (func_0010D650(0) == 2) {
        func_00153D40(0);
        D_003898B0[0] = 0;
    } else if (func_0010D650(0) == 0) {
        func_00153D40(0);
    } else {
        func_00153D40(1);
    }
    return 1;
}

extern void func_001536B8(s32);

s32 func_00157308(void) {
    func_001536B8(func_0010D650(0));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157330);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436208);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436210);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436214);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043621C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436220);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436224);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436228);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043622C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436230);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436234);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436238);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043623C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436240);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436244);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436248);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043624C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436250);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436254);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436258);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043625C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436260);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436264);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436268);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043626C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436270);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436274);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436278);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043627C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436280);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436284);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436288);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043628C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436290);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436294);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436298);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043629C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362EC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362FC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436300);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436304);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436308);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043630C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436310);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436314);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436318);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043631C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436320);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436324);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436328);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043632C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436330);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436338);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043633C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436340);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436344);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436348);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043634C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436350);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436354);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436358);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043635C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436360);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436364);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436368);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043636C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436370);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436374);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436378);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043637C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436380);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436384);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436388);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043638C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436390);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436394);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436398);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043639C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BE);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C2);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363F0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363F8);
