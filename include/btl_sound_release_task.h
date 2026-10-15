#ifndef BTL_SOUND_RELEASE_TASK_H
#define BTL_SOUND_RELEASE_TASK_H

#include "common.h"

struct BtlRuntimeTask;

struct BtlRuntimeTask *sndCreateReleaseTask(u32 frames);
struct BtlRuntimeTask *btlCreateSoundReleaseTask(u32 frames);

#endif /* BTL_SOUND_RELEASE_TASK_H */
