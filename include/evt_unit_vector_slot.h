#ifndef EVT_UNIT_VECTOR_SLOT_H
#define EVT_UNIT_VECTOR_SLOT_H

#include "common.h"

struct evtUnitHandle_t;

typedef enum EvtUnitVectorSlotState {
    EVT_UNIT_VECTOR_SLOT_EMPTY = 0,
    EVT_UNIT_VECTOR_SLOT_SHARED_FALLBACK = 2,
    EVT_UNIT_VECTOR_SLOT_UNIT_BOUND = 3
} EvtUnitVectorSlotState;

typedef s32 EvtUnitVectorSlotAuxCoordinatesResult;
enum {
    EVT_UNIT_VECTOR_SLOT_AUX_COORDINATES_NOT_FOUND = 0,
    EVT_UNIT_VECTOR_SLOT_AUX_COORDINATES_FOUND = 1
};

/* Native 0x40-byte slot: three vec4 followed by two auxiliary coordinates.
 * The static pool contains seven slots in DDS1 and ten in DDS2. */
typedef struct EvtUnitVectorSlot {
    EvtUnitVectorSlotState state; /* 0x00: slot ownership state. */
    struct evtUnitHandle_t *unitHandle; /* 0x04: unit bound to a state-3 slot. */
    f32 vec[12];                  /* 0x08: vec4 at 0x08/0x18/0x28. */
    f32 auxX;                     /* 0x38: auxiliary coordinates (scalar fields: */
    f32 auxY;                     /* 0x3C: writers split the address at 0x30). */
} EvtUnitVectorSlot;

typedef char EvtUnitVectorSlotSizeCheck[sizeof(EvtUnitVectorSlot) == 0x40 ? 1 : -1];
typedef char EvtUnitVectorSlot_unitHandle_offset_check[
    ((u32)&((EvtUnitVectorSlot *)0)->unitHandle == 0x04) ? 1 : -1];

void evtSetSlotVectors(s32 slotIndex, EvtUnitVectorSlotState slotState,
                       struct evtUnitHandle_t *unitHandle, const f32 *firstEndpoint,
                       const f32 *secondEndpoint, const f32 *color);

/* Copy auxiliary coordinates from the unit-bound slot or shared fallback.
 * Outputs are written only when a slot is found. */
EvtUnitVectorSlotAuxCoordinatesResult evtFindUnitSlotAuxCoordinates(
    struct evtUnitHandle_t *unit, f32 *outX, f32 *outY);

#endif /* EVT_UNIT_VECTOR_SLOT_H */
