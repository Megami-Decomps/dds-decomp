#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

typedef struct FldCamPose {
    u8 pad0[0x10];
    s32 world; /* 0x10 */
    s32 stage;
    u8 pad18[0x18];
    f32 focusPos[3]; /* 0x30 */
    u8 pad3C[0x14];
    s32 unk50;
    u8 pad54[0x10];
    f32 negatedAngle;
    u8 pad68[8];
    s32 unk70;
    u8 pad74[0x4C];
    s32 unkC0;
    u8 padC4[0x66];
    s16 unk12A;
    u8 pad12C[0x14];
    f32 x;
    f32 y;
    f32 z;
    u8 pad14C[0x18];
    f32 angle;
    u8 pad168[0x34];
    struct {
        s32 value; /* fldmix.LB node value */
        s32 block; /* sdfMemoryGetBlockAddress(value) */
    } fldmix[4]; /* 0x19C */
} FldCamPose;

typedef struct FldVec3 {
    f32 x;
    f32 y;
    f32 z;
} FldVec3;
typedef struct FldVec4 {
    f32 v[4];
} FldVec4;
extern char D_003A0800[]; /* "fldTitle" */
extern char D_003A0828[]; /* "fldTitleMini" */

extern u64 func_0011E998(void);

extern s32 fldGetRowValue(s32);

extern s32 fldGetActorMotionEntry(s32);

extern s32 fldGetActorStat1(s32);

extern s32 fldGetActorStat0(s32);

extern s32 scrReadIntParameter(s32);

extern s32 func_0010D6A0(void);
extern s32 fldFindRoomByTask(u32);

extern u32 D_003BAFC8;
extern u32 D_003BAFCC;
extern u32 D_003BAFD0;
extern u32 D_003BAFD4;
extern u32 D_003BAFD8;
extern u32 D_003BAFDC;
extern u32 D_003BAFE0;
extern u32 D_003BAFE4;
extern s32 D_003BAFF4;

extern s32 D_003BAFBC;

extern s32 D_003BAFC4;

extern s32 D_003BAFB4;

extern u32 D_003BAFA0;
extern u32 D_003BAFA8;

extern u32 D_003BAFB0;

extern s32 D_003BAA00;

extern u32 D_003BAED8;

extern u32 D_003BAEDC;
extern s32 D_003BAEE0;
extern s32 D_003BAEE4;

extern u32 D_003BAEA8;

extern u32 D_003BAEA4;

extern u32 D_003BAE80;

extern u32 D_003BAE70;
extern s32 D_003BAE8C;
extern s32 D_003BAEB0;
extern s32 D_003BAEB4;
extern s32 D_0032E3B0[];
extern s32 D_003BAF8C;
extern s32 D_003BAF90;
extern s32 D_0032E474[];
extern s32 D_0033EB78[];
extern void func_002E96D8(s32 arg0);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern s32 dds3GetWorldObject(void);
extern s32 func_00110A38(s32 arg0);
extern char D_003BB000[]; /* "BARIA" */
extern char *D_003BAE44;
extern void *func_00110A48(u64, s32, s32);
extern s32 func_0013A720(s32, void *);
extern s32 func_0013A9B0(s32, void *);
extern void func_001313E0(void);
extern void dds3InvokeSlot1Handler(s32 arg0, s32 arg1);
extern void effUpdateNode(u32 arg0);
extern void fldRelocatePackedTransferChunk(u32 arg0, s32 arg1);
extern s32 func_001277A8(s32 arg0);
extern void func_00123E00(void);
extern void func_0012E9E8(s32 arg0);
extern void evtSetSolarOverlayFullyVisible(void);
extern s32 D_0032E5C4[];
extern s32 D_0032E4C4[];
extern u8 D_0034D8F0[];
extern void func_0024D9D8(void *arg0);
extern void func_0024DA58(s32 arg0);
extern void func_00123EA8(void);
extern char *scrReadStringParameter(s32 idx);
extern s32 D_0032E478[];
extern f32 D_0034C8D0[];
extern f32 D_0034C8E0[];
extern s32 D_003D62C8[];
extern void *D_003D62A4[];
extern s32 D_003D62A8[];
extern s32 D_0032E3D8[];
extern s32 D_0032E400[];
extern s32 D_0032E408[];
extern void func_002E8DD0();
extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);
extern void sndStartTrackDefault(s32 arg0);
extern void *func_002CFEB8(s32 size);
extern void func_00101A68(s32 arg0, void *arg1);
extern s32 func_00141320(void);
extern s32 D_0032E3C0[];
extern s32 D_003BAE84;
extern s32 D_0032E570[];
extern void func_001130C8(s32 arg0);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[0xC];
} FldEnt14; /* 0x14 bytes */
extern FldEnt14 D_003D62E0[];

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x10C];
} FldEnt110; /* 0x110 bytes */
extern FldEnt110 *D_003BAA48;
extern s32 func_0013C5D0(void);

typedef struct {
    s32 *unk0;
    u8 pad4[0x4C];
} FldTbl50; /* 0x50 bytes */
extern FldTbl50 D_003D46C0[];
extern s32 kwlnTaskGetTaskByName(void *name);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern u8 D_003BAE78[];
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

typedef struct {
    s32 flags;
    s32 soundId;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FldClear18; /* 0x18 bytes */
extern FldClear18 D_003D3FE0[];

extern u64 dds3GetWorldSecondaryObject(void);
extern u32 *func_00110F80(u64 world, const char *name);
extern void func_003003F0(const char *fmt, ...);

extern u64 func_00101A70(void);
extern void func_00129720(u32);
extern void func_00129900(u32);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00129000(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001411F0);

extern s32 func_00124F08(void);
extern s32 func_00125140(void);
extern s32 fldTitleIsActive(void);
extern void func_001493D0(s32, s32, s32);
extern void func_00147188(s32);
extern s32 D_003BAB08;
s32 func_00141320(void) {
    if (func_00124F08() != 0) {
        return 0;
    }
    if (func_00125140() != 0) {
        return 0;
    }
    if (fileMenuTaskExists() != 0) {
        return 0;
    }
    if (fldTitleIsActive() != 0) {
        return 0;
    }
    if (D_003BAB08 != -999) {
        func_001493D0(0x80, 0, 0);
    } else {
        func_00147188(0);
    }
    return 0;
}


void *fldFieldTaskCreate(s32 task) {
    s32 *work;

    work = func_002CFEB8(0x10);
    work[0] = 0;
    work[1] = 0;
    work[2] = 0;
    work[3] = 0;
    func_00101A68(task, work);
    return func_00141320;
}

void fldFieldTaskDestroy(void) {
    u64 work;

    work = func_00101A70();
    func_002CFF98(work);
    D_003BAE70 = 0;
}


void fldEnsureTask(void) {
    if (D_003BAE70 == 0) {
        D_003BAE70 = kwlnTaskCreate(D_003BAE78, 0x2B0B, 1, 1, fldFieldTaskCreate, fldFieldTaskDestroy, 0);
    }
}


void fldDestroyTask(void) {
    if (D_003BAE70 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BAE70, 1);
    }
}


void fldResetPendingSounds(void) {
    FldClear18 *entry = D_003D3FE0;
    s32 remaining = 7;

    do {
        remaining -= 1;
        entry->flags = 0;
        entry->x = 0;
        entry->soundId = 0;
        entry += 1;
    } while (remaining >= 0);
    D_003BAE8C = 0;
}

void fldAppendClearEntry(s32 id, f32 x, f32 y, f32 z, f32 w) {
    s32 i = D_003BAE8C;

    D_003D3FE0[i].flags = 0;
    D_003D3FE0[i].soundId = id;
    D_003D3FE0[i].x = x;
    D_003D3FE0[i].y = y;
    D_003D3FE0[i].z = z;
    D_003D3FE0[i].w = w;
    D_003BAE8C = i + 1;
}


extern s32 D_0033EAB0[];
extern s32 mdlFlagTest();

void fldPlayPendingSounds(void) {
    s32 stage;
    s32 i;

    stage = D_0032E3C0[0];
    if (stage == 11) {
        stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
    }
    for (i = 0; i < D_003BAE8C; i++) {
        if (D_003D3FE0[i].flags & 1) {
            D_003D3FE0[i].flags &= ~1;
            func_002E8DD0(D_0033EAB0[stage] + D_003D3FE0[i].soundId);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001415F8);

extern s32 D_0033EB7C[];
extern s32 D_0033EB80[];
extern s32 D_0033EB84[];
extern s32 D_0033EB88[];
extern s32 D_0033EB90[];

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0470);

s32 fldResolveSpecialBgmTrack(s32 id) {
    s32 result;

    switch (id) {
    case 0x80:
        result = D_0033EB78[0] + 1;
        break;
    case 0x81:
        result = D_0033EB7C[0] + 1;
        break;
    case 0x82:
        result = D_0033EB80[0] + 1;
        break;
    case 0x83:
        result = D_0033EB7C[0] + 2;
        break;
    case 0x84:
        result = D_0033EB84[0] + 1;
        break;
    case 0x85:
        result = D_0033EB84[0] + 2;
        break;
    case 0x86:
        result = D_0033EB88[0] + 1;
        break;
    case 0x87:
        result = D_0033EB90[0] + 1;
        break;
    case 0x88:
        result = D_0033EB90[0] + 2;
        break;
    case 0x89:
        result = D_0033EB90[0] + 3;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

typedef struct {
    s16 id;
    s16 flag;
    s8 data[0x40];
} FldEnt44; /* 0x44 bytes */
extern FldEnt44 D_0032E5C8[32];

s8 fldFindSceneEntryData(s32 id, s32 idx) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_0032E5C8[i].id == id && (D_0032E5C8[i].flag == 0 || mdlFlagTest(D_0032E5C8[i].flag) != 0)) {
            return D_0032E5C8[i].data[idx];
        }
    }
    return 0;
}

void fldStartSceneBgm(void) {
    s32 handle = fldResolveSpecialBgmTrack(D_0032E3B0[10]);
    s32 stage;
    s32 idx;

    if (handle != -1 || D_0032E3B0[4] < 0x32) {
        if (handle == -1) {
            stage = D_0032E3B0[4];
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, D_0032E3B0[5] + 1);
            D_003BAE84 = D_0033EAB0[stage];
            D_0032E3B0[10] = idx;
            handle = D_003BAE84 + idx;
        }
        if (D_0032E3B0[76] != 1) {
            if (D_003BAE80 != handle) {
                func_002E96D8(D_003BAE80);
            }
            D_003BAE80 = handle;
            sndStartTrackDefault(handle);
        }
    }
}


void fldStartTitleBgmIfSelected(void) {
    if (D_0032E3D8[0] == 0x80) {
        sndStartTrackDefault(D_0033EB78[0] + 1);
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);
extern u64 sndStartTrackAlternate(s32);
void fldStartSceneBgmAlternate(void) {
    s32 handle = fldResolveSpecialBgmTrack(D_0032E3B0[10]);
    s32 stage;
    s32 idx;

    if (handle != -1 || D_0032E3B0[4] < 0x32) {
        if (handle == -1) {
            stage = D_0032E3B0[4];
            if (stage == 11) {
                stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
            }
            idx = fldFindSceneEntryData(stage, D_0032E3B0[5] + 1);
            D_003BAE84 = D_0033EAB0[stage];
            D_0032E3B0[10] = idx;
            handle = D_003BAE84 + idx;
        }
        if (D_0032E3B0[76] != 1) {
            D_003BAE80 = handle;
            sndStartTrackAlternate(handle);
        }
    }
}

void fldStopCurrentBgm(void) {
    func_002E8D58(D_003BAE80);
    D_003BAE80 = 0;
}


void fldReleaseCurrentBgm(void) {
    func_002E96D8(D_003BAE80);
    if (D_0032E3B0[10] >= 0x80) {
        D_0032E3B0[10] = 1;
    }
    D_003BAE80 = 0;
}

void func_00141D80(void) {
    func_002E8E00();
}


void fldPlayCurrentBgmSound(void) {
    func_002E8DD0(D_003BAE80);
    D_0032E3D8[0] = 0;
}

void fldPlayFieldSeVolumePan(s32 soundId) {
    if (D_0032E3C0[0] < 50 && soundId >= 16) {
        if (soundId == 0x80) {
            sndSetSequenceVolumePan(0x6C0060, 0x7F, 0x3F);
        } else if (soundId == 0x81) {
            sndSetSequenceVolumePan(0x6C0061, 0x7F, 0x3F);
        } else if (soundId == 0x82) {
            sndSetSequenceVolumePan(0x6C0064, 0x7F, 0x3F);
        } else {
            sndSetSequenceVolumePan(D_003BAE84 + soundId, 0x7F, 0x3F);
        }
    }
}

void fldPlayFieldSe(s32 soundId) {
    if (D_0032E3C0[0] < 50) {
        if (soundId == 0x100) {
            func_002E8DD0(0x6C0060);
        } else if (soundId == 0x101) {
            func_002E8DD0(0x6C0061);
        } else if (soundId == 0x102) {
            func_002E8DD0(0x6C0064);
        } else {
            func_002E8DD0(D_003BAE84 + soundId);
        }
    }
}


void fldSetSequenceVolume(s32 category, s32 volume) {
    if (D_0032E3C0[0] < 0x32) {
        sndSetSequenceVolumePan(D_003BAE84 + category, volume, 0x3F);
    }
}

extern s32 D_003BAE98;
extern s32 D_0033EAE8[];
extern void func_002E96D8();

void fldSetBgmMode(s32 mode) {
    switch (mode) {
    case -1:
        if (D_003BAE98 != mode) {
            func_002E8DD0(D_003BAE98);
            D_003BAE98 = mode;
        }
        break;
    case 0:
        mode = -1;
        if (D_003BAE98 != mode) {
            func_002E96D8(D_003BAE98);
            D_003BAE98 = mode;
        }
        break;
    case 1:
        mode = D_0033EB78[0] + 1;
        sndStartTrackDefault(mode);
        D_003BAE98 = mode;
        break;
    case 2:
        mode = D_0033EAE8[0] + 1;
        sndStartTrackDefault(mode);
        D_003BAE98 = mode;
        break;
    }
}

extern s32 fldResolveSpecialBgmTrack(s32);
extern s32 D_003BAE9C;
extern s32 D_003BAEA0;
void fldPrepareSceneBgmArchive(void) {
    s32 handle = fldResolveSpecialBgmTrack(D_0032E3B0[10]);
    s32 stage;
    s32 idx;

    if (handle == -1) {
        if (D_0032E3B0[4] >= 0x32) {
            return;
        }
        stage = D_0032E3B0[4];
        if (stage == 11) {
            stage = mdlFlagTest(0x28) != 0 ? 2 : stage;
        }
        idx = fldFindSceneEntryData(stage, D_0032E3B0[5] + 1);
        D_003BAE84 = D_0033EAB0[stage];
        D_0032E3B0[10] = idx;
        handle = D_003BAE84 + idx;
    }
    D_003BAE9C = handle;
    D_003BAEA0 = 0;
}

extern s32 func_002E92C0(s32);
extern void func_002E9340(s32);
s32 fldStepSceneBgmArchive(void) {
    switch (D_003BAEA0) {
    case 0:
        if (func_002E92C0(D_003BAE9C) == 0) {
            func_002E9340(D_003BAE9C);
            D_003BAEA0 = D_003BAEA0 + 1;
        } else {
            D_003BAEA0 = D_003BAEA0 + 2;
        }
        break;
    case 1:
        if (func_002E92C0(D_003BAE9C) == 1) {
            D_003BAEA0 = D_003BAEA0 + 1;
        }
        break;
    default:
        D_003BAEA0 = -1;
        return 1;
    }
    return 0;
}

u32 fldGetArchiveLoadPending(void) {
    return D_003BAEA4;
}


s32 fldPollArchiveLoad(s32 id) {
    s32 name = 0x30000000 + (id << 16);
    s32 result = func_002E92C0(name);
    if (result == 0) {
        func_002E9340(name);
        D_003BAEA4 = 1;
        return 0;
    }
    if (result == 1) {
        D_003BAEA4 = 0;
        return 1;
    }
    return 0;
}

void fldSetArchiveSoundVolumePan(s32 arg0, s32 arg1) {
    sndSetSequenceVolumePan(arg0 * 0x10000 + arg1 + 0x30000000, 0x7f, 0x3f);
}

void fldPlayArchiveSound(s32 arg0, s32 arg1) {
    func_002E8DD0(arg0 * 0x10000 + arg1 + 0x30000000);
}

void fldResetArchiveLoadPhase(void) {
    D_003BAEA8 = 0;
}

s32 fldStepArchiveLoad(void) {
    switch (D_003BAEA8) {
    case 0:
        if (func_002E92C0(0x670000) == 0) {
            func_002E9340(0x670000);
            D_003BAEA8 = D_003BAEA8 + 1;
        } else {
            D_003BAEA8 = D_003BAEA8 + 2;
        }
        break;
    case 1:
        if (func_002E92C0(0x670000) == 1) {
            D_003BAEA8 = D_003BAEA8 + 1;
        }
        break;
    default:
        D_003BAEA8 = -1;
        return 1;
    }
    return 0;
}

u32 fldGetCurrentBgmHandle(void) {
    return D_003BAE80;
}

extern s32 func_00121818(s32, s32);
extern s32 func_00121870(s32, s32);
extern s32 fldFindMapCoordinateIndex(s32, s32);
extern s32 strlen(const char *);
extern char D_0033F06C[][0x1C], D_0034286C[][0x1C], D_0034606C[][0x1C];
extern char D_0033EC90[][0x18];
extern s16 D_003D43B0[], D_003D4408[], D_003D4460[];
extern s16 D_003BD7D0;
void fldCacheMapLabelLengths(s32 arg0) {
    s32 i;

    for (i = 0; i < 41; i++) {
        D_003D43B0[i] = strlen(D_0033F06C[func_00121818(arg0, i)]);
    }
    for (i = 0; i < 24; i++) {
        D_003D4460[i] = strlen(D_0034286C[func_00121870(arg0, i)]);
    }
    for (i = 0; i < 41; i++) {
        D_003D4408[i] = strlen(D_0034606C[fldFindMapCoordinateIndex(arg0, i)]);
    }
    i = 0;
    if (D_0032E3C0[0] < 100) {
        i = D_0032E3C0[0];
    }
    D_003BD7D0 = strlen(D_0033EC90[i]);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142408);


void fldSetSceneRecordChunk(s32 arg0, s32 arg1) {
    /* Descriptor from func_001277A8 precedes the 0x14-byte scene rows. */
    typedef struct {
        u8 pad00[4];
        s32 rows; /* 0x04: first scene row */
        s32 count; /* 0x08: number of scene rows */
    } SceneHeader;
    if (D_0032E3C0[0] < 0xC8) {
        s32 source = arg0;
        s32 resource = arg1;
        s32 transferStart = source + 8;

        D_003BAEE4 = resource;
        fldRelocatePackedTransferChunk(arg0, transferStart);
        {
            s32 header = func_001277A8(transferStart);
            s32 rows = ((SceneHeader *)header)->rows;
            s32 count = ((SceneHeader *)header)->count;

            D_003BAEE0 = count;
            D_003BAEDC = rows;
        }
    }
}


void fldInitSceneMapLabels(void) {
    s32 sceneId = D_0032E3C0[0];

    if (sceneId < 0xC8) {
        fldCacheMapLabelLengths(sceneId % 100);
        func_00142408();
    }
}

void fldReleaseSceneRecordChunk(void) {
    if (D_003BAEE4 != 0) {
        func_002D0A10(D_003BAEE4);
    }
    D_003BAEE4 = 0;
    D_003BAEDC = 0;
    D_003BAEE0 = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001426E0);

typedef struct {
    u8 pad0[0xC];
    s32 *unkC;
} FldEmitterRes;

typedef struct {
    FldEmitterRes *res;
    u8 pad4[0x4C];
    f32 pos[4];
} FldEmitter;

extern u8 D_00325838[];
extern void func_002D8600();
extern void sdfModelUpdateCurrentFrameTransforms();
extern void func_002D9238();

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
    func_002D8600(handle);
    sdfModelUpdateCurrentFrameTransforms(emitter);
    func_002D9238(D_00325838, emitter);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142800);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001428C0);

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
    s32 model; /* 0xC */
    FldPoint *pos; /* 0x10 */
} FldSceneRecord; /* 0x14 bytes */

s32 fldFindRecordItem(s32 scene, u32 index) {
    s32 result = 1;
    FldSceneRecord *rec = (FldSceneRecord *)D_003BAEDC;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)D_003BAEE0; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (i == scene && j == index) {
                result = item->value + 1;
            }
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142C78);

s32 fldGetMaxItemValue(void) {
    s32 max = 0;
    FldSceneRecord *rec = (FldSceneRecord *)D_003BAEDC;
    s32 i;
    u32 j;
    FldItem *item;

    for (i = 0; i < (s32)D_003BAEE0; i++, rec++) {
        item = rec->items;
        for (j = 0; j < rec->count; j++, item++) {
            if (max < item->value) {
                max = item->value;
            }
        }
    }
    return max + 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00142D78);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001447D0);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00144D30);

extern void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z);

typedef struct FldFogParams {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s32 unk10;
} FldFogParams;
extern FldFogParams D_00324B30;
extern u128 D_00324A20;
extern u128 D_00324A30;
extern u128 D_00324A40;
extern s32 D_003BAEB8;
extern s32 D_003BAEBC;
extern s32 D_003BAEC0;
extern s32 D_003BAEC4;
extern s32 D_003BAEC8;
extern s32 D_003BAED0;
extern s32 D_003BAECC;
extern s32 D_003BAED4;
INCLUDE_ASM(const s32, "game/code_001411F0", func_00145B18);

extern u32 D_003D40A0[];
extern void sdfTexReleaseReferenceViaHandler();
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void func_00195548(s32);
void fldReleaseTitleSlots(void) {
    if (D_003D40A0[2] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[2]);
    }
    if (D_003D40A0[3] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[3]);
    }
    D_003D40A0[2] = 0;
    D_003D40A0[3] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    D_003BAED8 = 0;
    func_00195548(0x54);
}

extern u32 D_00348F30[];
typedef struct FldSlot {
    u32 unk_0;
    u32 unk_4;
    u32 unk_8;
} FldSlot;
extern FldSlot D_003D40B0[];
extern void sdfReleaseDevSlot(u32, s32, s32);
void fldReleaseMenuSlots(void) {
    s32 i;

    for (i = 0; i < D_003BAEE0; i++) {
        if (D_00348F30[i] != 0) {
            sdfReleaseDevSlot(D_00348F30[i], 1, 1);
        }
    }
    for (i = 0; i < 64; i++) {
        D_00348F30[i] = 0;
        D_003D40B0[i].unk_0 = 0;
        D_003D40B0[i].unk_4 = 0;
        D_003D40B0[i].unk_8 = 0;
    }
    if (D_003D40A0[0] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[0]);
    }
    D_003D40A0[0] = 0;
    if (D_003D40A0[1] != 0) {
        sdfTexReleaseReferenceViaHandler(D_003D40A0[1]);
    }
    D_003D40A0[1] = 0;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
        sdfWaitSlotReady();
    }
    D_003BAED8 = 0;
}

extern void func_00125DA8(s32);
extern f32 D_00324980[];
extern FldVec4 D_003A05D8[]; /* default camera up vectors (3 copies), the first still read by asm func_00145B18 */
extern void func_00133960(void);
extern void func_00110860(s32, s32);

/* Enters the field camera state for a fresh scene: releases the title slots and
 * centers the camera on the scene's entry point. */
void fldEnterSceneCamera(void) {
    FldCamPose *cam = (FldCamPose *)D_0032E3B0;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;

    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseTitleSlots();
    D_003BAED8 = 0;
    func_00125DA8(1);
    func_00123E00();
    evtSetSolarOverlayFullyVisible();
    func_00133960();
    cam->unk70 = 4;
    func_00195548(0x54);
    func_00110860(dds3GetWorldObject(), 1);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->stage;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[1], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, &up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x108010;
    D_00324980[4] = 2244.0f;
    D_00324980[5] = 2118.0f;
}

extern s32 sdfModelCreateWithAlternateItems(s32, s32);
extern s32 func_002D3288();

/* Loads the scene's models into the menu slots, then centers the camera on the
 * scene's entry point. */
void fldLoadSceneModelsAndCamera(void) {
    FldCamPose *cam;
    FldSceneRecord *rec;
    f32 focus[4];
    f32 eye[4];
    FldVec4 up;
    f32 entry[2];
    s32 ix;
    s32 iz;
    s32 cx;
    s32 cz;
    s32 i;

    rec = (FldSceneRecord *)D_003BAEDC;
    for (i = 0; i < D_003BAEE0; i++, rec++) {
        D_00348F30[i] = sdfModelCreateWithAlternateItems(0, rec->model);
        ((FldPoint *)D_003D40B0)[i].x = rec->pos->x;
        ((FldPoint *)D_003D40B0)[i].y = rec->pos->y;
        ((FldPoint *)D_003D40B0)[i].z = rec->pos->z;
    }
    cam = (FldCamPose *)D_0032E3B0;
    D_003D40A0[0] = func_002D3288(cam->fldmix[0].block);
    D_003D40A0[1] = func_002D3288(cam->fldmix[1].block);
    D_003BAED4 = 0;
    D_003BAEB4 = cam->stage;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    cx = cam->x;
    cz = cam->z;
    iz = cz + entry[1];
    ix = cx + entry[0];
    memcpy(&up, &D_003A05D8[2], sizeof(up));
    D_003BAECC = 0;
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, &up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x108010;
    D_00324980[4] = 2244.0f;
    D_00324980[5] = 2118.0f;
}

void fldReleaseMenuSlotsAfterWait(void) {
    sdfWaitSlotReady();
    sdfWaitSlotReady();
    fldReleaseMenuSlots();
}

u32 fldGetSceneReadyFlag(void) {
    return D_003BAED8;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_001462D8);

/* Re-centers the scene camera on the current scene's entry point. */
INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A05D8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0608);

void fldCenterCameraOnEntry(void) {
    FldCamPose *cam = (FldCamPose *)D_0032E3B0;
    f32 focus[4];
    f32 eye[4];
    f32 up[4] = {0.0f, 0.0f, -1.0f, 1.0f};
    f32 entry[2];
    s32 ix;
    s32 iz;

    D_003BAEB4 = cam->stage;
    D_003BAEB8 = cam->unkC0;
    D_003BAED0 = cam->unkC0;
    fldGetSceneEntryPosition(cam->stage, &entry[0], &entry[1]);
    ix = (f32)(s32)cam->x + entry[0];
    iz = (f32)(s32)cam->z + entry[1];
    D_003BAEC8 = iz;
    D_003BAEC4 = ix;
    D_003BAEBC = ix;
    D_003BAEC0 = iz;
    focus[0] = ix;
    focus[1] = 0.0f;
    focus[2] = iz;
    eye[0] = ix;
    eye[1] = -24000.0f;
    eye[2] = iz;
    PCP_COPY_VECTOR(&D_00324A30, eye);
    PCP_COPY_VECTOR(&D_00324A20, focus);
    PCP_COPY_VECTOR(&D_00324A40, up);
    D_00324B30.unk0 = 255.0f;
    D_00324B30.unk8 = 1000.0f;
    D_00324B30.unk4 = 255.0f;
    D_00324B30.unkC = 20000.0f;
    D_00324B30.unk10 = 0x808080;
}


void fldSetSceneLocation(s32 arg0, s32 arg1, s32 arg2) {
    D_0032E3B0[4] = arg0;
    D_0032E3B0[6] = arg2;
    D_003BAEB4 = D_0032E3B0[5] = arg1;
    D_003BAEB0 = arg0 % 100;
}


/* DDS1 floor flags start at +0x13F70; DDS2 stores them elsewhere. */
typedef struct FldAreaFlagsView {
    u8 pad00[0x13F70];
    u64 areaFlags[13][64];
} FldAreaFlagsView;

extern s32 D_0032C900[];

/* The area's last two digits map to a save-table row; floor and bit are zero-based. */
s32 fldGetFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex = D_0032C900[area % 100];
    if (areaIndex == -1) return 0;
    return (((FldAreaFlagsView *)D_003BAA00)->areaFlags[areaIndex][floor] >> bit) & 1;
}

/* Set a one-based flag on the current floor, and retain the associated record. */
void fldSetFlagAndFindRecord(s32 flagNumber) {
    s32 areaIndex;
    s32 bit;

    if (flagNumber > 0) {
        areaIndex = D_0032C900[D_003BAEB0 % 100];
        if (areaIndex != -1) {
            bit = flagNumber - 1;
            D_0032E3B0[6] = bit;
            D_0032E3B0[0x2F] = flagNumber;
            ((FldAreaFlagsView *)D_003BAA00)->areaFlags[areaIndex][D_0032E3B0[5]] |= 1ULL << bit;
            D_0032E3B0[0x30] = fldFindRecordItem(D_0032E3B0[5], bit);
        }
    }
}


/* The public setters take one-based floor and flag numbers. */
void fldSetFlagBit(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        ((FldAreaFlagsView *)D_003BAA00)->areaFlags[areaIndex][floor] |= 1ULL << bit;
    }
}

void fldClearFloorFlag(s32 area, s32 floor, s32 bit) {
    s32 areaIndex;

    floor--;
    bit--;
    areaIndex = D_0032C900[area % 100];
    if (areaIndex != -1) {
        ((FldAreaFlagsView *)D_003BAA00)->areaFlags[areaIndex][floor] &= ~(1ULL << bit);
    }
}


void func_00146CA8(s32 arg0) {
    if (arg0 >= 0x40) {
        D_0032E474[0] = -1;
    } else {
        D_0032E474[0] = arg0;
    }
}


void func_00146CD0(s32 arg0) {
    if (arg0 >= 0x40) {
        D_0032E478[0] = -1;
    } else {
        D_0032E478[0] = arg0;
    }
}

typedef struct {
    u8 pad0[8];
    u32 count;
    u8 padC[8];
} FldSceneEntry; /* 0x14 bytes */
extern s32 fldFindRecordItem(s32 scene, u32 index);

s32 fldFindPreviousMarkedValue(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = -1;
    FldSceneEntry *entry = (FldSceneEntry *)D_003BAEDC;

    for (i = 0; i < (s32)D_003BAEE0; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(D_003BAEB0, i, j) != 0) {
                item = fldFindRecordItem(i, j);
                if (item < limit && best < item) {
                    best = item;
                }
            }
        }
    }
    if (best == -1) {
        return limit;
    }
    return best;
}

s32 fldFindNextMarkedValue(s32 limit) {
    s32 i;
    u32 j;
    s32 item;
    s32 best = 999;
    FldSceneEntry *entry = (FldSceneEntry *)D_003BAEDC;

    for (i = 0; i < (s32)D_003BAEE0; i++, entry++) {
        for (j = 0; j < entry->count; j++) {
            if (fldGetFloorFlag(D_003BAEB0, i, j) != 0) {
                item = fldFindRecordItem(i, j);
                if (limit < item && item < best) {
                    best = item;
                }
            }
        }
    }
    if (best == 999) {
        return limit;
    }
    return best;
}


void fldGetSceneEntryPosition(s32 index, f32 *x, f32 *z) {
    s32 i;
    s32 entry = D_003BAEDC;
    for (i = 0; i < (s32)D_003BAEE0; i++, entry += 0x14) {
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

extern s32 D_003BAEAC;

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
    rec = (FldSceneRecord *)D_003BAEDC;
    for (; room < D_003BAEE0; room++, rec++) {
        item = rec->items;
        for (i = 0; i < rec->count; i++, item++) {
            visible = 0;
            if (fldGetFloorFlag(D_003BAEB0, room, i) != 0) {
                visible = 1;
            }
            if (D_003BAEAC != 0) {
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


INCLUDE_ASM(const s32, "game/code_001411F0", fldCheckSceneReady);


void func_001470E0(void) {
    if (D_003BAED8 == 1) {
        func_001447D0();
        func_00142D78();
    }
}

void fldResetCameraAndSceneView(void) {
    fldCenterCameraOnEntry();
    func_00144D30();
}

void fldClearAllAreaFloorFlags(void) {
    u64 *words;
    s32 remaining;
    s32 block;
    s32 index;

    index = 0;
    block = D_003BAA00;
    do {
        words = (u64 *)(block + 0x13f70);
        remaining = 0x3f;
        do {
            remaining = remaining - 1;
            *words = 0;
            words = words + 1;
        } while (-1 < remaining);
        index = index + 1;
        block = block + 0x200;
    } while (index < 0xd);
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147188);

extern s32 D_0034C870[];
extern s32 D_0034C880[];
extern s32 D_0034C890[];
extern s32 D_0034C8A0[];
extern s32 func_002EB028();
extern s32 func_0014FE28();
extern char *D_003A06B0[4]; /* {"/fld/f/bin/KUT_3Z.EPL", "ASI_2MZ", "ASI_2HZ", "MAN_2Z"} */
void fldLoadFieldEffectTextureSlots(void) {
    char *names[4];
    s32 i;

    memcpy(names, D_003A06B0, sizeof(names));
    for (i = 0; i < 4; i++) {
        D_0034C870[i] = func_002EB028(names[i], &D_0034C880[i], 0);
        D_0034C890[i] = func_0014FE28(D_0034C880[i]);
        D_0034C8A0[i] = 0;
    }
}

extern s32 D_0034C870[4];
extern s32 D_0034C880[4];
extern s32 D_0034C890[4];

void fldReleaseTextureSlots(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_0034C890[i] != 0) {
            effDestroyNode(D_0034C890[i]);
            D_0034C890[i] = 0;
            func_002D0A10(D_0034C870[i]);
            D_0034C870[i] = 0;
            D_0034C880[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147638);

extern s32 D_0034C8B0[];
extern s32 D_0034C8C0[];
extern s32 D_003BAF88;
void mnuInitializeResourceEntries(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        D_0034C8B0[i] = func_0014FE28(D_003BAF88);
        D_0034C8C0[i] = 0;
    }
}

extern void func_0014FB38(s32 handle);
extern void func_0014FBF0(s32 handle, f32 *pos);
extern s32 D_003BAF94;
void mnuSpawnResourceAtPosition(f32 x, f32 y, f32 z) {
    f32 pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    s32 handle;

    handle = D_0034C8B0[D_003BAF94];
    if (handle != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_0014FB38(handle);
        func_0014FBF0(D_0034C8B0[D_003BAF94], pos);
        D_0034C8C0[D_003BAF94] = 1;
        D_003BAF94 = (D_003BAF94 + 1) % 4;
    }
}


void fldQueuePrimaryEffectPosition(f32 arg0, f32 arg1, f32 arg2) {
    D_003BAF8C = 1;
    D_0034C8D0[0] = arg0;
    D_0034C8D0[1] = arg1;
    D_0034C8D0[2] = arg2;
}


void fldQueueSecondaryEffectPosition(f32 arg0, f32 arg1, f32 arg2) {
    D_003BAF90 = 1;
    D_0034C8E0[0] = arg0;
    D_0034C8E0[1] = arg1;
    D_0034C8E0[2] = arg2;
}

void mnuReleaseResourceEntries(void) {
    s32 i;
    D_003BAF8C = 0;
    D_003BAF90 = 0;
    for (i = 0; i < 4; i++) {
        if (D_0034C8B0[i] != 0) {
            effDestroyNode(D_0034C8B0[i]);
            D_0034C8B0[i] = 0;
        }
    }
}

void func_00147938(void) {
    s32 index;
    for (index = 0; index < 4; index++) {
        if (D_0034C8B0[index] != 0 && D_0034C8C0[index] != 0) {
            effUpdateNode(D_0034C8B0[index]);
        }
    }
}

extern void *func_00288A80(const char *);
extern void func_00288C50(void *);
extern void func_00288788(void *);
extern s32 sdfMemoryGetBlockAddress(s32);
extern s32 D_003BD7EC, D_003BD7F0, D_003BAF68, D_003BAF6C, D_003BAF50, D_003BAF54;
extern s32 D_003BAF74, D_003BAF78, D_003BAF5C, D_003BAF60, D_003BAF84, D_003BAF88;
extern s32 D_003BAF44, D_003BAF48, D_003BD7D4, D_003BD7D8, D_003BD7DC, D_003BD7E0;
extern s32 D_003BD7E4, D_003BD7E8;

typedef struct FldLbNode {
    struct FldLbNode *next; /* 0x00 */
    u8 unk04[4];
    s32 value;              /* 0x08 */
} FldLbNode;

typedef struct FldLbFile {
    u8 unk00[0x60];
    FldLbNode *nodes; /* 0x60 */
} FldLbFile;

#define FLD_WORK ((FldCamPose *)D_0032E3B0)

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0650);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0668);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0680);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0698);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A06B0);

void fldParseMixLb(void) {
    FldLbFile *lb;
    FldLbNode *node;
    u32 index;
    s32 value;

    index = 0;
    lb = func_00288A80("/fld/f/bin/fldmix.LB");
    func_00288C50(lb);
    for (node = lb->nodes; node != NULL; node = node->next, index++) {
        switch (index) {
        case 0:
            value = node->value;
            D_003BD7EC = value;
            D_003BD7F0 = sdfMemoryGetBlockAddress(value);
            break;
        case 1:
            value = node->value;
            D_003BAF68 = value;
            D_003BAF6C = sdfMemoryGetBlockAddress(value);
            break;
        case 2:
            value = node->value;
            D_003BAF50 = value;
            D_003BAF54 = sdfMemoryGetBlockAddress(value);
            break;
        case 3:
            value = node->value;
            D_003BAF74 = value;
            D_003BAF78 = sdfMemoryGetBlockAddress(value);
            break;
        case 4:
            value = node->value;
            D_003BAF5C = value;
            D_003BAF60 = sdfMemoryGetBlockAddress(value);
            break;
        case 5:
            value = node->value;
            D_003BAF84 = value;
            D_003BAF88 = sdfMemoryGetBlockAddress(value);
            break;
        case 6:
            value = node->value;
            D_003BAF44 = value;
            D_003BAF48 = sdfMemoryGetBlockAddress(value);
            break;
        case 7:
            value = node->value;
            D_003BD7D4 = value;
            D_003BD7D8 = sdfMemoryGetBlockAddress(value);
            break;
        case 8:
            value = node->value;
            D_003BD7DC = value;
            D_003BD7E0 = sdfMemoryGetBlockAddress(value);
            break;
        case 9:
            value = node->value;
            D_003BD7E4 = value;
            D_003BD7E8 = sdfMemoryGetBlockAddress(value);
            break;
        case 10:
            value = node->value;
            FLD_WORK->fldmix[0].value = value;
            FLD_WORK->fldmix[0].block = sdfMemoryGetBlockAddress(value);
            break;
        case 11:
            value = node->value;
            FLD_WORK->fldmix[1].value = value;
            FLD_WORK->fldmix[1].block = sdfMemoryGetBlockAddress(value);
            break;
        case 12:
            value = node->value;
            FLD_WORK->fldmix[2].value = value;
            FLD_WORK->fldmix[2].block = sdfMemoryGetBlockAddress(value);
            break;
        case 13:
            value = node->value;
            FLD_WORK->fldmix[3].value = value;
            FLD_WORK->fldmix[3].block = sdfMemoryGetBlockAddress(value);
            break;
        }
    }
    func_00288788(lb);
}

extern s32 D_003BAF48, D_003BAF4C, D_003BAF40, D_003BAF3C;
extern s32 D_003BAF30, D_003BAF34, D_003BAF38;
extern s32 D_003BD7F0, D_003BD7D8, D_003BD7E0, D_003BD7E8;
extern s32 func_0014FD20(s32);
extern s32 func_002D3288(s32);
void fldInitializeMenuResources(void) {
    if (D_0032E3C0[0] < 200) {
        D_003BAF4C = func_0014FD20(D_003BAF48);
        D_003BAF3C = func_0014FD20(D_003BD7F0);
        D_003BAF40 = 0;
        D_003BAF30 = func_002D3288(D_003BD7D8);
        D_003BAF34 = func_002D3288(D_003BD7E0);
        D_003BAF38 = func_002D3288(D_003BD7E8);
    }
}

extern void sdfTexReleaseReferenceViaHandler();
extern s32 D_003BAF30;
extern s32 D_003BAF34;
extern s32 D_003BAF38;
extern s32 D_003BAF3C;
extern s32 D_003BAF4C;
extern s32 D_003BAF58;
extern s32 D_003BAF64;

void fldReleaseResourceHandles(void) {
    if (D_003BAF30 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF30);
        D_003BAF30 = 0;
    }
    if (D_003BAF34 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF34);
        D_003BAF34 = 0;
    }
    if (D_003BAF38 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAF38);
        D_003BAF38 = 0;
    }
    if (D_003BAF4C != 0) {
        effDestroyNode(D_003BAF4C);
        D_003BAF4C = 0;
    }
    if (D_003BAF58 != 0) {
        effDestroyNode(D_003BAF58);
        D_003BAF58 = 0;
    }
    if (D_003BAF64 != 0) {
        effDestroyNode(D_003BAF64);
        D_003BAF64 = 0;
    }
    if (D_003BAF3C != 0) {
        effDestroyNode(D_003BAF3C);
        D_003BAF3C = 0;
    }
}


typedef struct {
    s32 unk0, unk4, unk8, unkC;
    u8 pad10[0x10];
    s32 *unk20;
    u8 pad24[8];
    s32 unk2C, unk30, unk34, unk38;
    s16 unk3C, unk3E;
    char name[0x10];
} FldTblEnt50; /* 0x50 bytes */
extern FldTblEnt50 D_003D46A0[];
extern s32 D_003BAF2C;
void fldClearMenuEntries(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        D_003D46A0[i].unk0 = 0;
        D_003D46A0[i].unk4 = 0;
        D_003D46A0[i].unk8 = 0;
        D_003D46A0[i].unkC = 0;
        D_003D46A0[i].unk20 = 0;
        D_003D46A0[i].unk2C = 0;
        D_003D46A0[i].unk30 = 0;
        D_003D46A0[i].unk34 = 0;
        D_003D46A0[i].unk38 = 0;
        D_003D46A0[i].unk3C = 0;
        D_003D46A0[i].unk3E = 0;
    }
    D_003BAF2C = 0;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0x14];
    s32 unk24;
    u8 pad28[8];
} FldObj30; /* 0x30 bytes */
extern FldObj30 D_003D50A0[32];
extern s32 D_003BAF70;
extern s32 D_003BAF7C;
extern s32 D_003BAF80;

void fldResetObjectSlots(void) {
    s32 i;

    D_003BAF80 = 0;
    for (i = 0; i < 32; i++) {
        D_003D50A0[i].unk4 = -1;
        D_003D50A0[i].unk0 = 0;
        D_003D50A0[i].unk8 = 0;
        D_003D50A0[i].unkC = 0;
        if (D_003D50A0[i].unk24 != 0) {
            effDestroyNode(D_003D50A0[i].unk24);
        }
        D_003D50A0[i].unk24 = 0;
    }
    D_003BAF70 = 0;
    D_003BAF7C = 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00147DB0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0718);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0728);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001480D0);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0758);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0768);

INCLUDE_ASM(const s32, "game/code_001411F0", func_001486D0);

void fldActivateObjectById(s32 id) {
    s32 i;

    for (i = 0; i < D_003BAF80; i++) {
        if (D_003D50A0[i].unk4 == id && D_003D50A0[i].unk8 == 0) {
            D_003D50A0[i].unkC = 1;
        }
    }
}

void fldReleaseObjectSlots(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (D_003D50A0[i].unk24 != 0) {
            effDestroyNode(D_003D50A0[i].unk24);
            D_003D50A0[i].unk24 = 0;
        }
    }
    D_003BAF80 = 0;
    for (i = 0; i < 32; i++) {
        D_003D50A0[i].unk4 = -1;
        D_003D50A0[i].unk0 = 0;
        D_003D50A0[i].unk8 = 0;
        D_003D50A0[i].unkC = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148D78);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0788);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0798);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00148FF0);

extern s32 D_0032E4C8[];
extern f32 D_003BAF98;
extern s32 ptyAnyUnitFlagMatch(s32, s32);
extern f32 sdfSinPoly(f32);
extern void func_00129178(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, u32);
void func_001493D0(s32 alpha, s32 x, s32 y) {
    u32 color;

    if (D_0032E4C8[0] != 1) {
        func_00129720(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        func_00129000(x + 0x123, y, 0x7D, 0x34, 1, 0x31, 0x7D, 0x34, 0x80808080, D_003BAF30);
        func_00129000(x + 0x1A0, y, 0x62, 0x34, 0x7D, 0x31, 1, 0x34, 0x80808080, D_003BAF30);
        if (ptyAnyUnitFlagMatch(0x5D0, 0) != 0) {
            color = 0x808080;
            func_00129000(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, 0x80808080, D_003BAF30);
            if (D_003BAF98 < 45.0f) {
                color = (s32)(sdfSinPoly(D_003BAF98 * 4.0f * 3.14f / 180.0f) * 128.0f) + 0x80;
                color |= (color << 8) | (color << 16);
            }
            func_00129900(1);
            func_00129178(x + 0x17F, y + 0x28, 0x73, 0x1A, 1, 0x66, 0x73, 0x1A, color, color | 0x5A000000, color | 0x5A000000, color, D_003BAF30);
            D_003BAF98 += 1.0f;
            if (D_003BAF98 > 90.0f) {
                D_003BAF98 = 0.0f;
            }
        }
        func_00129900(0);
    }
}

void fldDrawGaugeBar(s32 width) {
    s32 x;

    if (D_0032E3C0[0] < 200) {
        func_00129720(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        x = 0x9D - (width >> 1);
        func_00129000(x, 0x123, 0x63, 0x2E, 2, 1, 0x63, 0x2E, 0x80808080, D_003BAF30);
        func_00129000(x + 0x63, 0x123, width, 0x2E, 0x64, 1, 1, 0x2E, 0x80808080, D_003BAF30);
        x += width;
        func_00129000(x + 0x63, 0x123, 0x63, 0x2E, 0x65, 1, -0x63, 0x2E, 0x80808080, D_003BAF30);
        func_00129900(0);
    }
}

extern void func_00129720(u32);
extern void func_00129900(u32);
extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00129000(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

void fldDrawTitleBanner(s32 x, s32 y) {
    if (D_0032E3C0[0] < 200) {
        func_00129720(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
        func_00129000(x, y, 0x10, 0x11, 0x68, 1, 0x10, 0x11, 0x80808080, D_003BAF30);
        func_00129900(0);
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149810);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149A98);

INCLUDE_ASM(const s32, "game/code_001411F0", func_00149B68);

extern s32 D_003BAF2C;
extern s32 func_001215F8();
extern void dds3SetObjectFlags();

void fldFireRoomEffects(void) {
    s32 i;

    for (i = 0; i < D_003BAF2C; i++) {
        s32 room = D_003D46A0[i].unk38;
        if (room != 0 && func_001215F8(D_0032E3B0[4], D_0032E3B0[5] + 1, room) != 0) {
            if (D_003D46A0[i].unk20 != 0) {
                dds3SetObjectFlags(D_003D46A0[i].unk20, 1);
            }
        }
    }
}

extern u8 D_003D44A0[0x200];
extern void fldFormatAreaDirectory(char *, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 func_002E5C68(u32, void *, u32);
extern void func_002E5C38(u32);

void fldLoadNpcPalette(s32 field) {
    char path[64];
    char directory[32];
    u32 command;

    if (field < 100) {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, "%sF%03d.NPL", directory, field);
        command = sdfDevCreateCommandState(path);
        func_002E5C68(command, D_003D44A0, 0x200);
        func_002E5C38(command);
    }
}

void fldSetNpcPalette(void *src) {
    memcpy(D_003D44A0, src, 0x200);
}

extern int strcmp(const char *, const char *);
s32 fldFindEffectByName(const char *name) {
    s32 i;

    for (i = 0; i < D_003BAF2C; i++) {
        if (D_003D46A0[i].unk0 != 999 && strcmp(D_003D46A0[i].name, name) == 0) {
            return D_003D46A0[i].unk20[1];
        }
    }
    return 0;
}


s32 func_0014A250(void) {
    s32 index = func_0013C5D0();

    if (index < 0) {
        return 0;
    }
    return D_003D46C0[index].unk0[1];
}

void func_0014A298(u32 arg0) {
    D_003BAFB0 = arg0;
}


s32 fldTitleIsActive(void) {
    return kwlnTaskGetTaskByName(D_003A0800) != 0;
}

void func_0014A2C8(void) {
    D_003BAFA8 = 0;
    D_003BAFA0 = 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitle);

void func_0014A930(void) {
    if (D_003BAFB4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFB4);
        D_003BAFB4 = 0;
    }
}

extern void fldTitle(void);
extern void func_002D0918(s32);
extern u32 D_003BAFA4;
extern u32 D_003BAFAC;
extern u32 D_003BAF9C;
extern s32 func_002EB028();
extern char D_003A0810[]; /* "/fld/f/pnl/df%03d.tmx" */
void fldStartTitle(s32 field, s32 arg1, s32 arg2) {
    char path[32];
    s32 size;
    s32 handle;

    D_003BAFA8 = arg1;
    D_003BAFAC = arg2;
    D_003BAFA4 = field;
    D_003BAF9C = 0;
    D_003BAFA0 = 0;
    D_003BAFB0 = 0;
    func_003014F0(path, D_003A0810, field);
    handle = func_002EB028(path, &size, 0);
    D_003BAFB4 = func_002D3288(size);
    func_002D0918(handle);
    if (fldTitleIsActive() == 0) {
        kwlnTaskCreate(D_003A0800, 0x2B0A, 0, 1, fldTitle, func_0014A930, 0);
    }
}


void fldDestroyTitleTask(void) {
    if (fldTitleIsActive() != 0) {
        kwlnTaskDestroyWithHierarchyByName(D_003A0800, 1);
    }
}


s32 fldTitleMiniIsActive(void) {
    return kwlnTaskGetTaskByName(D_003A0828) != 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", fldTitleMini);

void fldReleaseTitleMiniTexture(void) {
    if (D_003BAFC4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFC4);
        D_003BAFC4 = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0800);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0810);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0824);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0828);

INCLUDE_ASM(const s32, "game/code_001411F0", fldStartMiniTitleForUnlock);

void fldRequestMiniTitleDismiss(void) {
    if (D_003BAFBC == 0) {
        D_003BAFBC = 1;
    }
}

extern s32 func_002EB028();
extern s32 func_002D3288();
extern s32 func_0014FE28();

void fldLoadWeatherEffects(void) {
    s32 handle;
    s32 size;

    handle = func_002EB028("/fld/f/bin/limit_00.tmx", &size, 0);
    D_003BAFF4 = func_002D3288(size);
    func_002D0A10(handle);
    D_003BAFC8 = func_002EB028("/fld/f/bin/FH_DAM_2.EPL", &D_003BAFCC, 0);
    D_003BAFD0 = func_0014FE28(D_003BAFCC);
    D_003BAFD4 = 0;
    D_003BAFD8 = func_002EB028("/fld/f/bin/YUK_2.EPL", &D_003BAFDC, 0);
    D_003BAFE0 = func_0014FE28(D_003BAFDC);
    D_003BAFE4 = 0;
}

void func_0014B4D0(void) {
    if (D_003BAFF4 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAFF4);
        D_003BAFF4 = 0;
    }
    effDestroyNode(D_003BAFD0);
    D_003BAFD0 = 0;
    D_003BAFD4 = 0;
    func_002D0A10(D_003BAFC8);
    D_003BAFC8 = 0;
    D_003BAFCC = 0;
    effDestroyNode(D_003BAFE0);
    D_003BAFE0 = 0;
    D_003BAFE4 = 0;
    func_002D0A10(D_003BAFD8);
    D_003BAFD8 = 0;
    D_003BAFDC = 0;
}

void fldSetWeatherEffectPos(f32 x, f32 y, f32 z) {
    f32 pos[4];

    memset(pos, 0, sizeof(pos));
    pos[3] = 1.0f;
    if (D_003BAFD0 != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_0014FB38(D_003BAFD0);
        func_0014FBF0(D_003BAFD0, pos);
        D_003BAFD4 = 1;
    }
    if (D_003BAFE0 != 0) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
        func_0014FB38(D_003BAFE0);
        func_0014FBF0(D_003BAFE0, pos);
        D_003BAFE4 = 1;
    }
}


void func_0014B5F8(void) {
    if (D_003BAFD0 != 0 && D_003BAFD4 != 0) {
        effUpdateNode(D_003BAFD0);
    }
    if (D_003BAFE0 != 0 && D_003BAFE4 != 0) {
        effUpdateNode(D_003BAFE0);
    }
}


void *func_0014B648(s32 arg0, s32 arg1) {
    FldEnt110 *entry = D_003BAA48;
    s32 index = 0;

    while (index < 8) {
        if (entry->unk0 == arg0) {
            if (entry->unk2 == arg1) {
                return entry;
            }
        }
        index++;
        entry++;
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08E8);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A08F8);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014B688);


void func_0014B858(void) {
    FldEnt14 *entry = D_003D62E0;
    s32 i = 0x10;

    do {
        s32 temp = entry->unk0;

        i--;
        if (temp != 0) {
            func_001130C8(temp);
            entry->unk0 = 0;
        }
        entry++;
    } while (i >= 0);
}

typedef struct FieldPair48 {
    f32 pos[4];
    f32 vel[4];
    s32 unk20;
    s32 active;
    s32 unk28;
    s16 unk2C;
    s16 unk2E;
} FieldPair48; /* 0x30 bytes */
extern FieldPair48 D_003D56A0[];
extern u32 effMiscRand();

extern s16 D_0032E4B4[];

void fldInitSparkTable(void) {
    s32 i;

    if (D_0032E4B4[0] == 0) {
        for (i = 0; i < 64; i++) {
            D_003D56A0[i].unk20 = 0;
            D_003D56A0[i].active = 0;
            D_003D56A0[i].unk28 = 0;
            D_003D56A0[i].unk2C = -1;
            D_003D56A0[i].unk2E = effMiscRand(0) % 60 + 15;
        }
    } else {
        for (i = 0; i < 64; i++) {
            D_003D56A0[i].unk20 = 0;
            D_003D56A0[i].unk2C = -1;
        }
    }
}

void fldResetSparkTable(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        D_003D56A0[i].active = 0;
        D_003D56A0[i].unk28 = 0;
        D_003D56A0[i].unk2C = -1;
        D_003D56A0[i].unk2E = effMiscRand(0) % 60 + 15;
    }
}


s32 fldSetSparkVectors(s32 index, const u128 *pos, const u128 *vel) {
    PCP_COPY_VECTOR(D_003D56A0[index].pos, pos);
    PCP_COPY_VECTOR(D_003D56A0[index].vel, vel);
    D_003D56A0[index].unk20 = 1;
    return 1;
}
INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BA50);

extern void effObjSetInnerFirstVec();
extern s32 *dds3GetUnk0C();
extern void dds3ClearObjectFlags();
extern FldVec4 D_003A0918;
void fldFreeSparkSlot(s32 index) {
    FldVec4 vec;
    s32 obj;
    s32 *flags;
    s16 slot;

    vec = D_003A0918;
    if (D_003D56A0[index].unk20 == 1 && D_003D56A0[index].active != 0 && D_003D56A0[index].unk2C != -1) {
        vec.v[0] = D_003D56A0[index].pos[0];
        vec.v[2] = D_003D56A0[index].pos[2];
        effObjSetInnerFirstVec(D_003D62E0[D_003D56A0[index].unk2C].unk0, &vec);
        obj = D_003D62E0[D_003D56A0[index].unk2C].unk0;
        flags = dds3GetUnk0C(obj);
        *flags |= 1;
        dds3ClearObjectFlags(obj, 0x400);
        slot = D_003D56A0[index].unk2C;
        D_003D56A0[index].unk2C = -1;
        D_003D62E0[slot].unk4 = -1;
    }
}

extern s32 D_003D62A0[];
extern void func_0014B688();
extern void func_0014BA50();
void fldUpdateSparkSlots(void) {
    s32 i;

    func_0014B688();
    for (i = 0; i < 64 && i < D_003D62A0[12]; i++) {
        if (D_003D56A0[i].unk20 != 0 && D_003D56A0[i].active != 0) {
            func_0014BA50(i, D_003D56A0[i].unk28);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014BDF8);

extern s32 D_003D62A0[];

s32 fldIsNearSpark(f32 x, f32 y, f32 z) {
    s32 i;

    for (i = 0; i < 64 && i < D_003D62A0[12]; i++) {
        if (D_003D56A0[i].active == 1 && D_003D56A0[i].unk2C != -1) {
            f32 dx = x - D_003D56A0[i].pos[0];
            f32 dy = y - D_003D56A0[i].pos[1];
            f32 dz = z - D_003D56A0[i].pos[2];

            if (fsqrtf(dx * dx + dy * dy + dz * dz) < 50.0f) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C210);

extern s32 D_003D62A0[];
extern void func_0024DAB8(s32);
extern void func_0024DAE8(s32);
extern void mdlFlagSet(s32);
INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C468);

extern s32 D_0032E5C4[];
extern s32 D_003D62A0[];
extern void func_001239C8(void);
extern void func_0014B858(void);
extern void func_0014B4D0(void);
extern void fldStartSceneBgmAlternate(void);
extern void func_00123E00(void);
extern void evtSetSolarOverlayFullyVisible(void);
typedef struct FldResetWork {
    u8 pad00[0x104];
    s16 eventActive; /* 0x104 */
} FldResetWork;

void fldFinishEventFieldState(void) {
    FldResetWork *state = (FldResetWork *)D_0032E3B0;

    if (state->eventActive != 0) {
        D_0032E5C4[0] = 0;
        func_001239C8();
        func_0014B858();
        func_0014B4D0();
        fldStartSceneBgmAlternate();
        func_00123E00();
        state->eventActive = 0;
        D_003D62A0[2] = 0;
        D_003D62A0[3] = 0;
        evtSetSolarOverlayFullyVisible();
    }
}

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0918);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0928);

INCLUDE_RODATA(const s32, "game/code_001411F0", D_003A0978);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014C648);


s32 func_0014CAF8(void) {
    return D_003D62C8[0];
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014CB08);


void fldResetEventSceneState(void) {
    func_0012E9E8(0);
    D_0032E5C4[0] = 0;
    D_0032E3B0[0x46] = 0;
    func_0014B858();
    func_0014B4D0();
    func_00123E00();
    D_0032E3B0[0x45] = 1;
    ((FldResetWork *)D_0032E3B0)->eventActive = 0;
    D_003D62A8[0] = 0;
    D_0032E3B0[0x4E] = 1;
    evtSetSolarOverlayFullyVisible();
}


void fldStartDeferredFieldExit(void) {
    func_0024D9D8(D_0034D8F0);
    func_0024DA58(5);
    func_00123EA8();
    D_0032E4C4[0] = 2;
}

void fldFinishDeferredExit(void) {
    if (D_0032E3B0[0x45] == 2) {
        func_0024DD78();
        if (!func_0024DC08()) {
            func_0024DBB0();
            func_0024DBC8();
            func_00123E00();
            D_0032E3B0[0x45] = 0;
        }
    }
}


s32 fldIsEventPhaseAtLeastTwo(void) {
    if (D_003D62A8[0] < 2) {
        return 0;
    }
    return 1;
}

void func_0014D0E8(void) {
    func_0014B5F8();
}


s32 func_0014D100(void) {
    return *(s16 *)((u8 *)D_003D62A4[0] + 0xC);
}

s32 func_0014D110(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = func_00110A38(world);
    void *entry;
    s32 result;

    if (unit == 0) {
        func_0010D5F0(0);
        return 1;
    }
    entry = func_00110A48(world, fldFindTaskRecordId(*(s32 *)(func_0010D6A0() + 0xE4)), 0x11);
    if (entry == 0) {
        func_0010D5F0(0);
        return 1;
    }
    if (func_0013A720(unit, entry) == 0) {
        func_0010D5F0(0);
        return 1;
    }
    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
    switch (result) {
    case 1:
        func_0010D5F0(1);
        return 1;
    case -1:
        func_0010D5F0(-1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}

s32 func_0014D1E8(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = func_00110A38(world);
    void *entry;
    s32 result;

    if (unit == 0) {
        func_0010D5F0(0);
        return 1;
    }
    entry = func_00110A48(world, fldFindTaskRecordId(*(s32 *)(func_0010D6A0() + 0xE4)), 0x11);
    if (entry == 0) {
        func_0010D5F0(0);
        return 1;
    }
    if (func_0013A9B0(unit, entry) == 0) {
        func_0010D5F0(0);
        return 1;
    }
    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
    switch (result) {
    case 1:
        func_0010D5F0(1);
        return 1;
    case -1:
        func_0010D5F0(-1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}


s32 func_0014D2C0(void) {
    s32 result;

    func_001411C0(scrReadIntParameter(0));
    func_00141190(1);
    result = func_001411F0();
    if (result == -1) {
        func_0010D5F0(-1);
        return 1;
    }
    if (result == 1) {
        func_0010D5F0(1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}

s32 func_0014D320(void) {
    s32 world = dds3GetWorldObject();
    s32 unit = func_00110A38(world);
    void *entry;

    if (unit == 0) {
        func_0010D5F0(0);
        return 1;
    }
    entry = func_00110A48(world, fldFindTaskRecordId(*(s32 *)(func_0010D6A0() + 0xE4)), 0x11);
    if (entry == 0) {
        func_0010D5F0(0);
        return 1;
    }
    if (func_0013A720(unit, entry) == 0) {
        func_0010D5F0(0);
    } else {
        func_0010D5F0(1);
    }
    return 1;
}


s32 func_0014D3C0(void) {
    s32 object;

    func_00123EA8();
    object = func_00110A38(dds3GetWorldObject());
    if (object == 0) {
        return 1;
    }
    func_001313E0();
    return 1;
}


s32 func_0014D400(void) {
    s32 object;

    object = func_00110A38(dds3GetWorldObject());
    if (object == 0) {
        return 1;
    }
    dds3InvokeSlot1Handler(object, 0);
    func_001313E0();
    func_00123E00();
    return 1;
}

u32 func_0014D458(void) {
    func_00125DD0(0x10);
    return 1;
}

u32 func_0014D478(void) {
    func_00125DE0(0x10);
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

extern void func_0012DB70(void);

s32 func_0014D498(void) {
    FldCamPose *work;
    FldObj *obj;
    u64 world = dds3GetWorldSecondaryObject();

    obj = func_00110A48(world, scrReadIntParameter(0), 4);
    if (obj == NULL) {
        return 1;
    }
    work = (FldCamPose *)D_0032E3B0;
    work->unk50 = 1;
    work->focusPos[0] = obj->model->pos[0];
    work->focusPos[1] = obj->model->pos[1];
    work->focusPos[2] = obj->model->pos[2];
    func_0012DB70();
    return 1;
}

extern FldVec3 D_00330610;
extern FldVec3 D_00330620;
extern f32 sdfSinPoly(f32);
extern f32 func_002E78F8(f32);
extern s32 func_001312D8();
extern void func_00131218();
extern void func_0012CB48();
s32 fldUpdateLookAtSegment(void) {
    FldCamPose *cam = (FldCamPose *)D_0032E3B0;
    FldVec3 near;
    FldVec3 far;

    cam->unk50 = 0;
    cam->negatedAngle = -cam->angle;
    near.x = cam->x - sdfSinPoly((cam->angle + 180.0f) * 3.14f / 180.0f);
    near.y = cam->y - 200.0f - 10.0f + 60.0f;
    near.z = cam->z + func_002E78F8((cam->angle + 180.0f) * 3.14f / 180.0f);
    far.x = cam->x + sdfSinPoly(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    far.y = cam->y - 200.0f - 10.0f + 60.0f;
    far.z = cam->z + func_002E78F8(cam->negatedAngle * 3.14f / 180.0f) * 550.0f;
    D_00330610.x = near.x;
    D_00330610.y = near.y;
    D_00330610.z = near.z;
    D_00330620.x = far.x;
    D_00330620.y = far.y;
    D_00330620.z = far.z;
    func_001312D8();
    func_00131218();
    func_0012CB48();
    return 1;
}


s32 func_0014D6C0(void) {
    D_0032E400[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014D6D8);

extern s32 func_00123DE0(void);
extern s32 func_001109B8(s32, s32);
extern void func_00112EE8(s32, f32 *, f32 *);
extern void func_0012D240(void);
extern f32 D_00330630[];
extern f32 D_00330640[];
extern s32 D_003BAD24;
extern s32 D_003BAD28;
s32 func_0014D8B8(void) {
    f32 pos[4];
    f32 rot[4];
    s32 handle;
    s32 object;
    s32 world;

    if (scrReadIntParameter(0) == -1) {
        handle = func_00123DE0();
        if (handle == 0) {
            return 1;
        }
        object = func_001109B8(dds3GetWorldObject(), handle);
        if (object == 0) {
            return 1;
        }
        func_00112EE8(object, pos, rot);
        func_0012D240();
        D_00330630[0] = pos[0];
        D_00330630[1] = pos[1];
        D_00330630[2] = pos[2];
        D_00330640[0] = rot[0];
        D_00330640[1] = rot[1];
        D_00330640[2] = rot[2];
        D_003BAD24 = 0;
        D_003BAD28 = scrReadIntParameter(1);
        D_0032E400[0] = 4;
    } else {
        world = dds3GetWorldObject();
        handle = func_00110A48(world, scrReadIntParameter(0), 4);
        if (handle == 0) {
            return 1;
        }
        func_001109B8(dds3GetWorldObject(), handle);
    }
    return 1;
}


s32 fldCmdSetRequestedSceneName(void) {
    char *name;

    name = scrReadStringParameter(0);
    D_0032E3B0[0x14] = 5;
    strcpy((char *)D_0032E3B0 + 0x40, name);
    return 1;
}

extern void evtSetSolarOverlayFullyTransparent(void);
extern void evtDisableSolarOverlayAlpha(void);
extern void evtEnableSolarOverlayAlpha(void);
s32 fldCmdSetSolarOverlayMode(void) {
    s32 mode = scrReadIntParameter(0);

    D_0032E3B0[51] = mode;
    D_0032E3B0[52] = 0;
    if (mode < 4) {
        D_0032E3B0[53] = mode;
    }
    switch (D_0032E3B0[51]) {
    case 0:
        evtSetSolarOverlayFullyTransparent();
        break;
    case 1:
        evtSetSolarOverlayFullyVisible();
        D_0032E3B0[78] = 1;
        break;
    case 2:
        evtDisableSolarOverlayAlpha();
        D_0032E3B0[52] = 0xF;
        break;
    case 3:
        evtEnableSolarOverlayAlpha();
        D_0032E3B0[52] = 0;
        D_0032E3B0[78] = 1;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        break;
    }
    return 1;
}

void func_0014DAF0(void) {
}

u32 func_0014DAF8(void) {
    return 1;
}

u32 func_0014DB00(void) {
    fldFreeDisplayObjects();
    return 1;
}

s32 fldFindSearchId(const char *name) {
    u32 *entry = func_00110F80(dds3GetWorldSecondaryObject(), name);
    if (entry != 0) {
        return entry[1];
    }
    func_003003F0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

extern char D_003BAFF8[];
extern s32 func_00302290(char *, const char *);
s32 fldParseRoomNumberFromName(char *name) {
    s32 index;
    if (func_00302290(name, D_003BAFF8) == 0) return 0;
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
    D_0032E408[0] = value;
    return 1;
}


s32 func_0014DC20(void) {
    s32 value;

    D_0032E570[11] = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    func_00132FD0(D_0032E570[11], value);
    return 1;
}

extern void fldSetFadeTarget(s32, s32, s32);
extern void fldSetSwayMode(s32);
s32 fldCmdSetFadeAndSway(void) {
    s32 second;
    s32 third;
    D_0032E570[13] = scrReadIntParameter(0) & 0xFF;
    second = scrReadIntParameter(1);
    third = scrReadIntParameter(2);
    fldSetFadeTarget(D_0032E570[13], second, third);
    fldSetSwayMode(scrReadIntParameter(3));
    return 1;
}

extern void func_00120FA0(s32, s32, s32, s32);
extern s64 func_00110400(u64);
extern s64 func_001104B0(u64);
extern u64 func_00110AB0(u64, u64);
extern void dds3DestroyWorldIndexNode(u64);
extern s32 func_00110490(u64);
typedef struct FldWorldItem {
    u8 pad0[0x18];
    s32 *data;
} FldWorldItem;
extern FldWorldItem *func_00110458(u64);
extern void func_00113E20(FldWorldItem *, s32);
s32 func_0014DCE0(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3B0[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3B0[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            func_00120FA0(world, stage, room, 0);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        func_00120FA0(world, stage, room, 0);
    }
    if (world == D_0032E3B0[4] && stage == D_0032E3B0[5] + 1) {
        list = func_00110AB0(dds3GetWorldSecondaryObject(), 6);
        if (func_00110400(list) != 0) {
            func_00110490(list);
            do {
                item = func_00110458(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        func_00113E20(item, 1);
                        break;
                    case 1:
                        func_00113E20(item, 5);
                        break;
                    case 2:
                        func_00113E20(item, 7);
                        break;
                    }
                }
            } while (func_001104B0(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

s32 func_0014DEB8(void) {
    s32 world;
    s32 stage;
    s32 mode;
    char *name;
    s32 room;
    u64 list;
    FldWorldItem *item;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3B0[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3B0[5] + 1;
    }
    mode = scrReadIntParameter(3);
    name = scrReadStringParameter(2);
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE44;
    }
    if (name == NULL) {
        for (room = 0; room < 16; room++) {
            func_00120FA0(world, stage, room, 1);
        }
    } else {
        room = fldParseRoomNumberFromName(name);
        func_00120FA0(world, stage, room, 1);
    }
    if (world == D_0032E3B0[4] && stage == D_0032E3B0[5] + 1) {
        list = func_00110AB0(dds3GetWorldSecondaryObject(), 6);
        if (func_00110400(list) != 0) {
            func_00110490(list);
            do {
                item = func_00110458(list);
                if (item->data[1] == room) {
                    switch (mode) {
                    case 0:
                        func_00113E20(item, 2);
                        break;
                    case 1:
                        func_00113E20(item, 6);
                        break;
                    case 2:
                        func_00113E20(item, 8);
                        break;
                    }
                }
            } while (func_001104B0(list) != 0);
            dds3DestroyWorldIndexNode(list);
        }
    }
    return 1;
}

extern void func_001210A0(s32, s32, s32, s32);
extern void func_00112008(void *, s32);
s32 func_0014E0B0(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3B0[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3B0[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_001210A0(world, stage, i, 0);
        }
    } else {
        func_001210A0(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    if (world == D_0032E3B0[4] && stage == D_0032E3B0[5] + 1) {
        id = fldFindSearchId(name);
        if (id != -1) {
            object = func_00110A48(dds3GetWorldSecondaryObject(), id, 6);
            switch (scrReadIntParameter(3)) {
            case 0:
                func_00112008(object, 0);
                break;
            case 1:
                func_00112008(object, 1);
                break;
            case 2:
                func_00112008(object, 2);
                break;
            case 3:
                func_00112008(object, 6);
                break;
            }
        }
    }
    return 1;
}

s32 func_0014E270(void) {
    s32 world;
    s32 stage;
    char *name;
    void *object;
    s32 id;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3B0[4];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3B0[5] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_001210A0(world, stage, i, 1);
        }
    } else {
        func_001210A0(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    if (world == D_0032E3B0[4] && stage == D_0032E3B0[5] + 1) {
        id = fldFindSearchId(name);
        if (id != -1) {
            object = func_00110A48(dds3GetWorldSecondaryObject(), id, 6);
            switch (scrReadIntParameter(3)) {
            case 0:
                func_00112008(object, 3);
                break;
            case 1:
                func_00112008(object, 4);
                break;
            case 2:
                func_00112008(object, 5);
                break;
            }
        }
    }
    return 1;
}

extern s32 D_0032E3C4[];
extern void func_001211A0(s32, s32, s32, s32);

s32 fldCmdSetSceneBits(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_001211A0(world, stage, i, 0);
        }
    } else {
        func_001211A0(world, stage, fldParseRoomNumberFromName(name), 0);
    }
    return 1;
}

extern char *D_003BAE48;
s32 func_0014E4D0(void) {
    s32 world;
    s32 stage;
    char *name;
    s32 i;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    name = scrReadStringParameter(2);
    if (strcmp(name, D_003BB000) == 0) {
        name = D_003BAE48;
    }
    if (name == NULL) {
        for (i = 0; i < 16; i++) {
            func_001211A0(world, stage, i, 1);
        }
    } else {
        func_001211A0(world, stage, fldParseRoomNumberFromName(name), 1);
    }
    return 1;
}

extern s32 D_0032E3C4[];
extern s32 func_001212A0(s32, s32, s32, s32);
extern s32 func_001213A0(s32, s32, s32, s32);
s32 func_0014E5B0(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_001212A0(area, floor, target, 0);
    return 1;
}

s32 func_0014E638(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_001212A0(area, floor, target, 1);
    return 1;
}

s32 func_0014E6C0(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_001213A0(area, floor, target, 0);
    return 1;
}

s32 func_0014E748(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    func_001213A0(area, floor, target, 1);
    return 1;
}

extern void fldSetMapSlotByte(s32, s32, s32, s32);

s32 fldCmdSetMapSlotByte(void) {
    s32 world;
    s32 stage;
    s32 slot;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    slot = scrReadIntParameter(2);
    if (slot == 0) {
        return 1;
    }
    fldSetMapSlotByte(world, stage, slot, scrReadIntParameter(3));
    return 1;
}

extern void fldSetFlagBit(s32, s32, s32);
s32 fldCmdSetFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldSetFlagBit(area, floor, target);
    return 1;
}

extern void fldClearFloorFlag(s32, s32, s32);
s32 fldCmdClearFloorFlag(void) {
    s32 area = scrReadIntParameter(0);
    s32 floor, target;
    if (area == 0) area = D_0032E3C0[0];
    floor = scrReadIntParameter(1);
    if (floor == 0) floor = D_0032E3C4[0] + 1;
    target = scrReadIntParameter(2);
    if (target == 0) return 1;
    fldClearFloorFlag(area, floor, target);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014E980);

INCLUDE_ASM(const s32, "game/code_001411F0", func_0014EA78);

extern void func_00121650(s32, s32, s32, s32);

s32 fldCmdSetSceneBitsValue(void) {
    s32 world;
    s32 stage;
    s32 room;

    world = scrReadIntParameter(0);
    if (world == 0) {
        world = D_0032E3C0[0];
    }
    stage = scrReadIntParameter(1);
    if (stage == 0) {
        stage = D_0032E3C4[0] + 1;
    }
    room = scrReadIntParameter(2);
    if (room == 0) {
        for (; room < 16; room++) {
            func_00121650(world, stage, room, scrReadIntParameter(3));
        }
    } else {
        func_00121650(world, stage, room, scrReadIntParameter(3));
    }
    return 1;
}


s32 fldSetFlagFromWorld1(void) {
    if (fldGetSceneStatusCode() == 1) {
        func_0010D5F0(1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}


s32 fldSetFlagFromWorld3(void) {
    if (fldGetSceneStatusCode() == 3) {
        func_0010D5F0(1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}


s32 fldSetFlagFromWorld4(void) {
    if (fldGetSceneStatusCode() == 4) {
        func_0010D5F0(1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}


s32 fldSetFlagFromWorld2(void) {
    if (fldGetSceneStatusCode() == 2) {
        func_0010D5F0(1);
        return 1;
    }
    func_0010D5F0(0);
    return 1;
}

typedef struct FldSceneParam {
    s32 unk_0;
    s16 unk_4;
    s16 unk_6;
    s32 unk_8;
    s32 unk_c;
} FldSceneParam;
extern FldSceneParam D_0034C8F0[];
s32 fldCmdPushSceneParam(void) {
    s32 room = fldFindRoomByTask(*(s32 *)(func_0010D6A0() + 0xE4));

    switch (scrReadIntParameter(0)) {
    case 0:
        func_0010D5F0(D_0034C8F0[room].unk_0);
        break;
    case 1:
        func_0010D5F0(D_0034C8F0[room].unk_4);
        break;
    case 2:
        func_0010D5F0(D_0034C8F0[room].unk_6);
        break;
    case 3:
        func_0010D5F0(D_0034C8F0[room].unk_8);
        break;
    case 4:
        func_0010D5F0(D_0034C8F0[room].unk_c);
        break;
    }
    return 1;
}

/* Script task's scene-record key, shared by the adjacent field commands. */
typedef struct FldTaskWork {
    u8 pad00[0xE4];
    s32 recordKey; /* 0xE4 */
} FldTaskWork;


u32 fldCmdActivateTaskRoomObject(void) {
    s32 task;
    u64 room;

    task = func_0010D6A0();
    room = fldFindRoomByTask(((FldTaskWork *)task)->recordKey);
    fldActivateFlaggedObject(room);
    return 1;
}


u32 fldCmdTestTaskRoomObjectActive(void) {
    if (fldTestObjectActivationFlag(fldFindRoomByTask(((FldTaskWork *)func_0010D6A0())->recordKey)) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_0014EEA0(void) {
    s32 scene;

    if (func_0013DF18()) {
        func_0013DF60(0);
        return 1;
    }
    scene = fldGetTaskRecordValue(((FldTaskWork *)func_0010D6A0())->recordKey);
    if (scene) {
        func_0013DF60(scene);
    }
    return 1;
}

u32 func_0014EEF8(void) {
    s32 value;

    value = scrReadIntParameter(0);
    func_0013E5A8(value);
    return 1;
}

u32 func_0014EF20(void) {
    return 1;
}

u32 fldCmdGetActorStat0(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = fldGetActorStat0(actorStat);
    func_0010D5F0(actorStat);
    return 1;
}

u32 fldCmdGetActorStat1(void) {
    s32 actorStat;

    actorStat = scrReadIntParameter(0);
    actorStat = fldGetActorStat1(actorStat);
    func_0010D5F0(actorStat);
    return 1;
}

u32 fldCmdGetActorMotionEntry(void) {
    s32 motionEntry;

    motionEntry = scrReadIntParameter(0);
    motionEntry = fldGetActorMotionEntry(motionEntry);
    func_0010D5F0(motionEntry);
    return 1;
}

u32 fldCmdGetRowValue(void) {
    s32 rowValue;

    rowValue = scrReadIntParameter(0);
    rowValue = fldGetRowValue(rowValue);
    func_0010D5F0(rowValue);
    return 1;
}

u32 func_0014EFE8(void) {
    s32 value;

    value = scrReadIntParameter(0);
    func_0013FA40(value);
    return 1;
}

u32 func_0014F010(void) {
    s32 first;
    s32 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_0011E810(first, second);
    return 1;
}

u32 func_0014F050(void) {
    func_0011E960();
    return 1;
}

u32 func_0014F070(void) {
    u64 result;

    result = func_0011E998();
    func_0010D5F0(result);
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE78);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE80);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE84);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE8C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE90);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE94);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE98);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAE9C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEA0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEA4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEA8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEC8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAECC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAED8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEDC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEE0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEE4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEF8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAEFC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF00);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF04);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF08);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF0C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF10);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF14);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF18);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF1C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF20);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF24);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF28);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF2C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF30);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF34);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF38);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF3C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF40);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF44);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF48);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF4C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF50);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF54);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF58);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF5C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF60);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF64);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF68);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF6C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF70);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF74);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF78);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF7C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF80);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF84);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF88);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF8C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF90);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF94);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF98);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAF9C);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFA8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFAC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFB8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFBC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFC8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFCC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFD0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFD4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFD8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFDC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFE0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFE4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFE8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFEC);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFF0);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFF4);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BAFF8);

INCLUDE_SDATA(const s32, "game/code_001411F0", D_003BB000);

