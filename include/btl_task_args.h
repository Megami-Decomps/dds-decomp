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

#endif /* BTL_TASK_ARGS_H */
