#include "common.h"

typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0x10];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
} UiObject;

typedef struct SoundTask {
    u8 enabled;
    u8 unk_01[0xF];
    u8 status;
    u8 unk_11[0xF];
    u16 taskId;
    u8 unk_22[0x2A];
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*process)(void);
        u32 (*playCustomSound)(u8 *);
        s32 (*releaseSound)(u16 *);
        s32 (*acquireSound)(u32 *);
    } callback;
} SoundTask;

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

typedef struct SoundResourceNode {
    u32 flags;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 resourceHandle;
    u32 unk_14;
    struct SoundResourceNode *previous;
    struct SoundResourceNode *next;
} SoundResourceNode;

typedef struct SoundLink {
    void *owner;
    void *sound;
    void *task;
    u16 variant;
    u16 unk_0E;
} SoundLink;

typedef struct SoundResourceLink {
    void *owner;
    void *sound;
    void *task;
    u32 variant;
    u32 unk_10;
} SoundResourceLink;

extern char D_003A5158[]; /* "%sMIDI%04X.SMG" */

extern s32 func_002E92C0(u32);

extern void func_001F0998(void);

extern void func_001F0B90(void);

extern s32 func_001F35A0(u32 *);

extern void func_001F3AA8(s32, s32);

extern u32 finishEarringPlayback(void);

extern s32 func_001F0AC8(u16 *);

extern s32 func_001F09F0(u32 *);

extern void *func_002CFF68(s32);

extern s32 func_001F4398();

extern s32 func_0026A720(void);

extern SoundResourceNode *nbSoundAllocResourceNode(void);

extern u32 D_003BA904;

extern s32 func_002D3EE8(void);

extern u32 D_003BB694;

extern u32 D_003BB698;

extern s8 D_00324530[];

extern u8 D_003583A0[];

extern void *D_00358408[];

extern void *D_00358450[];

extern s32 D_00358510[];

extern s32 D_00359A78[];

extern s32 D_00359A90[];

extern u8 D_0035F5D0[];

extern char D_003BB6B0[];

extern u64 func_0010FA80(void);

extern u32 func_001165A8(u64);

extern s32 func_001D6360();

extern s32 mdlFlagTest(u32);

extern void func_00215FE0(s32);

extern u32 D_003BB5EC;

extern u32 D_003BB5F0;

extern s32 func_00214868(void);

extern u64 func_001C85B8(u64, u64, u64);

extern s32 D_003BAA1C;

extern u32 func_001C1688(void);

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern s32 D_003BB3D8;

extern u32 D_003BB3DC;

extern u32 D_003BD834;

extern u32 D_003BD838;

extern u32 D_003BD830;

extern u32 D_003BB3A8;

extern u32 D_003BB3A4;

extern u32 D_003BB3B0;

extern u32 D_003BB3AC;

extern u32 D_003BB3BC;

extern u32 D_003BB3C4;

extern u32 D_003BB3CC;

extern u32 func_00101A70(s64);

extern u32 D_003BB3C8;

extern s64 kwlnTaskGetTaskByName(u32);

extern s64 func_001019C8(s64);

extern s32 func_001ADCB8(s32);

extern u32 D_003BB3E0;

extern s32 func_001A8DD8(s32, s32 *);

extern s32 battleCheckSpecialAbility(s32, s32);

extern void func_001DEFE0(s32, s32, f32);

extern void func_001B83D8(s32, s32, s32);

extern void func_001D5DF8(u8 *, s32, s32, f32);

extern s32 func_001A17F0(void);

extern s32 D_003BAA68;

extern s32 D_003BAA14;

extern s32 D_003BAA28;

extern s32 D_003BAA10;

extern s32 D_003BAA20;

extern s32 D_003BAA30;

extern s32 D_003BAA4C;

extern s32 D_003BAA50;

extern s32 D_003BAA00;

extern s32 D_003BAA54;

extern s32 D_003BAA60;

extern s32 dds3FindEntryIndex();

extern s32 func_001A1B78(s32);

extern s32 D_003BB2E4;

extern s32 func_00101938(u32);

extern u64 D_003BB2E8;

extern s8 D_00358308[13];

extern u32 D_003BB240;

extern s32 D_003BB244;

extern s32 func_001986E0(u32);

extern u8 D_003D6EB0[0x18];

extern void func_001F5028(s32 arg0);

void func_0019DB88(s32 arg0) {
    for (; arg0 != 0; arg0 = *(s32 *)(arg0 + 0x24)) {
        func_00195388(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DBC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DCB8);

void func_0019DDA8(u32 *arg0, s32 arg1) {
    if (arg1 != 0) {
        *arg0 = 0x280;
        arg0[1] = 0xa10;
    }
    arg0[2] = 0;
    *(u16 *)(arg0 + 3) = 0xffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DDD0);

void func_0019DE18(s32 object) {
    *(u32 *)(object + 0) = 0x560;
    *(u32 *)(object + 4) = 0xC48;
    *(s32 *)(object + 8) = 0;
    *(s32 *)(object + 0xC) = 0;
    *(s16 *)(object + 0x10) = 0;
    *(s16 *)(object + 0x12) = -1;
    *(s16 *)(object + 0x14) = -1;
    *(s16 *)(object + 0x16) = 0;
    *(s32 *)(object + 0x18) = 0;
    *(s32 *)(object + 0x1C) = 0;
    *(s16 *)(object + 0x20) = 0;
    *(s16 *)(object + 0x22) = 0;
}

void func_0019DE58(u32 *object) {
    object[0] = 0;
    object[1] = 0;
    object[2] = 0;
    func_0019E048((s32)object, 0, 0);
}

void func_0019DE88(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    puVar2 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0x1f;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_0019DEB8(s32 arg0, s32 arg1) {
    if (arg1 == 0) {
        *(u8 *)arg0 = 0;
    }
    *(u16 *)(arg0 + 2) = 0;
    *(u16 *)(arg0 + 6) = 0;
    *(u16 *)(arg0 + 4) = 0x40;
    *(u32 *)(arg0 + 8) = 0;
}

void func_0019DED8(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    *(s16 *)(arg0 + 2) = arg1;
    *(s16 *)(arg0 + 4) = arg2;
    *(s16 *)(arg0 + 6) = arg3;
}

void func_0019DEE8(u8 *effect) {
    u32 *handles = (u32 *)(effect + 0xA4);
    if (handles[0] != 0) {
        func_00199900(handles[0]);
        handles[0] = 0;
    }
    if (handles[1] != 0) {
        func_00199900(handles[1]);
        handles[1] = 0;
    }
    if (handles[2] != 0) {
        func_00199900(handles[2]);
        handles[2] = 0;
    }
    *(u32 *)effect &= ~0xF00;
}

void func_0019DF70(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = 0x1f;
    do {
        if (*arg0 != 0) {
            func_002D0918(arg0[0x20]);
            *arg0 = 0;
        }
        temp_v0 = temp_v0 - 1;
        arg0 = arg0 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019DFC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E048);

void func_0019E0F8(u32 arg0) {
    func_0019E130();
    func_0019E320(arg0);
    func_0019E4F8(arg0);
    func_0019EB18(arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E320);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E4F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E7B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E888);

typedef struct SoundSeq {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
} SoundSeq;

extern void func_0019D958(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void soundSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

void sndStepSequenceIndex(SoundSeq *obj, s32 dir) {
    s32 cur = obj->unk12;
    u16 maxv;
    func_0019D958(obj->unk08, cur, obj->unk16, 0);
    if (dir < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = obj->unk16 - 1;
        }
        maxv = obj->unk16;
    } else {
        cur = cur + 1;
        if (cur >= obj->unk16) {
            cur = 0;
        }
        maxv = obj->unk16;
    }
    func_0019D958(obj->unk08, cur, (s16)maxv, 1);
    obj->unk12 = cur;
    obj->unk14 = cur;
    soundSetSequenceVolumePan(1, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EA88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EB18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EBC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019ED78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EE58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F0F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F4C8);

s32 nbSoundVisitQueuedResources(void) {
    s32 node = *(s32 *)D_003D6EB0;
    while (node != 0) {
        func_0019E0F8(*(u32 *)(node + 0xC));
        node = *(s32 *)(node + 4);
    }
    return 0;
}

typedef struct SoundQueue {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
} SoundQueue;

extern SoundQueue D_003D6EA0;

extern void func_0019D0B8(s32 arg0);
extern void func_002D2D00(s32 arg0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F6A0);

void sndFlushMessageQueue(void) {
    u32 node = D_003D6EA0.unk10;
    s32 arg;
    while (node != 0) {
        arg = *(s32 *)(node + 8);
        node = *(u32 *)(node + 4);
        func_0019D0B8(arg);
    }
    func_002D2D00(D_003D6EA0.unk04);
    D_003D6EA0.unk04 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F770);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F850);

extern u8 D_00324510[];

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F9E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FA70);

INCLUDE_ASM(const s32, "game/code_0019DB88", sndCreateTestMsgTasks);

void func_0019FB78(void) {
    if (D_003BB244 != 0) {
        func_002D2D00(D_003BB244);
        D_003BB244 = 0;
    }
    D_003BB240 = (D_003BB240 + 1) & 3;
    if (D_003BB240 != 3) {
        D_003BB244 = func_001986E0(*(u32 *)(D_00358308 + D_003BB240 * 4));
    }
}

s32 func_0019FBD8(void) {
    if (D_00324530[0] < 0) {
        func_0019FB78();
    }
    return 0;
}

typedef struct SndDev {
    u8 unk0[0x10];
    void (*unk10)(void *, s32);
} SndDev;

extern SndDev D_003255A8;
extern u8 D_00358318[];
extern u8 D_00358328[];
extern u8 D_00358338[];
extern s32 func_002D3FD0(s32 size);
extern void func_002D4010(s32 mem);
extern void func_001994D8(s32 arg0, s32 arg1, s32 arg2);
extern void func_00198C70(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 sndUpdateTestMsgTask(void) {
    s32 mem;
    if ((D_003BB244 != 0) && (D_003BB240 != 3)) {
        mem = func_002D3FD0(0x20);
        func_002D4010(mem);
        func_001994D8(mem, 0, 0);
        func_00198C70(D_00358318, D_00358328, D_00358338, 0xFFF, D_003BB244, 0, mem);
        D_003255A8.unk10(&D_003255A8, mem);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1510);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1528);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1540);

void func_0019FCA8(void) {
    func_003003F0("********* AAAA ********\n");
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FCC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FF60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A04C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0910);

u64 *func_001A0A78(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)func_002E13E0(func_002D3FD0(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *func_001A0AD0(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)func_002E13E0(func_002D3FD0(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0B28);

void func_001A0CA0(void) {
    D_003BB2E8 = 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0CB0);

void func_001A0CD8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0xbe0;
    do {
        temp_v0 = temp_v1 + 1;
        mdlFlagClear(temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 0xbff);
}

s32 func_001A0D18(void) {
    s32 state = D_003BB2E4;
    if (state == 0) {
        return 0;
    }
    if ((*(u32 *)(state + 0x1F4) & 1) != 0) {
        func_001F24A8();
        func_001D3F00();
        func_001EFC50();
        func_001F44C0();
        updateBattleScene();
        func_001C8330();
        func_001D0870();
        func_001DA468();
        func_001FB088();
        func_001D4A10();
        func_001DBE68();
        ++*(s32 *)(D_003BB2E4 + 0x1F0);
    } else {
        func_001A1068();
    }
    return 0;
}

s32 func_001A0DC0(void) {
    s32 state = D_003BB2E4;
    if (state == 0) {
        return 0;
    }
    if ((*(u32 *)(state + 0x1F4) & 1) != 0) {
        func_001EFCE8();
        func_001EFF00();
        func_001F25C8();
        func_001DA780();
        func_0020FC48();
        func_001FB090();
        func_001BCDC8();
        clearDeferredBattleTasks();
    }
    return 0;
}

void func_001A0E38(void) {
}

extern u8 D_003BB2F0[];
extern char D_003A1598[]; /* "battle_draw" */
extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

void func_001A0E40(void) {
    s32 state = D_003BB2E4;
    u32 mainTask;
    u32 drawTask;
    mainTask = kwlnTaskCreate((u32)D_003BB2F0, 0x3F9, 0, 0, func_001A0D18, func_001A0E38, 0);
    *(u32 *)(state + 0x29C) = mainTask;
    drawTask = kwlnTaskCreate((u32)D_003A1598, 0x2B0E, 0, 0, func_001A0DC0, 0, 0);
    func_00101A80(mainTask, drawTask);
}
void func_001A0ED0(void) {
    s64 temp_v0;

    temp_v0 = func_00101938(0x3f9);
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(*(u32 *)(D_003BB2E4 + 0x29c), 1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A0F10);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1598);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1068);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A11F0);

void func_001A1410(void) {
    func_001F8280();
    func_001C44F8();
    func_001F2F00();
}

u8 func_001A1438(void) {
    return D_003BB2E4 != 0;
}

s32 func_001A1448(void) {
    if (func_001A1438() == 0) {
        return 0;
    }
    return (*(s32 *)(D_003BB2E4 + 0x1f4) & 0x6000000) == 0x6000000;
}

s32 func_001A1480(void) {
    s32 state;
    if (func_001A1438() == 0) {
        return 0;
    }
    state = D_003BB2E4;
    if (*(s32 *)(state + 0x224) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1C8) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x694) != 0;
}

void func_001A14C8(void) {
    s32 context = D_003BB2E4;
    s32 *entries = (s32 *)(D_003BAA00 + 0xBF8);
    u32 i;
    *(s32 *)(context + 0x2C0) = 0;
    *(s32 *)(context + 0x2C4) = 0;
    *(s32 *)(context + 0x2C8) = 0;
    *(s32 *)(context + 0x2CC) = 0;
    *(s32 *)(context + 0x2D0) = 0;
    for (i = 0; i < 5; i++) {
        *entries = 0;
        entries = (s32 *)((u8 *)entries + 0x1A4);
    }
    memset((void *)(D_003BB2E4 + 0x2B4), 0, 12);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1530);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1668);

s32 func_001A17F0(void) {
    return D_003BB2E4;
}

u16 func_001A17F8(s32 arg0) {
    return *(u16 *)(arg0 + 6);
}

u16 func_001A1800(s32 arg0) {
    return *(u16 *)(arg0 + 10);
}

void func_001A1808(void) {
    func_00118368();
}

void func_001A1820(void) {
    func_00118408();
}

u32 func_001A1838(s32 object) {
    return func_001190B0(object);
}

u32 func_001A1850(s32 object) {
    return func_001191B0(object);
}

void func_001A1868(u8 *object, s32 value) {
    func_001192B0(object, value);
}

void func_001A1880(u8 *object, s32 value) {
    func_001192D8(object, value);
}

u16 func_001A1898(s32 object) {
    u16 maximum = func_001A17F8(object);
    u32 value = func_001A1838(object);
    *(u16 *)(object + 8) = value;
    if (value < maximum) {
        *(u16 *)(object + 6) = value;
    }
    return *(u16 *)(object + 6);
}

u16 func_001A18E8(s32 object) {
    u16 maximum = func_001A1800(object);
    u32 value = func_001A1850(object);
    *(u16 *)(object + 12) = value;
    if (value < maximum) {
        *(u16 *)(object + 10) = value;
    }
    return *(u16 *)(object + 10);
}

u16 func_001A1938(s32 arg0) {
    return *(u16 *)(arg0 + 0xe) & 0x7fff;
}

void func_001A1948() {
    func_00119018();
}

void func_001A1960(void) {
    func_00119098();
}

void func_001A1978(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2f0) = arg1;
}

void func_001A1980(s32 arg0) {
    *(u32 *)(arg0 + 0x2f0) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1990);

s32 func_001A1B00(s32 arg0) {
    s32 temp_v0;

    if ((*(u32 *)(arg0 + 0x110) & 0x400) == 0) {
        temp_v0 = func_001A1B78(*(u8 *)(arg0 + 0x2c4));
        return temp_v0;
    }
    return arg0 + 0x120;
}

s32 func_001A1B38(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return D_003BAA00 + temp_v0 * 0x1a4 + 0xa60;
}

s32 func_001A1B78(s32 arg0) {
    return D_003BAA00 + arg0 * 0x1a4 + 0xa60;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1BA0);

s32 func_001A1CB8(s32 arg0) {
    return dds3FindEntryIndex(*(u16 *)(arg0 + 0x124));
}

void func_001A1CD0(void) {
}

s32 func_001A1CD8(s32 index) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if (index == *(u8 *)(node + 0x2C4)) {
                    return node;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A1D48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2608);

s32 getEntryFlagsUnlessDisabled(s32 entry) {
    if ((*(u16 *)entry & 4) != 0) {
        return 0;
    }
    return *(s32 *)(D_003BAA1C + *(u16 *)(entry + 4) * 76);
}

void func_001A29B8(void) {
    func_00119300();
}

void func_001A29D0(void) {
    func_00119368();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A29E8);

s8 func_001A2AE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BAA54 + arg0 * 0x10;
    return *(s8 *)(temp_v0 - 0x1aa4);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2B00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2CC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A2DE8);

s8 func_001A2F00(s32 object, s32 index) {
    if (index == 0 && (*(u32 *)(object + 0x110) & 0x400) != 0) {
        return *(s8 *)(D_003BAA1C + *(u16 *)(object + 0x124) * 76 + 0x46);
    }
    return *(s8 *)(D_003BAA4C + index * 2);
}

void func_001A2F50(s32 arg0) {
    func_001A2F68(arg0 + 0x120);
}

s32 func_001A2F68(s32 object, s32 value) {
    s32 (*handler)(s32, s32) = *(s32 (**)(s32, s32))(func_001A17F0() + 0x670);
    if (handler != 0) {
        s32 result = handler(object, value);
        if (result != -1) {
            return result;
        }
    }
    return func_00119520(object, value);
}

s32 func_001A2FD8(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA10 + arg1 * 0x270;
    }
    return D_003BAA20 + arg1 * 0x270;
}

s32 func_001A3020(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003583A0;
    }
    return D_003BAA30 + arg1 * 24;
}

s32 func_001A3050(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_003BAA14 + arg1 * 0x74;
    }
    return D_003BAA28 + arg1 * 0x74;
}

extern char D_003A1788[];

s32 func_001A3098(s32 index) {
    u16 item = *(u16 *)(D_003BAA68 + index * 8 + 2);
    func_001FB0A8(D_003A1788, index, item);
    return item;
}

s32 func_001A30E0(s32 arg0) {
    return *(u16 *)(arg0 * 8 + D_003BAA68 + 2);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1788);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A30F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3360);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3500);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3638);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3740);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3CE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A3F38);

void func_001A4060(void) {
    func_00119750();
}

s32 battleCheckSpecialAbility(s32 object, s32 flag) {
    if (func_001193A0(object, flag) == 0) {
        return 0;
    }
    switch (flag) {
    case 0x207:
        return func_002286C0() == 8;
    case 0x208:
        return func_002286C0() == 0;
    default:
        return 1;
    }
}

extern s8 D_00324550[];

s32 func_001A40E8(s32 object) {
    s32 result = func_001A4130(object, 1);
    if (result == 0) {
        result = (effMiscRand(D_00324550) & 1) != 0 ? 2 : 7;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4240);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4328);

s32 battleAllActiveUnitsReady(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 0xC0) != 0) {
                    return 0;
                }
                if ((flags & 0x20) != 0) {
                    if ((*(u32 *)(node + 0x114) & 1) == 0) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

extern s32 D_003BAA3C;
extern s32 D_003BAA6C;

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4598);

u32 func_001A4630(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(temp_v0 + 0x254);
}

s32 battleChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    s32 node = *(s32 *)(func_001A17F0() + 0x224);
    for (; node != 0; node = *(s32 *)(node + 0x16C)) {
        if ((*(u32 *)(node + 8) & 8) != 0) {
            s32 actor = *(s32 *)(node + 0x18);
            u32 flags = *(u32 *)(actor + 0x110);
            if ((flags & 1) != 0) {
                if ((flags & 0x200) != 0) {
                    if ((flags & 2) != 0) {
                        if ((flags & 0xE0) == 0) {
                            candidates[count++] = node;
                        }
                    }
                }
            }
        }
    }
    if (count != 0) {
        return candidates[effMiscRandMod(0, count)];
    }
    return 0;
}

s32 battleCountAvailableUnits(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 battleCountAvailableParticipants(void) {
    s32 count = 0;
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                if ((flags & 0xE0) == 0) {
                    count++;
                }
            }
        }
    }
    {
        u8 *entry = (u8 *)(D_003BAA00 + 0xA60);
        s32 i;
        for (i = 4; i >= 0; i--, entry += 0x1A4) {
            u16 flags = *(u16 *)entry;
            if ((flags & 1) != 0) {
                if ((flags & 2) == 0) {
                    if ((*(u16 *)(entry + 0xE) & 0x4000) == 0) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A47F0);

void func_001A4860(u32 arg0) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        func_001A49E8(arg0, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A48B0);

extern s16 D_003583D0[];

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A48E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4948);

void func_001A49D0(s32 arg0, s32 arg1, u16 arg2) {
    *(u16 *)(arg1 * 6 + arg0 + 0x2c6) = arg2;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A49E8);

s16 func_001A4A18(s32 arg0, s32 arg1) {
    return *(s16 *)(arg1 * 6 + arg0 + 0x2c6);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4A30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4C68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A4F48);

void func_001A4FE0(u8 *scene) {
    u32 index;
    s16 *timer = (s16 *)(scene + 0x2CA);
    for (index = 0; index < 7; index++, timer += 3) {
        if (*timer >= 0) {
            if (*timer == 0) {
                *timer = -1;
            }
            --*timer;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A51C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A53D8);

u32 func_001A5578(s32 arg0, s64 arg1) {
    if (arg1 == 0 && battleCheckSpecialAbility(arg0 + 0x120, 0x254) != 0) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A55A8);

s32 getSoundResourceForIndex(s32 index) {
    s8 resource = *(s8 *)(D_003BAA4C + index * 2);
    if (resource < 0) {
        return 0;
    }
    return (s32)D_00358408[resource];
}

void *func_001A5670(s32 arg0) {
    return D_00358450[*(u16 *)(arg0 + 0x124)];
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A18F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1908);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1918);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5690);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A57A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5958);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A5C40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6570);

s32 func_001A66B8(s32 object) {
    s32 (*predicate)(s32) = *(s32 (**)(s32))(func_001A17F0() + 0x650);
    if (predicate != 0 && predicate(object) != 0) {
        return 1;
    }
    return (*(u16 *)(object + 0x12E) & 0x806) != 0;
}

s32 func_001A6708(s32 object) {
    s32 result = 0;
    u16 category = *(u16 *)(object + 0x12E) & 0x7FFF;
    switch (category) {
    case 0x80:
    case 0x400:
    case 0x2000:
        result = -*(u16 *)(object + 0x128) / 5;
        break;
    }
    return result;
}


s32 func_001A6778(s32 unused, u8 *actor, u32 flags, u32 options) {
    s32 ratio;
    if ((*(u32 *)(func_001A17F0() + 0x1FC) & 0x80) != 0) return 0;
    if ((*(u32 *)(actor + 0x114) & 8) != 0) return 0;
    if ((flags & 1) == 0) return 0;
    if ((*(u16 *)(actor + 0x12E) & 1) != 0) return 0;
    ratio = 0;
    if (options & 2) {
        ratio = 30;
    } else if (options & 4) {
        ratio = 40;
    }
    func_001FB0A8("btl:fear ratio[%d]\n", ratio);
    return func_001FFCD8() < ratio;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6828);

f32 func_001A6958(void) {
    return 1.5f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6968);

u8 func_001A6A50(s32 object, s32 index) {
    if (index == 0) {
        if ((*(u32 *)(object + 0x110) & 0x400) != 0) {
            return *(u8 *)(D_003BAA1C + *(u16 *)(object + 0x124) * 76 + 0x48);
        }
        return 12;
    }
    return 12;
}

extern u8 D_00358490[];
extern char D_003A1A58[]; /* "btl:delay=%d\n" */

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6AA0);
s32 battleResolveSkillCategory(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x17C:
    case 0x17F:
        return 0x37;
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    default:
        return *(s8 *)(D_003BAA4C + id * 2 + 1) == 2 ? 0x2D : 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6BE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6EC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A6F98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7180);

s32 func_001A73A0(s32 first, s32 second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = func_00118DD8(other, first + 0x120, second + 0x120);
    if ((*(u16 *)(second + 0x12E) & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7548);

void func_001A7AD0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7AD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7C20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7D38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A7DF0);

u32 func_001A7ED8(void) {
    return 0;
}

extern char D_003A1C10[]; /* "btl:hunt mp rec[%d]\n" */
extern s32 D_003BAA5C;

s32 func_001A7EE0(u8 *actor) {
    s32 recovery = 0;
    if (battleCheckSpecialAbility((s32)(actor + 0x120), 0x24E)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * *(f32 *)(D_003BAA5C + 0x270));
    } else if (battleCheckSpecialAbility((s32)(actor + 0x120), 0x229)) {
        recovery = (s32)(*(u16 *)(actor + 0x12C) * *(f32 *)(D_003BAA5C + 0x148));
    }
    func_001FB0A8(D_003A1C10, recovery);
    return recovery;
}
s32 func_001A7F88(u8 *actor, s32 delta) {
    if (getEntryFlagsUnlessDisabled((s32)(actor + 0x120)) & 4) return 0;
    if (func_001A9EF8((s32)actor)) return 0;
    if ((*(u16 *)(actor + 0x12E) & 0x7FFF) == 0x4000) return 1;
    if ((*(u32 *)(func_001A17F0() + 0x1F4) & 0x80) == 0) return 0;
    return *(u16 *)(actor + 0x126) + delta < 1;
}

s32 func_001A8018(UiObject *object) {
    return object->currentValue * 100 / object->maximumValue < 25;
}

s32 func_001A8050(UiObject *object, s32 delta) {
    s32 value = object->currentValue + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->maximumValue < 25;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8098);

s32 func_001A8148(UiObject *object) {
    if ((object->flags & 0x400) != 0) {
        if (object->index >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 func_001A8178(UiObject *object) {
    if ((object->statusFlags & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1C10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8188);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8330);

s32 func_001A8410(s32 arg0) {
    u16 temp_v0;

    temp_v0 = *(u16 *)(D_003BAA50 + arg0 * 56 + 0x2e);
    return D_00358510[temp_v0 * 3];
}

extern s32 D_00358518[];

s32 func_001A8448(s32 object, s32 mask) {
    s32 index = *(s32 *)(object + 0x2F0);
    u16 item;
    if (index == -1) {
        return 0;
    }
    item = *(u16 *)(D_003BAA50 + index * 56 + 0x2E);
    return (D_00358518[item * 3] & func_001A3F38(mask)) != 0;
}

s32 func_001A84B8(s32 object) {
    s32 item = *(s32 *)(object + 0x2F0);
    if (item == -1) {
        return 0;
    }
    return func_001A8410(item);
}

extern s32 D_00358514[];

s32 func_001A84F0(s32 object) {
    s32 index = *(s32 *)(object + 0x2F0);
    if (index == -1) {
        return 0;
    }
    return D_00358514[*(u16 *)(D_003BAA50 + index * 56 + 0x2E) * 3];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8538);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8640);

extern u8 *D_003BAA34;
extern u8 *D_003BAA24;

s32 func_001A87A0(s32 object) {
    s32 context = func_001A17F0();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(s8 *)(D_003BAA34 + index * 40) != 0) {
        return 0;
    }
    if ((*(u16 *)(object + 0x12E) & 0x2A0F) != 0) {
        return 0;
    }
    return D_003BAA24[*(u16 *)(object + 0x124) * 0x15C] == 0;
}

s32 func_001A8818(s32 arg0) {
    if ((*(u16 *)(arg0 + 0x12e) & 0x40) != 0) {
        return 0;
    }
    return (getEntryFlagsUnlessDisabled(arg0 + 0x120) & 0x40) < 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8A30);

s32 func_001A8C68(s32 object) {
    u8 *status = (u8 *)(D_003BAA1C + *(u16 *)(object + 0x124) * 76 + 0x3E);
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

u32 func_001A8CC8(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) >> 0x1a) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8CE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A8DD8);

u8 func_001A8EA8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001A8DD8(arg0, 0);
    return temp_v0 != 0;
}

s32 func_001A8EC8(s32 object) {
    s32 choices[24];
    s32 count = func_001A8DD8(object, choices);
    if (count != 0) {
        return choices[effMiscRandMod(0, count)];
    }
    return -1;
}

s32 battleChooseEligibleSkill(s32 object) {
    s32 choices[24];
    s32 count = 0;
    u32 i;
    u16 *ids = (u16 *)(object + 0x142);
    for (i = 0; i < 24; i++) {
        u32 id = *ids++;
        if (id != 0) {
            if (id < 0x260) {
                s32 category = *(s8 *)(D_003BAA4C + id * 2 + 1);
                if (category != 2) {
                    if (category != 4) {
                        if ((*(u8 *)(D_003BAA50 + id * 56 + 1) & 2) != 0) {
                            if (id < 0xAB || (id >= 0xAD && id != 0xBF)) {
                                choices[count++] = id;
                            }
                        }
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    return choices[effMiscRandMod(0, count)];
}

extern s32 D_003BAA5C;

f32 func_001A8FF0(s32 object, s32 unused, s32 index) {
    s32 category = *(u16 *)(D_003BAA50 + index * 56 + 0x16);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    if (index == 0 && battleCheckSpecialAbility(object + 0x120, 0x21D) != 0) {
        return *(f32 *)(D_003BAA5C + 0xE8);
    }
    return 0.0f;
}

f32 func_001A9068(s32 unused0, s32 unused1, s32 index) {
    s32 category = *(u16 *)(D_003BAA50 + index * 56 + 0x1A);
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 func_001A90B0(s32 unused0, s32 unused1, s32 index) {
    return (f32)*(u16 *)(D_003BAA50 + index * 56 + 0x22) / 100.0f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A90F0);

s32 func_001A91A8(u32 flags, u32 secondary) {
    if (flags & 0x20000) return 1;
    if (flags & 0x40000) return 1;
    if (flags & 0x10000) return 1;
    if (flags & 2) return 1;
    if (secondary & 4) return 3;
    return secondary & 2 ? 2 : 1;
}

s32 func_001A9200(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x128);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void func_001A92B8(u32 arg0) {
    func_001A9200(arg0, 1);
}

void func_001A92D0(u32 arg0) {
    func_001A9200(arg0, 0);
}

s32 func_001A92E8(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x126);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void func_001A93A0(u32 arg0) {
    func_001A92E8(arg0, 1);
}

void func_001A93B8(u32 arg0) {
    func_001A92E8(arg0, 0);
}

s32 func_001A93D0(u32 mask, s8 allowDisabled) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    s32 sum = 0;
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    count++;
                    sum += *(u16 *)(node + 0x134);
                }
            }
        }
    }
    if (count > 0) return sum / count;
    return 1;
}

void func_001A9488(u32 arg0) {
    func_001A93D0(arg0, 1);
}

void func_001A94A0(u32 arg0) {
    func_001A93D0(arg0, 0);
}

s32 func_001A94B8(u32 mask, s32 attribute, s8 allowDisabled) {
    s32 sum = 0;
    s32 count = 0;
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if (allowDisabled == 0 || (flags & 0x20) == 0) {
                if ((*(u16 *)(node + 0x120) & mask) != 0) {
                    s32 value = func_00119368(node + 0x120, attribute);
                    count++;
                    sum += value;
                }
            }
        }
    }
    if (count >= 2) {
        sum /= count;
    }
    return sum;
}

void func_001A9598(u32 arg0, u32 arg1) {
    func_001A94B8(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    func_001A94B8(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A95C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9780);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1D28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A99B0);

void battleClearUnitStatusMask(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        u32 flags = *(u32 *)(node + 0x110);
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                *(u32 *)(node + 0x110) = flags & ~0x1000;
                *(u16 *)(node + 0x120) &= ~0x1000;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9D38);

extern char D_003A1DA0[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

s32 func_001A9E50(u8 *actor) {
    u8 *battle = (u8 *)func_001A17F0();
    s32 (*predicate)(u8 *) = *(s32 (**)(u8 *))(battle + 0x65C);
    if (predicate != 0 && predicate(actor) == 0) return 0;
    if (*(u32 *)(actor + 0x114) & 0x2000) return 0;
    if ((*(u16 *)(actor + 0x12E) & 0x7FFF) == 0x4000) return 0;
    if (battleCheckSpecialAbility((s32)(actor + 0x120), 0x231)) return 1;
    func_001FB0A8(D_003A1DA0, 5, 1.0);
    return func_001FFCD8() < 5;
}
s32 func_001A9EF8(s32 object) {
    if ((*(u32 *)(object + 0x110) & 0x400) == 0) {
        return 0;
    }
    return (*(s32 *)(D_003BAA1C + *(u16 *)(object + 0x124) * 76) & 0x100) > 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1DA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001A9F40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA130);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA548);

u32 func_001AA848(void) {
    func_001A17F0();
    return 0xffffffff;
}

void func_001AA868(s32 object, s32 status) {
    *(u16 *)(status + 0x26) &= ~3;
    *(u32 *)(object + 0x110) &= ~0x20;
    *(u16 *)(object + 0x12E) &= ~0x4080;
    if (*(u16 *)(object + 0x126) == 0) {
        *(u16 *)(object + 0x126) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AA8B0);

s32 func_001AAA18(u8 *actor, u8 *target, s32 recordIndex, s32 speciesIndex) {
    s32 (*callback)(u8 *, u8 *, s32) =
        *(s32 (**)(u8 *, u8 *, s32))(func_001A17F0() + 0x680);
    u8 *resource;
    if (callback != 0 && callback(actor, target, speciesIndex) == 0) {
        return 0;
    }
    resource = (u8 *)func_001A2FD8(*(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    if (*(s16 *)(resource + recordIndex * 20 + 0x2C) != 2) {
        return 0;
    }
    if (speciesIndex != 0 &&
        *(u8 *)(D_003BAA50 + speciesIndex * 56 + 8) != 0) {
        return 0;
    }
    return 1;
}

s32 func_001AAAD8(u8 *actor) {
    if (*(u32 *)(actor + 0xDC) != 1) {
        return 1;
    }
    switch (*(u32 *)(actor + 0xE0)) {
    case 0x26:
    case 0x54:
    case 0x153:
        return 0;
    default:
        return 1;
    }
}

s32 func_001AAB30(void) {
    u8 *resource;
    s8 index;
    s32 count;
    if (*(s8 *)D_003BB3E0 == 2) {
        resource = (u8 *)D_003BD830;
        index = *(s8 *)resource;
        count = *(s16 *)(resource + index * 2);
        if (count >= 0x80) {
            if (*(s32 *)(resource + index * 8 + 4) >= 11) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_001AAB88(void) {
    u8 *actor = *(u8 **)(func_001A17F0() + 0x228);
    s32 count = 0;
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x321) == 0x301 &&
            (*(u16 *)(actor + 0x12E) & 0x800) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AABE8);

typedef struct ActorClassIds {
    s16 values[8];
} ActorClassIds;
extern const ActorClassIds D_003A1FD8;

s16 func_001AAD08(s8 classId) {
    ActorClassIds table = D_003A1FD8;
    return table.values[classId];
}

typedef struct ActorClassPairTable {
    u32 values[14];
} ActorClassPairTable;
extern const ActorClassPairTable D_003A1FE8;

void func_001AAD50(s8 classId, u32 *first, u32 *second) {
    ActorClassPairTable pairs = D_003A1FE8;
    *first = pairs.values[classId * 2];
    *second = pairs.values[classId * 2 + 1];
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AADF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB150);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB270);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB558);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AB810);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ABDF8);

void func_001AC058(void) {
    func_002CFF98(D_003BB3E0);
    D_003BB3E0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC080);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC398);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC4A8);

void func_001AC5F8(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5) {
    *(u8 *)(arg0 + 0) = 1;
    *(u8 *)(arg0 + 0x28) = arg4 * 8 + 0x18;
    *(u16 *)(arg0 + 2) = arg1;
    *(f32 *)(arg0 + 4) = arg5;
    *(u32 *)(arg0 + 0x18) = arg2;
    *(u32 *)(arg0 + 0x1c) = arg3;
    *(u32 *)(arg0 + 0x24) = 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2050);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2070);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2090);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A20A0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A20D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2100);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2128);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2158);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2168);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A21A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A21B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC628);

extern u8 D_003BB3E5;
extern u32 *D_003BB3E8;

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC6D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC760);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC7D8);

void func_001AC920(void) {
    u8 *context = (u8 *)func_001A17F0();
    u8 *node = *(u8 **)(context + 0x224);
    while (node != 0) {
        s32 i;
        u8 *actor = *(u8 **)(node + 0x18);
        if (actor != 0) {
            if (*(u16 *)(context + 0x24C) == 1) {
                if (*(u32 *)(actor + 0x110) & 0x200) {
                    u32 *entries;
                    i = 0;
                    entries = (u32 *)(node + 0x148);
                    for (; i < 8; i++) {
                        *entries++ = 0;
                    }
                }
            } else if (*(u32 *)(actor + 0x110) & 0x400) {
                u32 *entries;
                i = 7;
                entries = (u32 *)(node + 0x164);
                for (; i >= 0; i--) {
                    *entries-- = 0;
                }
            }
        }
        node = *(u8 **)(node + 0x16C);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AC9E8);

void func_001ACAE0(void) {
    s32 temp_v0;
    s32 buf[4];

    temp_v0 = func_001A17F0();
    func_001C4198(temp_v0, buf);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACB08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACC20);

void func_001ACCF0(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 2;
    puVar1 = (u32 *)(D_003BAA00 + 0x2e9dc);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACD30);

void func_001ACDF0(void) {
    s32 *temp_v0;
    s32 temp_v1;

    temp_v0 = D_00359A78;
    temp_v0 += 2;
    temp_v1 = 2;
    do {
        temp_v1 = temp_v1 - 1;
        *temp_v0 = 0;
        temp_v0 = temp_v0 - 1;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACE28);

extern u32 D_003BB3A0;

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACED0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACF10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ACFE0);

u32 func_001AD1B0(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(10);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001AD1F8(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C8);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD230);

u32 func_001AD3A8(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(0xb);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3CC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_001AD3F0(void) {
    if (func_001AD3A8() == 0) {
        return 0x80;
    }
    return *(s8 *)func_00101A70(kwlnTaskGetTaskByName(D_003BB3CC));
}

u32 func_001AD428(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3CC);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD468);

void func_001AD5A0(void) {
    u8 *puVar1;
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)func_00101A70(temp_v0);
        *puVar1 = 2;
    }
}

u32 func_001AD5D8(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(9);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD620);

void func_001AD668(s32 mode) {
    s32 task = func_001ADCB8(7);
    if (task != 0) {
        s32 *data = (s32 *)func_00101A70(task);
        data[2] = mode;
        if (mode == 0) {
            data[1] = 1;
            data[5] = 0x80;
        } else {
            data[5] = 0xFF;
            data[1] = 4;
            data[6] = 0xFF;
        }
    }
}

void func_001AD6D8(s32 unused) {
    func_001A17F0();
    func_00101A70(func_001ADCB8(7));
    *(u8 *)(D_003BB3D8 + 0x48) = 0;
}

u32 func_001AD710(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(6);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3BC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD758);

u32 func_001AD928(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(1);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3AC);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AD970);

u32 func_001ADB30(void) {
    s64 temp_v0;

    temp_v0 = func_001ADCB8(0);
    if (temp_v0 != 0) {
        temp_v0 = func_001019C8(temp_v0);
        if (temp_v0 == 0) {
            return 0;
        }
        temp_v0 = kwlnTaskGetTaskByName(D_003BB3A8);
        if (temp_v0 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADB78);

s32 func_001ADCB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BB3D8 + arg0 * 4;
    return *(s32 *)temp_v0;
}

void func_001ADCD0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = D_003BB3D8 + arg0 * 4;
    *(s32 *)temp_v0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADCE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ADE68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE1B8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2228);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2258);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2270);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2298);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A22C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE250);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AE540);

void func_001AEDF8(s32 handle) {
    u8 *context = (u8 *)func_001A17F0();
    u8 *data = (u8 *)func_00101A70(handle);
    func_002CFF98(data);
    func_001ADCD0(11, 0);
    if (*(u16 *)(*(u8 **)(*(u8 **)(context + 0x164) + 0x18) + 0x12E) & 0x80) {
        func_001EF390(context + 0x70, context + 0x70);
    }
    *(u32 *)(context + 0x1F4) |= 0x100000;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AEE78);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2460);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AF058);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AF5D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2490);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A24A0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A24D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2500);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2530);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001AFF78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0460);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25C0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A25F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2608);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A27C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B05D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0938);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B09A8);

void func_001B0C70(s64 task) {
    s32 *entry = (s32 *)func_00101A70(task);
    itfMesCleanupWindow(entry[9], 0);
    func_002CFF98(entry);
    func_001ADCD0(9, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0CB8);

u8 func_001B0D58(s32 arg0) {
    return (~*(u64 *)(arg0 + 0x110) & 0x201) == 0;
}

void func_001B0D70(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x2c) + 0x18);
    *(s16 *)(temp_v0 + 0x2ac) = arg1;
    *(s16 *)(temp_v0 + 0x2ae) = arg2;
    *(s16 *)(temp_v0 + 0x2b0) = arg3;
}

s32 func_001B0D88(s32 context) {
    s32 node = *(s32 *)(context + 0x228);
    s32 count = 0;
    for (; node != 0; node = *(s32 *)(node + 0x344)) {
        if ((*(u64 *)(node + 0x110) & 0x201) == 0x201) {
            if ((*(u16 *)(node + 0x120) & 2) != 0) {
                count++;
            }
        }
    }
    return count;
}

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);
extern void func_00101A80(u32, u32);

extern u32 D_003BB3C0;
extern s32 func_001B0E68(s64);
extern void func_001B14E8(s64);

void func_001B0DD0(void) {
    s32 context = func_001A17F0();
    u32 data = (u32)func_002CFF68(0x20);
    u32 task = kwlnTaskCreate(D_003BB3C0, 0x2B0E, 1, 1, func_001B0E68, func_001B14E8, data);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    func_001ADCD0(7, task);
}

void func_001B0E48(s32 arg0) {
    *(u32 *)(arg0 + 4) = 1;
    *(u32 *)(arg0 + 12) = 0x80;
    *(u32 *)(arg0 + 16) = 0;
    *(u32 *)(arg0 + 0) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B0E68);

void func_001B14E8(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(7, 0);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A28C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B1518);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B19F8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2938);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B1C88);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2968);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2390);

s32 func_001B29F8(s64 task) {
    s32 *state = (s32 *)func_00101A70(task);
    s32 phase = func_001BCB18();
    if ((u8)(phase - 1) < 2) {
        return 0;
    }
    func_001B1518(state);
    if (*(s8 *)(D_003BB3D8 + 0x54) == 0) {
        func_001B19F8(state);
    }
    func_001B1C88(state);
    if (*(s8 *)(D_003BB3D8 + 0x54) == 0) {
        func_001B2390(state);
    }
    ++state[0];
    return state[0] < 50 ? 0 : -1;
}

void func_001B2A98(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2998);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2AC8);

void func_001B2D58(void) {
    func_002CFF98(D_003BD834);
    func_002CFF98(D_003BD838);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A29F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B2D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3300);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3DC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B3FB0);

void func_001B42D8(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    func_001ADCD0(5, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4308);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B47A8);

void func_001B49E0(s64 task) {
    func_001A17F0();
    func_002CFF98(func_00101A70(task));
    func_001ADCD0(1, 0);
}

void func_001B4A20(void) {
    if (D_00324530[13] < 0) {
        if (mdlFlagTest(0xC0E)) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4A70);

void func_001B4C98(s64 task) {
    s32 *entry;
    func_001A17F0();
    entry = (s32 *)func_00101A70(task);
    itfMesCleanupWindow(entry[1], 0);
    func_002CFF98(entry);
    func_001ADCD0(0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4CE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4D58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2AF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B4F10);

void func_001B5370(void) {
    func_001A17F0();
    func_002CFF98(D_003BB3DC);
    D_003BB3DC = 0;
    func_001ADCD0(4, 0);
}

s32 areSoundSlotsEmpty(void) {
    s32 *slot = (s32 *)(D_003BAA00 + 0x2E9DC);
    s32 i;
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            return 0;
        }
    }
    return 1;
}

extern u8 D_00359160[];

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B53E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5480);
extern u8 *func_001BD708(u8 *, s16 *);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B54C0);

extern u32 D_003BB3D0;

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5550);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B55A8);

void func_001B5910(s64 task) {
    s32 context = func_001A17F0();
    func_002CFF98(func_00101A70(task));
    func_001ADCD0(12, 0);
    *(u32 *)(context + 0x1F4) |= 0x100000;
    func_0024DBB0();
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5970);


INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5B68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5BC0);

extern u8 D_003A2B80[];

void func_001B5C28(void) {
    u8 initial[0x20];
    u8 *allocated;
    u32 *source;
    u32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, D_003A2B80, sizeof(initial));
    allocated = func_002CFF68(0x30);
    D_003BD830 = (u32)allocated;
    state = (s16 *)(allocated + 2);
    destination = (u32 *)(allocated + 0x10);
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    func_001ADCD0(2, 1);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2B80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B5CD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B60E8);

void func_001B62D0(void) {
    if (func_001ADCB8(2) != 0) {
        func_002CFF98(D_003BD830);
    }
    func_001ADCD0(2, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6308);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6498);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6850);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B6CF8);

void func_001B7178(s64 arg0) {
    u32 temp_v0;

    temp_v0 = func_00101A70(arg0);
    func_002D0918(*(s32 *)temp_v0);
    func_001ADCD0(3, 0);
}

s32 func_001B71A8(void) {
    s32 base;
    s32 i;
    if (func_001ADCB8(8) == 0) {
        if (func_001ADCB8(2) != 0) {
            base = D_003BD830;
            for (i = 0; i < 4; i++) {
                if (*(s16 *)(base + 2 + i * 2) < 0x80) {
                    return 0;
                }
                if (*(s32 *)(base + 0xC + i * 8) < 11) {
                    return 0;
                }
            }
            if (*(s32 *)(base + 0x18) == *(s32 *)(base + 0x10) + 23) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7238);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B74C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7838);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7880);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7C90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B7F50);

void func_001B83A0(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3B0);
    temp_v1 = func_00101A70(temp_v0);
    func_002D0918(*(s32 *)(temp_v1 + 0x1200));
    func_001ADCD0(8, 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B83D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8550);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8650);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8778);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8838);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B89B0);

void func_001B8B20(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8B70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8BB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B8CB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B91F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2C90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2CE8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9318);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B96F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001B9E98);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA198);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA408);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BA660);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BAB08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BAE08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB440);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2D68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB6C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BB990);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BBE18);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BC540);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BC978);

s32 func_001BCB18(void) {
    s64 first;
    s64 second;
    if (D_003BB3D8 != 0) {
        first = kwlnTaskGetTaskByName(D_003BB3A4);
        second = kwlnTaskGetTaskByName(D_003BB3B0);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (*(s8 *)(D_003BB3D8 + 0x3D) == 0) {
            return *(s8 *)(D_003BB3D8 + 0x3C);
        }
        return 0;
    }
    return -128;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCB88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCCE0);

void func_001BCDC8(void) {
    s32 context = func_001A17F0();
    func_001FEA78(7);
    if ((*(u32 *)(context + 0x1F4) & 0x400) != 0) {
        if (*(u16 *)(context + 0x24C) == 1) {
            func_001AD6D8(0);
            btlClearNodeFlags();
        } else {
            func_001AD6D8(1);
            btlClearNodeFlags();
        }
    }
    func_001B4A20();
    func_001BCCE0();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BCE48);

void func_001BCE90(void) {
}

void func_001BCE98(void) {
}

void func_001BCEA0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    func_00197220(0x13);
    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_001958A0(temp_v0, 1, 0x53);
    func_00194920(temp_v0);
    func_00197220(0xffffffffffffffff);
}

extern u32 D_003BAA8C;

void func_001BCF28(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_00197220(0x13);
    handle = func_00197760(x << 4, y << 3, z, w, D_003BAA8C + index * 17, 0);
    func_00195880(handle, 1);
    func_00194920(handle);
    func_00197220(-1);
}

extern u32 D_003BAA84;

void func_001BCFC8(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_00197220(0x13);
    handle = func_00197760(x << 4, y << 3, z, w, D_003BAA84 + index * 25, 0);
    func_00195880(handle, 1);
    func_00194920(handle);
    func_00197220(-1);
}

extern u8 *D_003BAA34;

s32 func_001BD070(s32 unused, u32 limit) {
    s32 context = func_001A17F0();
    s32 index = *(s32 *)(context + 0x27C);
    if (*(u16 *)(D_003BAA34 + index * 40 + 0x20) & 0x800) {
        return 1;
    }
    if (limit < (u32)func_001AAB88()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD0D0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2DD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD190);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD2C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD4F0);

extern u8 D_00358FE0[];

void func_001BD658(s32 unused, s16 *count) {
    s32 context = func_001A17F0();
    s32 index = *(s32 *)(context + 0x27C);
    s32 total = 0;
    if ((*(u16 *)(D_003BAA34 + index * 40 + 0x20) & 0x800) == 0) {
        u8 *selected = (u8 *)(D_003BAA00 + 0x12A0);
        u8 *flags = (u8 *)D_003BAA68;
        u8 *out = D_00358FE0;
        s32 i;
        for (i = 0; i < 0xC0; i++, flags += 8, selected++) {
            if (*selected != 0 && (*flags & 2)) {
                out[0] = i;
                out[1] = *selected;
                out += 2;
                total++;
            }
        }
    }
    *count = total;
}

extern u8 D_00359160[];

u8 *func_001BD708(u8 *object, s16 *value) {
    s32 result = func_001A3740(*(s32 *)(*(u8 **)(object + 0x2C) + 0x18), D_00359160);
    *value = result;
    return D_00359160;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BD750);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BDE98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BDF60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BE590);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BE8A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BEA80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BEB58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF040);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2EA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF0F8);

void func_001BF3C8(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_00101A70(arg0);
    func_002CFF98(temp_v0);
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 0x2a8) = 0;
    func_001B2D58();
}


INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF3F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF448);

s64 func_001BF488(void) {
    s64 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3A4);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)func_001BF448();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF4C0);

void func_001BF790(void) {
    s32 *task = (s32 *)func_001BF448();
    if (task != 0) {
        u8 *state = (u8 *)D_003BB3E0;
        *task = 5;
        *state = 3;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF7C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BF8B0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2EE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BFAD0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2FB8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A2FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001BFDE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C0650);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3020);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3030);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C08B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C0DF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1188);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3078);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1290);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C13E8);

void func_001C1620(u32 arg0) {
    func_001DAE08(*(u32 *)((s32)arg0 + 0x10));
    func_001DAE08(*(u32 *)((s32)arg0 + 0xc));
    func_002CFF98(arg0);
}

void func_001C1658(s64 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_00101A70(arg0);
    func_001C1620(temp_v0);
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1688);

u32 func_001C16C8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_001C1688();
    return *puVar1;
}

u32 func_001C16E8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)(func_001C1688() + 0x10);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1708);

void func_001C1820(void) {
    u32 *temp_v0;

    temp_v0 = (u32 *)func_001C1688();
    if (temp_v0 != 0) {
        *temp_v0 = 6;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1850);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A30D8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C1988);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A31A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2158);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C27B0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3220);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2938);

void func_001C2E60(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2E90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C2FC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3040);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C32B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3B28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C3F78);

s32 findBattleSceneSlotById(u8 *entry) {
    s32 context = func_001A17F0();
    u8 *slot = (u8 *)(context + 0x2D4);
    u32 i;
    for (i = 0; i < 8; i++, slot += 3) {
        if (slot[2] == entry[2]) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C40A8);

s32 fadeStaleBattleSceneSlots(void) {
    s32 context = func_001A17F0();
    u8 *scene = (u8 *)(context + 0x44C);
    u8 *slot = (u8 *)(context + 0x2D4);
    u32 i;
    s32 changed = 0;
    for (i = 0; i < 8; i++, slot += 3, scene += 8) {
        if (scene[2] != slot[2] && func_001C40A8(scene) == 0) {
            if (scene[3] != 0) {
                scene[3] -= 8;
                changed = 1;
            }
        }
    }
    return changed;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4198);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4278);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4418);

void func_001C4470(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x2b0) = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4490);

void func_001C44D0(void) {
    func_001A17F0();
    func_001BCB88(0, 8);
}

void func_001C44F8(void) {
    func_001AC628();
}

u32 func_001C4510(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

extern u32 D_003BB488;
extern s32 func_001C4418(s64);

void func_001C4548(void) {
    s64 oldTask = kwlnTaskGetTaskByName(D_003BB488);
    u8 *context;
    u32 task;
    if (oldTask == 0) {
        func_001A17F0();
    }
    context = (u8 *)func_001A17F0();
    task = kwlnTaskCreate(D_003BB488, 0x2B0E, 1, 1, func_001C4418, func_001C4470, 0);
    func_00101A80(*(u32 *)(context + 0x29C), task);
    *(u32 *)(context + 0x2B0) = task;
    func_001AC6D8();
    func_001B0DD0();
    func_001B6308();
    func_001B6498(1);
    func_001C4490();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C45F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4658);

void func_001C48A8(void) {
}

u32 func_001C48B0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C48B8);

s32 func_001C4968(void) {
    if (func_001F3390() != 0 &&
        func_002100A8() != 0 &&
        func_00213B60() != 0) {
        loadBattleSoundBank();
        func_002101C8();
        return 3;
    }
    return 0;
}

void func_001C49C0(u8 *context) {
    s32 *node = (s32 *)func_001F0178(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A));
    *(u64 *)((u8 *)node + 0x40) = 0x8000000000000001ULL;
    func_001D4860(node);
    func_001D4860(func_001F03A0(*(s16 *)(context + 0x288), *(s16 *)(context + 0x28A)));
}

extern s32 countBattleTasksForOwner(s64);
extern void func_001F33B8(void);
extern void func_001DC0E8(void);
extern void func_001EF420(void);
extern void func_001EFE08(s16, s16);

s32 func_001C4A18(u8 *object) {
    if (countBattleTasksForOwner(0x8000000000000001ULL) == 0) {
        func_001F33B8();
        func_001DC0E8();
        func_001EF420();
        func_001EFE08(*(s16 *)(object + 0x288), *(s16 *)(object + 0x28A));
        return 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4A80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C4F48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C50C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5740);

void func_001C5838(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5840);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C58A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5910);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5B90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5DB8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3248);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3258);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3268);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3278);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A32F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C5F80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6888);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6D48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C6E88);

void func_001C6FD0(s32 context) {
    s32 *entry;
    func_001F3408(context);
    entry = *(s32 **)(context + 0x224);
    while (entry != 0) {
        if (entry[0] != 30) {
            func_001D0728(entry, 30);
        }
        entry = *(s32 **)((u8 *)entry + 0x16C);
    }
    func_001D45F8();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7028);

void func_001C71C0(s32 arg0) {
    func_00215FE0(arg0);
    *(u32 *)(arg0 + 0x1fc) = *(u32 *)(arg0 + 0x1fc) | 4;
}

extern s32 func_00215FF8(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

u32 func_001C71F0(void) {
    if (func_00215FF8() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

extern SceneInitializer D_00359A88[];

void setBattleScene(s32 scene) {
    s32 context = func_001A17F0();
    *(s32 *)(context + 0x208) = scene;
    *(s32 *)(context + 0x210) = 0;
    *(s32 *)(context + 0x214) = 0;
    D_00359A88[scene].initialize(context);
}

void func_001C7280(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x20c) = arg0;
}

void updateBattleScene(void) {
    s32 context = func_001A17F0();
    s32 requested = *(s32 *)(context + 0x20C);
    s32 next;
    SceneInitializer *scene;

    if (requested != 0) {
        setBattleScene(requested);
        *(s32 *)(context + 0x20C) = 0;
    }
    scene = &D_00359A88[*(s32 *)(context + 0x208)];
    next = scene->update(context);
    if (next != 0) {
        func_001C7280(next);
    }
    ++*(s32 *)(context + 0x210);
}

void func_001C7328(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    setBattleScene(1);
    *(u32 *)(temp_v0 + 0x20c) = 0;
}

void func_001C7360(void) {
}

s32 func_001C7368(void) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(func_001A17F0() + 0x208);
    return D_00359A90[temp_v0 * 3];
}

s32 *func_001C73A0(s32 *object) {
    s32 context = func_001A17F0();
    s32 *result = (s32 *)(context + 0x33C);
    u32 flags;
    if (object[2] & 0x40) {
        return (s32 *)(context + 0x42C);
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200:
        result = (s32 *)(context + 0x2EC);
        break;
    case 0x400:
        break;
    case 0x800:
        result = (s32 *)(context + 0x3F0);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7418);

s32 func_001C7478(s32 *object) {
    u32 flags;
    func_001A17F0();
    if (object[2] & 0x40) {
        return 8;
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C74F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C75C8);

s32 func_001C7648(u8 *object) {
    u32 flags = *(u32 *)(*(u8 **)(object + 0x18) + 0x110) & 0xE00;
    s32 result;
    switch (flags) {
    case 0x200:
        result = 1;
        break;
    case 0x400:
        result = 2;
        break;
    case 0x800:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7690);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7980);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C79E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7B58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7CB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7D60);

s32 func_001C7DB8(void) {
    s32 context = func_001A17F0();
    u8 *slot;
    u32 i;
    if (*(u32 *)(context + 0x1F4) & 0x1000) {
        return 1;
    }
    slot = (u8 *)(context + 0x2D4);
    for (i = 0; i < 8; i++, slot += 3) {
        if (*slot != 0) {
            return 0;
        }
    }
    return 1;
}

void func_001C7E40(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_001C75C8(temp_v0 + 0x2ec, 0x14);
    func_001C74F0(temp_v0 + 0x33c, 0x2d);
    func_001C74F0(temp_v0 + 0x3f0, 0xf);
    func_001C7690();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7E90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C7F40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8000);

void func_001C8070(s32 value) {
    s32 *slot = (s32 *)(func_001A17F0() + 0x42C);
    while (*slot != 0) {
        slot++;
    }
    *slot = value;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C80C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8198);

void func_001C8258(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0xc;
}

void func_001C8280(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

s32 func_001C82B0(void) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v0 = func_001A17F0();
    temp_v1 = *(s32 *)(temp_v0 + 0x42c);
    if (temp_v1 != 0) {
        return temp_v1;
    }
    temp_v2 = func_001C7418(*(u8 *)(temp_v0 + 0x2d4));
    return *(s32 *)temp_v2;
}
s32 func_001C82E8(s32 index) {
    s32 context = func_001A17F0();

    if (*(u8 *)(context + 0x2D4) == 0) {
        return 0;
    }
    return *(s32 *)(func_001C7418(*(u8 *)(context + 0x2D4)) + index * 4);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8330);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8478);

u32 func_001C8578(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001C79E8(arg0[1], temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C85B8);

void func_001C8658(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8688);

extern u32 func_001C8688(s32 *);

u8 *func_001C86E8(u8 *owner, s32 parameter) {
    u8 *task = (u8 *)func_001D4748(8);
    u8 *data;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x5D;
    task[0x10] = 0;
    if (owner != 0) {
        *(u64 *)(task + 0x40) = *(u64 *)(*(u8 **)(owner + 0x18) + 0x108);
    }
    *(u32 *)(task + 0x48) = (u32)func_001C8658;
    *(u32 *)(task + 0x4C) = (u32)func_001C8688;
    data = (u8 *)func_001D47D8(task);
    *(u32 *)data = (u32)owner;
    *(s32 *)(data + 4) = parameter;
    return task;
}

u32 func_001C8780(u32 *arg0) {
    func_001C7CB0(*arg0);
    return 1;
}

s32 *func_001C87A0(s32 owner) {
    u8 *task = (u8 *)func_001D4748(4);
    *task = 1;
    *(u16 *)(task + 0x20) = 0x5E;
    *(u32 *)(task + 0x4C) = (u32)func_001C8780;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(s32 *)func_001D47D8(task) = owner;
    return (s32 *)task;
}

void func_001C8808(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void func_001C8818(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void func_001C8830(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((temp_v0 & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            temp_v0 = *(u32 *)(arg0 + 8);
            goto LAB_001c8880;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + D_003BAA1C + 0x15);
    }
    temp_v0 = *(u32 *)(arg0 + 8);
LAB_001c8880:
    *(u32 *)(arg0 + 8) = temp_v0 | 8;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A35A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A35B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8890);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C89E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8B00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8C78);

void func_001C8D38(void) {
}

void func_001C8D40(void) {
}

void func_001C8D48(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8D60);

void func_001C8E60(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8E78);

void func_001C8F88(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C8FA0);

void func_001C9090(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9098);

void func_001C93A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C93A8);

void func_001C9628(u32 arg0) {
    func_001A17F0();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001BF4C0(arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9660);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C97B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9960);

void func_001C9C20(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9C38);

void func_001C9E20(s32 arg0) {
    func_001B83D8(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9E58);

void func_001C9EC0(void) {
}

void func_001C9EC8(u8 *actor) {
    u8 *model;
    if (hasActiveActorSound() != 0) {
        return;
    }
    model = (u8 *)func_001DAE50(*(u32 *)(actor + 0x60), 0);
    if (*(u32 *)(actor + 0x20) == 1 &&
        (isActiveBattleActor((s32)model) == 0 ||
         (*(u64 *)(model + 0x110) & 0xE1) != 1)) {
        func_001D0728(actor, 26);
    } else {
        func_001D0728(actor, 12);
    }
}

void func_001C9F58(s32 arg0) {
    func_001ACC20();
    func_001D2B18(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x2f4) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001C9F90);

extern char D_003A3648[]; /* "btl:command=%d\n" */

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CA0C0);
void func_001CA158(u8 *command) {
    u8 *actor = *(u8 **)(command + 0x18);
    if (func_001C8B00(actor) == 0) {
        return;
    }
    if (*(u16 *)(command + 0x50) == 2) {
        func_001D4860(func_001FE468(actor, *(u32 *)(command + 0x54)));
    }
    func_001F0CA0(command, command + 0x20);
    func_001D0BA0(command, command + 0x20);
}

void func_001CA1F0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CA1F8);

void func_001CB408(void) {
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3658);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3670);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CB410);

void func_001CCD10(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CCD18);

void func_001CE0C0(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CE0D8);

void func_001CE5D8(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CE5F0);

void func_001CEA58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEA70);

void func_001CEB78(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEB80);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CED58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEED0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CEFA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CF050);

void func_001CF750(s32 *arguments) {
    s32 owner = arguments[0x34 / 4];
    s32 value = func_001FE320(owner, arguments[0x20 / 4]);
    func_001D4860(value);
    value = func_001D9468(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    func_001D4860(value);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CF7A0);

void func_001CFAB0(s32 object) {
    s32 context = func_001A17F0();
    s32 target = *(s32 *)(object + 0x18);
    *(s32 *)(context + 0x254) += 1;
    if (func_001A8640(target)) {
        *(u32 *)(context + 0x1F4) |= 0x2000;
    } else {
        *(u32 *)(context + 0x1F4) |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CFB10);

void func_001CFD70(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001CFD78);

void func_001D0040(void) {
}

void func_001D0048(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_001C85B8(arg0, 0x1194, 1);
    func_001D4860(temp_v0);
    func_001D0728(arg0, 0x1a);
}

void func_001D0088(void) {
}

extern s32 countBattleTasksForOwner(s64);

extern void func_001D0728(s32, s32);

void func_001D0090(s32 object) {
    s32 owner = *(s32 *)(object + 0x18);
    if (countBattleTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        func_001D0728(object, 0x1C);
    }
}

void func_001D00D8(void) {
}

void func_001D00E0(s32 object) {
    s32 owner = *(s32 *)(object + 0x18);
    if (countBattleTasksForOwner(*(s64 *)(owner + 0x108)) == 0) {
        *(u32 *)(owner + 0x110) &= ~0x4000;
        func_001D0728(object, 2);
    }
}

void func_001D0148(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0150);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0210);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0498);

void func_001D0590(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0598);

void func_001D0668(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

extern s32 func_001A17F0(void);

extern void func_001D0728(s32, s32);

void func_001D0680(s32 object) {
    void (*callback)(s32) = *(void (**)(s32))(func_001A17F0() + 0x660);
    if (callback != 0) {
        callback(object);
    }
    func_001D0728(object, 0x1A);
}

void func_001D06C0(void) {
}

void func_001D06C8(void) {
}

void func_001D06D0(void) {
    func_00214768();
}

void func_001D06E8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_00214868();
    if (temp_v0 == 0) {
        func_001D0728(arg0, 6);
        return;
    }
}

typedef struct BattleActionState {
    void (*start)(s32);
    void (*update)(s32);
    void (*finish)(s32);
} BattleActionState;
extern BattleActionState D_00359B28[];

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0728);

extern char D_003A3788[]; /* "btl:action seq create[%p]\n" */

u8 *func_001D0768(void) {
    u8 *sequence = func_002CFF68(0x170);
    u8 *context;
    u8 *head;

    *(u16 *)(sequence + 4) = 1;
    func_001D2BD0(sequence + 0x20);
    context = (u8 *)func_001A17F0();
    *(u8 **)(sequence + 0x168) = 0;
    head = *(u8 **)(context + 0x224);
    if (head != 0) {
        *(u8 **)(head + 0x168) = sequence;
        *(u8 **)(sequence + 0x16C) = *(u8 **)(context + 0x224);
    } else {
        *(u8 **)(sequence + 0x16C) = 0;
    }
    *(u8 **)(context + 0x224) = sequence;
    func_001D0728((s32)sequence, 0);
    func_001FB0A8(D_003A3788, sequence);
    return sequence;
}

extern char D_003A37A8[]; /* "btl:action seq delete[%p]\n" */

void func_001D07F0(s32 object) {
    func_001FB0A8(D_003A37A8, object);
    func_001D2C28(object + 0x20);
    if (*(s32 *)(object + 0x16C) != 0) {
        *(s32 *)(*(s32 *)(object + 0x16C) + 0x168) = *(s32 *)(object + 0x168);
    }
    if (*(s32 *)(object + 0x168) != 0) {
        *(s32 *)(*(s32 *)(object + 0x168) + 0x16C) = *(s32 *)(object + 0x16C);
    } else {
        s32 context = func_001A17F0();
        *(s32 *)(context + 0x224) = *(s32 *)(object + 0x16C);
    }
    func_002CFF98(object);
}

void func_001D0870(void) {
    s32 action = *(s32 *)(func_001A17F0() + 0x224);
    while (action != 0) {
        s32 flags = *(s32 *)(action + 8);
        s32 next = *(s32 *)(action + 0x16C);
        if (flags & 1) {
            D_00359B28[*(s32 *)action].update(action);
            *(s32 *)(action + 0x10) += 1;
        } else if (flags & 2) {
            func_001D07F0(action);
        }
        action = next;
    }
}

void func_001D0918(void) {
    u8 *node = *(u8 **)(func_001A17F0() + 0x224);
    while (node != 0) {
        u8 *next = *(u8 **)(node + 0x16C);
        func_001D07F0((s32)node);
        node = next;
    }
}

s32 func_001D0958(s32 target) {
    s32 context = func_001A17F0();
    s32 node = *(s32 *)(context + 0x224);
    while (node != 0) {
        if (*(s32 *)(node + 0x18) == target) {
            return node;
        }
        node = *(s32 *)(node + 0x16C);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3738);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3748);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3758);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3768);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3778);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3788);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A37A8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D09B8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0BA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0DD8);

s32 func_001D0E18(u8 *actor, s32 *argument) {
    switch (argument[0]) {
    case 1:
        if ((*(u64 *)(actor + 0x110) & 0x1200) == 0x200) {
            return func_001A30E0(*(u16 *)(actor + 0x172));
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        return 0;
    case 4:
        return func_001A3098(argument[2]);
    case 2:
    case 3:
    case 7:
    case 8:
        return argument[1];
    default:
        return -1;
    }
}

u32 func_001D0EA8(u8 *actor, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = func_001DAE48(*(s32 *)(argument + 0x40));
        if ((*(u64 *)(actor + 0x110) & 0x1200) == 0x1200 &&
            (*(u16 *)(actor + 0x12E) & 0x1000) == 0 &&
            count == 1) {
            u8 *option = *(u8 **)(argument + 0x60);
            if (*(s32 *)(option + 0xC) == 2 && option[0x14] == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (*(u32 *)(actor + 0x110) & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8:
        return *(u8 *)(D_003BAA60 + *(s32 *)(argument + 4) * 0x20);
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D0F98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D1118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D1218);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D12A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2B18);

void func_001D2BD0(u8 *object) {
    u32 handle;
    u32 value;
    *(u32 *)(object + 0x40) = (u32)allocateBattleIndexList(13);
    handle = func_002D03F8(0x836C);
    value = func_002D0A48(handle);
    *(u32 *)(object + 0x64) = handle;
    *(u32 *)(object + 0x60) = value;
    *(u64 *)(object + 0x48) = 0;
    func_001D2B18(object);
}

extern void func_001DAE08();

void func_001D2C28(u8 *object) {
    u32 handle = *(u32 *)(object + 0x64);
    if (handle != 0) {
        func_002D0918(handle);
        *(u32 *)(object + 0x64) = 0;
    }
    if (*(u32 *)(object + 0x40) != 0) {
        func_001DAE08(*(u32 *)(object + 0x40));
        *(u32 *)(object + 0x40) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2C78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D2F08);

u32 func_001D2FD8(u8 *arguments) {
    s32 context = func_001A17F0();
    u8 *actor = *(u8 **)arguments;
    s32 primary;
    u8 *resource;
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    primary = *(s32 *)(arguments + 0x20);
    if (primary == 0 && *(s32 *)(arguments + 0x24) == 0) {
        return 1;
    }
    if (*(u32 *)(actor + 0x110) & 0x60) {
        return 1;
    }
    resource = actor + 0x120;
    func_001A1868(resource, primary);
    func_001A1880(resource, *(s32 *)(arguments + 0x24));
    func_001D5990(actor);
    func_001A7F88(actor, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3078);

u32 func_001D3148(void *argument) {
    u32 *args = (u32 *)argument;
    s32 context = func_001A17F0();
    u8 *owner = (u8 *)args[0];
    if ((*(u32 *)(context + 0x1F4) & 0x80) == 0) {
        return 1;
    }
    func_001A1948(owner + 0x120, args[1]);
    func_001D5990(owner);
    func_001A7F88(owner, 0);
    return 1;
}

extern void *func_001D4748(s32);

extern u32 func_001D47D8(s32);

extern u32 func_001D3148(void *);

void *func_001D31B0(u8 *owner, u32 value) {
    u8 *task = func_001D4748(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x47;
    *(void **)(task + 0x4C) = func_001D3148;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3238);

extern u32 func_001D3238(void *);

void *func_001D3338(u8 *owner, u32 value, u32 extra) {
    u8 *task = func_001D4748(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D3238;
    *(u16 *)(task + 0x20) = 0x48;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = value;
    arguments[1] = extra;
    return task;
}

s32 func_001D33D0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    func_001A4C68(*(s32 *)temp_v0, *(s32 *)(temp_v0 + 0x14), *(s16 *)(temp_v0 + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3400);

u32 func_001D34D0(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        func_001A1978(*arg0, arg0[7]);
        func_001D5990(*arg0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3510);

u32 func_001D35E0(u32 *arg0) {
    func_001A1980(*arg0);
    func_001D5990(*arg0);
    return 1;
}

void *func_001D3618(u8 *owner) {
    u8 *task = func_001D4748(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D35E0;
    *(u16 *)(task + 0x20) = 0x4B;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3688);

extern void func_001D3688();

u8 *func_001D3998(u8 *arg0, s32 arg1) {
    u8 *task = func_001D4748(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4C;
    *(void **)(task + 0x4C) = func_001D3688;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 func_001D3A20(s32 arg0) {
    if ((*(u8 *)(*(s32 *)(arg0 + 4) * 8 + D_003BAA68 + 1) & 4) != 0) {
        func_00119900(*(s32 *)(arg0 + 4), 0xffffffffffffffff);
    }
    return 1;
}

u8 *func_001D3A60(s32 arg0, s32 arg1) {
    u8 *task = func_001D4748(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4D;
    *(void **)(task + 0x4C) = func_001D3A20;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = (u32)arg0;
    data[1] = (u32)arg1;
    return task;
}

u32 func_001D3AE8(s32 arg0) {
    func_00119900(*(u16 *)(arg0 + 4), 1);
    return 1;
}

u8 *func_001D3B10(u8 *arg0, u16 arg1) {
    u8 *task = func_001D4748(8);
    u32 *data;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4E;
    *(void **)(task + 0x4C) = func_001D3AE8;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = (u32)arg0;
    ((u16 *)data)[2] = arg1;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3B98);

extern u32 func_001D3B98(s32);

u8 *func_001D3C08(u8 *owner, s32 value) {
    u8 *task = func_001D4748(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x4F;
    *(void **)(task + 0x4C) = func_001D3B98;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3C90);

extern u32 func_001D3C90(void *);

void *func_001D3D00(u8 *owner, u32 value) {
    u8 *task = func_001D4748(8);
    s64 data;
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x50;
    *(void **)(task + 0x4C) = func_001D3C90;
    data = *(s64 *)(owner + 0x108);
    *(s64 *)(task + 0x40) = data;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    return task;
}

u32 func_001D3D88(void) {
    u8 *context = (u8 *)func_001A17F0();
    u8 *actor = *(u8 **)(context + 0x228);
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 &&
                    (u16)(*(u16 *)(actor + 0x124) - 1) < 0x17F) {
                    u32 entry = *(u32 *)(D_003BAA1C + *(u16 *)(actor + 0x124) * 76);
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((*(u32 *)(actor + 0x114) & 8) == 0) {
                                u16 prior = *(u16 *)(actor + 0x12E);
                                func_001A1948(actor + 0x120, 1);
                                func_001D5990(actor);
                                if (*(u16 *)(actor + 0x12E) == 1 &&
                                    prior != *(u16 *)(actor + 0x12E)) {
                                    *(u32 *)(actor + 0x114) |= 4;
                                    *(u32 *)(context + 0x1F8) |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return 1;
}

void *func_001D3EB8(void) {
    u8 *task = func_001D4748(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = func_001D3D88;
    *(u16 *)(task + 0x20) = 0x51;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}

extern char D_003A3A40[];
extern char D_003A3A50[];

void func_001D3F00(void) {
    u8 *context = (u8 *)func_001A17F0();
    u32 flags = *(u32 *)(context + 0x1F4);
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if ((s8)D_00324510[0x22] < 0 || (s8)D_00324510[0x23] < 0) {
            *(u32 *)(context + 0x1F4) = flags & ~0x8000;
            soundSetSequenceVolumePan(6, 0x7F, 0x3F);
            func_001AD668(0);
            func_001FB0A8(D_003A3A40);
        }
    } else if ((s8)D_00324510[0x22] < 0) {
        *(u32 *)(context + 0x1F4) = flags | 0x8000;
        soundSetSequenceVolumePan(5, 0x7F, 0x3F);
        func_001AD668(1);
        func_001FB0A8(D_003A3A50);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D3FE8);

void func_001D43E0(void) {
}

s32 func_001D43E8(s64 owner) {
    s32 node = *(s32 *)(func_001A17F0() + 0x230);
    while (node != 0) {
        if (*(s64 *)(node + 0x38) == owner) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 func_001D4448(s64 owner) {
    s32 node = *(s32 *)(func_001A17F0() + 0x230);
    while (node != 0) {
        if (*(s64 *)(node + 0x40) == owner) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 findBattleTaskByKind(u16 kind) {
    s32 node = *(s32 *)(func_001A17F0() + 0x230);
    while (node != 0) {
        if (*(u16 *)(node + 0x20) == kind) {
            return node;
        }
        node = *(s32 *)(node + 0x58);
    }
    return 0;
}

s32 func_001D4508(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(temp_v0 + 0x230); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

s32 countBattleTasksForOwner(s64 key) {
    s32 node = *(s32 *)(func_001A17F0() + 0x230);
    s32 count = 0;
    while (node != 0) {
        s64 owner = *(s64 *)(node + 0x40);
        node = *(s32 *)(node + 0x58);
        if (owner == key) {
            count++;
        }
    }
    return count;
}

s32 countBattleTasksByKind(u16 kind) {
    s32 node = *(s32 *)(func_001A17F0() + 0x230);
    s32 count = 0;
    while (node != 0) {
        u16 nodeKind = *(u16 *)(node + 0x20);
        node = *(s32 *)(node + 0x58);
        if (nodeKind == kind) {
            count++;
        }
    }
    return count;
}

void func_001D45F8(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x22C);
    while (node != 0) {
        u16 flags = *(u16 *)(node + 0x24);
        s32 next = *(s32 *)(node + 0x5C);
        if ((flags & 1) != 0) {
            *(u16 *)(node + 0x24) = flags | 4;
        }
        node = next;
    }
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3A40);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4648);

void *func_001D4748(s32 size) {
    u8 *task = func_002CFF68(size + 0x70);
    u8 *context;
    u8 *head;

    if (size > 0) {
        *(u8 **)(task + 0x54) = task + 0x70;
    } else {
        *(u8 **)(task + 0x54) = 0;
    }
    context = (u8 *)func_001A17F0();
    *(u8 **)(task + 0x58) = 0;
    head = *(u8 **)(context + 0x22C);
    if (head != 0) {
        *(u8 **)(head + 0x58) = task;
        *(u8 **)(task + 0x5C) = *(u8 **)(context + 0x22C);
    } else {
        *(u8 **)(context + 0x230) = task;
        *(u8 **)(task + 0x5C) = 0;
    }
    *(u8 **)(context + 0x22C) = task;
    *(u16 *)(task + 0x24) |= 1;
    return task;
}

u32 func_001D47D8(s32 arg0) {
    return *(u32 *)(arg0 + 0x54);
}

void func_001D47E0(s32 task) {
    s32 context;
    s32 next;
    s32 previous;
    void (*cleanup)(s32);
    cleanup = *(void (**)(s32))(task + 0x50);
    if (cleanup != 0) {
        cleanup(*(s32 *)(task + 0x54));
    }
    context = func_001A17F0();
    previous = *(s32 *)(task + 0x5C);
    if (previous != 0) {
        *(s32 *)(previous + 0x58) = *(s32 *)(task + 0x58);
    } else {
        *(s32 *)(context + 0x230) = *(s32 *)(task + 0x58);
    }
    next = *(s32 *)(task + 0x58);
    if (next != 0) {
        *(s32 *)(next + 0x5C) = *(s32 *)(task + 0x5C);
    } else {
        *(s32 *)(context + 0x22C) = *(s32 *)(task + 0x5C);
    }
    func_002CFF98(task);
}

extern u64 func_001A0CB0();

u64 func_001D4860(s32 task) {
    u64 value = func_001A0CB0();
    s32 callback = *(s32 *)(task + 0x48);
    *(u16 *)(task + 0x24) |= 8;
    *(u64 *)(task + 0x38) = value;
    *(u32 *)(task + 0x30) = 0;
    *(u32 *)(task + 0x34) = 0;
    *(u16 *)(task + 0x22) = 0;
    *(u32 *)(task + 0x60) = 0;
    *(u32 *)(task + 0x64) = 0;
    if (callback != 0) {
        ((void (*)(s32))callback)(*(s32 *)(task + 0x54));
    }
    return *(u64 *)(task + 0x38);
}

void func_001D48C0(void) {
    D_003BB5EC = 0;
    D_003BB5F0 = 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D48D0);

extern void func_001D48D0(s32);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4A10);

void clearDeferredBattleTasks(void) {
    s32 node = D_003BB5F0;
    while (node != 0) {
        s32 next = *(s32 *)(node + 0x60);
        func_001D48D0(node);
        node = next;
    }
    D_003BB5EC = 0;
    D_003BB5F0 = 0;
}

void func_001D4AE0(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x22C);
    while (node != 0) {
        s32 next = *(s32 *)(node + 0x5C);
        func_001D47E0(node);
        node = next;
    }
    D_003BB5EC = 0;
    D_003BB5F0 = 0;
}

u32 func_001D4B28(void) {
    return 1;
}

void *func_001D4B30(void) {
    u8 *task = func_001D4748(0);
    task[0] = 1;
    *(void **)(task + 0x4C) = func_001D4B28;
    *(u16 *)(task + 0x20) = 0x63;
    *(u32 *)(task + 0x48) = 0;
    task[0x10] = 0;
    return task;
}

extern s32 func_001FB0A8(const char *, ...);

void dumpBattleTaskQueue(void) {
    s32 context = func_001A17F0();
    s32 node = *(s32 *)(context + 0x230);
    while (node != 0) {
        func_001FB0A8("btl:packet[%d]\n", *(u16 *)(node + 0x20));
        node = *(s32 *)(node + 0x58);
    }
    func_001FB0A8("btl:packet head[%p]\n", *(void **)(context + 0x22C));
    func_001FB0A8("btl:packet tail[%p]\n", *(void **)(context + 0x230));
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4BF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4C38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4CA8);

extern s32 func_002183D0(s32);

extern s32 func_002183E0(s32);

s32 hasMatchingBattleModel(s32 effect, s32 model) {
    s32 context = func_001A17F0();
    s32 node = *(s32 *)(context + 0x228);
    while (node != 0) {
        if ((*(u32 *)(node + 0x110) & 2) != 0 &&
            *(s32 *)(node + 0x320) != 0 &&
            *(s32 *)(node + 0x308) != 0 &&
            func_002183D0(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == effect &&
            func_002183E0(*(s32 *)(*(s32 *)(node + 0x320) + 0x8C)) == model) {
            return 1;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4E60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D4E98);

extern const char D_003A3AD0[];

void func_001D5238(u8 *object) {
    s32 sound;
    s32 load;
    s32 model;
    u32 state;
    u32 flags;
    if (object[0xCC] == 0) {
        sound = *(s32 *)(object + 0x308);
        if (sound != 0) {
            func_001F3F40(sound);
            *(s32 *)(object + 0x308) = 0;
        }
        load = *(s32 *)(object + 0x324);
        if (load != 0) {
            func_002D7AC8(load, 1, 1);
            *(s32 *)(object + 0x324) = 0;
            func_001FB0A8(D_003A3AD0, object);
        }
        model = *(s32 *)(object + 0x31C);
        if (model != 0) {
            func_00110928(model);
            *(s32 *)(object + 0x31C) = 0;
            *(s32 *)(object + 0x320) = 0;
        }
    } else {
        *(s32 *)(object + 0x308) = 0;
        *(s32 *)(object + 0x324) = 0;
        *(s32 *)(object + 0x31C) = 0;
        *(s32 *)(object + 0x320) = 0;
    }
    state = *(u32 *)(object + 0x118) & ~1;
    flags = *(u32 *)(object + 0x110) & ~2;
    state &= ~2;
    *(u32 *)(object + 0x110) = flags;
    *(u32 *)(object + 0x118) = state;
}

void func_001D52F8(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_002118D8(arg1, arg2);
        return;
    }
    func_00217068(arg1, arg2, 0);
}

void func_001D5358(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_00211708(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D53B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5440);

void func_001D54C0(u8 *object) {
    s32 (*callback)(u8 *);
    u32 flags;
    u32 masked;

    callback = *(s32 (**)(u8 *))(func_001A17F0() + 0x658);
    if (callback != 0 && callback(object) == 0) {
        return;
    }
    flags = *(u32 *)(object + 0x110);
    masked = flags & ~4;
    masked &= ~8;
    *(u32 *)(object + 0x110) = masked;
    if ((flags & 2) != 0) {
        u32 *resource = *(u32 **)(*(u8 **)(object + 0x320) + 0x8C);
        *resource |= 1;
    }
}

u32 func_001D5538(u8 *object) {
    u32 flags = *(u32 *)(object + 0x110);
    if ((flags & 0x8000000) != 0) {
        return 0;
    }
    if ((flags & 1) == 0) {
        return 0;
    }
    if ((flags & 2) == 0) {
        return 0;
    }
    return **(u8 **)(*(u8 **)(object + 0x320) + 0x8C) & 1;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3AD0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5578);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5990);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5D58);

extern void func_001D5578(u8 *, s32, s32, f32);

void func_001D5DF8(u8 *object, s32 index, s32 argument, f32 scale) {
    u8 *resource = (u8 *)func_001A2FD8(*(s32 *)(object + 0xC4), *(s32 *)(object + 0xC8));
    f32 value = *(f32 *)(resource + index * 20 + 0x34);
    func_001D5578(object, index, argument, value * scale);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D5E70);

void func_001D5ED0(void) {
    s32 context = func_001A17F0();
    u8 *object = *(u8 **)(context + 0x228);

    while (object != 0) {
        if (*(u32 *)(object + 0x110) & 2) {
            u8 *resource = (u8 *)func_001A2FD8(*(s32 *)(object + 0xC4),
                                                  *(s32 *)(object + 0xC8));
            s32 model = *(s32 *)(*(s32 *)(object + 0x320) + 0x8C);
            s32 node = mdlGetNodeField2C(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                func_001D5D58(object) == 0) {
                func_001D5990(object);
                func_001D5578(object, *(s32 *)(object + 0xFC),
                              *(s32 *)(object + 0x100),
                              *(f32 *)(object + 0x104));
            }
        }
        object = *(u8 **)(object + 0x344);
    }
}

void func_001D5F98(u8 *object) {
    s32 context;
    u8 *resource;
    f32 volume;
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    context = func_001A17F0();
    *(u32 *)(object + 0xE8) &= ~1;
    volume = *(f32 *)(object + 0xF4);
    resource = *(u8 **)(object + 0x320);
    *(f32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) =
        volume * (30.0f / (f32)*(s8 *)(context + 0x490));
}

void func_001D6018(u8 *object) {
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        u8 *resource = *(u8 **)(object + 0x320);
        *(u32 *)(object + 0xE8) |= 1;
        *(u32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6050);

f32 func_001D60E0(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0.0f;
    }
    return *(f32 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x1c);
}

void func_001D6110(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_002DB538(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 800) + 0x8c) + 0x1c));
        return;
    }
}

s32 func_001D6148(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return 0;
    }
    return *(u16 *)(*(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c) + 0x2e);
}

void func_001D6170(s32 arg0) {
    extern void func_002DB538(void *, float);

    if ((*(u32 *)(arg0 + 0x110) & 2) == 0) {
        return;
    }
    func_002DB538(*(void **)(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c) + 0x1c), 0.0f);
}

void func_001D61A8(u8 *object) {
    s32 duration;
    u32 randomFrame;
    f32 frame;
    u8 *resource;
    extern void func_002DB538(void *, f32);

    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return;
    }
    duration = func_001D6148((s32)object);
    if (duration > 0) {
        randomFrame = (u32)effMiscRandMod(0, duration);
        frame = (f32)randomFrame;
        resource = *(u8 **)(object + 0x320);
        func_002DB538(*(void **)(*(u8 **)(resource + 0x8C) + 0x1C),
                       frame);
    }
}

u32 func_001D6238(s32 object) {
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 1;
    }
    if (*(s32 *)(object + 0xF0) != 2) {
        return 1;
    }
    return *(u8 *)(*(s32 *)(*(s32 *)(*(s32 *)(object + 0x320) + 0x8C) + 0x1C) + 0x30) == 5;
}

void func_001D6280(u8 *object, void *position) {
    f32 world[4] __attribute__((aligned(16)));
    s32 context;
    if ((*(u32 *)(object + 0x114) & 0x80) != 0) {
        return;
    }
    context = func_001A17F0();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(position));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x60) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder" : : "r"(context));
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(world) : "memory");
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        world[2] += *(f32 *)(object + 0x88);
        effObjSetInnerFirstVec(*(s32 *)(object + 0x31C), world);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6300);

void func_001D6318(u8 *object, void *worldPosition) {
    s32 context = func_001A17F0();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\tlqc2 vf11, 0(%1)\n\tvadd.xyzw vf10, vf10, vf11\n\tsqc2 vf10, 0(%2)\n\t.set reorder"
                     : : "r"(object + 0x60), "r"(context), "r"(worldPosition) : "memory");
}

extern void func_001D6820(u8 *);

s32 func_001D6360(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(func_001A17F0() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    func_001D6820(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)func_002D9E98(*(s32 *)(effect + 0x18), value);
    }
}

void func_001D63E8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001D6360();
    if (temp_v0 == 0) {
        func_001F6498(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6428);

extern s32 func_002D9E58(s32, s32);

s32 func_001D6518(object, value)
u8 *object;
s32 value;
{
    s32 (*callback)(u8 *, s32);
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        return 0;
    }
    callback = *(s32 (**)(u8 *, s32))(func_001A17F0() + 0x5BC);
    if (callback != 0) {
        value = callback(object, value);
    }
    func_001D6820(object);
    {
        u8 *resource = *(u8 **)(object + 0x320);
        u8 *effect = *(u8 **)(resource + 0x8C);
        return (s8)func_002D9E58(*(s32 *)(effect + 0x18), value);
    }
}

void func_001D65A0(void) {
    if (func_001D6518() != 0) {
        return;
    }
    __asm__ volatile(".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\tvmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder");
}

s32 func_001D65D8(u8 *object) {
    f32 position[3];
    func_001D6300(object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6640);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D66D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D66E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6758);

void func_001D67F0(s32 arg0, s32 arg1) {
    func_00218440(*(s32 *)(*(s32 *)(arg0 + 0x320) + 0x8c), (arg1 & 0xffffff) | 0x80000000);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6820);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6898);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6918);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D69A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6A80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3B70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6DB8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6E48);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D6FB0);

extern char D_003A3BA8[];
extern char D_003BB5F8[];

s32 func_001D71B8(u8 *actor, char *filename) {
    u32 flags;
    func_001A17F0();
    flags = *(u32 *)(actor + 0x110);
    if (!(flags & 0x200)) {
        return 0;
    }
    if (flags & 0x1000) {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8, 0,
                      *(u16 *)(actor + 0x124));
    } else {
        func_003014F0(filename, D_003A3BA8, D_003BB5F8,
                      func_001A30E0(*(u16 *)(actor + 0x172)),
                      *(u16 *)(actor + 0x124));
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3BA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D74B8);

u32 func_001D7578(u8 *arguments) {
    s32 index = *(s32 *)(arguments + 4);
    if (index >= 0) {
        func_001D5DF8(*(u8 **)arguments, index, *(s32 *)(arguments + 8),
                        *(f32 *)(arguments + 0xC));
    }
    return 1;
}

u8 *func_001D75B0(u8 *owner, s32 index, s32 value, f32 scale) {
    u8 *task = func_001D4748(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 9;
    *(void **)(task + 0x4C) = func_001D7578;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(f32 *)(arguments + 3) = scale;
    return task;
}

u32 func_001D7658(u32 *arg0) {
    func_001D5F98(*arg0);
    return 1;
}

void *func_001D7678(u8 *owner) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D7658;
    *(u16 *)(task + 0x20) = 10;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = (u32)owner;
    return task;
}

u32 func_001D76E8(s32 *arguments) {
    if ((s32)func_001D60E0(arguments[0]) >= arguments[1]) {
        if ((*(u32 *)(arguments[0] + 0xE8) & 1) == 0) {
            func_001D6018((u8 *)arguments[0]);
        }
        return 1;
    }
    return 0;
}


void *scheduleBattleThresholdTask(u8 *owner, u32 threshold) {
    u8 *task = func_001D4748(8);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0xB;
    *(void **)(task + 0x4C) = func_001D76E8;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = threshold;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D77D0);

extern u32 func_001D77D0(u32 *);

u8 *func_001D79D8(u8 *owner, s32 index, f32 scale) {
    u8 *task = func_001D4748(24);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D77D0;
    *(u16 *)(task + 0x20) = 0xE;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    *(f32 *)(arguments + 3) = scale;
    arguments[2] = 0;
    arguments[5] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7A78);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7B60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7C10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D7CF8);

extern char D_003A3BC8[];
extern char D_003A3BE8[];
extern void func_001D4E98(u8 *, u32, u32);

void func_001D7DA8(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        return;
    }
    if (hasMatchingBattleModel(effect, model)) {
        func_001D4E98(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            func_001D54C0(object);
            func_00221EF0(*(u32 *)(object + 0x320), 0, 0);
            *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
        }
        func_001FB0A8(D_003A3BC8, effect, model);
    } else {
        func_001D52F8(object, effect, model);
        *(u32 *)(object + 0x118) |= 1;
        func_001FB0A8(D_003A3BE8, effect, model);
    }
}

extern char D_003A3C08[];
extern s32 func_001D53B0(u8 *, u32, u32);

u32 func_001D7EA0(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((*(u32 *)(object + 0x110) & 2) == 0) {
        if (!func_001D53B0(object, effect, model)) {
            return 0;
        }
        func_001D4E98(object, effect, model);
        func_001D5358(object, effect, model);
        func_001FB0A8(D_003A3C08, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        func_001D54C0(object);
        func_00221EF0(*(u32 *)(object + 0x320), 0, 0);
        *(u32 *)(object + 0x84) = *(u32 *)(object + 0x54) & 0xFFFFFF;
    }
    *(u32 *)(object + 0x118) = (*(u32 *)(object + 0x118) & ~1) | 2;
    return 1;
}

u8 *func_001D7F90(u8 *owner, u32 index, u32 value, s8 mode) {
    u8 *task = func_001D4748(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x18;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = func_001D7DA8;
    *(void **)(task + 0x4C) = func_001D7EA0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = index;
    arguments[2] = value;
    *(s8 *)(arguments + 3) = mode;
    return task;
}

u32 func_001D8050(u32 *arg0) {
    func_001D54C0(*arg0);
    func_001D5238(*arg0);
    return 1;
}

void *scheduleBattleRefreshTask(u8 *owner) {
    u8 *task = func_001D4748(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D8050;
    *(u16 *)(task + 0x20) = 0x19;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3BC8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3BE8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3C08);

s32 beginBattleModelChange(u32 *arguments) {
    s32 owner = arguments[0];
    u32 model = arguments[1];
    u32 variant = arguments[2];
    s32 status = hasMatchingBattleModel(model, variant);

    if (status == 0) {
        func_001D52F8(owner, model, variant);
        *(u32 *)(owner + 0x118) = (*(u32 *)(owner + 0x118) | 1) & ~2;
        return func_001FB0A8("btl:model change start[%X,%X]\n", model, variant);
    }
    return status;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8190);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D87D0);

void func_001D88B0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        func_00221FE8(*(u32 *)(*(s32 *)(arg0 + 0xc) + 800));
        return;
    }
}

u32 func_001D88E0(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_00221D00(*(u32 *)(arg0[3] + 800), arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8930);

void func_001D89E0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        func_00221FE8(*(u32 *)(*(s32 *)(arg0 + 0x14) + 800));
        return;
    }
}

u32 func_001D8A10(u8 *arguments) {
    u8 *object = *(u8 **)(arguments + 0x14);
    if ((*(u32 *)(object + 0x110) & 2) != 0) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(arguments));
        func_00221D98(*(s32 *)(object + 0x320), *(s32 *)(arguments + 0x10));
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8A50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8AF0);

extern u32 func_001D8AF0(u32 *);

u8 *func_001D8C48(u8 *owner, u32 value, u32 variant) {
    u8 *task = func_001D4748(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D8AF0;
    *(u16 *)(task + 0x20) = 0x11;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[4] = 0x80808080;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8CF0);

extern u32 func_001D8CF0(u32 *);

u8 *func_001D8DE8(u8 *owner, u32 value, u32 variant) {
    u8 *task = func_001D4748(16);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D8CF0;
    *(u16 *)(task + 0x20) = 0x12;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = variant;
    arguments[3] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D8E80);

extern u32 func_001D8E80(u32 *);

u8 *func_001D9038(u8 *owner, u32 value) {
    u8 *task = func_001D4748(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x13;
    *(void **)(task + 0x4C) = func_001D8E80;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D90C0);

extern u32 func_001D90C0(u32 *);

u8 *func_001D91E0(u8 *owner, u32 value) {
    u8 *task = func_001D4748(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x14;
    *(void **)(task + 0x4C) = func_001D90C0;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9268);

extern u32 func_001D9268(u32 *);

u8 *func_001D9468(u8 *owner, u32 value) {
    u8 *task = func_001D4748(12);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x15;
    *(void **)(task + 0x4C) = func_001D9268;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D94F0);

extern u32 func_001D94F0(u32 *);

void func_001D9600(u32 *arguments) {
    u32 value = arguments[2];
    if (value != 0) {
        func_00160B00(value);
    }
    if (arguments[1] != 0) {
        func_00160800(arguments[1]);
    }
    func_001D54C0(arguments[0]);
    *(u32 *)(arguments[0] + 0x110) |= 0x40;
}

u8 *func_001D9660(u8 *owner) {
    u8 *task = func_001D4748(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x16;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x4C) = func_001D94F0;
    *(void **)(task + 0x50) = func_001D9600;
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = 0;
    arguments[4] = 0;
    arguments[3] = 0;
    return task;
}

u32 func_001D96F8(void) {
    func_001F7600();
    return 1;
}

SoundTask *func_001D9718(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001D96F8;
    task->taskId = 0x1B;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 func_001D9760(void) {
    func_001F76F0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9780);

u32 func_001D97D0(void) {
    return 1;
}

SoundTask *func_001D97D8(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001D97D0;
    task->taskId = 0x20;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9820);

extern u32 func_001D9820(u32 *);

void *func_001D9938(u8 *owner) {
    u8 *task = func_001D4748(8);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9820;
    *(u16 *)(task + 0x20) = 0x21;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D99B0);

extern u32 func_001D99B0(u32 *);

u8 *func_001D9BA0(u8 *owner, f32 value) {
    u8 *task = func_001D4748(12);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x1D;
    *(void **)(task + 0x4C) = func_001D99B0;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    *(f32 *)(arguments + 1) = value;
    arguments[2] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001D9C28);

extern void func_001D9C28();

u8 *func_001D9E48(u8 *arg0) {
    u8 *task = func_001D4748(0x10);
    u32 *data;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9C28;
    *(u16 *)(task + 0x20) = 0x1E;
    *(u64 *)(task + 0x40) = *(u64 *)(arg0 + 0x108);
    *(s32 *)(task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = (u32)arg0;
    data[1] = 0;
    return task;
}

u32 func_001D9EC0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    func_001D6640(temp_v0, temp_v0 + 0x40);
    return 1;
}

void *scheduleBattleActorUpdate(u8 *owner) {
    u8 *task = func_001D4748(4);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9EC0;
    *(u16 *)(task + 0x20) = 0x1F;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    return task;
}

u32 func_001D9F68(u32 *arg0) {
    func_001D6820(*arg0);
    return 1;
}

void *func_001D9F88(u8 *owner) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001D9F68;
    *(u16 *)(task + 0x20) = 0x22;
    *(s64 *)(task + 0x40) = *(s64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = (u32)owner;
    return task;
}

extern void func_002CF570(s32);
extern s32 func_00288B68(char *);

void func_001D9FF8(s32 task) {
    char filename[0x70];
    u8 *actor = *(u8 **)task;
    if ((*(u32 *)(actor + 0x110) & 0x400) != 0) {
        return;
    }
    if (*(s32 *)(actor + 0x30C) != 0) {
        func_002CF570(*(s32 *)(actor + 0x30C));
        *(s32 *)(actor + 0x30C) = 0;
    }
    if (func_001D71B8(actor, filename)) {
        s32 handle = func_00288B68(filename);
        *(s32 *)(task + 4) = handle;
        func_001FB0A8("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    *(u32 *)(actor + 0x118) = (*(u32 *)(actor + 0x118) | 4) & ~8;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA098);

extern void func_001D9FF8(s32);
extern u32 func_001DA098(u32 *);

u8 *func_001DA128(u8 *owner) {
    u8 *task = func_001D4748(8);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x23;
    *(u16 *)(task + 0x24) &= ~1;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = func_001D9FF8;
    *(void **)(task + 0x4C) = func_001DA098;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = 0;
    return task;
}

u32 func_001DA1B8(void) {
    func_001D5ED0();
    return 1;
}

SoundTask *func_001DA1D8(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001DA1B8;
    task->taskId = 0x24;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

extern void func_001D6DB8(u8 *);

extern s32 func_001DA220(u32 *);
s32 func_001DA220(u32 *arguments) {
    u8 *actor = (u8 *)arguments[0];
    if ((*(u32 *)(actor + 0x110) & 2) == 0) {
        return 0;
    }
    func_001D6DB8(actor);
    *(u32 *)((u8 *)arguments[0] + 0x110) |= 0x20000;
    return 1;
}

void *func_001DA278(u32 actor) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x25;
    *(void **)(task + 0x4C) = func_001DA220;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = actor;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA2E0);
extern s32 func_001DA2E0(u32 *);

u8 *func_001DA3A8(u8 *actor, u32 target, u32 index, u32 value, f32 scale) {
    u8 *task = func_001D4748(0x1C);
    u32 *arguments;
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001DA2E0;
    *(u16 *)(task + 0x20) = 0x26;
    *(u64 *)(task + 0x40) = *(u64 *)(actor + 0x108);
    *(u32 *)(task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)actor;
    arguments[1] = target;
    arguments[2] = index;
    arguments[4] = value;
    *(f32 *)(arguments + 5) = scale;
    arguments[3] = -1;
    arguments[6] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA468);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA780);

void func_001DA8F0(u8 *actor) {
    *(s32 *)(actor + 0x2F0) = -1;
    *(s32 *)(actor + 0x2F4) = -1;
    *(u32 *)(actor + 0x110) = 0;
    *(u32 *)(actor + 0x114) = 0;
    *(u32 *)(actor + 0x118) = 0;
    *(u16 *)(actor + 0x310) = 0;
    func_001A4860((u32)actor);
    *(u32 *)(actor + 0x2FC) = (u32)nbSoundAllocResourceLink(actor);
    *(u32 *)(actor + 0x300) = (u32)nbSoundAllocLink(actor);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DA948);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAA38);

extern char D_003A3D38[]; /* "btl:unit delete[%p]\n" */

void func_001DAB20(u8 *actor) {
    u8 *next;
    u8 *previous;

    func_001FB0A8(D_003A3D38, actor);
    func_001DAA38(actor);
    next = *(u8 **)(actor + 0x344);
    if (next != 0) {
        *(u8 **)(next + 0x340) = *(u8 **)(actor + 0x340);
    }
    previous = *(u8 **)(actor + 0x340);
    if (previous != 0) {
        *(u8 **)(previous + 0x344) = *(u8 **)(actor + 0x344);
    } else {
        *(u8 **)(func_001A17F0() + 0x228) = *(u8 **)(actor + 0x344);
    }
    func_002D0918(*(s32 *)(actor + 0x33C));
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DABA0);

void removeBattleActorsWithFlags(u32 mask) {
    s32 context = func_001A17F0();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        s32 next = *(s32 *)(actor + 0x344);
        if (*(u32 *)(actor + 0x110) & mask) {
            func_001DAB20(actor);
        }
        actor = next;
    }
}

s32 findBattleActorForOwner(s64 target) {
    s32 context = func_001A17F0();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (*(s64 *)(actor + 0x108) == target) {
            return actor;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 isActiveBattleActor(s32 candidate) {
    s32 context = func_001A17F0();
    s32 actor = *(s32 *)(context + 0x228);
    while (actor != 0) {
        if (actor == candidate) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

s32 func_001DACF8(s32 arg0) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) == 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

s32 func_001DAD60(s32 arg0) {
    s32 node = *(s32 *)(func_001A17F0() + 0x228);

    while (node != 0) {
        if (((*(u16 *)(node + 0x120) & 0x20) != 0) && (*(u16 *)(node + 0x124) == arg0)) {
            return node;
        }
        node = *(s32 *)(node + 0x344);
    }
    return 0;
}

void *allocateBattleIndexList(s32 capacity) {
    u8 *list = func_002CFF68(capacity * 4 + 12);
    *(s32 *)list = capacity;
    *(u32 **)(list + 8) = (u32 *)(list + 12);
    *(s32 *)(list + 4) = 0;
    return list;
}

void func_001DAE08(u32 ptr) {
    func_002CFF98(ptr);
}

void func_001DAE20(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 4);
    *(s32 *)(arg0 + 4) = temp_v0 + 1;
    *(u32 *)(temp_v0 * 4 + *(s32 *)(arg0 + 8)) = arg1;
}

void func_001DAE40(s32 arg0) {
    *(u32 *)(arg0 + 4) = 0;
}

u32 func_001DAE48(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001DAE50(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 8));
}

void copyBattleIndexList(s32 destination, s32 source) {
    u32 count;
    u32 index;

    func_001DAE40(destination);
    count = func_001DAE48(source);
    for (index = 0; index < count; index++) {
        func_001DAE20(destination, func_001DAE50(source, index));
    }
}

void func_001DAEE8(s32 arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    if (arg1 == arg2) {
        return;
    }
    temp_v0 = *(s32 **)(arg0 + 8);
    temp_v1 = temp_v0[arg1];
    temp_v2 = temp_v0[arg2];
    temp_v0[arg1] = temp_v2;
    temp_v0[arg2] = temp_v1;
}

u32 func_001DAF18(s32 arg0, s32 arg1) {
    u32 count = func_001DAE48(arg0);
    u32 index;

    for (index = 0; index < count; index++) {
        if (arg1 == func_001DAE50(arg0, index)) {
            return index;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DAF98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB048);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB218);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB300);

u32 func_001DB358(void) {
    return 1;
}

u32 func_001DB360(void) {
    return 1;
}

u32 func_001DB368(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB370);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB440);

extern void func_001F7CC8(u8 *, f32);
extern f32 func_001F7CD8(u8 *);
extern void func_001DC270(u8 *, u8 *);
extern void func_001DB370(u8 *, u8 *, u8 *, f32);

s32 func_001DB4F8(u8 *actor) {
    u8 *motion = actor + 0x134;
    u8 *position = actor + 0x30;
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        func_001F7CC8(motion, *(f32 *)(actor + 0x130));
        func_001DC270(actor, position);
    }
    value = func_001F7CD8(motion);
    func_001DB370(actor, position, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

extern void func_001F7D30(u8 *, f32);
extern f32 func_001F7D80(u8 *, f32);

s32 func_001DB590(u8 *actor) {
    f32 value;
    if (*(u32 *)(actor + 0x110) == 0) {
        *(u32 *)(actor + 0x128) = 0;
        func_001F7D30(actor + 0x13C, (f32)*(s32 *)(actor + 0x12C));
        func_001DC270(actor, actor + 0x30);
        return 0;
    }
    value = func_001F7D80(actor + 0x13C, 1.0f);
    func_001DB370(actor, actor + 0x30, actor + 0xC0, value);
    *(f32 *)(actor + 0x128) = value;
    return 0.9999990f <= value;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB630);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB698);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DB7D0);

u32 func_001DB8F8(u32 *arg0) {
    func_001DB048(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

void *func_001DB930(s32 owner, s32 variant) {
    SoundTask *task = (SoundTask *)func_001D4748(20);
    u32 *arguments;
    task->enabled = 1;
    task->taskId = 0x27;
    task->status = 0;
    if (owner != 0 && *(s32 *)(owner + 0x18) != 0) {
        *(u64 *)((u8 *)task + 0x40) = *(u64 *)(*(s32 *)(owner + 0x18) + 0x108);
    }
    task->callback.update = (void (*)(void))func_001DB8F8;
    *(u32 *)((u8 *)task + 0x48) = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = owner;
    arguments[3] = variant;
    arguments[1] = 0;
    arguments[2] = 0;
    arguments[4] = 0;
    return task;
}


void *func_001DB9D0(s32 owner, s32 variant, u32 target) {
    void *task = func_001DB930(owner, variant);
    u32 *arguments = (u32 *)func_001D47D8((s32)task);
    arguments[4] = target;
    return task;
}

void *func_001DBA10(s32 owner, s32 variant, u32 first, u32 second, u32 third) {
    void *task = func_001DB930(owner, second);
    u32 *arguments = (u32 *)func_001D47D8((s32)task);
    arguments[4] = third;
    arguments[1] = variant;
    arguments[2] = first;
    return task;
}

extern void func_001DC338(u8 *, f32, f32, f32, f32, f32, f32, f32, f32);

u32 func_001DBA78(u8 *arguments) {
    u8 *context = (u8 *)func_001A17F0();
    func_001DB048(1, *(u32 *)arguments, 0, 0, 0);
    func_001DC338(context + 0x70, *(f32 *)(arguments + 4), *(f32 *)(arguments + 8),
                    *(f32 *)(arguments + 0xC), *(f32 *)(arguments + 0x10),
                    *(f32 *)(arguments + 0x14), *(f32 *)(arguments + 0x18),
                    *(f32 *)(arguments + 0x1C), *(f32 *)(arguments + 0x20));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBAF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBBF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBCB0);

u32 func_001DBDF8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_001DC858(temp_v0 + 0x70);
    return 1;
}

SoundTask *scheduleBattleContextReset(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001DBDF8;
    task->taskId = 0x2A;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DBE68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC0E8);

void func_001DC220(void) {
    s32 context = func_001A17F0();
    s32 actor = *(s32 *)(context + 0x188);
    if (actor != 0) {
        func_001DAE08(actor);
        *(s32 *)(context + 0x188) = 0;
    }
    *(u32 *)(context + 0x1F4) &= ~0x10;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC270);

void func_001DC2A0(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC2A8);

extern void func_001DC2A8(u8 *, f32 *, f32 *);

void func_001DC338(u8 *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
                    f32 vz, f32 vw, f32 scale) {
    f32 origin[4];
    f32 direction[4];
    origin[0] = x;
    origin[1] = y;
    origin[2] = z;
    direction[0] = vx;
    direction[1] = vy;
    direction[2] = vz;
    direction[3] = vw;
    origin[3] = 0.0f;
    func_001DC2A8(object, origin, direction);
    *(f32 *)(object + 0x24) = scale * 0.017453293f;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC3A0);

u32 func_001DC470(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(temp_v0 + 0x174);
}

f32 func_001DC490(s32 arg0) {
    return *(f32 *)(arg0 + 0x128);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC498);

void func_001DC538(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x164) = 0;
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x400;
    func_001DAE40(*(u32 *)(temp_v0 + 0x188));
}

void func_001DC568(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) & 0xffffdfff;
}

void func_001DC598(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 0x160) = *(u32 *)(temp_v0 + 0x160) | 0x2000;
}

u32 func_001DC5C0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return ((*(s32 *)(temp_v0 + 0x160) >> 0xd) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC5F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC698);

s32 func_001DC6F0(void) {
    s32 context = func_001A17F0();
    if ((*(u32 *)(context + 0x1F4) & 2) == 0) {
        return 0;
    }
    {
        s32 state = func_001109F0(func_0010FD80());
        if (state == 0) {
            return 0;
        }
        return *(s32 *)(state + 8) == 0;
    }
}

s32 func_001DC740(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return temp_v0 + 0x70;
}

void func_001DC760(void) {
    func_001F73E0();
}

void func_001DC778(void) {
    func_001F7428();
}

void func_001DC790(s32 arg0) {
    func_001F7470(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0xf4) + 0x18) + 0x110) & 0x600);
}

void func_001DC7B8(u8 *resource) {
    u32 flags = 0;
    u32 count = func_001DAE48(*(s32 *)(resource + 0x118));
    u32 index;
    for (index = 0; index < count; index++) {
        u8 *actor = (u8 *)func_001DAE50(*(s32 *)(resource + 0x118), index);
        flags |= *(u32 *)(actor + 0x110) & 0x600;
    }
    if (flags != 0) {
        func_001F7470(flags);
    }
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D38);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3D90);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC858);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DC9B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCB10);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCC38);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCD00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCD70);

u32 func_001DCE48(u8 *object) {
    u8 *resource = *(u8 **)(object + 0xF4);
    u32 count;
    u32 index;
    u8 *entry;
    if (resource == 0) {
        return 0;
    }
    count = func_001DAE48(*(s32 *)(resource + 0x60));
    entry = *(u8 **)(resource + 0x80);
    for (index = 0; index < count; index++, entry += 0xA1C) {
        if (entry[0x10] != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCEC8);

extern f32 func_001F66D8(s32, s32, s32);

s32 func_001DCF48(void) {
    if (func_001F66D8(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DCF88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD068);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD128);

s32 func_001DD198(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(D_003BAA60 + index * 0x20 + 0x1C) & 0x100) == 0) {
        return 0;
    }
    return 1;
}

s32 func_001DD1C8(s32 arg0) {
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x114);
    if (temp_v1 == 0) {
        return 0;
    }
    return ((*(s32 *)(D_003BAA50 + temp_v1 * 56 + 0x30) ^ 2) < 1U);
}

u32 func_001DD200(s32 actor) {
    s32 index;
    if (func_001DD310(actor) != 0) {
        return 1;
    }
    if (func_001DCD70(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(D_003BAA60 + index * 0x20 + 0x1C) & 2) != 0) {
        return 1;
    }
    return 0;
}

s32 func_001DD270(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index != 0 && *(u8 *)(D_003BAA50 + index * 56 + 8) != 0) {
        return 0;
    }
    return func_001DAE48(*(s32 *)(actor + 0x118)) == 1;
}

u32 func_001DD2C0(s32 actor) {
    s32 index;
    if (func_001DCD70(actor) == 0) {
        return 0;
    }
    index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(D_003BAA60 + index * 0x20 + 0x1C) & 4) != 0) {
        return 1;
    }
    return 0;
}

s32 func_001DD310(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    return *(s32 *)(D_003BAA50 + index * 56 + 0x30) == 1;
}

s32 func_001DD348(s32 actor) {
    s32 index = *(s32 *)(actor + 0x114);
    if (index == 0) {
        return 0;
    }
    if ((*(u16 *)(D_003BAA60 + index * 0x20 + 0x1C) & 0x40) == 0) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD378);

s32 func_001DD3F8(u8 *node) {
    u8 *resource = *(u8 **)(node + 0xF4);
    u8 *actor;
    u32 id;
    if (resource == 0) return 0;
    if (func_001DAE48(*(s32 *)(resource + 0x60)) >= 2) return 0;
    actor = (u8 *)func_001DAE50(*(s32 *)(resource + 0x60), 0);
    if ((*(u32 *)(actor + 0x110) & 0x400) == 0) return 0;
    id = *(u32 *)(actor + 0xC8);
    if (id >= 0x180) return 0;
    if (*(u32 *)(D_003BAA1C + id * 76) & 0x1000) return 1;
    return 0;
}

u8 func_001DD488(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x5f;
}

u8 func_001DD498(s32 arg0) {
    return *(s32 *)(arg0 + 0x114) == 0x1a0;
}

void func_001DD4A8(void) {
}

void func_001DD4B0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD4B8);

void func_001DD678(void) {
}

void func_001DD680(u32 arg0) {
    func_001DF358(arg0, arg0);
}

void func_001DD698(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD6A0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD7E8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DD890);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DDE28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DDF20);

void func_001DE4A8(u8 *actor) {
    s32 (*callback)(u8 *) = *(s32 (**)(u8 *))(func_001A17F0() + 0x634);
    if (callback != 0 && callback(actor) != 0) {
        return;
    }
    if (*(u16 *)(actor + 0x10C) == 12) {
        func_001EF098(actor, actor);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE508);

void func_001DE5F0(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE5F8);

void func_001DE958(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DE960);

void func_001DEA68(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEA70);

void func_001DEBB0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E60C0(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEBE0);

void func_001DEDB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    if ((*(u32 *)(temp_v0 + 0xf0) & 0x10000) != 0) {
        return;
    }
    func_001E6260(arg0, temp_v0);
}

void func_001DEDE8(u32 arg0) {
    func_001E6368(arg0, arg0);
}

void func_001DEE00(u32 arg0) {
    func_001E6580(arg0, arg0);
}

void func_001DEE18(u32 arg0) {
    func_001E4AC0(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001DEE38(void) {
    func_001E4E50();
}

void func_001DEE50(u8 *actor) {
    u8 *resource = *(u8 **)(actor + 0xF4);
    func_001DAE20(*(s32 *)(actor + 0x118), *(s32 *)(resource + 0x18));
    func_001E5198(actor, actor + 0x30, actor + 0xC0);
    func_001F7428();
    resource = *(u8 **)(actor + 0xF4);
    func_001D5440(*(s32 *)(resource + 0x18));
    *(f32 *)(actor + 0x130) = 50.0f;
    *(u32 *)(actor + 0xF0) |= 0x41;
}

void func_001DEEC0(void) {
}

void func_001DEEC8(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0xf4) + 0x18) + 0x110) & 0x200) != 0) {
        func_001E4960(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x108) != 0x10) {
        func_001E4AA8(arg0, arg0);
        return;
    }
}

void func_001DEF20(void) {
}

void func_001DEF28(s32 arg0) {
    if (*(s32 *)(arg0 + 0xf4) != 0) {
        func_001EF158(*(s32 *)(arg0 + 0xf4));
        return;
    }
}

void func_001DEF58(void) {
}

s32 func_001DEF60(s32 actor) {
    s32 context = func_001A17F0();
    s32 (*callback)(s32) = *(void **)(context + 0x618);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

s32 func_001DEFA0(s32 actor) {
    s32 context = func_001A17F0();
    s32 (*callback)(s32) = *(void **)(context + 0x620);
    if (callback != 0) {
        return callback(actor);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DEFE0);

void func_001DF358(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 27.5f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF378);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DF768);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFA60);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFAE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFD70);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001DFE28);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0100);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0258);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0398);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0718);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0B68);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E0DA0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1288);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E16C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1CF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E1FD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E20C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2578);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2878);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2970);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2B98);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2D20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E2FF8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3310);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E37B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3920);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E3E58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4180);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4578);

void func_001E4708(void) {
    func_001DF378();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4720);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4960);

void func_001E4AA8(s32 arg0, s32 arg1) {
    func_001DEFE0(arg0, arg1, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4AC0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E4E50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5198);

void func_001E5460(u32 arg0) {
    func_001E2878(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5478);

void func_001E5700(u32 arg0) {
    func_001E5460(arg0);
}

void func_001E5718(void) {
    func_001E5478();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5730);

void func_001E57F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3E40);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3F00);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A3FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4000);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4180);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4190);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4310);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4320);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A43E0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A43F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4400);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4408);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4468);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E5800);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E60C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6180);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6260);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6368);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6580);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6620);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6668);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6AC8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6B58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4668);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A46F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E6BB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001E9DE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EB1B0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EB368);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EBE88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ECCA8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ED550);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001ED5C8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EDB20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EE160);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EE658);

u32 func_001EEA78(u32 mask) {
    s32 context = func_001A17F0();
    u8 *actor = *(u8 **)(context + 0x228);
    u32 count = 0;
    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if ((flags & 1) != 0 && (flags & mask) != 0 && (flags & 0x20) == 0) {
            count++;
        }
        actor = *(u8 **)(actor + 0x344);
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEAE0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEC20);

extern u8 D_0035F100[];

void func_001EECF0(s32 actor) {
    memset(D_0035F100, 0, 0x130);
    func_001E2878(actor, actor);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EED30);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEE08);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEED8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EEFB0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF098);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF158);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF1F8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF390);

void func_001EF420(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = func_001A17F0();
    temp_v0 = func_0010FA80();
    temp_v2 = func_001165A8(temp_v0);
    *(u32 *)(temp_v1 + 0x204) = temp_v2;
    D_003BB694 = 0;
}

s32 func_001EF460(void) {
    s32 context = func_001A17F0();
    if (*(u32 *)(context + 0x57C) != 0) {
        if (*(u32 *)(context + 0x580) != 0) {
            return 1;
        }
    }
    return 0;
}

void func_001EF4A0(void) {
    s32 context = func_001A17F0();
    u32 pointer = *(u32 *)(context + 0x580);
    if (pointer != 0) {
        func_002CF5C0(pointer);
        *(u32 *)(context + 0x580) = 0;
    }
    if (*(u32 *)(context + 0x57C) != 0) {
        func_002CF5C0(*(u32 *)(context + 0x57C));
        *(u32 *)(context + 0x57C) = 0;
    }
}

void func_001EF4F8(void) {
    s64 temp_v0;
    s32 temp_v1;

    func_0021FE70();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_0021FE38();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_001EF4A0();
    temp_v1 = func_001A17F0();
    *(u32 *)(temp_v1 + 500) = *(u32 *)(temp_v1 + 500) & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4AD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF560);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF618);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EF6B8);

s32 func_001EF8B8(s32 index) {
    s32 context = func_001A17F0();
    if (*(u32 *)(context + 0x1F4) & 0x30000000) {
        return *(s32 *)(context + 0x6A0);
    }
    return *(s32 *)(D_003BAA60 + index * 32 + 0x18);
}

u32 func_001EF910(void) {
    u8 *context = (u8 *)func_001A17F0();
    if (*(f32 *)(context + 0x50) != *(f32 *)(context + 0x50) ||
        *(f32 *)(context + 0x54) != *(f32 *)(context + 0x54) ||
        *(f32 *)(context + 0x58) != *(f32 *)(context + 0x58) ||
        *(f32 *)(context + 0x60) != *(f32 *)(context + 0x60) ||
        *(f32 *)(context + 0x64) != *(f32 *)(context + 0x64) ||
        *(f32 *)(context + 0x68) != *(f32 *)(context + 0x68)) {
        return 1;
    }
    return 0;
}

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_0035F5C0;

void func_001EF990(u32 resource, u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = func_00132B78();
    D_0035F5C0.resource = resource;
    D_0035F5C0.handle = handle;
}

void func_001EF9D8(u16 soundId) {
    u32 handle;
    D_0035F5C0.currentId = soundId;
    D_0035F5C0.nextId = soundId;
    handle = func_00132B78();
    D_0035F5C0.handle = handle;
    D_0035F5C0.resource = 0x80;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFA18);

typedef struct SoundTransition {
    u32 currentResource;
    u8 unk_04[0x14];
    u32 previousResource;
    u32 queuedResource;
    u16 soundId;
    u16 queuedId;
} SoundTransition;

void func_001EFAD8(u32 resource, u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = resource;
        transition->queuedResource = resource;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = resource;
}

void func_001EFB18(u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_0035F5D0;
        transition->soundId = 0;
        transition->currentResource = 0;
        transition->queuedResource = 0;
        return;
    }
    transition = (SoundTransition *)D_0035F5D0;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = 0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
}

extern s32 func_001F7BB0(s32, s32, f32);

void func_001EFB58(void) {
    SoundTransition *transition = (SoundTransition *)D_0035F5D0;

    if (transition->soundId != 0) {
        transition->currentResource = func_001F7BB0(transition->queuedResource, transition->previousResource, (f32)transition->soundId / (f32)transition->queuedId);
        transition->soundId--;
    } else {
        transition->currentResource = transition->queuedResource;
    }
    func_001EFA18();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFBD8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFC10);

void func_001EFC30(void) {
    func_001EFB18(0);
    func_00113E40(1);
}

extern f32 *D_00324770[];

void func_001EFC50(void) {
    u8 *context = (u8 *)func_001A17F0();
    f32 *position = D_00324770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        func_00113E40(0);
    } else {
        func_00113E40(1);
    }
    if (*(u32 *)(context + 0x1F8) & 0x20) {
        func_00113E40(0);
    }
    func_001EFB58();
}

void func_001EFCE8(void) {
    s32 temp_v0 = func_001A17F0();

    if ((((*(u32 *)(temp_v0 + 500) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x1f8) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x1fc) & 0x4000000) == 0)) {
        func_00132BD0();
        func_00131F08();
        func_00132010();
    }
    func_001EFBD8();
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFD58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFE08);

void func_001EFF00(void) {
    if (mdlFlagTest(0x32)) {
        return;
    }
    {
        s32 context = func_001A17F0();
        if (*(u32 *)(context + 0x1F8) & 0x20) {
            return;
        }
        if (*(u32 *)(context + 0x58C) != 0) {
            func_0029CF08(*(u32 *)(context + 0x58C));
        }
    }
}

extern char D_003A4B40[]; /* "btl:rain exit\n" */

void func_001EFF58(void) {
    s32 context = func_001A17F0();
    if (*(u32 *)(context + 0x58C) != 0) {
        func_001FB0A8(D_003A4B40);
        func_0029CE80(*(u32 *)(context + 0x58C));
        *(u32 *)(context + 0x58C) = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4B40);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001EFFA8);

extern u32 func_001EFFA8(u32 *);

s32 func_001F0178(u32 soundId, u32 variant) {
    u8 *task = func_001D4748(40);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 1;
    *(void **)(task + 0x4C) = func_001EFFA8;
    task[0x10] = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    memset(arguments, 0, 40);
    arguments[0] = soundId;
    arguments[1] = variant;
    arguments[9] = 0;
    return (s32)task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0218);

extern u32 func_001F0218(u32 *);

u8 *func_001F03A0(u32 soundId, u32 variant) {
    u8 *task = func_001D4748(20);
    u32 *arguments;

    task[0] = 1;
    *(u16 *)(task + 0x24) &= ~1;
    *(u16 *)(task + 0x20) = 2;
    *(void **)(task + 0x4C) = func_001F0218;
    task[0x10] = 0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[2] = soundId;
    arguments[3] = variant;
    arguments[0] = 0;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0430);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0580);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F06E0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0920);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0998);

SoundTask *func_001F09B8(void) {
    SoundTask *task = (SoundTask *)func_001F0920();
    task->taskId = 7;
    task->callback.update = func_001F0998;
    return task;
}

s32 func_001F09F0(u32 *arguments) {
    s32 context = func_001A17F0();
    if ((*(u32 *)(context + 0x1F4) & 0x20000000) == 0) {
        func_001EFAD8(arguments[0], *(u16 *)(arguments + 1));
    }
    D_003BB698++;
    return 1;
}

SoundTask *nbSoundCreateAcquireTask(u32 soundId, u32 flags) {
    SoundTask *task = (SoundTask *)func_001D4748(8);
    u32 *data;
    task->enabled = 1;
    task->taskId = 5;
    task->callback.acquireSound = func_001F09F0;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = soundId;
    data[1] = flags;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0AC8);

SoundTask *nbSoundCreateReleaseTask(sound)
    u32 *sound;

{
    SoundTask *task = (SoundTask *)func_001D4748(4);
    task->enabled = 1;
    task->taskId = 6;
    task->callback.releaseSound = func_001F0AC8;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = (u32)sound;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0B90);

SoundTask *func_001F0BB0(void) {
    SoundTask *task = (SoundTask *)nbSoundCreateReleaseTask();
    task->taskId = 8;
    task->callback.update = func_001F0B90;
    return task;
}

void func_001F0BE8(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

u32 nbSoundGetResourceStatus(u32 *sound) {
    u32 flags;
    if (!sound[1]) {
        return 0;
    }
    flags = sound[0];
    if (flags & 1) {
        return 0xFFFFFFF;
    }
    if (flags & 2) {
        return sound[3];
    }
    return 0;
}

s32 nbSoundLookupResourceType(s32 sound, s32 index) {
    s32 (*lookup)(s32, s32) = *(s32 (**)(s32, s32))(func_001A17F0() + 0x644);
    if (lookup != 0) {
        s32 value = lookup(sound, index);
        if (value != -1) {
            return value;
        }
    }
    return *(u8 *)(D_003BAA60 + index * 0x20 + 3);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F0CA0);

void nbSoundCreateSystemEffect(u32 *effect) {
    s32 handle;
    if (!(effect[0] & 8) || effect[4] || effect[1]) {
        return;
    }
    handle = func_001606C0(effect[5]);
    effect[4] = handle;
    func_001FB0A8("btl:system effect create[%p]\n", handle);
}

void nbSoundDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        func_001FB0A8("btl:system effect delete[%p]\n", effect[4]);
        func_00160800(effect[4]);
        effect[4] = 0;
    }
}

void addSoundEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    task[2] = 0;
    nbSoundCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    ++effect[1];
    ++source[0x314 / 4];
    ++target[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1110);

extern u32 func_001F1110(u32 *);

void releaseSoundEffectReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    u32 *target;

    if (task[2]) {
        func_00160B00(task[2]);
    }
    effect = (u32 *)task[0];
    target = (u32 *)task[6];
    source = (u32 *)task[3];
    --effect[1];
    --source[0x314 / 4];
    --target[0x314 / 4];
    nbSoundDeleteSystemEffect(effect);
}

s32 func_001F12E8(u32 effect, u32 soundId, u8 *owner, u16 variant) {
    u8 *task = func_001D4748(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2B;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = addSoundEffectReferences;
    *(void **)(task + 0x4C) = func_001F1110;
    *(void **)(task + 0x50) = releaseSoundEffectReferences;
    arguments = func_001D47D8((s32)task);
    *(u32 *)arguments = effect;
    *(u32 *)(arguments + 0xC) = soundId;
    *(u32 *)(arguments + 0x10) = soundId;
    *(u32 *)(arguments + 0x14) = soundId;
    *(u8 **)(arguments + 0x18) = owner;
    *(u16 *)(arguments + 4) = variant;
    *(u32 *)(arguments + 8) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return (s32)task;
}

SoundTask *nbSoundCreateEffectWithTargets(s32 sound, s32 flags, u32 *source, u32 *target, s32 mode, u16 variant) {
    SoundTask *task = (SoundTask *)func_001F12E8(sound, flags, mode, variant);
    u32 *data = (u32 *)func_001D47D8((s32)task);
    data[4] = (u32)source;
    data[5] = (u32)target;
    return task;
}

void nbSoundStartEffectTask(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    nbSoundCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1470);

extern u32 func_001F1470(u32 *);

void func_001F15F8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_00160B00(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    nbSoundDeleteSystemEffect(temp_v0);
}

u8 *func_001F1650(u32 effect, u8 *owner, u32 channel) {
    u8 *task = func_001D4748(20);
    u32 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2C;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = nbSoundStartEffectTask;
    *(void **)(task + 0x4C) = func_001F1470;
    *(void **)(task + 0x50) = func_001F15F8;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = effect;
    arguments[2] = (u32)owner;
    arguments[3] = channel;
    arguments[1] = 0;
    arguments[4] = 0;
    return task;
}

void func_001F1710(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

extern s32 func_001F22D0(s32, u16);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1728);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F17C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1888);

extern char D_003A4C88[];

u32 func_001F1910(u32 *args) {
    u8 *effect = (u8 *)args[0];
    s32 resource;

    if (*(u32 *)effect & 2) {
        return 1;
    }
    if (!func_00288BA8(args[1])) {
        return 0;
    }
    func_001FB0A8(D_003A4C88, args[2]);
    resource = func_00288B88(args[1]);
    *(u32 *)(effect + 0x10) =
        func_001606C0(func_002D0A48(resource));
    func_002D0918(resource);
    func_002887A0(args[1]);
    *(u32 *)effect = (*(u32 *)effect & ~1) | 2;
    return 0;
}

extern void func_001F1888(s32);
extern u32 func_001F1910(u32 *);

u8 *func_001F19B8(u32 soundId, const char *filename) {
    u8 *task = func_001D4748(strlen(filename) + 12);
    u8 *arguments;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x2F;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = func_001F1888;
    *(void **)(task + 0x4C) = func_001F1910;
    task[0x10] = 0;
    arguments = func_001D47D8((s32)task);
    name = (char *)(arguments + 12);
    *(u32 *)arguments = soundId;
    *(char **)(arguments + 8) = name;
    strcpy(name, filename);
    return task;
}

extern u32 func_001F1A58(void);
INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1A58);

void *func_001F1AC0(u32 sound) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x30;
    *(void **)(task + 0x4C) = func_001F1A58;
    task[0x10] = 0;
    *(u32 *)func_001D47D8((s32)task) = sound;
    return task;
}

u32 func_001F1B28(u32 *soundId) {
    u8 *object = *(u8 **)(func_001A17F0() + 0x228);
    while (object != 0) {
        u32 flags = *(u32 *)(object + 0x110);
        if (flags & 1) {
            if (flags & 2) {
                if (*(u32 *)(object + 0x320) != 0 &&
                    (flags & 0xE0) == 0) {
                    func_001D6758(object, *(u32 *)(object + 0x54), *soundId);
                }
            }
        }
        object = *(u8 **)(object + 0x344);
    }
    return 1;
}

void *func_001F1BB0(u32 sound) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x31;
    *(void **)(task + 0x4C) = func_001F1B28;
    task[0x10] = 0;
    *(u32 *)func_001D47D8((s32)task) = sound;
    return task;
}

u32 func_001F1C18(void) {
    func_00105888();
    return 1;
}

SoundTask *func_001F1C38(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F1C18;
    task->taskId = 0x32;
    task->status = 0;
    return task;
}

void addSoundSourceReferences(u32 *task) {
    u32 *effect;
    u32 *source;
    task[1] = 0;
    nbSoundCreateSystemEffect((u32 *)task[0]);
    effect = (u32 *)task[0];
    source = (u32 *)task[2];
    ++effect[1];
    ++source[0x314 / 4];
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F1CC8);

extern u32 func_001F1CC8(u32 *);

void func_001F1EC8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_00160B00(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x314) = *(s32 *)(temp_v1 + 0x314) - 1;
    nbSoundDeleteSystemEffect(temp_v0);
}

u8 *func_001F1F20(u32 effect, u8 *owner, u64 resource) {
    u8 *task = func_001D4748(32);
    u8 *arguments;

    task[0] = 1;
    task[0x10] = 0;
    *(u16 *)(task + 0x20) = 0x2D;
    *(u16 *)(task + 0x24) |= 2;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = addSoundSourceReferences;
    *(void **)(task + 0x4C) = func_001F1CC8;
    *(void **)(task + 0x50) = func_001F1EC8;
    arguments = func_001D47D8((s32)task);
    *(u32 *)arguments = effect;
    *(u8 **)(arguments + 8) = owner;
    *(u64 *)(arguments + 0x10) = resource;
    *(u32 *)(arguments + 4) = 0;
    *(u32 *)(arguments + 0x18) = 0;
    *(u32 *)(arguments + 0x1C) = 0;
    return task;
}

u32 func_001F1FE0(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

void *func_001F2000(u32 sound) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x35;
    *(void **)(task + 0x4C) = func_001F1FE0;
    task[0x10] = 0;
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = sound;
    return task;
}

u32 func_001F2068(u8 *arg0) {
    kwlnFadeInStart(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

SoundTask *createCustomSoundTask(u32 soundId, u32 options) {
    SoundTask *task = (SoundTask *)func_001D4748(8);
    u32 *data;
    task->enabled = 1;
    task->taskId = 0x36;
    task->callback.playCustomSound = func_001F2068;
    task->status = 0;
    *(u32 *)((u8 *)task + 0x48) = 0;
    data = (u32 *)func_001D47D8((s32)task);
    data[0] = soundId;
    data[1] = options;
    return task;
}

u32 func_001F2118(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) | 0x40000;
    func_0029B2B0();
    return 1;
}

SoundTask *nbSoundCreateSetBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F2118;
    task->taskId = 0x37;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 func_001F2198(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u32 *)(temp_v0 + 500) = *(u32 *)(temp_v0 + 500) & 0xfffbffff;
    func_0029B2E8();
    return 1;
}

SoundTask *nbSoundCreateClearBattleFlagTask(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F2198;
    task->taskId = 0x38;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4C88);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2218);

void func_001F22B0(s32 arg0, u16 arg1) {
    func_001608B8(*(u32 *)(arg0 + 0x10), arg1);
}

s32 func_001F22D0(s32 arg0, u16 arg1) {
    return func_00160858(*(u32 *)(arg0 + 0x10), arg1);
}

s32 func_001F22F0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) != 0) {
        return 1;
    }
    return *(u32 *)(arg0 + 8) != 0;
}

s32 hasActiveActorSound(void) {
    s32 actor = *(s32 *)(func_001A17F0() + 0x228);
    while (actor != 0) {
        s32 sound = *(s32 *)(actor + 0x2F8);
        if (sound != 0 && func_001F22F0(sound) != 0) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x344);
    }
    return 0;
}

SoundResourceNode *nbSoundAllocResourceNode(void) {
    SoundResourceNode *node = func_002CFF68(sizeof(SoundResourceNode));
    u8 *state;
    SoundResourceNode *first;
    node->unk_04 = 0;
    node->unk_08 = 0;
    node->unk_0C = 0;
    node->resourceHandle = 0;
    state = (u8 *)func_001A17F0();
    node->previous = 0;
    first = *(SoundResourceNode **)(state + 0x234);
    if (first) {
        first->previous = node;
        node->next = *(SoundResourceNode **)(state + 0x234);
    } else {
        node->next = 0;
    }
    *(SoundResourceNode **)(state + 0x234) = node;
    return node;
}

SoundResourceNode *nbSoundCreateResourceNode(u32 soundId) {
    SoundResourceNode *node = (SoundResourceNode *)nbSoundAllocResourceNode();
    node->resourceHandle = func_001606C0(soundId);
    node->flags |= 2;
    return node;
}

void nbSoundFreeResourceNode(SoundResourceNode *node) {
    if (node->resourceHandle) {
        func_00160800(node->resourceHandle);
    }
    if (node->next) {
        node->next->previous = node->previous;
    }
    if (node->previous) {
        node->previous->next = node->next;
    } else {
        *(SoundResourceNode **)(func_001A17F0() + 0x234) = node->next;
    }
    func_002CFF98(node);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F24A8);

void func_001F25C8(void) {
    func_0029B1D8();
}

void func_001F25E0(void) {
    func_00161A18();
    func_00292C40();
    D_003BA904 = D_003BA904 & 0xdfffffff;
}

void func_001F2618(void) {
    nbSoundClearResourceNodes();
    func_0029B320();
    func_00161A18();
    func_00292C40();
}

void nbSoundClearResourceNodes(void) {
    SoundResourceNode *node = *(SoundResourceNode **)(func_001A17F0() + 0x234);
    while (node) {
        SoundResourceNode *next = node->next;
        nbSoundFreeResourceNode(node);
        node = next;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2688);

SoundResourceLink *nbSoundAllocResourceLink(void *owner) {
    SoundResourceLink *node = func_002CFF68(sizeof(SoundResourceLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void nbSoundFreeResourceLink(SoundResourceLink *node) {
    if (node->sound) {
        func_00160B00(node->sound);
        --*(s32 *)((u8 *)node->task + 4);
        nbSoundDeleteSystemEffect(node->task);
    }
    func_002CFF98(node);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2818);

void func_001F2B60(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

SoundLink *nbSoundAllocLink(void *owner) {
    SoundLink *node = func_002CFF68(sizeof(SoundLink));
    node->owner = owner;
    node->sound = 0;
    node->variant = 0;
    node->task = 0;
    return node;
}

void nbSoundFreeLink(SoundLink *node) {
    if (node->sound) {
        func_00160B00(node->sound);
        --*(s32 *)((u8 *)node->task + 4);
        nbSoundDeleteSystemEffect(node->task);
    }
    func_002CFF98(node);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2C00);

u32 func_001F2E20(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u8 *)(temp_v0 + 0x584) = 0;
    return 1;
}

SoundTask *nbSoundCreateClearStateTask(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F2E20;
    task->taskId = 0x33;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

u32 func_001F2E90(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(u8 *)(temp_v0 + 0x584) = 1;
    return 1;
}

SoundTask *nbSoundCreateSetStateTask(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F2E90;
    task->taskId = 0x34;
    *(u32 *)((u8 *)task + 0x48) = 0;
    task->status = 0;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4CD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4CF0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D08);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D20);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D38);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D50);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D68);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D80);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4D98);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DB0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DC8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4DF8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E10);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E28);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E40);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E58);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E70);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4E88);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EA0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4EE0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F00);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F18);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F30);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F48);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F60);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F78);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4F90);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FA8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FC0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FD8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A4FF0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2F00);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F2FE8);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3048);

void func_001F30B8(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = func_001A17F0();
    puVar3 = (u32 *)nbSoundAllocResourceNode();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4b8) = puVar3;
    *puVar3 = temp_v0 | 10;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3118);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3188);

void func_001F3200(u32 arg0) {
    func_002944D8(*(u32 *)arg0);
    func_002CFF98(arg0);
}

void func_001F3230(void) {
}

void func_001F3238(u32 arg0) {
    soundSetSequenceVolumePan(arg0, 0x58, 0x3f);
}

void func_001F3258(u32 arg0) {
    soundSetSequenceVolumePan(arg0, 0x7f, 0x3f);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3278);

u8 func_001F3390(void) {
    s32 temp_v0;

    temp_v0 = func_0026A720();
    return temp_v0 - 2U < 2;
}

void func_001F33B8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if ((*(u32 *)(temp_v0 + 500) & 0x10000) != 0) {
        func_0026A778();
        return;
    }
}

void func_001F33F0(void) {
    advanceTitleStateUnderSemaphore();
}

void func_001F3408(void) {
    advanceTitleStateUnderSemaphore();
    func_002E8E28();
    func_002E8E00();
}

void func_001F3430(void) {
    func_001F33F0();
}

void func_001F3448(void) {
    func_001F3408();
}

s32 func_001F3460(u32 soundId) {
    s32 loaded = func_002E92C0(soundId);
    if (loaded != 0) {
        func_001F3238(soundId);
        return 1;
    }
    return loaded;
}

s32 nbSoundPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (func_001F3460(soundId)) {
        func_001FB0A8("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

SoundTask *nbSoundCreateStationedSeTask(u32 soundId) {
    SoundTask *task = (SoundTask *)func_001D4748(4);
    task->enabled = 1;
    task->taskId = 0x55;
    task->status = 0;
    task->callback.playSound = nbSoundPlayStationedSe;
    *(u32 *)func_001D47D8((s32)task) = soundId;
    return task;
}

s32 func_001F3548(void) {
    ActiveSoundNode *node = *(ActiveSoundNode **)(func_001A17F0() + 0x238);
    while (node != 0) {
        if (node->flags & 8) {
            return 1;
        }
        node = node->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F35A0);

SoundTask *nbSoundCreateSkillSeTask(s32 skill, u16 variant) {
    SoundTask *task = (SoundTask *)func_001D4748(8);
    u8 *data;

    task->enabled = 1;
    task->taskId = 0x52;
    task->status = 0;
    task->callback.playSound = func_001F35A0;
    data = (u8 *)func_001D47D8((s32)task);
    *(u32 *)data = *(u32 *)(skill + 8);
    *(u16 *)(data + 4) = variant;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3710);

extern char D_003A50D8[];
extern char D_003A50F0[];
extern char D_003A5110[];
extern char D_003A5138[];
extern void func_002E9450(s32, s32);

u32 func_001F3778(u32 *arguments) {
    u8 *sound = (u8 *)arguments[0];
    if (nbSoundHasActiveFileLoad()) {
        func_001FB0A8(D_003A50D8);
        return 0;
    }
    if ((*(u32 *)sound & 2) == 0) {
        if (func_00288BA8(arguments[1])) {
            s32 size;
            s32 data;
            func_001FB0A8(D_003A50F0, arguments[4]);
            arguments[2] = func_00288B88(arguments[1]);
            size = func_00288B98(arguments[1]);
            data = func_002D0A48(arguments[2]);
            if (func_002E92C0(*(u32 *)(sound + 8)) == 0) {
                func_002E9450(data, size);
                *(u32 *)sound |= 8;
                func_001FB0A8(D_003A5110, *(u16 *)(sound + 0xA), size);
            }
            *(u32 *)sound = (*(u32 *)sound & ~1) | 2;
        }
    } else if (func_002E92C0(*(u32 *)(sound + 8)) != 0) {
        func_001FB0A8(D_003A5138, *(u16 *)(sound + 0xA));
        func_002D0918(arguments[2]);
        func_002887A0(arguments[1]);
        *(u32 *)sound = (*(u32 *)sound & ~8) | 0x10;
        return 1;
    }
    return 0;
}

extern void func_001F3710(s32);

u8 *func_001F38C0(u32 soundId, u32 variant, const char *filename) {
    u8 *task = func_001D4748(strlen(filename) + 20);
    u8 *arguments;
    char *name;

    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x53;
    *(u16 *)(task + 0x24) &= ~1;
    *(void **)(task + 0x48) = func_001F3710;
    *(void **)(task + 0x4C) = func_001F3778;
    task[0x10] = 0;
    arguments = func_001D47D8((s32)task);
    name = (char *)(arguments + 20);
    *(u32 *)arguments = soundId;
    *(u32 *)(arguments + 12) = variant;
    *(char **)(arguments + 16) = name;
    strcpy(name, filename);
    return task;
}

s32 nbSoundLoadDataFile(s32 *data) {
    char filename[0x70];
    if (func_001F3A30()) {
        return 1;
    }
    func_001F3AA8(data[0], (s32)filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

void *func_001F39C0(u8 *owner) {
    u8 *task = func_001D4748(4);
    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = nbSoundLoadDataFile;
    *(u16 *)(task + 0x20) = 0x56;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(u32 *)(task + 0x48) = 0;
    *(u32 *)func_001D47D8((s32)task) = (u32)owner;
    return task;
}

s32 func_001F3A30(void) {
    return (s8)func_002E97E0();
}

s32 func_001F3A58(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void func_001F3A78(s32 arg0, s32 arg1) {
    func_003014F0(arg1, D_003A5158, D_003BB6B0, (arg0 + 0x200) & 0xffff);
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A50D8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A50F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5110);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5138);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5158);

void func_001F3AA8(s32 arg0, s32 arg1) {
    func_003014F0(arg1, "MDD_%03X.ADB", *(u16 *)(arg0 + 0x124));
}

s32 func_001F3AD0(s32 sound, s32 index) {
    s32 type = nbSoundLookupResourceType(sound, index);
    if (type < 0x1A && type != 0) {
        if (type >= 0xB) {
            return type + *(s32 *)(func_001A17F0() + 0x1E4) - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

ActiveSoundNode *nbSoundAllocListNode(void) {
    ActiveSoundNode *node = func_002CFF68(0x14);
    u8 *state = (u8 *)func_001A17F0();
    ActiveSoundNode *first;

    node->previous = 0;
    first = *(ActiveSoundNode **)(state + 0x238);
    if (first != 0) {
        first->previous = node;
        node->next = *(ActiveSoundNode **)(state + 0x238);
    } else {
        node->next = 0;
    }
    *(ActiveSoundNode **)(state + 0x238) = node;
    return node;
}

void nbSoundFreeListNode(ActiveSoundNode *node) {
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        *(ActiveSoundNode **)(func_001A17F0() + 0x238) = node->next;
    }
    func_002CFF98(node);
}

void nbSoundClearList(void) {
    ActiveSoundNode *node = *(ActiveSoundNode **)(func_001A17F0() + 0x238);
    while (node != 0) {
        ActiveSoundNode *next = node->next;
        nbSoundFreeListNode(node);
        node = next;
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F3C38);

extern char D_003A5178[];
extern char D_003A5188[];
extern char D_003A5198[];
extern char D_003A51A8[];
extern u32 func_001F3C38(u32 *, u32);

void func_001F3CC8(u32 *sound) {
    char filename[0x70];
    u32 slot = 0;
    s32 offset = 0x10;
    u8 *handleTable = (u8 *)sound + 8;
    do {
        u32 id = func_001F3C38(sound, slot);
        if (id != 0) {
            if (slot != 0xB) {
                func_003014F0(filename, D_003A5158, D_003BB6B0, id >> 16);
            } else if (sound[1] == 0) {
                func_003014F0(filename, D_003A5178, D_003A5188, sound[2]);
            } else {
                func_003014F0(filename, D_003A5198, D_003A5188, sound[2]);
            }
            *(u32 *)(handleTable + offset) = func_00288B48(filename);
            func_001FB0A8(D_003A51A8, slot, sound, filename);
        }
        slot++;
        offset += 4;
    } while (slot < 0x1D);
    sound[0] |= 1;
}

s32 findSoundListNodeForChannel(s32 soundId, s32 channel) {
    s32 context = func_001A17F0();
    s32 node = *(s32 *)(context + 0x23C);
    while (node != 0) {
        if (*(s32 *)(node + 4) == soundId && *(s32 *)(node + 8) == channel) {
            return node;
        }
        node = *(s32 *)(node + 0x104);
    }
    return 0;
}

extern char D_003A51D0[];

u8 *func_001F3E70(s32 soundId, s32 channel) {
    u8 *node = (u8 *)findSoundListNodeForChannel(soundId, channel);
    u8 *context;
    u8 *head;

    if (node != 0) {
        func_001FB0A8(D_003A51D0, node);
        (*(u32 *)(node + 0xC))++;
        return node;
    }
    node = func_002CFF68(0x108);
    *(s32 *)(node + 4) = soundId;
    *(s32 *)(node + 8) = channel;
    *(u32 *)(node + 0xC) = 1;
    context = (u8 *)func_001A17F0();
    *(u8 **)(node + 0x100) = 0;
    head = *(u8 **)(context + 0x23C);
    if (head != 0) {
        *(u8 **)(head + 0x100) = node;
        *(u8 **)(node + 0x104) = *(u8 **)(context + 0x23C);
    } else {
        *(u8 **)(node + 0x104) = 0;
    }
    *(u8 **)(context + 0x23C) = node;
    if (mdlFlagTest(0xC0F) == 0) {
        func_001F3CC8(node);
    }
    return node;
}

void func_001F3F40(u8 *node) {
    u32 count = *(u32 *)(node + 0xC) - 1;
    *(u32 *)(node + 0xC) = count;
    if (count == 0) {
        u32 i = 0;
        u32 *resources = (u32 *)(node + 0x8C);
        u32 *handles = (u32 *)(node + 0x18);
        for (; i < 0x1D; i++, handles++, resources++) {
            if (*handles != 0) {
                func_002887A0(*handles);
            }
            if (*resources != 0) {
                func_002D0918(*resources);
            }
        }
        if (*(u8 **)(node + 0x104) != 0) {
            *(u8 **)(*(u8 **)(node + 0x104) + 0x100) =
                *(u8 **)(node + 0x100);
        }
        if (*(u8 **)(node + 0x100) != 0) {
            *(u8 **)(*(u8 **)(node + 0x100) + 0x104) =
                *(u8 **)(node + 0x104);
        } else {
            *(u8 **)(func_001A17F0() + 0x23C) =
                *(u8 **)(node + 0x104);
        }
        func_002CFF98(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4038);

void func_001F4078(void) {
    s32 task = func_001F4398();
    func_001D4860(task);
}

s32 nbSoundHasActiveFileLoad(void) {
    u8 *node = *(u8 **)(func_001A17F0() + 0x23C);
    while (node) {
        if (*(u32 *)node & 8) {
            return 1;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F40F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5178);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5188);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5198);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A51A8);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A51D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F41C0);

extern void func_001F40F0(s32);
extern u32 func_001F41C0(u32 *);

s32 func_001F4398(u8 *owner, u32 soundId) {
    u8 *task = func_001D4748(16);
    u32 *arguments;

    task[0x10] = 0;
    task[0] = 1;
    *(u16 *)(task + 0x20) = 0x54;
    *(u64 *)(task + 0x40) = *(u64 *)(owner + 0x108);
    *(void **)(task + 0x48) = func_001F40F0;
    *(void **)(task + 0x4C) = func_001F41C0;
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[2] = soundId;
    arguments[1] = 0;
    arguments[3] = 0;
    return (s32)task;
}

void func_001F4430(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    *(s32 *)(temp_v0 + 0x264) = -1;
    *(s32 *)(temp_v0 + 0x268) = -1;
}

void loadBattleSoundBank(void) {
    if (func_002E92C0(0x10000) == 0) {
        func_002E9340(0x10000);
        func_001FB0A8("btl:sound load BSE SMG\n");
    }
}

u8 func_001F44A0(void) {
    s64 temp_v0;

    temp_v0 = func_002E92C0(0x10000);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F44C0);

s32 hasOccupiedSoundNodeSlots(void) {
    s32 context = func_001A17F0();
    s32 node = *(s32 *)(context + 0x23C);
    while (node != 0) {
        if ((*(u32 *)node & 2) == 0) {
            u32 index = 0;
            u32 *slot = (u32 *)(node + 0x18);
            for (; index < 0x1D; index++) {
                if (*slot != 0) {
                    return 1;
                }
                slot++;
            }
        }
        node = *(s32 *)(node + 0x104);
    }
    return 0;
}

extern s32 func_0026AD28(void);

extern void printTitleDebugBanner(void);

u32 finishEarringPlayback(void) {
    s32 status = func_0026AD28();
    if (status == 0) {
        return 1;
    }
    if (status == 2) {
        func_003003F0("%%%%%%%%%%%%%%%% EARRING(2)\n");
        printTitleDebugBanner();
    }
    return 0;
}

SoundTask *nbSoundCreateEarringTask(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = finishEarringPlayback;
    task->taskId = 0x57;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4828);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4950);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F49D0);

extern char D_003A5390[];
extern char D_003A53B0[];
extern char D_003A53D0[];

u32 func_001F4AA8(u32 *args) {
    u32 resource;
    u32 data;
    u32 size;

    if (args[1] == 0) {
        return 1;
    }
    if (args[2] == 0) {
        if (func_00288BA8(args[1]) != 0) {
            resource = func_00288B88(args[1]);
            args[2] = resource;
            data = func_002D0A48(resource);
            size = func_00288B98(args[1]);
            func_002887A0(args[1]);
            func_0026ABA8(data, size, 2);
            printTitleDebugBanner();
            func_003003F0(D_003A5390);
            func_001FB0A8(D_003A53B0);
        }
        return 0;
    }
    if (func_0026AD28() == 0) {
        func_001FB0A8(D_003A53D0);
        return 1;
    }
    return 0;
}

void nbSoundFinishEarringPlayback(u32 *sound) {
    u8 *state = (u8 *)func_001A17F0();
    if (sound[2]) {
        func_002D0918(sound[2]);
    }
    --*(u16 *)(state + 0x260);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4BF0);

u32 func_001F4C90(void) {
    func_001F3460(0x1c);
    return 1;
}

SoundTask *func_001F4CB0(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F4C90;
    task->taskId = 0x5A;
    task->status = 0;
    return task;
}

u32 func_001F4CF0(void) {
    func_001F33F0();
    return 1;
}

SoundTask *func_001F4D10(void) {
    SoundTask *task = (SoundTask *)func_001D4748(0);
    task->enabled = 1;
    task->callback.process = func_001F4CF0;
    task->taskId = 0x5B;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F4D50);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5028);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F53C0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F53F0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5410);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F55D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F57D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5A20);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5B50);

void func_001F5D00(void) {
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5D08);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5390);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A53B0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A53D0);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_001F5ED8);

extern u32 func_001F5ED8(u32 *);

u8 *func_001F6030(u8 *owner, u32 soundId, u32 variant, u32 channel, u32 flags) {
    u8 *task = func_001D4748(20);
    u32 *arguments;
    u8 *sound;

    task[0] = 1;
    task[0x10] = 0;
    *(void **)(task + 0x4C) = func_001F5ED8;
    sound = *(u8 **)(owner + 0x18);
    *(u16 *)(task + 0x20) = 0x5F;
    *(u32 *)(task + 0x48) = 0;
    *(u64 *)(task + 0x40) = *(u64 *)(sound + 0x108);
    arguments = (u32 *)func_001D47D8((s32)task);
    arguments[0] = (u32)owner;
    arguments[1] = soundId;
    arguments[2] = variant;
    arguments[3] = channel;
    arguments[4] = flags;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A5410);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB230);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB238);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB240);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB244);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB248);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB250);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB258);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB260);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB268);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB270);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB278);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB280);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB288);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB290);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB298);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2E4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB300);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB308);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB310);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB318);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB320);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB328);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB330);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB338);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB340);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB348);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB350);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB358);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB360);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB368);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB370);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB378);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB380);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB388);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB390);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB398);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3AC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3BC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3CC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3D4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3DC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E4);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E5);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB3F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB400);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB408);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB410);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB418);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB420);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB428);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB430);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB438);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB440);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB448);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB450);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB458);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB460);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB468);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB470);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB478);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB480);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB488);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB494);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB496);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB498);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB4F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB500);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB508);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB510);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB518);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB520);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB528);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB530);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB538);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB540);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB548);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB550);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB558);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB560);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB568);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB570);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB578);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB580);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB588);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB590);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB598);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5B0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5B8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5C0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5C8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5D0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5D8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5E0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5E8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5EC);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5F0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB5F8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB600);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB608);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB610);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB618);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB620);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB628);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB630);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB638);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB640);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB648);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB650);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB658);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB660);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB664);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB668);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB670);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB678);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB680);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB690);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB694);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB698);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB6B0);

