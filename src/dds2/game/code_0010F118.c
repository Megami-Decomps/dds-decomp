#include "common.h"
#include "pcp_vu0.h"
#include "eff_transform.h"
#include "dds3obj.h"
#include "sdf.h"

extern s32 scrReadIntParameter(s32);

extern s8 ptyReadSignedRosterStatByte(s32);
extern void scrSetIntegerReturnValue(s32);
extern void func_0011A328(s32);

extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *p);


extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);
void effObjInnerVecInit(ObjectTransform *node);

u32 func_0010F118(void) {
    s32 mode;

    mode = scrReadIntParameter(0);
    func_0011A328(mode);
    return 1;
}

u32 ptyScriptRestoreEntireParty(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 func_0010F160(void) {
    s32 value;

    value = scrReadIntParameter(0);
    value = ptyReadSignedRosterStatByte(value);
    scrSetIntegerReturnValue(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F190);

extern void *sdfCreateFormattedSifCommand(s32 x, s32 y, s32 flags, s32 mode, const char *format, ...);
extern void sdfAppendPacket(void *list, void *packet);
extern const char D_00412778[]; /* "FLAG  : 0x%08X" */

u32 dds3DrawObjectFlagDiagnostic(void *object, s32 x, s32 y, void *list) {
    ObjBase *handle;
    void *command;

    handle = dds3GetObjectOwnedHandle(object);
    command = sdfCreateFormattedSifCommand((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_00412778,
                                           handle->flags);
    sdfAppendPacket(list, command);
    return 1;
}


extern void *dds3GetWorldObject(void);
extern void *kwlnTaskGetUserValue();
extern s32 dds3ContainsNodeInAnyObjectChain(EffWorldNode *object, EffWorldNode *node);
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList();
extern s32 func_0010F190();
extern void kwlnDrawSpriteCell();
extern SdfPoolNode D_00380708;
extern s8 D_0037F543[];
extern void func_002458B8();

/* Draw the task's status panel; the returned value is the follow-up handler (or -1 / 0). */
s32 dds3DrawWorldNodeDiagnosticTask(void *task) {
    s32 width;
    u8 *node;
    SdfListHead *list;
    SdfListHead *spriteList;

    if (dds3GetWorldObject() == NULL) {
        return -1;
    }
    node = kwlnTaskGetUserValue(task);
    if (dds3ContainsNodeInAnyObjectChain(dds3GetWorldObject(), (EffWorldNode *)node) == 0) {
        return (s32)func_002458B8;
    }
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    width = func_0010F190(node, 3, 0xA, list) + 0xB;
    if (node[0xF] < 0xA) {
        if (node[0xF] >= 4) {
            width += dds3DrawObjectFlagDiagnostic(node, 3, width, list);
        }
    }
    spriteList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(spriteList);
    kwlnDrawSpriteCell(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00380708.append((SdfListHead *)&D_00380708, spriteList);
    D_00380708.append((SdfListHead *)&D_00380708, list);
    if (D_0037F543[0] < 0) {
        return (s32)func_002458B8;
    }
    return 0;
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern EffWorldOps *D_003849D8[EFF_WORLD_KIND_COUNT];
void effObjNodeDestroy(EffWorldNode *node);

/* Allocate a node of `kind`, link its owner and run the owner's create hook. */
EffWorldNode *dds3CreateWorldNodeForKind(u32 kind) {
    EffWorldNode *node;
    EffWorldOps *ops;

    if (kind >= EFF_WORLD_KIND_COUNT) {
        return NULL;
    }
    node = sdfAllocSizeClassBlock(0x44);
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
            effObjNodeDestroy(node);
            return NULL;
        }
    }
    return node;
}

/* Notify the owner before unlinking and freeing this transform node. */
void effObjNodeDestroy(EffWorldNode *node) {
    EffWorldOps *owner;
    EffWorldNode *next;
    EffWorldNode *prev;

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
        sdfReleaseChipBlock(node);
    }
}

s32 effObjInnerCreate(EffWorldNode *node) {
    ObjectTransform *inner;

    if (node == NULL) {
        return 0;
    }
    if (node->inner != NULL) {
        return 0;
    }
    inner = sdfAllocSizeClassBlock(sizeof(ObjectTransform));
    if (inner == NULL) {
        return 0;
    }
    inner->flags = 1;
    effObjInnerVecInit(inner);
    VU0_STORE_VF(vf0, &inner->unkB0);
    node->inner = inner;
    inner->unkC8 = 0;
    inner->radius = 0.0f;
    return 1;
}

void effObjFreeInner(EffWorldNode *node) {
    ObjectTransform *inner;

    if (node != 0) {
        inner = node->inner;
        if (inner != 0) {
            sdfReleaseChipBlock(inner);
            node->inner = 0;
        }
    }
}

void effObjSetNodeFlags(ObjectTransform *node, u32 flags) {
    node->flags = node->flags | flags;
}

void effObjClearNodeFlags(ObjectTransform *node, u32 flags) {
    node->flags = node->flags & ~flags;
}

u8 effObjTestNodeFlags(ObjectTransform *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void effObjInnerVecInit(ObjectTransform *node) {
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

void effObjInnerVecBackup(ObjectTransform *node) {
    PCP_COPY_VECTOR(&node->savedScale, &node->scale);
    PCP_COPY_VECTOR(&node->savedRotation, &node->rotation);
    PCP_COPY_VECTOR(&node->savedPosition, &node->position);
}

void effObjSetInnerFloat(EffWorldNode *node, f32 value) {
    node->inner->radius = value;
}

f32 effObjGetInnerFloat(EffWorldNode *node) {
    return node->inner->radius;
}

void effObjSetInnerFirstVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->position, vector);
}

void effObjSetInnerSecondVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->rotation, vector);
}

void effObjSetInnerThirdVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->scale, vector);
}

void effObjFetchInnerFirstVec(EffWorldNode *node) {
    u8 *p = (u8 *)&node->inner->position;

    VU0_LOAD_VF_MEMORY(vf10, p);
    VU0_SET_W_ONE(vf10);
}

void effObjFetchInnerSecondVecNorm(EffWorldNode *node) {
    u8 *p = (u8 *)&node->inner->rotation;

    VU0_LOAD_VF_MEMORY(vf10, p);
    effMiscNormalizeVU();
}

void effObjFetchInnerThirdVec(EffWorldNode *node) {
    u8 *p = (u8 *)&node->inner->scale;

    VU0_LOAD_VF_MEMORY(vf10, p);
}

void effObjAddInnerFirstVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->position);
    VU0_LOAD_VF($vf11, vector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->position);
}

void effObjQuatMulInnerSecondVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->rotation);
    VU0_LOAD_VF($vf11, vector);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, &inner->rotation);
}

void effObjMulInnerThirdVec(EffWorldNode *node, u128 *vector) {
    ObjectTransform *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->scale);
    VU0_LOAD_VF($vf11, vector);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->scale);
}

INCLUDE_RODATA(const s32, "game/code_0010F118", D_00412778);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D70);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D78);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D80);

