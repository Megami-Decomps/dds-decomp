#ifndef FLD_ROOM_PROBE_PLANE_RESULT_H
#define FLD_ROOM_PROBE_PLANE_RESULT_H

#include "common.h"

/* Tests the four side planes; the room's base plane is handled separately. */
typedef s32 FldRoomProbePlaneResult;
enum {
    FLD_ROOM_PROBE_PLANES_REJECTED = -1,
    FLD_ROOM_PROBE_PLANES_PASSED = 0,
};

FldRoomProbePlaneResult fldProbeRoomPlanes(f32 *direction, s32 roomIndex);

#endif /* FLD_ROOM_PROBE_PLANE_RESULT_H */
