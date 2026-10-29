#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_model_packet_callback.h"
#include "dds3_object_flags.h"
#include "sdf_model.h"
#include "sdf_model_eval_api.h"
#include "sdf_model_release_api.h"
#include "evt_gradient_rect.h"
#include "sdf_motion.h"
#include "sdf_motion_next_result.h"
#include "sdf_chip.h"
#include "dds3_path.h"
#include "dds3_slot1_handler_api.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_object_resource_state.h"
#include "dds3_object_base_resource_view.h"
#include "dds3_object_mode.h"
#include "eff_object.h"
#include "pcp_vu0.h"
#include "mdl.h"
#include "evt_unit.h"
#include "evt_script_model_api.h"

extern void Normalize(void);

extern dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *object);


Dds3ObjectControlSlotIndex BasicTypeToItemType(u32 kind);
void *dds3ObjectBaseGetUserValue(dds3Basic_t *object);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *object, Dds3MoverUpdate update);
void dds3MoverBasicMakePath(dds3Basic_t *object);
void dds3MoverBasicFreePath(dds3Basic_t *object);
void dds3AreaBasicGetItemArray(dds3Basic_t *slot, dds3Basic_t *owner);
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *object);


#define DDS3_OBJECT_SLOT_COUNT 8
#define DDS3_OBJECT_WORLD_SLOT_LIMIT 3

/* Process the owner's entry in the auxiliary handler index, destroy world nodes
 * in slots 1/2, release owned resources/devices, then free the index and base. */
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *base) {
    dds3Basic_t *owner;
    dds3Basic_t *handler;
    Dds3ObjectControlSlotIndex slotIndex;

    owner = base->slots[DDS3_OBJECT_OWNER_SLOT];
    handler = dds3ObjectBaseGetControlBasic(owner, DDS3_OBJECT_HANDLER_SLOT);
    if (handler != NULL) {
        dds3AreaBasicGetItemArray(handler, owner);
    }
    for (slotIndex = 0; slotIndex < DDS3_OBJECT_SLOT_COUNT; slotIndex++) {
        if (slotIndex < DDS3_OBJECT_WORLD_SLOT_LIMIT) {
            if (slotIndex > DDS3_OBJECT_OWNER_SLOT) {
                if (base->slots[slotIndex] != NULL) {
                    dds3DeleteSceneBasicData(base->slots[slotIndex]);
                }
            }
        }
    }
    dds3ObjectBaseFreeModelHandle(owner);
    if (base->devSlot != 0) {
        sdfModelRootFree(base->devSlot,
                         SDF_MODEL_ROOT_FREE_DEFERRED,
                         SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED);
    }
    dds3FreeBasicArrayHandle(base->worldIndexNode);
    sdfChipFreeDirect(base);
}

/* Set mask bits in the object's resolved base. */
void dds3ObjectBaseOnFlag(dds3Basic_t *object, u32 mask) {
    dds3ObjectBaseControlWorkData_t *base;

    base = dds3GetBaseControlWorkData(object);
    base->flags = base->flags | mask;
}

/* Clear mask bits in the object's resolved base. */
void dds3ObjectBaseOffFlag(dds3Basic_t *object, u32 mask) {
    dds3ObjectBaseControlWorkData_t *base;

    base = dds3GetBaseControlWorkData(object);
    base->flags = base->flags & ~mask;
}

/* Return whether any requested mask bit is set, not whether all bits are set. */
u8 dds3ObjectBaseCheckFlag(dds3Basic_t *object, u32 mask) {
    dds3ObjectBaseControlWorkData_t *base;

    base = dds3GetBaseControlWorkData(object);
    return (base->flags & mask) != 0;
}

/* Replace the extension pointer without releasing its previous value.
 * Keep the existing extension getter call before the store. */
void dds3ObjectBaseSetUserValue(dds3Basic_t *object, void *extensionData) {
    dds3ObjectBaseControlWorkData_t *base;

    base = dds3GetBaseControlWorkData(object);
    dds3ObjectBaseGetUserValue(object);
    base->extData = extensionData;
}

/* Return the stored extension pointer; no copy or ownership change. */
void *dds3ObjectBaseGetUserValue(dds3Basic_t *object) {
    return dds3GetBaseControlWorkData(object)->extData;
}

/* Exchange the slot selected by the data kind; NULL data leaves every slot alone. */
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *object, dds3Basic_t *slotData) {
    if (slotData == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(object, slotData, BasicTypeToItemType(slotData->kindTag >> 24));
}

/* Replace one caller-selected slot and return its previous pointer; no release. */
dds3Basic_t *dds3ObjectBaseSetControlBasicType(
    dds3Basic_t *object, dds3Basic_t *slotData, Dds3ObjectControlSlotIndex slotIndex) {
    dds3Basic_t *previousData;

    previousData = dds3ObjectBaseGetControlBasic(object, slotIndex);
    dds3GetBaseControlWorkData(object)->slots[slotIndex] = slotData;
    return previousData;
}

/* Read an indexed slot; the caller supplies a valid index. */
dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *object, Dds3ObjectControlSlotIndex slotIndex) {
    return dds3GetBaseControlWorkData(object)->slots[slotIndex];
}

/* Return the object's world-index node. */
WorldIndexNode *dds3ObjectBaseGetEventArray(dds3Basic_t *object) {
    return dds3GetBaseControlWorkData(object)->worldIndexNode;
}

/* Return a borrowed model-context or device-motion-root view; check
 * resourceState before interpreting it. Owner release or rebuild invalidates it. */
void *dds3ObjectBaseGetModelHandle(dds3Basic_t *object) {
    return dds3GetBaseControlWorkData(object)->resourceHandle;
}

/* A nonzero handle releases model/context state (0) or device/motion state (1),
 * then clears the handle and marks state 3. Other states skip backend release;
 * an absent handle leaves state untouched, and the stored motion pointer remains. */
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *object) {
    dds3ObjectBaseControlWorkData_t *base;
    EffectObjectData *data;
    Dds3ObjectResourceState resourceState;

    base = dds3GetBaseControlWorkData(object);
    if (base->resourceHandle != 0) {
        resourceState = base->resourceState;
        if (resourceState != DDS3_OBJECT_RESOURCE_DEV_MOTION) {
            if (resourceState == DDS3_OBJECT_RESOURCE_MODEL_CONTEXT) {
                mdlFree(dds3ObjectBaseBorrowModelContext(base));
                data = object->workdata;
                if (data->transitionWork != NULL) {
                    evtDeleteUnitHandle(data->transitionWork);
                    data->transitionWork = NULL;
                }
            }
        } else {
            sdfModelRootFree(dds3ObjectBaseBorrowDeviceMotionRoot(base),
                             SDF_MODEL_ROOT_FREE_DEFERRED,
                             SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED);
            sdfMotionCtrlrFree(base->motion);
        }
        base->resourceHandle = 0;
        base->resourceState = DDS3_OBJECT_RESOURCE_RELEASED;
    }
}

extern const char dds3BipedModelNames[5][10];
extern const char dds3ExtendedBipedModelNames[9][8];
extern const char dds3BipedChunkNames[17][20];
extern const char dds3ExtendedBipedChunkNames[18][20];
extern const char dds3PlayerChunkNames[17][20];
extern const char dds3Pc013ChunkNames[17][20];
extern const char D_003BA9C8[];
extern const char *sdfModelGetNodeName(sdfModelRootNode_t *model, s32 nameId);
extern s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *model, const char *name);

const char dds3PlayerModelName[16] = "player_a";

void dds3ResolveModelChunkBindings(dds3ObjectBaseControlWorkData_t *base, dds3ModelHandle_t *model) {
    const char *modelName;
    sdfModelRootNode_t *inner;
    s32 family;
    s32 chunkIndex;
    s32 chunkId;
    s32 subtype;
    s32 *slot;
    const char (*boneNameRow)[20];

    modelName = sdfModelGetNodeName(model->inner, 0);
    for (family = 0; family < 5; family++) {
        if (strcmp(modelName, dds3BipedModelNames[family]) == 0) {
            for (chunkIndex = 0; chunkIndex < 17; chunkIndex++) {
                inner = model->inner;
                chunkId = sdfModelGetNodeIndexByName(inner, dds3BipedChunkNames[chunkIndex]);
                if (chunkId > 0) {
                    base->resourceSlotCount = 17;
                    base->resourceSlots[chunkIndex] = chunkId;
                    base->resourceSlotFamily = family;
                }
            }
            return;
        }
    }

    for (family = 0; family < 9; family++) {
        if (strcmp(modelName, dds3ExtendedBipedModelNames[family]) == 0) {
            subtype = family + 0x1000;
            for (chunkIndex = 0; chunkIndex < 18; chunkIndex++) {
                inner = model->inner;
                chunkId = sdfModelGetNodeIndexByName(inner, dds3ExtendedBipedChunkNames[chunkIndex]);
                if (chunkId > 0) {
                    base->resourceSlotCount = 18;
                    base->resourceSlots[chunkIndex] = chunkId;
                    base->resourceSlotFamily = subtype;
                }
            }
            return;
        }
    }

    if (strcmp(modelName, dds3PlayerModelName) == 0) {
        for (chunkIndex = 0; chunkIndex < 17; chunkIndex++) {
            inner = model->inner;
            chunkId = sdfModelGetNodeIndexByName(inner, dds3PlayerChunkNames[chunkIndex]);
            if (chunkId > 0) {
                base->resourceSlotCount = 17;
                base->resourceSlots[chunkIndex] = chunkId;
                base->resourceSlotFamily = 0x2000;
            }
        }
        return;
    }

    if (strcmp(modelName, D_003BA9C8) == 0) {
        s32 count = ARRAY_COUNT(dds3Pc013ChunkNames);
        s32 category = 0x3000;
        slot = base->resourceSlots;
        boneNameRow = dds3Pc013ChunkNames;
        for (chunkIndex = count - 1; chunkIndex >= 0; chunkIndex--, slot++, boneNameRow++) {
            inner = model->inner;
            chunkId = sdfModelGetNodeIndexByName(inner, *boneNameRow);
            if (chunkId > 0) {
                base->resourceSlotCount = count;
                *slot = chunkId;
                base->resourceSlotFamily = category;
            }
        }
    }
}


/* Rebuild the model resource, synchronize object vectors and initialize
 * transition work only when no transition is already associated with it. */
void dds3RebuildObjectModelContext(dds3Basic_t *object, s32 resourceGroup, s32 resourceId) {
    f32 scale[4];
    dds3ObjectBaseControlWorkData_t *base;
    dds3ModelHandle_t *model;

    base = dds3GetBaseControlWorkData(object);
    if (base->resourceHandle != 0) {
        mdlFree((dds3ModelHandle_t *)base->resourceHandle);
    }
    model = mdlLoad(resourceGroup, resourceId);
    dds3ResolveModelChunkBindings(base, model);
    base->currentSlot = sdfModelGetNodeIndexByName(model->inner, "Bip01 Head");
    mdlGetScale(model);
    SDF_VU0_STOREVECTOR(scale);
    dds3SetSclPoseSet(object, (u128 *)scale);
    SDF_VU0_LOADVECTOR(object->poseset->rotation);
    Normalize();
    mdlSetRotate(model);
    SDF_VU0_LOADVECTOR(object->poseset->pos);
    SDF_VU0_VECTOR_W_SET_ONE();
    mdlSetPosition(model);
    dds3ObjectBaseSetModelContext(base, model);
    object->poseset->radius = model->objectRadius;
    base->resourceState = DDS3_OBJECT_RESOURCE_MODEL_CONTEXT;
    base->unk44 = 0;
    {
        EffectObjectData *data = object->workdata;

        if (data->transitionWork == NULL) {
            data->transitionWork = evtCreateUnitTransitionWork(object, model);
        }
    }
}

extern void dds3GetTMPoseSet(dds3Basic_t *object);

void dds3ObjectBaseMakeMatter(dds3Basic_t *object) {
    dds3ObjectBaseControlWorkData_t *base;
    const dds3Basic_t *scriptModelObject;
    sdfMotionCtrlr_t *motion;
    sdfModelRootNode_t *model;

    base = dds3GetBaseControlWorkData(object);
    scriptModelObject = dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_SCRIPT_MODEL_SLOT);
    model = dds3ModelBasicMakeModel(scriptModelObject);
    motion = dds3ModelBasicSetMotion(scriptModelObject, model);
    dds3GetTMPoseSet(object);
    VU0_STORE_MATRIX(model->matrix);
    sdfModelEvalTm2(model, 0);
    sdfModelEvalTm2(model, 1);
    base->resourceState = DDS3_OBJECT_RESOURCE_DEV_MOTION;
    dds3ObjectBaseSetDeviceMotionRoot(base, model);
    base->motion = motion;
    base->weight = 1.0f;
    base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED;
    base->unk44 = 0;
}


/* Modes 0, 4 and 5 use weight 1; the other valid modes use 0.
 * Values outside 0..6 leave both the current mode and weight unchanged. */
void dds3ObjectBaseMatterAnimeSpeed(
    dds3Basic_t *object, Dds3ObjectMatterAnimeMode requestedMode) {
    dds3ObjectBaseControlWorkData_t *base = dds3GetBaseControlWorkData(object);

    switch (requestedMode) {
    case DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED;
        base->weight = 1.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST;
        base->weight = 0.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW;
        base->weight = 0.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO;
        base->weight = 0.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST;
        base->weight = 1.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW;
        base->weight = 1.0f;
        break;
    case DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END:
        base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END;
        base->weight = 0.0f;
        break;
    }
}

/* Ensure the owner appears in its auxiliary handler's world index.
 * Return 0 for an absent handler, otherwise 1 after updating the index. */
s32 dds3ObjectBaseRegistArea(dds3Basic_t *object) {
    dds3Basic_t *handler;

    handler = dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_HANDLER_SLOT);
    if (handler == NULL) {
        return 0;
    }
    dds3AreaBasicRegistItem(handler, object);
    return 1;
}

extern void eglSetOt(u32 surfaceIndex);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);
extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);
extern void evtSubmitPrimaryFramePacket(void);
extern void evtSubmitAlternateFramePacket(void);
struct sdfGrOt_t;
extern struct sdfGrOt_t *kwlnSceneSurfaceSets[13][4];


void dds3DrawObjectWithGradientOverlay(dds3Basic_t *object) {
    dds3ObjectBaseControlWorkData_t *base;
    dds3ModelHandle_t *context;
    sdfModelRootNode_t *inner;

    base = dds3GetBaseControlWorkData(object);
    eglSetOt(0x4A);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryFramePacket();
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSubmitGradientRectAtDepth(0, 0, 0x200, 0x1C0, 0x0FFFFFFF, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    eglSetOt(0x4B);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSubmitPrimaryFramePacket();
    eglSetOt(0x4C);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 2, 0, 0, 1, 1);
    evtSubmitPrimaryFramePacket();

    context = dds3ObjectBaseBorrowModelContext(base);
    inner = context->inner;
    inner->flags |= 0x20;
    mdlLink(context, kwlnSceneSurfaceSets[9]);
    inner->flags &= ~0x20;
    dds3ObjectBaseOnFlag(object, 0x10000);

    eglSetOt(0x4E);
    evtSubmitAlternateFramePacket();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 2);
    evtSubmitGradientRectAtDepth(0, 0, 0x200, 0x1C0, 0x0EFFFFFF, 0x30586670, 0x30586670, 0x30586670, 0x30586670);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 1, 0, 0, 1, 2);
}

#include "fld_area_work.h"
extern u8 fldTestRoomObjectModeFlag(s32, u32, s32);
extern void mdlBlendEntryPitchYawAndUpdate(dds3ModelHandle_t *, struct sdfGrOt_t **, s32, f32, f32);
extern void sdfModelEvalTm(sdfModelRootNode_t *);

void effObjApplyTransformParameters(struct sdfGrOt_t **surfaces, dds3Basic_t *object) {
    dds3ObjectBaseControlWorkData_t *base;
    Dds3ObjectResourceState resourceState;
    struct SdfDmaTagHeader *lighting;
    sdfModelRootNode_t *model;
    s32 roomObjectIndex;

    base = dds3GetBaseControlWorkData(object);
    if (dds3ObjectBaseCheckFlag(object, 1)) {
        return;
    }
    if (base->resourceHandle == 0) {
        return;
    }

    resourceState = base->resourceState;
    if (resourceState != DDS3_OBJECT_RESOURCE_DEV_MOTION) {
        if (resourceState != DDS3_OBJECT_RESOURCE_MODEL_CONTEXT) {
            return;
        }
        if (dds3ObjectBaseCheckFlag(object, 0x800)) {
            dds3DrawObjectWithGradientOverlay(object);
            return;
        }
        lighting = (struct SdfDmaTagHeader *)((EffectObjectData *)object->workdata)->transitionWork->value;
        if (lighting != NULL) {
            dds3ObjectBaseBorrowModelContext(base)->inner->lighting = lighting;
        } else {
            dds3ObjectBaseBorrowModelContext(base)->inner->lighting = NULL;
        }
        if (dds3ObjectBaseCheckFlag(object, 0x2000)) {
            mdlBlendEntryPitchYawAndUpdate(dds3ObjectBaseBorrowModelContext(base), surfaces, base->currentSlot,
                                           ((EffectObjectData *)object->workdata)->angle,
                                           ((EffectObjectData *)object->workdata)->word34);
            return;
        }
        if (dds3ObjectBaseCheckFlag(object, 0x10000)) {
            model = dds3ObjectBaseBorrowModelContext(base)->inner;
            model->flags |= SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH;
            mdlLink(dds3ObjectBaseBorrowModelContext(base), kwlnSceneSurfaceSets[9]);
            model->flags &= ~SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH;
            dds3ObjectBaseOffFlag(object, 0x10000);
            return;
        }
        mdlLink(dds3ObjectBaseBorrowModelContext(base), surfaces);
        return;
    }

    if ((object->color >> 24) == 0) {
        return;
    }
    /* Each of the three motion paths calls the model update itself; retail
     * keeps a separate argument load per path (jump2 only merges the tail
     * after the call). */
    if (base->motion != NULL) {
        roomObjectIndex = ((EffectObjectData *)object->workdata)->word04;
        if (roomObjectIndex > 0) {
            if (fldTestRoomObjectModeFlag(fldGlobalBase.area, fldGlobalBase.floor + 1, roomObjectIndex) == 0) {
                switch (base->mode) {
                case DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED:
                    base->weight = 1.0f;
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST:
                    base->weight += 0.05f;
                    if (base->weight > 1.0f) {
                        base->weight = 1.0f;
                    }
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW:
                    base->weight += 0.025f;
                    if (base->weight > 1.0f) {
                        base->weight = 1.0f;
                    }
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO:
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST:
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW:
                case DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END:
                    break;
                }
                if (base->mode == DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END) {
                    sdfMotionSeek(base->motion, base->motion->frameCount);
                } else if (base->weight > 0.0f) {
                    base->motion->frameStep = base->weight;
                    sdfMotionNext(base->motion);
                }
                sdfModelEvalTm(dds3ObjectBaseBorrowDeviceMotionRoot(base));
            } else {
                switch (base->mode) {
                case DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED:
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST:
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW:
                    base->weight = 0.0f;
                    base->mode = DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO;
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO:
                    base->weight = 0.0f;
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST:
                    base->weight -= 0.05f;
                    if (base->weight < 0.0f) {
                        base->weight = 0.0f;
                    }
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW:
                    base->weight -= 0.02f;
                    if (base->weight < 0.0f) {
                        base->weight = 0.0f;
                    }
                    break;
                case DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END:
                    break;
                }
                if (base->mode == DDS3_OBJECT_MATTER_ANIME_MODE_SEEK_TO_END) {
                    sdfMotionSeek(base->motion, base->motion->frameCount);
                } else if (base->weight > 0.0f) {
                    base->motion->frameStep = base->weight;
                    sdfMotionNext(base->motion);
                }
                sdfModelEvalTm(dds3ObjectBaseBorrowDeviceMotionRoot(base));
            }
        } else {
            switch (base->mode) {
            case DDS3_OBJECT_MATTER_ANIME_MODE_FULL_SPEED:
                base->weight = 1.0f;
                break;
            case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_FAST:
                base->weight += 0.05f;
                if (base->weight > 1.0f) {
                    base->weight = 1.0f;
                }
                break;
            case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_UP_SLOW:
                base->weight += 0.025f;
                if (base->weight > 1.0f) {
                    base->weight = 1.0f;
                }
                break;
            case DDS3_OBJECT_MATTER_ANIME_MODE_HOLD_AT_ZERO:
                base->weight = 0.0f;
                break;
            case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_FAST:
                base->weight -= 0.05f;
                if (base->weight < 0.0f) {
                    base->weight = 0.0f;
                }
                /* Fallthrough is native. */
            case DDS3_OBJECT_MATTER_ANIME_MODE_RAMP_DOWN_SLOW:
                base->weight -= 0.02f;
                if (base->weight < 0.0f) {
                    base->weight = 0.0f;
                }
                break;
            }
            if (base->weight > 0.0f) {
                base->motion->frameStep = base->weight;
                sdfMotionNext(base->motion);
            }
            sdfModelEvalTm(dds3ObjectBaseBorrowDeviceMotionRoot(base));
        }
    }
    dds3ObjectBaseBorrowDeviceMotionRoot(base)->color = object->color;
    sdfBuildModelPacketsVia(sdfModelLinkOt, surfaces,
                           dds3ObjectBaseBorrowDeviceMotionRoot(base));
}

/* Create and attach kind-3 slot data only when the data slot is empty.
 * Existing data is retained; attachment still uses the new object's kind. */
void dds3ObjectBaseCreateMover(dds3Basic_t *object) {
    dds3Basic_t *existingData;
    dds3Basic_t *newData;

    existingData = dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_DATA_SLOT);
    if (existingData == NULL) {
        newData = dds3AddMoverBasic(object);
        dds3ObjectBaseSetControlBasic(object, newData);
        return;
    }
}

u8 dds3ObjectBaseSetPadControl(dds3Basic_t *object, Dds3MoverUpdate update) {
    return dds3ObjectBaseSetPrimaryControl(object, update);
}

/* Returns whether a slot-1 handler took the callback (0/1). The bool-width result is why
 * the wrapper above stays jal + epilogue in retail (001127A8, not a sibling j). */
u8 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *obj, Dds3MoverUpdate update) {
    dds3Basic_t *handler;

    handler = dds3ObjectBaseGetControlBasic(obj, DDS3_OBJECT_DATA_SLOT);
    if (handler == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(handler, update);
    return 1;
}

/* Store the source object on the data-slot handler, then replace the supplied
 * object's curve work. The replacement call uses object, not handler;
 * unlike the conditional dispatcher, this path does not check for NULL. */
void dds3ObjectBaseMakePath(dds3Basic_t *object, dds3Basic_t *sourceObject) {
    dds3Basic_t *handler;

    handler = dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_DATA_SLOT);
    dds3MoverBasicSetPath(handler, sourceObject);
    dds3MoverBasicMakePath(object);
}

/* Release the data slot's curve work and clear its handle, not the slot pointer.
 * The caller must have established the data slot. */
void dds3ObjectBaseFreePath(dds3Basic_t *object) {
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_DATA_SLOT));
}

/* Return the data slot's retained curve work; the data slot must exist. */
dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *object) {
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(object, DDS3_OBJECT_DATA_SLOT));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_003BA9C8);
