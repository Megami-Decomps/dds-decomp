#ifndef BTL_MODEL_RECORD_H
#define BTL_MODEL_RECORD_H

#include "common.h"

#ifdef VERSION_DDS1

/* One motion row in the side-indexed model parameter bank. */
typedef struct BtlActorMotionSlot {
    s16 moveKind;            /* 0x00: UV_MV mode */
    u16 frameCount;
    s16 kind;                /* 0x04: UV_END mode */
    s16 alphaStartFrame;
    f32 alphaFrameScale;
    f32 unk0C;
    s16 alphaDuration;
    s16 unk12;
} BtlActorMotionSlot;

/* The bank getter strides by 0x270; the editor copies the complete record.
 * The header is followed by all 29 motion rows, including opaque parameters.
 */
typedef struct BtlActorStatusRecord {
    f32 position[3];
    u8 pad0C[4];
    f32 scale;               /* 0x10: model scale */
    f32 z;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u16 shadowKind;
    u16 model;
    BtlActorMotionSlot motions[29]; /* 0x2C */
} BtlActorStatusRecord;

typedef char BtlActorMotionSlotSizeCheck[sizeof(BtlActorMotionSlot) == 0x14 ? 1 : -1];
typedef char BtlActorStatusRecordSizeCheck[sizeof(BtlActorStatusRecord) == 0x270 ? 1 : -1];

#endif

#endif
