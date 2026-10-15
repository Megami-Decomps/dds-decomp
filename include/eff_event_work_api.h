#ifndef EFF_EVENT_WORK_API_H
#define EFF_EVENT_WORK_API_H

#include "eff.h"
#include "eff_blur.h"

/* Return the live work blocks owned by the corresponding event effect. */
EffBlurScaleWork *effGetCh76Work(void);
EffResourceRectWork *effGetCh75Work(void);

#endif
