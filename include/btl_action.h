#ifndef BTL_ACTION_H
#define BTL_ACTION_H

#include "common.h"

/* Native 0x70-byte scheduler-task header, distinct from a queued command actor.
 * The condition evaluator reads the first byte and +8 query key; task startup
 * assigns handle, while owner lookup uses ownerId. Scene request constructors
 * share its task-ID/status prefix and native start/update callback slots.
 */
typedef struct BtlTaskCondition {
    u8 kind;
    u8 pad01[7];
    union {
        s32 count;
        u64 handle;
        u64 owner;
        u16 taskKind;
    } value;
} BtlTaskCondition;

typedef struct BtlRuntimeTask {
    BtlTaskCondition startCondition; /* 0x00 */
    BtlTaskCondition endCondition; /* 0x10 */
    u16 taskId; /* 0x20 */
    u16 state; /* 0x22 */
    u16 flags; /* 0x24: command tasks set bit 1 before starting */
    u8 pad26[2];
    s32 startDelay; /* Signed countdown before the task's running phase. */
    s32 endDelay;
    u32 pollCount;
    u32 runCount;
    u64 handle; /* 0x38 */
    u64 ownerId; /* 0x40 */
    void (*onStart)(); /* 0x48: each task kind owns its argument packet type. */
    s32 (*callback)(); /* 0x4C */
    void (*onFinish)(); /* 0x50 */
    void *args; /* 0x54 */
    struct BtlRuntimeTask *next; /* 0x58 */
    struct BtlRuntimeTask *prev; /* 0x5C */
    struct BtlRuntimeTask *deferNext; /* 0x60 */
    struct BtlRuntimeTask *deferPrev; /* 0x64 */
    u8 pad68[8];
} BtlRuntimeTask;

/* Eight-byte ability parameter rows; only the leading coefficient is decoded. */
typedef struct BtlAbilityParameter {
    f32 value;
    u8 unk04[4];
} BtlAbilityParameter;

#ifdef VERSION_DDS2
enum { BTL_ABILITY_PARAMETER_FIRST_SKILL = 0x220 };
#else
enum { BTL_ABILITY_PARAMETER_FIRST_SKILL = 0x200 };
#endif

extern BtlAbilityParameter *datAbilityParameters;

/* Native 0x20-byte action-animation descriptor, shared by motion and camera selection. */
typedef struct BtlActionAnimationRecord {
    u8 cameraKind; /* 0x00: values 6 and 7 select an aim pose. */
    u8 pad01[2];
    u8 kind;
    u16 displayCode;
    u8 pad06[0x16];
    u16 flags;
    u8 pad1E[2];
} BtlActionAnimationRecord;

/* Group policy takes precedence; target policy falls back to group framing
 * unless exactly one target is present. DDS1's fixed-preset bit selects explicit
 * camera keys, while the DDS2 consumers retain identical arms for that bit. */
enum {
    BTL_ANIMATION_FIXED_DEFEAT_CAMERA = 0x10,
    BTL_ANIMATION_GROUP_DEFEAT_CAMERA = 0x1000,
    BTL_ANIMATION_TARGET_DEFEAT_CAMERA = 0x2000
};

#endif /* BTL_ACTION_H */
