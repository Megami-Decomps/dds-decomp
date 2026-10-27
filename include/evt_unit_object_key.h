#ifndef EVT_UNIT_OBJECT_KEY_H
#define EVT_UNIT_OBJECT_KEY_H

#include "common.h"

/* The signed VM representation of the wrapping 16-bit world-object key. */
typedef s32 EvtUnitObjectKey;

/* Create a unit object and return its world-object key. */
EvtUnitObjectKey evtLoadUnit(s32 slotKind, s32 resource);

#endif /* EVT_UNIT_OBJECT_KEY_H */
