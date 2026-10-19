#ifndef MDL_BROADCAST_CALLBACK_H
#define MDL_BROADCAST_CALLBACK_H

#include "mdl.h"

static inline void mdlApplyBroadcastVia(
        void (*apply)(MdlCtx *, u32), MdlCtx *model, u32 value) {
    apply(model, value);
}

#endif
