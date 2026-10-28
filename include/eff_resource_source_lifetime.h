#ifndef EFF_RESOURCE_SOURCE_LIFETIME_H
#define EFF_RESOURCE_SOURCE_LIFETIME_H

#include "eff_resource_slots.h"

/* Transient sources resolve texture handles immediately and can then be freed;
 * retained sources stay with the owner for later texture resolution. */
typedef u32 EffResourceSourceLifetime;
enum {
    EFF_RESOURCE_SOURCE_TRANSIENT = 0,
    EFF_RESOURCE_SOURCE_RETAINED = 1
};

struct EffectSlotSet *effCreateResourceSlotSetFromAllocation(
    struct sdfMemHandle_t *resourceAllocation,
    EffResourceSourceLifetime sourceLifetime);

#ifdef VERSION_DDS2
struct EffectSlotSet *effLoadIndexedResource(
    const char *base, const char *name, s32 sourceLifetime);
#else
struct EffectSlotSet *effLoadIndexedResource(
    const char *base, const char *name, u32 sourceLifetime);
#endif

#endif
