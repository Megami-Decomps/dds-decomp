#include "fld_area_work.h"
#include "common.h"
#include "sdf_vu0.h"
#include "fld_bill_config.h"
#include "sdf_chip.h"
#include "bill_object_api.h"
#include "dds3obj.h"
#include "eff_event.h"
#include "eff_dependency.h"
#include "fld.h"
#include "fpu.h"
#include "pcp_vu0.h"


/* Clear the kind-7 owner link and entry, preserving every flag except 4 and 8.
 * The caller supplies an existing effect resource; this does not free its owner. */
void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *owner) {
    EffectDependencyState *resource = owner->data;

    resource->entryId = 0;
    resource->owner = NULL;
    resource->ownerKind = 0;
    resource->flags &= ~4;
    resource->flags &= ~8;
}

struct billHandle_t;
extern void dds3EffectObjectBasicStatOn(dds3Basic_t *object, u32 flags);
extern void dds3BillboardSetAnimNumber(struct billHandle_t *billboard, u32 entry);

/* Complete 0x30-byte FLD1 type-11 effect descriptor. The field resolver
 * passes its resource data pointer directly to the billboard binder. */


/* Bind a billboard config to the owner's resource, copy the source vector and set up the billboard by config kind. */
void dds3EffectObjectBasicSetFldEx(dds3Basic_t *owner, BillConfig *config) {
    EffectDependencyState *resource = owner->data;
    dds3PoseSet_t *source = owner->inner;

    resource->config = config;
    resource->sourcePosition[0] = source->position[0];
    resource->sourcePosition[1] = source->position[1];
    resource->sourcePosition[2] = source->position[2];
    resource->sourcePosition[3] = source->position[3];
    dds3EffectObjectBasicStatOn(owner, 0x10);
    switch (resource->config->kind) {
    case 0:
    case 3:
        dds3BillboardSetXYOffset((struct billHandle_t *)resource->handle, resource->config->width, resource->config->height);
        if (resource->config->kind == 3) {
            dds3BillboardSetDrawType(resource->handle, 1);
        }
        if (resource->config->flags & 1) {
            dds3BillboardSetMode(resource->handle, 2);
            return;
        }
        if (resource->config->flags & 2) {
            dds3BillboardSetMode(resource->handle, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        dds3BillboardSetAnimNumber(resource->handle, resource->config->entry);
        break;
    }
}

extern u8 fldTestMapTargetFlag(s32 mapId, u32 slotIndex, s32 bit);
extern s32 fldGetMapSlotByte(s32 mapId, u32 slotIndex, s32 valueIndex);
extern f32 sdfViewTargetVector[4];

void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *object) {
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
        s32 mapId = fldGlobalBase.area;
        u32 slotIndex = fldGlobalBase.floor + 1;

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
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(normalizedDirection);
        {
            dds3PoseSet_t *inner;

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
            dds3BillboardSetRGBA((struct billHandle_t *)resource->handle, parameter);
        }
        break;
    case 2:
        break;
    case 1:
        if (config->mapSelector != 0) {
            s32 entry = fldGetMapSlotByte(fldGlobalBase.area,
                                         (u32)fldGlobalBase.floor + 1,
                                         config->mapSelector);

            if (entry != -1) {
                dds3BillboardSetAnimNumber((struct billHandle_t *)resource->handle, (u32)entry);
            }
        }
        break;

    default:
        break;
    }
}

/* Attach a 16-byte slot block, then publish its newly created state handle.
 * The block is attached before the state constructor sees the object. Returns 1. */
u32 EventObjectBasicConstructor(dds3Basic_t *object) {
    DdsSlotResourceBlock *resource;
    dds3ObjectBaseControlWorkData_t *handle;

    dds3InitPoseSet(object);
    resource = (DdsSlotResourceBlock *)sdfChipAllocDirect(sizeof(DdsSlotResourceBlock));
    object->data = resource;
    handle = dds3CreateSlotResourceState(object);
    resource->baseControlWorkData = handle;
    return 1;
}
