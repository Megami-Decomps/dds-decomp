#include "common.h"

extern u32 func_001119D0(u32);

extern u32 func_00328D68(u32);

/* The resource pointer is stored at +0x18 in both games. */
typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

/* The record behind the owner's +0x18 pointer: a flag word, then the value
   pair at the end of the layout that a reset clears. */
typedef struct WorldResource {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0x18];
    u32 unk20;
    u16 unk24;
    u16 unk26;
} WorldResource;

void dds3ResetWorldResourceState(WorldResourceOwner *owner) {
    WorldResource *resource = (WorldResource *)owner->resource;

    resource->unk24 = 0;
    resource->unk20 = 0;
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
void func_00115F70(BillOwner *owner, BillConfig *config) {
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

INCLUDE_ASM(const s32, "game/code_00115F40", func_00116078);

/* Attach the owner's newly allocated resource slot and store its handle. */
u32 dds3InitializeResourceOwner(WorldResourceOwner *object) {
    u32 *resource;
    u32 handle;

    effObjInnerCreate();
    resource = (u32 *)func_00328D68(0x10);
    object->resource = resource;
    handle = func_001119D0((u32)object);
    *resource = handle;
    return 1;
}
