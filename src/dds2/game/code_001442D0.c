#include "common.h"

#include "pcp_vu0.h"

#include "fpu.h"

extern s32 fldGetSceneStatusCode(void);
extern void func_001442A0(s32);
extern void func_00144270(s32);

/* Field work area (D_00389770) fields reached through a pointer. */
typedef struct FldWorkView {
    u8 unk00[0x14];
    s32 stage;            /* 0x14 */
    u8 unk18[0x18];
    f32 focusPos[3];      /* 0x30 */
    u8 unk3C[0x14];
    s32 focusActive;      /* 0x50 */
    u8 unk54[0x10];
    f32 negatedAngle;     /* 0x64 */
    u8 unk68[8];
    s32 unk70;            /* 0x70 */
    u8 unk74[0x4C];
    s32 unkC0;            /* 0xC0 */
    u8 unkC4[0x40];
    s16 eventActive;      /* 0x104 */
    u8 unk106[0x24];
    s16 unk12A;           /* 0x12A */
    u8 unk12C[0x20];
    f32 x;                /* 0x14C */
    f32 y;
    f32 z;
    u8 unk158[0x18];
    f32 angle;            /* 0x170 */
    u8 unk174[0x4C];
    struct {
        s32 value;
        s32 block;
    } fldmix[5];          /* 0x1C0 */
} FldWorkView;

typedef struct FldSlot0C {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} FldSlot0C; /* 0xC bytes */

extern s32 D_003A5470[];
extern FldSlot0C D_0044F818[];
extern void func_00123038(s32, s32, s32, s32);
extern void func_00112230(void *, s32);

extern s32 D_004362D4;
extern u32 D_004362D8;
extern s32 D_004362E0;
extern s32 D_004362E4;
extern s32 D_004362EC;
extern s32 D_004362F0;
extern s32 D_004362F8;
extern s32 D_004362FC;
extern s32 D_00436304;
extern s32 D_00436308;
extern s32 D_00436310;
extern s32 D_00436314;
extern s32 D_00436318;
extern s32 D_0043631C;
extern s32 D_00436338;
extern s32 D_0043633C;
extern s32 D_00438EDC;
extern s32 D_00438EE0;
extern s32 D_00438EE4;
extern s32 D_00438EE8;
extern s32 D_00438EEC;
extern s32 D_00438EF0;

/* DDS2 saves store the area flag table earlier than the DDS1 save layout. */
typedef struct FldAreaFlagsView {
    u8 pad00[0xFCD0];
    u64 areaFlags[13][64];
} FldAreaFlagsView;

extern s32 D_004363C4;

extern u32 D_00435F0C;

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void evtSetSolarOverlayFullyVisible(void);

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

extern void *func_00101958();

extern u32 D_00436210;

extern u32 D_00436234;

extern u32 D_00436238;

extern u32 D_00436270;

extern s32 D_00436278;

extern u32 D_0043626C;

extern u32 D_00436364;

extern u32 D_00436354;

extern u32 D_0043635C;
extern s32 D_00436350;
extern s32 D_00436358;
extern s32 D_00436360;
extern void func_003297C8(s32);
extern void func_0014E6A8();

extern s32 D_0043637C;

extern s32 fldFindRoomByTask(u32);

extern s32 func_00120858(void);

extern void func_0010D818(s32 value);

extern s32 func_00140780(s32 value);

extern s32 func_0013FA98(s32 param0, s32 param1);

extern s32 func_0013FFF8(s32 param0, s32 param1);

extern void func_00140238(void);

extern void fldStopCurrentBgm(void);

extern void fldPlayCurrentBgmSound(void);

extern void fldReleaseCurrentBgm(void);

extern void fldPlayMenuSound(s32 param);

extern void fldPlayFieldSe(s32 param);

extern s32 fldPollArchiveLoad(s32 param);

extern void fldSetArchiveSoundVolumePan(s32 param0, s32 param1);

extern void fldPlayArchiveSound(s32 param0, s32 param1);

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

extern char *scrReadStringParameter(s32 idx);

extern s32 fldFindEffectByName(char *str);

extern s32 func_0014E620(void);

extern s32 sdfSoundIsCommandBusy(void);

extern void *func_00328D68(s32 size);

extern void func_00101950(s32 arg0, void *arg1);

extern s32 fldFieldTaskUpdate(void);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_00436208[];

extern s32 D_0043621C;

typedef struct {
    s32 flags;
    s32 soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */

extern FldClear18 D_0044F730[];

extern s32 D_00389770[];

extern void func_00342580(s32 arg0);

extern s32 D_00389798[];

extern void func_00341C78();

extern void fldRelocatePackedTransferChunk(u32 arg0, s32 arg1);

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

extern void effUpdateNode(u32 arg0);

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

extern void fldClearFloorFlag(s32, s32, s32);

extern void fldSetFloorFlag(s32, s32, s32);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 scrReadIntParameter(s32);

extern s32 D_0038984C[];

extern s32 func_00140B80(void);

extern void func_00123DE8(s32, s32, s32, s32, s32, s32);

extern s32 effMiscRandMod(s32, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 D_003AA720[];

extern void sdfWaitSlotReady(void);

extern void func_00149CE0(void);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern s32 func_0013F1B8(void);

typedef struct {
    s32 *resource;
    u8 pad[0x4C];
} FieldResourceSlot;

extern FieldResourceSlot D_0044FFB0[];

extern void fldCacheMapLabelLengths(s32);

extern void func_00145698(void);

extern char D_004363E8[];

extern s32 func_0035D600(char *, const char *);

extern void fldSetFadeTarget(s32, s32, s32);

extern void fldSetSwayMode(s32);

extern s32 D_00399FF0[];

extern s32 mdlFlagTest();

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */

extern FldEnt44 D_00389A70[32];

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 fldResolveSpecialBgmTrack(s32);

extern s32 D_0043622C;

extern s32 D_00436230;

extern s32 func_00342168(s32);

extern void func_003421E8(s32);

extern s32 D_00436274;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} FldPoint;

typedef struct {
    u8 pad0[0x10];
    s32 value;
    FldPoint *pointA; /* 0x14 */
    FldPoint *pointB; /* 0x18 */
} FldItem; /* 0x1C bytes */

typedef struct {
    u8 pad0[4];
    FldItem *items;
    u32 count;
    s32 model;         /* 0xC */
    FldPoint *pos;     /* 0x10 */
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

extern void *func_00110C70(u64, s32, s32);

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

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

typedef struct {
    u8 pad0[0xC];
    s32 *unkC;
} FldEmitterRes;

typedef struct {
    FldEmitterRes *res;
    u8 pad4[0x4C];
    f32 pos[4];
} FldEmitter;

extern u8 D_00380838[];

extern void func_003314B0();

extern void sdfModelUpdateCurrentFrameTransforms();

extern void func_003320E8();

extern s32 func_0013F790(s32 index);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001442D0);

extern u8 func_00127398(void);

extern u8 func_001275D0(void);

extern s32 fileMenuTaskExists(void);

extern s32 D_00435EE0;

s32 fldFieldTaskUpdate(void) {
    if (func_00127398() != 0) {
        return 0;
    }
    if (func_001275D0() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
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

void *fldFieldTaskCreate(s32 task) {
    s32 *work;

    work = func_00328D68(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    func_00101950(task, work);
    return fldFieldTaskUpdate;
}

void fldFieldTaskDestroy(void) {
    u64 work;

    work = func_00101958();
    func_00328E48(work);
    D_00436200 = 0;
}

void fldEnsureTask(void) {
    if (D_00436200 == 0) {
        D_00436200 = kwlnTaskCreate(D_00436208, 0x2B0B, 1, 1, fldFieldTaskCreate, fldFieldTaskDestroy, 0);
    }
}

void fldDestroyTask(void) {
    if (D_00436200 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00436200, 1);
    }
}

void fldResetPendingSounds(void) {
    FldClear18 *entry = D_0044F730;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flags = 0;
        entry->x = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    D_0043621C = 0;
}

void fldAppendPendingSoundForScene(s32 id, f32 x, f32 y, f32 z, f32 w) {
    if (D_00389770[4] == 0x1A && mdlFlagTest(0x1F) != 0) {
        return;
    }
    if (D_00389770[4] == 0x17 && D_00389770[5] == 7 && (mdlFlagTest(0x4C5) == 0 || mdlFlagTest(0x1C) != 0)) {
        return;
    }
    D_0044F730[D_0043621C].flags = 0;
    D_0044F730[D_0043621C].soundId = id;
    D_0044F730[D_0043621C].x = x;
    D_0044F730[D_0043621C].y = y;
    D_0044F730[D_0043621C].z = z;
    D_0044F730[D_0043621C].w = w;
    D_0043621C++;
}

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_00389780[0];
    if (stage == 11) {
        stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
    }
    for (i = 0; i < D_0043621C; i++) {
        if (D_0044F730[i].flags & 1) {
            D_0044F730[i].flags &= ~1;
            func_00341C78(D_00399FF0[stage] + D_0044F730[i].soundId);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001447A0);

extern s32 D_0039A0BC[];
extern s32 D_0039A0C0[];
extern s32 D_0039A0C4[];
extern s32 D_0039A0C8[];
extern s32 D_0039A0D0[];

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004136C0);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0039A0B8[0] + 1;
        break;
    case 0x81:
        result = D_0039A0BC[0] + 1;
        break;
    case 0x82:
        result = D_0039A0C0[0] + 1;
        break;
    case 0x83:
        result = D_0039A0BC[0] + 2;
        break;
    case 0x84:
        result = D_0039A0C4[0] + 1;
        break;
    case 0x85:
        result = D_0039A0C4[0] + 2;
        break;
    case 0x86:
        result = D_0039A0C8[0] + 1;
        break;
    case 0x87:
        result = D_0039A0D0[0] + 1;
        break;
    case 0x88:
        result = D_0039A0D0[0] + 2;
        break;
    case 0x89:
        result = D_0039A0D0[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_00389A70[i].id == id && (D_00389A70[i].flag == 0 || mdlFlagTest(D_00389A70[i].flag) != 0)) {
            return D_00389A70[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(D_00389770[10]);
        if (handle != -1 || D_00389770[4] < 0x32) {
            if (handle == -1) {
                stage = D_00389770[4];
                if (stage == 11) {
                    stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, D_00389770[5] + 1);
                D_00436214 = D_00399FF0[stage];
                D_00389770[10] = idx;
                handle = D_00436214 + idx;
            }
            if (D_00389770[0x130 / 4] != 1) {
                if (D_00436210 != handle) {
                    func_00342580(D_00436210);
                }
                D_00436210 = handle;
                sndStartTrackDefault(handle);
            }
        }
    }
}

s32 fldStartTitleBgmIfSelected(void) {
    if (D_003899D8[0] != 0) {
        if (D_00389798[0] == 0x80) {
            sndStartTrackDefault(D_0039A0B8[0] + 1);
        }
    }
}

extern void sndStartTrackAlternate(s32, s32);

void fldStartSceneBgmAlternate(void) {
    s32 handle;
    s32 stage;
    s32 idx;

    if (D_003899D8[0] != 0) {
        handle = fldResolveSpecialBgmTrack(D_00389770[10]);
        if (handle != -1 || D_00389770[4] < 0x32) {
            if (handle == -1) {
                stage = D_00389770[4];
                if (stage == 11) {
                    stage = mdlFlagTest(0x13) != 0 ? 2 : stage;
                }
                idx = fldFindSceneEntryData(stage, D_00389770[5] + 1);
                D_00436214 = D_00399FF0[stage];
                D_00389770[10] = idx;
                handle = D_00436214 + idx;
            }
            if (D_00389770[0x130 / 4] != 1) {
                D_00436210 = handle;
                sndStartTrackAlternate(handle, handle);
            }
        }
    }
}

void fldStopCurrentBgm(void) {
    func_00341C00(D_00436210);
    D_00436210 = 0;
}

void fldReleaseCurrentBgm(void) {
    func_00342580(D_00436210);
    if (D_00389770[10] >= 0x80) {
        D_00389770[10] = 1;
    }
    D_00436210 = 0;
}

void func_00144F48(void) {
    func_00341CA8();
}

void fldPlayCurrentBgmSound(void) {
    func_00341C78(D_00436210);
    D_00389798[0] = 0;
}

void fldPlayMenuSound(s32 id) {
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

void fldPlayFieldSe(s32 id) {
    if (D_00389780[0] < 50) {
        if (id == 0x100) {
            func_00341C78(0x6D0060);
        } else if (id == 0x101) {
            func_00341C78(0x6D0061);
        } else if (id == 0x102) {
            func_00341C78(0x6D0064);
        } else {
            func_00341C78(D_00436214 + id);
        }
    }
}

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
        sndStartTrackDefault(volume);
        D_00436228 = volume;
        break;
    case 2:
        volume = D_0039A028[0] + 1;
        sndStartTrackDefault(volume);
        D_00436228 = volume;
        break;
    }
}

void fldPrepareSceneBgmArchive(void) {
    s32 handle = fldResolveSpecialBgmTrack(D_00389770[10]);
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

s32 fldStepSceneBgmArchive(void) {
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

u32 fldGetArchiveLoadPending(void) {
    return D_00436234;
}

s32 fldPollArchiveLoad(s32 id) {
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

void fldSetArchiveSoundVolumePan(s32 arg0, s32 arg1) {
    sndSetSequenceVolumePan(arg0 * 0x10000 + arg1 + 0x30000000, 0x7f, 0x3f);
}

void fldPlayArchiveSound(s32 arg0, s32 arg1) {
    func_00341C78(arg0 * 0x10000 + arg1 + 0x30000000);
}

void fldResetArchiveLoadPhase(void) {
    D_00436238 = 0;
}

s32 fldStepArchiveLoad(void) {
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

u32 fldGetCurrentBgmHandle(void) {
    return D_00436210;
}

typedef struct FldName30 { char s[0x1E]; } FldName30;
typedef struct FldName34 { char s[0x22]; } FldName34;
typedef struct FldName28 { char s[0x1C]; } FldName28;
typedef struct FldName24 { char s[0x18]; } FldName24;

extern s16 D_0044FC98[];
extern s16 D_0044FD48[];
extern s16 D_0044FCF0[];
extern s16 D_00438ED8;
extern FldName30 D_0039A5AC[];
extern FldName34 D_0039E1AC[];
extern FldName28 D_003A25AC[];
extern FldName24 D_0039A1D0[];
extern s32 strlen(const char *);
extern s32 func_001237B0(s32, s32);
extern s32 func_00123808(s32, s32);
extern s32 fldFindMapCoordinateIndex(s32, s32);

void fldCacheMapLabelLengths(s32 world) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_0044FC98[i] = strlen(D_0039A5AC[func_001237B0(world, i)].s);
    }
    for (i = 0; i < 24; i++) {
        D_0044FD48[i] = strlen(D_0039E1AC[func_00123808(world, i)].s);
    }
    for (i = 0; i < 41; i++) {
        D_0044FCF0[i] = strlen(D_003A25AC[fldFindMapCoordinateIndex(world, i)].s);
    }
    i = 0;
    if (D_00389780[0] < 100) {
        i = D_00389780[0];
    }
    D_00438ED8 = strlen(D_0039A1D0[i].s);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145698);

void fldSetSceneRecordChunk(s32 arg0, s32 arg1) {
    /* Descriptor from func_00129D60 precedes the 0x14-byte scene rows. */
    typedef struct {
        u8 pad00[4];
        s32 rows; /* 0x04: first scene row */
        s32 count; /* 0x08: number of scene rows */
    } SceneHeader;
    if (D_00389780[0] < 0xC8) {
        s32 source = arg0;
        s32 resource = arg1;
        s32 transferStart = source + 8;

        D_00436278 = resource;
        fldRelocatePackedTransferChunk(arg0, transferStart);
        {
            s32 header = func_00129D60(transferStart);
            s32 rows = ((SceneHeader *)header)->rows;
            s32 count = ((SceneHeader *)header)->count;

            D_00436274 = count;
            D_00436270 = rows;
        }
    }
}

void fldInitSceneMapLabels(void) {
    if (D_00389780[0] < 200) {
        fldCacheMapLabelLengths(D_00389780[0] % 100);
        func_00145698();
    }
}

void fldReleaseSceneRecordChunk(void) {
    if (D_00436278 != 0) {
        func_003298C0(D_00436278);
    }
    D_00436278 = 0;
    D_00436270 = 0;
    D_00436274 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145818);

void fldSetEmitterPosition(FldEmitter *emitter, f32 x, f32 y, f32 z) {
    f32 pos[4];
    s32 handle;

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    handle = *emitter->res->unkC;
    __asm__ volatile (".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    __asm__ volatile (".set noreorder\n\tvmove.w vf10, vf0\n\t.set reorder");
    __asm__ volatile (".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(emitter->pos) : "memory");
    func_003314B0(handle);
    sdfModelUpdateCurrentFrameTransforms(emitter);
    func_003320E8(D_00380838, emitter);
}

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
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[1]);
    }
    if (D_0044F7F0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[2]);
    }
    if (D_0044F7F0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[3]);
    }
    if (D_0044F7F0[6] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[6]);
    }
    D_0044F7F0[1] = 0;
    D_0044F7F0[2] = 0;
    D_0044F7F0[3] = 0;
    D_0044F7F0[6] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    D_0043626C = 0;
    func_0019D1F8(0x54);
}

extern void sdfReleaseDevSlot(s32, s32, s32);


void func_00149CE0(void) {
    s32 i;

    for (i = 0; i < D_00436274; i++) {
        if (D_003A5470[i] != 0) {
            sdfReleaseDevSlot(D_003A5470[i], 1, 1);
        }
    }
    for (i = 0; i < 96; i++) {
        D_003A5470[i] = 0;
        D_0044F818[i].unk0 = 0;
        D_0044F818[i].unk4 = 0;
        D_0044F818[i].unk8 = 0;
    }
    if (D_0044F7F0[5] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[5]);
    }
    if (D_0044F7F0[7] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[7]);
    }
    if (D_0044F7F0[8] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[8]);
    }
    if (D_0044F7F0[9] != 0) {
        sdfTexReleaseReferenceViaHandler(D_0044F7F0[9]);
    }
    D_0044F7F0[5] = 0;
    D_0044F7F0[7] = 0;
    D_0044F7F0[8] = 0;
    D_0044F7F0[9] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    D_0043626C = 0;
}

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

typedef struct FldFogParams {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
} FldFogParams;
extern FldFogParams D_0037FB30;
extern u128 D_0037FA20;
extern u128 D_0037FA30;
extern u128 D_0037FA40;
extern s32 D_0043624C;
extern s32 D_00436250;
extern s32 D_00436254;
extern s32 D_00436258;
extern s32 D_0043625C;
extern s32 D_00436264;
extern void func_00128358(s32);
extern f32 D_0037F980[];
extern FldVec4 D_00413788[]; /* default camera up vectors (3 copies), the first still read by asm func_00149A00 */
extern void func_001363D8(void);
extern void func_00110A88(s32, s32);
extern s32 D_00436260;
extern s32 D_00436268;

/* Enters the field camera state for a fresh scene: releases the resource slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldWorkView *cam = (FldWorkView *)D_00389770;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;

    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseResourceSlots();
    D_0043626C = 0;
    func_00128358(1);
    func_00125F58();
    evtSetSolarOverlayFullyVisible();
    func_001363D8();
    cam->unk70 = 4;
    func_0019D1F8(0x54);
    func_00110A88(dds3GetWorldObject(), 1);
    D_00436268 = 0;
    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[1], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, &up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x108010;
    D_0037F980[4] = 2244.0f;
    D_0037F980[5] = 2118.0f;
}

extern s32 sdfModelCreateWithAlternateItems(s32, s32);
extern s32 func_0032C138();

/* Loads the scene's models into the resource slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldWorkView *cam;
    FldSceneRecord *rec;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 i;

    rec = (FldSceneRecord *)D_00436270;
    for (i = 0; i < D_00436274; i++, rec++) {
        D_003A5470[i] = sdfModelCreateWithAlternateItems(0, rec->model);
        ((FldPoint *)D_0044F818)[i].x = rec->pos->x;
        ((FldPoint *)D_0044F818)[i].y = rec->pos->y;
        ((FldPoint *)D_0044F818)[i].z = rec->pos->z;
    }
    cam = (FldWorkView *)D_00389770;
    D_0044F7F0[5] = func_0032C138(cam->fldmix[0].block);
    D_0044F7F0[7] = func_0032C138(cam->fldmix[2].block);
    D_0044F7F0[8] = func_0032C138(cam->fldmix[3].block);
    D_0044F7F0[9] = func_0032C138(cam->fldmix[4].block);
    D_00436268 = 0;
    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    memcpy(&up, &D_00413788[2], sizeof(up));
    D_00436260 = 0;
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, &up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x108010;
    D_0037F980[4] = 2241.0f;
    D_0037F980[5] = 2113.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    func_00149CE0();
}

u32 fldGetSceneReadyFlag(void) {
    return D_0043626C;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A258);


/* Re-centers the scene camera on the current scene's entry point. */
INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137B8);

void fldCenterCameraOnEntry(void) {
    FldWorkView *cam = (FldWorkView *)D_00389770;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_00436248 = cam->stage;
    D_0043624C = cam->unkC0;
    D_00436264 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_0043625C = iz;
    D_00436258 = ix;
    D_00436250 = ix;
    D_00436254 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_0037FA30, eye);
    PCP_COPY_VECTOR(&D_0037FA20, focus);
    PCP_COPY_VECTOR(&D_0037FA40, up);
    D_0037FB30.unk0 = 255.0f;
    D_0037FB30.unk8 = 1000.0f;
    D_0037FB30.unk4 = 255.0f;
    D_0037FB30.unkC = 20000.0f;
    D_0037FB30.unk10 = 0x808080;
}

void fldSetSceneLocation(s32 arg0, s32 arg1, s32 arg2) {
    D_00389770[4] = arg0;
    D_00389770[6] = arg2;
    D_00436248 = D_00389770[5] = arg1;
    D_00436244 = arg0 % 100;
}

extern s32 D_00387CE0[];

/* The area number's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];

    if (areaIndex == -1) {
        return 0;
    }
    return (((FldAreaFlagsView *)D_00435DD0)->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetCurrentFloorFlag(s32 flagNumber) {
    s32 areaIndex;
    s32 floor;
    s32 bitIndex;

    if (flagNumber <= 0) {
        return;
    }
    areaIndex = D_00387CE0[D_00436244 % 100];
    if (areaIndex == -1) {
        return;
    }
    floor = D_00389770[5];
    bitIndex = flagNumber - 1;
    {
        /* Keep byte-offset arithmetic: direct array indexing changes ee-gcc's codegen. */
        s32 byteOffset = 0xFCD0 + (areaIndex * 64 + floor) * 8;
        u64 mask = (u64)1 << bitIndex;
        u64 *flags = (u64 *)(D_00435DD0 + byteOffset);
        D_00389770[6] = bitIndex;
        D_00389770[47] = flagNumber;
        *flags |= mask;
    }
    D_00389770[48] = fldFindRecordItem(floor, bitIndex);
}

/* The public setters take one-based floor and flag numbers. */
void fldSetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (areaIndex != -1) {
        s32 byteOffset = 0xFCD0 + ((areaIndex * 64 + floor) * 8);
        u64 mask = (u64)1 << bit;
        u64 *flags = (u64 *)(D_00435DD0 + byteOffset);
        *flags |= mask;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_00387CE0[area % 100];
    floor--;
    bit--;
    if (areaIndex != -1) {
        s32 byteOffset = 0xFCD0 + ((areaIndex * 64 + floor) * 8);
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
        u32 entryCount = ((FldSceneRecord *)group)->count;
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
            entryCount = ((FldSceneRecord *)group)->count;
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
        u32 entryCount = ((FldSceneRecord *)group)->count;
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
            entryCount = ((FldSceneRecord *)group)->count;
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

extern s32 D_00436240;

void fldGetVisibleSceneBounds(f32 *minX, f32 *maxZ, f32 *maxX, f32 *minZ) {
    s32 room;
    u32 i;
    FldSceneRecord *rec;
    FldItem *item;
    FldPoint *a;
    s32 visible;
    FldPoint *b;

    room = 0;
    *minX = 0.0f;
    *maxZ = 0.0f;
    *maxX = 0.0f;
    *minZ = 0.0f;
    rec = (FldSceneRecord *)D_00436270;
    for (; room < D_00436274; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->count; i++, item++) {
            visible = 0;
            if (fldGetFloorFlag(D_00436244, room, i) != 0) {
                visible = 1;
            }
            if (D_00436240 != 0) {
                visible = 1;
            }
            if (visible != 0) {
                a = item->pointA;
                if (a != NULL) {
                    if (*minX > a->x) {
                        *minX = a->x;
                    }
                    b = item->pointB;
                    if (*minX > b->x) {
                        *minX = b->x;
                    }
                    if (*maxX < a->x) {
                        *maxX = a->x;
                    }
                    if (*maxX < b->x) {
                        *maxX = b->x;
                    }
                    if (*maxZ < a->z) {
                        *maxZ = a->z;
                    }
                    if (*maxZ < b->z) {
                        *maxZ = b->z;
                    }
                    if (a->z < *minZ) {
                        *minZ = a->z;
                    }
                    if (b->z < *minZ) {
                        *minZ = b->z;
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", fldCheckSceneReady);

void func_0014B050(void) {
    if (D_0043626C == 1) {
        func_00148488();
        func_00146250();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00148A98();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    s32 block;
    s32 blockIndex;

    blockIndex = 0;
    block = D_00435DD0;
    do {
        words = (u64 *)(block + 0xfcd0);
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        blockIndex = blockIndex + 1;
        block = block + 0x200;
    } while (blockIndex < 10);
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0F8);

extern s32 D_003A8E60[];
extern s32 func_001579C8();
extern char *D_00413860[4];
void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_00413860, sizeof(names));
    for (i = 0; i < 4; i++) {
        D_003A8E30[i] = func_00343ED0(names[i], &D_003A8E40[i], 0);
        D_003A8E50[i] = func_001579C8(D_003A8E40[i]);
        D_003A8E60[i] = 0;
    }
}

void fldReleaseTextureSlots(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_003A8E50[i] != 0) {
            effDestroyNode(D_003A8E50[i]);
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
        effDestroyNode(D_0043632C);
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
        effUpdateNode(D_0043632C);
    }
}

void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        D_003A8E70[i] = func_001579C8(D_0043633C);
        D_003A8E80[i] = 0;
    }
}

extern s32 D_00436348;

void mnuSpawnResourceAtPosition(f32 x, f32 y, f32 z) {
    f32 pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 handle;

    handle = D_003A8E70[D_00436348];
    if (handle != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_001576D8(handle);
        func_00157790(D_003A8E70[D_00436348], pos);
        D_003A8E80[D_00436348] = 1;
        D_00436348 = (D_00436348 + 1) % 4;
    }
}

void fldQueuePrimaryEffectPosition(f32 arg0, f32 arg1, f32 arg2) {
    D_00436340 = 1;
    D_003A8E90[0] = arg0;
    D_003A8E90[1] = arg1;
    D_003A8E90[2] = arg2;
}

void fldQueueSecondaryEffectPosition(f32 arg0, f32 arg1, f32 arg2) {
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
            effDestroyNode(D_003A8E70[i]);
            D_003A8E70[i] = 0;
        }
    }
}

void func_0014B9E0(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (D_003A8E70[i] != 0 && D_003A8E80[i] != 0) {
            effUpdateNode(D_003A8E70[i]);
        }
    }
}

extern void *func_002C7FF0(const char *);
extern void func_002C81D0(void *);
extern void func_002C7CE8(void *);
extern s32 sdfMemoryGetBlockAddress(s32);

typedef struct FldLbNode {
    struct FldLbNode *next; /* 0x00 */
    u8 unk04[4];
    s32 value;              /* 0x08 */
} FldLbNode;

typedef struct FldLbFile {
    u8 unk00[0x60];
    FldLbNode *nodes; /* 0x60 */
} FldLbFile;

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

void func_0014BA60(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;

    D_00389770[0x1E8 / 4] = func_00343ED0("/fld/f/bin/d2_fild1.tmx", &D_00389770[0x1EC / 4], 0);
    D_00389770[0x1F4 / 4] = func_00343ED0("/fld/f/bin/d2_fild2.tmx", &D_00389770[0x1F8 / 4], 0);
    D_00389770[0x200 / 4] = func_00343ED0("/fld/f/bin/d2_fild3.tmx", &D_00389770[0x204 / 4], 0);
    D_00389770[0x20C / 4] = func_00343ED0("/fld/f/bin/d2_fild4.tmx", &D_00389770[0x210 / 4], 0);
    D_00389770[0x1A8 / 4] = func_00343ED0("/fld/f/bin/autmap_1.tmx", &D_00389770[0x1AC / 4], 0);
    D_00389770[0x1B0 / 4] = func_00343ED0("/fld/f/bin/autmap_2.tmx", &D_00389770[0x1B4 / 4], 0);
    D_00389770[0x1B8 / 4] = func_00343ED0("/fld/f/bin/autmap_3.tmx", &D_00389770[0x1BC / 4], 0);
    D_00389770[0x1C0 / 4] = func_00343ED0("/fld/f/bin/autmap_5.tmx", &D_00389770[0x1C4 / 4], 0);
    D_00389770[0x1C8 / 4] = func_00343ED0("/fld/f/bin/autmap_6.tmx", &D_00389770[0x1CC / 4], 0);
    D_00389770[0x1D0 / 4] = func_00343ED0("/fld/f/bin/autmap_7.tmx", &D_00389770[0x1D4 / 4], 0);
    D_00389770[0x1D8 / 4] = func_00343ED0("/fld/f/bin/autmap_8.tmx", &D_00389770[0x1DC / 4], 0);
    D_00389770[0x1E0 / 4] = func_00343ED0("/fld/f/bin/autmap_9.tmx", &D_00389770[0x1E4 / 4], 0);
    index = 0;
    D_004362E0 = func_00343ED0("/fld/f/bin/TOPEN.D3P", &D_004362E4, 0);
    D_00436304 = func_00343ED0("/fld/f/bin/TAKARA2.D3P", &D_00436308, 0);
    D_004362EC = func_00343ED0("/fld/f/bin/TOPEN2.D3P", &D_004362F0, 0);
    D_00436310 = func_00343ED0("/fld/f/bin/TAKARA3.D3P", &D_00436314, 0);
    D_004362F8 = func_00343ED0("/fld/f/bin/TOPEN3.D3P", &D_004362FC, 0);
    D_00436318 = func_00343ED0("/fld/f/bin/TAKARA4.D3P", &D_0043631C, 0);
    D_00438EDC = func_00343ED0("/fld/f/bin/DAMAGE_1.D3P", &D_00438EE0, 0);
    D_00438EE4 = func_00343ED0("/fld/f/bin/DAMAGE_2.D3P", &D_00438EE8, 0);
    D_00438EEC = func_00343ED0("/fld/f/bin/DAMAGE_3.D3P", &D_00438EF0, 0);
    lb = func_002C7FF0("/fld/f/bin/fldmix.LB");
    func_002C81D0(lb);
    for (node = lb->nodes; node != NULL; node = node->next, index++) {
        switch (index) {
        case 5:
            D_00436338 = node->value;
            D_0043633C = sdfMemoryGetBlockAddress(node->value);
            break;
        case 6:
            D_004362D4 = node->value;
            D_004362D8 = sdfMemoryGetBlockAddress(node->value);
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            break;
        }
    }
    func_002C7CE8(lb);
}

extern s32 D_00438EE0;
extern s32 D_00438EE8;
extern s32 D_00438EF0;
extern u32 D_004362D8;
extern u32 D_004362DC;
extern u32 D_004362CC;
extern s32 D_004362D0;

void func_0014BD20(void) {
    if (D_00389770[4] < 200) {
        D_004362DC = func_001578C0(D_004362D8);
        if (D_00389770[4] == 26) {
            D_004362CC = func_001578C0(D_00438EE0);
        } else if (D_00389770[4] == 29) {
            D_004362CC = func_001578C0(D_00438EE8);
        } else if (D_00389770[4] == 30) {
            D_004362CC = func_001578C0(D_00438EF0);
        } else {
            D_004362CC = 0;
        }
        D_004362D0 = 0;
        D_00389770[0x1F0 / 4] = func_0032C138(D_00389770[0x1EC / 4]);
        D_00389770[0x1FC / 4] = func_0032C138(D_00389770[0x1F8 / 4]);
        D_00389770[0x208 / 4] = func_0032C138(D_00389770[0x204 / 4]);
        D_00389770[0x214 / 4] = func_0032C138(D_00389770[0x210 / 4]);
    }
}

extern u32 D_004362DC;

extern u32 D_004362E8;

extern u32 D_004362F4;

extern u32 D_004362CC;

void fldFreeSceneResources(void) {
    if (D_00389770[0x1F0 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(D_00389770[0x1F0 / 4]);
        D_00389770[0x1F0 / 4] = 0;
    }
    if (D_00389770[0x1FC / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(D_00389770[0x1FC / 4]);
        D_00389770[0x1FC / 4] = 0;
    }
    if (D_00389770[0x208 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(D_00389770[0x208 / 4]);
        D_00389770[0x208 / 4] = 0;
    }
    if (D_00389770[0x214 / 4] != 0) {
        sdfTexReleaseReferenceViaHandler(D_00389770[0x214 / 4]);
        D_00389770[0x214 / 4] = 0;
    }
    if (D_004362DC != 0) {
        effDestroyNode(D_004362DC);
        D_004362DC = 0;
    }
    if (D_004362E8 != 0) {
        effDestroyNode(D_004362E8);
        D_004362E8 = 0;
    }
    if (D_004362F4 != 0) {
        effDestroyNode(D_004362F4);
        D_004362F4 = 0;
    }
    if (D_004362CC != 0) {
        effDestroyNode(D_004362CC);
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
            effDestroyNode(D_00450990[i].unk24);
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
            effDestroyNode(D_00450990[i].unk24);
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

extern void *memset(void *, s32, u32);
extern s32 D_004362D0;

s32 func_0014D7B8(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = 1.0f;
    func_001576D8(D_004362CC);
    func_00157790(D_004362CC, pos);
    D_004362D0 = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D838);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DB50);

extern s32 func_00128370(u32);

void func_0014DDD8(void) {
    if (D_00389770[4] < 0xC8) {
        if (func_00128370(1) != 0) {
            return;
        }
        if (func_00127398() != 0) {
            return;
        }
        if (func_001275D0() != 0) {
            return;
        }
        if (D_00436344 != 0) {
            if (((FldWorkView *)D_00389770)->unk12A == 0) {
                mnuSpawnResourceAtPosition(D_003A8EA0[0], D_003A8EA0[1], D_003A8EA0[2]);
            }
            D_00436344 = 0;
        }
        if (D_00436340 != 0) {
            if (((FldWorkView *)D_00389770)->unk12A == 0) {
                mnuSpawnResourceAtPosition(D_003A8E90[0], D_003A8E90[1], D_003A8E90[2]);
            }
            D_00436340 = 0;
        }
    }
}

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

extern u32 sdfDevCreateCommandState(const char *);
extern u32 func_0033EB10(u32, void *, u32);
extern void func_0033EAE0(u32);
typedef struct FldNpcPalette {
    u32 word[0x80];
} FldNpcPalette; /* 0x200 bytes */

extern FldNpcPalette D_0044FD90;

void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    u32 command;

    if (field < 100) {
        fldFormatAreaDirectory(directory, field, 1);
        if (field == 23 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF033.NPL", directory);
        } else if (field == 24 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF034.NPL", directory);
        } else if (field == 27 && mdlFlagTest(25) != 0) {
            func_0035C860(path, "%sF037.NPL", directory);
        } else {
            func_0035C860(path, "%sF%03d.NPL", directory, field);
        }
        command = sdfDevCreateCommandState(path);
        func_0033EB10(command, &D_0044FD90, 0x200);
        func_0033EAE0(command);
    }
}

void fldSetNpcPalette(FldNpcPalette *src) {
    D_0044FD90 = *src;
}

extern s32 strcmp(const char *, const char *);

s32 fldFindEffectByName(char *name) {
    s32 i;

    for (i = 0; i < D_004362C8; i++) {
        if (D_0044FF90[i].unk0 != 999 && strcmp(D_0044FF90[i].name, name) == 0) {
            return D_0044FF90[i].unk20[1];
        }
    }
    return 0;
}

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

void fldReleaseTitleTextures(void) {
    if (D_00436368 != 0) {
        sdfTexReleaseReferenceViaHandler(D_00436368);
        D_00436368 = 0;
    }
    if (D_0043636C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043636C);
        D_0043636C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C10);

void func_0014EC00(s32 field, s32 arg1, s32 arg2) {
    char path[32];
    s32 size;
    s32 handle;

    D_00436350 = 0;
    D_00436354 = 0;
    D_00436358 = field;
    D_0043635C = arg1;
    D_00436360 = arg2;
    D_00436364 = 0;
    if ((u32)(field - 8) < 2) {
        func_0035C860(path, "/fld/f/pnl/df%03d_a.tmx", field);
    } else if (field == 0xC) {
        func_0035C860(path, "/fld/f/pnl/df%03d_b.tmx", 0xC);
    } else {
        func_0035C860(path, "/fld/f/pnl/df%03d.tmx", field);
    }
    handle = func_00343ED0(path, &size, 0);
    D_00436368 = func_0032C138(size);
    func_003297C8(handle);
    if (field == 0xC) {
        func_0035C860(path, "/fld/f/pnl/df_b.tmx");
        handle = func_00343ED0(path, &size, 0);
        D_0043636C = func_0032C138(size);
        func_003297C8(handle);
    }
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(D_00413C10, 0x2B0A, 0, 1, func_0014E6A8, fldReleaseTitleTextures, 0);
    }
}

void fldDestroyTitleTask(void) {
    if (fldTitleIsActive()) {
        kwlnTaskDestroyWithHierarchyByName(D_00413C10, 1);
    }
}

s32 fldTitleMiniIsActive(void) {
    return func_00101740(D_00413C80) != 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EDB8);

void fldReleaseTitleMiniTexture(void) {
    if (D_0043637C != 0) {
        sdfTexReleaseReferenceViaHandler(D_0043637C);
        D_0043637C = 0;
    }
}

extern s32 D_003898B4[];
extern s32 D_00436378;
extern void func_0014EDB8(void);
extern void fldReleaseTitleMiniTexture(void);
extern void func_003297C8(s32);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

void fldStartMiniTitleForUnlock(s32 id) {
    char path[0x20];
    s32 data;
    s32 handle;

    switch (id) {
    case 4:
        if (mdlFlagTest(0x420) == 0) {
            return;
        }
        break;
    case 5:
        if (mdlFlagTest(0x424) == 0) {
            return;
        }
        break;
    case 7:
    case 8:
    case 9:
    case 11:
    case 13:
    case 14:
    case 15:
    case 30:
        break;
    case 10:
        if (mdlFlagTest(0x421) == 0) {
            return;
        }
        break;
    case 12:
        if (mdlFlagTest(0x422) == 0) {
            return;
        }
        break;
    case 21:
        if (mdlFlagTest(0x440) == 0) {
            return;
        }
        break;
    case 22:
        if (mdlFlagTest(0x480) == 0) {
            return;
        }
        break;
    case 23:
        if (mdlFlagTest(0x4C0) == 0) {
            return;
        }
        break;
    case 24:
        if (mdlFlagTest(0x500) == 0) {
            return;
        }
        break;
    case 25:
        if (mdlFlagTest(0x540) == 0) {
            return;
        }
        break;
    case 26:
        if (mdlFlagTest(0x580) == 0) {
            return;
        }
        break;
    case 27:
        if (mdlFlagTest(0x5C0) == 0) {
            return;
        }
        break;
    case 28:
        if (mdlFlagTest(0x600) == 0) {
            return;
        }
        break;
    case 29:
        if (mdlFlagTest(0x640) == 0) {
            return;
        }
        break;
    case 31:
        if (mdlFlagTest(0x6E0) == 0) {
            return;
        }
        break;
    case 32:
        if (mdlFlagTest(0x6E1) == 0) {
            return;
        }
        break;
    case 33:
        if (mdlFlagTest(0x6E2) == 0) {
            return;
        }
        break;
    case 34:
        if (mdlFlagTest(0x6E3) == 0) {
            return;
        }
        break;
    case 35:
        if (mdlFlagTest(0x6E4) == 0) {
            return;
        }
        break;
    case 38:
        if (mdlFlagTest(0x6E5) == 0) {
            return;
        }
        break;
    default:
        return;
    }
    D_003898B4[0] = 20;
    D_00436378 = id;
    D_00436370 = 0;
    D_00436374 = 0;
    func_0035C860(path, "/fld/f/pnl/ds_%03d.tmx", id);
    handle = func_00343ED0(path, &data, 0);
    D_0043637C = func_0032C138(data);
    func_003297C8(handle);
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(D_00413C80, 0x2B0A, 0, 1, func_0014EDB8, fldReleaseTitleMiniTexture, 0);
    }
}

void fldRequestMiniTitleDismiss(void) {
    if (D_00436374 == 0) {
        D_00436370 = 0;
        D_00436374 = 1;
    }
}

void func_0014F318(void) {
    s32 data;
    s32 handle;

    handle = func_00343ED0("/fld/f/bin/d2_hunt1.tmx", &data, 0);
    D_004363AC = func_0032C138(data);
    func_003298C0(handle);
    handle = func_00343ED0("/fld/f/bin/d2_hunt2.tmx", &data, 0);
    D_004363B0 = func_0032C138(data);
    func_003298C0(handle);
    handle = func_00343ED0("/fld/f/bin/d2_hunt3.tmx", &data, 0);
    D_004363B4 = func_0032C138(data);
    func_003298C0(handle);
    D_00436380 = func_00343ED0("/fld/f/bin/FH_DAM_2.EPL", &D_00436384, 0);
    D_00436388 = func_001579C8(D_00436384);
    D_0043638C = 0;
    D_00436390 = func_00343ED0("/fld/f/bin/YUK_2.EPL", &D_00436394, 0);
    D_00436398 = func_001579C8(D_00436394);
    D_0043639C = 0;
}

void func_0014F408(void) {
    if (D_004363AC != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363AC);
        D_004363AC = 0;
    }
    if (D_004363B0 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B0);
        D_004363B0 = 0;
    }
    if (D_004363B4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_004363B4);
        D_004363B4 = 0;
    }
    effDestroyNode(D_00436388);
    D_00436388 = 0;
    D_0043638C = 0;
    func_003298C0(D_00436380);
    D_00436380 = 0;
    D_00436384 = 0;
    effDestroyNode(D_00436398);
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
        effUpdateNode(D_00436388);
    }
    if (D_00436398 != 0 && D_0043639C != 0) {
        effUpdateNode(D_00436398);
    }
}

typedef struct {
    s16 category;
    s16 id;
    s16 unk04;
    u8 unk06[2];
    s16 unk08;
    u8 unk0A[0xD6];
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

extern s32 D_003899E0[];
extern void func_00125B10(void);

void fldFinishEventFieldState(void) {
    FldWorkView *work = (FldWorkView *)D_00389770;

    if (work->eventActive != 0) {
        D_003899E0[0] = 0;
        func_00125B10();
        func_0014F788();
        func_0014F408();
        fldStartSceneBgmAlternate();
        func_00125F58();
        work->eventActive = 0;
        D_00451B90[3] = 0;
        D_00451B90[4] = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

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

void fldResetEventSceneState(void) {
    func_00130F98(0);
    D_003899E0[0] = 0;
    D_00389770[0x118 / 4] = 0;
    func_0014F788();
    func_0014F408();
    func_00125F58();
    D_00389770[0x114 / 4] = 1;
    ((FldWorkView *)D_00389770)->eventActive = 0;
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
    fldStartSceneBgmAlternate();
    D_00389770[0x114 / 4] = 0;
    ((FldWorkView *)D_00389770)->eventActive = 0;
    D_00451B9C[0] = 0;
    D_00389770[0x138 / 4] = 1;
    evtSetSolarOverlayFullyVisible();
}

void fldStartDeferredFieldExit(void) {
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

s32 fldIsEventPhaseAtLeastTwo(void) {
    if (D_00451B9C[0] < 2) {
        return 0;
    }
    return 1;
}

void func_00151480(void) {
    func_0014F560();
}

/* Reads the signed halfword at +6 of the current entry. The record's layout
 * and the meaning of this value are not established elsewhere in this unit. */
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

void fldReleaseTargetGuideResource(void) {
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

void fldMapGridToScreenPosition(s16 x, s16 y, f32 *outX, f32 *outY) {
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

FieldCoordinateRecord *fldFindCoordinateRecord(s16 x, s16 y, s16 z, s16 w) {
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

void fldTickTargetGuideCounter(void) {
    s32 count = D_00451D38[0x40 / 4];
    if (count > 0) {
        D_00451D38[0x40 / 4] = count - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151E00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151FF8);

extern void func_001519E8(s32);

void fldTickTargetGuideAndNotify(void) {
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

extern void fldReleaseTargetGuideResource(void);

extern void fldResetViewState(void);

extern void func_00341C78(s32);

void fldResetTargetViewAndSound(void) {
    fldReleaseTargetGuideResource();
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

s32 func_001533D0(void) {
    if (D_00451D3C[0] != 1) {
        if (func_00127398() != 0) {
            func_0035B6E0(D_00413F78);
        }
    }
}

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

extern s32 D_003898AC[];
extern s32 D_004363DC;

s32 func_00154528(void) {
    if (D_003898AC[0] != 0) {
        return D_004363DC;
    }
    return 0;
}

extern f32 D_004363E0;

extern f32 D_004363E4;

void func_00154540(f32 *x, f32 *y) {
    *x = D_004363E0;
    *y = D_004363E4;
}

extern s32 func_0013D308(s32, void *);
extern s32 func_0013D598(s32, void *);

s32 func_00154558(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = D_00435F0C;
    void *entry;
    s32 result;

    if (unit == 0) {
        func_0010D818(0);
        return 1;
    }
    entry = func_00110C70(world, fldFindTaskRecordId(((EffCmdWork *)func_0010D8C8())->key), 0x11);
    if (entry == 0) {
        func_0010D818(0);
        return 1;
    }
    if (func_0013D308(unit, entry) == 0) {
        func_0010D818(0);
        return 1;
    }
    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    switch (result) {
    case 1:
        func_0010D818(1);
        return 1;
    case -1:
        func_0010D818(-1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

s32 func_00154628(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = D_00435F0C;
    void *entry;
    s32 result;

    if (unit == 0) {
        func_0010D818(0);
        return 1;
    }
    entry = func_00110C70(world, fldFindTaskRecordId(((EffCmdWork *)func_0010D8C8())->key), 0x11);
    if (entry == 0) {
        func_0010D818(0);
        return 1;
    }
    if (func_0013D598(unit, entry) == 0) {
        func_0010D818(0);
        return 1;
    }
    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    switch (result) {
    case 1:
        func_0010D818(1);
        return 1;
    case -1:
        func_0010D818(-1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

extern void func_001442A0(s32);
extern void func_00144270(s32);

s32 func_001546F8(void) {
    s32 result;

    func_001442A0(scrReadIntParameter(0));
    func_00144270(1);
    result = func_001442D0();
    if (result == -1) {
        func_0010D818(-1);
        return 1;
    }
    if (result == 1) {
        func_0010D818(1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

s32 func_00154758(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = D_00435F0C;
    void *entry;

    if (unit == 0) {
        func_0010D818(0);
        return 1;
    }
    entry = func_00110C70(world, fldFindTaskRecordId(((EffCmdWork *)func_0010D8C8())->key), 0x11);
    if (entry == 0) {
        func_0010D818(0);
        return 1;
    }
    if (func_0013D308(unit, entry) == 0) {
        func_0010D818(0);
    } else {
        func_0010D818(1);
    }
    return 1;
}

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

typedef struct FldObjModel {
    u8 unk00[0x40];
    f32 pos[3]; /* 0x40 */
} FldObjModel;

typedef struct FldObj {
    u8 unk00[0x1C];
    FldObjModel *model; /* 0x1C */
} FldObj;

extern void func_001300A0(void);

s32 func_001548C0(void) {
    FldWorkView *work;
    FldObj *obj;
    u64 world = dds3GetWorldSecondaryObject();

    obj = func_00110C70(world, scrReadIntParameter(0), 4);
    if (obj == NULL) {
        return 1;
    }
    work = (FldWorkView *)D_00389770;
    work->focusActive = 1;
    work->focusPos[0] = obj->model->pos[0];
    work->focusPos[1] = obj->model->pos[1];
    work->focusPos[2] = obj->model->pos[2];
    func_001300A0();
    return 1;
}

typedef struct FldVec3 {
    f32 x;
    f32 y;
    f32 z;
} FldVec3;
extern f32 D_0038BAB0[];
extern f32 sdfSinPoly(f32);
extern f32 func_003407A0(f32);
extern s32 func_00133B10();
extern void func_00133A00();
extern void func_0012F078();
s32 fldUpdateLookAtSegment(void) {
    FldWorkView *cam = (FldWorkView *)D_00389770;
    FldVec3 near;
    FldVec3 far;

    cam->focusActive = 0;
    cam->negatedAngle = -cam->angle;
    near.x = cam->x - sdfSinPoly((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + func_003407A0((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + func_003407A0(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    D_0038BAB0[0] = near.x;
    D_0038BAB0[1] = near.y;
    D_0038BAB0[2] = near.z;
    D_0038BAC0[0] = far.x;
    D_0038BAC0[1] = far.y;
    D_0038BAC0[2] = far.z;
    func_00133B10();
    func_00133A00();
    func_0012F078();
    return 1;
}

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

    if (scrReadIntParameter(0) == -1) {
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
        D_004360B8 = scrReadIntParameter(1);
        D_003897C0[0] = 4;
    } else {
        world = dds3GetWorldObject();
        handle = (s32)func_00110C70(world, scrReadIntParameter(0), 4);
        if (handle == 0) {
            return 1;
        }
        func_00110BE0(dds3GetWorldObject(), handle);
    }
    return 1;
}

s32 fldCmdSetRequestedSceneName(void) {
    char *name;

    name = scrReadStringParameter(0);
    D_00389770[0x14] = 5;
    strcpy((char *)D_00389770 + 0x40, name);
    return 1;
}

extern void evtSetSolarOverlayFullyTransparent(void);
extern void evtDisableSolarOverlayAlpha(void);
extern void evtEnableSolarOverlayAlpha(void);

s32 fldCmdSetSolarOverlayMode(void) {
    s32 mode = scrReadIntParameter(0);

    D_00389770[0xCC / 4] = mode;
    D_00389770[0xD0 / 4] = 0;
    if (mode < 4) {
        D_00389770[0xD4 / 4] = mode;
    }
    switch (D_00389770[0xCC / 4]) {
    case 0:
        evtSetSolarOverlayFullyTransparent();
        break;
    case 1:
        evtSetSolarOverlayFullyVisible();
        D_00389770[0x138 / 4] = 1;
        break;
    case 2:
        evtDisableSolarOverlayAlpha();
        D_00389770[0xD0 / 4] = 15;
        break;
    case 3:
        evtEnableSolarOverlayAlpha();
        D_00389770[0xD0 / 4] = 0;
        D_00389770[0x138 / 4] = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return 1;
}

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

s32 fldParseRoomNumberFromName(char *name) {
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

s32 fldCmdSetSceneControlValue(void) {
    s32 value;

    value = scrReadIntParameter(0);
    D_003897C8[0] = value;
    return 1;
}

s32 func_00155048(void) {
    s32 value;

    D_00389988[11] = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    func_00135A68(D_00389988[11], value);
    return 1;
}

s32 fldCmdSetFadeAndSway(void) {
    s32 second;
    s32 third;
    D_00389988[13] = scrReadIntParameter(0) & 0xFF;
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    fldSetFadeTarget(D_00389988[13], second, third);
    fldSetSwayMode(scrReadIntParameter(3));
    return 1;
}

extern void func_00122F38(s32, s32, s32, s32);
extern s64 func_00110628(u64);
extern s64 func_001106D8(u64);
extern u64 func_00110CD8(u64, u64);
extern void dds3DestroyWorldIndexNode(u64);
extern s32 func_001106B8(u64);
typedef struct FldWorldItem {
    u8 pad0[0x18];
    s32 *data;
} FldWorldItem;
extern FldWorldItem *func_00110680(u64);
extern void func_00114048(FldWorldItem *, s32);

s32 func_00155108(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389770[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            func_00122F38(world, stage, room, 0);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        func_00122F38(world, stage, room, 0);
    }
    if (world == D_00389770[4] && stage == D_00389770[5] + 1) {
        list = func_00110CD8(dds3GetWorldSecondaryObject(), 6);
        if (func_00110628(list) != 0) {
            func_001106B8(list);
            do {
                item = func_00110680(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        func_00114048(item, 1);
                        break;
                    case 1:
                        func_00114048(item, 5);
                        break;
                    case 2:
                        func_00114048(item, 7);
                        break;
                    }
                }
            } while (func_001106D8(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001552E0);

extern char *D_004361D4;

s32 func_00155498(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389770[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D4;
    }
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            func_00122F38(world, stage, room, 1);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        func_00122F38(world, stage, room, 1);
    }
    if (world == D_00389770[4] && stage == D_00389770[5] + 1) {
        list = func_00110CD8(dds3GetWorldSecondaryObject(), 6);
        if (func_00110628(list) != 0) {
            func_001106B8(list);
            do {
                item = func_00110680(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        func_00114048(item, 2);
                        break;
                    case 1:
                        func_00114048(item, 6);
                        break;
                    case 2:
                        func_00114048(item, 8);
                        break;
                    }
                }
            } while (func_001106D8(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155690);

s32 func_00155848(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389770[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123038(world, stage, i, 0);
        }
    } else {
        func_00123038(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == D_00389770[4]) {
        if (stage == D_00389770[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = func_00110C70(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (scrReadIntParameter(3)) {
                case 0:
                    func_00112230(obj, 0);
                    break;
                case 1:
                    func_00112230(obj, 1);
                    break;
                case 2:
                    func_00112230(obj, 2);
                    break;
                case 3:
                    func_00112230(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 func_00155A08(s32 world, s32 stage, char *name, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = D_00389770[4];
    }
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123038(world, stage, i, 0);
        }
    } else {
        func_00123038(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == D_00389770[4]) {
        if (stage == D_00389770[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = func_00110C70(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (mode) {
                case 0:
                    func_00112230(obj, 0);
                    break;
                case 1:
                    func_00112230(obj, 1);
                    break;
                case 2:
                    func_00112230(obj, 2);
                    break;
                case 3:
                    func_00112230(obj, 6);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 func_00155BA8(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;
    s32 searchId;
    void *obj;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389770[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123038(world, stage, i, 1);
        }
    } else {
        func_00123038(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == D_00389770[4]) {
        if (stage == D_00389770[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = func_00110C70(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (scrReadIntParameter(3)) {
                case 0:
                    func_00112230(obj, 3);
                    break;
                case 1:
                    func_00112230(obj, 4);
                    break;
                case 2:
                    func_00112230(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}


s32 func_00155D48(s32 world, s32 stage, char *name, s32 mode) {
    s32 i;
    s32 searchId;
    void *obj;

    if (world == 0) {
        world = D_00389770[4];
    }
    if (stage == 0) {
        stage = D_00389770[5] + 1;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123038(world, stage, i, 1);
        }
    } else {
        func_00123038(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == D_00389770[4]) {
        if (stage == D_00389770[5] + 1) {
            searchId = fldFindSearchId(name);
            if (searchId != -1) {
                obj = func_00110C70(dds3GetWorldSecondaryObject(), searchId, 6);
                switch (mode) {
                case 0:
                    func_00112230(obj, 3);
                    break;
                case 1:
                    func_00112230(obj, 4);
                    break;
                case 2:
                    func_00112230(obj, 5);
                    break;
                }
            }
        }
    }
    return 1;
}

s32 fldCmdSetSceneBits(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123138(world, stage, i, 0);
        }
    } else {
        func_00123138(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    return 1;
}

s32 func_00155F90(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (strcmp(name, D_004363F0) == 0) {
        name = D_004361D8;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_00123138(world, stage, i, 1);
        }
    } else {
        func_00123138(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    return 1;
}

s32 func_00156070(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_00123238(area, floor, target, 0);
    return 1;
}

s32 func_001560F8(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_00123238(area, floor, target, 1);
    return 1;
}

s32 func_00156180(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_00123338(area, floor, target, 0);
    return 1;
}

s32 func_00156208(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_00123338(area, floor, target, 1);
    return 1;
}

extern void fldSetMapSlotByte(s32, s32, s32, s32);

s32 fldCmdSetMapSlotByte(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    slot = scrReadIntParameter(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrReadIntParameter(3));
    return 1;
}

s32 fldCmdClearFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetFloorFlag(area, floor, target);
    return 1;
}

s32 fldCmdSetFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_00389780[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_00389784[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldClearFloorFlag(area, floor, target);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156440);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156538);

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_00389780[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_00389784[0] + 1;
    }
    room = scrReadIntParameter(2);
    if (room == 0) {
        for (; room < 16; room++) {
            func_001235E8(world, stage, room, scrReadIntParameter(3));
        }
    } else {
        func_001235E8(world, stage, room, scrReadIntParameter(3));
    }
    return 1;
}

s32 fldSetFlagFromWorld1(void) {
    if (fldGetSceneStatusCode() == 1) {
        func_0010D818(1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

s32 fldSetFlagFromWorld3(void) {
    if (fldGetSceneStatusCode() == 3) {
        func_0010D818(1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

s32 fldSetFlagFromWorld4(void) {
    if (fldGetSceneStatusCode() == 4) {
        func_0010D818(1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

s32 fldSetFlagFromWorld2(void) {
    if (fldGetSceneStatusCode() == 2) {
        func_0010D818(1);
        return 1;
    }
    func_0010D818(0);
    return 1;
}

typedef struct FldSceneParamRow {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s32 unk8;
    s32 unkC;
} FldSceneParamRow; /* 0x10 bytes */

extern FldSceneParamRow D_003A8EB0[];

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414090);

s32 fldCmdPushSceneParam(void) {
    s32 room;

    room = fldFindRoomByTask(((EffCmdWork *)func_0010D8C8())->key);
    switch (scrReadIntParameter(0)) {
    case 0:
        func_0010D818(D_003A8EB0[room].unk0);
        break;
    case 1:
        func_0010D818(D_003A8EB0[room].unk4);
        break;
    case 2:
        func_0010D818(D_003A8EB0[room].unk6);
        break;
    case 3:
        func_0010D818(D_003A8EB0[room].unk8);
        break;
    case 4:
        func_0010D818(D_003A8EB0[room].unkC);
        break;
    }
    return 1;
}

u32 fldCmdActivateTaskRoomObject(void) {
    s32 task;
    u64 room;

    task = func_0010D8C8();
    room = fldFindRoomByTask(((EffCmdWork *)task)->key);
    fldActivateFlaggedObject(room);
    return 1;
}

u32 fldCmdTestTaskRoomObjectActive(void) {
    if (fldTestObjectActivationFlag(fldFindRoomByTask(((EffCmdWork *)func_0010D8C8())->key)) != 0) {
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
    scene = fldGetTaskRecordValue(((EffCmdWork *)func_0010D8C8())->key);
    if (scene) {
        func_00140BC8(scene);
    }
    return 1;
}

u32 func_001569B8(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_001411F8(value);
    return 1;
}

u32 func_001569E0(void) {
    return 1;
}

u32 fldCmdGetActorStat0(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = fldGetActorStat0(actorStat);
    func_0010D818(actorStat);
    return 1;
}

u32 fldCmdGetActorStat1(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = func_001421C0(actorStat);
    func_0010D818(actorStat);
    return 1;
}

u32 fldCmdGetActorMotionEntry(void) {
    s32 motionEntry;

    motionEntry = scrReadIntParameter(0);
    motionEntry = fldGetActorMotionEntry(motionEntry);
    func_0010D818(motionEntry);
    return 1;
}

u32 fldCmdGetRowValue(void) {
    s32 rowValue;

    rowValue = scrReadIntParameter(0);
    rowValue = fldGetRowValue(rowValue);
    func_0010D818(rowValue);
    return 1;
}

u32 func_00156AA8(void) {
    u64 value;

    value = scrReadIntParameter(0);
    func_00142670(value);
    return 1;
}

u32 func_00156AD0(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_001206D0(first, second);
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
s32 fldCmdQuerySceneValue(void) {
    func_0010D818(func_00140780(scrReadIntParameter(0)));
    return 1;
}

u32 fldCmdUpdateTaskRecordScene(void) {
    s32 scene;
    if (func_00140B80()) {
        scene = 0;
    } else {
        scene = fldFindTaskRecordId(((EffCmdWork *)func_0010D8C8())->key);
    }
    func_00140830(scene);
    return 1;
}

void fldCmdResetAfterEvent(void) {
    fldResetAfterEvent();
}

s32 func_00156BE8(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    func_0010D818(func_0013FA98(param0, param1));
    return 1;
}

s32 func_00156C30(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    func_0010D818(func_0013FFF8(param0, param1));
    return 1;
}

void func_00156C78(void) {
    func_0010D818(func_0013F790(1));
}

s32 func_00156C98(void) {
    func_00140238();
    return 1;
}

s32 fldCmdStartSceneBgm(void) {
    D_00389798[0] = scrReadIntParameter(0);
    fldStartSceneBgm();
    return 1;
}

s32 fldCmdStartSceneBgmAlternate(void) {
    D_00389798[0] = scrReadIntParameter(0);
    fldStartSceneBgmAlternate();
    return 1;
}

s32 fldCmdStopCurrentBgm(void) {
    fldStopCurrentBgm();
    return 1;
}

s32 fldCmdPlayCurrentBgmSound(void) {
    fldPlayCurrentBgmSound();
    return 1;
}

s32 fldCmdReleaseCurrentBgm(void) {
    fldReleaseCurrentBgm();
    return 1;
}

s32 fldCommandPlaySeVolumePan(void) {
    fldPlayMenuSound(scrReadIntParameter(0));
    return 1;
}

s32 fldCommandPlaySe(void) {
    fldPlayFieldSe(scrReadIntParameter(0));
    return 1;
}

u8 fldCommandLoadArchive(void) {
    return fldPollArchiveLoad(scrReadIntParameter(0)) != 0;
}

s32 fldCmdSetArchiveSoundVolumePan(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    fldSetArchiveSoundVolumePan(param0, param1);
    return 1;
}

s32 fldCmdPlayArchiveSound(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    fldPlayArchiveSound(param0, param1);
    return 1;
}

s32 fldCommandStartTitle(void) {
    s32 param0 = scrReadIntParameter(0);
    s32 param1 = scrReadIntParameter(1);

    func_0014EC00(param0, param1, 0x3c);
    return 1;
}

s32 func_00156EB8(void) {
    s32 value;

    value = scrReadIntParameter(0);
    D_0038984C[0] = value;
    return 1;
}

s32 fldCmdApplyTaskRecordEntry(void) {
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
    func_0014BF98(func_0023CC00(scrReadIntParameter(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 fldCommandFindEffectByName(void) {
    char *param = scrReadStringParameter(0);

    func_0010D818(fldFindEffectByName(param));
    return 1;
}

s32 func_00156FA0(void) {
    func_0010D818(func_0014E620());
    return 1;
}

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommandClearSelectedFlag(void) {
    s32 changed = 0;
    switch (scrReadIntParameter(0)) {
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

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void) {
    s32 solarPhaseThreshold = D_003AA720[evtGetMirroredSolarPhase()];
    if (solarPhaseThreshold >= effMiscRandMod(0, 100)) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 fldCommandSendSoundControl(void) {
    func_00342690();
    return 1;
}

/* Return sound-command busy state to the field script interpreter. */
s32 fldCommandIsSoundBusy(void) {
    func_0010D818(sdfSoundIsCommandBusy());
    return 1;
}

/* Pass five field-script arguments to the underlying handler. */
s32 func_001571C8(void) {
    s32 first = scrReadIntParameter(0);
    s32 second = scrReadIntParameter(1);
    s32 third = scrReadIntParameter(2);
    s32 fourth = scrReadIntParameter(3);
    func_00123DE8(D_00389780[0], first, second, third, fourth, scrReadIntParameter(4));
    return 1;
}

extern s32 D_003898A4[];

s32 func_00157258(void) {
    D_003898A4[0] = 1;
    return 1;
}


typedef struct FldSceneParam {
    s32 unk0;
    s32 value;
} FldSceneParam;

extern FldSceneParam *D_00435F1C;

s32 fldCmdGetPendingSceneParam(void) {
    if (D_00435F1C == NULL) {
        func_0010D818(-1);
        return 1;
    }
    func_0010D818(D_00435F1C->value);
    return 1;
}

s32 func_001572A0(void) {
    if (scrReadIntParameter(0) == 2) {
        func_00153D40(0);
        D_003898B0[0] = 0;
    } else if (scrReadIntParameter(0) == 0) {
        func_00153D40(0);
    } else {
        func_00153D40(1);
    }
    return 1;
}

extern void func_001536B8(s32);

s32 func_00157308(void) {
    func_001536B8(scrReadIntParameter(0));
    return 1;
}

extern void func_00341348(void *);
extern void func_002D2C50(void);
extern void func_00157BE0(void);
extern void parSysReset(void);
extern void func_0015B270(void);
extern void func_00194A70(void);
extern void effInitWorks(void);
extern void effBTLFieldColorResetFlags(void);
extern void func_00157400(void);
extern void effManagerUpdateAndDispatch(void);
extern void effManagerInitializeSubsystems(void);
extern u8 D_003AA868[];
extern char D_004363F8[];

void fldCreateFieldEffectTask(void) {
    func_00341348(D_003AA868);
    func_002D2C50();
    func_00157BE0();
    parSysReset();
    func_0015B270();
    kwlnTaskCreate("effect_f", 0x2B04, 0, 0, func_00157400, NULL, 0);
    kwlnTaskCreate(D_004363F8, 0x2B18, 0, 0, effManagerUpdateAndDispatch, effManagerInitializeSubsystems, 0);
    func_00194A70();
    effInitWorks();
    effBTLFieldColorResetFlags();
}

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

