#ifndef EVT_MODEL_HEADER_H
#define EVT_MODEL_HEADER_H

#include "common.h"

/* Shared event-model prefix; DDS2 also consumes the opaque +0x1C..+0x2B gap
 * and the target pointer at +0x2C in the model-cut opcode. */
typedef struct EvtModelHeader {
    u8 pad00[4];
    u32 flags;          /* 0x04: bit 2 selects the header transform */
    u8 pad08[8];
    f32 positionX;      /* 0x10: script-supplied translation */
    f32 positionY;      /* 0x14 */
    f32 positionZ;      /* 0x18 */
#ifdef VERSION_DDS2
    u8 pad1C[0x10];
    void *target2C;     /* 0x2C: scaled by the model-cut opcode */
#endif
} EvtModelHeader;

typedef char EvtModelHeaderFlagsOffsetCheck[
    ((u32)&((EvtModelHeader *)0)->flags == 0x04) ? 1 : -1];
typedef char EvtModelHeaderPositionXOffsetCheck[
    ((u32)&((EvtModelHeader *)0)->positionX == 0x10) ? 1 : -1];
typedef char EvtModelHeaderPositionYOffsetCheck[
    ((u32)&((EvtModelHeader *)0)->positionY == 0x14) ? 1 : -1];
typedef char EvtModelHeaderPositionZOffsetCheck[
    ((u32)&((EvtModelHeader *)0)->positionZ == 0x18) ? 1 : -1];
#ifdef VERSION_DDS2
typedef char EvtModelHeaderTargetOffsetCheck[
    ((u32)&((EvtModelHeader *)0)->target2C == 0x2C) ? 1 : -1];
#endif

#endif
