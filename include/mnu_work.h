#ifndef MNU_WORK_H
#define MNU_WORK_H
#include "common.h"

#define MNU_WORK_TAG_CLASS_MASK 0xFFFF0000u
#define MNU_WORK_TAG_MOVEMENT_TABLE 0x01000000u
#define MNU_WORK_TAG_REGISTRY_TABLE 0x02010000u

struct MnuShootingWork;
struct MnuModelNode;
struct ModelInstance;

typedef struct MenuInputActionSnapshot {
    s8 verticalNegative;
    s8 verticalPositive;
    s8 horizontalNegative;
    s8 horizontalPositive;
    s8 actionSlot0;
    s8 actionSlot1;
    u8 reserved06[6];
} MenuInputActionSnapshot;

typedef struct MenuInputActionSnapshotStorage {
    MenuInputActionSnapshot snapshot;
    u8 unknown0C[4];
} MenuInputActionSnapshotStorage;

typedef char MenuInputActionSnapshotLayoutAssert[
    (sizeof(MenuInputActionSnapshot) == 12 &&
     sizeof(MenuInputActionSnapshotStorage) == 16 &&
     (unsigned long)&((MenuInputActionSnapshotStorage *)0)->snapshot == 0) ? 1 : -1];

void mnuSetInputActionSnapshot(const MenuInputActionSnapshot *snapshot);

/* Eight-byte initial parameters copied together into the named state record. */
typedef struct MenuInitialTag {
    u8 reserved;
    u8 flags;
    s16 group;
    s16 duration;
    s16 index;
} MenuInitialTag;

/* Named-record constructors allocate and clear exactly 0x22 bytes. */
typedef struct MenuStateRecord {
    union {
        u16 word;
        struct {
            u16 completed : 1;
            u16 valueGated : 1;
            u16 hasRange : 1;
            u16 waitPending : 1;
            u16 unk4 : 1;
            u16 direction : 4;
            u16 unk9 : 7;
        } bits;
    } flags;
    u8 pad02[2];
    u16 waitCount;
    s16 elapsedCount;
    s16 value;
    MenuInitialTag tag; /* 0x0A: copied as one eight-byte group. */
    u8 pad12[2];
    s16 waitLimit;
    s16 limit;
    s16 unk18;
    u8 pad1A[2];
    s16 offsetX;
    s16 offsetY;
    s16 effect;
} MenuStateRecord;

typedef char MenuStateRecordLayoutAssert[
    (sizeof(MenuInitialTag) == 8 && sizeof(MenuStateRecord) == 0x22 &&
     (unsigned long)&((MenuStateRecord *)0)->tag == 0x0A &&
     (unsigned long)&((MenuStateRecord *)0)->unk18 == 0x18 &&
     (unsigned long)&((MenuStateRecord *)0)->effect == 0x20) ? 1 : -1];

MenuStateRecord *mnuCreateNamedRecord(const void *tagData);

typedef union MenuWorkControl {
    u32 word;
    u8 bytes[4];
    struct {
        u32 loopMode : 4;
        u32 repeatMode : 4;
        u32 countdownEnabled : 1;
        u32 countdown : 8;
        u32 unused17 : 15;
    } bits;
} MenuWorkControl;

typedef struct MenuWorkFlagBits {
    u32 active : 1;
    u32 updated : 1;
    u32 finished : 1;
    u32 pendingDeactivate : 1; /* bit 3: dispatch then deactivate */
    u32 pendingStart : 1; /* bit 4: set by mnuCreateAnimatedEffect */
    u32 halfRemainingCountReached : 1; /* bit 5: latched at half the initial remaining count */
    u32 unk6 : 8;
    u32 inputDisabled : 1;
    u32 mode : 4;
    u32 modelMotionStarted : 1;
    u32 unk20 : 12;
} MenuWorkFlagBits;

typedef union MenuWorkFlags {
    u32 word;
    MenuWorkFlagBits bits;
} MenuWorkFlags;

/* Primary work pool: 100 records occupy the constructor's 0x1C20 allocation. */
typedef struct MenuWorkEntry {
    MenuWorkControl control;
    u32 tag; /* High-half class; low-half table index where that class is recognized. */
    s32 resourceRecordIndex;
    union {
        struct MnuModelNode *modelNode;
        struct ModelInstance *modelInstance;
    } object;
    f32 x0, y0, scale0;
    u8 pad1C[4];
    f32 x1, y1, scale1;
    s16 recordIndex;
    s16 shortListIndex;
    s16 frameCounter; /* 0x30: frames shown of the current short record (func_003230A0) */
    s16 repeatCount;
    u16 unk34;
    s16 remaining;
    u16 unk38;
    u16 elapsed;
    u32 callback; /* Callback-list node address, not a direct function pointer. */
    /* Retail 0x00323960 ORs the whole word with MNU_WORK_FINISHED (4).
     * 0x00323610..0x00323658 extracts bit 5 and increments/masks bits 6..13;
     * 0x0031A3F8..0x0031A404 extracts bit 5 and compares it with one. */
    union {
        u32 flags;
        MenuWorkFlagBits flagsBits;
    };
    union {
        u8 pad44[4];
        struct {
            s16 inputCountdown; /* 0x44: decremented by the active-effect updater */
            u8 pad46[2];
        };
    };
} MenuWorkEntry;

typedef union MenuRuntimeState {
    u32 word;
    struct {
        u8 flags;
        u8 kind;
        s16 directionDegrees;
    };
} MenuRuntimeState;

/* The two separately allocated runtime arrays use 0x24-byte records. */
typedef struct MenuRuntimeRecord {
    MenuRuntimeState state;
    f32 baseX;
    f32 baseY;
    f32 rotatedOffsetX;
    f32 rotatedOffsetY;
    f32 angle;
    f32 displacementX;
    f32 displacementY;
    s16 speed;
    s16 remaining;
} MenuRuntimeRecord;

/* Static owners for runtime-record arrays supplied by the menu setup path. */
typedef struct MenuRuntimeList {
    MenuRuntimeRecord *records;
    s32 capacity;
    u32 activeCount;
    u32 unk0C;
} MenuRuntimeList;

typedef char MenuWorkLayoutAssert[(sizeof(MenuWorkControl)==4 && sizeof(MenuWorkFlags)==4 && sizeof(MenuWorkEntry)==0x48 &&
    (unsigned long)&((MenuWorkEntry*)0)->control==0 &&
    (unsigned long)&((MenuWorkEntry*)0)->tag==4 &&
    (unsigned long)&((MenuWorkEntry*)0)->resourceRecordIndex==8 &&
    sizeof(((MenuWorkEntry*)0)->object)==4 &&
    (unsigned long)&((MenuWorkEntry*)0)->object==0x0C &&
    (unsigned long)&((MenuWorkEntry*)0)->x0==0x10 &&
    (unsigned long)&((MenuWorkEntry*)0)->y0==0x14 &&
    (unsigned long)&((MenuWorkEntry*)0)->scale0==0x18 &&
    (unsigned long)&((MenuWorkEntry*)0)->pad1C==0x1C &&
    sizeof(((MenuWorkEntry*)0)->pad1C)==4 &&
    (unsigned long)&((MenuWorkEntry*)0)->x1==0x20 &&
    (unsigned long)&((MenuWorkEntry*)0)->y1==0x24 &&
    (unsigned long)&((MenuWorkEntry*)0)->scale1==0x28 &&
    (unsigned long)&((MenuWorkEntry*)0)->recordIndex==0x2C &&
    (unsigned long)&((MenuWorkEntry*)0)->shortListIndex==0x2E &&
    (unsigned long)&((MenuWorkEntry*)0)->frameCounter==0x30 &&
    (unsigned long)&((MenuWorkEntry*)0)->repeatCount==0x32 &&
    (unsigned long)&((MenuWorkEntry*)0)->unk34==0x34 &&
    (unsigned long)&((MenuWorkEntry*)0)->remaining==0x36 &&
    (unsigned long)&((MenuWorkEntry*)0)->unk38==0x38 &&
    (unsigned long)&((MenuWorkEntry*)0)->elapsed==0x3A &&
    (unsigned long)&((MenuWorkEntry*)0)->callback==0x3C &&
    (unsigned long)&((MenuWorkEntry*)0)->flags==0x40 &&
    (unsigned long)&((MenuWorkEntry*)0)->pad44==0x44 &&
    sizeof(((MenuWorkEntry*)0)->pad44)==4 &&
    (unsigned long)&((MenuWorkEntry*)0)->inputCountdown==0x44 &&
    (unsigned long)&((MenuWorkEntry*)0)->pad46==0x46)?1:-1];
typedef char MenuRuntimeLayoutAssert[(sizeof(MenuRuntimeRecord)==0x24 && sizeof(MenuRuntimeState)==4 &&
    (unsigned long)&((MenuRuntimeState*)0)->flags==0 &&
    (unsigned long)&((MenuRuntimeState*)0)->kind==1 &&
    (unsigned long)&((MenuRuntimeState*)0)->directionDegrees==2 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->baseX==4 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->baseY==8 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->rotatedOffsetX==0x0C &&
    (unsigned long)&((MenuRuntimeRecord*)0)->rotatedOffsetY==0x10 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->angle==0x14 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->displacementX==0x18 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->displacementY==0x1C &&
    (unsigned long)&((MenuRuntimeRecord*)0)->speed==0x20 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->remaining==0x22)?1:-1];
typedef char MenuRuntimeListLayoutAssert[(sizeof(MenuRuntimeList)==0x10 &&
    (unsigned long)&((MenuRuntimeList*)0)->records==0 &&
    (unsigned long)&((MenuRuntimeList*)0)->capacity==4 &&
    (unsigned long)&((MenuRuntimeList*)0)->activeCount==8 &&
    (unsigned long)&((MenuRuntimeList*)0)->unk0C==0xC)?1:-1];

typedef void (*MenuWorkCallback)(MenuWorkEntry *, struct MnuShootingWork *);
typedef void (*MenuRuntimeCallback)(MenuRuntimeRecord *);
typedef void (*MenuRuntimeWorkCallback)(MenuRuntimeRecord *, MenuWorkEntry *, struct MnuShootingWork *);
typedef void (*MenuRuntimePairCallback)(MenuRuntimeRecord *, MenuRuntimeRecord *, struct MnuShootingWork *);

extern MenuWorkEntry *mnuActiveEffectEntry;
MenuWorkEntry *mnuGetActiveEffectWorkEntry(void);
void mnuInitializeActiveEffectWorkEntry(MenuWorkEntry *entry);

/* Resource-progress records use a 0x1C-byte stride. */
typedef struct MenuResourceRecord {
    u32 flags;
    u32 tag;
    s32 x;
    s32 progress;
    u8 unk10[4];
    s16 angleDegrees;
    u8 unk16[6];
} MenuResourceRecord;

/* Entries in the separately bound 0x18-byte movement table. */
typedef struct MenuMovementRecord18 {
    union {
        u8 pad00[0xA];
        struct {
            u8 pad00To04[4];
            u32 parameterTag;
            u8 pad08[2];
            u16 movementScale;
            u8 pad0C[0xC];
        };
    };
} MenuMovementRecord18;

typedef char MenuMovementRecordLayoutAssert[
    (sizeof(MenuMovementRecord18) == 0x18 &&
     (unsigned long)&((MenuMovementRecord18 *)0)->parameterTag == 4 &&
     (unsigned long)&((MenuMovementRecord18 *)0)->movementScale == 0xA) ? 1 : -1];

typedef struct MenuProgressParameters {
    u16 width;
    u16 height;
    u32 word04;
    s32 x;
    s32 y;
} MenuProgressParameters;

typedef struct MenuRegistryParameters {
    union {
        u8 pad00[0x26];
        struct {
            u8 pad00To24[0x24];
            s16 hitOffsetX;
        };
    };
    s16 hitOffsetY;
    union {
        u8 pad28[2];
        u16 hitWidth;
    };
    u16 hitHeight;
    u8 pad2C[4];
} MenuRegistryParameters;

typedef struct MenuShortRecord {
    u8 kind;
    u8 pad01;
    s16 parameters[3];
} MenuShortRecord;

typedef struct MenuShortRecordList {
    s32 count;
    MenuShortRecord *records;
} MenuShortRecordList;

/* A registry row contains two separately traversed short-record lists. */
typedef struct MenuRegistryRecord {
    u8 pad00[4];
    u16 firstCount;
    u16 secondCount;
    MenuShortRecordList *lists;
    MenuShortRecordList *secondLists;
} MenuRegistryRecord;

typedef char MenuRegistryRecordLayoutAssert[
    (sizeof(MenuShortRecord) == 8 &&
     sizeof(MenuShortRecordList) == 8 &&
     sizeof(MenuRegistryRecord) == 0x10 &&
     (unsigned long)&((MenuRegistryRecord *)0)->firstCount == 4 &&
     (unsigned long)&((MenuRegistryRecord *)0)->secondCount == 6 &&
     (unsigned long)&((MenuRegistryRecord *)0)->lists == 8 &&
     (unsigned long)&((MenuRegistryRecord *)0)->secondLists == 0xC) ? 1 : -1];

/* Accessed registry-table prefix; the backing object's total extent is unknown. */
typedef struct MenuRegistryTable {
    u32 flags;
    u8 pad04[0xA];
    u16 recordCount;
    MenuRegistryRecord *recordBase;
} MenuRegistryTable;

typedef char MenuRegistryTablePrefixLayoutAssert[
    ((unsigned long)&((MenuRegistryTable *)0)->recordCount == 0xE &&
     (unsigned long)&((MenuRegistryTable *)0)->recordBase == 0x10) ? 1 : -1];

/* Registry entries are addressed with a 0x1C-byte stride. */
typedef struct MenuRegistry {
    u8 pad00[4];
    u32 parameterIndex;
    s16 unk08; /* 0x08: signed half-value threshold in func_00323748 */
    u8 pad0A[2];
    MenuRegistryTable *table;
    u8 pad10[8];
    u16 score;
    u16 progress;
} MenuRegistry;

typedef char MenuResourceLayoutsAssert[
    (sizeof(MenuProgressParameters)==0x10 &&
     sizeof(MenuResourceRecord)==0x1C &&
     (unsigned long)&((MenuResourceRecord*)0)->tag==4 &&
     (unsigned long)&((MenuResourceRecord*)0)->x==8 &&
     (unsigned long)&((MenuResourceRecord*)0)->progress==0x0C &&
     (unsigned long)&((MenuResourceRecord*)0)->angleDegrees==0x14 &&
     (unsigned long)&((MenuRegistry*)0)->unk08==8 &&
     sizeof(((MenuRegistry*)0)->unk08)==2 &&
     sizeof(MenuRegistryParameters)==0x30 &&
     (unsigned long)&((MenuRegistryParameters*)0)->hitOffsetX==0x24 &&
     sizeof(((MenuRegistryParameters*)0)->hitOffsetX)==2 &&
     (unsigned long)&((MenuRegistryParameters*)0)->hitWidth==0x28 &&
     sizeof(((MenuRegistryParameters*)0)->hitWidth)==2 &&
     (unsigned long)&((MenuRegistryParameters*)0)->hitOffsetY==0x26 &&
     sizeof(((MenuRegistryParameters*)0)->hitOffsetY)==2 &&
     (unsigned long)&((MenuRegistryParameters*)0)->hitHeight==0x2A &&
     sizeof(((MenuRegistryParameters*)0)->hitHeight)==2 &&
     sizeof(MenuRegistry)==0x1C &&
     (unsigned long)&((MenuRegistry*)0)->parameterIndex==4 &&
     (unsigned long)&((MenuRegistry*)0)->table==0x0C &&
     (unsigned long)&((MenuRegistry*)0)->score==0x18 &&
     (unsigned long)&((MenuRegistry*)0)->progress==0x1A)?1:-1];

typedef char MenuWorkTagClassValuesAssert[
    ((MNU_WORK_TAG_CLASS_MASK & MNU_WORK_TAG_MOVEMENT_TABLE) == MNU_WORK_TAG_MOVEMENT_TABLE &&
     (MNU_WORK_TAG_CLASS_MASK & MNU_WORK_TAG_REGISTRY_TABLE) == MNU_WORK_TAG_REGISTRY_TABLE &&
     (MNU_WORK_TAG_MOVEMENT_TABLE & 0xFFFFu) == 0 &&
     (MNU_WORK_TAG_REGISTRY_TABLE & 0xFFFFu) == 0) ? 1 : -1];

void mnuBindResourceRecordTable(MenuResourceRecord *, s32);
MenuResourceRecord *mnuGetResourceRecordByIndex(s32);
MenuWorkEntry *mnuFindUnusedWorkEntry(void);
/* Initializes/registers an existing work entry from its tagged registry. */
void mnuInitializeRegistryWorkEntry(MenuWorkEntry *, u32, s32, s32, s32, f32);
MenuWorkEntry *mnuCreateAnimatedEffect(u32, f32, f32, f32);

MenuProgressParameters *mnuGetResourceProgressParameters(void);
void mnuCopyResourceProgressParameters(MenuProgressParameters *);
void mnuBindMenuRecordRegistry(MenuRegistry *, u32);
MenuRegistry *mnuGetMenuRecordRegistryEntry(u32);
MenuRegistryRecord *mnuResolveTaggedRegistryRecord(MenuWorkEntry *work);
MenuShortRecord *mnuFindFirstFixedKindShortRecord(MenuShortRecordList *list);
MenuShortRecord *func_003225C0(MenuShortRecordList *list);
void mnuBindMenuRegistryParameters(MenuRegistryParameters *records, u32 count);
/* The native selector uses the low byte; the stored count is not checked. */
MenuRegistryParameters *mnuGetMenuRegistryParametersByIndex(u32 parameterIndex);
void mnuBindMovementRecordTable(MenuMovementRecord18 *records, u32 count);
/* The native getter narrows the u32 tag to a low-16-bit movement index. */
MenuMovementRecord18 *mnuGetMovementRecordByIndex(u32 movementRecordIndex);

void mnuSetWorkEntryStartCallback(MenuWorkCallback callback);
void mnuSetWorkEntryFinishOrDeactivateCallback(MenuWorkCallback callback);
void mnuSetActiveWorkVisitor(MenuWorkCallback callback);
void mnuSetRuntimeRecordInitializationCallback(MenuRuntimeCallback callback);
void mnuSetRuntimeWorkHitCallback(MenuRuntimeWorkCallback callback);
void mnuSetRuntimeRecordPairCallback(MenuRuntimePairCallback callback);

void func_003191B0(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319388(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319A58(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319E48(MenuRuntimeRecord *);
void func_0031A288(MenuRuntimeRecord *, MenuWorkEntry *, struct MnuShootingWork *);
void func_0031A638(MenuRuntimeRecord *, MenuRuntimeRecord *, struct MnuShootingWork *);
#endif
