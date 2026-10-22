#ifndef MDL_OPTION_STATE_H
#define MDL_OPTION_STATE_H

#include "common.h"
#include "sdf_draw.h"

typedef union MdlOptionControl {
    u32 packed;
    struct {
        u8 active;
        u8 blink;
        s16 cursor;
    } fields;
} MdlOptionControl;

/* Complete option-panel state used by the FACE-DIV soften/bias task. */
typedef struct MdlOptionState {
    s8 *pad;
    MdlOptionControl control;
    u8 pad08[0x20];
    f32 value;
    f32 adjustment;
    sdfDrawTag_t *packetList;
    u8 pad34[4];
} MdlOptionState;

typedef char MdlOptionStateSize[sizeof(MdlOptionState) == 0x38 ? 1 : -1];

#endif
