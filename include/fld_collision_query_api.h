#ifndef FLD_COLLISION_QUERY_API_H
#define FLD_COLLISION_QUERY_API_H

#include "common.h"

/* Segment sweeps return the intersected FldValueRecord index; -1 means no hit. */
typedef s32 FldValueRecordIndex;
enum {
    FLD_VALUE_RECORD_INDEX_NO_HIT = -1
};

FldValueRecordIndex fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);
FldValueRecordIndex fldHitChkEnemy(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

#endif /* FLD_COLLISION_QUERY_API_H */
