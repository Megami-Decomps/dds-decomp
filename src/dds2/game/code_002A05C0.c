#include "common.h"

extern u32 D_00438FE8;

extern u64 func_0010D650(u64);

extern u64 sdfSoundIsCommandBusy(void);

extern s32 func_002A2330(void);

extern u32 D_00437A2C;

extern s32 D_00437A40;

extern s32 func_00101958();

extern s32 D_004379F8;

extern u32 D_00454D30[];

extern u32 D_00438FEC;

extern u32 D_00455D70[];

extern char D_00428680[]; /* "titleProc" */

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_00437A00[];

extern char *strcat(char *, char *);

extern u8 D_00437A08[];

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

extern void func_0035C860();

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A05C0);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A08D8);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A0EE8);

void mnuSetTitleSequenceVolumePan(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

char *func_002A1040(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_00437A00;
    return strcat(arg0, arg1);
}

char *func_002A1070(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_00437A08;
    return strcat(arg0, arg1);
}

extern s8 D_004379FC;

extern char D_003E0B30[];

extern char D_003E0B50[];

extern s32 D_003D9D60[];

extern s32 D_00437A18;

extern s16 D_00438FCC;

extern s32 D_00437A1C;

extern void func_00341AD8(char *, s32, char *, s32);

extern void func_003421E8(s32);

extern void func_002A1E58(void);

extern void mnuCreateTitleEffectTask(void);

void func_002A10A0(void) {
    if (D_004379FC != 0) {
        func_00341CF8();
        return;
    }
    D_004379FC = 1;
    func_00341AD8(D_003E0B30, 4, D_003E0B50, 4);
    func_003421E8(D_003D9D60[0]);
    D_00437A18 = 4;
    D_00438FCC = -1;
    D_00437A1C = 0;
    func_002A1E58();
    mnuCreateTitleEffectTask();
}

u32 func_002A1118(void) {
    return 0x608;
}

u32 mnuIncrementTitleEffectFrameCounter(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(void) {
    func_00328E48(func_00101958());
    D_004379F8 = 0;
}

extern u8 D_00437A10[];

void mnuCreateTitleEffectTask(void) {
    u32 *data = (u32 *)func_00328D68(8);
    u32 task = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    D_004379F8 = task;
    func_00101950(task, data);
    data[0] = 0;
    data[1] = 0;
}

void mnuResetTitleEffectState(s32 effect) {
    s32 context = func_00101958(D_004379F8);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(effect, 0x7f);
    *(s32 *)(context + 4) = 0;
}

void func_002A1248(s32 arg0) {
    *(s32 *)func_00101958(D_004379F8) = arg0;
}

extern char D_00428610[];

extern char D_00428628[];

extern char D_003E09F0[];

void func_002A1278(char *filename) {
    char path[16];
    u32 *state = (u32 *)func_00101958(D_004379F8);
    if (sdfSoundIsCommandBusy() != 0) {
        func_0035B6E0(D_00428610);
        func_00342690();
    }
    func_0035C860(path, D_00428628, D_003E09F0 + 5 * state[0], filename);
    sdfSoundSendNamedCommand(path, 0x64);
    state[1] = 0;
}

void func_002A1308(void) {
    func_00342690();
}

void func_002A1320(void) {
    sdfSoundIsCommandBusy();
}

void func_002A1338(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return *(s32 *)(func_00101958(D_004379F8) + 4);
}

u32 func_002A1368(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00341BB8(temp_v0);
    return 1;
}

u32 func_002A1390(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    sndSetSequenceVolumePan(temp_v0, 0x7f, 0x3f);
    return 1;
}

u32 func_002A13C0(void) {
    func_00341CF8();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 value;
    value = func_0010D7D0(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_00342690();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 func_002A1430(void) {
    func_00342690();
    return 1;
}

u32 func_002A1450(void) {
    u64 temp_v0;

    temp_v0 = sdfSoundIsCommandBusy();
    func_0010D818(temp_v0);
    return 1;
}

u32 func_002A1478(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (func_002A2330() == 0) {
        func_002A2200(func_0010D650(0));
        return 0;
    }
    return func_002A2330() != 1;
}

u32 func_002A14D0(void) {
    func_002A2388();
    return 1;
}

u32 func_002A14F0(void) {
    func_002A2408();
    func_002A2550();
    return 1;
}

u32 func_002A1518(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    return 1;
}

u8 func_002A1538(void) {
    s64 temp_v0;

    temp_v0 = func_002A2330();
    return temp_v0 == 0;
}

s32 func_002A1558(u32 source, u32 destination, u32 words) {
    struct {
        u32 source;
        u32 destination;
        u32 size;
        u32 attributes;
    } transfer;
    s32 request;
    transfer.source = source;
    transfer.destination = destination;
    transfer.size = words * 4;
    transfer.attributes = 0;
    FlushCache(0);
    request = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(request) >= 0) {
    }
    return request;
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A15B0);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1678);

void func_002A1760(u32 arg0) {
    sceSdRemoteInit();
    func_002A15B0(arg0);
    D_00437A2C = 0;
}

extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern u64 func_0034E820();

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1790);

void func_002A1820(u32 source) {
    func_002A1558(D_00437A28, D_00437A20[0], source);
    func_002A1558(D_00437A28, D_00437A20[1], source);
    func_0034E820(1, 0x8010, 0xf80, 0);
    func_0034E820(1, 0x8010, 0x1080, 0);
    func_0034E820(1, 0x80e0, 0, 2, 0, 0);
}

typedef struct SndSampleBuf {
    u8 pad0[0x18];
    s16 *samples;
} SndSampleBuf;

void sndMixSampleBuffers(s16 *dst, SndSampleBuf *b, SndSampleBuf *a) {
    s16 *src = a->samples;
    s16 *out = dst;
    s32 i;
    for (i = 0; i < 0x800; i++) {
        *out++ = *src++;
    }
    src = b->samples;
    out -= 0x800;
    for (i = 0; i < 0x800; i++) {
        s32 v = *out + *src++;
        if (v > 0x7FFF) {
            v = 0x7FFF;
        }
        if (v < -0x7FFF) {
            v = -0x7FFF;
        }
        *out++ = v;
    }
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1928);

void func_002A1DD8(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(D_00438FE8);
        func_002A1928();
        SignalSema(D_00438FE8);
    }
}

extern s32 D_00438FE0;

extern u8 D_00456DB0[];

void func_002A1E08(void) {
    D_00438FE8 = sdfCreateSemaphore(1, 0xff, 0);
    func_00328918(&D_00438FE0, func_002A1DD8, D_00456DB0,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A1E58);

void func_002A1F50(AtracInfo *out) {
    WaitSema(D_00438FE8);
    out->unk0 = D_00454D30[0];
    out->unk4 = D_00454D30[1];
    out->unk8 = D_00454D30[3];
    SignalSema(D_00438FE8);
}

void func_002A1FA0(u32 *values) {
    WaitSema(D_00438FE8);
    D_00454D30[0] = values[0];
    D_00454D30[1] = values[1];
    D_00454D30[3] = values[2];
    SignalSema(D_00438FE8);
}

void func_002A1FF0(char *filePath, u32 *work) {
    void *fileData;
    s32 frames;
    u32 request = func_00343ED0(filePath, &fileData, 0);
    s32 bytes = sdfMemoryGetBlockSize(request);
    memcpy((void *)work[5], fileData, bytes);
    frames = bytes / (s32)work[2];
    work[1] = 0;
    work[0] = frames;
    func_003298C0(request);
}

void mnuStoreTaskResult(char *audioPath) {
    D_00438FEC = func_002C80C8(audioPath);
    D_00454D30[9] = 1;
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A20A0);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428600);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428610);

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428628);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2198);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2200);

u32 mnuUpdateTitleTransition(void) {
    if (D_00454D30[9] == 1) {
        func_002A20A0(D_00454D30);
    }
    return D_00454D30[9];
}

s32 func_002A2330(void) {
    WaitSema(D_00438FE8);
    if (D_00454D30[9] == 1) {
        func_002A20A0(D_00454D30);
    }
    SignalSema(D_00438FE8);
    return D_00454D30[9];
}

extern u32 D_00454D68[];

void func_002A2388(void) {
    WaitSema(D_00438FE8);
    if (mnuUpdateTitleTransition() == 1) {
        func_002C81E8();
        func_002A20A0(D_00454D30);
    }
    if (D_00454D30[4] != 1) {
        D_00454D30[4] = 0;
        D_00454D30[9] = 3;
        D_00454D68[0] = 0;
    }
    SignalSema(D_00438FE8);
}

void func_002A2408(void) {
    WaitSema(D_00438FE8);
    D_00454D30[1] = 0;
    D_00454D30[4] = 2;
    SignalSema(D_00438FE8);
}

extern u32 D_00437A38;

void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(D_00438FE8);
    if (D_00454D30[4] == 1 && D_00454D30[9] == 3) {
        D_00454D30[9] = 4;
        D_00454D68[0] = 0;
        D_00437A38 = 6;
    }
    SignalSema(D_00438FE8);
}

void func_002A24A0(void) {
    WaitSema(D_00438FE8);
    if (D_00454D30[4] == 1 && D_00454D30[9] == 3) {
        D_00437A38 = D_00454D30[9];
        D_00454D30[9] = 4;
        D_00454D68[0] = 0;
    }
    SignalSema(D_00438FE8);
}

extern u8 D_00455DB0[];

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2500);

void func_002A2550(void) {
    WaitSema(D_00438FE8);
    func_002A2500();
    SignalSema(D_00438FE8);
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428650);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2580);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2628);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A27A8);

s32 func_002A2928(void) {
    WaitSema(D_00438FE8);
    if (D_00455D70[8] == 0) {
        SignalSema(D_00438FE8);
        return 0;
    }
    if (D_00455D70[4] == 2) {
        SignalSema(D_00438FE8);
        return 2;
    }
    SignalSema(D_00438FE8);
    return 3;
}

void func_002A2998(void) {
    WaitSema(D_00438FE8);
    if (D_00455D70[4] != 1) {
        D_00455D70[4] = 0;
    }
    SignalSema(D_00438FE8);
}

void mnuReleaseSoundBuffer(void);

void mnuResetSoundBuffer(void) {
    D_00455D70[1] = 0;
    D_00455D70[4] = 2;
    mnuReleaseSoundBuffer();
}

void mnuReleaseSoundBuffer(void) {
    u32 *temp_v0 = D_00455D70;
    u32 temp_v1 = temp_v0[8];

    if (temp_v1 == 0) {
        return;
    }
    func_003298C0(temp_v1);
    temp_v0[8] = 0;
}

void func_002A2A40(void) {
    WaitSema(D_00438FE8);
    mnuResetSoundBuffer();
    SignalSema(D_00438FE8);
}

void func_002A2A70(void) {
    WaitSema(D_00438FE8);
    mnuReleaseSoundBuffer();
    SignalSema(D_00438FE8);
}

void func_002A2AA0(void) {
    func_002A3D70();
    func_002A3E38(1);
}

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2AC0);

extern void func_002A5F40(void);

extern void func_003458E8(u32);

extern void func_002A3DE8(void);

extern void func_002A3C58(void);

extern void func_003297C8(u32);

extern u32 D_00435BB0;

void func_002A2BD0(void) {
    func_002A5F40();
    func_003458E8(0);
    func_002A3DE8();
    mnuReleaseSpriteHandle();
    func_002A3C58();
    func_003297C8(*(u32 *)D_00437A40);
    D_00437A40 = 0;
    D_00435BB0 = 1;
}

INCLUDE_RODATA(const s32, "game/code_002A05C0", D_00428680);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A2C28);

INCLUDE_ASM(const s32, "game/code_002A05C0", func_002A30C0);

u32 func_002A3A50(void) {
    func_002A2AC0();
    return 0xffffffff;
}

s32 func_002A3A70(void) {
    func_002A2BD0();
    kwlnTaskDestroyWithHierarchyByName(D_00428680, 1);
    return 0;
}

u32 func_002A3AA0(void) {
    func_002A2AC0(0);
    return 0;
}

void func_002A3AC0(void) {
    evtDestroyWorldSecondaryNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}
