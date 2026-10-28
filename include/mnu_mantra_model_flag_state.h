#ifndef MNU_MANTRA_MODEL_FLAG_STATE_H
#define MNU_MANTRA_MODEL_FLAG_STATE_H

#include "common.h"

/* The highest active flag in 0x920-0x922 selects the available model level. */
typedef s32 MnuMantraModelFlagState;
enum {
    MNU_MANTRA_MODEL_FLAG_STATE_LEVEL_0 = 0,
    MNU_MANTRA_MODEL_FLAG_STATE_LEVEL_1 = 1,
    MNU_MANTRA_MODEL_FLAG_STATE_LEVEL_2 = 2,
    MNU_MANTRA_MODEL_FLAG_STATE_LEVEL_3 = 3,
};

MnuMantraModelFlagState mnuGetActiveMantraModelFlagState(void);

#endif /* MNU_MANTRA_MODEL_FLAG_STATE_H */
