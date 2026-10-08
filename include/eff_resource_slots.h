#ifndef EFF_RESOURCE_SLOTS_H
#define EFF_RESOURCE_SLOTS_H

#include "common.h"

struct EffectSlotSet;
struct BdWork;
struct SdfMemBlock;
struct EffMappedResource;
struct EffMappedRecord;
struct EffTimedState;

/* EffTimedState.flags bits used by phase initialization and endpoint handling. */
enum EffTimedStateFlag {
    /* Selects incrementing from zero instead of decrementing from the endpoint. */
    EFF_TIMED_STATE_DIRECTION_FORWARD = 0x1,
    /* Initialize the phase when a resource is bound to the state. */
    EFF_TIMED_STATE_INITIALIZE_PHASE_ON_BIND = 0x2,
    /* At either endpoint, restart the work or reverse direction when ping-pong is set. */
    EFF_TIMED_STATE_RESTART_AT_ENDPOINT = 0x4,
    /* With restart-at-endpoint set, switch between forward and reverse. */
    EFF_TIMED_STATE_PING_PONG = 0x8,
};

u32 effSetSlotIndexedResource(struct EffTimedState *target,
                              struct EffMappedResource *resources, s32 item,
                              u32 flags);
u32 effSetSlotResourceAndFlags(struct EffTimedState *target,
                               struct EffMappedRecord *resource, u32 flags);

struct EffMappedResource *effCreateStatusBatch(u32 category);
#ifdef VERSION_DDS2
s32 effDestroyPackedBatch(struct EffMappedResource *batch);
#else
u32 effDestroyPackedBatch(struct EffMappedResource *batch);
#endif
/* Serialized source bytes are borrowed; callback output slots retain their word ABI. */
struct EffMappedResource *effCreateMappedResource(const u8 *source);
struct EffMappedResource *effLoadMappedResource(const char *base, const char *name);
void effRequestMappedResource(const char *base, const char *name, u32 *outMappedResource);


#ifdef VERSION_DDS2
struct EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, s32 keepAllocation);
#else
struct EffectSlotSet *effLoadIndexedResource(const char *base, const char *name, u32 keepAllocation);
#endif

/* Nonzero keepAllocation transfers the source descriptor to the returned owner. */
struct EffectSlotSet *effCreateResourceSlotSetFromAllocation(
    struct SdfMemBlock *resourceAllocation, u32 keepAllocation);

struct EffectSlotSet *effCreateResourceSlotSet(struct EffectSlotSet *source, u32 slot, u32 count);
u32 effReleaseSlotWorkAllocation(struct EffectSlotSet *owner);
void effInitializeAllSlotWork(struct EffectSlotSet *owner);
void effAttachSlotWorkOwner(struct EffectSlotSet *owner, s32 slotIndex,
                            struct BdWork *entry);
void effResetSlotWork(struct EffectSlotSet *owner, u32 slotIndex);
void effResolveAndReleaseResource(struct EffectSlotSet *owner);
void effResolveAndReleaseSelectedResource(struct EffectSlotSet *owner, s32 slot);
void effInitializeSlotWork(struct EffectSlotSet *owner, s32 slotIndex);
#ifdef VERSION_DDS2
void effReleaseSlotTextureReferencesAndResetWork(struct EffectSlotSet *owner, s32 preserveWork);
#endif
void effReleaseTextureHandlesAndResetSlots(struct EffectSlotSet *owner);
u8 effHasFirstTextureHandle(struct EffectSlotSet *owner);
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
