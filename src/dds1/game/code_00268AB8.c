#include "common.h"

extern char D_003AFD48[]; /* "---------- AT3 --------\n", followed by 8 zero bytes no C function emits */

extern char D_003AFD80[]; /* "titleProc" */

extern u32 D_003DA180[];

extern u32 D_003D9140[];

extern u32 D_003BD8D0;

extern u32 D_003BD8D4;

extern u32 D_003BC5BC;

extern u32 D_003BC5C8;

extern u8 D_003D9178[];

extern s32 func_0026A720(void);

extern u64 sdfSoundIsCommandBusy(void);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);

extern s32 func_00101A70();

extern s32 D_003BC588;

extern void func_00134CF0(void);

extern void func_002CF430(void);

extern s32 evtDestroySecondaryWorldNode(void);

extern void mnuCreateTitleEffectTask(void);

extern u8 D_003BC5A0[];

extern u64 scrReadIntParameter(u64);

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern u64 func_002F5990();

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00268D40);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_002692E0);

void mnuSetTitleSequenceVolumePan(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

char *func_00269420(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC590;
    return strcat(arg0, arg1);
}

char *func_00269450(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC598;
    return strcat(arg0, arg1);
}

extern s8 D_003BC58C;

extern char D_00377318[];

extern char D_00377338[];

extern s32 D_00370D10[];

extern s32 D_003BC5A8;

extern s16 D_003BD8B0;

extern s32 D_003BC5AC;

extern void func_002E8C30(char *, s32, char *, s32);

extern void func_002E9340(s32);

extern void func_0026A248(void);

extern void mnuCreateTitleEffectTask(void);

void func_00269480(void) {
    if (D_003BC58C != 0) {
        func_002E8E50();
        return;
    }
    D_003BC58C = 1;
    func_002E8C30(D_00377318, 4, D_00377338, 4);
    func_002E9340(D_00370D10[0]);
    D_003BC5A8 = 4;
    D_003BD8B0 = -1;
    D_003BC5AC = 0;
    func_0026A248();
    mnuCreateTitleEffectTask();
}

u32 func_002694F8(void) {
    return 0x599;
}

typedef struct TitleEffectState {
    s32 soundNameIndex; /* Selects a five-byte sound-name entry in the DDS2 twin. */
    s32 frameCounter;
} TitleEffectState;

u32 mnuIncrementTitleEffectFrameCounter(void) {
    TitleEffectState *state;

    state = (TitleEffectState *)func_00101A70();
    state->frameCounter = state->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(void) {
    func_002CFF98(func_00101A70());
    D_003BC588 = 0;
}

void mnuCreateTitleEffectTask(void) {
    TitleEffectState *state = (TitleEffectState *)func_002CFEB8(8);
    u32 task = kwlnTaskCreate(D_003BC5A0, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    D_003BC588 = task;
    func_00101A68(task, state);
    state->soundNameIndex = 0;
    state->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *state = (TitleEffectState *)func_00101A70(D_003BC588);
    if (sdfSoundIsCommandBusy() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    state->frameCounter = 0;
}

void func_00269628(s32 value) {
    ((TitleEffectState *)func_00101A70(D_003BC588))->soundNameIndex = value;
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFC80);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269658);

void func_002696F8(void) {
    func_002E97E8();
}

void func_00269710(void) {
    sdfSoundIsCommandBusy();
}

void func_00269728(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)func_00101A70(D_003BC588))->frameCounter;
}

u32 func_00269758(void) {
    u64 sequence;

    sequence = scrReadIntParameter(0);
    sndStartTrackDefault(sequence);
    return 1;
}

u32 func_00269780(void) {
    u64 sequence;

    sequence = scrReadIntParameter(0);
    sndSetSequenceVolumePan(sequence, 0x7f, 0x3f);
    return 1;
}

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 value;
    value = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        func_002E97E8();
    }
    sdfSoundSendNamedCommand(value, 0x7f);
    return 1;
}

u32 func_00269820(void) {
    func_002E97E8();
    return 1;
}

u32 func_00269840(void) {
    u64 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    func_0010D5F0(soundBusy);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (func_0026A720() == 0) {
        func_0026A5F0(scrReadIntParameter(0));
        return 0;
    }
    return func_0026A720() != 1;
}

u32 func_002698C0(void) {
    func_0026A778();
    return 1;
}

u32 func_002698E0(void) {
    func_0026A808();
    func_0026A950();
    return 1;
}

u32 func_00269908(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    return 1;
}

u8 func_00269928(void) {
    s64 soundState;

    soundState = func_0026A720();
    return soundState == 0;
}

s32 func_00269948(u32 source, u32 destination, u32 words) {
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

INCLUDE_ASM(const s32, "game/code_00268AB8", func_002699A0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269A68);

void func_00269B50(u32 arg0) {
    sceSdRemoteInit();
    func_002699A0(arg0);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269B80);

void func_00269C10(u32 source) {
    func_00269948(D_003BC5B8, D_003BC5B0[0], source);
    func_00269948(D_003BC5B8, D_003BC5B0[1], source);
    func_002F5990(1, 0x8010, 0xf80, 0);
    func_002F5990(1, 0x8010, 0x1080, 0);
    func_002F5990(1, 0x80e0, 0, 2, 0, 0);
}

typedef struct MixSource {
    u8 unk0[0x18];
    s16 *samples;   /* 0x18 */
} MixSource;

void sndMixSampleBuffers(s16 *dst, MixSource *first, MixSource *second) {
    s16 *out = dst;
    s16 *in = second->samples;
    s32 i;

    for (i = 0; i < 0x800; i++) {
        *out++ = *in++;
    }
    in = first->samples;
    out -= 0x800;
    for (i = 0; i < 0x800; i++) {
        s32 sample = *out + *in++;

        if (sample > 0x7FFF) {
            sample = 0x7FFF;
        }
        if (sample < -0x7FFF) {
            sample = -0x7FFF;
        }
        *out++ = sample;
    }
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_00269D18);

void func_0026A1C8(void) {
    for (;;) {
        sdfSleepThreadCount(1);
        WaitSema(D_003BD8D0);
        func_00269D18();
        SignalSema(D_003BD8D0);
    }
}

extern u32 D_003BD8D0;

extern s32 D_003BD8C8;

extern u8 D_003DB1C0[];

void func_0026A1F8(void) {
    D_003BD8D0 = sdfCreateSemaphore(1, 0xff, 0);
    func_002CFA68(&D_003BD8C8, func_0026A1C8, D_003DB1C0,
                  0x1000, 0x45, 0);
    sdfThreadSleepSelf();
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A248);

typedef struct AtracInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} AtracInfo;

extern s32 WaitSema(u32);
extern s32 SignalSema(u32);

void func_0026A340(AtracInfo *out) {
    WaitSema(D_003BD8D0);
    out->unk0 = D_003D9140[0];
    out->unk4 = D_003D9140[1];
    out->unk8 = D_003D9140[3];
    SignalSema(D_003BD8D0);
}

extern u32 D_003BD8D0;

extern u32 D_003D9140[];

void func_0026A390(u32 *values) {
    WaitSema(D_003BD8D0);
    D_003D9140[0] = values[0];
    D_003D9140[1] = values[1];
    D_003D9140[3] = values[2];
    SignalSema(D_003BD8D0);
}

void func_0026A3E0(char *filePath, u32 *work) {
    void *fileData;
    s32 frames;
    u32 request = func_002EB028(filePath, &fileData, 0);
    s32 bytes = sdfMemoryGetBlockSize(request);
    memcpy((void *)work[5], fileData, bytes);
    frames = bytes / (s32)work[2];
    work[1] = 0;
    work[0] = frames;
    func_002D0A10(request);
}

void mnuStoreTaskResult(void) {
    D_003BD8D4 = func_00288B48();
    D_003D9140[9] = 1;
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A490);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A588);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A5F0);

u32 mnuUpdateTitleTransition(void) {
    if (D_003D9140[9] == 1) {
        func_0026A490(D_003D9140);
    }
    return D_003D9140[9];
}

s32 func_0026A720(void) {
    WaitSema(D_003BD8D0);
    if (D_003D9140[9] == 1) {
        func_0026A490(D_003D9140);
    }
    SignalSema(D_003BD8D0);
    return D_003D9140[9];
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFCF0);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A778);

void func_0026A808(void) {
    WaitSema(D_003BD8D0);
    D_003D9140[1] = 0;
    D_003D9140[4] = 2;
    SignalSema(D_003BD8D0);
}

void mnuAdvanceTitleStateUnderSemaphore(void) {
    WaitSema(D_003BD8D0);
    if (D_003D9140[4] == 1 && D_003D9140[9] == 3) {
        D_003D9140[9] = 4;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(D_003BD8D0);
}

extern u32 D_003BC5C8;

void func_0026A8A0(void) {
    WaitSema(D_003BD8D0);
    if (D_003D9140[4] == 1 && D_003D9140[9] == 3) {
        D_003BC5C8 = D_003D9140[9];
        D_003D9140[9] = 4;
        *(u32 *)D_003D9178 = 0;
    }
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A900);

void func_0026A950(void) {
    WaitSema(D_003BD8D0);
    func_0026A900();
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026A980);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AA28);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026ABA8);

s32 func_0026AD28(void) {
    WaitSema(D_003BD8D0);
    if (D_003DA180[8] == 0) {
        SignalSema(D_003BD8D0);
        return 0;
    }
    if (D_003DA180[4] == 2) {
        SignalSema(D_003BD8D0);
        return 2;
    }
    SignalSema(D_003BD8D0);
    return 3;
}

void mnuPrintTitleDebugBanner(void) {
    WaitSema(D_003BD8D0);
    if (D_003DA180[4] != 1) {
        D_003DA180[4] = 0;
    }
    func_003003F0(D_003AFD48);
    SignalSema(D_003BD8D0);
}

void mnuResetSoundBuffer(void) {
    D_003DA180[1] = 0;
    D_003DA180[4] = 2;
    mnuReleaseSoundBuffer();
}

void mnuReleaseSoundBuffer(void) {
    u32 *temp_v0 = D_003DA180;
    u32 temp_v1 = temp_v0[8];

    if (temp_v1 == 0) {
        return;
    }
    func_002D0918(temp_v1);
    temp_v0[8] = 0;
}

void func_0026AE50(void) {
    WaitSema(D_003BD8D0);
    mnuResetSoundBuffer();
    SignalSema(D_003BD8D0);
}

void func_0026AE80(void) {
    WaitSema(D_003BD8D0);
    mnuReleaseSoundBuffer();
    SignalSema(D_003BD8D0);
}

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFD48);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AEC8);

extern u8 D_003DC1C0[];
extern u8 D_003DC1D0[];
extern void *D_003BD8E0;
extern void effObjSetInnerFirstVec(void *, void *);
extern void effObjSetInnerSecondVec(void *, void *);

s32 func_0026AF30(void) {
    effObjSetInnerFirstVec(D_003BD8E0, D_003DC1C0);
    effObjSetInnerSecondVec(D_003BD8E0, D_003DC1D0);
    return (*(s32 (**)(void *))(*(s32 *)((u8 *)D_003BD8E0 + 0x10) + 8))(D_003BD8E0);
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026AF78);

s64 mnuMovieShutdownA(void) {
    func_00134CF0();
    func_002CF430();
    return evtDestroySecondaryWorldNode();
}

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B050);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B160);

extern void func_0026CB10(void *, void *);

s64 func_0026B1C0(void) {
    func_0026CB10(D_003DC1C0, D_003DC1D0);
    return func_0026AF30();
}

INCLUDE_RODATA(const s32, "game/code_00268AB8", D_003AFD80);

INCLUDE_ASM(const s32, "game/code_00268AB8", func_0026B1F0);

u32 func_0026BCE8(void) {
    func_0026B050();
    return 0xffffffff;
}

s32 func_0026BD08(void) {
    func_0026B160();
    kwlnTaskDestroyWithHierarchyByName(D_003AFD80, 1);
    return 0;
}

u32 func_0026BD38(void) {
    func_0026B050(0);
    return 0;
}

void func_0026BD58(void) {
    evtDestroySecondaryWorldNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC588);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC58C);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC590);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC598);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5A8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5AC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5B8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5BC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C0);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C4);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5C8);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5CC);

INCLUDE_SDATA(const s32, "game/code_00268AB8", D_003BC5D0);

