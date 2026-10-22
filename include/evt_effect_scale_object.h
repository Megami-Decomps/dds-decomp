#ifndef EVT_EFFECT_SCALE_OBJECT_H
#define EVT_EFFECT_SCALE_OBJECT_H

#include "common.h"

struct effEveBEDCutHandle_t;

/* Party/enemy model-table entry; only the scale field is established. */
typedef struct EvtModelScaleEntry {
    u8 pad00[0x18];
    f32 modelScale; /* 0x18 */
    u8 pad1C[0x254];
} EvtModelScaleEntry;

/* Consumer view of the effect object passed to the event scale setter. */
typedef struct EvtEffectInner {
    u8 pad00[0x2C];
    struct effEveBEDCutHandle_t *scaledObject; /* 0x2C: passed to effEveBEDCutSetScale. */
} EvtEffectInner;

typedef struct EvtEffectObject {
    u8 pad00[0x18];
    EvtEffectInner *inner; /* 0x18 */
} EvtEffectObject;

typedef char EvtModelScaleEntry_size_check[
    (sizeof(EvtModelScaleEntry) == 0x270) ? 1 : -1];
typedef char EvtModelScaleEntry_modelScale_offset_check[
    ((u32)&((EvtModelScaleEntry *)0)->modelScale == 0x18) ? 1 : -1];
typedef char EvtEffectInner_size_check[
    (sizeof(EvtEffectInner) == 0x30) ? 1 : -1];
typedef char EvtEffectInner_scaledObject_offset_check[
    ((u32)&((EvtEffectInner *)0)->scaledObject == 0x2C) ? 1 : -1];
typedef char EvtEffectObject_size_check[
    (sizeof(EvtEffectObject) == 0x1C) ? 1 : -1];
typedef char EvtEffectObject_inner_offset_check[
    ((u32)&((EvtEffectObject *)0)->inner == 0x18) ? 1 : -1];

#endif /* EVT_EFFECT_SCALE_OBJECT_H */
