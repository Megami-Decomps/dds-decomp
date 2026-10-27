#ifndef FLD_SEQUENCE_CONTROLLER_H
#define FLD_SEQUENCE_CONTROLLER_H

#include "common.h"

/* Field-scene lifecycle states; DDS2 also pauses for its target-guide archive. */
typedef s32 FldSequencePhase;
enum FldSequencePhaseValue {
    FLD_SEQUENCE_PHASE_INITIALIZE = 0,
    FLD_SEQUENCE_PHASE_WAIT_FOR_BGM_ARCHIVE = 1,
    FLD_SEQUENCE_PHASE_ADVANCE_TO_AREA_ARCHIVE = 2,
    FLD_SEQUENCE_PHASE_WAIT_FOR_AREA_ARCHIVE = 3,
    FLD_SEQUENCE_PHASE_ACTIVE = 4,
    FLD_SEQUENCE_PHASE_EXIT_PREPARATION = 5,
    FLD_SEQUENCE_PHASE_NOOP = 6,
    FLD_SEQUENCE_PHASE_RELEASE_NEXT_AREA = 7,
    FLD_SEQUENCE_PHASE_WAIT_FOR_TARGET_GUIDE_ARCHIVE = 8
};

typedef struct FldSequenceController {
    u32 flags;
    FldSequencePhase phase;
} FldSequenceController;

typedef char FldSequenceController_size_must_be_0x08[
    (sizeof(FldSequenceController) == 0x08) ? 1 : -1];
typedef char FldSequenceController_phase_offset_must_be_0x04[
    ((u32)&((FldSequenceController *)0)->phase == 0x04) ? 1 : -1];

#endif
