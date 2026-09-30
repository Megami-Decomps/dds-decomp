#ifndef MNU_H
#define MNU_H

#include "common.h"

extern s64 func_002C4038(s32, s32 *, u64, u64);

static inline s64 menuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 8, (s32 *)(context + 0x54), mode, callback);
}
extern s64 func_00285670(s32, s32 *, u64, u64);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

static inline void panelSetVec4(u32 *vec, u32 red, u32 green, u32 blue, u32 alpha) {
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = alpha;
}

#endif /* MNU_H */
