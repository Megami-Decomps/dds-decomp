#include "common.h"
#include "pcp_vu0.h"

extern u64 scrReadIntParameter(u64);

extern u64 func_0011A318(u64);

extern void sdfReleaseChipBlock(void *p);

typedef struct EffTransformNode EffTransformNode;

typedef struct {
    s32 (*create)(EffTransformNode *);  /* 0x00 called to initialise a new node of this kind */
    void (*notify)(EffTransformNode *); /* 0x04 called with the node being destroyed */
} EffTransformOwner;

/* Transform node (0xD0 bytes). The links at 0x10/0x20/0x24 tie a node into
 * its owner's list; inner points at a child node whose vectors live in VU
 * registers between calls (loaded with lqc2, stored with sqc2).
 */
struct EffTransformNode {
    u32 word0;        /* 0x00 */
    u32 word4;        /* 0x04 */
    u32 word8;        /* 0x08 */
    u32 kindTag;      /* 0x0C: node kind in the top byte */
    EffTransformOwner *owner; /* 0x10 */
    u32 word14;       /* 0x14 */
    u32 ownerData;    /* 0x18: owner-specific payload; camera nodes use CameraData */
    EffTransformNode *inner;  /* 0x1C */
    EffTransformNode *prev;   /* 0x20 */
    EffTransformNode *next;   /* 0x24 */
    u32 word28;       /* 0x28 */
    u32 word2C;       /* 0x2C */
    u32 word30;       /* 0x30 */
    u32 color;        /* 0x34 */
    u8 pad38[0x8];    /* 0x38 */
    u128 vec40;       /* 0x40 */
    u128 vec50;       /* 0x50 */
    u128 vec60;       /* 0x60 */
    u8 pad70[0x10];   /* 0x70 */
    u128 vec80;       /* 0x80 copy of vec40 */
    u128 vec90;       /* 0x90 copy of vec50 */
    u128 vecA0;       /* 0xA0 copy of vec60 */
    u8 vecB0[0x10];   /* 0xB0 cleared on alloc */
    u32 flags;        /* 0xC0 bit0 set, bit1 cleared on vector write */
    f32 scalar;       /* 0xC4 */
    u32 unkC8;        /* 0xC8 */
};

extern void effMiscNormalizeVU(void);
extern void effMiscQuatMultiplyVU(void);

u32 func_0010F118(void) {
    u64 id;

    id = scrReadIntParameter(0);
    func_0011A328(id);
    return 1;
}

u32 ptyScriptRestoreEntireParty(void) {
    ptyRecoverAllUnits();
    return 1;
}

u32 func_0010F160(void) {
    u64 id;

    id = scrReadIntParameter(0);
    id = func_0011A318(id);
    scrSetIntegerReturnValue(id);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0010F118", func_0010F190);

extern s32 dds3GetObjectOwnedHandle(void *object);
extern void *sdfCreateFormattedSifCommand(s32 x, s32 y, s32 flags, s32 mode, const char *format, ...);
extern void sdfAppendPacket(void *list, void *packet);
extern const char D_00412778[]; /* "FLAG  : 0x%08X" */

u32 dds3DrawObjectFlagDiagnostic(void *object, s32 x, s32 y, void *list) {
    void *handle;
    void *command;

    handle = (void *)dds3GetObjectOwnedHandle(object);
    command = sdfCreateFormattedSifCommand((x * 3 << 6) + 0x7000,
                                           (y * 3 << 5) + 0x7900,
                                           0xFEFFFF,
                                           0,
                                           D_00412778,
                                           *(u32 *)handle);
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
extern s32 func_0010F190();
extern void kwlnDrawSpriteCell();
extern DrawOps D_00380708;
extern s8 D_0037F543[];
extern void func_002458B8();

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
        return (s32)func_002458B8;
    }
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    width = func_0010F190(node, 3, 0xA, list) + 0xB;
    if (node[0xF] < 0xA) {
        if (node[0xF] >= 4) {
            width += dds3DrawObjectFlagDiagnostic(node, 3, width, list);
        }
    }
    spriteList = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(spriteList);
    kwlnDrawSpriteCell(spriteList, 0x24, 0x72, 0x23, width - 9);
    D_00380708.draw(&D_00380708, spriteList);
    D_00380708.draw(&D_00380708, list);
    if (D_0037F543[0] < 0) {
        return (s32)func_002458B8;
    }
    return 0;
}

extern void *sdfAllocSizeClassBlock(s32 size);
extern EffTransformOwner *D_003849D8[];
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
    owner = D_003849D8[kind];
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

INCLUDE_ASM(const s32, "game/code_0010F118", effObjInnerCreate);

void effObjFreeInner(EffTransformNode *node) {
    EffTransformNode *inner;

    if (node != 0) {
        inner = node->inner;
        if (inner != 0) {
            sdfReleaseChipBlock(inner);
            node->inner = 0;
        }
    }
}

void effObjSetNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags = node->flags | flags;
}

void effObjClearNodeFlags(EffTransformNode *node, u32 flags) {
    node->flags = node->flags & ~flags;
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

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec40, vector);
}

void effObjSetInnerSecondVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec50, vector);
}

void effObjSetInnerThirdVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & ~2;
    PCP_COPY_VECTOR(&inner->vec60, vector);
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

void effObjAddInnerFirstVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec40);
    VU0_LOAD_VF($vf11, vector);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->vec40);
}

void effObjQuatMulInnerSecondVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec50);
    VU0_LOAD_VF($vf11, vector);
    effMiscQuatMultiplyVU();
    VU0_STORE_VF($vf10, &inner->vec50);
}

void effObjMulInnerThirdVec(EffTransformNode *node, u128 *vector) {
    EffTransformNode *inner = node->inner;

    inner->flags = (inner->flags | 1) & 0xFFFFFFFD;
    VU0_LOAD_VF($vf10, &inner->vec60);
    VU0_LOAD_VF($vf11, vector);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF($vf10, &inner->vec60);
}

INCLUDE_RODATA(const s32, "game/code_0010F118", D_00412778);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D70);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D78);

INCLUDE_SDATA(const s32, "game/code_0010F118", D_00435D80);

