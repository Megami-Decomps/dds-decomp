#ifndef DDS3_MODEL_RANGE_H
#define DDS3_MODEL_RANGE_H

#include "common.h"

/* Release-view of the allocation-owned range payload; the leading word is
 * retained as opaque because only the stored backing-resource address is read. */
typedef struct Dds3ModelRangeDataView {
    u8 opaque_00[4];
    s32 resourceAddress;
} Dds3ModelRangeDataView;

/* Only the range-data pointer at +0x18 is established by the release provider;
 * the preceding object bytes remain opaque and no allocation extent is claimed. */
typedef struct Dds3ModelRangeObjectView {
    u8 opaque_00[0x18];
    Dds3ModelRangeDataView *rangeData;
} Dds3ModelRangeObjectView;

typedef char Dds3ModelRangeDataView_resource_at_04[
    ((u32)&((Dds3ModelRangeDataView *)0)->resourceAddress == 0x04) ? 1 : -1];
typedef char Dds3ModelRangeObjectView_data_at_18[
    ((u32)&((Dds3ModelRangeObjectView *)0)->rangeData == 0x18) ? 1 : -1];

#endif
