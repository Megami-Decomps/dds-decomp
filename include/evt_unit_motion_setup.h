#ifndef EVT_UNIT_MOTION_SETUP_H
#define EVT_UNIT_MOTION_SETUP_H

#include "common.h"
#include "evt_unit.h"

/* Typed view of the three stored motion setup halfwords at EvtUnit +0xC4. */
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
    ((u32)&((EvtUnit *)0)->motionSetupId == 0xC4) ? 1 : -1];
typedef char EvtUnitMotionSetupStartOverlayOffsetCheck[
    ((u32)&((EvtUnit *)0)->motionSetupStartFrame == 0xC6) ? 1 : -1];
typedef char EvtUnitMotionSetupEndOverlayOffsetCheck[
    ((u32)&((EvtUnit *)0)->motionSetupEndFrame == 0xC8) ? 1 : -1];

/* Overlay the six-byte record on EvtUnit bytes +0xC4..+0xC9. */
#define EVT_UNIT_MOTION_SETUP(unit) \
    ((EvtUnitMotionSetup *)((u8 *)(unit) + 0xC4))

#endif /* EVT_UNIT_MOTION_SETUP_H */
