#ifndef EVT_SEL_STATE_H
#define EVT_SEL_STATE_H

#include "common.h"

/* Values stored at offset zero and dispatched by the interpolation assembly. */
typedef u32 EvtSelStateMode;
enum {
    EVT_SEL_STATE_MODE_RAMP_UP = 0,
    EVT_SEL_STATE_MODE_HOLD = 1,
    EVT_SEL_STATE_MODE_RAMP_DOWN = 2,
};
#define EVT_SEL_STATE_UNBOUNDED_LIMIT (-1)

/* The selection shake: a per-axis offset that ramps up, holds and ramps down. */
typedef struct EvtSelState {
    EvtSelStateMode mode;  /* 0x00: current interpolation phase */
    s32 limit;              /* 0x04: deadline frame; unbounded sentinel never ends */
    s32 rampUpFrames;   /* 0x08: frames to ramp up over */
    s32 rampDownFrames; /* 0x0C: frames to ramp down over */
    s16 amplitude[3];  /* 0x10: per-axis peak shake offset */
    u8 pad16[2];
    s32 count;         /* 0x18: frames elapsed */
    f32 offset[3];     /* 0x1C: current per-axis shake limit */
} EvtSelState;
typedef char EvtSelState_size_check[(sizeof(EvtSelState) == 0x28) ? 1 : -1];

#endif
