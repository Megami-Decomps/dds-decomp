#ifndef BTL_PROJECTED_SCREEN_POINT_H
#define BTL_PROJECTED_SCREEN_POINT_H

#include "common.h"

typedef struct BtlProjectedScreenPoint {
    s32 x;
    s32 y;
} BtlProjectedScreenPoint;

typedef char BtlProjectedScreenPoint_size_must_be_0x08[
    (sizeof(BtlProjectedScreenPoint) == 0x08) ? 1 : -1];
typedef char BtlProjectedScreenPoint_y_offset_must_be_0x04[
    ((u32)&((BtlProjectedScreenPoint *)0)->y == 0x04) ? 1 : -1];

typedef s32 BtlProjectedScreenPointResult;
enum {
    BTL_PROJECTED_SCREEN_POINT_REJECTED = 0,
    BTL_PROJECTED_SCREEN_POINT_WRITTEN = 1,
};

/* Rejected points leave the output unchanged; written points provide screen XY. */
BtlProjectedScreenPointResult btlProjectForwardPositionToScreen(BtlProjectedScreenPoint *outPoint);

#endif /* BTL_PROJECTED_SCREEN_POINT_H */
