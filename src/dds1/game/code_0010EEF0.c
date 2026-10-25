#include "common.h"
#include "sdf_vu0.h"
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

extern s8 ptyReadSignedRosterStatByte(s32);
extern s32 scrGetIntPara(s32);
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

extern char D_003BA9A0[];
extern char D_003BA9A8[];
extern char D_003BA9B0[];

/* Each transform section owns its row coordinate; see docs/idioms.md,
 * "Trace-justified diagnostic row coordinates". */
s32 DispBaseData(dds3Basic_t *node, s32 x, s32 y, sdfDrawTag_t *list) {
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
            positionY, 0xFEFFFF, 0, D_003BA9A0));
        valueX = ((x + 84) << 4) + 0x7000;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            positionY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f",
            transform->position[0], transform->position[1], transform->position[2]));
        y += 2;
        rotationY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            rotationY, 0xFEFFFF, 0, D_003BA9A8));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            rotationY, 0xFEFFFF, 0xE, "%3.2f %3.2f %3.2f %3.2f",
            transform->rotation[0], transform->rotation[1],
            transform->rotation[2], transform->rotation[3]));
        y += 2;
        scaleY = (y * 6 << 4) + 0x7900;
        sdfDrawTagLink(list, (u32)sdfDevPrintf(labelX,
            scaleY, 0xFEFFFF, 0, D_003BA9B0));
        sdfDrawTagLink(list, (u32)sdfDevPrintf(valueX,
            scaleY, 0xFEFFFF, 0xE, "%3.1f %3.1f %3.1f",
            transform->scale[0], transform->scale[1], transform->scale[2]));
        lines = 8;
    }
    return lines;
}

extern const char D_0039F5F8[]; /* "FLAG  : 0x%08X" */

u32 DispObjectBaseData(dds3Basic_t *worldNode, s32 x, s32 y, void *list) {
    dds3ObjectBaseControlWorkData_t *handle;
    void *command;

    handle = dds3GetBaseControlWorkData(worldNode);
    command = sdfDevPrintf((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_0039F5F8,
                                           handle->flags);
    sdfDrawTagLink(list, (u32)command);
    return 1;
}


#include "dds3_world_primary.h"
#include "dds3_scene_membership.h"
#include "sdf_packet_alloc.h"
extern SdfPoolNode D_00325708;
extern s8 D_00324543[];
extern s32 evtSceneDispProcess(void *task);

/* Draw the task's status panel; the returned value is the follow-up handler (or -1 / 0). */
s32 dds3DebugBasic(dds3ProcessID_t *task) {
    s32 width;
    dds3Basic_t *node;
    sdfDrawTag_t *list;
    sdfDrawTag_t *spriteList;

    if (dds3GetActiveScene() == NULL) {
        return -1;
    }
    node = (dds3Basic_t *)dds3GetProcessData(task);
    if (dds3SceneBasicExistBasic(dds3GetActiveScene(), node) == DDS3_SCENE_MEMBERSHIP_ABSENT) {
        return (s32)evtSceneDispProcess;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    width = DispBaseData(node, 3, 0xA, list) + 0xB;
    if ((node->kindTag >> 24) < 0xA) {
        if ((node->kindTag >> 24) >= 4) {
            width += DispObjectBaseData(node, 3, width, list);
        }
    }
    spriteList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(spriteList);
    dds3ConsPanelTagLink(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00325708.append(&D_00325708, spriteList);
    D_00325708.append(&D_00325708, list);
    if (D_00324543[0] < 0) {
        return (s32)evtSceneDispProcess;
    }
    return 0;
}

extern EffWorldOps *D_003299C0[EFF_WORLD_KIND_COUNT];
void dds3DeleteBasic(dds3Basic_t *node);

/* Allocate a node of `kind`, link its owner and run the owner's create hook. */
dds3Basic_t *dds3AllocBasic(u32 kind) {
    dds3Basic_t *node;
    EffWorldOps *ops;

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
    node->value = 0;
    ops = D_003299C0[kind];
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

u8 dds3InitPoseSet(dds3Basic_t *node) {
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

    if (node != NULL) {
        inner = node->inner;
        if (inner != NULL) {
            sdfChipFreeDirect(inner);
            node->inner = NULL;
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
    u8 *p40 = (u8 *)&node->position;
    u8 *p50;
    u8 *p60;

    VU0_STORE_VF(vf0, p40);
    p50 = (u8 *)&node->rotation;
    VU0_STORE_VF(vf0, p50);
    SDF_VU0_SETSCALE1VECTOR();
    p60 = (u8 *)&node->scale;
    SDF_VU0_STOREVECTOR(p60);
}

void dds3SetRadiusPoseSet(dds3PoseSet_t *node) {
    SDF_VU0_COPYVECTOR(&node->savedScale, &node->scale);
    SDF_VU0_COPYVECTOR(&node->savedRotation, &node->rotation);
    SDF_VU0_COPYVECTOR(&node->savedPosition, &node->position);
}

void dds3GetRadiusPoseSet(dds3Basic_t *node, f32 value) {
    node->inner->radius = value;
}

f32 effObjGetInnerFloat(dds3Basic_t *node) {
    return node->inner->radius;
}

void dds3SetPosPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;
    f32 *dst = inner->position;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3SetRotPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;
    f32 *dst = inner->rotation;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3SetSclPoseSet(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;
    f32 *dst = inner->scale;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_COPYVECTOR(dst, vector);
}

void dds3GetPosPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->position;

    SDF_VU0_LOADVECTOR_BARRIER(p);
    SDF_VU0_VECTOR_W_SET_ONE();
}

void dds3GetRotPoseSet(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->rotation;

    SDF_VU0_LOADVECTOR_BARRIER(p);
    Normalize();
}

void effObjFetchInnerScale(dds3Basic_t *node) {
    u8 *p = (u8 *)&node->inner->scale;

    SDF_VU0_LOADVECTOR_BARRIER(p);
}

void dds3GetPosPoseSetMCR(dds3Basic_t *node, u128 *vector) {
    dds3PoseSet_t *inner = node->inner;
    u8 *src = (u8 *)&inner->position;
    u8 *dst;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(vector);
    SDF_VU0_ADDVECTOR();
    VU0_STORE_VF10_BASE_OFF(dst, inner, 0x40);
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
    u8 *src = (u8 *)&inner->scale;
    u8 *dst;

    inner->flags = (inner->flags | OBJECT_TRANSFORM_FLAG_UPDATE_PENDING) &
        ~OBJECT_TRANSFORM_FLAG_MATRIX_CACHE_VALID;
    SDF_VU0_LOADVECTOR(src);
    SDF_VU0_LOADVECTOR_B_BARRIER(vector);
    SDF_VU0_MULVECTOR();
    VU0_STORE_VF10_BASE_OFF(dst, inner, 0x60);
}

INCLUDE_RODATA(const s32, "game/code_0010EEF0", D_0039F5F8);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A0);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A8);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9B0);
