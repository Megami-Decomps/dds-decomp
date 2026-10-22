#ifndef MDL_BROADCAST_CALLBACK_H
#define MDL_BROADCAST_CALLBACK_H

#include "mdl.h"

static inline void mdlApplyBroadcastVia(
        void (*apply)(dds3ModelHandle_t *, u32), dds3ModelHandle_t *model, u32 value) {
    apply(model, value);
}

#endif
