#include "common.h"
#include "dds3obj.h"
#include "eff_event.h"
#include "eff_dependency.h"

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 effObjInnerCreate(EffWorldNode *object);

/* Clear the kind-7 owner link and entry, preserving every flag except 4 and 8.
 * The caller supplies an existing effect resource; this does not free its owner. */
void dds3ResetWorldResourceState(EffWorldNode *owner) {
    EffectDependencyState *resource = owner->data;

    resource->entryId = 0;
    resource->owner = NULL;
    resource->ownerKind = 0;
    resource->flags &= ~4;
    resource->flags &= ~8;
}

struct EffectObj;
struct BillObj;
extern void effObjSetFlags(struct EffectObj *object, s32 flags);
extern void billSetKind1Entry(struct BillObj *billboard, u32 entry);
extern void billSetChildHalfExtents(s32 billboard, f32 width, f32 height);
extern void billSetVariantValue(struct BillObj *billboard, s32 value);
extern void billSetBillboardMode(struct BillObj *billboard, s32 mode);

typedef struct BillConfig {
    u32 flags;   /* 0x00: bit 0 / bit 1 select the billboard mode */
    u32 kind;    /* 0x04 */
    u8 pad08[4];
    f32 width;   /* 0x0C */
    f32 height;  /* 0x10 */
    u8 pad14[0xC];
    u32 entry;   /* 0x20 */
} BillConfig;

/* Bind a billboard config to the owner's resource, copy the source vector and set up the billboard by config kind. */
void billCopySourceVectorAndSetConfig(EffWorldNode *owner, BillConfig *config) {
    EffectDependencyState *resource = owner->data;
    ObjectTransform *source = owner->inner;

    resource->config = config;
    resource->sourcePosition[0] = source->position[0];
    resource->sourcePosition[1] = source->position[1];
    resource->sourcePosition[2] = source->position[2];
    resource->sourcePosition[3] = source->position[3];
    effObjSetFlags((struct EffectObj *)owner, 0x10);
    switch (resource->config->kind) {
    case 0:
    case 3:
        billSetChildHalfExtents((s32)resource->handle, resource->config->width, resource->config->height);
        if (resource->config->kind == 3) {
            billSetVariantValue(resource->handle, 1);
        }
        if (resource->config->flags & 1) {
            billSetBillboardMode(resource->handle, 2);
            return;
        }
        if (resource->config->flags & 2) {
            billSetBillboardMode(resource->handle, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        billSetKind1Entry(resource->handle, resource->config->entry);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00115CD8", effUpdateConfiguredBillboard);

/* Attach a 16-byte slot block, then publish its newly created state handle.
 * The block is attached before the state constructor sees the object. Returns 1. */
u32 dds3InitializeResourceOwner(EffWorldNode *object) {
    DdsSlotResourceBlock *resource;
    ObjBase *handle;

    effObjInnerCreate(object);
    resource = (DdsSlotResourceBlock *)sdfAllocSizeClassBlock(0x10);
    object->data = resource;
    handle = dds3CreateSlotResourceState(object);
    resource->resourceState = handle;
    return 1;
}
