#include "common.h"

extern s32 func_002C4038();

/* Sliding menu bar: direction flag and 0..max position */
typedef struct { s32 active; s32 pos; } SlideBar;

extern s32 func_002A46C8(s32);

extern u32 D_00438FE8;

extern u32 func_0029D790();

extern u64 func_0010D650(u64);

extern u64 sdfSoundIsCommandBusy(void);

extern s32 func_002A2330(void);

extern u32 D_00437A2C;

extern s32 D_00437A40;

extern u16 D_00435BAC;

extern u32 *D_00437AB0;

extern s32 sdfGraphHasPendingWorkInterruptSafe(void);

extern u32 D_00437AE8;

extern s32 kwlnFadeIsActive(void);

extern s32 func_00101958();

extern s32 D_004379F8;

extern u32 D_00454D30[];

extern u32 D_00438FEC;

extern u32 D_00455D70[];

extern char D_00428680[]; /* "titleProc" */

extern char D_00429938[]; /* "staffImageProc" */

extern char D_00429968[]; /* "staffProc" */

extern u8 D_003E5608[];

extern u8 D_003803C8[];

extern u32 D_00437ACC;

extern u8 D_00457E60[];

extern char D_0042A418[];

extern u32 D_00457E48[];

extern s32 func_002A88A0();

extern u8 D_00437AC0[];

extern char D_00437B78[]; /* "camp" */

extern char D_0042AA08[]; /* "camp_draw" */

extern char D_0042AA18[]; /* "camp_update" */

extern s8 D_00437B72;

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

extern u8 *D_00435DD0;

extern s8 D_00437B73;

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BC58);

typedef struct {
    u8 pad0[6];
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
} TitleSeq;

extern void func_00299A38(TitleSeq *, u8 *);

extern void btlAddBaseStats(u8 *, TitleSeq *);

void mnuTitleApplySequenceState(u8 *work) {
    s32 state = *(s32 *)(work + 0xB6F4);
    TitleSeq *seq = **(TitleSeq ***)(work + 0x9C);

    switch (state) {
    case 5:
        break;
    case 4:
        btlAddBaseStats(work + 0x3F4, seq);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        seq->unk6 = seq->unk8;
        seq->unkA = seq->unkC;
        seq->unkE = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        seq->unk6 = seq->unk8;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        seq->unkA = seq->unkC;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    func_00299A38(seq, work);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029BFB8);

INCLUDE_ASM(const s32, "game/code_0029BC58", itfRunPanelMode1);

s64 itfRunPanelMode2(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C8E8(0);
    return func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C120);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C3F0);

extern u32 func_00309138(u32, u32, s32);

void func_0029C450(u8 *work) {
    s32 remaining = 0x100 - *(s32 *)(work + 0xB6E0);
    u32 opacity;
    if (*(s8 *)(work + 0xAEA8) != 0) {
        return;
    }
    opacity = func_00309138(0x80808080, 0x80808000, remaining) & 0xFF;
    *(u32 *)(work + 0xAEB4) = opacity;
    if (opacity >= 0x80) {
        *(s8 *)(work + 0xAEA8) = 1;
    }
}

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;

extern void func_00308620(BurstSprite *, u32 *, s32, s32, s32);

void mnuTitleDrawBurstSprites(s32 arg0, s32 arg1) {
    BurstTable table = D_00428420;
    s32 scaled = arg0 * 0x13 / 256;
    u32 i;

    for (i = 0; i < 8; i++) {
        func_00308620(&table.sprite[i], &table.sprite[i].word[3], 0, scaled, arg1);
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C618);

void func_0029C800(void) {
}

void func_0029C808(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C810);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C848);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C860);

void func_0029C878(void) {
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029C880);

extern u32 D_004379C8[];

extern s32 func_0019F6C8();

extern void func_0035C860();

void mnuCampDrawMenuIconLayer(s32 x, s32 y, s32 z, u32 alpha, u8 *res, s32 arg5, u8 *work) {
    char name[32];
    u32 color[4];
    s32 sprite;
    u32 packed;

    packed = (alpha & 0xFF) | 0xA09DC300;
    func_0035C860(name, D_004379C8, *(u32 *)(res + 0x10));
    sprite = func_0019F6C8(x, y, z, packed, name, 0);
    frFontSetChainFlag(sprite, 3);
    func_0019D550(sprite, 1, arg5);
    func_0019C5B0(sprite);
    color[0] = alpha;
    color[1] = alpha;
    color[2] = alpha;
    color[3] = alpha;
    func_00306C28(x + 0x180, y + 0x78, 0, color, 0, *(s32 *)(work + 0xAEB0), 0x1B, 0x53);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CB70);

extern struct { s32 v[6]; } D_003D6500;

extern void mnuCampDrawMenuIconLayer(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029CB70(s32, s32, s32, u32, u8 *, s32, u8 *);

extern void func_0029C880(s32, s32, s32, u32, u8 *, s32, u8 *);

void mnuTitleDrawFadeMenuEntries(u8 *work) {
    u8 *res = work + 0x5C;
    u32 color = func_00309138(0xFFF06480, 0xFFF06400, 0x100 - *(s32 *)(work + 0xB6E0));

    mnuCampDrawMenuIconLayer(D_003D6500.v[0], D_003D6500.v[1], 0, color, res, 0x53, work);
    func_0029CB70(D_003D6500.v[2], D_003D6500.v[3], 0, color, res, 0x53, work);
    func_0029C880(D_003D6500.v[4], D_003D6500.v[5], 0, color, res, 0x53, work);
}

void mnuTitleRenderFadeAndPanels(u8 *work) {
    s32 remaining = 0x100 - *(s32 *)(work + 0xB6E0);

    func_0029C450(work);
    func_0029C618(work);
    func_0029C120(work);
    func_0029C3F0(work);
    func_0029DB58(0x1D0, 0x3B8, 0, remaining, work + 0x408, 0x53);
    mnuTitleDrawFadeMenuEntries(work);
}

void func_0029CDD8(void) {
    func_0029C810();
}

extern u8 D_003D9D58[];

u8 func_0029CDF0(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= D_003D9D58[i * 2]) {
            return D_003D9D58[i * 2 + 1];
        }
    }
    return D_003D9D58[1];
}

u8 func_0029CE30(s32 position, s32 increment) {
    u8 *table = D_003D9D58;
    s32 i = 2;
    u8 *limit = table + 4;
    s32 end = position + increment;
    do {
        if (position < *limit && end >= *limit) {
            return limit[1];
        }
        limit -= 2;
    } while (--i >= 0);
    return 0;
}

u32 func_0029CE80(void) {
    return 1;
}

u32 func_0029CE88(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CE90);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029CF00);

/* DDS2 twin of DDS1 brsCalcExpGain: enemy units (flag 2) keep exp; others
   start at 0, halve on reward flag 0x23F and restore on 0x240. */
s32 func_0029CF88(u8 *unit, s32 exp, s32 a2) {
    s32 result;

    if ((*(u16 *)unit & 2) != 0) {
        result = exp;
    } else {
        result = 0;
        if (func_00315098(unit, 0x23F) != 0) {
            result = exp / 2;
        }
        if (func_00315098(unit, 0x240) != 0) {
            result = exp;
        }
    }
    return result;
}

u32 func_0029D000(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D008);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D1C8);

s32 mnuCountAdvancingTitleAnimations(void) {
    s32 offset = 0;
    s32 count = 0;
    s32 remaining = 4;
    do {
        s32 step = func_0029D1C8(D_00435DD0 + 0xa60 + offset);
        count += step > 0;
        offset += 0x1c4;
    } while (--remaining >= 0);
    return count;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D2D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D3D8);

s32 mnuAdvanceTitleEntryAnimation(u8 *entry) {
    s32 step = func_0029D1C8(entry);
    *(u16 *)(entry + 0x14) += step;
    func_003144E8(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", btlAddBaseStats);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D5B8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029D790);

void mnuTitleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}

extern s32 func_0029D1C8(u8 *);
extern s32 func_0029CE90(u8 *, s32);
extern u32 func_00314728(u8 *, u32);
extern s32 func_00314C10(s32);
extern u32 func_00314690(u16);

/* DDS2 twin of DDS1 brsBuildUnitProgressRow. */
void func_0029D970(u8 *state, u8 *entry) {
    s32 levelDelta;
    s32 profilePoints;

    memset(state, 0, 0x2C);
    *(u32 *)(state + 0x8) = (u32)entry;
    levelDelta = func_0029D1C8(entry);
    mnuTitleInitFourParameters((u32 *)(state + 0xC), 0x6E0, 0x50,
        *(s32 *)(entry + 0x10) - func_0029CE90(entry, levelDelta),
        func_0029CE90(entry, levelDelta + 1) - func_0029CE90(entry, levelDelta));
    profilePoints = func_00314728(entry, 0);
    mnuTitleInitFourParameters((u32 *)(state + 0x1C), 0x3C0, 0x50, profilePoints,
        func_00314690(func_00314C10((s32)entry) & 0xFFFF));
}

u32 func_0029DA58(u32 a, u32 b, u32 c, s32 blend, u8 *resource) {
    func_00314C10(*(u32 *)(resource + 8));
    return func_00309138(0x80808080, 0x80808000, blend);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DA98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DB58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DF18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428410);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428420);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004284E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428550);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428570);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029DFB0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E220);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E478);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E548);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029E820);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029EE80);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029F440);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029FA98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_0029FBE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0148);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0278);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A05C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A08D8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A0EE8);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A15B0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1678);

void func_002A1760(u32 arg0) {
    sceSdRemoteInit();
    func_002A15B0(arg0);
    D_00437A2C = 0;
}

extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern u64 func_0034E820();

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1790);

void func_002A1820(u32 source) {
    func_002A1558(D_00437A28, D_00437A20[0], source);
    func_002A1558(D_00437A28, D_00437A20[1], source);
    func_0034E820(1, 0x8010, 0xf80, 0);
    func_0034E820(1, 0x8010, 0x1080, 0);
    func_0034E820(1, 0x80e0, 0, 2, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", sndMixSampleBuffers);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1928);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A1E58);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A20A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004285F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428600);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428610);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428628);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2198);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2200);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2500);

void func_002A2550(void) {
    WaitSema(D_00438FE8);
    func_002A2500();
    SignalSema(D_00438FE8);
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428650);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2580);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2628);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A27A8);

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

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2AC0);

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

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428680);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A2C28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A30C0);

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
    func_00117998();
    func_00117908();
}

extern s16 D_003E3792[];

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3AE8);

s16 func_002A3B10(u32 index) {
    return D_003E3792[index * 4];
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3B28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3BE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3C58);

u32 func_002A3C78(void) {
    return **(u32 **)(*(s32 *)(D_00437A40 + 0x24) + 0x1c);
}

void func_002A3C90(s32 arg0) {
    func_002B8968(*(u32 *)(D_00437A40 + 0x24));
    if (0 < arg0) {
        do {
            arg0 = arg0 - 1;
            func_002B8CF0(*(u32 *)(D_00437A40 + 0x24));
        } while (arg0 != 0);
    }
}

extern s8 D_0037F510[];

s32 func_002A3CE0(void) {
    if (D_0037F510[0x21] < 0 || D_0037F510[0x23] < 0 ||
        D_0037F510[0x22] < 0 || D_0037F510[0x20] < 0 ||
        D_0037F510[0x2a] < 0 || D_0037F510[0x2b] < 0 ||
        D_0037F510[0x28] < 0 || D_0037F510[0x29] < 0 ||
        D_0037F510[0x2d] < 0 || D_0037F510[0x2c] < 0) {
        return 1;
    }
    return 0;
}

extern u8 D_00437A48[];

extern u8 D_003E3760[];

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3D70);

void func_002A3DE8(void) {
    u32 *state = (u32 *)D_00437A40;
    if (state[1] != 0) {
        func_003054E8(state[1]);
        state = (u32 *)D_00437A40;
        state[1] = 0;
    }
    if (state[3] != 0) {
        func_003054E8(state[3]);
        state = (u32 *)D_00437A40;
        state[3] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3E38);

u8 mnuHasSpriteHandle(void) {
    return *(s32 *)(D_00437A40 + 8) != 0;
}

void mnuReleaseSpriteHandle(void) {
    if (*(s32 *)(D_00437A40 + 8) != 0) {
        func_003054E8(*(s32 *)(D_00437A40 + 8));
        *(u32 *)(D_00437A40 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", mnuStartMovieMenuSfx16);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F60);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3F98);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A3FE0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A40C8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4110);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A41C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4208);

void func_002A4380(s32 arg0) {
    *(u8 *)(arg0 + 8) = 0;
}

typedef struct PickEntry {
    s8 id;
    u8 unk1;
    u8 unk2;
} PickEntry;

typedef struct PickList {
    u8 unk0[8];
    u8 count;
    PickEntry entry[32];
} PickList;

void titlePickRandomSlot(list)
    PickList *list;
{
    u32 range = 0x20;
    s32 tries;
    tries = 0;
    do {
        u32 id;
        s32 i;
        s32 found;
        s32 n = list->count;
        if (n >= 0x20) {
            return;
        }
        id = effMiscRand(0) % range;
        found = 0;
        for (i = 0; i < list->count; i++) {
            if (list->entry[i].id == id) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            list->entry[list->count].id = id;
            list->entry[list->count].unk1 = 0;
            list->entry[list->count].unk2 = 10;
            list->count++;
        }
        tries++;
    } while (tries <= 0);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A44C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4670);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A46C8);

s32 func_002A4728(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v0 = 0;
    do {
        temp_v1 = temp_v0 + 1;
        temp_v0 = func_002A46C8(temp_v0);
        temp_v2 = temp_v2 + temp_v0;
        temp_v0 = temp_v1;
    } while (temp_v1 < 3);
    return temp_v2;
}

s32 func_002A4770(s32 segment) {
    s32 i = 0;
    s32 total = 0;
    s32 duration = func_002A4728();

    while (i < segment) {
        total += func_002A46C8(i++);
    }
    return (total << 12) / duration;
}

s32 func_002A47E8(s32 position) {
    s32 segment = 0;
    s32 total = 0;
    s32 duration = func_002A4728();

    while (segment < 3) {
        total += func_002A46C8(segment);
        if (position < (total << 12) / duration) {
            return segment;
        }
        segment++;
    }
    return 3;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4870);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A48F0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A49C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4A68);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4B70);

extern void titlePickRandomSlot();

extern void func_002A44C0(SlideBar *, s32);

extern void func_002A4B70(SlideBar *, s32);

extern void func_002A3B28(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSlideBar(SlideBar *bar, s32 arg1) {
    if (bar->active != 0 || bar->pos != 0) {
        titlePickRandomSlot();
        func_002A44C0(bar, arg1);
        func_002A3B28(0, 0, 0, bar->pos, 0, 0x16, arg1);
        func_002A4B70(bar, arg1);
        if (bar->active != 0) {
            bar->pos += 8;
        } else {
            bar->pos -= 8;
        }
        if (bar->pos < 0) {
            bar->pos = 0;
        }
        if (bar->pos > 0x80) {
            bar->pos = 0x80;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A4DF0);

typedef struct { s32 active; s32 pos; s32 id; s32 timer; } SlideBarTimed;

extern void func_002A4DF0(SlideBarTimed *, s32, s32);

extern void func_002A3B28(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceTimedSlideBar(SlideBarTimed *bar, s32 arg1) {
    if (bar->timer > 0) {
        bar->timer--;
        if (bar->timer == 0) {
            func_002A4DF0(bar, bar->id, 0);
        }
    }
    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_002A3B28(0, 0, 0, bar->pos / 4, 0, 0x12, arg1);
    if (bar->active != 0) {
        bar->pos += 0x10;
    } else {
        bar->pos -= 8;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
}

void mnuAdvanceMultiSpriteSlideBar(SlideBar *bar, s32 arg1) {
    s32 half;

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    half = bar->pos / 4;
    func_002A3B28(0, 0, 0, half, 0, 0x13, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1A, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1B, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1C, arg1);
    if (bar->active != 0) {
        bar->pos += 0x1B;
    } else {
        bar->pos -= 0x1B;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5040);

void func_002A50E8(s32 arg0, s32 arg1, u8 arg2) {
    *(u8 *)(arg0 + arg1) = arg2;
}

void func_002A50F8(u8 *work) {
    u32 i;
    for (i = 0; i < 2; i++) {
        work[i] = 0;
        *(u32 *)(work + 4 + i * 4) = 0;
    }
}

typedef struct {
    u8 active[2];
    u8 pad2[2];
    s32 pos[2];
} SlideBarPair;

void mnuAdvancePairedSlideBars(SlideBarPair *bars, s32 arg1) {
    u32 i;

    for (i = 0; i < 2; i++) {
        s32 pos = bars->pos[i];

        switch (i) {
        case 0:
            func_002A3B28(0, 0, 0, pos, 0, 0xF, arg1);
            func_002A3B28(0, 0, 0, pos, 0, 0x10, arg1);
            break;
        case 1:
            func_002A3B28(0, 0, 0, pos, 0, 0x11, arg1);
            break;
        }
        if (bars->active[i] == 0) {
            bars->pos[i] -= 8;
        } else {
            bars->pos[i] += 8;
        }
        if (bars->pos[i] < 0) {
            bars->pos[i] = 0;
        }
        if (bars->pos[i] > 0x80) {
            bars->pos[i] = 0x80;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5260);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A55B8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5890);

void func_002A58C0(void) {
    u8 *state = (u8 *)D_00437A40;

    *(u32 *)(state + 0x28) = 0;
    *(u32 *)(state + 0x14) = 0;
    *(u32 *)(state + 0x1C) = 0;
}

void func_002A58D8(void) {
    u8 *state = (u8 *)D_00437A40;

    *(u32 *)(state + 0x28) = 0;
    *(u32 *)(state + 0x14) = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A58E8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5A20);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5A78);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5B08);

void mnuTitleResetSequenceTimers(void) {
    u32 *title = (u32 *)D_00437A40;
    title[0x28 / 4] = 1;
    title[0x1C / 4] = title[0x14 / 4] = 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5C58);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5E00);

void func_002A5EE8(u32 arg0, s32 arg1) {
    s32 temp_v0;

    func_002A7AF0();
    temp_v0 = D_00437A40;
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 1;
        if (arg1 == 0) {
            *(u32 *)(temp_v0 + 0x110) = 0x80;
        }
        else {
            *(u32 *)(temp_v0 + 0x110) = 0;
        }
        *(u32 *)(D_00437A40 + 0x114) = 0;
    }
}

void func_002A5F40(void) {
    func_002A7FD0();
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 0;
    }
}

u32 func_002A5F68(void) {
    u32 temp_v0;

    temp_v0 = 0;
    if (D_00437A40 != 0) {
        temp_v0 = *(u32 *)(D_00437A40 + 0x10c);
    }
    return temp_v0;
}

extern void func_003458F0(u32, u32, u32, u32, u32);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A5F80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004287E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004287F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428810);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428820);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428860);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428870);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428880);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428898);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004288F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428908);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428920);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428930);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428948);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428958);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428968);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428978);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428998);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004289F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A10);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428A90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428AF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B20);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B50);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B68);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428B90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BC0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428BF0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C68);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C88);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428C98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CD0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CE0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428CF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D10);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D30);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428D90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428DE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E00);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E30);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428E88);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428ED8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428EE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F00);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F28);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F50);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F60);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F70);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F80);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428F90);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FA0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FC8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FE8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00428FF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429008);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429020);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429030);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429040);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429050);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429068);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429078);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429088);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429098);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004290F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429100);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429118);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429130);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429140);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429150);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429160);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429170);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429180);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004291F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429208);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429218);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429228);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429238);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429250);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429260);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429278);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429288);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004292F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429318);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429330);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429350);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429368);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429390);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004293E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429408);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429420);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429430);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429448);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429458);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429468);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429480);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429498);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004294F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429508);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429518);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429530);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429540);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429560);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429570);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429588);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004295F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429600);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429620);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429638);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429650);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429660);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429678);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429688);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004296F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429708);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429720);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429730);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429740);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429750);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429760);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429778);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429788);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429798);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297E8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004297F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429808);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429830);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429858);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429868);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429878);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429888);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429898);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004298E8);

void mnuLoadMovieRollSprite(void) {
    D_00437AB0[1] = effLoadIndexedResource(D_00437AC0, "staff_01.spr", 0);
}

void func_002A6000(void) {
    mnuLoadStaffFonts();
}

void func_002A6018(void) {
    mnuUnloadStaffFonts();
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6030);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6180);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6480);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429938);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6858);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6C28);

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSpriteSlideBar(SlideBar *bar) {
    u32 sprite = D_00437AB0[1];

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_00306CD0(0, 0, 0, bar->pos / 2, 0, sprite, 9, 0x53);
    if (bar->active == 0) {
        bar->pos -= 8;
    } else {
        bar->pos += 8;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6D28);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6D68);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A6F88);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7260);

void mnuTitleSetPaletteTransition(u32 *state, s32 mode) {
    switch (mode) {
    case 2:
        state[1] = 0;
        mode = 0;
        state[2] = 0;
        func_002A7260(state, 0);
        break;
    case 3:
        state[1] = 0x200;
        mode = 1;
        break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A73C0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7560);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A75A8);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7730);

void func_002A78B0(void) {
    s64 temp_v0;

    D_00435BAC = 2;
    func_002A2408();
    func_002A2550();
    func_002A6018();
    do {
        temp_v0 = sdfGraphHasPendingWorkInterruptSafe();
    } while (temp_v0 != 0);
    func_003298C0(*D_00437AB0);
    D_00437AB0 = (u32 *)0x0;
}

void func_002A7900(void) {
    func_003054E8(D_00437AB0[1]);
    while (sdfGraphHasPendingWorkInterruptSafe() != 0) {
    }
    func_003458E8(0);
}

extern u32 D_00437AB4;

extern u32 D_00437AB8;

void func_002A7938(void) {
    D_00437AB8 = 4;
    D_00437AB4 = 0;
    mnuLoadMovieRollSprite();
    func_003458E8(1);
    func_003458F0(0x80, 0x60, 0x180, 0x100, 0x80808080);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7980);

u32 mnuStartStaffMovieRequest(void) {
    func_002A7980();
    return 0xffffffff;
}

s32 mnuStopStaffTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00429938, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00429968, 1);
    return 0;
}

s32 mnuMovieDraw(void) {
    func_00345BA0(D_003E5608, D_003803C8);
    return 0;
}

extern char D_0042A338[]; /* "mnuMovieDraw" */

void func_002A7A98(u32 resource, void *data) {
    if (D_00437ACC == 0) {
        func_00346778(D_003E5608, data, resource);
        D_00437ACC = kwlnTaskCreate(D_0042A338, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

extern struct {
    u32 handle;
    u8 data[20];
} D_003E4C48[];

void func_002A7AF0(index)
s32 index;
{
    u32 *entry = (u32 *)&D_003E4C48[index];
    func_002A7A98(*entry, entry + 1);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7B28);

extern char D_0042A348[];

extern char D_0042A380[];

extern u32 D_00437AD0;

s32 func_002A7D28(s32 procedure) {
    if (D_00437ACC == 0) {
        func_0035B6E0(D_0042A348);
        return -1;
    }
    func_002A7B28(D_003E5608, D_003803C8);
    D_00437AD0++;
    if (D_00437AD0 == 0x1E) {
        func_0035B6E0(D_0042A380, procedure);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7DB0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429968);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429978);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429998);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_004299F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429A98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429AF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429B98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429BF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429C98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429CF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429D98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429DF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429E98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429EB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429ED8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429EF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F18);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F38);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F58);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F78);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429F98);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FB8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FD8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_00429FF8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A018);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A038);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A058);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A078);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A098);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A0F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A118);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A138);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A158);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A178);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A198);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A1F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A218);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A238);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A258);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A278);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A298);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2B8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A2F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A318);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A338);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A348);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A380);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A7E60);

void func_002A7F98(s32 index) {
    u32 *entry = (u32 *)&D_003E4C48[index];
    func_002A7E60(*entry, entry + 1);
}

void func_002A7FD0(void) {
    if (D_00437ACC == 0) {
        return;
    }
    func_00346988(D_003E5608);
    kwlnTaskDestroyWithHierarchy(D_00437ACC, 0);
    D_00437ACC = 0;
}

void func_002A8008(void) {
    func_00346A60(D_003E5608);
}

extern u8 D_003E563C[];

u8 func_002A8028(void) {
    return D_003E563C[0];
}

u8 func_002A8038(void) {
    return D_003E5608[0];
}

s32 mnuSetFrameDivisor(void) {
    func_00345488(0x3c / D_00435BAC);
    return 0;
}

void mnuCreateMovieManagerTask(void) {
    func_003456F8();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 func_002A80C0(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_002A7AF0(temp_v0);
    D_00437AE8 = 0;
    return 1;
}

u32 func_002A80F0(void) {
    func_002A7FD0();
    D_00437AE8 = 0;
    kwlnDrawEnableDc8(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8120);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A81C8);

INCLUDE_ASM(const s32, "game/code_0029BC58", mnuClearMovieList);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8268);

u32 mnuGetMovieListNodeAtOffset(void) {
    u8 *state = (u8 *)D_00457E48;
    u32 entry = D_00457E48[1];
    s32 remaining = *(s16 *)(state + 0xA);
    if (entry != 0 && remaining > 0) {
        do {
            entry = *(u32 *)entry;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return entry;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8610);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A87F0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A88A0);

void mnuCreateMovieViewerTask(void) {
    func_002A8268();
    D_00457E48[0] = kwlnTaskCreate(D_0042A418, 0x2b02, 1, 0, func_002A88A0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", mnuDestroyMovieViewerTask);

void func_002A8B78(void) {
    D_00457E60[2] = 1;
    D_00457E60[3] = 1;
}

void func_002A8B90(void) {
    u32 *task = (u32 *)D_00457E60;
    task[1] = 0x10002010;
    task[2] = (u32)D_003E5608;
    func_002A8B78();
}

/* DDS2 twin of DDS1 func_00270B10. */
void func_002A8BC8(void) {
    u8 *state = D_00457E60;

    if (state[2] != 0) {
        *(s32 *)(state + 0xC) = **(s32 **)(state + 4);
    }
    if (state[3] != 0) {
        memcpy(state + 0x10, *(void **)(state + 8), 0x40);
    }
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A418);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A8C80);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9068);

extern u32 D_00438FF8[2];

extern u32 D_003E6848[];

extern char D_0042A950[];

extern u32 effLoadIndexedResource(char *, u32, u32);

void mnuLoadCampResources(void) {
    s32 i;
    for (i = 0; i < 2; i++) {
        D_00438FF8[i] = effLoadIndexedResource(D_0042A950, D_003E6848[i * 2], 1);
    }
}

extern void func_00305068(u32);

extern void effResolveAndReleaseResource(u32);

void func_002A91A0(u32 *destination) {
    s32 i;
    for (i = 0; i < 2; i++) {
        effResolveAndReleaseResource(D_00438FF8[i]);
        destination[i] = D_00438FF8[i];
    }
}

void func_002A9200(u32 *destination) {
    s32 remaining = 1;
    u32 offset = 0;
    do {
        func_00305068(*(u32 *)((u8 *)D_00438FF8 + offset));
        *(u32 *)((u8 *)destination + offset) = 0;
        offset += 4;
    } while (--remaining >= 0);
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A440);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A450);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A460);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A470);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A480);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A490);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4D8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A4F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A500);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A518);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A530);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A540);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A558);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A570);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A588);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A598);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5C8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A5F8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A608);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A620);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A638);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A650);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A668);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A680);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A690);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A6F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A700);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A710);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A720);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A730);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A740);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A750);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A760);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A770);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A780);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A790);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A7F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A800);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A810);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A820);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A830);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A840);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A850);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A870);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A888);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8A0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8B0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8C0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8D0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8E0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A8F0);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A900);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A910);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A920);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A930);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A940);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042A950);

u8 *mnuGetStaffCategoryEntries(s32 kind, s32 *count, u8 *work) {
    switch (kind) {
    case 1:
        *count = 1;
        return work + 0xC8;
    case 2:
        *count = 1;
        return work + 0xC4;
    case 3:
        *count = 2;
        return work + 0x68;
    case 4:
        *count = 9;
        return work + 0xCC;
    case 5:
        *count = 1;
        return work + 0xF0;
    default:
        *count = 0;
        return 0;
    }
}

void func_002A92D8(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        u8 *slot = D_00435DD0 + 0xA60 + i * 0x1C4;

        if ((*(u16 *)slot & 1) != 0) {
            s32 index = *(u16 *)(slot + 4) + D_00437B73;

            effResolveAndReleaseResource(*(u32 *)(list + index * 4 - 4));
        }
    }
}

void movReleaseCategoryModels(s32 kind, u8 *work) {
    s32 count;
    s32 *entries = (s32 *)mnuGetStaffCategoryEntries(kind, &count, work);
    if (kind != 4) {
        s32 i;
        for (i = 0; i < count; i++) {
            effResolveAndReleaseResource(entries[i]);
        }
    } else {
        func_002A92D8(entries, count, work);
    }
}

void func_002A93F8(s32 kind, u8 *work) {
    s32 count;
    s32 i = 0;
    u8 *buffer = mnuGetStaffCategoryEntries(kind, &count, work);

    if (count > 0) {
        u32 *handles = (u32 *)buffer;
        do {
            func_00305068(*handles++);
        } while (++i < count);
    }
}

void func_002A9460(s32 kind, u8 *work) {
    s32 old = *(s32 *)(work + 0xAA4C);
    if (kind == old) {
        return;
    }
    if (old != 0) {
        func_002A93F8(old, work);
    }
    if (kind != 0) {
        movReleaseCategoryModels(kind, work);
    }
    *(s32 *)(work + 0xAA4C) = kind;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A94D0);

void movReleaseTitleEffects(u32 *state) {
    u32 *handles = state + 0x110 / 4;
    u32 i;
    effDestroyPackedBatch(state[0x100 / 4]);
    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(*handles++);
    }
}

void func_002A95B0(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002BCD90(arg0, arg3, *arg1, 1, arg1[1], 0x2d, arg1[1], 0x1d);
    func_002BC498(arg0, arg1[1]);
    func_002BC5D0(arg0, arg1 + 4);
    func_002BC600(arg0, arg1 + 0xc);
    mnuRegisterResourceHandles(arg0, arg1 + 0x14);
    func_002BCA98(arg0);
    func_002BE6E8(arg0, arg1[1]);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9640);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9788);

s32 movAreTitleEffectsReady(s32 mode, u32 *state) {
    u32 *entry;
    u32 *tail;
    s32 i;
    func_00303E88(mode);
    i = 0;
    entry = state;
    for (; i < 2; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 4;
    for (; i < 16; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 0x50 / 4;
    for (; i < 5; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    tail = state + 2;
    i = 0;
    for (; i < 2; i++) {
        if (*tail++ == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9908);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9A40);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9AB8);

void func_002A9BC8(s32 arg0, u32 arg1, u32 arg2, s32 arg3, u32 arg4,
                                    u32 arg5) {
    func_00306F80(arg0 + 0x60, arg1, arg2, 1, *(u32 *)(*(s32 *)(arg3 + 0x30) + 100), 10,
                                arg5);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9BF8);

extern s32 func_002B9FF8(s32, s32, s32);

extern s32 func_002A9BF8(void *, s32, s32, s32, u8 *, void *);

extern void func_002B9568(s32, s32);

extern void func_002BAF10(u8 *);

extern void func_002BAF50(s32, u8 *);

extern u8 D_003E56D0[], D_003E56F0[], D_003E5708[], D_003E6978[], D_003E6998[];

void mnuStaffInitResourceLists(u8 *work) {
    u8 *ctx = work + 0xB10C;
    s32 list;

    *(s32 *)(work + 0xF4) = func_002B9FF8(0, *(s32 *)(work + 0x64), *(s32 *)(work + 0x100));
    *(s32 *)(work + 0xF8) = func_002B9FF8(1, *(s32 *)(work + 0x64), *(s32 *)(work + 0x100));
    *(s32 *)(work + 0xFC) = func_002B9FF8(3, *(s32 *)(work + 0x64), *(s32 *)(work + 0x100));
    *(s32 *)(work + 0x104) = func_002A9BF8(D_003E56D0, 8, 0x1C0, 0x10, work, D_003E6978);
    list = func_002A9BF8(D_003E56F0, 5, 0x1C0, 0x10, work, D_003E6998);
    *(s32 *)(work + 0x108) = list;
    func_002B9568(list, 0x100);
    list = func_002A9BF8(D_003E5708, 2, 0x1C0, 0x10, work, 0);
    *(s32 *)(work + 0x10C) = list;
    func_002B9568(list, 0x100);
    func_002BAF10(ctx);
    func_002BAF50(*(s32 *)(work + 0x104), ctx);
}

extern void func_002B9520(u32);

extern void mnuReleaseResourceList(u32);

void func_002A9F08(u8 *work) {
    u32 *handles = (u32 *)(work + 0x104);
    u32 i;

    for (i = 0; i < 3; i++) {
        func_002B9520(*handles++);
    }
    mnuReleaseResourceList(*(u32 *)(work + 0xF4));
    mnuReleaseResourceList(*(u32 *)(work + 0xF8));
    mnuReleaseResourceList(*(u32 *)(work + 0xFC));
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9F78);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002A9FF0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA068);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA0D8);

void mnuDestroyStaffMenuTask(u32 task) {
    u8 *work = (u8 *)func_00101958(task);
    if (work == NULL) {
        return;
    }
    func_002C3FC8(work + 8, task);
    func_002A9F08(work);
    func_002BB418(*(u32 *)(work + 0x118));
    mnuShutdownContext(work + 0x284);
    func_0026C728();
    mnuDestroyEffectResources(work + 0x11c);
    func_002A9A40(work);
    movReleaseTitleEffects(work);
    func_00303D58(*(u32 *)(work + 0x5c));
    func_003297C8(*(u32 *)work);
    D_00437B72 = 2;
    func_003425D8();
}

u32 func_002AA278(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C1B70(temp_v0 + 0xaa50, 0x53);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA2A8);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA08);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA18);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA360);

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437B78, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA08, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA18, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = D_00437B72;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437B72 = 0;
    }
    return 0;
}

u8 mnuIsFadeIdle(void) {
    s64 temp_v0;

    temp_v0 = kwlnFadeIsActive();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA530);

extern u32 D_003E5710[];

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_0019F460(0x340, 0x148, 0, 0xa09dc35a,
                                      D_003E5710[index], 0);
    func_0019D550(object, 1, 0x54);
    func_0019C5B0(object);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AA9D8);

extern void func_002AA9D8(u32, u32, u32, u32, u32, u32, u32, u32, u32);

void func_002AAC70(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002AA9D8(a, b, c, d, e, f, 0, 0, g);
}

void func_002AAC98(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    func_002AAC70(a, b, c, d, e, 0, f);
}

INCLUDE_ASM(const s32, "game/code_0029BC58", func_002AACB8);

void func_002AAE80(u32 arg0) {
    func_002AACB8(0, arg0);
}

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_0029BC58", D_0042AC70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379C8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379D8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379E8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379F8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_004379FC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A1C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A2C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A34);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A3C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A40);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A70);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A80);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A88);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A90);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437A98);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AA8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AB8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AC8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ACC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AD8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437ADC);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE4);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AE8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AF0);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437AF8);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B00);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B08);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B10);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B18);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B20);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B28);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B30);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B38);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B40);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B48);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B50);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B58);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B60);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B68);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B6C);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B6E);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B72);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B73);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B78);

INCLUDE_SDATA(const s32, "game/code_0029BC58", D_00437B80);

