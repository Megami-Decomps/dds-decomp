#ifndef FLD_ROOM_STATE_H
#define FLD_ROOM_STATE_H

#include "common.h"

/* Capacities shared by field-record allocation and reset code. */
enum {
    FIELD_VALUE_RECORD_STORAGE_SIZE = 0x72000,
    FIELD_AUX_RECORD_STORAGE_SIZE = 0x4A00,
    FIELD_ROOM_RECORD_COUNT = 64,
    FIELD_ROOM_CORNER_COUNT = 8,
    FIELD_ROOM_PLANE_COUNT = 6,
    FIELD_ACTOR_SLOT_COUNT = 256
};

/* Interaction modes stored in FldRoomState.mode; the field remains s16. */
typedef s16 FldRoomInteractionMode;
enum FldRoomInteractionModeValue {
    FLD_ROOM_INTERACTION_NONE = 0,
    FLD_ROOM_INTERACTION_NPC = 1,
    FLD_ROOM_INTERACTION_ITEM_BOX = 2,
    FLD_ROOM_INTERACTION_SAVE_POINT = 3,
    FLD_ROOM_INTERACTION_HEAL_POINT = 4
};

/* Stored selector for the two-dimensional projection used by room probes. */
typedef s16 FldRoomProbePlaneMode;

/* Room IDs are stored in room records; -1 means a task has no room. */
typedef s16 FldRoomId;

/* Shared room geometry/runtime record. DDS1 and DDS2 use the same 0x140-byte
 * layout; unclassified state words and the intervening gap retain their raw
 * widths and positions. */
typedef struct FldRoomState {
    f32 corner[FIELD_ROOM_CORNER_COUNT][4]; /* 0x00 */
    f32 center[4];    /* 0x80 */
    f32 plane[FIELD_ROOM_PLANE_COUNT][4];  /* 0x90 */
    f32 limit[FIELD_ROOM_PLANE_COUNT];     /* 0xF0 */
    s32 worldObjectKey; /* 0x108: compared with an dds3Basic_t key */
    f32 sizeX;        /* 0x10C: the trigger shape's extents on its own axes */
    f32 sizeY;
    f32 sizeZ;
    f32 unk118;
    f32 unk11C;
    s32 savePointValue; /* 0x120: a save point's (mode 3) spawn value, else -1 */
    u8 pad124[0xC];
    s16 unk130;
    FldRoomId roomId; /* 0x132: returned by fldFindRoomIdByTask */
    s16 mapSlot;      /* 0x134: above 0, the map slot whose flag hides the trigger */
    FldRoomInteractionMode mode; /* 0x136: selects the room's interaction behavior. */
    s16 entryIndex; /* 0x138: active entry index when mode is 1. */
    /* Probes use YZ when zero and XY for any nonzero mode. */
    FldRoomProbePlaneMode probePlaneMode;
    s32 crossingDirection; /* 0x13C: a plane trigger's last crossing inside the room (1, -1; 0 none);
                            * its probe tests it against the plane kind, 1 */
} FldRoomState;

typedef char FldRoomStateSizeCheck[(sizeof(FldRoomState) == 0x140) ? 1 : -1];
typedef char FldRoomStateWorldObjectKeyOffsetCheck[
    ((u32)&((FldRoomState *)0)->worldObjectKey == 0x108) ? 1 : -1];
typedef char FldRoomStateRoomIdOffsetCheck[
    ((u32)&((FldRoomState *)0)->roomId == 0x132) ? 1 : -1];
typedef char FldRoomStateCrossingDirectionOffsetCheck[
    ((u32)&((FldRoomState *)0)->crossingDirection == 0x13C) ? 1 : -1];

#endif /* FLD_ROOM_STATE_H */
