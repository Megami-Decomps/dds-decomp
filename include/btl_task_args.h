#ifndef BTL_TASK_ARGS_H
#define BTL_TASK_ARGS_H

#include "common.h"

/* Return the runtime scheduler task's allocated argument block, or NULL. */
void *btlGetTaskArguments(void *task);

/* The scene scheduler copies these request records into its argument block. */
typedef struct BtlSceneCounterArgs {
#ifdef VERSION_DDS1
    struct BtlTask *actor;
#else
    struct ActionStateLink *actor;
#endif
    u32 amount;
    u8 mode;
    u8 pad09[3];
} BtlSceneCounterArgs;

typedef struct BtlSceneInsertArgs {
#ifdef VERSION_DDS1
    struct BtlTask *actor;
#else
    struct ActionStateLink *actor;
#endif
    u32 count;
} BtlSceneInsertArgs;

/* The model-blend task allocates and initializes this complete 0x1C-byte packet. */
typedef struct BtlActorModelBlendArgs {
    struct BtlUnit *unit;
    struct BtlUnit *target;
    s32 index;
    s32 previousModelValue;
    s32 value;
    f32 scale;
    u32 stage;
} BtlActorModelBlendArgs;
typedef char BtlActorModelBlendArgsSizeCheck[sizeof(BtlActorModelBlendArgs) == 0x1C ? 1 : -1];

#endif /* BTL_TASK_ARGS_H */
