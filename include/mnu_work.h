#ifndef MNU_WORK_H
#define MNU_WORK_H
#include "common.h"

struct MnuShootingWork;
struct MnuModelNode;
struct ModelInstance;

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

/* Primary work pool: 100 records occupy the constructor's 0x1C20 allocation. */
typedef struct MenuWorkEntry {
    MenuWorkControl control;
    u32 tag;
    s32 unk08;
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
        struct {
            u32 active : 1;
            u32 updated : 1;
            u32 finished : 1;
            u32 pendingDeactivate : 1; /* bit 3: dispatch then deactivate */
            u32 pendingStart : 1; /* bit 4: set by mnuCreateAnimatedEffect */
            u32 unk5 : 1;
            u32 unk6 : 8;
            u32 unk14 : 5;
            u32 modelMotionStarted : 1;
            u32 unk20 : 12;
        } flagsBits;
    };
    u8 pad44[4];
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
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 angle;
    f32 unk18;
    f32 unk1C;
    s16 speed;
    s16 remaining;
} MenuRuntimeRecord;

typedef char MenuWorkLayoutAssert[(sizeof(MenuWorkControl)==4 && sizeof(MenuWorkEntry)==0x48 &&
    (unsigned long)&((MenuWorkEntry*)0)->control==0 &&
    (unsigned long)&((MenuWorkEntry*)0)->tag==4 &&
    (unsigned long)&((MenuWorkEntry*)0)->unk08==8 &&
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
    sizeof(((MenuWorkEntry*)0)->pad44)==4)?1:-1];
typedef char MenuRuntimeLayoutAssert[(sizeof(MenuRuntimeRecord)==0x24 && sizeof(MenuRuntimeState)==4 &&
    (unsigned long)&((MenuRuntimeState*)0)->flags==0 &&
    (unsigned long)&((MenuRuntimeState*)0)->kind==1 &&
    (unsigned long)&((MenuRuntimeState*)0)->directionDegrees==2 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk04==4 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk08==8 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk0C==0x0C &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk10==0x10 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->angle==0x14 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk18==0x18 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->unk1C==0x1C &&
    (unsigned long)&((MenuRuntimeRecord*)0)->speed==0x20 &&
    (unsigned long)&((MenuRuntimeRecord*)0)->remaining==0x22)?1:-1];

typedef void (*MenuWorkCallback)(MenuWorkEntry *, struct MnuShootingWork *);
typedef void (*MenuRuntimeCallback)(MenuRuntimeRecord *);
typedef void (*MenuRuntimeWorkCallback)(MenuRuntimeRecord *, MenuWorkEntry *, struct MnuShootingWork *);
typedef void (*MenuRuntimePairCallback)(MenuRuntimeRecord *, MenuRuntimeRecord *, struct MnuShootingWork *);

typedef struct MenuProgressParameters {
    u16 width;
    u16 height;
    u32 word04;
    s32 x;
    s32 y;
} MenuProgressParameters;

typedef struct MenuRegistryParameters {
    u8 pad00[0x26];
    s16 unk26;
    u8 pad28[2];
    u16 unk2A;
    u8 pad2C[4];
} MenuRegistryParameters;

typedef struct MenuRegistryTable MenuRegistryTable;

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
     (unsigned long)&((MenuRegistry*)0)->unk08==8 &&
     sizeof(((MenuRegistry*)0)->unk08)==2 &&
     sizeof(MenuRegistryParameters)==0x30 &&
     (unsigned long)&((MenuRegistryParameters*)0)->unk26==0x26 &&
     (unsigned long)&((MenuRegistryParameters*)0)->unk2A==0x2A &&
     sizeof(MenuRegistry)==0x1C &&
     (unsigned long)&((MenuRegistry*)0)->parameterIndex==4 &&
     (unsigned long)&((MenuRegistry*)0)->table==0x0C &&
     (unsigned long)&((MenuRegistry*)0)->score==0x18 &&
     (unsigned long)&((MenuRegistry*)0)->progress==0x1A)?1:-1];

MenuProgressParameters *mnuGetResourceProgressParameters(void);
void mnuCopyResourceProgressParameters(MenuProgressParameters *);
void mnuBindMenuRecordRegistry(MenuRegistry *, u32);
MenuRegistry *mnuGetMenuRecordRegistryEntry(u32);
void func_00322540(MenuRegistryParameters *, u32);
MenuRegistryParameters *func_00322550(u32);

void func_00323918(MenuWorkCallback);
void func_00323920(MenuWorkCallback);
void mnuSetActiveWorkVisitor(MenuWorkCallback);
void func_003214C8(MenuRuntimeCallback);
void func_00323930(MenuRuntimeWorkCallback);
void func_00323938(MenuRuntimePairCallback);

void func_003191B0(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319388(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319A58(MenuWorkEntry *, struct MnuShootingWork *);
void func_00319E48(MenuRuntimeRecord *);
void func_0031A288(MenuRuntimeRecord *, MenuWorkEntry *, struct MnuShootingWork *);
void func_0031A638(MenuRuntimeRecord *, MenuRuntimeRecord *, struct MnuShootingWork *);
#endif
