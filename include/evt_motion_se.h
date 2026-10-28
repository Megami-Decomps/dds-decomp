#ifndef EVT_MOTION_SE_H
#define EVT_MOTION_SE_H

#include "common.h"
#include "evt_event_pack.h"

/* The model's world key is also the result; this sentinel means it is absent. */
typedef s32 EvtMotionSeModelKey;
enum { EVT_MOTION_SE_MODEL_KEY_NOT_FOUND = -1 };

/* MODEL_BE key shared by the model and motion-cue row lookups. */
typedef s32 EvtMotionSeResourceId;

/* Runtime user value for the event's motion-sound task. */
typedef struct EvtMotionSeTaskParams {
    EvtMotionSeModelKey modelKey;
    EvtPackEventId eventId;
    EvtMotionSeResourceId resourceId;
} EvtMotionSeTaskParams;

typedef char EvtMotionSeTaskParamsSizeCheck[sizeof(EvtMotionSeTaskParams) == 0xC ? 1 : -1];

/* Kind-5 world-node payload prefix used by the motion-sound callbacks at
 * DDS1 002420B8 / DDS2 0025D4D0: +0x08 retains the event motion object. */
typedef struct EvtMotionSeUnitLink {
    u8 unk00[8];
    struct evtUnitHandle_t *unit;
} EvtMotionSeUnitLink;

/* Type-6 BE entries contain 0x10-byte cues compared with the current frame. */
typedef struct EvtMotionSeCue {
    s32 frame;
    s32 fade;
    u8 unk08[8];
} EvtMotionSeCue;

typedef char EvtMotionSeCueSizeCheck[sizeof(EvtMotionSeCue) == 0x10 ? 1 : -1];

#endif /* EVT_MOTION_SE_H */
