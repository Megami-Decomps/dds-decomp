#include "common.h"
#include "pcp_vu0.h"
#include "eff_transform.h"
#include "dds3obj.h"

extern s8 ptyReadSignedRosterStatByte(s32);
extern s32 scrReadIntParameter(s32);
extern void scrSetIntegerReturnValue(s32);
extern void func_00119B08(s32);

extern void *sdfAllocSizeClassBlock(s32 size);
extern void sdfReleaseChipBlock(void *p);
extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);



void effObjInnerVecInit(EffTransformNode *node);


u32 func_0010EEF0(void) {
    s32 mode;

    mode = scrReadIntParameter(0);
    func_00119B08(mode);
    return 1;
}

u32 ptyScriptRestoreEntireParty(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 func_0010EF38(void) {
    s32 value;

    value = scrReadIntParameter(0);
    value = ptyReadSignedRosterStatByte(value);
    scrSetIntegerReturnValue(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010EEF0", func_0010EF68);

extern void *sdfCreateFormattedSifCommand(s32 x, s32 y, s32 flags, s32 mode, const char *format, ...);
extern void sdfAppendPacket(void *list, void *packet);
extern const char D_0039F5F8[]; /* "FLAG  : 0x%08X" */

u32 dds3DrawObjectFlagDiagnostic(void *object, s32 x, s32 y, void *list) {
    ObjBase *handle;
    void *command;

    handle = dds3GetObjectOwnedHandle(object);
    command = sdfCreateFormattedSifCommand((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_0039F5F8,
                                           handle->flags);
    sdfAppendPacket(list, command);
    return 1;
}

typedef struct DrawOps {
    u8 pad00[0x10];
    void (*draw)(struct DrawOps *self, void *list); /* 0x10 */
} DrawOps;

extern void *dds3GetWorldObject(void);
extern void *kwlnTaskGetUserValue();
extern s32 dds3ContainsNodeInAnyObjectChain();
extern void *sdfAllocPacketAligned();
extern void sdfInitPacketList();
extern s32 func_0010EF68();
extern void kwlnDrawSpriteCell();
extern DrawOps D_00325708;
extern s8 D_00324543[];
extern void func_0022AF50();

/* Draw the task's status panel; the returned value is the follow-up handler (or -1 / 0). */
s32 dds3DrawWorldNodeDiagnosticTask(void *task) {
    s32 width;
    u8 *node;
    void *list;
    void *spriteList;

    if (dds3GetWorldObject() == NULL) {
        return -1;
    }
    node = kwlnTaskGetUserValue(task);
    if (dds3ContainsNodeInAnyObjectChain(dds3GetWorldObject(), node) == 0) {
        return (s32)func_0022AF50;
    }
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    width = func_0010EF68(node, 3, 0xA, list) + 0xB;
    if (node[0xF] < 0xA) {
        if (node[0xF] >= 4) {
            width += dds3DrawObjectFlagDiagnostic(node, 3, width, list);
        }
    }
    spriteList = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(spriteList);
    kwlnDrawSpriteCell(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00325708.draw(&D_00325708, spriteList);
    D_00325708.draw(&D_00325708, list);
    if (D_00324543[0] < 0) {
        return (s32)func_0022AF50;
    }
    return 0;
}

extern EffTransformOwner *D_003299C0[];
void effObjNodeDestroy(EffTransformNode *node);

/* Allocate a node of `kind`, link its owner and run the owner's create hook. */
EffTransformNode *dds3CreateWorldNodeForKind(u32 kind) {
    EffTransformNode *node;
    EffTransformOwner *owner;

    if (kind >= 0x12) {
        return NULL;
    }
    node = sdfAllocSizeClassBlock(0x44);
    if (node == NULL) {
        return NULL;
    }
    node->kindTag = kind << 24;
    node->color = 0x80808080;
    node->word0 = 0;
    node->word4 = 0;
    node->word8 = 0;
    owner = D_003299C0[kind];
    node->owner = owner;
    node->word14 = 0;
    node->ownerData = 0;
    node->inner = NULL;
    node->prev = NULL;
    node->next = NULL;
    node->word28 = 0;
    node->word2C = 0;
    node->word30 = 0;
    if (owner != NULL && owner->create != NULL) {
        if (owner->create(node) != 1) {
            effObjNodeDestroy(node);
            return NULL;
        }
    }
    return node;
}

/* Notify the owner before unlinking and freeing this transform node. */
void effObjNodeDestroy(EffTransformNode *node) {
    EffTransformOwner *owner;
    EffTransformNode *next;
    EffTransformNode *prev;

    if (node != NULL) {
        owner = node->owner;
        if (owner != NULL) {
            if (owner->notify != NULL) {
                owner->notify(node);
            }
        }
        next = node->next;
        if (next != NULL) {
            next->prev = node->prev;
        }
        prev = node->prev;
        if (prev != NULL) {
            prev->next = node->next;
        }
        sdfReleaseChipBlock(node);
    }
}

s32 effObjInnerCreate(EffTransformNode *node) {
    EffTransformNode *inner;

    if (node == NULL) {
        return 0;
    }
    if (node->inner != NULL) {
        return 0;
    }
    inner = sdfAllocSizeClassBlock(sizeof(EffTransformNode));
    if (inner == NULL) {
        return 0;
    }
    inner->flags = 1;
    effObjInnerVecInit(inner);
    VU0_STORE_VF(vf0, &inner->vecB0);
    node->inner = inner;
    inner->unkC8 = 0;
    inner->scalar = 0.0f;
    return 1;
}

void effObjFreeInner(EffTransformNode *node) {
    EffTransformNode *inner;

    if (node != NULL) {
        inner = node->inner;
        if (inner != NULL) {
            sdfReleaseChipBlock(inner);
            node->inner = NULL;
        }
    }
}

void effObjSetNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags |= flags;
}

void effObjClearNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags &= ~flags;
}

u8 effObjTestNodeFlags(EffTransformNode *node, u32 flags) {
    return (node->flags & flags) != 0;
}

void effObjInnerVecInit(EffTransformNode *node) {
    u8 *p40 = (u8 *)&node->vec40;
    u8 *p50;
    u8 *p60;

    VU0_STORE_VF(vf0, p40);
    p50 = (u8 *)&node->vec50;
    VU0_STORE_VF(vf0, p50);
    VU0_SET_ONES_XYZ(vf10);
    p60 = (u8 *)&node->vec60;
    VU0_STORE_VF(vf10, p60);
}

void effObjInnerVecBackup(EffTransformNode *node) {
    PCP_COPY_VECTOR(&node->vecA0, &node->vec60);
    PCP_COPY_VECTOR(&node->vec90, &node->vec50);
    PCP_COPY_VECTOR(&node->vec80, &node->vec40);
}

void effObjSetInnerFloat(EffTransformNode *node, f32 value) {
    node->inner->scalar = value;
}

f32 effObjGetInnerFloat(EffTransformNode *node) {
    return node->inner->scalar;
}

void effObjSetInnerFirstVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;
    u128 *dst = &inner->vec40;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(dst, vector);
}

void effObjSetInnerSecondVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;
    u128 *dst = &inner->vec50;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(dst, vector);
}

void effObjSetInnerThirdVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;
    u128 *dst = &inner->vec60;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(dst, vector);
}

void effObjFetchInnerFirstVec(EffTransformNode *node) {
    u8 *p = (u8 *)&node->inner->vec40;

    VU0_LOAD_VF_MEMORY(vf10, p);
    VU0_SET_W_ONE(vf10);
}

void effObjFetchInnerSecondVecNorm(EffTransformNode *node) {
    u8 *p = (u8 *)&node->inner->vec50;

    VU0_LOAD_VF_MEMORY(vf10, p);
    effMiscNormalizeVU();
}

void effObjFetchInnerThirdVec(EffTransformNode *node) {
    u8 *p = (u8 *)&node->inner->vec60;

    VU0_LOAD_VF_MEMORY(vf10, p);
}

void effObjAddInnerFirstVec(EffTransformNode *node, void *vector) {
    EffTransformNode *inner = node->inner;
    u8 *src = (u8 *)&inner->vec40;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    VU0_LOAD_VF(vf10, src);
    VU0_LOAD_VF_MEMORY(vf11, vector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF10_BASE_OFF(dst, inner, 0x40);
}

void effObjQuatMulInnerSecondVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec50);
    VU0_LOAD_VF($vf11, vector);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, &inner->vec50);
}

void effObjMulInnerThirdVec(EffTransformNode *node, void *vector) {
    EffTransformNode *inner = node->inner;
    u8 *src = (u8 *)&inner->vec60;
    u8 *dst;

    inner->flags = (inner->flags | 1) & ~2;
    VU0_LOAD_VF(vf10, src);
    VU0_LOAD_VF_MEMORY(vf11, vector);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF10_BASE_OFF(dst, inner, 0x60);
}

INCLUDE_RODATA(const s32, "game/code_0010EEF0", D_0039F5F8);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A0);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9A8);

INCLUDE_SDATA(const s32, "game/code_0010EEF0", D_003BA9B0);

