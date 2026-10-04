#ifndef BTL_ACTION_H
#define BTL_ACTION_H

#include "common.h"

/* Native 0x70-byte scheduler-task header, distinct from a queued command actor.
 * The condition evaluator reads the first byte and +8 query key; task startup
 * assigns handle, while owner lookup uses ownerId. The remaining SDK fields
 * beyond ownerId stay opaque here.
 */
typedef struct BtlRuntimeTask {
    u8 conditionKind;
    u8 pad01[7];
    u64 conditionHandle;
    u8 pad10[0x18];
    s32 startDelay; /* Signed countdown before the task's running phase. */
    u8 pad2C[0xC];
    u64 handle;
    u64 ownerId;
    u8 pad48[0x28];
} BtlRuntimeTask;

/* Native 0x20-byte action-animation descriptor, shared by motion and camera selection. */
typedef struct BtlActionAnimationRecord {
    u8 pad00[3];
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
