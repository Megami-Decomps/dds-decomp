#include "common.h"

extern u64 duplicateFileJob(u64);

extern u64 func_003292A8(u64);

extern u64 func_003298F8(u64);

extern s64 sdfDevCreateCommandState(u64);

extern u64 func_0033EB30(s64);

extern s32 D_00437D84;

extern u32 D_00437CE8;

extern u32 D_00437CEC;

extern u32 D_00437CF0;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);
extern u32 func_0019F460(s32, s32, u64, u64, u64, u64);
extern u32 D_00439004;
extern u32 D_00439008;

extern u32 D_0043900C;

extern u32 func_0019CE78(u32, u32, u32, u32, u32);

extern s32 D_00437CD8;

extern s32 D_00437CE4;

extern s32 D_00437D38;

extern u32 D_00437D48;

extern u32 D_00437CD0;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern u32 D_00437D34;

extern u32 D_00437D04;

extern s32 D_00435DD0;

extern u32 D_00437DEC;

extern s32 D_00439058;

extern u32 D_00437E08;

extern u32 func_002DDF48(u32);

extern s8 D_00437CD4;
extern s32 D_00437D7C;
extern s32 D_00437D88;
extern void func_0035C860(void *buffer, const char *format, s32 titleId, s32 slot);

extern s32 func_002CB130(void);

extern void fileReqBegin(s32 arg0);

extern void *func_002CABC0(void);

extern void func_002CB2C0(void);

extern s32 func_002CB100(void);

extern s32 fileReqGetSize(s32 arg0);

extern u32 D_00439020;

extern void func_002CB678(void);

extern void *func_002CB710(u32 arg0);

extern void func_002CB740(void);

extern s32 D_00437D10;

extern void func_002C9710(void *arg0, s32 arg1);

extern void func_002C9328(u32 arg0, void *arg1);

extern void *func_002CB988(void);

extern void *func_002CBCA0(void);

extern void *func_002CC038(void);

extern void func_002CC0F8(void);

extern void func_001004A0(void);
extern s8 D_0037F510[];
extern void soundSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern void func_00342580(s32 command);
extern void beginFileWait(s32 result);

extern void func_002C92D0(u32 arg0);

extern void handleSaveDetectionResult(void);

extern char D_0042B720[];

typedef struct KwlnTask KwlnTask;

extern KwlnTask *func_00101740(const char *name);

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    u32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    u32 unkC;   /* 0x0C */
    u8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    u32 unk14;  /* 0x14 */
} LoadCtx374A0;

extern LoadCtx374A0 D_003E7FD8;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern u32 D_003E8008[];

extern u32 D_003E9150[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
} Cb3714C;

extern Cb3714C D_003E916C[];

extern void func_002D31C0(void *src);

extern void func_002D4818(void *dst, void *src);

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
typedef struct EffDispatchEntry {
    void *(*func)(void *); /* 0x00 handler, may be NULL */
    u8 pad04[0x08];        /* 0x04 */
    u32 unk0C;             /* 0x0C fallback selector (create table only) */
    u8 pad10[0x18];        /* 0x10 */
} EffDispatchEntry; /* 0x28 */

/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct EffParamWork {
    u16 id;       /* 0x00 effect kind */
    u8 pad02[2];  /* 0x02 */
    void *data;   /* 0x04 parameter block */
} EffParamWork; /* 0x08 */

extern EffDispatchEntry D_003E917C[];

extern EffDispatchEntry D_003E9180[];

extern EffDispatchEntry D_003E9184[];

extern EffDispatchEntry D_003E9188[];

extern EffDispatchEntry D_003E918C[];

extern s8 D_00437DE5;

extern void func_002D02D0(void);

extern void func_002CAED0(void);

void func_002C9660(void) {
    func_0034F490();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9678);

void func_002C96D0(void) {
    if (D_00437CD8 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437CD8, 1);
        D_00437CD8 = 0;
        D_00437CD4 = 0;
    }
}

s8 func_002C9700(void) {
    return D_00437CD4;
}

void func_002C9708(void) {
}

void func_002C9710(void *buffer, s32 slot) {
    s32 titleId = 0x52a0;
    if (D_00437D7C != 0) {
        titleId = 0x51ee;
    }
    D_00437D88 = titleId;
    func_0035C860(buffer, "BASLUS-%05d-new-%d", titleId, slot);
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileReqGetSlotCode);

u32 func_002C9788(void) {
    return 0x1e840;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9798);

u8 func_002C97D8(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

u8 func_002C97F8(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

void func_002C9818(s32 x, s32 y, u64 width, u64 height) {
    u32 handle = func_0019F460(x << 4, y << 3, 0, width, height, 0);
    D_00439004 = handle;
    func_0019D530(handle, 1);
    func_0019C5B0(D_00439004);
}

void func_002C9860(s32 x, s32 y, u64 width, u64 height) {
    u32 handle = func_0019F460(x << 4, y << 3, 0, width, height, 0);
    D_00439008 = handle;
    func_0019D120(handle, 3);
    func_0019D530(D_00439008, 1);
    func_0019C5B0(D_00439008);
}

void func_002C98B8(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_0019D1D0(1);
    D_0043900C = func_0019CE78(arg3, 0, 0, 0, 0);
    func_0019D1E0(1);
    func_0019D058(D_0043900C, 1);
    func_0019D100(D_0043900C, arg0 << 4, arg1 << 3);
    func_0019D178(D_0043900C, arg2);
    func_0019D550(D_0043900C, 0, 0x56);
    func_0019C5B0(D_0043900C);
    func_0019D1F8(0x54);
}

void func_002C9970(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F5E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D530(temp_v0, 1);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C99C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9AF0);

INCLUDE_ASM(const s32, "game/code_002C9660", isLoadStepComplete);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9BD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9CF8);

void func_002CA1D8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00437D38;
    D_00437D38 = arg0;
    if (temp_v0 == 0) {
        D_00437D48 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA1F0);

void func_002CA638(void) {
    func_002C91B8(D_00437CD0);
    func_002C91E8(D_00437CD0, 0);
    func_002C91E8(D_00437CD0, 1);
    func_002C91E8(D_00437CD0, 2);
    func_002C91E8(D_00437CD0, 3);
    func_002C91E8(D_00437CD0, 4);
    func_002C91E8(D_00437CD0, 5);
    func_002C91E8(D_00437CD0, 6);
    func_002C91E8(D_00437CD0, 7);
    func_002C91E8(D_00437CD0, 8);
    func_002C91E8(D_00437CD0, 9);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA6D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA7B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA828);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA888);

INCLUDE_ASM(const s32, "game/code_002C9660", beginFileWait);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA08);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA50);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA90);

void *func_002CAAD0(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    return func_002CB130;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAAF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB40);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB80);

void *func_002CABC0(void) {
    func_002CA638();
    func_002CA1D8(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    fileReqBegin(D_00437CD0);
    return func_002CB2C0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", resetFileSelection);
extern void func_001027D8(s32, const s32 *, s32, s32);
extern void func_002D09C8(void);


u32 func_002CAC80(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

void *func_002CACA8(void) {
    s32 mode = 0;
    if (D_00437D7C != 0) {
        return func_002D09C8;
    }
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void func_002CACF0(void) {
    func_002CACA8();
}

void *func_002CAD08(void) {
    s32 mode = 2;
    D_00437D34 = 1;
    if (D_00437D7C != 0) {
        return func_002D09C8;
    }
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void func_002CAD60(void) {
    func_002CA638();
    func_002CA1D8(1);
    D_00437D3C = 0;
    D_00437CF8 = 0;
    resetFileSelection();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAD90);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAE58);

extern u32 D_00439030;
extern u32 D_00437CE0;
extern char D_0042B6A8[];

void func_002CAEA8(void) {
    D_00439030 = 0;
    D_00437CE0 = func_002C80C8(D_0042B6A8);
    resetFileSelection();
}

void func_002CAED0(void) {
    resetFileSelection();
}

void func_002CAEE8(void) {
    D_00439030 = 0;
    D_00437CE0 = func_002C80C8(D_0042B6A8);
    func_002CAA08();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAF10);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAF48);

INCLUDE_ASM(const s32, "game/code_002C9660", countSelectableFiles);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB100);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB130);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB1F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB2C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB4D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB5A0);

INCLUDE_ASM(const s32, "game/code_002C9660", updateFileWait);

void *func_002CB660(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return func_002CB678;
}

void func_002CB678(void) {
    if (func_002C97D8((u32)D_0037F510[0x21] >> 31) ||
        func_002C97D8((u32)D_0037F510[0x23] >> 31)) {
        func_002CA1D8(0);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        if (D_00437D7C == 0) {
            func_00342580(0x310000);
        }
        if (D_00439020 == (u32)-1) {
            beginFileWait(-1);
            return;
        }
        ((void (*)(void))D_00439020)();
    }
}

void *func_002CB710(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    fileReqBegin(D_00437CD0);
    return func_002CB740;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB740);

void *func_002CB948(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_002C9710(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CB988;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB988);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveSlotWriteResult);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBA90);

INCLUDE_ASM(const s32, "game/code_002C9660", resetSaveSlotMetadata);

void *func_002CBC60(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    func_002C9710(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CBCA0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBCA0);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveDirectoryWriteResult);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBDD0);

INCLUDE_ASM(const s32, "game/code_002C9660", clearSaveSlotMetadata);

INCLUDE_ASM(const s32, "game/code_002C9660", prepareSaveDirectory);

void *func_002CBF80(void) {
    u8 buf[0x50];
    u32 entry = D_00437CD0;
    s32 v = func_002C9280(entry);

    buf[0] = 0x2F;
    func_002C9710(&buf[1], v);
    func_002C9328(entry, buf);
    return func_002CC038;
}

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveSearchResult);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC038);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC098);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC0F8);

void *func_002CC140(void) {
    func_002C96D0();
    return func_002CC0F8;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC168);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6B8);

INCLUDE_ASM(const s32, "game/code_002C9660", requestBaseIcon);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC210);

INCLUDE_ASM(const s32, "game/code_002C9660", chooseSaveLoadPath);

INCLUDE_ASM(const s32, "game/code_002C9660", beginFileRequest);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC530);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveLoadResult);

INCLUDE_ASM(const s32, "game/code_002C9660", dispatchSaveReadCallback);

INCLUDE_ASM(const s32, "game/code_002C9660", finishFileRequest);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveDetectionResult);

void *func_002CC760(void) {
    func_002CA1D8(5);
    func_001004A0();
    func_002C92D0(D_00437CD0);
    return handleSaveDetectionResult;
}

INCLUDE_ASM(const s32, "game/code_002C9660", selectFileMenuBranch);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC7F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC8E0);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveSetupResult);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC9F0);

extern s32 func_002D0968();

void *func_002CCAA8(void) {
    func_002CA1D8(13);
    return func_002CB660((u32)func_002D0968);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CCAD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CD028);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CDF38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CDFD8);

void func_002CE1A8(void) {
    func_002CA1D8(0);
    D_00437D04 = 0;
    D_00437CE8 = 0;
    D_00437D18 = 0xffffffff;
    D_00437D34 = 0;
    D_00437D3C = 0;
    D_00437D40 = 0;
    D_00437CF8 = 0;
    D_00437D08 = 0;
    D_00437CF0 = 0;
    D_00437CFC = 0;
    D_00437CEC = 0;
    D_00437D14 = 0;
    D_00437D1C = 0;
    D_00437D20 = 0;
    D_00437D44 = 0;
    D_00437D48 = 0;
    func_002CA638();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE208);

void func_002CE738(void) {
    func_002CE758();
}

void func_002CE750(void) {
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE758);

u32 func_002CE920(void) {
    return 1;
}

s32 func_002CE928(void) {
    return func_00101740(D_0042B720) != NULL;
}

u32 func_002CE950(void) {
    return D_00437D34;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE958);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEE70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEEC0);

u32 func_002CF928(void) {
    return D_00437D04;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF930);

void func_002CF958(u32 arg0) {
    D_003E8008[0] = arg0;
    D_003E7FD8.unkC = 0x80;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF978);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF9D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFC38);

extern u8 D_003E8018[];
extern s32 D_003E7FE4[];

void func_002CFD48(void) {
    s32 *state = (s32 *)D_003E8018;
    D_003E8018[1] = 0;
    D_003E7FE4[0] = 0x80;
    D_003E8018[0] = 1;
    D_003E8018[2] = 2;
    state[0x34 / 4] = 0x80;
    state[0x38 / 4] = 0x80;
    state[0x3C / 4] = 0x80;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFD80);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B720);

INCLUDE_RODATA(const s32, "game/code_002C9660", jtbl_0042B730);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B770);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B7C0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B868);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFF38);

INCLUDE_ASM(const s32, "game/code_002C9660", fileLoadSetMode);

void func_002D0148(void) {
    D_003E7FD8.unk14 = 0;
    D_003E7FD8.unk10 = 0;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0160);

void func_002D02D0(void) {
    if (func_002C97D8((u32)D_0037F510[0x21] >> 31) ||
        func_002C97D8((u32)D_0037F510[0x23] >> 31)) {
        func_002CA1D8(0);
        soundSetSequenceVolumePan(8, 0x7f, 0x3f);
        ((void (*)(void))D_00439020)();
    }
}

void *func_002D0340(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return func_002D02D0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0358);

extern u32 D_00439024;
extern u32 D_00439028;
extern u32 D_0043902C;
extern s32 func_002D0358();

void *func_002D0470(u32 ready, u32 completed, u32 cancelled, u32 state) {
    D_00439024 = ready;
    D_00439028 = completed;
    D_0043902C = cancelled;
    D_00437D18 = state;
    D_00437D20 = 0;
    return func_002D0358;
}

u32 func_002D0490(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B8F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0498);

extern s32 func_002D0498();

void *func_002D0678(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    func_00342580(0x310000);
    return func_002D0498;
}

void func_002D06B0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 0;
    func_002D0678();
}

void func_002D06D0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 1;
    func_002D0678();
}

void func_002D06F0(void) {
    func_002CA1D8(0);
    D_00437D1C = 12;
    func_002D0470((u32)func_002D06B0, (u32)func_002D06D0, 0, 0);
}

void *func_002D0730(void) {
    D_00437D18 = -1;
    D_00437D1C = 0;
    func_002CA1D8(0x18);
    *(u8 *)(D_00437D84 + 0x30) = 1;
    return func_002D0340((u32)func_002D06F0);
}

void *func_002D0770(void) {
    func_002CE1A8();
    return func_002CAED0;
}

void func_002D0798(void) {
    func_002CA1D8(0);
    D_00437D1C = 11;
    func_002D0470((u32)func_002D0730, (u32)func_002D0770, (u32)func_002D0770, 0);
}

extern s32 func_002D0810();

void *func_002D07D8(void) {
    if (isLoadStepComplete() != 0) {
        return func_002D0340((u32)func_002D0810);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0810);

extern u32 D_00435CD4;

s32 func_002D08A0(void) {
    s32 mode;
    if (D_00435CD4 & 2) {
        return 0;
    }
    D_00437D18 = -1;
    mode = 3;
    D_00437D1C = 0;
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void *func_002D08F0(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    return func_002D08A0;
}

void func_002D0920(void) {
    func_002CA1D8(0);
    D_00437D1C = 10;
    *(u8 *)(D_00437D84 + 0x30) = 0;
    func_002D0470((u32)func_002D0770, (u32)func_002D0678, (u32)func_002D08F0, 0);
}

s32 func_002D0968(void) {
    u32 state = func_002CF928();
    u32 block;
    s32 next = (s32)func_002D09C8;
    if (state != 0) {
        if (state == 2) {
            block = D_00437D84;
            *(u16 *)(block + 0x36) = 0;
            func_002D0D90(block);
            soundSetSequenceVolumePan(8, 0x7f, 0x3f);
            next = (s32)func_002D0810;
        } else {
            next = 0;
        }
    }
    return next;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D09C8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0A20);

void func_002D0A90(void) {
    if (D_00437D84 != 0) {
        func_003298C0(*(u32 *)(D_00437D84 + 0x3c));
        D_00437D84 = 0;
    }
}

extern char D_0042B920[];

void func_002D0AB8(void) {
    func_0035B6E0(D_0042B920);
}

extern char D_0042B938[];

void func_002D0AD8(void) {
    u8 *state = (u8 *)D_00435DD0;
    u32 money = *(u32 *)(state + 0x20);
    *(u32 *)(state + 0x1E650) = money;
    func_0035B6E0(D_0042B938, money);
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B920);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B938);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0B08);

void func_002D0D00(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00435DD0;
    *(u32 *)(D_00435DD0 + 0x20) = *(u32 *)(arg0 + 0x20);
    *(u32 *)(temp_v0 + 0x24) = *(u32 *)(arg0 + 0x24);
    *(u32 *)(temp_v0 + 0x28) = *(u32 *)(arg0 + 0x28);
    *(u32 *)(temp_v0 + 0x2c) = *(u32 *)(arg0 + 0x2c);
}

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

void func_002D0D28(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0D90);

INCLUDE_ASM(const s32, "game/code_002C9660", fileLoadStateChanged);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0EB0);

void func_002D0EC8(void) {
    *(u32 *)(D_00435DD0 + 0xa54) = D_00437DEC;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0ED8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B970);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B980);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B998);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9B0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9D0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9F0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA10);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA30);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA50);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA70);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA90);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0FB8);

void func_002D1038(u32 arg0) {
    func_002D0FB8(arg0, D_00435DD0 + 0xa54);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D11F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1300);

INCLUDE_ASM(const s32, "game/code_002C9660", configTasksDestroy);

s32 func_002D13F0(void) {
    s32 state = D_00437DE5;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437DE5 = 0;
    }
    return 0;
}

u32 func_002D1428(s32 arg0) {
    if (arg0 < 4) {
        return *(u32 *)((s32)arg0 * 4 + D_00439058 + 0x10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAF0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB00);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1450);

INCLUDE_ASM(const s32, "game/code_002C9660", startQueuedFileLoad);

u32 func_002D1910(void) {
    u32 temp_v0;

    temp_v0 = 0xffffffff;
    if ((*(u32 *)(D_00439058 + 0x28) & 0x80000000) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1930);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D27A0);

void func_002D2C50(void) {
    func_002E6390();
    loadWindEffectTexture();
    loadScalyEffectTexture();
    func_002D2CA8();
}

void func_002D2C80(u32 arg0) {
    D_00437E08 = D_00437E08 | arg0;
}

void func_002D2C90(u32 arg0) {
    D_00437E08 = D_00437E08 & ~arg0;
}

void func_002D2CA8(void) {
    D_00437E08 = 0;
}

u32 func_002D2CB0(s32 arg0) {
    return D_003E9150[arg0];
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2CC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2D48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2EB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2FB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3128);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D31C0);

INCLUDE_ASM(const s32, "game/code_002C9660", createFileJob);

INCLUDE_ASM(const s32, "game/code_002C9660", resolvePrimaryFileBuffer);

INCLUDE_ASM(const s32, "game/code_002C9660", resolveSecondaryFileBuffer);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D34B8);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobDestroy);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobFreePrimaryBuffer);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobFreeSecondaryBuffer);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobCreateChild);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobNotifyPair);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobNotifyComplete);

void func_002D3710(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_003E916C[idx].cb(data);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3748);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3788);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D37C8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3808);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3848);

void func_002D38E8(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = func_003298F8(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D3848(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D39C8);

void func_002D3A68(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = func_003298F8(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D39C8(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3B48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3CB8);

INCLUDE_ASM(const s32, "game/code_002C9660", duplicateFileJob);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3DF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3E98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3F08);

void func_002D3F80(u32 arg0) {
    s32 temp_v0;

    memset(arg0, 0, 0x90);
    temp_v0 = (s32)arg0;
    *(u32 *)(temp_v0 + 0x84) = 1;
    *(u8 *)(temp_v0 + 0x88) = 8;
    *(u8 *)(temp_v0 + 0x89) = 0;
    *(u8 *)(temp_v0 + 0x8a) = 0;
    func_002D3F08(arg0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueAppend);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueInsertAfter);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueRemove);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueCreate);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobCreate);

void func_002D4120(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4138);

void func_002D4380(u32 arg0, u32 arg1) {
    func_002D4138(arg1);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4398);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4548);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D45B8);

void func_002D46A0(u8 *owner) {
    u8 *job = *(u8 **)(owner + 0x8C);
    while (job != 0) {
        fileJobNotifyComplete(*(u32 *)(job + 0x90));
        job = *(u8 **)(job + 0xAC);
    }
    *(u32 *)(owner + 0x84) = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D46F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4818);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D48D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D49B8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4A98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4AB0);

f32 func_002D4AC8(s32 object) {
    return *(f32 *)(object + 0x60);
}

u32 func_002D4AD0(s32 arg0) {
    return *(u32 *)(arg0 + 100);
}

void func_002D4AD8(void *dst, void *src) {
    s128 vec;
    func_002D31C0(src);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_002D4818(dst, &vec);
}

INCLUDE_ASM(const s32, "game/code_002C9660", appendFileJob);

void duplicateAndAppendFileJob(u64 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = duplicateFileJob(arg1);
    appendFileJob(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", appendFileJobFromEntry);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4BD8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4CF0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4E60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4F10);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5010);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D50D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D55B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5AA8);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueFindById);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueFindFlaggedById);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueFindBySector);

INCLUDE_ASM(const s32, "game/code_002C9660", fileQueueGetAt);

INCLUDE_ASM(const s32, "game/code_002C9660", findQueuedJobIndex);

s32 func_002D5D90(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(arg0 + 0x8c); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0xac)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5DC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5EC0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5FB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6020);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB28);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6160);

INCLUDE_ASM(const s32, "game/code_002C9660", loadObjectCreateChild);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D62D8);

INCLUDE_ASM(const s32, "game/code_002C9660", loadObjectSetResource);

INCLUDE_ASM(const s32, "game/code_002C9660", loadObjectOpenNamedDevice);

INCLUDE_ASM(const s32, "game/code_002C9660", loadObjectOpenDevice);

INCLUDE_ASM(const s32, "game/code_002C9660", loadObjectOpenAndStartDevice);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6710);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6808);

void func_002D6900(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x48) != 0) {
        releaseEffectReferenceHolder(*(s32 *)(arg0 + 0x48));
    }
    temp_v0 = func_002DDF48(arg1);
    *(u32 *)(arg0 + 0x48) = temp_v0;
}

void func_002D6950(s32 arg0) {
    if (*(s32 *)(arg0 + 0x4c) != 0) {
        clearFileRecordReferences(*(s32 *)(arg0 + 0x4c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6980);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D69B8);

void func_002D7398(u32 arg0) {
    func_002D6980();
    func_002D69B8(arg0);
}

void func_002D73C0(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x4c));
}

void func_002D73D8(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x4c));
}

void func_002D73F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D73F8);

void resetFileSlotStates(s32 arg0) {
    u32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = *(u32 *)(arg0 + 8);
    temp_v2 = 0;
    temp_v1 = *(s32 *)(arg0 + 0x18);
    if (temp_v0 != 0) {
        do {
            temp_v2 = temp_v2 + 1;
            *(u32 *)(temp_v1 + 0x10) = 0xffffffff;
            temp_v1 = temp_v1 + 0x20;
        } while (temp_v2 < temp_v0);
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7458);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7770);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D78E8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7A58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7AC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7B58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D81B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D8A38);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D8AD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D91A0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9228);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9A60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9AF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA2D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA358);

void func_002DAA58(u8 *object, f32 factor) {
    f32 *source = *(f32 **)(object + 0x24);
    f32 *destination = *(f32 **)(object + 0x20);
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DAAE0);

void func_002DB200(u8 *object, f32 factor) {
    f32 *source = *(f32 **)(object + 0x24);
    f32 *destination = *(f32 **)(object + 0x20);
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

void func_002DB288(u8 *node, const f32 *value) {
    if (*(u16 *)node == 7) {
        f32 *sprite = *(f32 **)(node + 0x20);
        sprite[0xFC / 4] = value[0];
        sprite[0x100 / 4] = value[1];
        sprite[0x104 / 4] = value[2];
    }
}

void func_002DB2C0(u8 *node, const f32 *value) {
    if (*(u16 *)node == 7) {
        f32 *sprite = *(f32 **)(node + 0x20);
        sprite[0x108 / 4] = value[0];
        sprite[0x10C / 4] = value[1];
        sprite[0x110 / 4] = value[2];
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB2F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB3E0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBE78);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBED8);

void func_002DC028(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x28));
}

void clearFileRecordReferences(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", acquireFileRecord);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0A8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DC0F0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CFC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D04);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D0C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D14);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D1C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D20);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D24);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D28);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D2C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D30);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D34);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D38);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D3C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D40);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D44);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D48);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D4C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D50);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D54);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D58);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D5C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D60);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D64);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D68);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D6C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D70);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D74);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D78);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D7C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D80);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D84);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D88);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D8C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D90);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D94);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D98);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DC0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E20);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E28);

