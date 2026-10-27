#ifndef EFF_SLOT_PHASE_H
#define EFF_SLOT_PHASE_H

#include "common.h"

/* Per-update signed 16.16 phase delta; negative callback results hold the phase. */
typedef s32 EffSlotPhaseStep;

enum {
    EFF_SLOT_PHASE_ONE = 0x10000, /* 1.0 in signed 16.16 */
};

#endif /* EFF_SLOT_PHASE_H */
