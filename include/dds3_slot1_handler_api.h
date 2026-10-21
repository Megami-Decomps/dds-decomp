#ifndef DDS3_SLOT1_HANDLER_API_H
#define DDS3_SLOT1_HANDLER_API_H

#include "dds3_path.h"

/* Set the callback on an object's slot-1 handler; returns whether it exists. */
u8 dds3SetSlot1HandlerCallback(struct EffWorldNode *object,
                               Dds3MoverUpdate update);

#endif
