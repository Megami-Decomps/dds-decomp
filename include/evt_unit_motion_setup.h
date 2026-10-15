#ifndef EVT_UNIT_MOTION_SETUP_H
#define EVT_UNIT_MOTION_SETUP_H

#include "common.h"
#include "evt_unit.h"

/* Provider-local view of EvtUnit's three stored motion setup words at +0xC4. */
typedef struct EvtUnitMotionSetup {
    s16 motionId;
    s16 startFrame;
    s16 endFrame;
} EvtUnitMotionSetup;

typedef char EvtUnitMotionSetupSizeCheck[(sizeof(EvtUnitMotionSetup) == 6) ? 1 : -1];
typedef char EvtUnitMotionSetupMotionIdOffsetCheck[
    ((u32)&((EvtUnitMotionSetup *)0)->motionId == 0) ? 1 : -1];
typedef char EvtUnitMotionSetupStartFrameOffsetCheck[
    ((u32)&((EvtUnitMotionSetup *)0)->startFrame == 2) ? 1 : -1];
typedef char EvtUnitMotionSetupEndFrameOffsetCheck[
    ((u32)&((EvtUnitMotionSetup *)0)->endFrame == 4) ? 1 : -1];
typedef char EvtUnitMotionSetupOverlayOffsetCheck[
    ((u32)&((EvtUnit *)0)->unkC4 == 0xC4) ? 1 : -1];

/* Overlay the six-byte record on EvtUnit bytes +0xC4..+0xC9. */
#define EVT_UNIT_MOTION_SETUP(unit) \
    ((EvtUnitMotionSetup *)((u8 *)(unit) + 0xC4))

#endif /* EVT_UNIT_MOTION_SETUP_H */
