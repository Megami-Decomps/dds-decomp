#ifndef EFF_EVENT_WORK_API_H
#define EFF_EVENT_WORK_API_H

#include "eff.h"
#include "eff_blur.h"

/* Return the live work blocks owned by the corresponding event effect. */
effBlurScaleWork_t *Eve_RippleBlurGetParams(void);
dds3Effect_PartialMultiFilter_t *Eve_PartialMultiFilterGetParams(void);

#endif
