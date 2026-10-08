#ifndef EFF_RESOURCE_SLOTS_H
#define EFF_RESOURCE_SLOTS_H

#include "common.h"

struct EffectSlotSet;
struct EffMappedResource;
struct EffTimedState;

u32 effSetSlotIndexedResource(struct EffTimedState *target,
                              struct EffMappedResource *resources, s32 item,
                              u32 flags);

struct EffMappedResource *effCreateStatusBatch(u32 category);
#ifdef VERSION_DDS2
s32 effDestroyPackedBatch(struct EffMappedResource *batch);
#else
u32 effDestroyPackedBatch(struct EffMappedResource *batch);
#endif
/* Serialized source addresses and callback output slots retain their word ABI. */
struct EffMappedResource *effCreateMappedResource(u32 sourceAddress);
struct EffMappedResource *effLoadMappedResource(const char *base, const char *name);
void effRequestMappedResource(const char *base, const char *name, u32 *outMappedResource);


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

#ifdef VERSION_DDS2
s32 effConfigureIndexedSlotResource(struct EffectSlotSet *owner, u32 slot,
                                    struct EffMappedResource *resources, u32 item,
                                    u32 flags);
#else
u32 effConfigureIndexedSlotResource(struct EffectSlotSet *owner, s32 slot,
                                    struct EffMappedResource *resources, s32 item,
                                    u32 flags);
#endif

#ifdef VERSION_DDS2
s32 effConfigureIndexedSlotMaterial(struct EffectSlotSet *owner, u32 slot,
                                    struct EffMappedResource *resources, u32 item,
                                    u32 materialFlags, u32 materialValue,
                                    u32 stateFlags);
#else
u32 effConfigureIndexedSlotMaterial(struct EffectSlotSet *owner, s32 slot,
                                    struct EffMappedResource *resources, s32 item,
                                    u32 materialFlags, u32 materialValue,
                                    u32 stateFlags);
#endif
u32 effConfigureWithDefaultSetting(struct EffectSlotSet *owner, u32 slot,
                                   struct EffMappedResource *resources, u32 item,
                                   u32 materialFlags, u32 stateFlags);

#endif /* EFF_RESOURCE_SLOTS_H */
