#ifndef FILE_CONFIG_TASK_H
#define FILE_CONFIG_TASK_H

#include "common.h"

struct sdfMemHandle_t;
struct MenuList;
struct EffectSlotSet;
struct MenuResourceWork;

/* Lifecycle phase for the save/config menu's input and exit tasks. */
typedef s32 FileConfigTaskState;
enum {
    FILE_CONFIG_TASK_STATE_WAITING_FOR_RESOURCES = 0,
    FILE_CONFIG_TASK_STATE_FADING_IN = 1,
    FILE_CONFIG_TASK_STATE_ACTIVE = 2,
    FILE_CONFIG_TASK_STATE_BEGIN_CLOSE = 3,
    FILE_CONFIG_TASK_STATE_FADING_OUT = 4,
    FILE_CONFIG_TASK_STATE_CLOSE_COMPLETE = 5
};

/* Set after the close-complete phase to tell both owner tasks to exit. */
#define FILE_CONFIG_TASK_EXIT_FLAG 0x80000000

/* Save/config menu owner; slot count and tail differ between the games. */
typedef struct FileConfigTask {
    struct sdfMemHandle_t *backingAllocation; /* 0x00 */
#ifdef VERSION_DDS2
    u32 mode; /* 0x04: selects the configuration task's display behavior. */
#else
    s32 mode; /* 0x04: selects the configuration task's display behavior. */
#endif
    s32 ticks; /* 0x08 */
    struct MenuList *frame; /* 0x0C */
#ifdef VERSION_DDS1
    struct EffectSlotSet *slots[4]; /* 0x10 */
    u8 pad20[4];
    FileConfigTaskState state; /* 0x24 */
    s16 previousIndex; /* 0x28 */
    s16 transitionTicks; /* 0x2A */
    f32 choiceFade; /* 0x2C */
    f32 labelFade; /* 0x30 */
    u32 pending; /* 0x34 */
    struct MenuResourceWork *effect; /* 0x38 */
#else
    struct EffectSlotSet *slots[5]; /* 0x10 */
    u8 pad24[4];
    FileConfigTaskState state; /* 0x28 */
    s16 previousIndex; /* 0x2C */
    s16 transitionTicks; /* 0x2E */
    f32 choiceFade; /* 0x30 */
    f32 labelFade; /* 0x34 */
    u32 pending; /* 0x38 */
#endif
} FileConfigTask;

typedef char FileConfigTask_size_check[
    (sizeof(FileConfigTask) == 0x3C) ? 1 : -1];
typedef char FileConfigTask_mode_offset_check[
	((u32)&((FileConfigTask *)0)->mode == 0x04) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char FileConfigTask_state_offset_check[
    ((u32)&((FileConfigTask *)0)->state == 0x24) ? 1 : -1];
typedef char FileConfigTask_pending_offset_check[
    ((u32)&((FileConfigTask *)0)->pending == 0x34) ? 1 : -1];
typedef char FileConfigTask_effect_offset_check[
    ((u32)&((FileConfigTask *)0)->effect == 0x38) ? 1 : -1];
#else
typedef char FileConfigTask_state_offset_check[
    ((u32)&((FileConfigTask *)0)->state == 0x28) ? 1 : -1];
typedef char FileConfigTask_pending_offset_check[
    ((u32)&((FileConfigTask *)0)->pending == 0x38) ? 1 : -1];
#endif

#endif /* FILE_CONFIG_TASK_H */
