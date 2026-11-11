#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_command_parameter_index.h"
#include "dds3_basic_lifecycle.h"
#include "ps2asm.h"
#include "eff_inner_float_api.h"
#include "sdf_sif_command.h"
#include "scr_integer_return_value_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kernel.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "sdf.h"
#include "pty_read_roster_stat_api.h"
#include "pty_event_penalty_api.h"
#include "dds3_vu_transform.h"
#include "sdf_heap_api.h"
#include "basic.h"
#include "dds3_world_scene_node_view.h"
#include "dds3_world_value.h"
#include "dds3_world_lifecycle.h"
#include "fpu.h"
#include "dds3_world_counter.h"
#include "dds3_basic_array_cursor.h"
#include "dds3_basic_array_search.h"
#include "dds3_world_selection.h"
#include "fld_area_room_resource_load_api.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "sdf_texture_offset_list.h"
#include "dds3_basic_array_visit.h"
#include "scr.h"
#include "dds3_task_lifecycle.h"
#include "dds3_scene_membership.h"
#include "dds3_area_callback.h"
#include "dds3_object_control_slot.h"
#include "dds3_path.h"
#include "evt_action_object.h"
#include "sdk_ps2.h"
#include "dds3_camera_update_flags.h"
#include "eff_object.h"
#include "sdf_model_packet_callback.h"
#include "dds3_object_flags.h"
#include "sdf_model.h"
#include "sdf_model_node_index_api.h"
#include "sdf_model_eval_api.h"
#include "sdf_model_release_api.h"
#include "evt_gradient_rect.h"
#include "sdf_motion.h"
#include "sdf_motion_next_result.h"
#include "dds3_slot1_handler_api.h"
#include "dds3_object_resource_state.h"
#include "dds3_object_base_resource_view.h"
#include "dds3_object_mode.h"
#include "mdl.h"
#include "evt_unit.h"
#include "evt_script_model_api.h"

extern void Normalize(void);
void dds3BackupPoseSet(dds3PoseSet_t *node);
extern EffWorldOps *D_003299C0[EFF_WORLD_KIND_COUNT];
void dds3DeleteBasic(dds3Basic_t *node);

/* Allocate a node of `kind`, link its owner and run the owner's create hook. */
dds3Basic_t *dds3AllocBasic(u32 kind) {
    dds3Basic_t *node;
    EffWorldOps *ops;
    Dds3BasicCreateResult createResult;

    if (kind >= EFF_WORLD_KIND_COUNT) {
        return NULL;
    }
    node = sdfChipAllocDirect(sizeof(dds3Basic_t));
    if (node == NULL) {
        return NULL;
    }
    node->kindTag = kind << 24;
    node->color = 0x80808080;
    node->word0 = 0;
    node->key = 0;
    node->name = 0;
    ops = D_003299C0[kind];
    node->ops = ops;
    node->word14 = 0;
    node->workdata = NULL;
    node->poseset = NULL;
    node->next = NULL;
    node->previous = NULL;
    node->word28 = 0;
    node->word2C = 0;
    node->owner = NULL;
    if (ops != NULL && ops->create != NULL) {
        createResult = ops->create(node);
        if (createResult != DDS3_BASIC_CREATE_ACCEPTED) {
            dds3DeleteBasic(node);
            return NULL;
        }
    }
    return node;
}

/* Notify the owner before unlinking and freeing this transform node. */
void dds3DeleteBasic(dds3Basic_t *node) {
    EffWorldOps *owner;
    dds3Basic_t *next;
    dds3Basic_t *prev;

    if (node != NULL) {
        owner = node->ops;
        if (owner != NULL) {
            if (owner->destroy != NULL) {
                owner->destroy(node);
            }
        }
        next = node->previous;
        if (next != NULL) {
            next->next = node->next;
        }
        prev = node->next;
        if (prev != NULL) {
            prev->previous = node->previous;
        }
        sdfChipFreeDirect(node);
    }
}

u8 dds3InitPoseSet(dds3Basic_t *node) {
    dds3PoseSet_t *inner;

    if (node == NULL) {
        return 0;
    }
    if (node->poseset != NULL) {
        return 0;
    }
    inner = sdfChipAllocDirect(sizeof(dds3PoseSet_t));
    if (inner == NULL) {
        return 0;
    }
    inner->flags = OBJECT_TRANSFORM_FLAG_UPDATE_PENDING;
    dds3BackupPoseSet(inner);
    SDF_VU0_STOREIDENTITYVECTOR(&inner->unkB0);
    node->poseset = inner;
    inner->unkC8 = 0;
    inner->radius = 0.0f;
    return 1;
}

void dds3FreePoseSet(dds3Basic_t *node) {
    dds3PoseSet_t *inner;

    if (node != NULL) {
        inner = node->poseset;
        if (inner != NULL) {
            sdfChipFreeDirect(inner);
            node->poseset = NULL;
        }
    }
}

void dds3OnPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    node->flags |= flags;
}

void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    node->flags &= ~flags;
}

u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void dds3BackupPoseSet(dds3PoseSet_t *node) {
    u8 *p40 = (u8 *)&node->pos;
    u8 *p50;
    u8 *p60;

    SDF_VU0_STOREIDENTITYVECTOR(p40);
    p50 = (u8 *)&node->rotation;
    SDF_VU0_STOREIDENTITYVECTOR(p50);
    SDF_VU0_SETSCALE1VECTOR();
    p60 = (u8 *)&node->scale;
    SDF_VU0_STOREVECTOR(p60);
}

void dds3SetRadiusPoseSet(dds3PoseSet_t *node) {
    SDF_VU0_COPYVECTOR(&node->savedScale, &node->scale);
    SDF_VU0_COPYVECTOR(&node->savedRotation, &node->rotation);
    SDF_VU0_COPYVECTOR(&node->savedPosition, &node->pos);
}

void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value) {
    node->poseset->radius = value;
}

f32 effObjGetInnerFloat(dds3Basic_t *node) {
    return node->poseset->radius;
}

void dds3SetPosPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;
    f32 *dst = inner->pos;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3SetRotPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;
    f32 *dst = inner->rotation;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3SetSclPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;
    f32 *dst = inner->scale;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3GetPosPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->poseset->pos;

    SDF_VU0_LOADVECTOR_BARRIER(p);
    SDF_VU0_VECTOR_W_SET_ONE();
}

void dds3GetRotPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->poseset->rotation;

    SDF_VU0_LOADVECTOR_BARRIER(p);
    Normalize();
}

void effObjFetchInnerScale(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->poseset->scale;

    SDF_VU0_LOADVECTOR_BARRIER(p);
}

void dds3GetPosPoseSetMCR(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;
    u8 *src = (u8 *)&inner->pos;
    u8 *dst;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(vector);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_OFFSET(dst, inner, 0x40);
}

void dds3AddRotPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_LOADVECTOR(&inner->rotation);
    SDF_VU0_LOADVECTOR_B(vector);
    Mult();
    SDF_VU0_STOREVECTOR(&inner->rotation);
}

void dds3GetSclPoseSetMCR(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->poseset;
    u8 *src = (u8 *)&inner->scale;
    u8 *dst;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(vector);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_OFFSET(dst, inner, 0x60);
}
/* vu0 routine: build the world matrix in vf28-vf31 from rotation (quaternion),
   per-row scale and translation. */
void dds3MakeMatrix(const f32 scale[4],
                                       const f32 rotation[4],
                                       const f32 translation[4]) {
    SDF_VU0_LOADVECTOR(rotation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(scale);
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(translation);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
}
extern u32 dds3ObjectIdCounter;

/* Load the cached VU matrix, or rebuild and cache it when flags bit 1 is clear. */
void dds3GetTMPoseSet(dds3Basic_t *object) {
    dds3PoseSet_t *transform = object->poseset;
    u32 flags = transform->flags;

    if (flags & OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID) {
        SDF_VU0_LOADMATRIX(transform->matrix);
    } else {
        transform->flags = flags | OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
        dds3MakeMatrix(transform->scale, transform->rotation, transform->pos);
        SDF_VU0_STOREMATRIX(transform->matrix);
    }
}

extern float fabsf(float);

s32 dds3ComparePosPoseSet(const dds3PoseSet_t *left, const dds3PoseSet_t *right) {
    f32 length;
    f32 leftLimit;
    f32 rightLimit;

    SDF_VU0_LOADVECTOR(left->pos);
    SDF_VU0_LOADVECTOR_B(right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    leftLimit = left->radius;
    rightLimit = right->radius;
    if (fabsf(length) - (leftLimit + rightLimit) < 0.0f) {
        return 1;
    }
    return 0;
}

/* vu0 routine: whether the distance between the position vectors is inside the second object's radius */
s32 dds3ComparePosPoseSet2(dds3PoseSet_t *left, dds3PoseSet_t *right) {
    f32 length;
    f32 rightLimit;
    SDF_VU0_LOADVECTOR(left->pos);
    SDF_VU0_LOADVECTOR_B(right->pos);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(length);
    rightLimit = right->radius;
    return ffabsf(length) - rightLimit < 0.0f;
}



/* Invoke the holder's first / second lifecycle callback when present; the result is 1 when there is nothing to call. */
s32 dds3CallBasicMainProc(dds3Basic_t *holder) {
    s32 result = 1;

    if (holder != NULL) {
        EffWorldOps *table = holder->ops;

        if (table != NULL && table->update != NULL) {
            result = table->update(holder);
        }
    }
    return result;
}

s32 dds3CallBasicDrawProc(dds3Basic_t *holder) {
    s32 result = 1;

    if (holder != NULL) {
        EffWorldOps *table = holder->ops;

        if (table != NULL && table->draw != NULL) {
            result = table->draw(holder);
        }
    }
    return result;
}

/* Advance and return the 16-bit sequence stored in a 32-bit backing word. */
u32 dds3AllocObjectId(void) {
    dds3ObjectIdCounter = (dds3ObjectIdCounter + 1) & 0xffff;
    return dds3ObjectIdCounter;
}

void dds3SetUnitObjectId(dds3Basic_t *node, const char *objectName) {
    if (node != NULL) {
        node->name = objectName;
    }
}

u32 dds3GetWorldNodeValue(dds3Basic_t *node) {
    u32 worldNodeValue;

    worldNodeValue = 0;
    if (node != NULL) {
        worldNodeValue = (u32)node->name;
    }
    return worldNodeValue;
}

INCLUDE_SDATA(const s32, "basic/dds3Basic", dds3ObjectIdCounter);

INCLUDE_SDATA(const s32, "basic/dds3Basic", dds3TheWorld);

