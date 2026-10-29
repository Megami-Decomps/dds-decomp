#include "sdk_ps2.h"
#include "common.h"
#include "sdf_map_position_api.h"
#include "sdf_vu0.h"
#include "dds3_object_flags.h"
#include "mdl.h"
#include "sdf_chip.h"
#include "bill_object_api.h"
#include "sdf_resource.h"
#include "eff_dependency.h"
#include "eff_parameter_position_readiness.h"
#include "eff_object.h"
#include "eff_object_dispatch_api.h"
#include "pcp_vu0.h"
#include "dds3obj.h"
#include "dds3_object_control_slot.h"
#include "dds3_object_base_resource_view.h"
#include "dds3_object_resource_state.h"
#include "dds3_scene_membership.h"
#include "eff.h"
#include "eff_magatuhi_api.h"
#include "eff_node.h"
#include "eff_create_node_from_parameter.h"
#include "eff_node_descriptor.h"
#include "eff_event.h"
#include "eff_event_sound.h"
#include "evt_world_source_transform.h"
#include "sdf_draw.h"
#include "btl_sound.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"

#define EFF_OBJ_KIND EFF_WORLD_KIND_EFFECT_OBJECT
#define EFF_OBJ_VECTOR_BYTES 0x10
#define EFF_OBJ_ENTRY_ID_MASK 0xFFFF
#define EFF_OBJ_FLAG_OWNER_LINK 4
#define EFF_OBJ_FLAG_OWNER_BILL_ENTRY 8
#define EFF_OBJ_OWNER_KIND_FIRST 4
#define EFF_OBJ_OWNER_KIND_END 10
#define EFF_OBJ_OWNER_KIND_EXTRA 0x11
#define EFF_OBJ_OWNER_BILL_KIND 5



typedef dds3PoseSet_t EffectParameters;


void dds3FreePoseSet(dds3Basic_t *node);
void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *objectBase);
/* Dispatchers take (bill handle, 16-byte vector, extra); the vector is
   loaded with lqc2 and the extra is forwarded to dds3AddEffectObjectBasic. */
dds3Basic_t *AddP2AEffect(billHandle_t *bill, const u128 *vec, s32 extra);
void dds3BillboardSetAnimNumber(struct billHandle_t *arg, u32 entryIndex);
dds3Basic_t *AddTMXEffect(void *bill, const u128 *vec, s32 extra);
dds3Basic_t *effObjCreateWithBoundBill(void *bill, const u128 *vec, void *secondVector);
dds3Basic_t *effObjCreateBillboardInWorld(void *bill, const u128 *firstVector, void *secondVector);
dds3Basic_t *AddBEDEffect();
dds3Basic_t *AddESPEffect(const char *name, const u128 *firstVector, void *secondVector, f32 scale);
/* Old-style (K&R) callee: callers pass (kind, value) positionally. */
dds3Basic_t *AddMG12Effect();
extern const f32 D_0039F800[10];
extern const f32 D_0039F828[20];
EffectDependencyState *CheckESP(dds3Basic_t *obj);
extern void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *obj);

struct EffNode;
extern dds3Basic_t *dds3GetFirstWorldObjectNodeOfKind2(void);

extern billHandle_t *billCreateFromResource(s32 kind, const char *path);





extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);



/* Release dependencies, object base, then slot data; the object and data must exist. */
void dds3EffectObjectBasicDestructor(dds3Basic_t *obj) {
    EffectDependencyState *data;

    data = obj->workdata;
    FreeEffectData(data);
    dds3FreePoseSet(obj);
    dds3ObjectBaseFreeControlWorkData(data->objectHandle);
    sdfChipFreeDirect(obj->workdata);
    obj->workdata = NULL;
}

#include "dds3_world_primary.h"
extern void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags);
extern void Normalize(void);

/* Refresh the world position and orientation from the owner link (or the object's own
   inner transform), then forward them to the bound node according to the state. */
s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *obj) {
    f32 matrix[16];
    f32 position[4];
    f32 rotation[4] = {0, 0, 0, 1.0f};
    EffectDependencyState *data;
    s32 moved = 0;
    struct EffNode *node;
    struct billHandle_t *bill;
    s32 result;

    data = obj->workdata;
    if ((data->flags & EFF_OBJ_FLAG_OWNER_LINK) != 0) {
        if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), data->ownerKind, data->owner) != 0) {
            if ((data->flags & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) != 0) {
                dds3EffectObjectBasicGetObjectIndex(obj);
            } else {
                switch (data->ownerKind) {
                case 4: case 5: case 6: case 7: case 8: case 9:
                    SDF_VU0_LOADVECTOR(data->owner->poseset->rotation);
                    SDF_VU0_STOREVECTOR(rotation);
                    effMiscQuaternionToMatrixVU();
                    break;
                case EFF_OBJ_OWNER_KIND_EXTRA:
                    SDF_VU0_LOADVECTOR(((EvtWorldSourceTransformPrefix *)data->owner->workdata)->rotation);
                    SDF_VU0_STOREVECTOR(rotation);
                    effMiscQuaternionToMatrixVU();
                    break;
                default:
                    SDF_VU0_LOADIDENTITY();
                    break;
                }
            }
            SDF_VU0_COPYMATRIX_A_TO_B();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR(data->ownerLocalOffset);
            SDF_VU0_VECTOR_W_SET_ONE();
            SDF_VU0_TRANSLATE();
            MultMatrixRevNoArg();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_APPLYMATRIX();
            if ((data->flags & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) == 0) {
                switch (data->ownerKind) {
                case 4: case 5: case 6: case 7: case 8: case 9:
                    SDF_VU0_LOADVECTOR_B(data->owner->poseset->pos);
                    SDF_VU0_ADDVECTOR();
                    break;
                case EFF_OBJ_OWNER_KIND_EXTRA:
                    SDF_VU0_LOADVECTOR_B(&((EvtWorldSourceTransformPrefix *)data->owner->workdata)->position);
                    SDF_VU0_ADDVECTOR();
                    break;
                }
            }
            SDF_VU0_STOREVECTOR(position);
            dds3SetRadiusPoseSet(obj->poseset);
            SDF_VU0_STOREVECTOR(obj->poseset->pos);
            moved = 1;
            dds3OffPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        } else {
            dds3EffectObjectBasicUnlinkObject(obj);
        }
    } else {
        dds3OffPoseSetFlag(obj->poseset, OBJECT_TRANSFORM_FLAG_UPDATE_PENDING);
        SDF_VU0_LOADVECTOR(obj->poseset->pos);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
        dds3SetRadiusPoseSet(obj->poseset);
        SDF_VU0_LOADVECTOR(obj->poseset->rotation);
        Normalize();
        SDF_VU0_STOREVECTOR(rotation);
        moved = 1;
    }
    switch (data->state) {
    case EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE:
        node = data->handle;
        if (node != NULL && moved) {
            effCopyVectorToNodeInstance(node, position);
            SDF_VU0_LOADVECTOR(rotation);
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(matrix);
            effApplyNodeTransformMatrix(node, matrix);
        }
        result = effInvokeNodeConditionOrAcceptDefault(node);
    expired:
        if (result == EFF_NODE_CONDITION_EXPIRED) {
            u32 flags = data->flags;

            data->flags = flags & ~1;
            if (flags & 2) {
                dds3DeleteSceneBasicData(obj);
            }
        }
        return 1;
    case EFF_DEPENDENCY_STATE_BILLBOARD_ANIMATION:
        bill = data->handle;
        if (bill != NULL && moved) {
            effCopyVector(bill->position, position);
        }
        result = dds3BillboardGetAnimState(bill);
        goto expired;
    case EFF_DEPENDENCY_STATE_BILLBOARD_NODE:
        if (data->handle != NULL && moved) {
            effCopyVector(((billHandle_t *)data->handle)->position, position);
        }
        return 1;
    case EFF_DEPENDENCY_STATE_BORROWED_HANDLE:
        return 1;
    case EFF_DEPENDENCY_STATE_EVENT_CUT: {
        void *eventNode = data->node;

        if (eventNode != NULL) {
            if (moved) {
                u8 *record = data->vector;

                if (record != NULL) {
                    SDF_VU0_COPYVECTOR(record, position);
                    SDF_VU0_COPYVECTOR(record + 0x10, rotation);
                    effEveBEDCutSetParams((effEveBEDCutHandle_t *)eventNode, record);
                }
            }
        }
        return 1;
    }
    }
    return 1;
}

struct billHandle_t;
struct EffNode;

extern void dds3BillboardDraw(struct billHandle_t *bill);

s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *obj) {
    EffectDependencyState *data;

    data = obj->workdata;
    dds3EffectObjectBasicCalcFldEx(obj);
    if ((data->flags & 1) == 0) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(obj, 1)) {
        return 1;
    }

    switch (data->state) {
    case 1:
    case 7:
    case 8:
        if (data->handle != NULL) {
            dds3EffectDispatchUpdate(data->handle);
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            dds3BillboardDraw(data->handle);
        }
        break;
    case 5:
        if (data->node != NULL) {
            effEventUpdateEffectParameters(data->node);
        }
        break;
    }
    return 1;
}

/* Return the object-base handle without checking the object or its slot data. */
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *object) {
    return ((EffectDependencyState *)object->workdata)->objectHandle;
}

dds3Basic_t *dds3AddEffectObjectBasic(u32 worldCounter, const u128 *position, const u128 *rotation) {
    dds3Basic_t *obj;
    EffectDependencyState *data;

    obj = dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_OBJECT);
    if (obj == NULL) {
        return NULL;
    }
    obj->key = worldCounter;
    dds3ObjectBaseCreateMover(obj);
    dds3SetPosPoseSet(obj, (u128 *)position);
    dds3SetRotPoseSet(obj, (u128 *)rotation);
    dds3SetRadiusPoseSet(obj->poseset);
    data = obj->workdata;
    data->flags = 0;
    data->state = EFF_DEPENDENCY_STATE_EMPTY;
    data->handle = NULL;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    return obj;
}



dds3Basic_t *AddP2AEffect(billHandle_t *bill, const u128 *vec, s32 extra) {
    f32 vector[4];
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *handle;
    dds3Basic_t *id;

    memset(vector, 0, sizeof(vector));
    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), vec, (u128 *)extra);
    if (obj == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(vec);
    SDF_VU0_STOREVECTOR(vector);
    effCopyVector(bill->position, vector);
    data = obj->workdata;
    data->handle = bill;
    data->flags = 0;
    data->state = EFF_DEPENDENCY_STATE_BILLBOARD_ANIMATION;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    handle = dds3EffectObjectBasicGetControlWorkData(obj);
    handle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    id = dds3GetFirstWorldObjectNodeOfKind2();
    if (id != NULL) {
        handle->slots[DDS3_OBJECT_HANDLER_SLOT] = id;
        dds3AreaBasicRegistItem(id, obj);
    }
    return obj;
}

/* Clone the shared bill and return the newly constructed effect object. */
dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *obj, const u128 *firstVector, s32 secondVectorAddress) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((EffectDependencyState *)obj->workdata)->handle);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

/* Create a kind-one indexed bill; the native constructor result is discarded. */
void dds3AddP2AEffectObjectBasic2(u32 billId, const u128 *firstVector, s32 secondVectorAddress) {
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(1, billId);
    AddP2AEffect(bill, firstVector, secondVectorAddress);
}

/* Create a kind-one resource bill and return its effect object. */
dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, const u128 *firstVector, s32 secondVectorAddress) {
    billHandle_t *bill;

    bill = billCreateFromResource(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

/* Select the stored bill's kind-one entry; no object/data/bill checks are made. */
void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *obj, u32 entryIndex) {
    dds3BillboardSetAnimNumber(((EffectDependencyState *)obj->workdata)->handle, entryIndex);
}

/* Create a state-three bill effect and attach the first kind-two world node if present.
   secondVectorAddress retains the integer ABI used for the second vector pointer. */
dds3Basic_t *AddTMXEffect(void *bill, const u128 *firstVector, s32 secondVectorAddress) {
    f32 copiedVector[4];
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector, (void *)secondVectorAddress);
    if (obj == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(firstVector);
    SDF_VU0_STOREVECTOR(copiedVector);
    effCopyVector(((billHandle_t *)bill)->position, copiedVector);
    data = obj->workdata;
    data->state = EFF_DEPENDENCY_STATE_BILLBOARD_NODE;
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

/* Clone the shared bill and return its state-three effect object. */
dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *obj, const u128 *firstVector, s32 secondVectorAddress) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((EffectDependencyState *)obj->workdata)->handle);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

/* Create a kind-zero indexed bill for the state-three constructor. */
dds3Basic_t *effObjCreateIndexedKindZero(u32 billId, const u128 *firstVector, s32 secondVectorAddress) {
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(0, billId);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

/* Create a kind-zero resource bill for the state-three constructor. */
dds3Basic_t *effObjCreateResourceKindZero(const char *path, const u128 *firstVector, s32 secondVectorAddress) {
    void *bill;

    bill = billCreateFromResource(0, path);
    return AddTMXEffect(bill, firstVector, secondVectorAddress);
}

/* Bind the supplied bill through its node-instance vector copy, selecting state one.
   The object-base state word remains a separate native value of two. */
dds3Basic_t *effObjCreateWithBoundBill(void *bill, const u128 *firstVector, void *secondVector) {
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector,
                                  (u128 *)secondVector);
    if (obj == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(firstVector);
    SDF_VU0_STOREVECTOR(copiedVector);
    effCopyVectorToNodeInstance(bill, copiedVector);
    data = obj->workdata;
    data->state = EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE;
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

/* Create the descriptor-sourced node and return its bound world object. */
dds3Basic_t *effObjSpawnDescriptorBoundEffect(struct EffNodeDescriptor *descriptor, const u128 *firstVector, void *secondVector) {
    struct EffNode *bill;

    bill = dds3EffectLoad(descriptor);
    return effObjCreateWithBoundBill(bill, firstVector, secondVector);
}

/* Load and bind the named resource, returning the created object. */
dds3Basic_t *effObjSpawnLoadedResourceEffect(const char *resourceName, const u128 *firstVector, void *secondVector) {
    struct EffNode *bill;

    bill = dds3EffectFileLoad(resourceName);
    return effObjCreateWithBoundBill(bill, firstVector, secondVector);
}

/* The world-bill entry uses the same state-one/node-instance initialization as binding. */
dds3Basic_t *effObjCreateBillboardInWorld(void *bill, const u128 *firstVector, void *secondVector) {
    u8 copiedVector[EFF_OBJ_VECTOR_BYTES];
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    memset(copiedVector, 0, sizeof(copiedVector));
    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector,
                                  (u128 *)secondVector);
    if (obj == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(firstVector);
    SDF_VU0_STOREVECTOR(copiedVector);
    effCopyVectorToNodeInstance(bill, copiedVector);
    data = obj->workdata;
    data->state = EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE;
    data->handle = bill;
    data->flags = 0;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

/* Feed the third source factory's result to the world-bill constructor. */
dds3Basic_t *dds3AddD3PEffectObjectBasic(void *resourceData, const u128 *firstVector, void *secondVector) {
    struct EffNode *bill;

    bill = effCreateNodeFromParameter((u32)resourceData);
    return effObjCreateBillboardInWorld(bill, firstVector, secondVector);
}

extern const EffectEventVectorParameters D_0039F7D0;


dds3Basic_t *AddBEDEffect(source, firstVector, secondVector)
    SoundMixer *source;
    const u128 *firstVector;
    const u128 *secondVector;
{
    EffectEventVectorParameters parameters = D_0039F7D0;
    dds3Basic_t *obj;
    SoundMixer *mixer;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector,
                                  (u128 *)secondVector);
    if (obj == NULL) {
        return NULL;
    }
    SDF_VU0_COPYVECTOR(parameters.firstVector, firstVector);
    SDF_VU0_COPYVECTOR(parameters.secondVector, secondVector);
    mixer = effEveBEDCreate(source);
    data = obj->workdata;
    data->state = EFF_DEPENDENCY_STATE_EVENT_CUT;
    data->handle = mixer;
    data->flags = 0;
    data->node = NULL;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    data->vector = sdfChipAllocDirect(sizeof(parameters));
    memcpy(data->vector, &parameters, sizeof(parameters));
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}


dds3Basic_t *dds3AddBEDEffectObjectBasic2(struct SoundMixer *resource, const u128 *position, const void *scale) {
    return AddBEDEffect(resource, position, scale);
}

/* Resolve the named resource, create from it, then release the temporary resource. */
dds3Basic_t *dds3AddBEDEffectObjectBasic(const char *resourcePath, const u128 *firstVector, void *secondVector) {
    u32 resolvedId;
    struct sdfMemHandle_t *resourceHandle;
    dds3Basic_t *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(resourcePath, &resolvedId, 0);
    created = AddBEDEffect((SoundMixer *)resolvedId, firstVector, secondVector);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

/* For state-five data with a bill and vector, release the old event node and replace it.
   The lookup uses only the low sixteen bits; its result replaces the released node. */
void dds3StartBEDEffectObjectBasic(dds3Basic_t *obj, u32 entryId) {
    EffectDependencyState *data;

    data = obj->workdata;
    if (data->state != EFF_DEPENDENCY_STATE_EVENT_CUT) {
        return;
    }
    if (data->vector == NULL) {
        return;
    }
    if (data->handle == NULL) {
        return;
    }
    if (data->node != NULL) {
        effEveBEDCutDelete(data->node);
        data->node = NULL;
    }
    data->node = effEveBEDCutCreate(data->handle, (u16)(entryId & EFF_OBJ_ENTRY_ID_MASK), data->vector);
}

dds3Basic_t *AddESPEffect(const char *name, const u128 *firstVector, void *secondVector, f32 scale) {
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), firstVector,
                                  (u128 *)secondVector);
    if (obj == NULL) {
        return NULL;
    }
    data = obj->workdata;
    data->state = EFF_DEPENDENCY_STATE_PARAMETERS_READY;
    obj->poseset->scale[0] = scale;
    obj->poseset->scale[1] = scale;
    obj->poseset->scale[2] = scale;
    data->flags = 0;
    data->handle = NULL;
    data->node = NULL;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    obj->name = name;
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, const u128 *firstVector, void *secondVector, f32 scale) {
    return AddESPEffect(name, firstVector, secondVector, scale);
}

dds3Basic_t *AddMG12Effect(kind, descriptor)
    s32 kind;
    struct EffNodeDescriptor *descriptor;
{
    f32 firstVector[4];
    f32 secondVector[4];
    f32 twoRows[10];
    f32 fourRows[20];
    struct EffNode *bill;
    dds3Basic_t *obj;
    EffectDependencyState *data;
    dds3ObjectBaseControlWorkData_t *objectHandle;
    dds3Basic_t *worldNode;

    memset(firstVector, 0, sizeof(firstVector));
    memset(secondVector, 0, sizeof(secondVector));
    secondVector[3] = 1.0f;
    memcpy(twoRows, D_0039F800, sizeof(twoRows));
    memcpy(fourRows, D_0039F828, sizeof(fourRows));
    bill = dds3EffectLoad(descriptor);
    obj = dds3AddEffectObjectBasic(dds3AllocObjectId(), (u128 *)firstVector,
                                  (u128 *)secondVector);
    if (obj == NULL) {
        return NULL;
    }
    data = obj->workdata;
    data->flags = 0;
    if (kind == 1) {
        data->state = EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS;
    } else {
        data->state = EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS;
    }
    data->handle = bill;
    data->owner = NULL;
    data->entryId = 0;
    data->ownerKind = 0;
    data->node = sdfChipAllocDirect(0x10);
    memset(data->node, 0, 0x10);
    switch (kind) {
    case 1:
        data->vector = sdfChipAllocDirect(sizeof(twoRows));
        memcpy(data->vector, twoRows, sizeof(twoRows));
        effMagaSCSetArea(bill, data->vector);
        break;
    case 2:
        data->vector = sdfChipAllocDirect(sizeof(fourRows));
        memcpy(data->vector, fourRows, sizeof(fourRows));
        effMagaP4SetPoint(bill, data->vector);
        break;
    }
    objectHandle = dds3EffectObjectBasicGetControlWorkData(obj);
    objectHandle->resourceState = DDS3_OBJECT_RESOURCE_EFFECT_DEPENDENCY;
    worldNode = dds3GetFirstWorldObjectNodeOfKind2();
    if (worldNode != NULL) {
        objectHandle->slots[DDS3_OBJECT_HANDLER_SLOT] = worldNode;
        dds3AreaBasicRegistItem(worldNode, obj);
    }
    return obj;
}

dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 kind, struct EffNodeDescriptor *descriptor) {
    return AddMG12Effect(kind, descriptor);
}

/* Resolve the loader's address word as a descriptor before passing it onward. */
dds3Basic_t *dds3AddMG12EffectObjectBasic(s32 kind, const char *resourcePath) {
    union {
        u32 address;
        EffNodeDescriptor *descriptor;
    } resource;
    struct sdfMemHandle_t *resourceHandle;
    dds3Basic_t *created;

    resource.address = 0;
    resourceHandle = sdfDevLoadFileEx(resourcePath, &resource.address, 0);
    created = AddMG12Effect(kind, resource.descriptor);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

/* Return data only for a non-NULL effect-kind object in parameter-ready state.
   Matching objects are still assumed to contain valid slot data. */
EffectDependencyState *CheckESP(dds3Basic_t *obj) {
    EffectDependencyState *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (((u8 *)&obj->kindTag)[3] != EFF_OBJ_KIND) {
        return data;
    }
    data = obj->workdata;
    if (data->state != EFF_DEPENDENCY_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}

/* Publish the position in vf10; report readiness separately from the scalar value. */
EffectParameterPositionReadiness GetESPPos(dds3Basic_t *obj) {
    if (CheckESP(obj) == NULL) {
        return EFFECT_PARAMETER_POSITION_NOT_READY;
    }
    SDF_VU0_LOADVECTOR(obj->poseset->pos);
    return EFFECT_PARAMETER_POSITION_READY;
}

/* Truncate the ready scalar; zero can be data, so callers check position readiness first. */
s32 GetESPR(dds3Basic_t *obj) {
    EffectParameters *parameters;

    if (CheckESP(obj) == NULL) {
        return 0;
    }
    parameters = obj->poseset;
    return (s32)parameters->scale[0];
}

extern void effMagaP4Randomize(struct EffNode *node);
extern void effMagaRandomize(struct EffNode *node);

/* Dispatch bound-bill setup or four-row history initialization; two-row state does nothing.
   The legacy s32 result is unspecified and every caller discards it. */
s32 dds3EffectObjectPresetMG12(dds3Basic_t *obj) {
    if (((u8 *)&obj->kindTag)[3] == EFF_OBJ_KIND) {
        EffectDependencyState *data = obj->workdata;

        switch ((s32)data->state) {
        case EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS:
            effMagaP4Randomize(data->handle);
            break;
        case EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE:
            effMagaRandomize(data->handle);
            break;
        }
    }
}
/* Copy ready inputs into two or four vector rows, then dispatch the whole existing block.
   An unavailable input leaves its destination row/scalar unchanged, not zeroed. */
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *obj, dds3Basic_t *first, dds3Basic_t *second, dds3Basic_t *third, dds3Basic_t *fourth)
{
    EffectDependencyState *data;
    f32 *parameterRows;

    if (((u8 *)&obj->kindTag)[3] != EFF_OBJ_KIND) {
        return 0;
    }
    data = obj->workdata;
    switch (data->state) {
    case EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS:
        parameterRows = data->vector;
        if (GetESPPos(first) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows);
            parameterRows[8] = GetESPR(first);
        }
        if (GetESPPos(second) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows + 4);
            parameterRows[9] = GetESPR(second);
        }
        effMagaSCSetArea(data->handle, (const EffMagatuhiFloatParams *)parameterRows);
        return 1;
    case EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS:
        parameterRows = data->vector;
        if (GetESPPos(first) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows);
            parameterRows[16] = GetESPR(first);
        }
        if (GetESPPos(second) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows + 4);
            parameterRows[17] = GetESPR(second);
        }
        if (GetESPPos(third) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows + 8);
            parameterRows[18] = GetESPR(third);
        }
        if (GetESPPos(fourth) != EFFECT_PARAMETER_POSITION_NOT_READY) {
            SDF_VU0_STOREVECTOR(parameterRows + 12);
            parameterRows[19] = GetESPR(fourth);
        }
        effMagaP4SetPoint(data->handle, (const EffMagatuhiRowsSrc *)parameterRows);
        return 1;
    }
    return 0;
}

/* OR the requested flag bits into slot data; no object/data validation is performed. */
void dds3EffectObjectBasicStatOn(dds3Basic_t *obj, u32 flags) {
    ((EffectDependencyState *)obj->workdata)->flags |= flags;
}

/* Clear the requested flag bits from the unsigned effect-state mask. */
void dds3EffectObjectBasicStatOff(dds3Basic_t *obj, u32 flags) {
    ((EffectDependencyState *)obj->workdata)->flags &= ~flags;
}

/* Accept owner kinds [4,10) or seventeen, reset link bookkeeping, and return one.
   Unsupported kinds return zero without changing the destination; owner must exist. */
s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *obj, dds3Basic_t *owner) {
    EffectDependencyState *data;
    u8 kind;

    kind = ((u8 *)&owner->kindTag)[3];
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = obj->workdata;
    data->ownerLocalOffset[2] = 0.0f;
    data->owner = owner;
    data->entryId = 0;
    data->flags |= EFF_OBJ_FLAG_OWNER_LINK;
    data->flags &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    data->ownerLocalOffset[0] = 0.0f;
    data->ownerKind = ((u8 *)&owner->kindTag)[3];
    data->ownerLocalOffset[1] = 0.0f;
    return 1;
}

/* Forward the saved entry only for flagged kind-five owners whose bill state is zero.
   Owner/bill/parameter pointers are trusted once the flag selects this path. */
void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *obj) {
    EffectDependencyState *data;
    dds3Basic_t *owner;
    dds3ObjectBaseControlWorkData_t *objectBase;
    dds3ModelHandle_t *model;

    data = obj->workdata;
    if (data->flags & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->owner;
        if (((u8 *)&owner->kindTag)[3] == EFF_OBJ_OWNER_BILL_KIND) {
            objectBase = ((EffectObjectData *)owner->workdata)->modelHolder;
            if (objectBase->resourceState != DDS3_OBJECT_RESOURCE_MODEL_CONTEXT) {
                return;
            }
            model = dds3ObjectBaseBorrowModelContext(objectBase);
            sdfModelGetMarkPosMatrix(model->inner, data->entryId);
        }
    }
}

/* Save a truncated u16 entry and enable forwarding after owner validation.
   Native status is always one, including rejected owners. */
s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *obj, dds3Basic_t *owner, s32 entryId) {
    EffectDependencyState *data;

    if (dds3EffectObjectBasicLinkObject(obj, owner) == 0) {
        return 1;
    }
    data = obj->workdata;
    data->entryId = entryId;
    data->flags |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    return 1;
}

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F7D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F800);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F828);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F878);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F890);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8C0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8E8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F900);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F918);
