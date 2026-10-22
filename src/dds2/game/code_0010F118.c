#include "common.h"
#include "eff_inner_float_api.h"
#include "sdf_sif_command.h"
#include "scr_integer_return_value_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "pcp_vu0.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "sdf.h"

extern s32 scrGetIntPara(s32);

extern s8 ptyReadSignedRosterStatByte(s32);
extern void ptyApplyEventPenalty(s32 penaltyMode);



extern void Normalize(void);
void dds3BackupPoseSet(dds3PoseSet_t *node);

u32 ptyScriptApplyEventPenalty(void) {
    s32 penaltyMode;

    penaltyMode = scrGetIntPara(0);
    ptyApplyEventPenalty(penaltyMode);
    return 1;
}

extern void ptyRecoverAllUnits(void);

u32 ptyScriptRestoreEntireParty(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 ptyScriptGetRosterStat(void) {
    s32 value;

    value = scrGetIntPara(0);
    value = ptyReadSignedRosterStatByte(value);
    scrSetIntReturnValue(value);
    return 1;
}

extern char D_00435D70[];
extern char D_00435D78[];
extern char D_00435D80[];

/* Each transform section owns its row coordinate; see docs/idioms.md,
 * "Trace-justified diagnostic row coordinates". */
s32 DispBaseData(void *worldNode, s32 x, s32 y, sdfDrawTag_t *list) {
    dds3Basic_t *node = worldNode;
    dds3PoseSet_t *transform;
    s32 labelX;
    s32 valueX;
    s32 positionY;
    s32 rotationY;
    s32 scaleY;
    s32 lines;

    x *= 12;
    labelX = (x << 4) + 0x7000;
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "ID    : 0x%05X", node->key));
    sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
        (y++ * 6 << 4) + 0x7900, 0xFEFFFF, 0, "NAME  : %s", node->value));
    transform = node->inner;
    lines = 2;
    if (transform != NULL) {
        y++;
        positionY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            positionY, 0xFEFFFF, 0, D_00435D70));
        valueX = ((x + 84) << 4) + 0x7000;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            positionY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f",
            transform->position[0], transform->position[1], transform->position[2]));
        y += 2;
        rotationY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            rotationY, 0xFEFFFF, 0, D_00435D78));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            rotationY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f %3.2f",
            transform->rotation[0], transform->rotation[1],
            transform->rotation[2], transform->rotation[3]));
        y += 2;
        scaleY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            scaleY, 0xFEFFFF, 0, D_00435D80));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            scaleY, 0xFEFFFF, 0xE, "%3.1f %3.1f %3.1f",
            transform->scale[0], transform->scale[1], transform->scale[2]));
        lines = 8;
    }
    return lines;
}

extern const char D_00412778[]; /* "FLAG  : 0x%08X" */

u32 DispObjectBaseData(void *worldNode, s32 x, s32 y, void *list) {
    dds3ObjectBaseControlWorkData_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(worldNode);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_00412778,
                                           handle->flags);
    sdfDrawTagLink(list, (u32)command);
    return 1;
}


#include "dds3_world_primary.h"
extern s32 dds3SceneBasicExistBasic(dds3Basic_t *object, dds3Basic_t *node);
#include "sdf_packet_alloc.h"
extern SdfPoolNode D_00380708;
extern s8 D_0037F543[];
extern s32 evtSceneDispProcess(void *task);

/* Draw the task's status panel; the returned value is the follow-up handler (or -1 / 0). */
s32 dds3DebugBasic(void *task) {
    s32 width;
    u8 *node;
    sdfDrawTag_t *list;
    sdfDrawTag_t *spriteList;

    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    node = dds3GetProcessData(task);
    if (dds3SceneBasicExistBasic(dds3GetActiveScene(), (dds3Basic_t *)node) == 0) {
        return (s32)evtSceneDispProcess;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    width = DispBaseData(node, 3, 0xA, list) + 0xB;
    if (node[0xF] < 0xA) {
        if (node[0xF] >= 4) {
            width += DispObjectBaseData(node, 3, width, list);
        }
    }
    spriteList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(spriteList);
    dds3ConsPanelTagLink(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00380708.append(&D_00380708, spriteList);
    D_00380708.append(&D_00380708, list);
    if (D_0037F543[0] < 0) {
        return (s32)evtSceneDispProcess;
    }
    return 0;
}

extern EffWorldOps *D_003849D8[EFF_WORLD_KIND_COUNT];
void dds3DeleteBasic(dds3Basic_t *node);

/* Allocate a node of `kind`, link its owner and run the owner's create hook. */
dds3Basic_t *dds3AllocBasic(u32 kind) {
    dds3Basic_t *node;
    EffWorldOps *ops;

    if (kind >= EFF_WORLD_KIND_COUNT) {
        return NULL;
    }
    node = sdfChipAllocDirect(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->kindTag = kind << 24;
    node->color = 0x80808080;
    node->word0 = 0;
    node->key = 0;
    node->value = 0;
    ops = D_003849D8[kind];
    node->ops = ops;
    node->word14 = 0;
    node->data = NULL;
    node->inner = NULL;
    node->next = NULL;
    node->previous = NULL;
    node->word28 = 0;
    node->word2C = 0;
    node->owner = NULL;
    if (ops != NULL && ops->create != NULL) {
        if (ops->create(node) != 1) {
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

s32 dds3InitPoseSet(dds3Basic_t *node) {
    dds3PoseSet_t *inner;

    if (node == NULL) {
        return 0;
    }
    if (node->inner != NULL) {
        return 0;
    }
    inner = sdfChipAllocDirect(sizeof(dds3PoseSet_t));
    if (inner == NULL) {
        return 0;
    }
    inner->flags = OBJECT_TRANSFORM_FLAG_UPDATE_PENDING;
    dds3BackupPoseSet(inner);
    VU0_STORE_VF(vf0, &inner->unkB0);
    node->inner = inner;
    inner->unkC8 = 0;
    inner->radius = 0.0f;
    return 1;
}

void dds3FreePoseSet(dds3Basic_t *node) {
    dds3PoseSet_t *inner;

    if (node != 0) {
        inner = node->inner;
        if (inner != 0) {
            sdfChipFreeDirect(inner);
            node->inner = 0;
        }
    }
}

void dds3OnPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    node->flags = node->flags | flags;
}

void dds3OffPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    node->flags = node->flags & ~flags;
}

u8 dds3CheckPoseSetFlag(dds3PoseSet_t *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void dds3BackupPoseSet(dds3PoseSet_t *node) {
    u8 *p40 = (u8 *)&node->position;
    u8 *p50;
    u8 *p60;

    VU0_STORE_VF(vf0, p40);
    p50 = (u8 *)&node->rotation;
    VU0_STORE_VF(vf0, p50);
    VU0_SET_ONES_XYZ(vf10);
    p60 = (u8 *)&node->scale;
    VU0_STORE_VF(vf10, p60);
}

void dds3SetRadiusPoseSet(dds3PoseSet_t *node) {
    PCP_COPY_VECTOR(&node->savedScale, &node->scale);
    PCP_COPY_VECTOR(&node->savedRotation, &node->rotation);
    PCP_COPY_VECTOR(&node->savedPosition, &node->position);
}

void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value) {
    node->inner->radius = value;
}

f32 effObjGetInnerFloat(dds3Basic_t *node) {
    return node->inner->radius;
}

void dds3SetPosPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    PCP_COPY_VECTOR(&inner->position, vector);
}

void dds3SetRotPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    PCP_COPY_VECTOR(&inner->rotation, vector);
}

void dds3SetSclPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    PCP_COPY_VECTOR(&inner->scale, vector);
}

void dds3GetPosPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->position;

    VU0_LOAD_VF_MEMORY(vf10, p);
    VU0_SET_W_ONE(vf10);
}

void dds3GetRotPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->rotation;

    VU0_LOAD_VF_MEMORY(vf10, p);
    Normalize();
}

void effObjFetchInnerScale(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->scale;

    VU0_LOAD_VF_MEMORY(vf10, p);
}

void dds3GetPosPoseSetMCR(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    VU0_LOAD_VF($vf10, &inner->position);
    VU0_LOAD_VF($vf11, vector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->position);
}

void dds3AddRotPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    VU0_LOAD_VF($vf10, &inner->rotation);
    VU0_LOAD_VF($vf11, vector);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, &inner->rotation);
}

void dds3GetSclPoseSetMCR(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    VU0_LOAD_VF($vf10, &inner->scale);
    VU0_LOAD_VF($vf11, vector);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->scale);
}

INCLUDE_RODATA(const s32, "game/code_0010F118", D_00412778);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D70);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D78);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D80);

