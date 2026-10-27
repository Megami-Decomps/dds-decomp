#ifndef DDS3_AREA_ROOM_KEY_H
#define DDS3_AREA_ROOM_KEY_H

#include "common.h"

struct dds3Basic_t;

/* Signed packed area/room key; -1 means the world object has no location yet. */
typedef s32 Dds3AreaRoomKey;

#define DDS3_AREA_ROOM_KEY_UNASSIGNED ((Dds3AreaRoomKey)-1)

/* Legacy MapNo names access the packed (area << 16) + room key. The getter
 * returns UNASSIGNED for a NULL node; the setter ignores one. */
Dds3AreaRoomKey dds3GetMapNo(const struct dds3Basic_t *worldNode);
void dds3SetMapNo(struct dds3Basic_t *worldNode, Dds3AreaRoomKey areaRoomKey);

#endif /* DDS3_AREA_ROOM_KEY_H */
