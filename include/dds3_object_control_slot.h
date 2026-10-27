#ifndef DDS3_OBJECT_CONTROL_SLOT_H
#define DDS3_OBJECT_CONTROL_SLOT_H

#include "common.h"

struct dds3Basic_t;

/* Index domain for object-base control slots. */
typedef s32 Dds3ObjectControlSlotIndex;

enum {
    DDS3_OBJECT_OWNER_SLOT = 0,
    DDS3_OBJECT_DATA_SLOT = 1,
    DDS3_OBJECT_SCRIPT_MODEL_SLOT = 3,
    DDS3_OBJECT_ACTION_B_SLOT = 4,
    DDS3_OBJECT_HANDLER_SLOT = 5
};

struct dds3Basic_t *dds3ObjectBaseSetControlBasicType(
    struct dds3Basic_t *object, struct dds3Basic_t *slotData,
    Dds3ObjectControlSlotIndex slotIndex);
struct dds3Basic_t *dds3ObjectBaseGetControlBasic(
    struct dds3Basic_t *object, Dds3ObjectControlSlotIndex slotIndex);

#endif /* DDS3_OBJECT_CONTROL_SLOT_H */
