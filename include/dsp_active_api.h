#ifndef DSP_ACTIVE_API_H
#define DSP_ACTIVE_API_H

#include "common.h"

/* Update the current message window's gate and disable flags. Enabling also
 * marks its panel active; disabling leaves its control-state byte unchanged.
 * Callers must provide a valid current window. */
void dspSetActive(s32 enabled);

#endif /* DSP_ACTIVE_API_H */
