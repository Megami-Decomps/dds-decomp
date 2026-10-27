#ifndef SDF_DISPLAY_THREAD_STATE_H
#define SDF_DISPLAY_THREAD_STATE_H

#include "common.h"

/* Byte protocol shared by the graphics worker and the vblank loop. The worker
 * marks itself waiting before SleepThread; vblank requests its wake before
 * WakeupThread; framebuffer capture marks the readback phase. */
typedef u8 SdfDisplayThreadState;

enum {
    SDF_DISPLAY_THREAD_STARTUP = 0,
    SDF_DISPLAY_THREAD_WAITING_FOR_VBLANK = 1,
    SDF_DISPLAY_THREAD_WAKE_REQUESTED = 2,
    SDF_DISPLAY_THREAD_CAPTURING_FRAME = 5,
};

typedef char SdfDisplayThreadState_size_must_be_1[
    (sizeof(SdfDisplayThreadState) == 1) ? 1 : -1];

#endif /* SDF_DISPLAY_THREAD_STATE_H */
