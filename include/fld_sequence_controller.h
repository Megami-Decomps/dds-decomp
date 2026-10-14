#ifndef FLD_SEQUENCE_CONTROLLER_H
#define FLD_SEQUENCE_CONTROLLER_H

#include "common.h"

typedef struct FldSequenceController {
    u32 flags;
    s32 stage;
} FldSequenceController;

typedef char FldSequenceController_size_must_be_0x08[
    (sizeof(FldSequenceController) == 0x08) ? 1 : -1];
typedef char FldSequenceController_stage_offset_must_be_0x04[
    ((u32)&((FldSequenceController *)0)->stage == 0x04) ? 1 : -1];

#endif
