#include "common.h"
#include "dds3obj.h"
#include "eff_event.h"

extern void *sdfAllocSizeClassBlock(s32 size);
extern s32 effObjInnerCreate(EffWorldNode *object);

/* Object resource storage is polymorphic: reset expects the kind-7 effect
 * prefix below, while initialization attaches a separate 16-byte slot block. */
typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    void *resource;
} WorldResourceOwner;

/* Kind-7 effect owner-link prefix, also consumed by dds3EffectObjectBasic. */
typedef struct WorldResource {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0x18];
    void *owner; /* +0x20: owner used by the billboard parameter lookup */
    u16 entryId; /* +0x24: entry forwarded to that lookup */
    u16 unk26; /* Written from the bound owner's kind; no read established. */
} WorldResource;

/* Clear the kind-7 owner link and entry, preserving every flag except 4 and 8.
 * The caller supplies an existing effect resource; this does not free its owner. */
void dds3ResetWorldResourceState(WorldResourceOwner *owner) {
    WorldResource *resource = owner->resource;

    resource->entryId = 0;
    resource->owner = NULL;
    resource->unk26 = 0;
    resource->flags &= ~4;
    resource->flags &= ~8;
}

extern void effObjSetFlags(void *object, s32 flags);
extern void billSetKind1Entry(u32 billboard, u32 entry);
extern void billSetChildHalfExtents(s32 billboard, f32 width, f32 height);
extern void billSetVariantValue(u32 billboard, s32 value);
extern void billSetBillboardMode(u32 billboard, s32 mode);

typedef struct BillConfig {
    u32 flags;   /* 0x00: bit 0 / bit 1 select the billboard mode */
    u32 kind;    /* 0x04 */
    u8 pad08[4];
    f32 width;   /* 0x0C */
    f32 height;  /* 0x10 */
    u8 pad14[0xC];
    u32 entry;   /* 0x20 */
} BillConfig;

typedef struct BillSource {
    u8 pad00[0x40];
    f32 vec[4];  /* 0x40 */
} BillSource;

typedef struct BillResource {
    u8 pad00[0xC];
    u32 billboard;  /* 0x0C */
    u8 pad10[0x2C];
    BillConfig *config; /* 0x3C */
    f32 vec[4];  /* 0x40 */
} BillResource;

typedef struct BillOwner {
    u8 pad00[0x18];
    BillResource *resource; /* 0x18 */
    BillSource *source;     /* 0x1C */
} BillOwner;

/* Bind a billboard config to the owner's resource, copy the source vector and set up the billboard by config kind. */
void billCopySourceVectorAndSetConfig(BillOwner *owner, BillConfig *config) {
    BillResource *resource = owner->resource;
    BillSource *source = owner->source;

    resource->config = config;
    resource->vec[0] = source->vec[0];
    resource->vec[1] = source->vec[1];
    resource->vec[2] = source->vec[2];
    resource->vec[3] = source->vec[3];
    effObjSetFlags(owner, 0x10);
    switch (resource->config->kind) {
    case 0:
    case 3:
        billSetChildHalfExtents(resource->billboard, resource->config->width, resource->config->height);
        if (resource->config->kind == 3) {
            billSetVariantValue(resource->billboard, 1);
        }
        if (resource->config->flags & 1) {
            billSetBillboardMode(resource->billboard, 2);
            return;
        }
        if (resource->config->flags & 2) {
            billSetBillboardMode(resource->billboard, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        billSetKind1Entry(resource->billboard, resource->config->entry);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115E10);

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
