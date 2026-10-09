#include "fld_area_work.h"
#include "common.h"
#include "sdf_chip.h"
#include "bill_object_api.h"
#include "dds3obj.h"
#include "eff_event.h"
#include "eff_dependency.h"
#include "fld.h"
#include "fpu.h"
#include "pcp_vu0.h"


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
extern void effObjSetFlags(struct EffectObj *object, u32 flags);
extern void billSetKind1Entry(struct BillObj *billboard, u32 entry);
extern void billSetVariantValue(struct BillObj *billboard, s32 value);

/* Complete 0x30-byte FLD1 type-11 effect descriptor. The field resolver
 * passes its resource data pointer directly to the billboard binder. */
typedef struct BillConfig {
    u32 flags;   /* 0x00: bit 0 / bit 1 select the billboard mode */
    u32 kind;    /* 0x04 */
    u32 resourceIndex; /* 0x08 */
    f32 width;   /* 0x0C */
    f32 height;  /* 0x10 */
    f32 depth;   /* 0x14 */
    u32 projectionDistance; /* 0x18 */
    s32 mapSelector;        /* 0x1C: flag test and map-slot byte offset */
    union {
        u32 entry;          /* 0x20: kind-1 entry */
        u32 fadeStart;      /* 0x20: kind-0/3 lower fade distance */
    };
    u32 fadeEnd;            /* 0x24: kind-0/3 upper fade distance */
    u32 parameters28[2];   /* 0x28..0x2C: unconsumed parameters */
} BillConfig;

typedef char BillConfig_size_must_be_0x30[(sizeof(BillConfig) == 0x30) ? 1 : -1];

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
        billSetChildHalfExtents((struct BillObj *)resource->handle, resource->config->width, resource->config->height);
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

extern s32 fldTestMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit);
extern s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueOffset);
extern void billSetChildParameter(struct BillObj *billboard, u32 parameter);
extern f32 sdfViewTargetVector[4];

void effUpdateConfiguredBillboard(EffWorldNode *object) {
    EffectDependencyState *resource = object->data;
    BillConfig *config;
    f32 direction[4] __attribute__((aligned(16)));
    f32 normalizedDirection[4] __attribute__((aligned(16)));
    f32 dx;
    f32 dy;
    f32 value; /* z delta, then projection distance */
    f32 distance;
    u32 projectionDistanceWord;

    if ((resource->flags & 0x10) == 0) {
        return;
    }

    config = resource->config;
    if (config->mapSelector != 0) {
        s32 mapId = fldAreaState.area;
        u32 slotIndex = fldAreaState.floor + 1;

        if (fldTestMapTargetFlag(mapId, slotIndex, config->mapSelector)) {
            resource->flags &= ~1;
        } else {
            resource->flags |= 1;
        }
    }

    dx = sdfViewTargetVector[0] - resource->sourcePosition[0];
    dy = sdfViewTargetVector[1] - resource->sourcePosition[1];
    value = sdfViewTargetVector[2] - resource->sourcePosition[2];
    projectionDistanceWord = config->projectionDistance;
    distance = fsqrtf(dx * dx + dy * dy + value * value);

    if (projectionDistanceWord != 0) {
        direction[0] = -dx;
        direction[1] = -dy;
        direction[2] = -value;
        direction[3] = 1.0f;
        VU0_LOAD_VF(vf10, direction);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, normalizedDirection);
        {
            ObjectTransform *inner;

            value = (f32)projectionDistanceWord;
            inner = object->inner;

            inner->position[0] = resource->sourcePosition[0] - normalizedDirection[0] * value;
            inner->position[1] = resource->sourcePosition[1] - normalizedDirection[1] * value;
            inner->position[2] = resource->sourcePosition[2] - normalizedDirection[2] * value;
        }
    }

    switch (config->kind) {
    case 0:
    case 3:
        if (config->fadeStart == 0 && config->fadeEnd == 0) {
            return;
        }
        {
            f32 start = (f32)config->fadeStart;
            f32 end = (f32)config->fadeEnd;
            s32 alpha;
            u32 parameter;

            if (distance < start) {
                parameter = 0x00808080;
            } else if (end < distance) {
                parameter = 0x80808080;
            } else {
                alpha = (s32)(((distance - start) * 128.0f) / (end - start));
                if (alpha >= 129) {
                    alpha = 128;
                }
                if (alpha < 0) {
                    alpha = 0;
                }
                parameter = ((u32)alpha << 24) | 0x00808080;
            }
            billSetChildParameter((struct BillObj *)resource->handle, parameter);
        }
        break;
    case 2:
        break;
    case 1:
        if (config->mapSelector != 0) {
            s32 entry = fldGetMapSlotByte(fldAreaState.area,
                                         (u32)fldAreaState.floor + 1,
                                         config->mapSelector);

            if (entry != -1) {
                billSetKind1Entry((struct BillObj *)resource->handle, (u32)entry);
            }
        }
        break;

    default:
        break;
    }
}

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
