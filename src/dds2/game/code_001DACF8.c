#include "common.h"
#include "pcp_vu0.h"

extern s64 func_002A2928(void);
extern void func_001EC868(void *, f32 *, f32);

typedef struct BtlUnitData {
    u8 pad0[0x1C];
    f32 f1C;
    union {
        s32 unk20;
        f32 f20;
    };
    u8 pad24[10];
    u16 s2E;
    u8 b30;
} BtlUnitData;

typedef struct BtlUnitInfo {
    union {
        u8 b0;
        u32 flags;
    };
    u8 pad1[0x14];
    s32 unk18;
    BtlUnitData *data;
} BtlUnitInfo;

typedef struct BtlUnitExt {
    u8 pad0[0x8C];
    BtlUnitInfo *info;
} BtlUnitExt;

typedef struct BtlUnit BtlUnit;

typedef struct BtlWork {
    u8 pad0[0x218];
    u32 flags218;
    u8 pad21C[0x2C];
    BtlUnit *head;
    BtlUnit *list24C;
    struct SoundTask *taskList250;
    struct SoundTask *taskList254;
    struct SoundResourceNode *resourceList258;
    struct ActiveSoundNode *soundList;
    struct SoundSlotOwner *list260;
    u8 pad264[0x14];
    s32 unk278;
    u8 pad27C[0x78];
    s32 unk2F4;
    u8 pad2F8[0x1CC];
    s8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    u8 pad4CC[0xE4];
    s32 unk5B0;
    s32 unk5B4;
    u8 pad5B8[0x38];
    s32 (*hook5F0)(BtlUnit *, s32);
    u8 pad5F4[0x5C];
    s32 (*hook650)(BtlUnit *);
    u8 pad654[4];
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[0x28];
    s32 (*hook684)(s32, s32);
    u8 pad688[0x14];
    s32 (*hook69C)(BtlUnit *);
    s32 (*hook6A0)(BtlUnit *);
    u8 pad6A4[4];
    void (*hook)(BtlUnit *);
    u8 pad6AC[0x78];
    s32 unk724;
} BtlWork;

struct BtlUnit {
    u8 pad0[0x18];
    BtlUnit *link18;
    u8 pad1C[0x38];
    u32 unk54;
    u8 pad58[0x2C];
    u32 unk84;
    f32 unk88;
    u8 pad8C[0x40];
    u8 unkCC;
    u8 padCD[0x1B];
    u32 unkE8;
    u8 padEC[4];
    s32 unkF0;
    f32 fF4;
    u8 padF8[0x10];
    u64 owner;
    u32 flags;
    u32 stateFlags;
    s32 unk118;
    u8 unk11C;
    u8 pad11D[3];
    u16 unk120;
    u8 pad122[2];
    u16 mode;
    u8 pad126[0x4E];
    BtlUnit *prev;
    BtlUnit *next;
    u8 pad17C[0x168];
    u8 unk2E4;
    u8 pad2E5[0x2B];
    s32 unk310;
    s32 unk314;
    struct SoundResourceNode *node318;
    struct SoundResourceLink *link31C;
    struct SoundLink *link320;
    struct ActiveSoundNode *node324;
    s32 unk328;
    s32 unk32C;
    u16 unk330;
    u8 pad332[2];
    s32 unk334;
    u8 pad338[4];
    s32 unk33C;
    BtlUnitExt *ext;
    s32 unk344;
    u8 pad348[4];
    s32 unk34C;
    s32 unk350;
    u8 pad354[8];
    u32 handle35C;
    BtlUnit *link360;
    BtlUnit *link364;
};

typedef struct BtlStateHandler {
    void (*fn)(void *);
    u32 unk4;
    u32 unk8;
} BtlStateHandler;

extern BtlStateHandler D_003B69D8[];

typedef struct BtlFx {
    u8 pad0[0x90];
    s128 vec90;
    u8 padA0[0x10];
    f32 fB0;
    f32 fB4;
    u8 padB8[8];
    f32 fC0;
} BtlFx;

typedef struct FxTask {
    u8 pad0[0x10];
    s32 unk10;
    BtlUnit *unit;
} FxTask;

typedef struct SoundLink {
    u32 owner;
    s32 effectHandle;
    u32 *effect;
    u16 flags;
} SoundLink;

typedef struct SoundResourceLink {
    u32 owner;
    s32 effectHandle;
    u32 *effect;
    u32 flags;
} SoundResourceLink;

typedef struct SoundEntry {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} SoundEntry;

extern SoundEntry D_003BDE18[];
extern s128 D_003B6B80;
extern u8 D_003BD7D0[];
extern void func_0023C908(BtlUnitExt *, s32);

typedef struct XformData {
    s128 vec0;
    s128 vec1;
    f32 f20;
    f32 f24;
} XformData;


extern u64 func_00205018(void);

extern u32 D_00436AD4;

extern u64 func_0010FCA8(void);

extern u32 func_00116810(u64);

extern s8 func_001E3168(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_0022F180(void);

extern u64 fldCreateSceneGroupAction(u64, u64, u64);

extern s32 func_001AA6F8(void);

extern struct SoundTask *D_00436A1C;

extern struct SoundTask *D_00436A20;

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
    u16 unk_22;
    u16 flags;
    u8 unk_26[0xA];
    u32 unk_30;
    u32 unk_34;
    u64 unk_38;
    u64 owner;
    void (*onStart)(u32);
    union {
        void (*update)(void);
        s32 (*playSound)(u32 *);
        u32 (*command)(s32);
        u32 (*poll)(void);
        u32 (*commandArgs)(u32 *);
    } callback;
    void (*onFinish)(u32);
    void *args;
    struct SoundTask *next;
    struct SoundTask *nextActive;
    struct SoundTask *deferNext;
    struct SoundTask *deferPrev;
} SoundTask;

extern s64 func_00201520(void);

extern s64 func_00201718(void);

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

extern SoundResourceNode *sndAllocResourceNode(void);

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

extern s32 btlCountTasksForOwner(s64);

extern void func_001E15F0(s32);

extern s32 func_0020D128(const char *, ...);

extern s32 func_00232EE8(s32);

extern s32 func_00232EF8(s32);

extern f32 func_00208000(s32, s32, s32);

extern void func_0035C860();
extern char D_004192E8[]; /* "MDD_%03X.ADB" */

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


extern s32 func_00201578(u32 *);

extern u32 D_00436AD8;

typedef struct {
    union {
        void *actor;
        s32 value;
    };
    union {
        s32 option;
        u16 optionId;
    };
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
} SoundTaskArgs;

extern SoundTask *func_001E1468(s32);

extern SoundTaskArgs *func_001E14F8(s32);

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
    startBattleTask(value);
    value = func_001E6428(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    startBattleTask(value);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DBE70);

void func_001DC278(BtlUnit *unit) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    work->unk278 = work->unk278 + 1;
    if (func_001B2AF8(unit->link18) != 0) {
        work->flags218 |= 0x2000;
    } else {
        work->flags218 |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC2D8);

void func_001DC538(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC540);

void func_001DC7F0(void) {
}

void func_001DC7F8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = fldCreateSceneGroupAction(arg0, 0x1194, 1);
    startBattleTask(temp_v0);
    dispatchBattleStateHandler(arg0, 0x1b);
}

void func_001DC838(void) {
}

void func_001DC840(BtlUnit *unit) {
    if (btlCountTasksForOwner(*(u64 *)((u8 *)unit->link18 + 0x108)) == 0) {
        dispatchBattleStateHandler(unit, 0x1D);
    }
}

void func_001DC888(void) {
}

void func_001DC890(BtlUnit *unit) {
    BtlUnit *owner = unit->link18;
    if (btlCountTasksForOwner(*(u64 *)((u8 *)owner + 0x108)) == 0) {
        owner->flags &= ~0x4000;
        dispatchBattleStateHandler(unit, 2);
    }
}

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

void func_001DCE70(BtlUnit *unit) {
    void (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook;
    if (hook != 0) {
        hook(unit);
    }
    dispatchBattleStateHandler(unit, 0x1B);
}

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
        dispatchBattleStateHandler(arg0, 6);
        return;
    }
}

void dispatchBattleStateHandler(s32 *obj, s32 kind) {
    obj[0] = kind;
    obj[4] = 0;
    D_003B69D8[kind].fn(obj);
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417508);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417518);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417528);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417538);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417548);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DCF58);

void func_001DCFE0(BtlUnit *unit) {
    func_0020D128("btl:action seq delete[%p]\n", unit);
    releaseBattleObjectBuffers((s32 *)((u8 *)unit + 0x20));
    if (unit->next != 0) {
        unit->next->prev = unit->prev;
    }
    if (unit->prev != 0) {
        unit->prev->next = unit->next;
    } else {
        ((BtlWork *)func_001AA6F8())->head = unit->next;
    }
    func_00328E48(unit);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD060);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD108);

BtlUnit *findBattleUnitByActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->head; unit != 0; unit = unit->next) {
        if (unit->link18 == actor) {
            return unit;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD1A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD390);

s32 func_001DD5E8(s32 *state) {
    switch (*state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

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

extern void *btlAllocateIndexList(s32);

extern u32 func_003292A8(s32);

extern u32 sdfResourceRetainAddress(u32);

extern void func_001DF700(BattleIndexWork *);

void func_001DF7B8(BattleIndexWork *work) {
    u32 command;
    work->indices = btlAllocateIndexList(13);
    command = func_003292A8(0x48EC);
    work->device = sdfResourceRetainAddress(command);
    work->command = command;
    work->previous = 0;
    func_001DF700(work);
}

void releaseBattleObjectBuffers(s32 *object) {
    if (object[0x6C / 4] != 0) {
        func_003297C8(object[0x6C / 4]);
        object[0x6C / 4] = 0;
    }
    if (object[0x40 / 4] != 0) {
        func_001E8018(object[0x40 / 4]);
        object[0x40 / 4] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DF860);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFB08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFBE0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFC80);

u32 func_001DFD58(void *arg) {
    s32 *args = arg;
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit = (BtlUnit *)args[0];
    if (!(work->flags218 & 0x80)) {
        return 1;
    }
    func_001AA850((u8 *)unit + 0x120, args[1]);
    func_001E2758(unit);
    func_001B2430(unit, 0);
    return 1;
}

SoundTask *func_001DFDC0(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4B;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001DFD58;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

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

extern u32 func_001E02A8(s32);

SoundTask *func_001E05B8(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x50;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E02A8;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

typedef struct {
    u32 unk0;
    s32 soundIndex;
} BattleVoiceWork;

extern u8 *D_00435E38;
extern void func_0011A118(s32, s32);
extern void func_001AF060(void);

s32 func_001E0640(BattleVoiceWork *work) {
    s32 index = work->soundIndex;
    if (D_00435E38[index * 8 + 1] & 4) {
        func_0011A118(index, -1);
        switch (work->soundIndex) {
        case 0x53:
        case 0x54:
            func_001AF060();
            break;
        }
    }
    return 1;
}

SoundTask *func_001E06B0(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x51;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E0640;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 func_001E0738(s32 arg0) {
    func_0011A118(*(u16 *)(arg0 + 4), 1);
    return 1;
}

SoundTask *func_001E0760(void *actor, u16 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x52;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E0738;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->optionId = option;
    return task;
}

u32 func_001E07E8(s32 arg0) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (((s32 *)arg0)[1] == 0) {
        return 1;
    }
    if ((*(BtlUnit **)arg0)->flags & 0x400) {
        return 1;
    }
    work->unk2F4 += ((s32 *)arg0)[1];
    func_0020D128("btl:epall=%d[%d](packet)\n", work->unk2F4, ((s32 *)arg0)[1]);
    return 1;
}

extern u32 func_001E07E8(s32);

SoundTask *func_001E0858(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x53;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E07E8;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E08E0);

extern u32 func_001E08E0(s32);

SoundTask *func_001E0950(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x54;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E08E0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E09D8);

extern u32 func_001E09D8(void);

SoundTask *func_001E0B08(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E09D8;
    task->taskId = 0x55;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_001E0B50(s32 arg0) {
    func_0011A0D0(*(u32 *)(arg0 + 4));
    return 1;
}

SoundTask *func_001E0B70(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x56;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E0B50;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0BF8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0CE0);

void func_001E1100(void) {
}

SoundTask *findBattleTaskByHandle(u64 value) {
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->unk_38 == value) {
            return task;
        }
    }
    return 0;
}

SoundTask *findBattleTaskByOwner(u64 owner) {
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->owner == owner) {
            return task;
        }
    }
    return 0;
}

SoundTask *btlFindTaskByKind(u16 kind) {
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->taskId == kind) {
            return task;
        }
    }
    return 0;
}

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

s32 btlCountTasksForOwner(s64 owner) {
    s32 count = 0;
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->owner == owner) {
            count++;
        }
    }
    return count;
}

s32 btlCountTasksByKind(u16 kind) {
    s32 count = 0;
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->taskId == kind) {
            count++;
        }
    }
    return count;
}

void flagBattleTasksForUpdate(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)func_001AA6F8())->taskList250; task != 0; task = next) {
        u16 flags = task->flags;
        next = task->nextActive;
        if (flags & 1) {
            task->flags = flags | 4;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1368);

extern void *func_00328E18(s32);

SoundTask *func_001E1468(s32 size) {
    SoundTask *task = func_00328E18(size + 0x70);
    BtlWork *work;
    if (size > 0) {
        task->args = (u8 *)task + 0x70;
    } else {
        task->args = 0;
    }
    work = (BtlWork *)func_001AA6F8();
    task->next = 0;
    if (work->taskList250 != 0) {
        work->taskList250->next = task;
        task->nextActive = work->taskList250;
    } else {
        work->taskList254 = task;
        task->nextActive = 0;
    }
    work->taskList250 = task;
    task->flags |= 1;
    return task;
}

SoundTaskArgs *func_001E14F8(s32 arg0) {
    return *(SoundTaskArgs **)(arg0 + 0x54);
}

extern void func_00328E48(void *);

void func_001E1500(SoundTask *task) {
    BtlWork *work;
    if (task->onFinish != 0) {
        task->onFinish((u32)task->args);
    }
    work = (BtlWork *)func_001AA6F8();
    if (task->nextActive != 0) {
        task->nextActive->next = task->next;
    } else {
        work->taskList254 = task->next;
    }
    if (task->next != 0) {
        task->next->nextActive = task->nextActive;
    } else {
        work->taskList250 = task->nextActive;
    }
    func_00328E48(task);
}

u64 startBattleTask(task)
    SoundTask *task;
{
    task->unk_38 = func_001A9920();
    task->flags |= 8;
    task->unk_30 = 0;
    task->unk_34 = 0;
    task->unk_22 = 0;
    task->deferNext = 0;
    task->deferPrev = 0;
    if (task->onStart != 0) {
        task->onStart((u32)task->args);
    }
    return task->unk_38;
}

void func_001E15E0(void) {
    D_00436A1C = 0;
    D_00436A20 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E15F0);

void func_001E1730(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = next) {
        next = task->next;
        if (!(task->flags & 2)) {
            func_001E15F0((s32)task);
        } else {
            task->deferNext = 0;
            if (D_00436A1C != 0) {
                D_00436A1C->deferNext = task;
                task->deferPrev = D_00436A1C;
            } else {
                D_00436A20 = task;
                task->deferPrev = 0;
            }
            D_00436A1C = task;
        }
    }
}

void btlClearDeferredTasks(void) {
    SoundTask *node = D_00436A20;
    while (node != 0) {
        SoundTask *next = node->deferNext;
        func_001E15F0((s32)node);
        node = next;
    }
    D_00436A1C = 0;
    D_00436A20 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1800);

u32 func_001E1848(void) {
    return 1;
}

SoundTask *func_001E1850(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E1848;
    task->taskId = 0x6A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", btlDumpTaskQueue);

void func_001E1918(BtlFx *fx) {
    PCP_COPY_VECTOR(&fx->vec90, &D_003B6B80);
    fx->fB0 = 220.0f;
    fx->fB4 = 80.0f;
    fx->fC0 = 75.0f;
}

extern s128 D_003B6B90;
extern s128 D_003B6BA0;

typedef struct BtlFxLight {
    s128 vecA;
    s128 vecB;
    f32 intensity;
    u32 color;
    u32 unk8;
} BtlFxLight;

typedef struct BtlFxLights {
    u8 pad0[0x30];
    BtlFxLight light0;
    BtlFxLight light1;
} BtlFxLights;

void func_001E1958(BtlFxLights *fx) {
    PCP_COPY_VECTOR(&fx->light0.vecA, &D_003B6B90);
    PCP_COPY_VECTOR(&fx->light0.vecB, &D_003B6BA0);
    fx->light0.unk8 = 0;
    fx->light0.intensity = 1.0f;
    fx->light0.color = 0x80808080;
    PCP_COPY_VECTOR(&fx->light1.vecA, &D_003B6B90);
    PCP_COPY_VECTOR(&fx->light1.vecB, &D_003B6BA0);
    fx->light1.intensity = 1.0f;
    fx->light1.color = 0x80808080;
    fx->light1.unk8 = 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E19C8);

INCLUDE_ASM(const s32, "game/code_001DACF8", btlHasMatchingModel);

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

void func_001E21A0(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook69C;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags |= 4;
        if (!(unit->flags & 0x8000000)) {
            unit->flags |= 8;
            if (unit->flags & 2) {
                unit->ext->info->flags &= ~1;
            }
        }
    }
}

void func_001E2220(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook6A0;
    if (hook == 0 || hook(unit) != 0) {
        unit->flags &= ~4;
        unit->flags &= ~8;
        if (unit->flags & 2) {
            unit->ext->info->flags |= 1;
        }
    }
}

u32 func_001E2298(BtlUnit *unit) {
    if (unit->flags & 0x8000000) {
        return 0;
    }
    if (!(unit->flags & 1)) {
        return 0;
    }
    if (!(unit->flags & 2)) {
        return 0;
    }
    return unit->ext->info->b0 & 1;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417940);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E22D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2758);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2B60);

extern s32 func_001ABFD8(s32, s32);
extern void func_001E22D8(u8 *, s32, s32, f32);

void func_001E2C00(u8 *unit, s32 index, s32 arg2, f32 scale) {
    u8 *table = (u8 *)func_001ABFD8(*(s32 *)(unit + 0xC4), *(s32 *)(unit + 0xC8));
    func_001E22D8(unit, index, arg2, *(f32 *)(table + index * 0x14 + 0x34) * scale);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2C78);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2CD8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2DA0);

void func_001E2E20(BtlUnit *unit) {
    if (unit->flags & 2) {
        unit->unkE8 |= 1;
        unit->ext->info->data->unk20 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2E58);

f32 func_001E2EE8(BtlUnit *unit) {
    f32 value = 0.0f;
    if (unit->flags & 2) {
        value = unit->ext->info->data->f1C;
    }
    return value;
}

void func_001E2F18(s32 arg0) {
    if ((*(u32 *)(arg0 + 0x110) & 2) != 0) {
        func_003343E8(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x340) + 0x8c) + 0x1c));
        return;
    }
}

u16 func_001E2F50(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 0;
    }
    return unit->ext->info->data->s2E;
}

extern void func_003343E8(BtlUnitData *, f32);

void func_001E2F78(BtlUnit *unit) {
    if (unit->flags & 2) {
        func_003343E8(unit->ext->info->data, 0.0f);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2FB0);

s32 func_001E3040(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return 1;
    }
    if (unit->unkF0 != 2) {
        return 1;
    }
    return unit->ext->info->data->b30 == 5;
}

extern void effObjSetInnerFirstVec(s32, f32 *);

void func_001E3088(BtlUnit *unit, f32 *vec) {
    f32 pos[4];
    if (!(unit->stateFlags & 0x80)) {
        u8 *work = (u8 *)func_001AA6F8();
        __asm__ volatile("lqc2 vf10, 0(%0)" : : "r"(vec));
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"((u8 *)unit + 0x60));
        __asm__ volatile("lqc2 vf11, 0(%0)\n\t"
                         "vadd.xyzw vf10, vf10, vf11\n\t"
                         "sqc2 vf10, 0(%1)"
                         : : "r"(work), "r"(pos));
        if (unit->flags & 2) {
            pos[2] += unit->unk88;
            effObjSetInnerFirstVec(unit->unk33C, pos);
        }
    }
}

void func_001E3108(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x60);
}

void func_001E3120(u8 *unit, s128 *dst) {
    u8 *work = (u8 *)func_001AA6F8();
    __asm__ volatile(".set noreorder\n\t"
                     "lqc2 vf10, 0(%0)\n\t"
                     "lqc2 vf11, 0(%1)\n\t"
                     "vadd.xyzw vf10, vf10, vf11\n\t"
                     "sqc2 vf10, 0(%2)\n\t"
                     ".set reorder"
                     : : "r"(unit + 0x60), "r"(work), "r"(dst) : "memory");
}

extern s32 func_00332D48(s32, s32);
extern void func_001E3628(BtlUnit *);

s8 func_001E3168(BtlUnit *unit, s32 mode) {
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlWork *)func_001AA6F8())->hook5F0;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    func_001E3628(unit);
    return func_00332D48(unit->ext->info->unk18, mode);
}

void func_001E31F0(BtlUnit *arg0, s32 mode) {
    if (func_001E3168(arg0, mode) == 0) {
        btlUnitGetMuzzlePosVU(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3230);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3320);

void func_001E33A8(void) {
    if (func_001E3320() == 0) {
        __asm__ volatile(".set noreorder\n\t"
                         "vsub.xyzw vf28, vf0, vf0\n\t"
                         "vmr32.xyzw vf30, vf0\n\t"
                         "vmove.xyzw vf31, vf0\n\t"
                         "vaddw.x vf28, vf28, vf0w\n\t"
                         "vmr32.xyzw vf29, vf30\n\t"
                         ".set reorder");
    }
}

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

extern u8 D_004179E0[];
extern void effMiscQuatMultiplyVU(void);
extern void effObjSetInnerSecondVec(s32, f32 *);

void func_001E3448(BtlUnit *unit, s128 *quat) {
    f32 result[4];
    if (!(unit->stateFlags & 0x100)) {
        __asm__ volatile("lqc2 vf10, 0(%0)" : : "r"(quat));
        if (unit->flags & 0x10) {
            __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_004179E0));
            effMiscQuatMultiplyVU();
        }
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"((u8 *)unit + 0x70));
        __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(D_004179E0));
        effMiscQuatMultiplyVU();
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(result));
        if (unit->flags & 2) {
            effObjSetInnerSecondVec(unit->unk33C, result);
        }
    }
}

void func_001E34D8(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x70);
}

extern void func_0023C978(BtlUnitExt *, s32, u32);

void func_001E34F0(BtlUnit *unit, u32 color, s32 mode) {
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        unit->unk54 = (unit->unk54 & 0xFF000000) | (color & 0xFFFFFF);
        func_0023C978(unit->ext, mode, color);
    }
}

void func_001E3560(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (unit->unk54 & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        unit->unk84 = (unit->unk84 & 0xFF000000) | (color & 0xFFFFFF);
        func_0023C978(unit->ext, mode, blended);
    }
}

void func_001E35F8(BtlUnit *unit, u32 value) {
    func_00232F58(unit->ext->info, (value & 0xFFFFFF) | 0x80000000);
}

extern void effObjFetchInnerFirstVec(s32);
extern void effObjFetchInnerSecondVecNorm(s32);
extern void func_00232AA0(BtlUnitInfo *);
extern void func_00232AD0(BtlUnitInfo *);
extern void func_00331B18(s32);

void func_001E3628(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(unit->unk33C);
    func_00232AA0(unit->ext->info);
    effObjFetchInnerSecondVecNorm(unit->unk33C);
    func_00232AD0(unit->ext->info);
    func_00331B18(unit->ext->info->unk18);
}

extern s32 func_002091C8(s128 *, s128 *);
extern void btlUnitGetBodyPosVU(BtlUnit *);
extern void func_001E3448(BtlUnit *, s128 *);

void func_001E36A0(BtlUnit *unit, BtlUnit *target) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&from));
        btlUnitGetBodyPosVU(target);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&to));
        if (func_002091C8(&from, &to) != 0) {
            __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&hit));
            func_001E3448(unit, &hit);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3720);

typedef struct {
    u8 unk00[0x110];
    u32 flags;
    u8 unk114[0x10];
    u16 objectId;
} BattleEntryHeader;

extern s32 btlGetEntryFlagsUnlessDisabled(const void *);

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
        return (btlGetEntryFlagsUnlessDisabled((u8 *)entry + 0x120) >> 14) & 1;
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

extern u32 func_001E4618(s32);

SoundTask *btlScheduleThresholdTask(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0xB;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E4618;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

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

INCLUDE_ASM(const s32, "game/code_001DACF8", btlScheduleRefreshTask);

INCLUDE_ASM(const s32, "game/code_001DACF8", btlBeginModelChange);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E50E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5790);

void func_001E5870(s32 arg0) {
    if ((*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x110) & 2) != 0) {
        evtUnitSetStateBits(*(u32 *)(*(s32 *)(arg0 + 0xc) + 0x340));
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
        evtUnitSetStateBits(*(u32 *)(*(s32 *)(arg0 + 0x14) + 0x340));
        return;
    }
}

s32 func_001E59D0(FxTask *task) {
    BtlUnit *unit = task->unit;
    if (unit->flags & 2) {
        __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(task) : "memory");
        func_0023C908(unit->ext, task->unk10);
    }
    return 1;
}

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

SoundTask *func_001E66D8(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E66B8;
    task->taskId = 0x1B;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_001E6720(void) {
    func_00209078();
    return 1;
}

SoundTask *func_001E6740(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->taskId = 0x1C;
    task->flags |= 2;
    task->status = 0;
    task->onStart = 0;
    task->callback.update = func_001E6720;
    return task;
}

u32 func_001E6790(void) {
    return 1;
}

SoundTask *func_001E6798(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E6790;
    task->taskId = 0x20;
    task->onStart = 0;
    task->status = 0;
    return task;
}

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

INCLUDE_ASM(const s32, "game/code_001DACF8", btlScheduleActorUpdate);

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

SoundTask *func_001E71A8(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E7188;
    task->taskId = 0x24;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_001E71F0(u32 *task) {
    BtlUnit *unit = *(BtlUnit **)task;
    if (!(unit->flags & 2)) {
        return 0;
    }
    func_001E3C28(unit);
    (*(BtlUnit **)task)->flags |= 0x20000;
    return 1;
}

extern u32 func_001E71F0(u32 *);

SoundTask *func_001E7248(u32 value) {
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x25;
    task->callback.commandArgs = func_001E71F0;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E72B0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7378);

u32 func_001E7438(s32 arg0) {
    s128 hit[1];
    s128 from;
    s128 to;
    btlUnitGetBodyPosVU(*(BtlUnit **)arg0);
    __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&from));
    btlUnitGetBodyPosVU(*(BtlUnit **)(arg0 + 4));
    __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&to));
    if (func_002091C8(&from, &to) != 0) {
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(hit));
        func_001E3448(*(BtlUnit **)arg0, hit);
    }
    return 1;
}

SoundTask *func_001E74A0(void *actor, s32 option) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x27;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback.command = func_001E7438;
    task->onStart = 0;
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

void resetBattleUnitLinks(BtlUnit *unit) {
    unit->unk310 = -1;
    unit->unk314 = -1;
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->unk118 = 0;
    unit->unk330 = 0;
    func_001ADC48(unit);
    unit->link31C = sndAllocResourceLink(unit);
    unit->link320 = sndAllocLink(unit);
}

extern s64 func_001A9920(void);
extern void *memset(void *, s32, u32);

BtlUnit *func_001E7B58(void) {
    u32 handle = func_003292A8(0x368);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlWork *work;
    memset(unit, 0, 0x368);
    unit->handle35C = handle;
    unit->owner = func_001A9920();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->unk11C = unit->unk310 = -1;
    unit->unk2E4 = 6;
    unit->unk118 = 0;
    unit->unk334 = 0;
    unit->node318 = 0;
    unit->unk32C = 0;
    unit->unk33C = 0;
    unit->ext = 0;
    func_001E1918((BtlFx *)unit);
    func_001E1958((BtlFxLights *)unit);
    resetBattleUnitLinks(unit);
    work = (BtlWork *)func_001AA6F8();
    unit->link360 = 0;
    if (work->list24C != 0) {
        work->list24C->link360 = unit;
        unit->link364 = work->list24C;
    } else {
        unit->link364 = 0;
    }
    work->list24C = unit;
    func_0020D128("btl:unit create[%p]\n", unit);
    return unit;
}

extern void sndFreeResourceNode(struct SoundResourceNode *);
extern void sndFreeResourceLink(struct SoundResourceLink *);
extern void sndFreeLink(struct SoundLink *);
extern void func_00328420(s32);
extern void func_003298C0(s32);
extern void func_001E1F98(BtlUnit *);
extern void sndFreeListNode(struct ActiveSoundNode *);

void func_001E7C48(BtlUnit *unit) {
    func_0020D128("btl:unit data free[%p]\n", unit);
    if (unit->node318 != 0) {
        sndFreeResourceNode(unit->node318);
        unit->node318 = 0;
    }
    if (unit->link31C != 0) {
        sndFreeResourceLink(unit->link31C);
        unit->link31C = 0;
    }
    if (unit->link320 != 0) {
        sndFreeLink(unit->link320);
        unit->link320 = 0;
    }
    if (unit->unk32C != 0) {
        func_00328420(unit->unk32C);
        unit->unk32C = 0;
        unit->unk118 &= ~4;
        unit->unk118 &= ~8;
    }
    if (unit->node324 != 0) {
        sndFreeListNode(unit->node324);
        unit->node324 = 0;
    }
    func_001E1F98(unit);
    if (unit->unk350 != 0) {
        func_003298C0(unit->unk350);
        unit->unk350 = 0;
        unit->unk34C = 0;
    }
}

extern void func_003297C8(u32);

void func_001E7D30(BtlUnit *unit) {
    func_0020D128("btl:unit delete[%p]\n", unit);
    func_001E7C48(unit);
    if (unit->link364 != 0) {
        unit->link364->link360 = unit->link360;
    }
    if (unit->link360 != 0) {
        unit->link360->link364 = unit->link364;
    } else {
        ((BtlWork *)func_001AA6F8())->list24C = unit->link364;
    }
    func_003297C8(unit->handle35C);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7DB0);

void btlRemoveActorsWithFlags(u32 mask) {
    s32 actor = *(s32 *)(func_001AA6F8() + 0x24c);
    s32 next;
    while (actor != 0) {
        next = *(s32 *)(actor + 0x364);
        if (*(u32 *)(actor + 0x110) & mask) {
            func_001E7D30(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(u64 owner) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if (unit->owner == owner) {
            return unit;
        }
    }
    return 0;
}

s32 btlIsActiveActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if (unit == actor) {
            return 1;
        }
    }
    return 0;
}

BtlUnit *func_001E7F08(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if (!(unit->unk120 & 0x20) && unit->mode == mode) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *func_001E7F70(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if ((unit->unk120 & 0x20) && unit->mode == mode) {
            return unit;
        }
    }
    return 0;
}

void *btlAllocateIndexList(s32 capacity) {
    u8 *list = func_00328E18(capacity * 4 + 12);
    *(s32 *)list = capacity;
    *(u32 **)(list + 8) = (u32 *)(list + 12);
    *(s32 *)(list + 4) = 0;
    return list;
}

void func_001E8018(s32 list) {
    func_00328E48(list);
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

void btlCopyIndexList(s32 destination, s32 source) {
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

u32 func_001E8128(s32 arg0, s32 value) {
    u32 count = func_001E8058(arg0);
    u32 i;
    for (i = 0; i < count; i++) {
        if (value == func_001E8060(arg0, i)) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E81A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8258);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8428);

void func_001E8510(f32 *src) {
    f32 vec[4];
    f32 step = -src[8];
    vec[3] = 0.0f;
    vec[0] = src[4] * step + src[0];
    vec[1] = src[5] * step + src[1];
    vec[2] = src[6] * step + src[2];
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(vec) : "memory");
}

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

extern void func_001E8580(XformData *, XformData *, XformData *, f32);
extern void func_00209720(u8 *, f32);
extern f32 func_00209770(f32);

s32 func_001E8650(u8 *fx) {
    f32 t;
    if (*(s32 *)(fx + 0x130) == 0) {
        *(s32 *)(fx + 0x14C) = 0;
        func_00209720(fx + 0x160, (f32)(*(s32 *)(fx + 0x150) * 2));
        func_001E9598((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    if (t > 0.5f) {
        t = 0.5f;
    }
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t + t);
    *(f32 *)(fx + 0x14C) = t;
    if (0.5f <= t) {
        return 1;
    }
    return 0;
}

extern void func_002096B8(u8 *, f32);
extern f32 func_002096C8(u8 *);
extern void func_001E8580(XformData *, XformData *, XformData *, f32);

s32 func_001E8708(u8 *fx) {
    u8 *timer = fx + 0x158;
    u8 *pose = fx + 0x30;
    f32 t;
    if (*(s32 *)(fx + 0x130) == 0) {
        func_002096B8(timer, *(f32 *)(fx + 0x154));
        func_001E9598((XformData *)fx, (XformData *)pose);
    }
    t = func_002096C8(timer);
    func_001E8580((XformData *)fx, (XformData *)pose, (XformData *)(fx + 0xC0), t);
    *(f32 *)(fx + 0x14C) = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(XformData *, XformData *, XformData *, f32);
extern void func_00209720(u8 *, f32);
extern f32 func_00209770(f32);

s32 func_001E87A0(u8 *fx) {
    f32 t;
    if (*(s32 *)(fx + 0x130) == 0) {
        *(s32 *)(fx + 0x14C) = 0;
        func_00209720(fx + 0x160, (f32)*(s32 *)(fx + 0x150));
        func_001E9598((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t);
    *(f32 *)(fx + 0x14C) = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E8840);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E88A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E89E0);

u32 func_001E8B08(u32 *arg0) {
    func_001E8258(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

SoundTask *btlCreateCommandSoundTask(s32 actor, s32 mode) {
    SoundTask *task = func_001E1468(0x14);
    SoundTaskArgs *args;

    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2A;
    if (actor != 0 && *(s32 *)(actor + 0x18) != 0) {
        task->owner = *(u64 *)(*(s32 *)(actor + 0x18) + 0x108);
    }
    task->callback.playSound = func_001E8B08;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = (void *)actor;
    args->unk_0C = mode;
    args->option = 0;
    args->unk_08 = 0;
    args->unk_10 = 0;
    return task;
}

SoundTask *func_001E8BE0(s32 actor, s32 mode, u32 command) {
    SoundTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = func_001E14F8((s32)task);

    args->unk_10 = command;
    return task;
}

SoundTask *func_001E8C20(s32 actor, s32 option, s32 flag, s32 mode, s32 command) {
    SoundTask *task = btlCreateCommandSoundTask(actor, mode);
    SoundTaskArgs *args = func_001E14F8((s32)task);

    args->unk_10 = command;
    args->option = option;
    args->unk_08 = flag;
    return task;
}

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

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_001E90C0;
    task->taskId = 0x2D;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9130);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9410);

void btlClearPendingSoundList(void) {
    s32 context = func_001AA6F8();
    s32 list = *(s32 *)(context + 0x1A8);

    if (list != 0) {
        func_001E8018(list);
        *(s32 *)(context + 0x1A8) = 0;
    }
    *(u32 *)(context + 0x218) &= ~0x10;
}

void func_001E9598(XformData *dst, XformData *src) {
    PCP_COPY_VECTOR(&dst->vec0, &src->vec0);
    PCP_COPY_VECTOR(&dst->vec1, &src->vec1);
    dst->f20 = src->f20;
    dst->f24 = src->f24;
}

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

f32 func_001E97B8(u8 *unit) {
    return *(f32 *)(unit + 0x14C);
}

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

extern s32 D_00436A9C;
extern s32 D_00436AA0;

typedef struct WorldMotionData {
    u8 pad0[8];
    s32 unk8;
} WorldMotionData;

extern void *dds3GetWorldObject(void);
extern WorldMotionData *func_00110C18(void *);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E99C0);

s32 func_001E9A18(void) {
    WorldMotionData *data;
    if (!(((BtlWork *)func_001AA6F8())->flags218 & 2)) {
        return 0;
    }
    data = func_00110C18(dds3GetWorldObject());
    if (data == 0) {
        return 0;
    }
    return data->unk8 == 0;
}

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

s32 btlHasMarkedEntry14(s32 actor) {
    s32 linked = *(s32 *)(actor + 0x114);
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = func_001E8058(*(s32 *)(linked + 0x60));
    entry = *(u8 **)(linked + 0x88);
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x14] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = *(u32 *)(actor + 0x124);
    s32 linked;
    s32 category;

    switch (status) {
    case 5:
    case 6:
    case 7:
    case 8:
        break;
    default:
        return 1;
    }
    linked = *(s32 *)(actor + 0x114);
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14(actor)) {
        return 0;
    }
    if (*(u16 *)(*(s32 *)(linked + 0x18) + 0x12E) & 0x480) {
        return 0;
    }
    category = *(s32 *)(actor + 0x134);
    if (category != 0 && (*(u16 *)(D_00435E30 + category * 32 + 0x1C) & 1)) {
        return 0;
    }
    return 1;
}

s32 btlHasMarkedEntry10(s32 actor) {
    s32 linked = *(s32 *)(actor + 0x114);
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = func_001E8058(*(s32 *)(linked + 0x60));
    entry = *(u8 **)(linked + 0x88);
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x10] != 0) {
            return 1;
        }
    }
    return 0;
}

extern f32 btlUnitGetTopY(s32);

s32 btlCheckActorDistanceLimit(void) {
    s32 actor = *(s32 *)(func_001AA6F8() + 0x24C);

    while (actor != 0) {
        u32 flags = *(u32 *)(actor + 0x110);
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY(actor) > 400.0f) {
                    return 0;
                }
            }
        }
        actor = *(s32 *)(actor + 0x364);
    }
    return 1;
}

s32 func_001EA3B8(void) {
    if (func_00208000(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA3F8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA4D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA598);

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(*(u16 *)(D_00435E30 + category * 32 + 0x1C), 0x100);
}

s32 btlIsActorCategoryTypeTwo(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category == 0) {
        return 0;
    }
    return *(s32 *)(D_00435E20 + category * 56 + 0x30) == 2;
}

extern s32 btlIsActorCategoryMarked(s32);

s32 btlCanUseActorCategoryFlag2(s32 actor) {
    s32 category;

    if (btlIsActorCategoryMarked(actor)) {
        return 1;
    }
    if (!btlCanUseLinkedActor(actor)) {
        return 0;
    }
    category = *(s32 *)(actor + 0x134);
    if (category == 0) {
        return 0;
    }
    return btlHasFlag(*(u16 *)(D_00435E30 + category * 32 + 0x1C), 2);
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category != 0 && *(u8 *)(D_00435E20 + category * 56 + 8) != 0) {
        return 0;
    }
    return func_001E8058(*(s32 *)(actor + 0x138)) == 1;
}

s32 btlCanUseActorCategoryFlag4(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category == 0) {
        return 0;
    }
    if ((*(u8 *)(D_00435E20 + category * 56 + 9) & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(*(u16 *)(D_00435E30 + *(s32 *)(actor + 0x134) * 32 + 0x1C), 4);
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category == 0) {
        return 0;
    }
    return *(s32 *)(D_00435E20 + category * 56 + 0x30) == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 category = *(s32 *)(actor + 0x134);

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(*(u16 *)(D_00435E30 + category * 32 + 0x1C), 0x40);
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    s32 entry;

    switch (*(u32 *)(actor + 0x124)) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = *(s32 *)(actor + 0x114);
    if (linked == 0) {
        return 0;
    }
    if (func_001E8058(*(s32 *)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = func_001E8060(*(s32 *)(linked + 0x60), 0);
    return ((*(u32 *)(*(s32 *)(linked + 0x18) + 0x110) ^ *(u32 *)(entry + 0x110)) & 0x600) == 0;
}

extern s32 D_00435DEC;

s32 btlHasFirstLinkedCategoryFlag1000(s32 actor) {
    s32 linked = *(s32 *)(actor + 0x114);
    s32 entry;
    u32 category;

    if (linked == 0) {
        return 0;
    }
    if (func_001E8058(*(s32 *)(linked + 0x60)) >= 2) {
        return 0;
    }
    entry = func_001E8060(*(s32 *)(linked + 0x60), 0);
    if ((*(u32 *)(entry + 0x110) & 0x400) == 0) {
        return 0;
    }
    category = *(u32 *)(entry + 0xC8);
    if (category >= 0x180) {
        return 0;
    }
    return btlHasFlag(*(u32 *)(D_00435DEC + category * 76), 0x1000);
}

u8 func_001EA940(s32 arg0) {
    return *(s32 *)(arg0 + 0x134) == 0x5f;
}

s32 btlMapActorCategory(s32 actor) {
    switch (*(u32 *)(actor + 0x134)) {
    case 0x09:
        return 0x29;
    case 0x12:
        return 0x51;
    case 0x1B:
        return 0x2B;
    case 0x24:
        return 0x4D;
    case 0x2D:
        return 0x3C;
    case 0x5B:
        return 0x45;
    case 0x5C:
        return 0x6E;
    case 0x5D:
        return 0xAA;
    default:
        return 0;
    }
}

s32 btlIsSpecialActorCategory(s32 actor) {
    switch (*(u32 *)(actor + 0x134)) {
    case 0x5B:
    case 0x5C:
    case 0x5D:
        return 1;
    default:
        return 0;
    }
}

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

void func_001EC3E8(u32 arg0) {
    if (!(*(u32 *)(arg0 + 0x110) & 0x10000)) {
        func_001F4D70(arg0, arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC418);

void func_001EC5F0(u32 arg0) {
    if (!(*(u32 *)(arg0 + 0x110) & 0x10000)) {
        func_001F4F10(arg0, arg0);
    }
}

void func_001EC620(u32 arg0) {
    func_001F5018(arg0, arg0);
}

void func_001EC638(u32 arg0) {
    btlAdvanceCommandCursor(arg0, arg0);
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

s32 func_001EC7E8(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook650;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

s32 func_001EC828(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook658;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC868);

void func_001ECBF8(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 27.5f);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ECC18);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ECCB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001ED008);

extern void func_001E88A8(u8 *);

void func_001ED300(u8 *fx) {
    BtlUnit *unit = *(BtlUnit **)(*(u8 **)(fx + 0x114) + 0x18);
    if (unit->flags & 2) {
        if (func_001E3168(unit, 1) == 0) {
            btlUnitGetMuzzlePosVU(unit);
        }
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(fx + 0xC0));
        func_001E88A8(fx + 0xC0);
    }
}

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

void func_001F2740(u8 *unit, f32 *vec) {
    func_001EC868(unit, vec, 0.0f);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2758);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2AE8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F2E30);

void func_001F3228(u32 arg0) {
    func_001F01E8(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F3240);

void func_001F34C8(u32 arg0) {
    func_001F3228(arg0);
}

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

INCLUDE_ASM(const s32, "game/code_001DACF8", btlAdvanceCommandCursor);

typedef struct {
    u8 pad[0x11C];
    u8 category;
} BattleActorLink;

typedef struct {
    u8 pad[0x18];
    BattleActorLink *primary;
} BattleActorLinks;

typedef struct {
    u8 pad[0x114];
    BattleActorLinks *links;
    BattleActorLink *secondary;
    BattleActorLink *tertiary;
} BattleActorLinkOwner;

BattleActorLink *btlFindActorLinkByCategory(BattleActorLinkOwner *actor, s32 category) {
    BattleActorLink *candidate = actor->links->primary;
    if (candidate->category == category) {
        return candidate;
    }
    candidate = actor->secondary;
    if (candidate != NULL && candidate->category == category) {
        return candidate;
    }
    candidate = actor->tertiary;
    if (candidate != NULL && candidate->category == category) {
        return candidate;
    }
    return actor->links->primary;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5320);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5780);

void func_001F5810(u32 unit, u8 mode) {
    s128 pos;
    switch (mode) {
    case 1:
        func_001E31F0(unit, 1);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&pos) : "memory");
        break;
    case 0:
    default:
        btlUnitGetMuzzlePosVU(unit);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&pos) : "memory");
        break;
    }
    __asm__ volatile("lqc2 vf10, 0(%0)" : : "r"(&pos) : "memory");
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004184D8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418568);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001F5868);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FA480);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FB908);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FBAC0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FC5E0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FD400);

BtlUnit *func_001FDCA8(s32 id) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & 0x200) {
                    if (unit->unk11C == id) {
                        return unit;
                    }
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDD20);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDE60);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FDF18);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FE068);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FE5C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FEC00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF0F8);

s32 func_001FF570(u32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            if (!(unit->flags & 0x20)) {
                count++;
            }
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF5D8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF820);

void func_001FF8F0(u32 arg0) {
    memset(D_003BD7D0, 0, 0x130);
    func_001F01E8(arg0, arg0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FF930);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFA08);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFAD8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFBB0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFC70);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFD30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFDD0);

extern void func_001F5868(s32, s32, s32, s32);
extern void func_001F5320(s32, s32, s32, s32);
extern s32 func_001FB908(s32, s32, s32, s32);

s32 func_001FFF68(s32 arg0, s32 arg1) {
    s32 result;
    memset(D_003BD7D0, 0, 0x130);
    func_001F5868(arg0, arg1, 2, 6);
    func_001F5320(arg0, arg1, 0, 0);
    result = func_001FB908(arg0, arg1, 0, 1);
    *(u16 *)(D_003BD7D0 + 0xC) = 2;
    return result;
}

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

s32 func_00200038(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->unk5B0 != 0) {
        if (work->unk5B4 != 0) {
            return 1;
        }
    }
    return 0;
}

void releaseBattleWorkBuffers(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->unk5B4 != 0) {
        func_00328470(work->unk5B4);
        work->unk5B4 = 0;
    }
    if (work->unk5B0 != 0) {
        func_00328470(work->unk5B0);
        work->unk5B0 = 0;
    }
}

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
    releaseBattleWorkBuffers();
    temp_v1 = func_001AA6F8();
    *(u32 *)(temp_v1 + 0x218) = *(u32 *)(temp_v1 + 0x218) & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418C58);

INCLUDE_ASM(const s32, "game/code_001DACF8", btlFreeFieldBlocks);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002001F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200290);

s32 func_00200490(s32 index) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->flags218 & 0x30000000) {
        return work->unk724;
    }
    return *(s32 *)((index << 5) + D_00435E30 + 0x18);
}

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

extern u32 btlBlendColor(u32, u32, f32);

void func_00200730(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    if (transition->soundId != 0) {
        transition->currentResource = btlBlendColor(transition->queuedResource, transition->previousResource,
                                                    (f32)transition->soundId / (f32)transition->queuedId);
        transition->soundId += 0xFFFF;
    } else {
        transition->currentResource = transition->queuedResource;
    }
    func_002005F0();
}

void func_002007B0(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    if (transition->currentResource & 0xFF000000) {
        func_0018F840(transition);
    }
}

void sndResetTransition(void) {
    SoundTransition *transition = (SoundTransition *)D_003BDCA0;
    D_00436AD4 = 0;
    D_00436AD8 = 0;
    transition->soundId = 0;
    transition->currentResource = 0;
}

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
        fldUpdateSwayOffset();
        func_00134A18();
    }
    func_002007B0();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200930);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002009E0);

void func_00200AD8(void) {
}

void func_00200AE0(void) {
    u8 *work = (u8 *)func_001AA6F8();
    if (*(s32 *)(work + 0x5C0) != 0) {
        func_0020D128("btl:rain exit\n");
        func_002DEBB0(*(s32 *)(work + 0x5C0));
        *(s32 *)(work + 0x5C0) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200B30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200D00);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200DA0);

extern u32 func_00200DA0(void);

SoundTask *func_00200F28(s32 arg0, s32 arg1) {
    SoundTask *task = func_001E1468(0x14);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 2;
    task->flags &= ~1;
    task->callback.poll = func_00200DA0;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->unk_08 = arg0;
    args->unk_0C = arg1;
    args->value = 0;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200FB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201108);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201268);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002014A8);

s64 func_00201520(void) {
    D_00436AD4 = 1;
    return func_00201268();
}

SoundTask *func_00201540(void) {
    SoundTask *task = (SoundTask *)func_002014A8();
    task->taskId = 7;
    task->callback.update = func_00201520;
    return task;
}

s32 func_00201578(u32 *arg0) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (!(work->flags218 & 0x20000000)) {
        func_002006B0(arg0[0], *(u16 *)(arg0 + 1));
    }
    D_00436AD8++;
    return 1;
}

SoundTask *sndCreateAcquireTask(s32 arg0, s32 arg1) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 5;
    task->callback.playSound = func_00201578;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0;
    args->option = arg1;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201650);

extern s32 func_00201650();

SoundTask *sndCreateReleaseTask(value)
    u32 value;
{
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 6;
    task->callback.poll = func_00201650;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

s64 func_00201718(void) {
    D_00436AD8 = 1;
    return func_00201650();
}

SoundTask *func_00201738(void) {
    SoundTask *task = (SoundTask *)sndCreateReleaseTask();
    task->taskId = 8;
    task->callback.update = func_00201718;
    return task;
}

void func_00201770(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0xc) < arg1) {
        *(s32 *)(arg0 + 0xc) = arg1;
    }
}

u32 sndGetResourceStatus(u32 *sound) {
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

s32 sndLookupResourceType(s32 arg0, s32 arg1) {
    s32 (*hook)(s32, s32) = ((BtlWork *)func_001AA6F8())->hook684;
    s32 type;
    if (hook != 0) {
        type = hook(arg0, arg1);
        if (type != -1) {
            return type;
        }
    }
    return *(u8 *)((arg1 << 5) + D_00435E30 + 3);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201828);

extern s32 func_001682B0(s32);

void sndCreateSystemEffect(u32 *effect) {
    if (effect[0] & 8) {
        if (effect[4] == 0) {
            if (effect[1] == 0) {
                effect[4] = func_001682B0(effect[5]);
                func_0020D128("btl:system effect create[%p]\n", effect[4]);
            }
        }
    }
}

void sndDeleteSystemEffect(u32 *effect) {
    if ((effect[0] & 8) && effect[4] && !effect[1]) {
        func_0020D128("btl:system effect delete[%p]\n", effect[4]);
        func_001683F0(effect[4]);
        effect[4] = 0;
    }
}

void sndAddEffectReferences(s32 *arg0) {
    s32 *effect;
    s32 *target;
    s32 *source;
    arg0[2] = 0;
    sndCreateSystemEffect(arg0[0]);
    effect = (s32 *)arg0[0];
    target = (s32 *)arg0[6];
    source = (s32 *)arg0[3];
    effect[1] = effect[1] + 1;
    source[0x334 / 4] = source[0x334 / 4] + 1;
    target[0x334 / 4] = target[0x334 / 4] + 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201C98);

void sndReleaseEffectReferences(s32 *arg0) {
    s32 *effect;
    s32 *target;
    s32 *source;
    if (arg0[2] != 0) {
        func_001686F0(arg0[2]);
    }
    effect = (s32 *)arg0[0];
    target = (s32 *)arg0[6];
    source = (s32 *)arg0[3];
    effect[1] = effect[1] - 1;
    source[0x334 / 4] = source[0x334 / 4] - 1;
    target[0x334 / 4] = target[0x334 / 4] - 1;
    sndDeleteSystemEffect((u32 *)effect);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201F08);

extern s32 func_00201F08(s32, s32, s32, s32);

s32 sndCreateEffectWithTargets(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 effect = func_00201F08(arg0, arg1, arg4, arg5 & 0xFFFF);
    SoundTaskArgs *args = func_001E14F8(effect);
    args->unk_10 = arg2;
    args->unk_14 = arg3;
    return effect;
}

void sndStartEffectTask(s32 *arg0) {
    s32 *effect;
    s32 *unit;
    arg0[1] = 0;
    sndCreateSystemEffect(arg0[0]);
    effect = (s32 *)arg0[0];
    unit = (s32 *)arg0[2];
    effect[1] = effect[1] + 1;
    unit[0x334 / 4] = unit[0x334 / 4] + 1;
}

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
    sndDeleteSystemEffect(temp_v0);
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

u32 func_002026E8(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit;
    if (work->flags218 & 0x40000000) {
        return 1;
    }
    for (unit = work->list24C; unit != 0; unit = unit->link364) {
    }
    return 1;
}

SoundTask *func_00202750(u32 value) {
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x33;
    task->callback.poll = func_002026E8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002027B8);

extern u32 func_002027B8(void);

SoundTask *func_00202840(u32 value) {
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x34;
    task->callback.poll = func_002027B8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

u32 func_002028A8(void) {
    func_001057A8();
    return 1;
}

SoundTask *func_002028C8(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_002028A8;
    task->taskId = 0x35;
    task->status = 0;
    return task;
}

void sndAddSourceReferences(s32 *arg0) {
    s32 *effect;
    s32 *unit;
    arg0[1] = 0;
    sndCreateSystemEffect(arg0[0]);
    effect = (s32 *)arg0[0];
    unit = (s32 *)arg0[2];
    effect[1] = effect[1] + 1;
    unit[0x334 / 4] = unit[0x334 / 4] + 1;
}

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
    sndDeleteSystemEffect(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202BB0);

u32 func_00202C70(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

SoundTask *func_00202C90(u32 value) {
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x38;
    task->callback.poll = func_00202C70;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

u32 func_00202CF8(u8 *arg0) {
    kwlnFadeInStart(*arg0, arg0[1], arg0[2], *(u32 *)(arg0 + 4));
    return 1;
}

extern u32 func_00202CF8(u8 *);

SoundTask *sndCreateCustomTask(s32 arg0, s32 arg1) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x39;
    task->callback.commandArgs = func_00202CF8;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0;
    args->option = arg1;
    return task;
}

u32 func_00202DA8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) | 0x40000;
    func_002DCC30();
    return 1;
}

SoundTask *sndCreateSetBattleFlagTask(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00202DA8;
    task->taskId = 0x3A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_00202E28(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffbffff;
    func_002DCC68();
    return 1;
}

SoundTask *sndCreateClearBattleFlagTask(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00202E28;
    task->taskId = 0x3B;
    task->onStart = 0;
    task->status = 0;
    return task;
}

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

s32 sndHasActiveActor(void) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->list24C; unit != 0; unit = unit->link364) {
        if (unit->node318 != 0 && func_00202F80((s32)unit->node318) != 0) {
            return 1;
        }
    }
    return 0;
}

SoundResourceNode *sndAllocResourceNode(void) {
    SoundResourceNode *node = func_00328E18(0x20);
    BtlWork *work;
    node->unk_04 = 0;
    node->unk_08 = 0;
    node->unk_0C = 0;
    node->resourceHandle = 0;
    work = (BtlWork *)func_001AA6F8();
    node->previous = 0;
    if (work->resourceList258 != 0) {
        work->resourceList258->previous = node;
        node->next = work->resourceList258;
    } else {
        node->next = 0;
    }
    work->resourceList258 = node;
    return node;
}

SoundResourceNode *sndCreateResourceNode(u32 soundId) {
    SoundResourceNode *node = (SoundResourceNode *)sndAllocResourceNode();
    node->resourceHandle = func_001682B0(soundId);
    node->flags |= 2;
    return node;
}

extern void func_001683F0(u32);
extern void func_00328E48(void *);

void sndFreeResourceNode(SoundResourceNode *node) {
    if (node->resourceHandle != 0) {
        func_001683F0(node->resourceHandle);
    }
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        ((BtlWork *)func_001AA6F8())->resourceList258 = node->next;
    }
    func_00328E48(node);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203138);

void func_00203258(void) {
    func_002DCB58();
}

void func_00203270(void) {
    effBTLFieldColorResetFlags();
    func_002D2CA8();
    D_00435CD4 = D_00435CD4 & 0xdfffffff;
}

void func_002032A8(void) {
    sndClearResourceNodes();
    func_002DCCA0();
    effBTLFieldColorResetFlags();
    func_002D2CA8();
}

INCLUDE_ASM(const s32, "game/code_001DACF8", sndClearResourceNodes);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203318);

SoundResourceLink *sndAllocResourceLink(u32 owner) {
    SoundResourceLink *link = func_00328E18(0x14);
    link->owner = owner;
    link->effectHandle = 0;
    link->flags = 0;
    link->effect = 0;
    return link;
}

void sndFreeResourceLink(SoundResourceLink *link) {
    if (link->effectHandle != 0) {
        func_001686F0(link->effectHandle);
        link->effect[1] = link->effect[1] - 1;
        sndDeleteSystemEffect(link->effect);
    }
    func_00328E48(link);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002034A8);

void func_002037F0(s32 arg0) {
    *(u8 *)(arg0 + 0x10) = 1;
}

SoundLink *sndAllocLink(u32 owner) {
    SoundLink *link = func_00328E18(0x10);
    link->owner = owner;
    link->effectHandle = 0;
    link->flags = 0;
    link->effect = 0;
    return link;
}

void sndFreeLink(SoundLink *link) {
    if (link->effectHandle != 0) {
        func_001686F0(link->effectHandle);
        link->effect[1] = link->effect[1] - 1;
        sndDeleteSystemEffect(link->effect);
    }
    func_00328E48(link);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203890);

u32 func_00203AB0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 0;
    return 1;
}

SoundTask *sndCreateClearStateTask(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00203AB0;
    task->taskId = 0x36;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_00203B20(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u8 *)(temp_v0 + 0x5b8) = 1;
    return 1;
}

SoundTask *sndCreateSetStateTask(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00203B20;
    task->taskId = 0x37;
    task->onStart = 0;
    task->status = 0;
    return task;
}

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

void refreshBattleSoundEntries(void) {
    u32 i;
    func_001AA6F8();
    for (i = 0; i < 0x31; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            func_00203D48(i, D_003BDE18[i].unk4);
        }
    }
}

void func_00203CD8(void) {
    SoundResourceNode **slots = (SoundResourceNode **)((u8 *)func_001AA6F8() + 0x4EC);
    u32 i;
    for (i = 0; i < 0x31; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            sndFreeResourceNode(slots[i]);
            slots[i] = 0;
        }
    }
}

void func_00203D48(s32 arg0, u32 arg1) {
    u32 temp_v0;
    s32 temp_v1;
    u32 *puVar3;

    temp_v1 = func_001AA6F8();
    puVar3 = (u32 *)sndAllocResourceNode();
    temp_v0 = *puVar3;
    puVar3[5] = arg1;
    *(u32 **)(arg0 * 4 + temp_v1 + 0x4ec) = puVar3;
    *puVar3 = temp_v0 | 10;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00203DA8);

extern void func_00232A88(s32);
extern void func_002D46F0(s32, f32 *);
extern void func_002D4398(s32);

void func_00203E18(s32 *args) {
    f32 pos[4];
    if (func_00332D48(*(s32 *)(args[1] + 0x18), 1) == 0) {
        func_00232A88(args[1]);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(pos) : "memory");
        pos[1] -= 150.0f;
    } else {
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(pos) : "memory");
    }
    func_002D46F0(args[0], pos);
    func_002D4398(args[0]);
}

void func_00203E90(u32 arg0) {
    func_002D4548(*(u32 *)arg0);
    func_00328E48(arg0);
}

void func_00203EC0(void) {
}

void func_00203EC8(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x58, 0x3f);
}

void func_00203EE8(u32 arg0) {
    sndSetSequenceVolumePan(arg0, 0x319c, 0x3f);
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
    mnuAdvanceTitleStateUnderSemaphore();
}

void func_00204050(void) {
    mnuAdvanceTitleStateUnderSemaphore();
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

s32 sndPlayStationedSe(u32 *sound) {
    u32 soundId = *sound;
    if (func_002040A8(soundId)) {
        func_0020D128("btl:sound stationedSE play[%X-%X]\n", soundId >> 16, soundId & 0xFFFF);
    }
    return 1;
}

SoundTask *sndCreateStationedSeTask(u32 value) {
    SoundTask *task = func_001E1468(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5A;
    task->callback.playSound = sndPlayStationedSe;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

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

SoundTask *sndCreateSkillSeTask(s32 *arg0, u16 arg1) {
    SoundTask *task = func_001E1468(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x57;
    task->callback.commandArgs = func_002041E8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0[2];
    args->optionId = arg1;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204358);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002043C0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204508);

s32 sndLoadDataFile(s32 *data) {
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
    return (s8)sdfSoundIsCommandBusy();
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
    func_0035C860(output, D_004192D8, D_00436AE8, (u16)(source + 0x200));
}

void func_002046F0(s32 unit, s32 output) {
    func_0035C860(output, D_004192E8, *(u16 *)(unit + 0x124));
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204718);

ActiveSoundNode *sndAllocListNode(void) {
    ActiveSoundNode *node = func_00328E18(0x14);
    BtlWork *work = (BtlWork *)func_001AA6F8();
    node->previous = 0;
    if (work->soundList != 0) {
        work->soundList->previous = node;
        node->next = work->soundList;
    } else {
        node->next = 0;
    }
    work->soundList = node;
    return node;
}

void sndFreeListNode(ActiveSoundNode *node) {
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        ((BtlWork *)func_001AA6F8())->soundList = node->next;
    }
    func_00328E48(node);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", sndClearList);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002048C8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192D8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192E8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419318);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00204958);

void *sndFindListNodeForChannel(s32 category, s32 id) {
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
    startBattleTask(temp_v0);
}

s32 sndHasActiveFileLoad(void) {
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

extern char D_00419408[]; /* "btl:sound load BSE SMG\n" */

void sndLoadBattleBank(void) {
    if (func_00342168(0x10000) == 0) {
        func_003421E8(0x10000);
        func_0020D128(D_00419408);
    }
}

u8 func_00205140(void) {
    s64 temp_v0;

    temp_v0 = func_00342168(0x10000);
    return temp_v0 != 0;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419408);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205160);

typedef struct SoundSlotOwner {
    u32 flags;
    u8 pad4[0x14];
    s32 slot[0x1D];
    u8 pad8C[0x78];
    struct SoundSlotOwner *next;
} SoundSlotOwner;

s32 sndHasOccupiedNodeSlots(void) {
    SoundSlotOwner *owner;
    u32 i;
    for (owner = ((BtlWork *)func_001AA6F8())->list260; owner != 0; owner = owner->next) {
        if (!(owner->flags & 2)) {
            for (i = 0; i < 0x1D; i++) {
                if (owner->slot[i] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

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

SoundTask *sndCreateEarringTask(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00205438;
    task->taskId = 0x5C;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002054B8);

extern u32 func_002054B8(void);

SoundTask *func_002055E0(s32 arg0) {
    SoundTask *task = func_001E1468(0xC);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5D;
    task->flags &= ~1;
    task->callback.poll = func_002054B8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->unk_08 = arg0;
    args->option = 0;
    args->value = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205660);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205758);

void sndFinishEarringPlaybackTask(s32 *arg0) {
    u8 *work = (u8 *)func_001AA6F8();
    if (arg0[2] != 0) {
        func_003297C8(arg0[2]);
    }
    *(u16 *)(work + 0x284) += 0xFFFF;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205890);

u32 func_00205930(void) {
    func_002040A8(0x1c);
    return 1;
}

SoundTask *func_00205950(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00205930;
    task->taskId = 0x5F;
    task->status = 0;
    return task;
}

u32 func_00205990(void) {
    func_00204038();
    return 1;
}

SoundTask *func_002059B0(void) {
    SoundTask *task = func_001E1468(0);
    task->enabled = 1;
    task->callback.poll = func_00205990;
    task->taskId = 0x60;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002059F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205CC8);

void func_00206060(void) {
    s128 v;
    PCP_COPY_VECTOR(&v, func_001AA6F8());
    func_002059F0(&v);
}

s64 func_00206090(void) {
    return func_00205CC8(0x400);
}

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

