#ifndef EFF_RESOURCE_SLOTS_H
#define EFF_RESOURCE_SLOTS_H

#include "common.h"

struct EffectSlotSet;

#ifdef VERSION_DDS2
struct EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, s32 keepAllocation);
#else
struct EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation);
#endif

struct EffectSlotSet *effCreateResourceSlotSet(struct EffectSlotSet *source, u32 slot, u32 count);
void effResolveAndReleaseResource(struct EffectSlotSet *owner);
void effResolveAndReleaseSelectedResource(struct EffectSlotSet *owner, s32 slot);
void effReleaseTextureHandlesAndResetSlots(struct EffectSlotSet *owner);
u32 effDestroyResourceSlotSet(struct EffectSlotSet *owner);

#endif /* EFF_RESOURCE_SLOTS_H */
