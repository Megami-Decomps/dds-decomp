#ifndef FLD_ROOM_RESOURCE_API_H
#define FLD_ROOM_RESOURCE_API_H

#include "common.h"

struct FldFileResource;
struct dds3Basic_t;

/* Zero-based slot shared by fldRoomResources and fldEveHitTable. */
typedef s32 FldRoomRecordIndex;

/* Registers both sides of a room record and returns their shared slot. */
FldRoomRecordIndex fldRegisterRoomObject(
    struct FldFileResource *resource, struct dds3Basic_t *object);

#endif /* FLD_ROOM_RESOURCE_API_H */
