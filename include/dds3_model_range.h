#ifndef DDS3_MODEL_RANGE_H
#define DDS3_MODEL_RANGE_H

#include "eff_transform.h"

/* Kind-11 data block. Release only reads the stored backing-resource address;
 * the leading word remains opaque. */
typedef struct Dds3ModelRangeDataView {
    u8 opaque_00[4];
    const void *resourceData;
} Dds3ModelRangeDataView;

typedef char Dds3ModelRangeDataView_resource_at_04[
    ((u32)&((Dds3ModelRangeDataView *)0)->resourceData == 0x04) ? 1 : -1];
typedef char Dds3ModelRangeDataView_size_must_be_0x08[
    (sizeof(Dds3ModelRangeDataView) == 0x08) ? 1 : -1];
typedef char Dds3ModelRangeNode_data_at_18[
    ((u32)&((dds3Basic_t *)0)->data == 0x18) ? 1 : -1];

/* The kind-11 destroy hook is dispatched through EffWorldOps.destroy. */
void dds3ModelBasicFreeModel(dds3Basic_t *object);

#endif
