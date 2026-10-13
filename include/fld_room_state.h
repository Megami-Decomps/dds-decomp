#ifndef FLD_ROOM_STATE_H
#define FLD_ROOM_STATE_H

#include "common.h"

/* Shared room geometry/runtime record. DDS1 and DDS2 use the same 0x140-byte
 * layout; unclassified state words and the intervening gap retain their raw
 * widths and positions. */
typedef struct FldRoomState {
    f32 corner[8][4]; /* 0x00 */
    f32 center[4];    /* 0x80 */
    f32 plane[6][4];  /* 0x90 */
    f32 limit[6];     /* 0xF0 */
    s32 worldObjectKey; /* 0x108: compared with an EffWorldNode key */
    f32 unk10C;
    f32 unk110;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    s32 unk120;
    u8 pad124[0xC];
    s16 unk130;
    s16 roomId;       /* 0x132: returned by fldFindRoomByTask */
    s16 unk134;
    s16 mode;
    s16 unk138;
    s16 axisMode;
    s32 shapeKind;    /* 0x13C: compared with FldTaskInfo.shape->kind */
} FldRoomState;

typedef char FldRoomStateSizeCheck[(sizeof(FldRoomState) == 0x140) ? 1 : -1];
typedef char FldRoomStateWorldObjectKeyOffsetCheck[
    ((u32)&((FldRoomState *)0)->worldObjectKey == 0x108) ? 1 : -1];
typedef char FldRoomStateRoomIdOffsetCheck[
    ((u32)&((FldRoomState *)0)->roomId == 0x132) ? 1 : -1];
typedef char FldRoomStateShapeKindOffsetCheck[
    ((u32)&((FldRoomState *)0)->shapeKind == 0x13C) ? 1 : -1];

#endif /* FLD_ROOM_STATE_H */
