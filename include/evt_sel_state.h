#ifndef EVT_SEL_STATE_H
#define EVT_SEL_STATE_H

#include "common.h"

/* The selection shake: a per-axis offset that ramps up, holds and ramps down. */
typedef struct EvtSelState {
    u32 mode;          /* 0x00: 0 ramps the offset up, 1 holds it, 2 ramps it down */
    s32 limit;         /* 0x04: frame count at which the shake ends; -1 never ends */
    s32 unk_08;        /* 0x08: frames to ramp up over */
    s32 unk_0C;        /* 0x0C: frames to ramp down over */
    s16 amplitude[3];  /* 0x10: per-axis peak shake offset */
    u8 pad16[2];
    s32 count;         /* 0x18: frames elapsed */
    f32 offset[3];     /* 0x1C: current per-axis shake limit */
} EvtSelState;
typedef char EvtSelState_size_check[(sizeof(EvtSelState) == 0x28) ? 1 : -1];

#endif
