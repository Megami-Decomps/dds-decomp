#ifndef BTL_ACTION_H
#define BTL_ACTION_H

#include "common.h"

/* Native 0x70-byte scheduler-task header, distinct from a queued command actor.
 * The condition evaluator reads the first byte and +8 query key; task startup
 * assigns handle, while owner lookup uses ownerId. Scene request constructors
 * share its task-ID/status prefix and native start/update callback slots.
 */
typedef struct BtlRuntimeTask {
    u8 conditionKind;
    u8 pad01[7];
    u64 conditionHandle;
    u8 status; /* 0x10 */
    u8 pad11[7];
    u64 secondaryConditionHandle; /* 0x18: the second dependency checked with +0x10 */
    u16 taskId; /* 0x20 */
    u8 pad22[2];
    u16 flags; /* 0x24: command tasks set bit 1 before starting */
    u8 pad26[2];
    s32 startDelay; /* Signed countdown before the task's running phase. */
    u8 pad2C[0xC];
    u64 handle;
    u64 ownerId;
    void (*onStart)(); /* 0x48: native start callback, with unspecified arguments. */
    s32 (*callback)(); /* 0x4C: returns nonzero when the task has completed. */
    u8 pad50[0x20];
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
