#ifndef EFFECT_VALUE_OBJECT_H
#define EFFECT_VALUE_OBJECT_H

#include "common.h"

/* Data consulted by both titles' proximity transition updater. Flag 0x4 keeps
 * the value record out of proximity-based transitions. */
typedef struct EffectValueData {
    u8 pad00[0x64];
    u32 flags; /* +0x64 */
} EffectValueData;

/* Value-chain entry read by effUpdateValueTransitionProximity in both titles.
 * The updater tracks valueId, tests source against the object's radius and
 * consults data->flags before starting or ending a transition. */
typedef struct EffectValueObject {
    u8 pad00[4];
    s32 valueId; /* +0x04 */
    u8 pad08[0x10];
    EffectValueData *data; /* +0x18 */
    f32 *source; /* +0x1C: point used by the proximity test */
} EffectValueObject;

typedef char EffectValueObject_valueId_offset[
    ((u32)&((EffectValueObject *)0)->valueId == 0x04) ? 1 : -1];
typedef char EffectValueObject_data_offset[
    ((u32)&((EffectValueObject *)0)->data == 0x18) ? 1 : -1];
typedef char EffectValueObject_source_offset[
    ((u32)&((EffectValueObject *)0)->source == 0x1C) ? 1 : -1];
typedef char EffectValueData_flags_offset[
    ((u32)&((EffectValueData *)0)->flags == 0x64) ? 1 : -1];

#endif /* EFFECT_VALUE_OBJECT_H */
