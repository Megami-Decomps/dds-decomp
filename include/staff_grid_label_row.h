#ifndef STAFF_GRID_LABEL_ROW_H
#define STAFF_GRID_LABEL_ROW_H

#include "common.h"

#ifdef VERSION_DDS1
#define STAFF_GRID_LABEL_CAPACITY 5
#define STAFF_GRID_LABEL_PADDING_BYTES 3
#else
#define STAFF_GRID_LABEL_CAPACITY 10
#define STAFF_GRID_LABEL_PADDING_BYTES 2
#endif

typedef struct StaffGridLabelRow {
    s32 count;
    s32 x[STAFF_GRID_LABEL_CAPACITY];
    s8 gridIds[STAFF_GRID_LABEL_CAPACITY];
    u8 padding[STAFF_GRID_LABEL_PADDING_BYTES];
} StaffGridLabelRow;

typedef char StaffGridLabelRow_size[
    (sizeof(StaffGridLabelRow) ==
     (0x04 + STAFF_GRID_LABEL_CAPACITY * 0x04 + STAFF_GRID_LABEL_CAPACITY +
      STAFF_GRID_LABEL_PADDING_BYTES)) ? 1 : -1];
typedef char StaffGridLabelRow_count_offset[
    ((u32)&((StaffGridLabelRow *)0)->count == 0x00) ? 1 : -1];
typedef char StaffGridLabelRow_x_offset[
    ((u32)&((StaffGridLabelRow *)0)->x == 0x04) ? 1 : -1];
typedef char StaffGridLabelRow_gridIds_offset[
    ((u32)&((StaffGridLabelRow *)0)->gridIds ==
     (0x04 + STAFF_GRID_LABEL_CAPACITY * 0x04)) ? 1 : -1];

#endif
