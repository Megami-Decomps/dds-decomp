#include "common.h"

extern s64 func_002A2928(void);

extern u64 func_00205018(void);

extern u32 D_00436AD4;

extern u64 func_0010FCA8(void);

extern u32 func_00116810(u64);

extern s32 func_001E3168(void);

extern s32 mdlFlagTest(u32);

extern s32 func_0022F180(void);

extern u64 func_001D4160(u64, u64, u64);

extern s32 func_001AA6F8(void);

extern u32 D_00436A1C;

extern u32 D_00436A20;

extern s32 func_0032CD98(void);

extern u32 D_00435CD4;

extern s32 func_002A2330(void);

extern s32 func_00342168(u32);

typedef struct SoundTask {
    u8 enabled;
    u8 unk_01[0xF];
    u8 status;
    u8 unk_11[0xF];
    u16 taskId;
    u8 unk_22[0x1E];
    u64 owner;
    u32 unk_48;
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*command)(s32);
    } callback;
} SoundTask;

extern void func_00201520(void);

extern void func_00201718(void);

extern s32 func_002041E8(u32 *);

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

extern SoundResourceNode *nbSoundAllocResourceNode(void);

extern void func_002046F0(s32, s32);

extern s32 D_00435E20;

extern s32 D_00435E30;

extern void *func_00328E18(s32);

typedef struct ActiveSoundNode {
    u32 flags;
    u8 unk_04[8];
    struct ActiveSoundNode *previous;
    struct ActiveSoundNode *next;
} ActiveSoundNode;

extern void func_001E8018();

extern s32 countBattleTasksForOwner(s64);

extern void func_001E15F0(s32);

extern s32 func_0020D128(const char *, ...);

extern s32 func_00232EE8(s32);

extern s32 func_00232EF8(s32);

extern f32 func_00208000(s32, s32, s32);

extern void func_0035C860(s32, const void *, const void *, u16);

extern char D_004192D8[];

extern char D_00436AE8[];

extern u32 func_001DFD58(void *);

extern void func_001E2C00(u8 *, s32, s32, f32);

extern void func_001E9660(u8 *, f32, f32, f32, f32, f32, f32, f32, f32);

extern void func_001E95D0(u8 *, f32 *, f32 *);

typedef struct SoundCommand {
    u32 handle;
    u32 resource;
    u16 currentId;
    u16 nextId;
} SoundCommand;

extern SoundCommand D_003BDC90;

extern u8 D_003BDCA0[];

typedef struct SoundTransition {
    u32 currentResource;
    u8 unk_04[0x14];
    u32 previousResource;
    u32 queuedResource;
    u16 soundId;
    u16 queuedId;
} SoundTransition;

extern char D_00418CC0[]; /* "btl:rain exit\n" */

extern s32 func_00201578(u32 *);

extern u32 D_00436AD8;

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DACF8);

void func_001DB048(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB050);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB258);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB440);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB518);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB5E0);

void func_001DBE20(s32 *arguments) {
    s32 owner = arguments[0x34 / 4];
    s32 value = func_00210360(owner, arguments[0x20 / 4]);
    func_001E1580(value);
    value = func_001E6428(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    func_001E1580(value);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DBE70);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC278);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC2D8);

void func_001DC538(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC540);

void func_001DC7F0(void) {
}

void func_001DC7F8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = func_001D4160(arg0, 0x1194, 1);
    func_001E1580(temp_v0);
    func_001DCF18(arg0, 0x1b);
}

void func_001DC838(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC840);

void func_001DC888(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC890);

void func_001DC8F8(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC900);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC9C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCC48);

void func_001DCD80(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCD88);

void func_001DCE58(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) = *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x110) | 0x4000
    ;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCE70);

void func_001DCEB0(void) {
}

void func_001DCEB8(void) {
}

void func_001DCEC0(void) {
    func_0022F068();
}

void func_001DCED8(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_0022F180();
    if (temp_v0 == 0) {
        func_001DCF18(arg0, 6);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCF18);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417508);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417518);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417528);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417538);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417548);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCF58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCFE0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD060);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD108);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD148);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD1A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD390);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD5E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD628);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD6D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD810);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD9A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DDAD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DDB60);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DF700);

typedef struct {
    u8 unk00[0x40];
    void *indices;
    u32 unk44;
    u64 previous;
    u8 unk50[0x18];
    u32 device;
    u32 command;
} BattleIndexWork;

extern void *allocateBattleIndexList(s32);

extern u32 func_003292A8(s32);

extern u32 func_003298F8(u32);

extern void func_001DF700(BattleIndexWork *);

void func_001DF7B8(BattleIndexWork *work) {
    u32 command;
    work->indices = allocateBattleIndexList(13);
    command = func_003292A8(0x48EC);
    work->device = func_003298F8(command);
    work->command = command;
    work->previous = 0;
    func_001DF700(work);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DF810);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DF860);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFB08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFBE0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFC80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFD58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFDC0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFE48);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFF48);

s32 func_001DFFE0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0;
    func_001ADFE0(*(s32 *)temp_v0, *(s32 *)(temp_v0 + 0x14), *(s16 *)(temp_v0 + 0x18));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0010);

u32 func_001E00E8(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        func_001AA880(*arg0, arg0[7]);
        func_001E2758(*arg0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0128);

u32 func_001E0200(u32 *arg0) {
    func_001AA888(*arg0);
    func_001E2758(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0238);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E02A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E05B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0640);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E06B0);

u32 func_001E0738(s32 arg0) {
    func_0011A118(*(u16 *)(arg0 + 4), 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0760);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E07E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0858);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E08E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0950);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E09D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0B08);

u32 func_001E0B50(s32 arg0) {
    func_0011A0D0(*(u32 *)(arg0 + 4));
    return 1;
}

typedef struct {
    void *actor;
    s32 option;
} SoundTaskArgs;

extern SoundTask *func_001E1468(s32);

extern SoundTaskArgs *func_001E14F8(s32);

SoundTask *func_001E0B70(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x56;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E0B50;
    task->unk_48 = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0BF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0CE0);

void func_001E1100(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1108);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1168);

INCLUDE_ASM(const s32, "game/code_001DACF8", findBattleTaskByKind);

s32 func_001E1228(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001AA6F8();
    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(temp_v0 + 0x254); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", countBattleTasksForOwner);

INCLUDE_ASM(const s32, "game/code_001DACF8", countBattleTasksByKind);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1318);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1368);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1468);

SoundTaskArgs *func_001E14F8(s32 arg0) {
    return *(SoundTaskArgs **)(arg0 + 0x54);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1500);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1580);

void func_001E15E0(void) {
    D_00436A1C = 0;
    D_00436A20 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E15F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1730);

void clearDeferredBattleTasks(void) {
    s32 node = D_00436A20;
    while (node != 0) {
        s32 next = *(s32 *)(node + 0x60);
        func_001E15F0(node);
        node = next;
    }
    D_00436A1C = 0;
    D_00436A20 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1800);

u32 func_001E1848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1850);

INCLUDE_ASM(const s32, "game/code_001DACF8", dumpBattleTaskQueue);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1918);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1958);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E19C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", hasMatchingBattleModel);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1B80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1BB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1F98);

void func_001E2058(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_0022CD60(arg1, arg2);
        return;
    }
    func_00231B80(arg1, arg2, 0);
}

void func_001E20B8(u32 arg0, u32 arg1, u32 arg2) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0xc0f);
    if (temp_v0 != 0) {
        func_0022CB68(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2110);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E21A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2220);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2298);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417940);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E22D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2758);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2B60);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2C00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2C78);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2CD8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2DA0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2E20);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2E58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2EE8);

void func_001E2F18(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_003343E8(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x340) + 0x8c) + 0x1c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2F50);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2F78);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2FB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3040);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3088);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3108);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3120);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3168);

void func_001E31F0(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001E3168();
    if (temp_v0 == 0) {
        func_00207DC0(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3230);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3320);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E33A8);

s32 func_001E33E0(u8 *object) {
    f32 position[3];
    func_001E3108(object, position);
    if (*(f32 *)(object + 0x30) == position[0] &&
        *(f32 *)(object + 0x34) == position[1] &&
        *(f32 *)(object + 0x38) == position[2]) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3448);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E34D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E34F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3560);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E35F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3628);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E36A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3720);

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

extern s32 getEntryFlagsUnlessDisabled(const void *);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004179E0);

s32 func_001E37A8(BattleEntryHeader *entry) {
    if (!(entry->flags & 0x400)) {
        return 0;
    }
    switch (entry->objectId) {
    case 0x109: case 0x10A: case 0x110: case 0x111: case 0x112:
    case 0x119: case 0x11D: case 0x11E: case 0x11F: case 0x120:
    case 0x121: case 0x127: case 0x12E: case 0x12F: case 0x131:
    case 0x132: case 0x133: case 0x134: case 0x135: case 0x136:
        return 2;
    default:
        return (getEntryFlagsUnlessDisabled((u8 *)entry + 0x120) >> 14) & 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3810);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E38F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3C28);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3CB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3E20);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4028);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E40F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4378);

u32 func_001E44A8(u8 *arguments) {
    s32 index = *(s32 *)(arguments + 4);
    if (index >= 0) {
        func_001E2C00(*(u8 **)arguments, index, *(s32 *)(arguments + 8),
                        *(f32 *)(arguments + 0xC));
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E44E0);

u32 func_001E4588(u32 *arg0) {
    func_001E2DA0(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E45A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4618);

INCLUDE_ASM(const s32, "game/code_001DACF8", scheduleBattleThresholdTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4700);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4908);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E49A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4A90);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4B40);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4C30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4CF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4DF0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4EE0);

u32 func_001E4FA0(u32 *arg0) {
    func_001E2220(*arg0);
    func_001E1F98(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", scheduleBattleRefreshTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", beginBattleModelChange);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E50E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5790);

void func_001E5870(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        func_0023CB58(*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x340));
        return;
    }
}

u32 func_001E58A0(u32 *arg0) {
    if ((*(u64 *)(arg0[3] + 0x110) & 0x1000000002) == 0x1000000002) {
        func_0023C870(*(u32 *)(arg0[3] + 0x340), arg0[2], *arg0, arg0[1]);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E58F0);

void func_001E59A0(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x110) & 2) != 0) {
        func_0023CB58(*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x340));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E59D0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5A10);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5AB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5C08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5CB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5DA8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5E40);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5FF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6080);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E61A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6228);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6428);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E64B0);

void func_001E65C0(u32 *arguments) {
    u32 value = arguments[2];
    if (value != 0) {
        func_001686F0(value);
    }
    if (arguments[1] != 0) {
        func_001683F0(arguments[1]);
    }
    func_001E2220(arguments[0]);
    *(u32 *)(arguments[0] + 0x110) |= 0x40;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6620);

u32 func_001E66B8(void) {
    func_00208F78();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E66D8);

u32 func_001E6720(void) {
    func_00209078();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6740);

u32 func_001E6790(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6798);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E67E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E68F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6970);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6B70);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6BF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6E18);

u32 func_001E6E90(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *(u32 *)(temp_v0 + 0x110) = *(u32 *)(temp_v0 + 0x110) & 0xffffffef;
    func_001E3448(temp_v0, temp_v0 + 0x40);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", scheduleBattleActorUpdate);

u32 func_001E6F38(u32 *arg0) {
    func_001E3628(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6F58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6FC8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7068);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E70F8);

u32 func_001E7188(void) {
    func_001E2CD8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E71A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E71F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7248);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E72B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7378);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7438);

extern u32 func_001E7438(s32);

SoundTask *func_001E74A0(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x27;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E7438;
    task->unk_48 = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 func_001E7528(u32 *arg0) {
    func_001E21A0(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7548);

u32 func_001E75B8(u32 *arg0) {
    func_001E2220(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E75D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7648);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7960);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7B00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7B58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7C48);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7D30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7DB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", removeBattleActorsWithFlags);

INCLUDE_ASM(const s32, "game/code_001DACF8", findBattleActorForOwner);

INCLUDE_ASM(const s32, "game/code_001DACF8", isActiveBattleActor);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7F08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7F70);

void *allocateBattleIndexList(s32 capacity) {
    u8 *list = func_00328E18(capacity * 4 + 12);
    *(s32 *)list = capacity;
    *(u32 **)(list + 8) = (u32 *)(list + 12);
    *(s32 *)(list + 4) = 0;
    return list;
}

void func_001E8018(void) {
    func_00328E48();
}

void func_001E8030(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 4);
    *(s32 *)(arg0 + 4) = temp_v0 + 1;
    *(u32 *)(temp_v0 * 4 + *(s32 *)(arg0 + 8)) = arg1;
}

void func_001E8050(s32 arg0) {
    *(u32 *)(arg0 + 4) = 0;
}

u32 func_001E8058(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001E8060(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + *(s32 *)(arg0 + 8));
}

void copyBattleIndexList(s32 destination, s32 source) {
    u32 count;
    u32 index;

    func_001E8050(destination);
    count = func_001E8058(source);
    for (index = 0; index < count; index++) {
        func_001E8030(destination, func_001E8060(source, index));
    }
}

void func_001E80F8(s32 arg0, s32 arg1, s32 arg2) {
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

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8128);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E81A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8258);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8428);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8510);

u32 func_001E8568(void) {
    return 1;
}

u32 func_001E8570(void) {
    return 1;
}

u32 func_001E8578(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8580);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8650);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8708);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E87A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8840);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E88A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E89E0);

u32 func_001E8B08(u32 *arg0) {
    func_001E8258(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8B40);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8BE0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8C20);

u32 func_001E8C88(u8 *arguments) {
    u8 *context = (u8 *)func_001AA6F8();
    func_001E8258(1, *(u32 *)arguments, 0, 0, 0);
    func_001E9660(context + 0x70, *(f32 *)(arguments + 4), *(f32 *)(arguments + 8),
                    *(f32 *)(arguments + 0xC), *(f32 *)(arguments + 0x10),
                    *(f32 *)(arguments + 0x14), *(f32 *)(arguments + 0x18),
                    *(f32 *)(arguments + 0x1C), *(f32 *)(arguments + 0x20));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8D00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8E08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8EC0);

u32 func_001E9008(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001E8E08(arg0);
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x80000;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9058);

u32 func_001E90C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001E9B80(temp_v0 + 0x70);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", scheduleBattleContextReset);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9130);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9410);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9548);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9598);

void func_001E95C8(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E95D0);

void func_001E9660(u8 *object, f32 x, f32 y, f32 z, f32 vx, f32 vy,
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
    func_001E95D0(object, origin, direction);
    *(f32 *)(object + 0x24) = scale * 0.017453293f;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E96C8);

u32 func_001E9798(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return *(u32 *)(temp_v0 + 0x194);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E97B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E97C0);

void func_001E9860(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x184) = 0;
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x400;
    func_001E8050(*(u32 *)(temp_v0 + 0x1a8));
}

void func_001E9890(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) & 0xffffdfff;
}

void func_001E98C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x180) = *(u32 *)(temp_v0 + 0x180) | 0x2000;
}

u32 func_001E98E8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return ((*(s32 *)(temp_v0 + 0x180) >> 0xd) ^ 1U) & 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9918);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E99C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9A18);

s32 func_001E9A68(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return temp_v0 + 0x70;
}

void func_001E9A88(void) {
    func_00208D58();
}

void func_001E9AA0(void) {
    func_00208DA0();
}

void func_001E9AB8(s32 arg0) {
    func_00208DE8(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x114) + 0x18) + 0x110) & 0x600);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9AE0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C78);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C88);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C98);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417CA8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417CB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9B80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9CD8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9DD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9F30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA058);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA120);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA190);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA210);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA2B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA338);

s32 func_001EA3B8(void) {
    if (func_00208000(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA3F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA4D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA598);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA620);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA650);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA688);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA6F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA748);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA7C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA800);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA830);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA8B0);

u8 func_001EA940(s32 arg0) {
    return *(s32 *)(arg0 + 0x134) == 0x5f;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA950);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA9D8);

u32 func_001EAA00(void) {
    return 0;
}

u8 func_001EAA08(s32 arg0) {
    return *(s32 *)(arg0 + 0x134) == 0x1a0;
}

void func_001EAA18(void) {
}

void func_001EAA20(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EAA28);

void func_001EAC08(void) {
}

void func_001EAC10(u32 arg0) {
    func_001ECBF8(arg0, arg0);
}

void func_001EAC28(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EAC30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EADC0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EAE88);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EB490);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EB5B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EBB88);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EBD40);

void func_001EBE28(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EBE30);

void func_001EC190(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC198);

void func_001EC2A0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC2A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC3E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC418);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC5F0);

void func_001EC620(u32 arg0) {
    func_001F5018(arg0, arg0);
}

void func_001EC638(u32 arg0) {
    func_001F5230(arg0, arg0);
}

void func_001EC650(u32 arg0) {
    func_001F2758(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

void func_001EC670(void) {
    func_001F2AE8();
}

void func_001EC688(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    func_001E8030(*(u32 *)(temp_v0 + 0x138), *(u32 *)(*(s32 *)(temp_v0 + 0x114) + 0x18));
    func_001F2E30(arg0, temp_v0 + 0x30, temp_v0 + 0xc0);
}

void func_001EC6C8(void) {
}

void func_001EC6D0(u32 arg0) {
    if ((*(u32 *)(*(s32 *)(*(s32 *)((s32)arg0 + 0x114) + 0x18) + 0x110) & 0x200) != 0) {
        func_001F25F8(arg0, arg0);
        return;
    }
    if (*(s32 *)((s32)arg0 + 0x128) != 0x10) {
        func_001F2740(arg0, arg0);
        return;
    }
}

void func_001EC728(void) {
}

void func_001EC730(s32 arg0) {
    if (*(s32 *)(arg0 + 0x114) != 0) {
        func_001FFD30(*(s32 *)(arg0 + 0x114));
        return;
    }
}

void func_001EC760(void) {
}

void func_001EC768(u32 arg0) {
    func_001F35C8(arg0, (s32)arg0 + 0x30, (s32)arg0 + 0xc0);
}

typedef struct {
    u8 unk00[0x674];
    s32 (*allowDefaultSound)(void *);
} SoundEventCallbacks;

extern void func_001F3888(void *, void *, void *);

void func_001EC788(void *actor) {
    SoundEventCallbacks *callbacks = (SoundEventCallbacks *)func_001AA6F8();
    if (callbacks->allowDefaultSound && callbacks->allowDefaultSound(actor)) {
        return;
    }
    func_001F3888(actor, (u8 *)actor + 0x30, (u8 *)actor + 0xC0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC7E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC828);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC868);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ECBF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ECC18);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ECCB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED008);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED300);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED380);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED610);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED6C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED9A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EDAF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EDC38);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EDFB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EE458);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EE690);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EEB78);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EF030);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EF668);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EF948);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EFA30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EFEE8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F01E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F02E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F0508);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F0690);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F0968);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F0C80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F1120);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F1290);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F17C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F1B00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F1F20);

void func_001F20B0(void) {
    func_001ECC18();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F20C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2308);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F25F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2740);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2E30);

void func_001F3228(u32 arg0) {
    func_001F01E8(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F3240);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F34C8);

void func_001F34E0(void) {
    func_001F3240();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F34F8);

void func_001F35C0(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F35C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F3888);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F3C30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F3E48);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F41F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F4D70);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417D70);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417E30);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417EF0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417F30);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004180B0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004180C0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418240);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418250);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418310);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418320);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418330);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418338);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418398);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004183D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F4E30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F4F10);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5018);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5230);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F52D0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5320);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5780);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5810);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004184D8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418568);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5868);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FA480);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FB908);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FC5E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FD400);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDCA8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDD20);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDE60);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDF18);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FE068);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FE5C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FEC00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF0F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF570);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF5D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF820);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF8F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF930);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFA08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFAD8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFBB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFC70);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFD30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFDD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFF68);

void func_001FFFF8(void) {
    u64 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    temp_v1 = func_001AA6F8();
    temp_v0 = func_0010FCA8();
    temp_v2 = func_00116810(temp_v0);
    *(u32 *)(temp_v1 + 0x228) = temp_v2;
    D_00436AD4 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200038);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200078);

void func_002000D0(void) {
    s64 temp_v0;
    s32 temp_v1;

    func_0023A9E0();
    do {
        temp_v0 = func_0032CD98();
    } while (temp_v0 != 0);
    func_0023A9A8();
    do {
        temp_v0 = func_0032CD98();
    } while (temp_v0 != 0);
    func_00200078();
    temp_v1 = func_001AA6F8();
    *(u32 *)(temp_v1 + 0x218) = *(u32 *)(temp_v1 + 0x218) & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418C58);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200138);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002001F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200290);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200490);

u32 func_002004E8(void) {
    u8 *context = (u8 *)func_001AA6F8();
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

void func_00200568(u32 resource, u16 soundId) {
    u32 handle;
    D_003BDC90.currentId = soundId;
    D_003BDC90.nextId = soundId;
    handle = func_00135580();
    D_003BDC90.resource = resource;
    D_003BDC90.handle = handle;
}

void func_002005B0(u16 soundId) {
    u32 handle;
    D_003BDC90.currentId = soundId;
    D_003BDC90.nextId = soundId;
    handle = func_00135580();
    D_003BDC90.handle = handle;
    D_003BDC90.resource = 0x80;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002005F0);

void func_002006B0(u32 resource, u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_003BDCA0;
        transition->soundId = 0;
        transition->currentResource = resource;
        transition->queuedResource = resource;
        return;
    }
    transition = (SoundTransition *)D_003BDCA0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = resource;
}

void func_002006F0(u16 soundId) {
    SoundTransition *transition;
    if (soundId == 0) {
        transition = (SoundTransition *)D_003BDCA0;
        transition->soundId = 0;
        transition->currentResource = 0;
        transition->queuedResource = 0;
        return;
    }
    transition = (SoundTransition *)D_003BDCA0;
    transition->previousResource = transition->currentResource;
    transition->queuedResource = 0;
    transition->soundId = soundId;
    transition->queuedId = soundId;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200730);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002007B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002007E8);

void func_00200808(void) {
    func_002006F0(0);
    func_00114068(1);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200828);

void func_002008C0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((((*(u32 *)(temp_v0 + 0x218) & 0x20000) != 0) && ((*(u32 *)(temp_v0 + 0x21c) & 0x20) == 0)) &&
          ((*(u32 *)(temp_v0 + 0x220) & 0x4000000) == 0)) {
        func_001355D8();
        func_00134910();
        func_00134A18();
    }
    func_002007B0();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200930);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002009E0);

void func_00200AD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200AE0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200B30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200D00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200DA0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200F28);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200FB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201108);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201268);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002014A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201520);

SoundTask *func_00201540(void) {
    SoundTask *task = (SoundTask *)func_002014A8();
    task->taskId = 7;
    task->callback.update = func_00201520;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201578);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateAcquireTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201650);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateReleaseTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201718);

SoundTask *func_00201738(void) {
    SoundTask *task = (SoundTask *)nbSoundCreateReleaseTask();
    task->taskId = 8;
    task->callback.update = func_00201718;
    return task;
}

void func_00201770(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundLookupResourceType);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201828);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateSystemEffect);

void nbSoundDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        func_0020D128("btl:system effect delete[%p]\n", effect[4]);
        func_001683F0(effect[4]);
        effect[4] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", addSoundEffectReferences);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201C98);

INCLUDE_ASM(const s32, "game/code_001DACF8", releaseSoundEffectReferences);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201F08);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateEffectWithTargets);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundStartEffectTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202100);

void func_00202288(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x334) = *(s32 *)(temp_v1 + 0x334) - 1;
    nbSoundDeleteSystemEffect(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002022E0);

void func_002023A0(s32 *arg0) {
    *(s32 *)(*arg0 + 8) = *(s32 *)(*arg0 + 8) + 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002023B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202450);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202518);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002025A0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202648);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002026E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202750);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002027B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202840);

u32 func_002028A8(void) {
    func_001057A8();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002028C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", addSoundSourceReferences);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202958);

void func_00202B58(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    temp_v0 = *arg0;
    temp_v1 = arg0[2];
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) - 1;
    *(s32 *)(temp_v1 + 0x334) = *(s32 *)(temp_v1 + 0x334) - 1;
    nbSoundDeleteSystemEffect(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202BB0);

u32 func_00202C70(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202C90);

u32 func_00202CF8(u8 *arg0) {
    kwlnFadeInStart(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", createCustomSoundTask);

u32 func_00202DA8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) | 0x40000;
    func_002DCC30();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateSetBattleFlagTask);

u32 func_00202E28(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffbffff;
    func_002DCC68();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateClearBattleFlagTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202EA8);

void func_00202F40(s32 arg0, u16 arg1) {
    func_001684A8(*(u32 *)(arg0 + 0x10), arg1);
}

void func_00202F60(s32 arg0, u16 arg1) {
    func_00168448(*(u32 *)(arg0 + 0x10), arg1);
}

s32 func_00202F80(s32 arg0) {
    if (*(s32 *)(arg0 + 4) != 0) {
        return 1;
    }
    return *(u32 *)(arg0 + 8) != 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", hasActiveActorSound);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundAllocResourceNode);

SoundResourceNode *nbSoundCreateResourceNode(u32 soundId) {
    SoundResourceNode *node = (SoundResourceNode *)nbSoundAllocResourceNode();
    node->resourceHandle = func_001682B0(soundId);
    node->flags |= 2;
    return node;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundFreeResourceNode);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203138);

void func_00203258(void) {
    func_002DCB58();
}

void func_00203270(void) {
    func_00169608();
    func_002D2CA8();
    D_00435CD4 = D_00435CD4 & 0xdfffffff;
}

void func_002032A8(void) {
    nbSoundClearResourceNodes();
    func_002DCCA0();
    func_00169608();
    func_002D2CA8();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundClearResourceNodes);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203318);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundAllocResourceLink);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundFreeResourceLink);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002034A8);

void func_002037F0(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundAllocLink);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundFreeLink);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203890);

u32 func_00203AB0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateClearStateTask);

u32 func_00203B20(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateSetStateTask);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418E58);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418E70);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418E88);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418EA0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418EB8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418ED0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418EE8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F00);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F18);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F30);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F48);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F60);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F78);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418F90);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418FA8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418FC0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418FD8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418FF0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419008);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419020);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419040);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419060);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419080);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419098);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004190B0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004190C8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004190E0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004190F8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419110);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419128);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419140);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419158);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419170);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203B90);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203C78);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203CD8);

void func_00203D48(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = func_001AA6F8();
    puVar3 = (u32 *)nbSoundAllocResourceNode();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4ec) = puVar3;
    *puVar3 = temp_v0 | 10;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203DA8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203E18);

void func_00203E90(u32 arg0) {
    func_002D4548(*(u32 *)arg0);
    func_00328E48(arg0);
}

void func_00203EC0(void) {
}

void func_00203EC8(u32 arg0) {
    soundSetSequenceVolumePan(arg0, 0x58, 0x3f);
}

void func_00203EE8(u32 arg0) {
    soundSetSequenceVolumePan(arg0, 0x319c, 0x3f);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203F08);

u8 func_00203FD8(void) {
    s32 temp_v0;

    temp_v0 = func_002A2330();
    return temp_v0 - 2U < 2;
}

void func_00204000(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x10000) != 0) {
        func_002A2388();
        return;
    }
}

void func_00204038(void) {
    advanceTitleStateUnderSemaphore();
}

void func_00204050(void) {
    advanceTitleStateUnderSemaphore();
    func_00341CD0();
    func_00341CA8();
}

void func_00204078(void) {
    func_00204038();
}

void func_00204090(void) {
    func_00204050();
}

s32 func_002040A8(u32 soundId) {
    s32 loaded = func_00342168(soundId);
    if (loaded != 0) {
        func_00203EC8(soundId);
        return 1;
    }
    return loaded;
}

s32 nbSoundPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (func_002040A8(soundId)) {
        func_0020D128("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateStationedSeTask);

s32 func_00204190(void) {
    s32 node = *(s32 *)(func_001AA6F8() + 0x25C);
    while (node != 0) {
        if ((*(u32 *)node & 8) != 0) {
            return 1;
        }
        node = *(s32 *)(node + 0x10);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002041E8);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateSkillSeTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204358);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002043C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204508);

s32 nbSoundLoadDataFile(s32 *data) {
    char filename[0x70];
    if (func_00204678()) {
        return 1;
    }
    func_002046F0(data[0], (s32)filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204608);

s32 func_00204678(void) {
    return (s8)func_00342688();
}

s32 func_002046A0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    if ((temp_v0 & 1) != 0) {
        return 1;
    }
    return (temp_v0 & 8) > 0;
}

void func_002046C0(s32 source, s32 output) {
    func_0035C860(output, D_004192D8, D_00436AE8, source + 0x200);
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002046F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204718);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundAllocListNode);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundFreeListNode);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundClearList);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002048C8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419318);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204958);

void *findSoundListNodeForChannel(s32 category, s32 id) {
    u8 *node = *(u8 **)(func_001AA6F8() + 0x260);
    while (node != 0) {
        if (*(s32 *)(node + 4) == category && *(s32 *)(node + 8) == id) {
            return node;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204B00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204BD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204CC8);

void func_00204D08(void) {
    u64 temp_v0;

    temp_v0 = func_00205018();
    func_001E1580(temp_v0);
}

s32 nbSoundHasActiveFileLoad(void) {
    u8 *node = *(u8 **)(func_001AA6F8() + 0x260);
    while (node != 0) {
        if ((*(u32 *)node & 8) != 0) {
            return 1;
        }
        node = *(u8 **)(node + 0x104);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204D80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204E50);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205018);

void func_002050D0(void) {
    s32 context = func_001AA6F8();
    *(s32 *)(context + 0x288) = -1;
    *(s32 *)(context + 0x28C) = -1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", loadBattleSoundBank);

u8 func_00205140(void) {
    s64 temp_v0;

    temp_v0 = func_00342168(0x10000);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205160);

INCLUDE_ASM(const s32, "game/code_001DACF8", hasOccupiedSoundNodeSlots);

u32 func_00205438(void) {
    u32 temp_v0;
    s64 temp_v1;

    temp_v1 = func_002A2928();
    temp_v0 = 1;
    if (temp_v1 != 0) {
        if (temp_v1 == 2) {
            func_002A2998();
            temp_v0 = 0;
        }
        else {
            temp_v0 = 0;
        }
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundCreateEarringTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002054B8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002055E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205660);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205758);

INCLUDE_ASM(const s32, "game/code_001DACF8", nbSoundFinishEarringPlayback);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205890);

u32 func_00205930(void) {
    func_002040A8(0x1c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205950);

u32 func_00205990(void) {
    func_00204038();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002059B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002059F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205CC8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206060);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206090);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002060B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002061B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206370);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206570);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206840);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206970);

void func_00206C10(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206C18);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00206EA8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00207268);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00207438);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002076E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00207728);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002077C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00207958);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419540);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A10);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A18);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A1C);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A20);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A28);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A30);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A38);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A40);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A48);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A50);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A58);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A60);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A68);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A70);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A78);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A80);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A88);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A90);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A98);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436A9C);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AA0);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AB0);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AB8);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AC0);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AD0);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AD4);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AD8);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AE0);

INCLUDE_SDATA(const s32, "game/code_001DACF8", D_00436AE8);

