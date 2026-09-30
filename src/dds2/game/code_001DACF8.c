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

typedef struct BtlLightSource {
    s128 vec0;
    s128 vec10;
    u8 pad20[0x20];
    s128 vec40;
} BtlLightSource;

typedef struct BtlExtModel {
    u8 pad0[0x18];
    BtlLightSource *light;
} BtlExtModel;

typedef struct BtlUnitExt {
    u8 pad0[0x84];
    BtlExtModel *model;
    u8 pad88[4];
    BtlUnitInfo *info;
    u8 pad90[0x18];
    u32 flagsA8;
} BtlUnitExt;

typedef struct BtlUnit BtlUnit;

typedef struct BtlWork {
    u8 pad0[0x180];
    u32 runtimeFlags;        /* 0x180 */
    void *activeSlot;        /* 0x184 */
    u8 pad188[0xC];
    u32 activeUnitId;        /* 0x194 */
    u8 pad198[0x10];
    s32 pendingSoundList; /* 0x1A8 */
    u8 pad1AC[0x5C];
    s32 unk208;
    u8 pad20C[0xC];
    u32 battleFlags;
    u32 flags21C;
    u32 flags220;           /* 0x220 */
    u8 pad224[4];
    s32 unk228;
    s32 unk22C;
    u8 pad230[0x18];
    BtlUnit *head;
    BtlUnit *actorList;
    struct SoundTask *taskList250;
    struct SoundTask *taskList254;
    struct SoundResourceNode *resourceList258;
    struct ActiveSoundNode *soundList;
    struct SoundSlotOwner *soundSlotOwners;
    u8 pad264[4];
    u16 unk268;
    u8 pad26A[0xE];
    s32 unk278;
    u8 pad27C[2];
    u16 unk27E;
    u8 pad280[4];
    u16 earringPlaybackCount; /* 0x284 */
    u8 pad286[2];
    s32 unk288;
    s32 unk28C;
    u8 pad290[0x3C];
    s32 unk2CC;
    u8 pad2D0[0x18];
    s32 moneyEarned;
    u8 pad2EC[8];
    s32 experienceEarned;
    u8 pad2F8[0x1CC];
    s8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    u8 pad4CC[0xB8];
    struct BtlBattleData *battleData; /* 0x584 */
    u8 pad588[0x28];
    s32 primaryBuffer;
    s32 secondaryBuffer;
    u8 fadeEnabled;
    u8 pad5B9[3];
    u32 fadeColor;
    s32 soundTransitionTask; /* 0x5C0 */
    u8 pad5C4[0x14];
    s32 (*hook5D8)(s32);
    u8 pad5DC[0x14];
    s32 (*hook5F0)(BtlUnit *, s32);
    u8 pad5F4[0x5C];
    s32 (*hook650)(BtlUnit *);
    u8 pad654[4];
    s32 (*hook658)(BtlUnit *);
    u8 pad65C[0x28];
    s32 (*hook684)(s32, s32);
    s32 (*hook688)(s32, s32);
    u8 pad68C[0x10];
    s32 (*hook69C)(BtlUnit *);
    s32 (*hook6A0)(BtlUnit *);
    u8 pad6A4[4];
    void (*hook)(BtlUnit *);
    u8 pad6AC[0x34];
    s32 (*hook6E0)(BtlUnit *);
    s32 (*hook6E4)(BtlUnit *);
    s32 (*hook6E8)(BtlUnit *);
    u8 pad6EC[0x24];
    s32 (*hook710)(BtlUnit *, s32);
    u8 pad714[8];
    u32 tint71C;
    u8 pad720[4];
    s32 unk724;
} BtlWork;

struct BtlUnit {
    s32 state;
    u8 pad4[4];
    u32 seqFlags;
    u8 padC[4];
    s32 stateTime;
    u8 pad14[4];
    BtlUnit *link18;
    u8 pad1C[0x14];
    f32 positionX;   /* 0x30: current unit position */
    f32 positionY;   /* 0x34 */
    f32 unk38;
    u8 pad3C[0x18];
    u32 baseColor;
    u8 pad58[0x2C];
    u32 overlayColor;
    f32 positionZOffset;
    u8 pad8C[0x38];
    s32 resourceKind;    /* 0xC4 */
    s32 resourceIndex;   /* 0xC8 */
    u8 unkCC;
    u8 padCD[0x1B];
    u32 unkE8;
    s32 unkEC;
    s32 unkF0;
    f32 fF4;
    u8 padF8[4];
    s32 effectIndex;     /* 0xFC */
    s32 effectParameter; /* 0x100 */
    f32 effectScale;     /* 0x104 */
    u64 owner;
    union {
        u64 flags64;
        struct {
            u32 flags;
            u32 stateFlags;
        };
    };
    s32 gunResourceFlags;
    u8 lookupId;
    u8 pad11D[3];
    u16 unk120;
    u8 pad122[2];
    u16 mode;
    u8 pad126[8];
    u16 unk12E;
    u8 pad130[0x42];
    u16 unk172;
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
    void *gunResource;
    u16 unk330;
    u8 pad332[2];
    s32 unk334;
    u8 pad338[4];
    s32 effectObject;
    BtlUnitExt *ext;
    s32 unk344;
    u8 pad348[4];
    s32 unk34C;
    s32 unk350;
    u8 pad354[8];
    u32 handle35C;
    BtlUnit *previousActor;
    BtlUnit *nextActor;
};

/* Command actor and its linked action/index state; distinct from BtlUnit. */
typedef struct BattleActionLinkState {
    u8 pad00[0x18];
    BtlUnit *unit;          /* 0x18 */
    u8 pad1C[0x44];
    s32 actorIndices;       /* 0x60 */
    u8 pad64[0x24];
    u8 *entries;            /* 0x88 */
} BattleActionLinkState;

typedef struct ActionUnit {
    u8 pad00[0x110];
    u32 flags;              /* 0x110 */
    BattleActionLinkState *link;  /* 0x114 */
    u8 pad118[0xC];
    u32 status;             /* 0x124 */
    s32 actionKind;         /* 0x128 */
    u8 pad12C[8];
    s32 category;           /* 0x134 */
    s32 actorIndices;       /* 0x138 */
} ActionUnit;

/* Metadata records reached through the battle table pointers. */
typedef struct BtlActionTableEntry {
    u8 kind;               /* 0x00 */
    u8 pad01[2];
    u8 resourceType;       /* 0x03 */
    u8 pad04[0x14];
    s32 defaultValue;      /* 0x18 */
    u16 flags;             /* 0x1C */
    u8 pad1E[2];
} BtlActionTableEntry;

typedef struct BtlCategoryTableEntry {
    u8 pad00[8];
    u8 restriction;        /* 0x08 */
    u8 flags09;            /* 0x09 */
    u8 pad0A[0x26];
    s32 categoryType;      /* 0x30 */
    u8 pad34[4];
} BtlCategoryTableEntry;

typedef struct BtlResourceTableEntry {
    u32 flags;
    u8 pad04[72];
} BtlResourceTableEntry;

typedef struct BtlIndexList {
    s32 capacity;       /* 0x00: allocated entry count */
    s32 count;          /* 0x04: live entry count */
    u32 *entries;       /* 0x08: points just past this header */
} BtlIndexList;

/* Pose command state: the progress slot is initialized as bits, then used as float. */
typedef struct BattlePoseBlendState {
    u8 pad00[0x130];
    s32 blendMode;           /* 0x130 */
    u8 pad134[0x18];
    union {
        s32 progressBits;
        f32 progress;
    };                      /* 0x14C */
    s32 durationFrames;     /* 0x150 */
    f32 duration;           /* 0x154 */
} BattlePoseBlendState;

typedef struct BtlStateHandler {
    void (*start)(void *);
    void (*update)(void *);
    void (*finish)(void *);
} BtlStateHandler;

extern BtlStateHandler D_003B69D8[];

typedef struct BtlFx {
    u8 pad0[0x50];
    f32 f50;
    u8 pad54[4];
    f32 f58;
    u8 pad5C[0x24];
    f32 f80;
    u8 pad84[4];
    f32 f88;
    u8 pad8C[4];
    union {
        s128 vec90;
        struct {
            f32 f90;
            f32 f94;
            f32 f98;
            f32 f9C;
        };
    };
    s128 vecA0;
    f32 fB0;
    f32 fB4;
    f32 fB8;
    f32 fBC;
    f32 fC0;
} BtlFx;

typedef struct BtlFxSrcA {
    f32 f0;
    f32 f4;
    f32 f8;
    f32 fC;
    f32 f10;
    f32 f14;
} BtlFxSrcA;

typedef struct BtlFxSrcB {
    s128 vec0;
    f32 f10;
    f32 f14;
    f32 f18;
    f32 f1C;
    f32 f20;
} BtlFxSrcB;

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

/* Two camera vectors are written at work+0x50 and work+0x60 by func_002001F0. */
typedef struct BtlCameraVectors {
    u8 pad00[0x50];
    f32 eye[3];
    u8 pad5C[4];
    f32 target[3];
} BtlCameraVectors;

typedef struct BtlCameraTaskArgs {
    u32 kind;
    f32 component[8]; /* camera origin/direction inputs, offsets 0x04..0x20 */
} BtlCameraTaskArgs;

typedef struct BtlVectorTaskArgs {
    u8 pad00[0x20];
    f32 scale;
    u32 state24;
    u32 state28;
    union {
        u32 unit2C;
        s8 mode2C;
    };
    u32 unit30;
} BtlVectorTaskArgs;

typedef struct BtlCommandOption {
    u8 pad00[0xC];
    s32 kind;           /* 0x0C */
    u8 pad10[4];
    u8 inactive;        /* 0x14 */
} BtlCommandOption;

typedef struct BtlCommandArgument {
    s32 command;        /* 0x00 */
    s32 index;          /* 0x04 */
    u8 pad08[0x38];
    s32 actorIndices;   /* 0x40 */
    u8 pad44[0x24];
    BtlCommandOption *option; /* 0x68 */
} BtlCommandArgument;

typedef struct BtlShapeResource {
    u8 pad00[0x14];
    s32 itemKind;       /* 0x14 */
    s32 itemIndex;      /* 0x18 */
} BtlShapeResource;

typedef struct BtlActiveSlot {
    u8 pad00[0x18];
    s32 unit;           /* 0x18 */
} BtlActiveSlot;

typedef struct BtlDeferredStats {
    BtlUnit *actor;    /* 0x00 */
    u8 pad04[0x1C];
    s32 primary;       /* 0x20 */
    s32 secondary;     /* 0x24 */
} BtlDeferredStats;

extern struct SoundTask *func_00205018();

extern u32 D_00436AD4;

extern u64 dds3AdvanceWorldCounter(void);

extern u32 evtSpawnActionObj9(u64);

extern s8 btlSetActorEffectParameter(BtlUnit *, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_0022F180(void);

extern u64 fldCreateSceneGroupAction(u64, u64, u64);

extern s32 func_001AA6F8(void);

extern struct SoundTask *D_00436A1C;

extern struct SoundTask *D_00436A20;

extern s32 sdfGraphHasPendingWorkInterruptSafe(void);

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
    u8 unk_26[2];
    s32 startDelay;
    s32 endDelay;
    u32 unk_30;
    u32 unk_34;
    u64 unk_38;
    u64 owner;
    void (*onStart)(u32);
    s32 (*callback)();
    void (*onFinish)(u32 *);
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
    s32 fadeCountdown;
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

extern void func_001E15F0(SoundTask *);

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
        f32 scale;
    };
    u32 unk_08;
    union {
        u32 unk_0C;
        f32 scale2;
        s8 mode;
    };
    u32 unk_10;
    union {
        u32 unk_14;
        f32 scale14;
    };
    union {
        u32 unk_18;
        struct {
            u8 flag18;
            u8 flag19;
        };
    };
    u32 unk_1C;
} SoundTaskArgs;

extern SoundTask *btlAllocTask(s32);

extern SoundTaskArgs *func_001E14F8(s32);

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern u8 D_0037F510[];

extern char D_004178A8[];

extern char D_004178B8[];

extern char D_00417AF0[];

extern char D_00417B10[];

extern void func_001E1BB8(u8 *, u32, u32);

extern char D_00417B30[];

extern s32 func_001E2110(u8 *, u32, u32);

extern void func_001E8258(s32, s32, s32, s32, s32);

extern void func_001E96C8(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void effMiscQuaternionToMatrixVU(void);

extern void func_001E9890(void);

extern u8 D_003E9130[];

extern s32 D_003BBF70[];

extern s32 D_003BBF88[];

extern void func_001FA480(s32, s32, s32);

extern void func_001FBAC0(s32, s32);

typedef struct SoundCursor {
    u16 unk_00;
    u16 frame;
    s16 mode;
    s16 index;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
} SoundCursor;

#define CURSOR ((SoundCursor *)D_003BD7D0)

extern char D_00418C58[];

extern char D_00418C70[]; /* "btl:free field F2\n" */

extern char D_00418C88[]; /* "btl:free field F1\n" */

extern void sdfFreeMemoryFromEitherHeap(void *);

extern s32 sndHasActiveFileLoad(void);

extern s32 fileGetResourceSize(s32);

extern void func_003422F8(s32, s32);

typedef struct BattleFieldBlocks {
    u8 unk_00[0x290];
    s32 fieldF1;
    s32 fieldF2;
    s32 fieldF3;
} BattleFieldBlocks;

#define VU_LOAD10(p) __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

#define VU_STORE10(p) __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

extern f32 *D_0037F770[];

extern u8 D_0037F780[];

extern void fldApplyLightSetCurrent(void);

extern f32 *D_0037F770[];

extern s32 func_00202F60(s32, u16);

extern u32 func_002D4138(u32);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DACF8);

void func_001DB048(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB050);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB258);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB440);

void func_001DB518(BtlUnit *unit) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *owner = unit->link18;
    u8 *task;
    if (!(owner->flags & 0x200)) {
        if (func_001B3200(owner) == 0) {
            task = (u8 *)btlCreateEffObjB(unit->link18, 0x67);
            task[0] = 0xA;
            *(u16 *)(task + 8) = 0x43;
            btlStartTask(task);
            if (unit->link18->unk12E & 0x480) {
                btlDispatchStateHandler(unit, 0x19);
                return;
            }
            btlDispatchStateHandler(unit, 0x1B);
            return;
        }
        work->unk27E += 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DB5E0);

void func_001DBE20(s32 *arguments) {
    s32 owner = arguments[0x34 / 4];
    s32 value = btlCreateEffObjA(owner, arguments[0x20 / 4]);
    btlStartTask(value);
    value = func_001E6428(owner, 1);
    *(s32 *)(value + 0x28) = 7;
    btlStartTask(value);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DBE70);

void func_001DC278(BtlUnit *unit) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    work->unk278 = work->unk278 + 1;
    if (func_001B2AF8(unit->link18) != 0) {
        work->battleFlags |= 0x2000;
    } else {
        work->battleFlags |= 0x1000;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC2D8);

void func_001DC538(void) {
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DC540);

void func_001DC7F0(void) {
}

void func_001DC7F8(u64 arg0) {
    u64 task;

    task = fldCreateSceneGroupAction(arg0, 0x1194, 1);
    btlStartTask(task);
    btlDispatchStateHandler(arg0, 0x1b);
}

void func_001DC838(void) {
}

void func_001DC840(BtlUnit *unit) {
    if (btlCountTasksForOwner(*(u64 *)((u8 *)unit->link18 + 0x108)) == 0) {
        btlDispatchStateHandler(unit, 0x1D);
    }
}

void func_001DC888(void) {
}

void func_001DC890(BtlUnit *unit) {
    BtlUnit *owner = unit->link18;
    if (btlCountTasksForOwner(*(u64 *)((u8 *)owner + 0x108)) == 0) {
        owner->flags &= ~0x4000;
        btlDispatchStateHandler(unit, 2);
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
    ((BtlUnit *)((BtlUnit *)arg0)->link18)->flags =
        ((BtlUnit *)((BtlUnit *)arg0)->link18)->flags | 0x4000;
}

void func_001DCE70(BtlUnit *unit) {
    void (*hook)(BtlUnit *) = ((BtlWork *)func_001AA6F8())->hook;
    if (hook != 0) {
        hook(unit);
    }
    btlDispatchStateHandler(unit, 0x1B);
}

void func_001DCEB0(void) {
}

void func_001DCEB8(void) {
}

void func_001DCEC0(void) {
    func_0022F068();
}

void func_001DCED8(u32 arg0) {
    s64 status;

    status = func_0022F180();
    if (status == 0) {
        btlDispatchStateHandler(arg0, 6);
        return;
    }
}

void btlDispatchStateHandler(s32 *obj, s32 kind) {
    obj[0] = kind;
    obj[4] = 0;
    D_003B69D8[kind].start(obj);
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417508);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417518);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417528);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417538);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417548);

BtlUnit *btlCreateActionSeq(void) {
    BtlUnit *seq = func_00328E18(0x180);
    BtlWork *work;
    *(u16 *)((u8 *)seq + 4) = 1;
    func_001DF7B8((u8 *)seq + 0x20);
    work = (BtlWork *)func_001AA6F8();
    seq->prev = 0;
    if (work->head != 0) {
        work->head->prev = seq;
        seq->next = work->head;
    } else {
        seq->next = 0;
    }
    work->head = seq;
    btlDispatchStateHandler(seq, 0);
    func_0020D128("btl:action seq create[%p]\n", seq);
    return seq;
}

void btlDestroyActionSeq(BtlUnit *unit) {
    func_0020D128("btl:action seq delete[%p]\n", unit);
    btlReleaseObjectBuffers((s32 *)((u8 *)unit + 0x20));
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

void btlUpdateActionSeqs(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)func_001AA6F8())->head; unit != 0; unit = next) {
        next = unit->next;
        if (unit->seqFlags & 1) {
            D_003B69D8[unit->state].update(unit);
            unit->stateTime++;
        } else if (unit->seqFlags & 2) {
            btlDestroyActionSeq(unit);
        }
    }
}

void btlDestroyAllActionSeqs(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)func_001AA6F8())->head; unit != 0; unit = next) {
        next = unit->next;
        btlDestroyActionSeq(unit);
    }
}

BtlUnit *btlFindUnitByActor(BtlUnit *actor) {
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

s32 func_001DD628(BtlUnit *unit, s32 *argument) {
    s32 value;
    switch (argument[0]) {
    case 1:
        if ((unit->flags64 & 0x1200) == 0x200 && (unit->unk120 & 0x10) == 0) {
            return func_001AC0E0(unit->unk172);
        }
        if (argument[1] > 0) {
            return argument[1];
        }
        value = func_001B5688();
        if (value > 0) {
            return value;
        }
        return 0;
    case 4:
        return func_001AC098(argument[2]);
    case 2:
    case 3:
    case 7:
    case 8:
        return argument[1];
    default:
        return -1;
    }
}

u32 func_001DD6D8(BtlUnit *unit, u8 *argument) {
    switch (*(s32 *)argument) {
    case 1: {
        u32 count = func_001E8058(((BtlCommandArgument *)argument)->actorIndices);
        if ((unit->flags & 0x200) && ((unit->flags & 0x1000) || (unit->unk120 & 0x10)) &&
            (unit->unk12E & 0x1000) == 0 && count == 1) {
            u8 *option = *(u8 **)(argument + 0x68);
            if (((BtlCommandOption *)option)->kind == 2 && option[0x14] == 0) {
                return 0x17;
            }
        }
        return 3;
    }
    case 4:
        return (unit->flags & 0x200) ? 0xC : 4;
    case 2:
    case 3:
    case 7:
    case 8: {
        s32 index = ((BtlCommandArgument *)argument)->index;
        if (index == 0xD6 && (unit->flags64 & 0x1200) == 0x200 && (unit->unk120 & 0x10) == 0) {
            return 0xC;
        }
        return ((BtlActionTableEntry *)D_00435E30)[index].kind;
    }
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD810);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DD9A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DDAD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DDB60);

typedef struct BattleIndexEntry {
    u8 unk0;
    u8 pad1[7];
    s32 unk8;
    u8 padC[4];
    u8 unk10;
    u8 pad11[3];
    u8 unk14;
    u8 pad15[0x587];
} BattleIndexEntry;

typedef struct BattleIndexWork {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 unk24[9];
    u8 unk2D;
    u8 unk2E;
    u8 pad2F;
    u16 unk30;
    u8 pad32[2];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    void *indices;
    u32 unk44;
    u64 previous;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u16 unk5C;
    u8 pad5E[2];
    s32 unk60;
    u8 unk64;
    u8 pad65[3];
    u32 device;
    u32 command;
} BattleIndexWork;

void func_001DF700(BattleIndexWork *work) {
    u32 i;
    s32 offset;
    BattleIndexEntry *entry;
    BattleIndexEntry *next;
    work->unk00 = -1;
    work->unk04 = -1;
    work->unk08 = -1;
    work->unk0C = 0;
    work->unk10 = 0;
    work->unk14 = 0;
    work->unk18 = -1;
    work->unk1C = 0;
    work->unk20 = 8;
    work->unk30 = 0;
    work->unk34 = 0;
    work->unk38 = -1;
    work->unk3C = 0;
    work->unk2D = 0;
    work->unk2E = 0;
    work->unk50 = 0;
    work->unk54 = 0;
    work->unk58 = 0;
    work->unk5C = 0;
    work->unk64 = 0;
    work->unk60 = 0;
    for (i = 0, offset = 0; i < 13; i++) {
        *(u8 *)(offset + work->device) = 0;
        entry = (BattleIndexEntry *)(offset + work->device);
        entry->unk8 = 0;
        entry->unk14 = 0;
        next = (BattleIndexEntry *)(offset + work->device);
        offset += 0x59C;
        next->unk10 = 0;
    }
    func_001E8050(work->indices);
}

extern void *btlAllocateIndexList(s32);

extern u32 func_003292A8(s32);

extern u32 sdfResourceRetainAddress(u32);

void func_001DF7B8(BattleIndexWork *work) {
    u32 command;
    work->indices = btlAllocateIndexList(13);
    command = func_003292A8(0x48EC);
    work->device = sdfResourceRetainAddress(command);
    work->command = command;
    work->previous = 0;
    func_001DF700(work);
}

void btlReleaseObjectBuffers(s32 *object) {
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

extern u32 func_001DF860(s32);

typedef struct TaskBlock {
    u32 word[11];
} TaskBlock;

SoundTask *func_001DFB08(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x49;
    task->owner = unit->owner;
    task->callback = func_001DF860;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 btlApplyDeferredActorStats(u8 *arguments) {
    s32 context = func_001AA6F8();
    u8 *actor = *(u8 **)arguments;
    s32 primary;
    u8 *resource;
    if ((((BtlWork *)context)->battleFlags & 0x80) == 0) {
        return 1;
    }
    primary = ((BtlDeferredStats *)arguments)->primary;
    if (primary == 0 && ((BtlDeferredStats *)arguments)->secondary == 0) {
        return 1;
    }
    if (((BtlUnit *)actor)->flags & 0x60) {
        return 1;
    }
    resource = actor + 0x120;
    func_001AA770(resource, primary);
    func_001AA788(resource, ((BtlDeferredStats *)arguments)->secondary);
    func_001E2758(actor);
    func_001B2430(actor, 0);
    return 1;
}

SoundTask *func_001DFC80(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4A;
    task->owner = unit->owner;
    task->callback = btlApplyDeferredActorStats;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 func_001DFD58(void *arg) {
    s32 *args = arg;
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit = (BtlUnit *)args[0];
    if (!(work->battleFlags & 0x80)) {
        return 1;
    }
    func_001AA850((u8 *)unit + 0x120, args[1]);
    func_001E2758(unit);
    func_001B2430(unit, 0);
    return 1;
}

SoundTask *func_001DFDC0(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4B;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001DFD58;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001DFE48);

extern u32 func_001DFE48(s32);

SoundTask *func_001DFF48(BtlUnit *unit, u32 arg1, u32 arg2) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001DFE48;
    task->taskId = 0x4C;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->unk_08 = arg1;
    args->option = arg2;
    return task;
}

s32 func_001DFFE0(s32 arg0) {
    s32 args;

    args = arg0;
    func_001ADFE0(*(s32 *)args, *(s32 *)(args + 0x14), *(s16 *)(args + 0x18));
    return 1;
}

SoundTask *func_001E0010(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4D;
    task->owner = unit->owner;
    task->callback = func_001DFFE0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 func_001E00E8(u32 *arg0) {
    if (0 < (s32)arg0[7]) {
        func_001AA880(*arg0, arg0[7]);
        func_001E2758(*arg0);
    }
    return 1;
}

SoundTask *func_001E0128(BtlUnit *unit, TaskBlock *block) {
    SoundTask *task = btlAllocTask(0x30);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x4E;
    task->owner = unit->owner;
    task->callback = func_001E00E8;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = (s32)unit;
    *(TaskBlock *)&args->option = *block;
    return task;
}

u32 func_001E0200(u32 *arg0) {
    func_001AA888(*arg0);
    func_001E2758(*arg0);
    return 1;
}

SoundTask *func_001E0238(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E0200;
    task->taskId = 0x4F;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E02A8);

extern u32 func_001E02A8(s32);

SoundTask *func_001E05B8(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x50;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E02A8;
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
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x51;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E0640;
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
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x52;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E0738;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->optionId = option;
    return task;
}

/* Task payload shared by the experience and money reward callbacks. */
typedef struct BattleRewardPacket {
    BtlUnit *actor;
    s32 amount;
} BattleRewardPacket;

u32 btlAddEpFromPacket(s32 packetAddress) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->flags & 0x400) {
        return 1;
    }
    work->experienceEarned += packet->amount;
    func_0020D128("btl:epall=%d[%d](packet)\n", work->experienceEarned, packet->amount);
    return 1;
}

extern u32 btlAddEpFromPacket(s32);

SoundTask *func_001E0858(void *actor, s32 amount) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x53;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = btlAddEpFromPacket;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = amount;
    return task;
}

u32 btlAddMoneyFromPacket(s32 packetAddress) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BattleRewardPacket *packet = (BattleRewardPacket *)packetAddress;
    if (packet->amount == 0) {
        return 1;
    }
    if (packet->actor->flags & 0x400) {
        return 1;
    }
    work->moneyEarned += packet->amount;
    func_0020D128("btl:money=%d[%d](packet)\n", work->moneyEarned, packet->amount);
    return 1;
}

extern u32 btlAddMoneyFromPacket(s32);

SoundTask *func_001E0950(void *actor, s32 amount) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x54;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = btlAddMoneyFromPacket;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = amount;
    return task;
}

extern s32 D_00435DEC;

u32 btlRefreshEligibleActors(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit = work->actorList;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 0x400) {
            if (flags & 1) {
                if ((flags & 0xE0) == 0 && (u16)(unit->mode - 1) < 0x17F) {
                    u32 entry = ((BtlResourceTableEntry *)D_00435DEC)[unit->mode].flags;
                    if ((entry & 0x40) == 0) {
                        if ((entry & 0x400) == 0) {
                            if ((unit->stateFlags & 8) == 0) {
                                u16 prior = unit->unk12E;
                                func_001AA850((u8 *)unit + 0x120, 1);
                                func_001E2758(unit);
                                if (unit->unk12E == 1 && prior != unit->unk12E) {
                                    unit->stateFlags |= 4;
                                    work->flags21C |= 0x100;
                                }
                            }
                        }
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

SoundTask *func_001E0B08(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = btlRefreshEligibleActors;
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
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x56;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E0B50;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

void btlUpdateAutoMusic(void) {
    u8 *context = (u8 *)func_001AA6F8();
    u32 flags = ((BtlWork *)context)->battleFlags;
    if ((flags & 0x100000) == 0 || (flags & 0x6000000) == 0x6000000 ||
        (flags & 0x800) != 0) {
        return;
    }
    if (flags & 0x8000) {
        if ((s8)D_0037F510[0x22] < 0 || (s8)D_0037F510[0x23] < 0) {
            ((BtlWork *)context)->battleFlags = flags & ~0x8000;
            sndSetSequenceVolumePan(6, 0x7F, 0x3F);
            func_001B8278(0);
            func_0020D128(D_004178A8);
        }
    } else if ((s8)D_0037F510[0x22] < 0) {
        ((BtlWork *)context)->battleFlags = flags | 0x8000;
        sndSetSequenceVolumePan(5, 0x7F, 0x3F);
        func_001B8278(1);
        func_0020D128(D_004178B8);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E0CE0);

void func_001E1100(void) {
}

SoundTask *btlFindTaskByHandle(u64 value) {
    SoundTask *task;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = task->next) {
        if (task->unk_38 == value) {
            return task;
        }
    }
    return 0;
}

SoundTask *btlFindTaskByOwner(u64 owner) {
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
    s32 task;
    s32 count;

    task = func_001AA6F8();
    count = 0;
    for (task = (s32)((BtlWork *)task)->taskList254; task != 0; task = (s32)((SoundTask *)task)->next) {
        count = count + 1;
    }
    return count;
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

void btlFlagTasksForUpdate(void) {
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

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004178A8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004178B8);

typedef struct SoundCondition {
    u8 kind;
    u8 pad1[7];
    union {
        s32 count;
        u64 key;
        u16 kindId;
    };
} SoundCondition;

extern SoundTask *btlFindTaskByHandle(u64);
extern SoundTask *btlFindTaskByOwner(u64);
extern SoundTask *btlFindTaskByKind(u16);

s32 func_001E1368(SoundCondition *cond, s32 value) {
    s32 result = 0;
    SoundTask *task;
    switch (cond->kind) {
    case 0:
        break;
    case 1:
        result = 1;
        break;
    case 2:
        if (!(value < cond->count)) {
            result = 1;
        }
        break;
    case 3:
        if (btlFindTaskByHandle(cond->key) != 0) {
            result = 1;
        }
        break;
    case 4:
        if (btlFindTaskByHandle(cond->key) == 0) {
            result = 1;
        }
        break;
    case 5:
        task = btlFindTaskByHandle(cond->key);
        if (task != 0) {
            if (task->unk_22 == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 6:
        if (btlFindTaskByOwner(cond->key) != 0) {
            result = 1;
        }
        break;
    case 7:
        if (btlFindTaskByOwner(cond->key) == 0) {
            result = 1;
        }
        break;
    case 8:
        task = btlFindTaskByOwner(cond->key);
        if (task != 0) {
            if (task->unk_22 == 2) {
                result = 1;
            }
        } else {
            result = 1;
        }
        break;
    case 9:
        if (btlFindTaskByKind(cond->kindId) != 0) {
            result = 1;
        }
        break;
    case 10:
        result = btlFindTaskByKind(cond->kindId) == 0;
        break;
    }
    return result;
}

extern void *func_00328E18(s32);

SoundTask *btlAllocTask(s32 size) {
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

void btlFreeTask(SoundTask *task) {
    BtlWork *work;
    if (task->onFinish != 0) {
        task->onFinish((u32 *)task->args);
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

u64 btlStartTask(task)
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

void func_001E15F0(SoundTask *task) {
    u32 counter;
    if (!(task->flags & 8)) {
        return;
    }
    if (task->flags & 4) {
        btlFreeTask(task);
        return;
    }
    counter = task->unk_30;
    task->unk_30 = counter + 1;
    switch (task->unk_22) {
    case 0:
        if (func_001E1368(task, counter) == 0) {
            break;
        }
        task->unk_22 = 1;
    case 1:
        if (task->startDelay <= 0) {
            task->unk_22 = 2;
        } else {
            task->startDelay = task->startDelay - 1;
            break;
        }
    case 2:
        if (func_001E1368((u8 *)task + 0x10, task->unk_34) != 0) {
            task->unk_22 = 3;
        } else if (task->callback(task->args) != 0) {
            task->unk_22 = 3;
        } else {
            task->unk_34 = task->unk_34 + 1;
            break;
        }
    case 3:
        if (task->endDelay <= 0) {
            btlFreeTask(task);
        } else {
            task->endDelay = task->endDelay - 1;
        }
        break;
    }
}

void btlSweepFinishedTasks(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)func_001AA6F8())->taskList254; task != 0; task = next) {
        next = task->next;
        if (!(task->flags & 2)) {
            func_001E15F0(task);
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
        func_001E15F0(node);
        node = next;
    }
    D_00436A1C = 0;
    D_00436A20 = 0;
}

void btlClearTaskLists(void) {
    SoundTask *task;
    SoundTask *next;
    for (task = ((BtlWork *)func_001AA6F8())->taskList250; task != 0; task = next) {
        next = task->nextActive;
        btlFreeTask(task);
    }
    D_00436A1C = 0;
    D_00436A20 = 0;
}

u32 func_001E1848(void) {
    return 1;
}

SoundTask *func_001E1850(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E1848;
    task->taskId = 0x6A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

void btlDumpTaskQueue(void) {
    s32 context = func_001AA6F8();
    s32 node = (s32)((BtlWork *)context)->taskList254;
    while (node != 0) {
        func_0020D128("btl:packet[%d]\n", ((SoundTask *)node)->taskId);
        node = (s32)((SoundTask *)node)->next;
    }
    func_0020D128("btl:packet head[%p]\n", *(void **)(context + 0x250));
    func_0020D128("btl:packet tail[%p]\n", *(void **)(context + 0x254));
}

void btlInitUnitFxDefaults(BtlFx *fx) {
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

void btlInitFxLights(BtlFxLights *fx) {
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

extern void *func_001AC020(s32, s32);

void func_001E19C8(BtlFx *fx, s32 arg1, s32 arg2) {
    BtlFxSrcA *alt = func_001AC020(arg1, arg2);
    BtlFxSrcB *base = (BtlFxSrcB *)func_001ABFD8(arg1, arg2);
    if (alt->fC == 0.0f) {
        PCP_COPY_VECTOR(&fx->vec90, base);
        fx->fB4 = base->f18;
        fx->fB0 = base->f1C;
        fx->fC0 = base->f20;
    } else {
        fx->f90 = alt->f0;
        fx->f94 = alt->f4;
        fx->f98 = alt->f8;
        fx->f9C = 0.0f;
        fx->fB4 = alt->f10;
        fx->fB0 = alt->f14;
    }
    PCP_COPY_VECTOR(&fx->vecA0, base);
    fx->fBC = base->f18;
    fx->fB8 = base->f1C;
    fx->fC0 = base->f20;
    fx->f80 = base->f10;
    fx->f50 = base->f10;
    fx->f88 = base->f14;
    fx->f58 = base->f14;
}

s32 btlHasMatchingModel(s32 effect, s32 model) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit = work->actorList;
    while (unit != NULL) {
        if ((unit->flags & 2) != 0 &&
            unit->ext != NULL &&
            unit->unk328 != 0 &&
            func_00232EE8((s32)unit->ext->info) == effect &&
            func_00232EF8((s32)unit->ext->info) == model) {
            return 1;
        }
        unit = unit->nextActor;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1B80);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E1BB8);

extern void sndReleaseSlotOwner(struct SoundSlotOwner *);
extern void sdfReleaseDevSlot(s32, s32, s32);
extern void func_00110B50(s32);
extern char D_00417940[]; /* "btl:unit transparency delete[%p]\n" */

void btlReleaseActorModelResources(BtlUnit *unit) {
    if (unit->unkCC == 0) {
        if (unit->unk328 != 0) {
            sndReleaseSlotOwner((struct SoundSlotOwner *)unit->unk328);
            unit->unk328 = 0;
        }
        if (unit->unk344 != 0) {
            sdfReleaseDevSlot(unit->unk344, 1, 1);
            unit->unk344 = 0;
            func_0020D128(D_00417940, unit);
        }
        if (unit->effectObject != 0) {
            func_00110B50(unit->effectObject);
            unit->effectObject = 0;
            unit->ext = 0;
        }
    } else {
        unit->unk328 = 0;
        unit->unk344 = 0;
        unit->effectObject = 0;
        unit->ext = 0;
    }
    unit->gunResourceFlags &= ~1;
    unit->flags &= ~2;
    unit->gunResourceFlags &= ~2;
}

void func_001E2058(u32 arg0, u32 arg1, u32 arg2) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        func_0022CD60(arg1, arg2);
        return;
    }
    mdlRequestAsset(arg1, arg2, 0);
}

void func_001E20B8(u32 arg0, u32 arg1, u32 arg2) {
    s64 available;

    available = mdlFlagTest(0xc0f);
    if (available != 0) {
        func_0022CB68(arg1, arg2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2110);

void btlFlagUnitDefeatCandidate(BtlUnit *unit) {
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

s32 func_001E2B60(BtlUnit *unit) {
    BtlWork *work;
    if (!(unit->flags & 2)) {
        return 0;
    }
    work = (BtlWork *)func_001AA6F8();
    if (work->hook5D8 != 0 && work->hook5D8(0) == unit->unkEC) {
        return 1;
    }
    switch (unit->unkEC) {
    case 0:
    case 2:
    case 9:
    case 10:
    case 11:
        return 1;
    default:
        return 0;
    }
}

extern s32 func_001ABFD8(s32, s32);

extern void func_001E22D8(u8 *, s32, s32, f32);

void func_001E2C00(u8 *unit, s32 index, s32 arg2, f32 scale) {
    u8 *table = (u8 *)func_001ABFD8(((BtlUnit *)unit)->resourceKind, ((BtlUnit *)unit)->resourceIndex);
    func_001E22D8(unit, index, arg2, *(f32 *)(table + index * 0x14 + 0x34) * scale);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E2C78);

void btlUpdateUnitEffects(void) {
    s32 context = func_001AA6F8();
    u8 *object = *(u8 **)(context + 0x24C);

    while (object != 0) {
        if (((BtlUnit *)object)->flags & 2) {
            u8 *resource = (u8 *)func_001ABFD8(((BtlUnit *)object)->resourceKind,
                                                  ((BtlUnit *)object)->resourceIndex);
            s32 model = (s32)((BtlUnit *)object)->ext->info;
            s32 node = mdlGetNodeField2C(model, 0);
            if (*(s16 *)(resource + node * 20 + 0x30) == 1 &&
                func_001E2B60(object) == 0) {
                func_001E2758(object);
                func_001E22D8(object, ((BtlUnit *)object)->effectIndex,
                              ((BtlUnit *)object)->effectParameter,
                              ((BtlUnit *)object)->effectScale);
            }
        }
        object = *(u8 **)(object + 0x364);
    }
}

void func_001E2DA0(u8 *object) {
    s32 context;
    u8 *resource;
    f32 volume;
    if ((((BtlUnit *)object)->flags & 2) == 0) {
        return;
    }
    context = func_001AA6F8();
    ((BtlUnit *)object)->unkE8 &= ~1;
    volume = ((BtlUnit *)object)->fF4;
    resource = *(u8 **)(object + 0x340);
    *(f32 *)(*(u8 **)(*(u8 **)(resource + 0x8C) + 0x1C) + 0x20) =
        volume * (30.0f / (f32)((BtlWork *)context)->unk4C4);
}

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
    if ((((BtlUnit *)arg0)->flags & 2) != 0) {
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

extern s32 func_003343E8(BtlUnitData *, f32);

void func_001E2F78(BtlUnit *unit) {
    if (unit->flags & 2) {
        func_003343E8(unit->ext->info->data, 0.0f);
    }
}

extern u32 effMiscRandMod(s32, s32);

s64 btlSeekRandomModelFrame(BtlUnit *unit) {
    s32 count;
    f32 amount;
    if (unit->flags & 2) {
        count = func_001E2F50(unit);
        if (count > 0) {
            amount = effMiscRandMod(0, count);
            return func_003343E8(unit->ext->info->data, amount);
        }
    }
}

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

void btlSetUnitPosition(BtlUnit *unit, f32 *vec) {
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
            pos[2] += unit->positionZOffset;
            effObjSetInnerFirstVec(unit->effectObject, pos);
        }
    }
}

void func_001E3108(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x60);
}

void btlGetUnitWorldPos(u8 *unit, s128 *dst) {
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

extern void btlUpdateUnitEffectVectors(BtlUnit *);

s8 btlSetActorEffectParameter(BtlUnit *unit, s32 mode) {
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlWork *)func_001AA6F8())->hook5F0;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlUpdateUnitEffectVectors(unit);
    return func_00332D48(unit->ext->info->unk18, mode);
}

void func_001E31F0(BtlUnit *arg0, s32 mode) {
    if (btlSetActorEffectParameter(arg0, mode) == 0) {
        btlUnitGetMuzzlePosVU(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3230);

extern s32 func_00332D08(s32, s32);

s8 btlSetActorAlternateEffectParameter(unit, mode)
    BtlUnit *unit;
    s32 mode;
{
    s32 (*hook)(BtlUnit *, s32);
    if (!(unit->flags & 2)) {
        return 0;
    }
    hook = ((BtlWork *)func_001AA6F8())->hook5F0;
    if (hook != 0) {
        mode = hook(unit, mode);
    }
    btlUpdateUnitEffectVectors(unit);
    return func_00332D08(unit->ext->info->unk18, mode);
}

void func_001E33A8(void) {
    if (btlSetActorAlternateEffectParameter() == 0) {
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
    if (((BtlUnit *)object)->positionX == position[0] &&
        ((BtlUnit *)object)->positionY == position[1] &&
        ((BtlUnit *)object)->unk38 == position[2]) {
        return 1;
    }
    return 0;
}

extern u8 D_004179E0[];

extern void effMiscQuatMultiplyVU(void);

extern void effObjSetInnerSecondVec(s32, f32 *);

void btlSetUnitRotation(BtlUnit *unit, s128 *quat) {
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
            effObjSetInnerSecondVec(unit->effectObject, result);
        }
    }
}

void func_001E34D8(u8 *unit, s128 *dst) {
    PCP_COPY_VECTOR(dst, unit + 0x70);
}

extern void func_0023C978(BtlUnitExt *, s32, u32);

void btlSetUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        unit->baseColor = (unit->baseColor & 0xFF000000) | (color & 0xFFFFFF);
        func_0023C978(unit->ext, mode, color);
    }
}

void btlBlendUnitColor(BtlUnit *unit, u32 color, s32 mode) {
    u32 base;
    u32 blended;
    if (unit->flags & 2) {
        color = (color & 0xFFFFFF) | 0x80000000;
        base = (unit->baseColor & 0xFFFFFF) | 0x80000000;
        blended = (base & color) + (((base ^ color) & 0xFEFEFEFE) >> 1);
        unit->overlayColor = (unit->overlayColor & 0xFF000000) | (color & 0xFFFFFF);
        func_0023C978(unit->ext, mode, blended);
    }
}

void func_001E35F8(BtlUnit *unit, u32 value) {
    mdlReleaseInnerResourceHandle(unit->ext->info, (value & 0xFFFFFF) | 0x80000000);
}

extern void effObjFetchInnerFirstVec(s32);

extern void effObjFetchInnerSecondVecNorm(s32);

extern void mdlStorePrimaryVectorVU(BtlUnitInfo *);

extern void func_00232AD0(BtlUnitInfo *);

extern void sdfModelUpdateCurrentFrameTransforms(s32);

void btlUpdateUnitEffectVectors(BtlUnit *unit) {
    if (!(unit->flags & 2)) {
        return;
    }
    effObjFetchInnerFirstVec(unit->effectObject);
    mdlStorePrimaryVectorVU(unit->ext->info);
    effObjFetchInnerSecondVecNorm(unit->effectObject);
    func_00232AD0(unit->ext->info);
    sdfModelUpdateCurrentFrameTransforms(unit->ext->info->unk18);
}

extern s32 func_002091C8(s128 *, s128 *);

extern void btlUnitGetBodyPosVU(BtlUnit *);

extern void btlSetUnitRotation(BtlUnit *, s128 *);

void btlUnitFaceTarget(BtlUnit *unit, BtlUnit *target) {
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
            btlSetUnitRotation(unit, &hit);
        }
    }
}

extern s32 func_00209258(s128 *, s128 *, f32);

void func_001E3720(BtlUnit *unit, BtlUnit *target, f32 scale) {
    s128 from;
    s128 to;
    s128 hit;
    if (unit->flags & 0x80000) {
        btlUnitGetBodyPosVU(unit);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&from));
        btlUnitGetBodyPosVU(target);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&to));
        func_00209258(&from, &to, scale);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(&hit));
        btlSetUnitRotation(unit, &hit);
    }
}

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

typedef struct BtlUnitStats {
    u32 word[0x71];
} BtlUnitStats;

void func_001E3810(s32 arg0, s32 arg1) {
    BtlUnitStats *stats = (BtlUnitStats *)(arg0 + 0x120);
    *stats = *(BtlUnitStats *)arg1;
    func_001AA7A0(stats);
    func_001AA7F0(stats);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E38F0);

extern s32 sdfModelCreateWithItems(s32, s32);
extern void dds3SetObjectFlags(s32, s32);

void btlCreateUnitTransparency(BtlUnit *unit) {
    u8 *shape;
    if ((unit->flags & 2) == 0) {
        return;
    }
    if (unit->unk344 != 0) {
        return;
    }
    if (unit->unkCC != 0) {
        return;
    }
    shape = *(u8 **)((u8 *)unit->ext->info + 0xC);
    unit->unk344 = sdfModelCreateWithItems(((BtlShapeResource *)shape)->itemKind, ((BtlShapeResource *)shape)->itemIndex);
    dds3SetObjectFlags(unit->effectObject, 1);
    func_0020D128("btl:unit transparency create[%p]\n", unit);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3CB8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E3E20);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4028);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E40F0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4378);

u32 func_001E44A8(u8 *arguments) {
    s32 index = ((SoundTaskArgs *)arguments)->option;
    if (index >= 0) {
        func_001E2C00(*(u8 **)arguments, index, ((SoundTaskArgs *)arguments)->unk_08,
                        ((SoundTaskArgs *)arguments)->scale2);
    }
    return 1;
}

SoundTask *func_001E44E0(BtlUnit *unit, s32 index, s32 value, f32 scale) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 9;
    task->callback = func_001E44A8;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->scale2 = scale;
    return task;
}

u32 func_001E4588(u32 *arg0) {
    func_001E2DA0(*arg0);
    return 1;
}

SoundTask *func_001E45A8(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E4588;
    task->taskId = 0xA;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

u32 btlPollThresholdTask(s32 *arguments) {
    if ((s32)func_001E2EE8((BtlUnit *)arguments[0]) >= arguments[1]) {
        if ((((BtlUnit *)arguments[0])->unkE8 & 1) == 0) {
            func_001E2E20((BtlUnit *)arguments[0]);
        }
        return 1;
    }
    return 0;
}

SoundTask *btlScheduleThresholdTask(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0xB;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = btlPollThresholdTask;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4700);

extern u32 func_001E4700(u32 *);

SoundTask *func_001E4908(BtlUnit *unit, s32 index, f32 scale) {
    SoundTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E4700;
    task->taskId = 0xE;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = index;
    args->scale2 = scale;
    args->unk_08 = 0;
    args->unk_14 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E49A8);

extern u32 func_001E49A8(u32 *);

SoundTask *func_001E4A90(BtlUnit *unit, f32 *target, f32 scale) {
    SoundTask *task = btlAllocTask(0x30);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xC;
    task->owner = unit->owner;
    task->callback = func_001E49A8;
    task->onStart = 0;
    args = (u8 *)func_001E14F8((s32)task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->unit2C = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x60);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E4B40);

extern u32 func_001E4B40(u32 *);

SoundTask *func_001E4C30(BtlUnit *unit, f32 *target, s8 mode, f32 scale) {
    SoundTask *task = btlAllocTask(0x34);
    u8 *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0xD;
    task->owner = unit->owner;
    task->callback = func_001E4B40;
    task->onStart = 0;
    args = (u8 *)func_001E14F8((s32)task);
    ((BtlVectorTaskArgs *)args)->scale = scale;
    ((BtlVectorTaskArgs *)args)->mode2C = mode;
    ((BtlVectorTaskArgs *)args)->unit30 = (u32)unit;
    ((BtlVectorTaskArgs *)args)->state24 = 0;
    ((BtlVectorTaskArgs *)args)->state28 = 0;
    PCP_COPY_VECTOR(args, (u8 *)unit + 0x70);
    PCP_COPY_VECTOR(args + 0x10, target);
    return task;
}

void btlRequestModelOrReuse(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->flags & 2) != 0) {
        return;
    }
    if (btlHasMatchingModel(effect, model)) {
        func_001E1BB8(object, effect, model);
        if (*(char *)(arguments + 3) == 0) {
            func_001E2220(object);
            func_0023CA60((u32)((BtlUnit *)object)->ext, 0, 0);
            ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
        }
        func_0020D128(D_00417AF0, effect, model);
    } else {
        func_001E2058(object, effect, model);
        ((BtlUnit *)object)->gunResourceFlags |= 1;
        func_0020D128(D_00417B10, effect, model);
    }
}

u32 btlPollModelLoadCompletion(u32 *arguments) {
    u8 *object = (u8 *)arguments[0];
    u32 effect = arguments[1];
    u32 model = arguments[2];
    if ((((BtlUnit *)object)->flags & 2) == 0) {
        if (!func_001E2110(object, effect, model)) {
            return 0;
        }
        func_001E1BB8(object, effect, model);
        func_001E20B8(object, effect, model);
        func_0020D128(D_00417B30, effect, model, object);
    }
    if (*(s8 *)(arguments + 3) == 0) {
        func_001E2220(object);
        func_0023CA60((u32)((BtlUnit *)object)->ext, 0, 0);
        ((BtlUnit *)object)->overlayColor = ((BtlUnit *)object)->baseColor & 0xFFFFFF;
    }
    ((BtlUnit *)object)->gunResourceFlags = (((BtlUnit *)object)->gunResourceFlags & ~1) | 2;
    return 1;
}

SoundTask *func_001E4EE0(BtlUnit *unit, u32 index, u32 value, s8 mode) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x18;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))btlRequestModelOrReuse;
    task->callback = btlPollModelLoadCompletion;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = index;
    args->unk_08 = value;
    args->mode = mode;
    return task;
}

u32 func_001E4FA0(u32 *arg0) {
    func_001E2220(*arg0);
    btlReleaseActorModelResources(*arg0);
    return 1;
}

SoundTask *btlScheduleRefreshTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E4FA0;
    task->taskId = 0x19;
    task->owner = unit->owner;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417AF0);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417B10);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417B30);

s32 btlBeginModelChange(u32 *arguments) {
    s32 owner = arguments[0];
    u32 model = arguments[1];
    u32 variant = arguments[2];
    s32 status = btlHasMatchingModel(model, variant);

    if (status == 0) {
        func_001E2058(owner, model, variant);
        *(u32 *)(owner + 0x118) = (*(u32 *)(owner + 0x118) | 1) & ~2;
        return func_0020D128("btl:model change start[%X,%X]\n", model, variant);
    }
    return status;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E50E0);

extern u32 func_001E50E0(u32 *);

SoundTask *btlCreateModelChangeTask(BtlUnit *unit, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5) {
    SoundTask *task = btlAllocTask(0x1C);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x1A;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))btlBeginModelChange;
    task->callback = func_001E50E0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = arg1;
    args->unk_08 = arg2;
    args->unk_0C = arg3;
    args->unk_10 = arg4;
    args->flag19 = arg5;
    args->flag18 = 0;
    args->unk_14 = 0;
    return task;
}

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

SoundTask *btlCreateUnitTask0F(BtlUnit *unit, s32 arg1, s32 arg2, s32 arg3) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0xF;
    task->owner = unit->owner;
    task->onStart = func_001E5870;
    task->callback = func_001E58A0;
    args = func_001E14F8((s32)task);
    args->unk_0C = (u32)unit;
    args->value = arg1;
    args->option = arg2;
    args->unk_08 = arg3;
    return task;
}

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

SoundTask *btlCreateUnitTask10(BtlUnit *unit, f32 *vec, s32 arg2) {
    SoundTask *task = btlAllocTask(0x18);
    FxTask *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x10;
    task->owner = unit->owner;
    task->onStart = (void (*)(u32))func_001E59A0;
    task->callback = func_001E59D0;
    args = (FxTask *)func_001E14F8((s32)task);
    args->unit = unit;
    args->unk10 = arg2;
    PCP_COPY_VECTOR(args, vec);
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5AB0);

extern u32 func_001E5AB0(u32 *);

SoundTask *func_001E5C08(BtlUnit *unit, u32 value, u32 variant) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E5AB0;
    task->taskId = 0x11;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->unk_10 = 0x80808080;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5CB0);

extern u32 func_001E5CB0(u32 *);

SoundTask *func_001E5DA8(BtlUnit *unit, u32 value, u32 variant) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E5CB0;
    task->taskId = 0x12;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = value;
    args->unk_08 = variant;
    args->unk_0C = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E5E40);

extern u32 func_001E5E40(s32);

SoundTask *func_001E5FF8(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x13;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E5E40;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6080);

extern u32 func_001E6080(s32);

SoundTask *func_001E61A0(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x14;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E6080;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6228);

extern u32 func_001E6228(s32);

SoundTask *func_001E6428(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x15;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E6228;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    args->unk_08 = 0;
    return task;
}

typedef struct BtlBattleData {
    u8 pad0[0x14];
    s32 unk14;
} BtlBattleData;

typedef struct BtlEffectHandle {
    u8 pad0[0xC];
    u16 flags;
} BtlEffectHandle;

typedef struct UnitEffectTaskArgs {
    BtlUnit *unit;
    s32 effectId;
    BtlEffectHandle *effect;
    s32 duration;
    s32 counter;
} UnitEffectTaskArgs;

extern s32 func_001682B0(s32);
extern BtlEffectHandle *func_00168548(s32, s32, BtlUnit *, s32);
extern void effBattleUpdateSelectedValue(BtlEffectHandle *, s32);
extern void func_00168978(BtlEffectHandle *);

s32 func_001E64B0(UnitEffectTaskArgs *args) {
    BtlUnit *unit = args->unit;
    BtlWork *work;
    if (!(unit->flags & 2)) {
        return 1;
    }
    work = (BtlWork *)func_001AA6F8();
    if (args->effect == 0) {
        s32 handle = work->battleData->unk14;
        unit->flags |= 0x80;
        args->effectId = func_001682B0(handle);
        args->effect = func_00168548(args->effectId, 2, unit, 0);
        args->duration = 0xE;
        args->effect->flags &= 0xFFF9;
        effBattleUpdateSelectedValue(args->effect, 0xE);
        unit->flags &= ~8;
        if (unit->flags & 2) {
            unit->ext->info->flags |= 1;
        }
    }
    args->counter = args->counter + 1;
    if (args->counter >= args->duration) {
        unit->flags = (unit->flags & ~0x80) | 0x40;
        return 1;
    }
    func_00168978(args->effect);
    return 0;
}

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

SoundTask *func_001E6620(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x16;
    task->flags |= 2;
    task->owner = unit->owner;
    task->callback = func_001E64B0;
    task->onFinish = func_001E65C0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->unk_08 = 0;
    args->unk_10 = 0;
    args->unk_0C = 0;
    return task;
}

u32 func_001E66B8(void) {
    func_00208F78();
    return 1;
}

SoundTask *func_001E66D8(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E66B8;
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
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->taskId = 0x1C;
    task->flags |= 2;
    task->status = 0;
    task->onStart = 0;
    task->callback = func_001E6720;
    return task;
}

u32 func_001E6790(void) {
    return 1;
}

SoundTask *func_001E6798(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E6790;
    task->taskId = 0x20;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E67E0);

extern u32 func_001E67E0(u32 *);

SoundTask *func_001E68F8(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E67E0;
    task->taskId = 0x21;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6970);

extern u32 func_001E6970(u32 *);

SoundTask *func_001E6B70(BtlUnit *unit, f32 value) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x1D;
    task->callback = func_001E6970;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->scale = value;
    args->unk_08 = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E6BF8);

extern u32 func_001E6BF8(u32 *);

SoundTask *func_001E6E18(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E6BF8;
    task->taskId = 0x1E;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 func_001E6E90(s32 *arg0) {
    s32 unit;

    unit = *arg0;
    *(u32 *)(unit + 0x110) = *(u32 *)(unit + 0x110) & 0xffffffef;
    btlSetUnitRotation(unit, unit + 0x40);
    return 1;
}

SoundTask *btlScheduleActorUpdate(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E6E90;
    task->taskId = 0x1F;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

u32 func_001E6F38(u32 *arg0) {
    btlUpdateUnitEffectVectors(*arg0);
    return 1;
}

SoundTask *func_001E6F58(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E6F38;
    task->taskId = 0x22;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

extern s32 func_001E4028(BtlUnit *, char *);
extern s32 func_002C80E8(char *);

void btlStartGunFinishLoad(s32 *task) {
    char filename[0x70];
    BtlUnit *unit = *(BtlUnit **)task;
    if (unit->flags & 0x400) {
        return;
    }
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
    }
    if (func_001E4028(unit, filename)) {
        s32 handle = func_002C80E8(filename);
        task[1] = handle;
        func_0020D128("btl:gun & finish load start[%s][%p]\n", filename, handle);
    }
    unit->gunResourceFlags = (unit->gunResourceFlags | 4) & ~8;
}

extern s32 func_002C8128(s32);

extern s32 fileGetResourceHandle(s32);

extern void func_002C7D00(s32);

typedef struct GunLoadArgs {
    BtlUnit *unit;
    s32 handle;
} GunLoadArgs;

u32 btlPollGunLoad(s32 arg) {
    GunLoadArgs *args = (GunLoadArgs *)arg;
    BtlUnit *unit = args->unit;
    if (args->handle == 0) {
        return 1;
    }
    if (func_002C8128(args->handle) == 0) {
        return 0;
    }
    func_0020D128("btl:gun & finish load end[%p]\n", args->handle);
    unit->gunResource = sdfResourceRetainAddress(fileGetResourceHandle(args->handle));
    func_002C7D00(args->handle);
    unit->gunResourceFlags = (unit->gunResourceFlags & ~4) | 8;
    return 1;
}

SoundTask *func_001E70F8(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x23;
    task->flags &= ~1;
    task->owner = unit->owner;
    task->onStart = btlStartGunFinishLoad;
    task->callback = btlPollGunLoad;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = 0;
    return task;
}

u32 func_001E7188(void) {
    btlUpdateUnitEffects();
    return 1;
}

SoundTask *func_001E71A8(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E7188;
    task->taskId = 0x24;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 btlCreateActorTransparency(u32 *task) {
    BtlUnit *unit = *(BtlUnit **)task;
    if (!(unit->flags & 2)) {
        return 0;
    }
    btlCreateUnitTransparency(unit);
    (*(BtlUnit **)task)->flags |= 0x20000;
    return 1;
}

extern u32 btlCreateActorTransparency(u32 *);

SoundTask *func_001E7248(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x25;
    task->callback = btlCreateActorTransparency;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E72B0);

extern u32 func_001E72B0(u32 *);

SoundTask *btlCreateActorModelBlendTask(BtlUnit *unit, u32 target, u32 index, u32 value, f32 scale) {
    SoundTask *task = btlAllocTask(0x1C);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E72B0;
    task->taskId = 0x26;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = target;
    args->unk_08 = index;
    args->unk_10 = value;
    args->scale14 = scale;
    args->unk_0C = -1;
    args->unk_18 = 0;
    return task;
}

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
        btlSetUnitRotation(*(BtlUnit **)arg0, hit);
    }
    return 1;
}

SoundTask *func_001E74A0(void *actor, s32 option) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x27;
    task->owner = *(u64 *)((u8 *)actor + 0x108);
    task->callback = func_001E7438;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = actor;
    args->option = option;
    return task;
}

u32 func_001E7528(u32 *arg0) {
    btlFlagUnitDefeatCandidate(*arg0);
    return 1;
}

SoundTask *func_001E7548(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E7528;
    task->taskId = 0x28;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

u32 func_001E75B8(u32 *arg0) {
    func_001E2220(*arg0);
    return 1;
}

SoundTask *func_001E75D8(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_001E75B8;
    task->taskId = 0x29;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7648);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E7960);

void btlResetUnitLinks(BtlUnit *unit) {
    unit->unk310 = -1;
    unit->unk314 = -1;
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->gunResourceFlags = 0;
    unit->unk330 = 0;
    func_001ADC48(unit);
    unit->link31C = sndAllocResourceLink(unit);
    unit->link320 = sndAllocLink(unit);
}

extern s64 func_001A9920(void);

extern void *memset(void *, s32, u32);

BtlUnit *btlCreateUnit(void) {
    u32 handle = func_003292A8(0x368);
    BtlUnit *unit = (BtlUnit *)sdfResourceRetainAddress(handle);
    BtlWork *work;
    memset(unit, 0, 0x368);
    unit->handle35C = handle;
    unit->owner = func_001A9920();
    unit->flags = 0;
    unit->stateFlags = 0;
    unit->lookupId = unit->unk310 = -1;
    unit->unk2E4 = 6;
    unit->gunResourceFlags = 0;
    unit->unk334 = 0;
    unit->node318 = 0;
    unit->gunResource = 0;
    unit->effectObject = 0;
    unit->ext = 0;
    btlInitUnitFxDefaults((BtlFx *)unit);
    btlInitFxLights((BtlFxLights *)unit);
    btlResetUnitLinks(unit);
    work = (BtlWork *)func_001AA6F8();
    unit->previousActor = 0;
    if (work->actorList != 0) {
        work->actorList->previousActor = unit;
        unit->nextActor = work->actorList;
    } else {
        unit->nextActor = 0;
    }
    work->actorList = unit;
    func_0020D128("btl:unit create[%p]\n", unit);
    return unit;
}

extern void sndFreeResourceNode(struct SoundResourceNode *);

extern void sndFreeResourceLink(struct SoundResourceLink *);

extern void sndFreeLink(struct SoundLink *);

extern void sdfFreeMemoryFromEitherHeap(void *);

extern void func_003298C0(s32);

extern void btlReleaseActorModelResources(BtlUnit *);

extern void sndFreeListNode(struct ActiveSoundNode *);

void btlReleaseUnitResources(BtlUnit *unit) {
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
    if (unit->gunResource != 0) {
        sdfFreeMemoryFromEitherHeap(unit->gunResource);
        unit->gunResource = 0;
        unit->gunResourceFlags &= ~4;
        unit->gunResourceFlags &= ~8;
    }
    if (unit->node324 != 0) {
        sndFreeListNode(unit->node324);
        unit->node324 = 0;
    }
    btlReleaseActorModelResources(unit);
    if (unit->unk350 != 0) {
        func_003298C0(unit->unk350);
        unit->unk350 = 0;
        unit->unk34C = 0;
    }
}

extern void func_003297C8(u32);

void btlDestroyUnit(BtlUnit *unit) {
    func_0020D128("btl:unit delete[%p]\n", unit);
    btlReleaseUnitResources(unit);
    if (unit->nextActor != 0) {
        unit->nextActor->previousActor = unit->previousActor;
    }
    if (unit->previousActor != 0) {
        unit->previousActor->nextActor = unit->nextActor;
    } else {
        ((BtlWork *)func_001AA6F8())->actorList = unit->nextActor;
    }
    func_003297C8(unit->handle35C);
}

void btlDestroyAllUnits(void) {
    BtlUnit *unit;
    BtlUnit *next;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = next) {
        next = unit->nextActor;
        btlDestroyUnit(unit);
    }
}

void btlRemoveActorsWithFlags(u32 mask) {
    s32 actor = (s32)((BtlWork *)func_001AA6F8())->actorList;
    s32 next;
    while (actor != 0) {
        next = (s32)((BtlUnit *)actor)->nextActor;
        if (((BtlUnit *)actor)->flags & mask) {
            btlDestroyUnit(actor);
        }
        actor = next;
    }
}

BtlUnit *btlFindActorForOwner(u64 owner) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->owner == owner) {
            return unit;
        }
    }
    return 0;
}

s32 btlIsActiveActor(BtlUnit *actor) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit == actor) {
            return 1;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeClear(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
        if (!(unit->unk120 & 0x20) && unit->mode == mode) {
            return unit;
        }
    }
    return 0;
}

BtlUnit *btlFindUnitByModeFlagged(s32 mode) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
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
    ((BtlIndexList *)list)->count = 0;
    return list;
}

void func_001E8018(s32 list) {
    func_00328E48(list);
}

void func_001E8030(s32 arg0, u32 arg1) {
    s32 index;

    index = ((BtlIndexList *)arg0)->count;
    ((BtlIndexList *)arg0)->count = index + 1;
    ((BtlIndexList *)arg0)->entries[index] = arg1;
}

void func_001E8050(s32 arg0) {
    ((BtlIndexList *)arg0)->count = 0;
}

u32 func_001E8058(s32 arg0) {
    return ((BtlIndexList *)arg0)->count;
}

u32 func_001E8060(s32 arg0, s32 arg1) {
    return ((BtlIndexList *)arg0)->entries[arg1];
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
    s32 *entries;
    s32 first;
    s32 second;

    if (arg1 == arg2) {
        return;
    }
    entries = *(s32 **)(arg0 + 8);
    first = entries[arg1];
    second = entries[arg2];
    entries[arg1] = second;
    entries[arg2] = first;
}

u32 btlFindListIndex(s32 arg0, s32 value) {
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

s32 btlStepPoseBlendHalf(u8 *fx) {
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        ((BattlePoseBlendState *)fx)->progressBits = 0;
        func_00209720(fx + 0x160, (f32)(((BattlePoseBlendState *)fx)->durationFrames * 2));
        func_001E9598((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    if (t > 0.5f) {
        t = 0.5f;
    }
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t + t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.5f <= t) {
        return 1;
    }
    return 0;
}

extern void func_002096B8(u8 *, f32);

extern f32 func_002096C8(u8 *);

extern void func_001E8580(XformData *, XformData *, XformData *, f32);

s32 btlStepPoseBlend(u8 *fx) {
    u8 *timer = fx + 0x158;
    u8 *pose = fx + 0x30;
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        func_002096B8(timer, ((BattlePoseBlendState *)fx)->duration);
        func_001E9598((XformData *)fx, (XformData *)pose);
    }
    t = func_002096C8(timer);
    func_001E8580((XformData *)fx, (XformData *)pose, (XformData *)(fx + 0xC0), t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

extern void func_001E8580(XformData *, XformData *, XformData *, f32);

extern void func_00209720(u8 *, f32);

extern f32 func_00209770(f32);

s32 btlStepPoseBlendFrame(u8 *fx) {
    f32 t;
    if (((BattlePoseBlendState *)fx)->blendMode == 0) {
        ((BattlePoseBlendState *)fx)->progressBits = 0;
        func_00209720(fx + 0x160, (f32)((BattlePoseBlendState *)fx)->durationFrames);
        func_001E9598((XformData *)fx, (XformData *)(fx + 0x30));
        return 0;
    }
    t = func_00209770(1.0f);
    func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), t);
    ((BattlePoseBlendState *)fx)->progress = t;
    if (0.999999f <= t) {
        return 1;
    }
    return 0;
}

s32 func_001E8840(u8 *fx) {
    f32 ratio = (f32)((BattlePoseBlendState *)fx)->blendMode / (f32)((BattlePoseBlendState *)fx)->durationFrames;
    if (ratio <= 1.0f) {
        func_001E8580((XformData *)fx, (XformData *)(fx + 0x30), (XformData *)(fx + 0xC0), ratio);
        return 0;
    }
    func_001E9598((XformData *)fx, (XformData *)(fx + 0xC0));
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E88A8);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E89E0);

u32 func_001E8B08(u32 *arg0) {
    func_001E8258(arg0[3], *arg0, arg0[1], arg0[2], arg0[4]);
    return 1;
}

SoundTask *btlCreateCommandSoundTask(s32 actor, s32 mode) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;

    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2A;
    if (actor != 0 && ((BtlUnit *)actor)->link18 != 0) {
        task->owner = ((BtlUnit *)actor)->link18->owner;
    }
    task->callback = func_001E8B08;
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
    func_001E9660(context + 0x70, ((BtlCameraTaskArgs *)arguments)->component[0], ((BtlCameraTaskArgs *)arguments)->component[1],
                    ((BtlCameraTaskArgs *)arguments)->component[2], ((BtlCameraTaskArgs *)arguments)->component[3],
                    ((BtlCameraTaskArgs *)arguments)->component[4], ((BtlCameraTaskArgs *)arguments)->component[5],
                    ((BtlCameraTaskArgs *)arguments)->component[6], ((BtlCameraTaskArgs *)arguments)->component[7]);
    return 1;
}

SoundTask *btlCreateFloatTask28(BtlUnit *actor, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h) {
    SoundTask *task = btlAllocTask(0x24);
    f32 *args;
    task->enabled = 1;
    task->taskId = 0x2B;
    task->status = 0;
    if (actor != 0 && actor->link18 != 0) {
        task->owner = actor->link18->owner;
    }
    task->callback = func_001E8C88;
    task->onStart = 0;
    args = (f32 *)func_001E14F8((s32)task);
    *(BtlUnit **)args = actor;
    args[1] = a;
    args[2] = b;
    args[3] = c;
    args[4] = d;
    args[5] = e;
    args[6] = f;
    args[7] = g;
    args[8] = h;
    return task;
}

s32 func_001E8E08(f32 *args) {
    s32 context = func_001AA6F8();
    func_001E8258(1, *(s32 *)args, 0, 0, 0);
    func_001E96C8(context + 0x70, args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8],
                  args[9], args[10], args[11], args[12], args[13], args[14], args[15], args[16]);
    return 1;
}

SoundTask *btlCreateFloatTask29(BtlUnit *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    SoundTask *task = btlAllocTask(0x44);
    f32 *args;
    task->enabled = 1;
    task->taskId = 0x2C;
    task->status = 0;
    if (actor != 0 && actor->link18 != 0) {
        task->owner = actor->link18->owner;
    }
    task->callback = func_001E8E08;
    task->onStart = 0;
    args = (f32 *)func_001E14F8((s32)task);
    *(BtlUnit **)args = actor;
    args[1] = a1;
    args[2] = a2;
    args[3] = a3;
    args[4] = a4;
    args[5] = a5;
    args[6] = a6;
    args[7] = a7;
    args[8] = a8;
    args[9] = a9;
    args[10] = a10;
    args[11] = a11;
    args[12] = a12;
    args[13] = a13;
    args[14] = a14;
    args[15] = a15;
    args[16] = a16;
    return task;
}

u32 func_001E9008(u32 arg0) {
    s32 work;

    work = func_001AA6F8();
    func_001E8E08(arg0);
    ((BtlWork *)work)->runtimeFlags |= 0x80000;
    return 1;
}

SoundTask *func_001E9058(BtlUnit *actor, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, f32 a8, f32 a9, f32 a10, f32 a11, f32 a12, f32 a13, f32 a14, f32 a15, f32 a16) {
    SoundTask *task = btlCreateFloatTask29(actor, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
    task->callback = func_001E9008;
    return task;
}

u32 func_001E90C0(void) {
    s32 work;

    work = func_001AA6F8();
    func_001E9B80(work + 0x70);
    return 1;
}

SoundTask *btlScheduleContextReset(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_001E90C0;
    task->taskId = 0x2D;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9130);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9410);

void btlClearPendingSoundList(void) {
    s32 context = func_001AA6F8();
    s32 list = ((BtlWork *)context)->pendingSoundList;

    if (list != 0) {
        func_001E8018(list);
        ((BtlWork *)context)->pendingSoundList = 0;
    }
    ((BtlWork *)context)->battleFlags &= ~0x10;
}

void func_001E9598(XformData *dst, XformData *src) {
    PCP_COPY_VECTOR(&dst->vec0, &src->vec0);
    PCP_COPY_VECTOR(&dst->vec1, &src->vec1);
    dst->f20 = src->f20;
    dst->f24 = src->f24;
}

void func_001E95C8(s32 arg0, f32 arg1) {
    ((XformData *)arg0)->f24 = arg1;
}

void func_001E95D0(u8 *object, f32 *origin, f32 *direction) {
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(direction));
    effMiscQuaternionToMatrixVU();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(D_003E9130));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddz.xyzw vf10, vf30, vf10z\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(1.0f));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        ".set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(origin));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw vf10, vf10, vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(object) : "memory");
    ((XformData *)object)->f20 = 1.0f;
    ((XformData *)object)->f24 = 0.6981317f;
    func_001E9890();
}

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
    ((XformData *)object)->f24 = scale * 0.017453293f;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E96C8);

u32 func_001E9798(void) {
    s32 workAddress;

    workAddress = func_001AA6F8();
    return ((BtlWork *)workAddress)->activeUnitId;
}

f32 func_001E97B8(u8 *unit) {
    return ((BattlePoseBlendState *)unit)->progress;
}

s32 btlIsUnitInActiveList(s32 unit) {
    u8 *work = (u8 *)func_001AA6F8();
    u8 *slot = (u8 *)((BtlWork *)work)->activeSlot;
    u32 count;
    u32 i;
    if (slot != 0 && ((BtlActiveSlot *)slot)->unit == unit) {
        return 1;
    }
    count = func_001E8058(((BtlWork *)work)->pendingSoundList);
    for (i = 0; i < count; i++) {
        if (func_001E8060(((BtlWork *)work)->pendingSoundList, i) == unit) {
            return 1;
        }
    }
    return 0;
}

void func_001E9860(void) {
    s32 workAddress;

    workAddress = func_001AA6F8();
    ((BtlWork *)workAddress)->activeSlot = 0;
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags | 0x400;
    func_001E8050(((BtlWork *)workAddress)->pendingSoundList);
}

void func_001E9890(void) {
    s32 workAddress;

    workAddress = func_001AA6F8();
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags & 0xffffdfff;
}

void func_001E98C0(void) {
    s32 workAddress;

    workAddress = func_001AA6F8();
    ((BtlWork *)workAddress)->runtimeFlags = ((BtlWork *)workAddress)->runtimeFlags | 0x2000;
}

u32 func_001E98E8(void) {
    s32 workAddress;

    workAddress = func_001AA6F8();
    return ((((s32)((BtlWork *)workAddress)->runtimeFlags >> 0xd)) ^ 1U) & 1;
}

typedef struct WorldMotionData {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x14];
    s32 unk20;
} WorldMotionData;

extern void *dds3GetWorldObject(void);

extern WorldMotionData *func_00110C18(void *);

typedef struct WorldObjectSub {
    u8 pad0[0x34];
    s32 handle;
} WorldObjectSub;

typedef struct WorldObjectHead {
    u8 pad0[8];
    WorldObjectSub *sub;
} WorldObjectHead;

typedef struct WorldObj {
    u8 pad0[0x18];
    WorldObjectHead *head;
} WorldObj;

extern void func_00110BE0(void *, s32);
extern f32 dds3GetCameraValue(s32);
extern void func_001063A8(f32);

void func_001E9918(void) {
    WorldObj *object;
    s32 handle;
    if (((BtlWork *)func_001AA6F8())->battleFlags & 2) {
        object = dds3GetWorldObject();
        if (object != NULL) {
            handle = (s32)func_00110C18(object);
            if (handle != 0) {
                if (((WorldMotionData *)handle)->unk20 != 0) {
                    handle = ((WorldMotionData *)handle)->unk20;
                } else {
                    handle = object->head->sub->handle;
                }
                func_00110BE0(object, handle);
                func_001063A8(dds3GetCameraValue(handle));
            }
        }
    }
}

extern s32 D_00436A9C;

extern s32 D_00436AA0;

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E99C0);

s32 func_001E9A18(void) {
    WorldMotionData *data;
    if (!(((BtlWork *)func_001AA6F8())->battleFlags & 2)) {
        return 0;
    }
    data = func_00110C18(dds3GetWorldObject());
    if (data == 0) {
        return 0;
    }
    return data->unk8 == 0;
}

s32 func_001E9A68(void) {
    s32 work;

    work = func_001AA6F8();
    return work + 0x70;
}

void func_001E9A88(void) {
    func_00208D58();
}

void func_001E9AA0(void) {
    func_00208DA0();
}

void func_001E9AB8(s32 arg0) {
    func_00208DE8(((ActionUnit *)arg0)->link->unit->flags & 0x600);
}

extern void func_00208DE8(s32);

void btlApplyCombinedActorFlags(u8 *fx) {
    u32 i = 0;
    s32 bits = 0;
    u32 count = func_001E8058(((ActionUnit *)fx)->actorIndices);
    if (count != 0) {
        do {
            bits |= ((BtlUnit *)func_001E8060(((ActionUnit *)fx)->actorIndices, i++))->flags & 0x600;
        } while (i < count);
    }
    if (bits != 0) {
        func_00208DE8(bits);
    }
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C78);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C88);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417C98);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417CA8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00417CB8);

extern f32 D_003B6D90[];

void func_001E9B80(s32 arg0) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit;
    f32 current;
    f32 limit;
    if (work->activeUnitId == 1 || btlHasSingleLinkedResource(arg0) != 0) {
        for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
            if (unit->flags & 1) {
                if (unit->flags & 0x200) {
                    if (unit->flags & 2) {
                        if (unit->ext != 0) {
                            s32 node = mdlGetNodeField2C(unit->ext->info, 0);
                            if (node == 0xD || node == 0x12) {
                                current = func_001E2EE8(unit);
                                limit = (f32)func_001E2F50(unit);
                                if (unit->mode < 0xA) {
                                    limit = limit * D_003B6D90[unit->mode];
                                } else {
                                    limit = limit * 0.7f;
                                }
                                if (current < limit) {
                                    func_003343E8(unit->ext->info->data, limit);
                                    func_0020D128("btl:camera mot reset[%p]\n", unit);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

s32 func_001E9CD8(ActionUnit *arg0) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit;
    u32 count;
    f32 pos[4];
    if (work->unk268 != 3) {
        return 0;
    }
    count = func_001E8058(arg0->actorIndices);
    if (count != 1) {
        return 0;
    }
    unit = (BtlUnit *)func_001E8060(arg0->actorIndices, 0);
    if (!(unit->flags & 0x200)) {
        return 0;
    }
    if (unit->lookupId != count) {
        return 0;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
        if ((unit->flags & 0x100) && unit->lookupId != 1) {
            func_001E3108((u8 *)unit, (s128 *)pos);
            pos[2] = unit->unk38 - 90.0f;
            btlSetUnitPosition(unit, pos);
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9DD0);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001E9F30);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA058);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EA120);

s32 btlHasMarkedEntry14(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = func_001E8058(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x14] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 btlCanUseLinkedActor(s32 actor) {
    u32 status = ((ActionUnit *)actor)->status;
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
    linked = (s32)((ActionUnit *)actor)->link;
    if (linked == 0) {
        return 1;
    }
    if (btlHasMarkedEntry14(actor)) {
        return 0;
    }
    if (((BattleActionLinkState *)linked)->unit->unk12E & 0x480) {
        return 0;
    }
    category = ((ActionUnit *)actor)->category;
    if (category != 0 && (((BtlActionTableEntry *)D_00435E30)[category].flags & 1)) {
        return 0;
    }
    return 1;
}

s32 btlHasMarkedEntry10(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    u32 count;
    u8 *entry;
    u32 i;

    if (linked == 0) {
        return 0;
    }
    count = func_001E8058(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (i = 0; i < count; i++, entry += 0x59C) {
        if (entry[0x10] != 0) {
            return 1;
        }
    }
    return 0;
}

extern f32 btlUnitGetTopY(s32);

s32 btlCheckActorDistanceLimit(void) {
    BtlUnit *unit = ((BtlWork *)func_001AA6F8())->actorList;

    while (unit != NULL) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (btlUnitGetTopY((s32)unit) > 400.0f) {
                    return 0;
                }
            }
        }
        unit = unit->nextActor;
    }
    return 1;
}

s32 func_001EA3B8(void) {
    if (func_00208000(0x400, 0, 0) > 600.0f) {
        return 0;
    }
    return 1;
}

s32 func_001EA3F8(u8 *actor) {
    u8 *linked = (u8 *)((ActionUnit *)actor)->link;
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = func_001E8058(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (; i < count; i++, entry += 0x59C) {
        if (entry[0x10] == 0 && *(s32 *)(entry + 8) == 1 && *(s32 *)(entry + 0xC) == 2 &&
            !(((BtlUnit *)func_001E8060(((BattleActionLinkState *)linked)->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 func_001EA4D8(u8 *actor) {
    u8 *linked = (u8 *)((ActionUnit *)actor)->link;
    u32 count;
    u32 i;
    u8 *entry;
    if (linked == NULL) {
        return 0;
    }
    i = 0;
    count = func_001E8058(((BattleActionLinkState *)linked)->actorIndices);
    entry = ((BattleActionLinkState *)linked)->entries;
    for (; i < count; i++, entry += 0x59C) {
        if (*(s32 *)(entry + 8) == 2 &&
            !(((BtlUnit *)func_001E8060(((BattleActionLinkState *)linked)->actorIndices, i))->flags & 0x80002000)) {
            return 1;
        }
    }
    return 0;
}

s32 func_001EA598(u8 *fx) {
    u8 *task;
    u8 *owner;
    s32 index;
    u8 *table;
    if (((ActionUnit *)fx)->link == 0) {
        return 0;
    }
    if (btlHasSingleLinkedResource() == 0) {
        return 0;
    }
    task = (u8 *)((ActionUnit *)fx)->link;
    owner = *(u8 **)(task + 0x18);
    index = *(s32 *)(task + 0x44);
    table = (u8 *)func_001ABFD8(((BtlUnit *)owner)->resourceKind, ((BtlUnit *)owner)->resourceIndex);
    if (((ActionUnit *)fx)->category == 0x91) {
        return 0;
    }
    return *(s16 *)(table + index * 0x14 + 0x2C) == 2;
}

static inline s32 btlHasFlag(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

s32 btlHasActorCategoryFlag100(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)D_00435E30)[category].flags, 0x100);
}

s32 btlIsActorCategoryTypeTwo(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return ((BtlCategoryTableEntry *)D_00435E20)[category].categoryType == 2;
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
    category = ((ActionUnit *)actor)->category;
    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)D_00435E30)[category].flags, 2);
}

s32 btlHasSingleLinkedResource(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category != 0 && ((BtlCategoryTableEntry *)D_00435E20)[category].restriction != 0) {
        return 0;
    }
    return func_001E8058(((ActionUnit *)actor)->actorIndices) == 1;
}

s32 btlCanUseActorCategoryFlag4(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    if ((((BtlCategoryTableEntry *)D_00435E20)[category].flags09 & 1) == 0) {
        if (!btlCanUseLinkedActor(actor)) {
            return 0;
        }
    }
    return btlHasFlag(((BtlActionTableEntry *)D_00435E30)[((ActionUnit *)actor)->category].flags, 4);
}

s32 btlIsActorCategoryMarked(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return ((BtlCategoryTableEntry *)D_00435E20)[category].categoryType == 1;
}

s32 btlHasActorCategoryFlag40(s32 actor) {
    s32 category = ((ActionUnit *)actor)->category;

    if (category == 0) {
        return 0;
    }
    return btlHasFlag(((BtlActionTableEntry *)D_00435E30)[category].flags, 0x40);
}

s32 btlMatchLinkedActorFlags(s32 actor) {
    s32 linked;
    s32 entry;

    switch (((ActionUnit *)actor)->status) {
    case 4:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    linked = (s32)((ActionUnit *)actor)->link;
    if (linked == 0) {
        return 0;
    }
    if (func_001E8058(((BattleActionLinkState *)linked)->actorIndices) >= 2) {
        return 0;
    }
    entry = func_001E8060(((BattleActionLinkState *)linked)->actorIndices, 0);
    return ((((BattleActionLinkState *)linked)->unit->flags ^ ((BtlUnit *)entry)->flags) & 0x600) == 0;
}

s32 btlHasFirstLinkedCategoryFlag1000(s32 actor) {
    s32 linked = (s32)((ActionUnit *)actor)->link;
    s32 entry;
    u32 category;

    if (linked == 0) {
        return 0;
    }
    if (func_001E8058(((BattleActionLinkState *)linked)->actorIndices) >= 2) {
        return 0;
    }
    entry = func_001E8060(((BattleActionLinkState *)linked)->actorIndices, 0);
    if ((((BtlUnit *)entry)->flags & 0x400) == 0) {
        return 0;
    }
    category = ((BtlUnit *)entry)->resourceIndex;
    if (category >= 0x180) {
        return 0;
    }
    return btlHasFlag(((BtlResourceTableEntry *)D_00435DEC)[category].flags, 0x1000);
}

u8 func_001EA940(s32 arg0) {
    return ((ActionUnit *)arg0)->category == 0x5f;
}

s32 btlMapActorCategory(s32 actor) {
    switch ((u32)((ActionUnit *)actor)->category) {
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
    switch ((u32)((ActionUnit *)actor)->category) {
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
    return ((ActionUnit *)arg0)->category == 0x1a0;
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

void func_001EBD40(ActionUnit *arg0) {
    BtlUnit *target;
    if (((BtlWork *)func_001AA6F8())->flags220 & 1) {
        target = arg0->link->unit;
        if (target->flags & 0x400) {
            func_001ECBF8(arg0, arg0, target);
            return;
        }
    }
    if (arg0->actionKind == arg0->status || arg0->actionKind == 0xA || (arg0->flags & 0x40000)) {
        func_001E9598((XformData *)((u8 *)arg0 + 0x30), (XformData *)arg0);
        func_001F17C8(arg0, (u8 *)arg0 + 0xC0, arg0->link->unit, 0);
        ((BattlePoseBlendState *)arg0)->duration = 7.0f;
        arg0->flags = (arg0->flags | 0x1041) & 0xFFFBFFFF;
    } else {
        func_001F17C8(arg0, arg0, arg0->link->unit, 0);
    }
}

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
    if (!(((BtlUnit *)arg0)->flags & 0x10000)) {
        func_001F4D70(arg0, arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001EC418);

void func_001EC5F0(u32 arg0) {
    if (!(((BtlUnit *)arg0)->flags & 0x10000)) {
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
    s32 actor;

    actor = (s32)arg0;
    func_001E8030(((ActionUnit *)actor)->actorIndices, (u32)((ActionUnit *)actor)->link->unit);
    func_001F2E30(arg0, actor + 0x30, actor + 0xc0);
}

void func_001EC6C8(void) {
}

void func_001EC6D0(u32 arg0) {
    if ((((ActionUnit *)arg0)->link->unit->flags & 0x200) != 0) {
        func_001F25F8(arg0, arg0);
        return;
    }
    if (((ActionUnit *)arg0)->actionKind != 0x10) {
        func_001F2740(arg0, arg0);
        return;
    }
}

void func_001EC728(void) {
}

void func_001EC730(s32 arg0) {
    if (((ActionUnit *)arg0)->link != 0) {
        func_001FFD30((s32)((ActionUnit *)arg0)->link);
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
    BtlUnit *unit = ((ActionUnit *)fx)->link->unit;
    if (unit->flags & 2) {
        if (btlSetActorEffectParameter(unit, 1) == 0) {
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

void btlAdvanceCommandCursor(s32 arg0, s32 arg1) {
    if (CURSOR->mode == 0) {
        func_001FA480(arg0, arg1, D_003BBF70[CURSOR->index]);
    } else {
        func_001FA480(arg0, arg1, D_003BBF88[CURSOR->index]);
    }
    func_001FBAC0(arg0, arg1);
    CURSOR->frame++;
}

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

void btlUnitGetPosVU(u32 unit, u8 mode) {
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

BtlUnit *btlFindFlaggedUnitById(s32 id) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & 0x200) {
                    if (unit->lookupId == id) {
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

s32 btlCountUnitsByFlags(u32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
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

void func_001FFBB0(s32 arg0, s32 arg1) {
    s32 first;
    func_001AA6F8();
    first = func_001E8060(((ActionUnit *)arg0)->actorIndices, 0);
    memset(CURSOR, 0, 0x130);
    func_001E4378(first);
    if (btlHasFirstLinkedCategoryFlag1000(arg0) != 0) {
        func_001F5868(arg0, arg1, 5, 1);
    } else {
        func_001F5868(arg0, arg1, 5, 0);
    }
    CURSOR->unk_0C = 0;
    func_001F5320(arg0, arg1, 0, 0x11);
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_001FFC70);

extern void func_001E4378(s32);

extern void func_001E9AA0(void);

void func_001FFD30(u8 *arg0) {
    u8 *scene = (u8 *)func_001AA6F8() + 0x70;
    *(u8 **)(scene + 0x114) = arg0;
    memset(D_003BD7D0, 0, 0x130);
    CURSOR->unk_0A = 0;
    CURSOR->unk_0E = 0;
    func_001E4378((s32)((BtlUnit *)arg0)->link18);
    func_001F5868((s32)scene, (s32)scene, 6, 0);
    func_001E9AA0();
    btlFlagUnitDefeatCandidate(*(BtlUnit **)(arg0 + 0x18));
    CURSOR->unk_0C = 0;
    func_001F5320((s32)scene, (s32)scene, 0, 0);
}

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
    CURSOR->unk_0C = 2;
    return result;
}

void func_001FFFF8(void) {
    u64 worldCounter;
    s32 work;
    u32 action;

    work = func_001AA6F8();
    worldCounter = dds3AdvanceWorldCounter();
    action = evtSpawnActionObj9(worldCounter);
    ((BtlWork *)work)->unk228 = action;
    D_00436AD4 = 0;
}

s32 btlAreWorkBuffersReady(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->primaryBuffer != 0) {
        if (work->secondaryBuffer != 0) {
            return 1;
        }
    }
    return 0;
}

void btlReleaseWorkBuffers(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->secondaryBuffer != 0) {
        func_00328470(work->secondaryBuffer);
        work->secondaryBuffer = 0;
    }
    if (work->primaryBuffer != 0) {
        func_00328470(work->primaryBuffer);
        work->primaryBuffer = 0;
    }
}

void func_002000D0(void) {
    s64 pending;
    s32 work;

    func_0023A9E0();
    do {
        pending = sdfGraphHasPendingWorkInterruptSafe();
    } while (pending != 0);
    evtDestroyWorldSecondaryNode();
    do {
        pending = sdfGraphHasPendingWorkInterruptSafe();
    } while (pending != 0);
    btlReleaseWorkBuffers();
    work = func_001AA6F8();
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags & 0xfffffffd;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00418C58);

INCLUDE_ASM(const s32, "game/code_001DACF8", btlFreeFieldBlocks);

void func_002001F0(void) {
    u8 *context = (u8 *)func_001AA6F8();
    fldApplyLightSetCurrent();
    func_00200568(0x80, 0);
    VU_LOAD10((u8 *)D_0037F770[0] + 0x10);
    VU_STORE10(context + 0x10);
    VU_STORE10(context + 0x40);
    VU_LOAD10(D_0037F770[0]);
    VU_STORE10(context + 0x20);
    VU_STORE10(context + 0x50);
    VU_LOAD10(D_0037F780);
    VU_STORE10(context + 0x30);
    VU_STORE10(context + 0x60);
    ((BtlWork *)context)->tint71C = 0x807E5C5E;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200290);

s32 func_00200490(s32 index) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (work->battleFlags & 0x30000000) {
        return work->unk724;
    }
    return ((BtlActionTableEntry *)D_00435E30)[index].defaultValue;
}

u32 func_002004E8(void) {
    u8 *context = (u8 *)func_001AA6F8();
    if (((BtlCameraVectors *)context)->eye[0] != ((BtlCameraVectors *)context)->eye[0] ||
        ((BtlCameraVectors *)context)->eye[1] != ((BtlCameraVectors *)context)->eye[1] ||
        ((BtlCameraVectors *)context)->eye[2] != ((BtlCameraVectors *)context)->eye[2] ||
        ((BtlCameraVectors *)context)->target[0] != ((BtlCameraVectors *)context)->target[0] ||
        ((BtlCameraVectors *)context)->target[1] != ((BtlCameraVectors *)context)->target[1] ||
        ((BtlCameraVectors *)context)->target[2] != ((BtlCameraVectors *)context)->target[2]) {
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

void btlStepBlendColor(void) {
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

void func_00200828(void) {
    u8 *context = (u8 *)func_001AA6F8();
    f32 *position = D_0037F770[0];

    if (position[0] == 0.0f && position[1] == 0.0f &&
        position[2] == 0.0f) {
        func_00114068(0);
    } else {
        func_00114068(1);
    }
    if (((BtlWork *)context)->flags21C & 0x20) {
        func_00114068(0);
    }
    btlStepBlendColor();
}

void func_002008C0(void) {
    s32 work;

    work = func_001AA6F8();
    if ((((((BtlWork *)work)->battleFlags & 0x20000) != 0) && ((((BtlWork *)work)->flags21C & 0x20) == 0)) &&
          ((((BtlWork *)work)->flags220 & 0x4000000) == 0)) {
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
    if (((BtlWork *)work)->soundTransitionTask != 0) {
        func_0020D128("btl:rain exit\n");
        func_002DEBB0(((BtlWork *)work)->soundTransitionTask);
        ((BtlWork *)work)->soundTransitionTask = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200B30);

extern u32 func_00200B30(void);

SoundTask *func_00200D00(s32 arg0, s32 arg1) {
    SoundTask *task = btlAllocTask(0x28);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 1;
    task->flags &= ~1;
    task->callback = func_00200B30;
    task->status = 0;
    args = func_001E14F8((s32)task);
    memset(args, 0, 0x28);
    args->value = arg0;
    args->option = arg1;
    *(s32 *)((u8 *)args + 0x24) = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00200DA0);

extern u32 func_00200DA0(void);

SoundTask *func_00200F28(s32 arg0, s32 arg1) {
    SoundTask *task = btlAllocTask(0x14);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 2;
    task->flags &= ~1;
    task->callback = func_00200DA0;
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

extern u32 func_00200FB8(u32 *);

SoundTask *func_00201108(u8 *source, u32 value) {
    SoundTask *task = btlAllocTask(0x34);
    u8 *arguments;
    task->enabled = 1;
    task->taskId = 3;
    task->flags |= 2;
    task->callback = func_00200FB8;
    task->status = 0;
    task->onStart = 0;
    arguments = (u8 *)func_001E14F8((s32)task);
    *(u32 *)(arguments + 0x30) = value;
    if (source != 0) {
        memcpy(arguments, source, 0x30);
    } else {
        memcpy(arguments, (u8 *)func_001AA6F8() + 0x10, 0x30);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201268);

extern s32 func_00201268();

SoundTask *func_002014A8(value)
    u32 value;
{
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 4;
    task->flags |= 2;
    task->callback = func_00201268;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

s64 func_00201520(void) {
    D_00436AD4 = 1;
    return func_00201268();
}

SoundTask *func_00201540(void) {
    SoundTask *task = (SoundTask *)func_002014A8();
    task->taskId = 7;
    task->callback = func_00201520;
    return task;
}

s32 func_00201578(u32 *arg0) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    if (!(work->battleFlags & 0x20000000)) {
        func_002006B0(arg0[0], *(u16 *)(arg0 + 1));
    }
    D_00436AD8++;
    return 1;
}

SoundTask *sndCreateAcquireTask(s32 arg0, s32 arg1) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 5;
    task->callback = func_00201578;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0;
    args->option = arg1;
    return task;
}

s32 sndTickFadeCounter(soundId)
    u16 *soundId;
{
    s32 context = func_001AA6F8();

    if (D_00436AD8 == 0) {
        return 1;
    }
    D_00436AD8--;
    if (D_00436AD8 != 0) {
        return 1;
    }
    if ((((BtlWork *)context)->battleFlags & 0x20000000) != 0) {
        return 1;
    }
    func_002006F0(*soundId);
    return 1;
}

extern s32 sndTickFadeCounter();

SoundTask *sndCreateReleaseTask(value)
    u32 value;
{
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 6;
    task->callback = sndTickFadeCounter;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

s64 func_00201718(void) {
    D_00436AD8 = 1;
    return sndTickFadeCounter();
}

SoundTask *func_00201738(void) {
    SoundTask *task = (SoundTask *)sndCreateReleaseTask();
    task->taskId = 8;
    task->callback = func_00201718;
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
    return ((BtlActionTableEntry *)D_00435E30)[arg1].resourceType;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00201828);

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

extern u32 func_00201C98(u32 *);

SoundTask *func_00201F08(u32 effect, s32 arg1, BtlUnit *actor, u16 frames) {
    s32 v1 = 0;
    s32 v2;
    SoundTask *task = btlAllocTask(32);
    SoundTaskArgs *args;
    BtlWork *work;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2E;
    task->flags |= 2;
    task->owner = actor->owner;
    task->onStart = (void (*)(u32))sndAddEffectReferences;
    task->callback = func_00201C98;
    task->onFinish = (void (*)(u32 *))sndReleaseEffectReferences;
    work = (BtlWork *)func_001AA6F8();
    if (work->hook6E4 != 0) {
        v1 = work->hook6E4(actor);
    }
    if (v1 == 0) {
        v1 = arg1;
    }
    v2 = 0;
    if (work->hook6E8 != 0) {
        v2 = work->hook6E8(actor);
    }
    if (v2 == 0) {
        v2 = arg1;
    }
    if (work->hook6E0 != 0) {
        s32 r = work->hook6E0(actor);
        if (r != 0) {
            actor = (BtlUnit *)r;
        }
    }
    args = func_001E14F8((s32)task);
    args->value = effect;
    args->unk_0C = arg1;
    args->unk_10 = v1;
    args->unk_14 = v2;
    args->optionId = frames;
    args->unk_18 = (u32)actor;
    args->unk_08 = 0;
    args->unk_1C = 0;
    return task;
}

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
    s32 effect;
    s32 unit;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    effect = *arg0;
    unit = arg0[2];
    *(s32 *)(effect + 4) = *(s32 *)(effect + 4) - 1;
    *(s32 *)(unit + 0x334) = *(s32 *)(unit + 0x334) - 1;
    sndDeleteSystemEffect(effect);
}

extern u32 func_00202100(u32 *);

SoundTask *func_002022E0(u32 effect, BtlUnit *owner, u32 channel) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x2F;
    task->flags |= 2;
    task->owner = owner->owner;
    task->onStart = (void (*)(u32))sndStartEffectTask;
    task->callback = func_00202100;
    task->onFinish = func_00202288;
    args = func_001E14F8((s32)task);
    args->value = effect;
    args->unk_08 = (u32)owner;
    args->unk_0C = channel;
    args->option = 0;
    args->unk_10 = 0;
    return task;
}

typedef struct SoundEffectNode {
    u32 flags;
    s32 referenceCount;     /* 0x04 */
    u32 activeCount;        /* 0x08 */
    u8 padC[4];
    u32 handle;
} SoundEffectNode;

void func_002023A0(s32 *arg0) {
    ((SoundEffectNode *)*arg0)->activeCount = ((SoundEffectNode *)*arg0)->activeCount + 1;
}

u32 func_002023B8(u32 *args) {
    u32 *effect = (u32 *)args[0];
    s32 frames;

    if ((effect[0] & 2) == 0) {
        return 0;
    }
    frames = func_00202F60((s32)effect, (u16)args[1]);
    if ((s32)args[5] >= frames) {
        ((SoundEffectNode *)args[0])->activeCount -= 1;
        if ((s32)args[3] >= 0) {
            func_001E2C00((u8 *)args[2], args[3], args[4], 1.0f);
        }
        return 1;
    }
    args[5]++;
    return 0;
}

SoundTask *func_00202450(u32 effect, BtlUnit *actor, u16 frames, u32 channel, u32 volume) {
    SoundTask *task = btlAllocTask(24);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x31;
    task->owner = actor->owner;
    task->onStart = (void (*)(u32))func_002023A0;
    task->callback = func_002023B8;
    task->onFinish = 0;
    args = func_001E14F8((s32)task);
    args->value = effect;
    args->unk_08 = (u32)actor;
    args->optionId = frames;
    args->unk_0C = channel;
    args->unk_10 = volume;
    args->unk_14 = 0;
    return task;
}

extern void *func_002C80C8(const char *);

typedef struct EffectLoadArgs {
    SoundEffectNode *effect;
    void *loadHandle;
    const char *name;
} EffectLoadArgs;

void sndBeginEffectLoad(EffectLoadArgs *args) {
    SoundEffectNode *effect = args->effect;
    if (effect->flags & 2) {
        if (effect->handle != 0) {
            func_001683F0(effect->handle);
            effect->handle = 0;
        }
        effect->flags &= ~2;
    }
    args->loadHandle = func_002C80C8(args->name);
    effect->flags |= 1;
    func_0020D128("btl:effect load start[%s]\n", args->name);
}

u32 sndPollEffectLoad(s32 arg) {
    EffectLoadArgs *args = (EffectLoadArgs *)arg;
    SoundEffectNode *effect = args->effect;
    s32 resource;
    if (effect->flags & 2) {
        return 1;
    }
    if (func_002C8128((s32)args->loadHandle) == 0) {
        return 0;
    }
    func_0020D128("btl:effect load end[%s]\n", args->name);
    resource = fileGetResourceHandle((s32)args->loadHandle);
    effect->handle = func_001682B0(sdfResourceRetainAddress(resource));
    func_003297C8(resource);
    func_002C7D00((s32)args->loadHandle);
    effect->flags = (effect->flags & ~1) | 2;
    return 0;
}

extern u32 sndPollEffectLoad(s32);

SoundTask *sndCreateEffectLoadTask(s32 arg0, char *name) {
    SoundTask *task = btlAllocTask(strlen(name) + 0xC);
    SoundTaskArgs *args;
    char *copy;
    task->enabled = 1;
    task->taskId = 0x32;
    task->flags &= ~1;
    task->onStart = (void (*)(u32))sndBeginEffectLoad;
    task->callback = sndPollEffectLoad;
    task->status = 0;
    args = func_001E14F8((s32)task);
    copy = (char *)args + 0xC;
    args->value = arg0;
    args->unk_08 = (u32)copy;
    strcpy(copy, name);
    return task;
}

u32 btlWaitUnitListIdle(void) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    BtlUnit *unit;
    if (work->battleFlags & 0x40000000) {
        return 1;
    }
    for (unit = work->actorList; unit != 0; unit = unit->nextActor) {
    }
    return 1;
}

SoundTask *func_00202750(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x33;
    task->callback = btlWaitUnitListIdle;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

u32 sndApplyToActiveActors(arg0)
    s32 *arg0;
{
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 2) {
                if (unit->ext != 0) {
                    if (!(unit->flags & 0xE0)) {
                        btlBlendUnitColor(unit, unit->baseColor, *arg0);
                    }
                }
            }
        }
    }
    return 1;
}

SoundTask *func_00202840(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x34;
    task->callback = sndApplyToActiveActors;
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
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_002028A8;
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
    s32 effect;
    s32 unit;

    if (arg0[1] != 0) {
        func_001686F0(arg0[1]);
    }
    effect = *arg0;
    unit = arg0[2];
    ((SoundEffectNode *)effect)->referenceCount = ((SoundEffectNode *)effect)->referenceCount - 1;
    ((BtlUnit *)unit)->unk334 = ((BtlUnit *)unit)->unk334 - 1;
    sndDeleteSystemEffect(effect);
}

extern u32 func_00202958(u32 *);

SoundTask *func_00202BB0(u32 effect, BtlUnit *owner, u64 resource) {
    SoundTask *task = btlAllocTask(32);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x30;
    task->flags |= 2;
    task->owner = owner->owner;
    task->onStart = (void (*)(u32))sndAddSourceReferences;
    task->callback = func_00202958;
    task->onFinish = func_00202B58;
    args = func_001E14F8((s32)task);
    args->value = effect;
    args->unk_08 = (u32)owner;
    *(u64 *)&args->unk_10 = resource;
    args->option = 0;
    args->unk_18 = 0;
    args->unk_1C = 0;
    return task;
}

u32 func_00202C70(u32 *arg0) {
    kwlnFadeStartIn(*arg0);
    return 1;
}

SoundTask *func_00202C90(u32 value) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x38;
    task->callback = func_00202C70;
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
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x39;
    task->callback = func_00202CF8;
    task->status = 0;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0;
    args->option = arg1;
    return task;
}

u32 func_00202DA8(void) {
    s32 work;

    work = func_001AA6F8();
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags | 0x40000;
    mdlClearListedObjectFlag();
    return 1;
}

SoundTask *sndCreateSetBattleFlagTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00202DA8;
    task->taskId = 0x3A;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_00202E28(void) {
    s32 work;

    work = func_001AA6F8();
    ((BtlWork *)work)->battleFlags = ((BtlWork *)work)->battleFlags & 0xfffbffff;
    mdlSetListedObjectFlag();
    return 1;
}

SoundTask *sndCreateClearBattleFlagTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00202E28;
    task->taskId = 0x3B;
    task->onStart = 0;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00202EA8);

void func_00202F40(s32 arg0, u16 arg1) {
    func_001684A8(((SoundEffectNode *)arg0)->handle, arg1);
}

s32 func_00202F60(s32 arg0, u16 arg1) {
    return func_00168448(((SoundEffectNode *)arg0)->handle, arg1);
}

s32 func_00202F80(s32 arg0) {
    if (((SoundEffectNode *)arg0)->referenceCount != 0) {
        return 1;
    }
    return ((SoundEffectNode *)arg0)->activeCount != 0;
}

s32 sndHasActiveActor(void) {
    BtlUnit *unit;
    for (unit = ((BtlWork *)func_001AA6F8())->actorList; unit != 0; unit = unit->nextActor) {
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
    node->fadeCountdown = 0;
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

void btlUpdateFadeColor(void) {
    BtlWork *context = (BtlWork *)func_001AA6F8();
    SoundResourceNode *node;

    for (node = context->resourceList258; node != 0; node = node->next) {
        if (node->unk_04 == 0) {
            node->fadeCountdown = 0;
        } else if (node->fadeCountdown > 0) {
            node->fadeCountdown = node->fadeCountdown - 1;
        }
    }
    if ((u32)(func_001E9798() - 9) < 2 || context->unk2CC != 0 || context->unk22C == 8) {
        context->fadeEnabled = 0;
    } else {
        context->fadeEnabled = 1;
    }
    switch (context->fadeEnabled) {
    case 0: {
        u32 packedColor = context->fadeColor;

        /* The high byte rises by 0x10 per frame, capped at the opaque gray tint. */
        if (packedColor <= 0x8080807F) {
            context->fadeColor = packedColor + 0x10000000;
        } else {
            context->fadeColor = 0x80808080;
        }
        break;
    }
    case 1: {
        u32 packedColor = context->fadeColor;

        if (packedColor > 0x808080) {
            context->fadeColor = packedColor - 0x10000000;
        } else {
            context->fadeColor = 0x808080;
        }
        break;
    }
    }
    func_002F7580();
}

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
    mdlMarkAndProcessObjectNodes();
    effBTLFieldColorResetFlags();
    func_002D2CA8();
}

void sndClearResourceNodes(void) {
    SoundResourceNode *node;
    SoundResourceNode *next;
    for (node = ((BtlWork *)func_001AA6F8())->resourceList258; node != 0; node = next) {
        next = node->next;
        sndFreeResourceNode(node);
    }
}

s32 btlButtonMaskToIndex(u32 mask) {
    switch (mask & 0x7FFF) {
    case 0:
        return 0;
    case 0x0001:
        return 0xE;
    case 0x0002:
        return 0xB;
    case 0x0004:
        return 0xA;
    case 0x0008:
        return 8;
    case 0x0010:
        return 6;
    case 0x0020:
        return 7;
    case 0x0040:
        return 9;
    case 0x0080:
        return 5;
    case 0x0100:
        return 0xD;
    case 0x0200:
        return 4;
    case 0x0400:
        return 3;
    case 0x0800:
        return 0xC;
    case 0x1000:
        return 2;
    case 0x2000:
        return 1;
    case 0x4000:
        return 0x10;
    default:
        return 0;
    }
}

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
    s32 work;

    work = func_001AA6F8();
    ((BtlWork *)work)->fadeEnabled = 0;
    return 1;
}

SoundTask *sndCreateClearStateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00203AB0;
    task->taskId = 0x36;
    task->onStart = 0;
    task->status = 0;
    return task;
}

u32 func_00203B20(void) {
    s32 work;

    work = func_001AA6F8();
    ((BtlWork *)work)->fadeEnabled = 1;
    return 1;
}

SoundTask *sndCreateSetStateTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00203B20;
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

void btlRefreshSoundEntries(void) {
    u32 i;
    func_001AA6F8();
    for (i = 0; i < 0x31; i++) {
        if (D_003BDE18[i].unk4 != 0) {
            func_00203D48(i, D_003BDE18[i].unk4);
        }
    }
}

void sndFreeBattleSoundEntries(void) {
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
    u32 flags;
    s32 work;
    u32 *node;

    work = func_001AA6F8();
    node = (u32 *)sndAllocResourceNode();
    flags = *node;
    node[5] = arg1;
    *(u32 **)(arg0 * 4 + work + 0x4ec) = node;
    *node = flags | 10;
}

typedef struct SoundHandleNode {
    u32 handle;
    void *actor;
} SoundHandleNode;

SoundHandleNode *sndCreateSystemEffectHandle(void *actor, s32 index) {
    SoundHandleNode *node = func_00328E18(8);
    SoundEntry *entry = &D_003BDE18[index];
    node->actor = actor;
    node->handle = func_002D4138(entry->unk4);
    return node;
}

extern void mdlLoadPrimaryVectorVU(s32);

extern void func_002D46F0(s32, f32 *);

extern void func_002D4398(s32);

void func_00203E18(s32 *args) {
    f32 pos[4];
    if (func_00332D48(*(s32 *)(args[1] + 0x18), 1) == 0) {
        mdlLoadPrimaryVectorVU(args[1]);
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(pos) : "memory");
        pos[1] -= 150.0f;
    } else {
        __asm__ volatile("sqc2 vf10, 0(%0)" : : "r"(pos) : "memory");
    }
    func_002D46F0(args[0], pos);
    func_002D4398(args[0]);
}

void func_00203E90(u32 arg0) {
    fileQueueDestroy(*(u32 *)arg0);
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
    s32 status;

    status = func_002A2330();
    return status - 2U < 2;
}

void func_00204000(void) {
    s32 work;

    work = func_001AA6F8();
    if ((((BtlWork *)work)->battleFlags & 0x10000) != 0) {
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
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5A;
    task->callback = sndPlayStationedSe;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = value;
    return task;
}

s32 func_00204190(void) {
    s32 node = (s32)((BtlWork *)func_001AA6F8())->soundList;
    while (node != 0) {
        if ((((ActiveSoundNode *)node)->flags & 8) != 0) {
            return 1;
        }
        node = (s32)((ActiveSoundNode *)node)->next;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002041E8);

SoundTask *sndCreateSkillSeTask(s32 *arg0, u16 arg1) {
    SoundTask *task = btlAllocTask(8);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x57;
    task->callback = func_002041E8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->value = arg0[2];
    args->optionId = arg1;
    return task;
}

typedef struct SoundFileNode {
    u32 flags;
    u8 mode;
    u8 pad5[3];
    u32 position;
} SoundFileNode;

typedef struct FileLoadArgs {
    SoundFileNode *node;
    void *loadHandle;
    s32 resourceHandle;
    s32 frames;
    const char *name;
} FileLoadArgs;

void sndStartFileLoad(FileLoadArgs *args) {
    SoundFileNode *node = args->node;
    args->loadHandle = func_002C80C8(args->name);
    node->flags |= 1;
    node->position = (args->frames + 0x200) << 16;
    node->mode = 2;
    func_0020D128("btl:sound file load start[%s]\n", args->name);
}

u32 sndPollMotSeFileAndSpu(FileLoadArgs *request) {
    SoundFileNode *node = request->node;
    if (sndHasActiveFileLoad()) {
        func_0020D128("btl:sound wait[motSE]\n");
        return 0;
    }
    if ((node->flags & 2) == 0) {
        if (func_002C8128((s32)request->loadHandle)) {
            s32 size;
            s32 data;
            func_0020D128("btl:sound file load end[%s]\n", request->name);
            request->resourceHandle = fileGetResourceHandle((s32)request->loadHandle);
            size = fileGetResourceSize((s32)request->loadHandle);
            data = sdfResourceRetainAddress(request->resourceHandle);
            if (func_00342168(node->position) == 0) {
                func_003422F8(data, size);
                node->flags |= 8;
                func_0020D128("btl:sound SPU load start[%X][size:%d]\n", *(u16 *)((u8 *)node + 0xA), size);
            }
            node->flags = (node->flags & ~1) | 2;
        }
    } else if (func_00342168(node->position) != 0) {
        func_0020D128("btl:sound SPU load end[%X]\n", *(u16 *)((u8 *)node + 0xA));
        func_003297C8(request->resourceHandle);
        func_002C7D00((s32)request->loadHandle);
        node->flags = (node->flags & ~8) | 0x10;
        return 1;
    }
    return 0;
}

SoundTask *sndCreateFileLoadTask(s32 arg0, s32 arg1, char *name) {
    SoundTask *task = btlAllocTask(strlen(name) + 0x14);
    SoundTaskArgs *args;
    char *copy;
    task->enabled = 1;
    task->taskId = 0x58;
    task->flags &= ~1;
    task->onStart = sndStartFileLoad;
    task->callback = sndPollMotSeFileAndSpu;
    task->status = 0;
    args = func_001E14F8((s32)task);
    copy = (char *)args + 0x14;
    args->value = arg0;
    args->unk_0C = arg1;
    args->unk_10 = (u32)copy;
    strcpy(copy, name);
    return task;
}

s32 sndLoadDataFile(s32 *data) {
    char filename[0x70];
    if (func_00204678()) {
        return 1;
    }
    func_002046F0(data[0], (s32)filename);
    sdfSoundSendNamedCommand(filename, 0x34);
    return 1;
}

SoundTask *sndCreateDataFileLoadTask(BtlUnit *unit) {
    SoundTask *task = btlAllocTask(4);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = sndLoadDataFile;
    task->taskId = 0x5B;
    task->owner = unit->owner;
    task->onStart = 0;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    return task;
}

s32 func_00204678(void) {
    return (s8)sdfSoundIsCommandBusy();
}

s32 func_002046A0(s32 arg0) {
    s32 flags;

    flags = *(s32 *)arg0;
    if ((flags & 1) != 0) {
        return 1;
    }
    return (flags & 8) > 0;
}

void func_002046C0(s32 source, s32 output) {
    func_0035C860(output, D_004192D8, D_00436AE8, (u16)(source + 0x200));
}

void func_002046F0(s32 unit, s32 output) {
    func_0035C860(output, D_004192E8, ((BtlUnit *)unit)->mode);
}

s32 sndResolveResourceId(s32 category, s32 id) {
    BtlWork *work = (BtlWork *)func_001AA6F8();
    s32 type = -1;
    if (work->hook688 != 0) {
        type = work->hook688(category, id);
    }
    if (type == -1) {
        type = sndLookupResourceType(category, id);
    }
    if (type < 26) {
        if (type == 0) {
            return -1;
        }
        if (type >= 11) {
            return type + work->unk208 - 6;
        }
        return type + 0xFFFF;
    }
    return -1;
}

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

void sndClearList(void) {
    ActiveSoundNode *node;
    ActiveSoundNode *next;
    for (node = ((BtlWork *)func_001AA6F8())->soundList; node != 0; node = next) {
        next = node->next;
        sndFreeListNode(node);
    }
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002048C8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192D8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192E8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_004192F8);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419308);

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419318);

INCLUDE_ASM(const s32, "game/code_001DACF8", sndLoadMotSeFiles);

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

typedef struct SoundSlotOwner {
    u32 flags;
    s32 category;
    s32 id;
    s32 refCount;
    u8 pad10[8];
    s32 slot[0x1D];
    s32 handle[0x1D];
    struct SoundSlotOwner *prev;
    struct SoundSlotOwner *next;
} SoundSlotOwner;

extern void sndLoadMotSeFiles(SoundSlotOwner *);

SoundSlotOwner *sndAcquireSlotOwner(s32 category, s32 id) {
    SoundSlotOwner *owner = sndFindListNodeForChannel(category, id);
    BtlWork *work;
    if (owner != 0) {
        func_0020D128("btl:motSE search hit[%p]\n", owner);
        owner->refCount++;
        return owner;
    }
    owner = func_00328E18(0x108);
    owner->category = category;
    owner->id = id;
    owner->refCount = 1;
    work = (BtlWork *)func_001AA6F8();
    owner->prev = 0;
    if (work->soundSlotOwners != 0) {
        work->soundSlotOwners->prev = owner;
        owner->next = work->soundSlotOwners;
    } else {
        owner->next = 0;
    }
    work->soundSlotOwners = owner;
    if (mdlFlagTest(0xC0F) == 0) {
        sndLoadMotSeFiles(owner);
    }
    return owner;
}

extern void func_002C7D00(s32);

extern void func_003297C8(u32);

void sndReleaseSlotOwner(SoundSlotOwner *owner) {
    u32 i;
    if (--owner->refCount == 0) {
        for (i = 0; i < 0x1D; i++) {
            if (owner->slot[i] != 0) {
                func_002C7D00(owner->slot[i]);
            }
            if (owner->handle[i] != 0) {
                func_003297C8(owner->handle[i]);
            }
        }
        if (owner->next != 0) {
            owner->next->prev = owner->prev;
        }
        if (owner->prev != 0) {
            owner->prev->next = owner->next;
        } else {
            ((BtlWork *)func_001AA6F8())->soundSlotOwners = owner->next;
        }
        func_00328E48(owner);
    }
}

void sndReleaseAllSlotOwners(void) {
    SoundSlotOwner *owner;
    SoundSlotOwner *next;
    for (owner = ((BtlWork *)func_001AA6F8())->soundSlotOwners; owner != 0; owner = next) {
        next = owner->next;
        sndReleaseSlotOwner(owner);
    }
}

void func_00204D08(void) {
    u64 task;

    task = func_00205018();
    btlStartTask(task);
}

s32 sndHasActiveFileLoad(void) {
    u8 *node = (u8 *)((BtlWork *)func_001AA6F8())->soundSlotOwners;
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

extern void func_00204D80(u32);
extern u32 func_00204E50(u32 *);

struct SoundTask *func_00205018(unit, arg1)
    BtlUnit *unit;
    s32 arg1;
{
    SoundTask *task = btlAllocTask(16);
    SoundTaskArgs *args;
    BtlWork *work;
    task->status = 0;
    task->enabled = 1;
    task->taskId = 0x59;
    task->owner = unit->owner;
    task->onStart = func_00204D80;
    task->callback = func_00204E50;
    work = (BtlWork *)func_001AA6F8();
    if (work->hook710 != 0) {
        arg1 = work->hook710(unit, arg1);
    }
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->unk_08 = arg1;
    args->option = 0;
    args->unk_0C = 0;
    return task;
}

void func_002050D0(void) {
    s32 context = func_001AA6F8();
    ((BtlWork *)context)->unk288 = -1;
    ((BtlWork *)context)->unk28C = -1;
}

extern char D_00419408[]; /* "btl:sound load BSE SMG\n" */

void sndLoadBattleBank(void) {
    if (func_00342168(0x10000) == 0) {
        func_003421E8(0x10000);
        func_0020D128(D_00419408);
    }
}

u8 func_00205140(void) {
    s64 status;

    status = func_00342168(0x10000);
    return status != 0;
}

INCLUDE_RODATA(const s32, "game/code_001DACF8", D_00419408);

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205160);

s32 sndHasOccupiedNodeSlots(void) {
    SoundSlotOwner *owner;
    u32 i;
    for (owner = ((BtlWork *)func_001AA6F8())->soundSlotOwners; owner != 0; owner = owner->next) {
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
    u32 ready;
    s64 status;

    status = func_002A2928();
    ready = 1;
    if (status != 0) {
        if (status == 2) {
            func_002A2998();
            ready = 0;
        }
        else {
            ready = 0;
        }
    }
    return ready;
}

SoundTask *sndCreateEarringTask(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00205438;
    task->taskId = 0x5C;
    task->status = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002054B8);

extern u32 func_002054B8(void);

SoundTask *func_002055E0(s32 arg0) {
    SoundTask *task = btlAllocTask(0xC);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->taskId = 0x5D;
    task->flags &= ~1;
    task->callback = func_002054B8;
    task->status = 0;
    args = func_001E14F8((s32)task);
    args->unk_08 = arg0;
    args->option = 0;
    args->value = 0;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_00205660);

s32 func_00205758(u32 *args) {
    s32 data;
    s32 size;
    if (args[1] == 0) {
        return 1;
    }
    if (args[2] == 0) {
        if (func_002C8128(args[1]) != 0) {
            args[2] = fileGetResourceHandle(args[1]);
            data = sdfResourceRetainAddress(args[2]);
            size = fileGetResourceSize(args[1]);
            func_002C7D00(args[1]);
            func_002A27A8(data, size, 2);
            func_002A2998();
            func_0020D128("btl:ATRAC3 dead load end\n");
        }
        return 0;
    }
    if (func_002A2928() == 0) {
        func_0020D128("btl:ATRAC3 dead play end\n");
        return 1;
    }
    return 0;
}

void sndFinishEarringPlaybackTask(s32 *arg0) {
    u8 *work = (u8 *)func_001AA6F8();
    if (arg0[2] != 0) {
        func_003297C8(arg0[2]);
    }
    ((BtlWork *)work)->earringPlaybackCount += 0xFFFF;
}

extern void func_00205660(u32);

SoundTask *sndCreateEarringPlaybackTask(BtlUnit *owner) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->taskId = 0x5E;
    task->flags &= ~1;
    task->owner = owner->owner;
    task->onStart = func_00205660;
    task->callback = func_00205758;
    task->onFinish = (void (*)(u32 *))sndFinishEarringPlaybackTask;
    args = func_001E14F8((s32)task);
    args->actor = owner;
    args->option = 0;
    args->unk_08 = 0;
    return task;
}

u32 func_00205930(void) {
    func_002040A8(0x1c);
    return 1;
}

SoundTask *func_00205950(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00205930;
    task->taskId = 0x5F;
    task->status = 0;
    return task;
}

u32 func_00205990(void) {
    func_00204038();
    return 1;
}

SoundTask *func_002059B0(void) {
    SoundTask *task = btlAllocTask(0);
    task->enabled = 1;
    task->callback = func_00205990;
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

void func_002060B0(BtlUnit *unit) {
    f32 pos[4];
    BtlUnit *other = ((BtlWork *)func_001AA6F8())->actorList;
    u32 mask = unit->link18->flags & 0x600;
    for (; other != NULL; other = other->nextActor) {
        if ((other->flags & 1) && (other->flags & mask) && other != unit->link18) {
            func_001E2220(other);
            if ((other->flags64 & 0x102) == 0x102) {
                func_001E3108((u8 *)other, (s128 *)pos);
                pos[1] += 1000000.0f;
                pos[0] = 0;
                effObjSetInnerFirstVec(other->effectObject, pos);
                btlSetUnitPosition(other, pos);
            }
        }
    }
    func_001E3108((u8 *)unit->link18, (s128 *)pos);
    pos[0] = 0;
    btlSetUnitPosition(unit->link18, pos);
}

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

u32 func_002076E0(u32 *command) {
    s32 category = command[1];
    if (category >= 0x1AB) {
        return 1;
    }
    if (category >= 0x5E) {
        return 1;
    }
    if (category >= 0x5B) {
        func_002060B0((BtlUnit *)command[0]);
    }
    return 1;
}

SoundTask *func_00207728(BtlUnit *unit, s32 arg1, s32 arg2) {
    SoundTask *task = btlAllocTask(12);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_002076E0;
    task->taskId = 0x64;
    task->onStart = 0;
    task->owner = unit->link18->owner;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = arg1;
    args->unk_08 = arg2;
    return task;
}

INCLUDE_ASM(const s32, "game/code_001DACF8", func_002077C0);

extern u32 func_002077C0(u32 *);

SoundTask *func_00207958(BtlUnit *unit, u32 soundId, u32 variant, u32 channel, u32 flags) {
    SoundTask *task = btlAllocTask(20);
    SoundTaskArgs *args;
    task->enabled = 1;
    task->status = 0;
    task->callback = func_002077C0;
    task->taskId = 0x65;
    task->onStart = 0;
    task->owner = unit->link18->owner;
    args = func_001E14F8((s32)task);
    args->actor = unit;
    args->option = soundId;
    args->unk_08 = variant;
    args->unk_0C = channel;
    args->unk_10 = flags;
    return task;
}

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

