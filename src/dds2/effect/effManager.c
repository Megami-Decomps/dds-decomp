#include "common.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_node_descriptor.h"


/* Per-effect-type operations act on the instance returned by create. */
typedef struct EffTypeOps {
    s32 (*create)(s32, s32); /* 0x00 */
    void (*update)(s32);     /* 0x04 */
    void (*destroy)(s32);    /* 0x08 */
    void (*fn0C)(s32);       /* 0x0C */
    s32 (*fn10)();           /* 0x10: returns 1 when absent */
    void (*fn14)(s32, const void *); /* 0x14: copy a supplied vector into the instance. */
    void (*fn18)(s32, const void *); /* 0x18: apply a supplied transform matrix. */
    void (*fn1C)(s32, u32);       /* 0x1C: opaque parameter word */
    void (*fn20)();          /* 0x20 */
    s32 (*fn24)();           /* 0x24: returns 1 when absent */
    void (*fn28)();          /* 0x28 */
    void (*fn2C)(s32);       /* 0x2C */
} EffTypeOps;

typedef struct EffNode {
    u32 type;
    s32 arg;
    s32 instance;
    f32 unkC;
} EffNode;

extern EffTypeOps effNodeTypeOperations[];

void effManagerInitializeSubsystems(void) {
    func_0015B290();
    func_00162E40();
    effBillDispatchAll();
}

INCLUDE_ASM(const s32, "effect/effManager", func_00157400);

u32 effManagerUpdateAndDispatch(void) {
    func_00163010();
    parDrawPendingCellSystems();
    billFlushPendingChildPackets();
    billFlushPendingRenderPairs();
    func_00167EE8();
    effDispatchActive();
    return 0;
}

void func_001575B0(void) {
}

u32 func_001575B8(void) {
    return 1;
}

void func_001575C0(void) {
}

u32 func_001575C8(void) {
    return 1;
}

EffNode *effCreateNode(u16 type, u16 arg, s32 param) {
    EffNode *node = (EffNode *)sdfAllocSizeClassBlock(0x10);

    node->type = type;
    node->unkC = 1.03f;
    node->arg = arg;
    node->instance = effNodeTypeOperations[type].create(arg, param);
    return node;
}

void effDestroyNode(EffNode *node) {
    effNodeTypeOperations[node->type].destroy(node->instance);
    sdfReleaseChipBlock(node);
}

void effUpdateNode(EffNode *node) {
    effNodeTypeOperations[node->type].update(node->instance);
}

void effRestartNodeInstance(EffNode *node) {
    effNodeTypeOperations[node->type].fn0C(node->instance);
}

void effApplyNodeScale(EffNode *node) {
    effNodeTypeOperations[node->type].fn2C(node->instance);
}

s32 effInvokeNodeConditionOrAcceptDefault(EffNode *node) {
    if (effNodeTypeOperations[node->type].fn10 == NULL) {
        return 1;
    }
    return effNodeTypeOperations[node->type].fn10(node->instance);
}

void effCopyVectorToNodeInstance(EffNode *node, const void *vector) {
    effNodeTypeOperations[node->type].fn14(node->instance, vector);
}

void effApplyNodeTransformMatrix(EffNode *node, const void *matrix) {
    effNodeTypeOperations[node->type].fn18(node->instance, matrix);
}

void effSetNodeParameterValue(EffNode *node, u32 value) {
    effNodeTypeOperations[node->type].fn1C(node->instance, value);
}

void effDispatchOptionalNodeFlag(EffNode *node, u8 flag) {
    if (effNodeTypeOperations[node->type].fn20 != NULL) {
        effNodeTypeOperations[node->type].fn20(node->instance, flag);
    }
}

s32 effInvokeOptionalNodeInstanceCallback(EffNode *node) {
    if (effNodeTypeOperations[node->type].fn24 == NULL) {
        return 1;
    }
    return effNodeTypeOperations[node->type].fn24(node->instance);
}

extern void func_0035B6E0(const char *fmt, ...);

typedef struct EffNodeInstance {
    u8 pad00[0x20];
    u8 matrix20[0x40]; /* 0x20 */
    u8 pad60[0x50];
    u8 matrixB0[0x40]; /* 0xB0 */
} EffNodeInstance;

extern void func_00157AC8(EffNodeDescriptor *descriptor);

/* Build the effect node for a resource descriptor; descriptors older than 1.03 are converted first, and ones up to 1.02 get an identity matrix. */
EffNode *effCreateNodeFromDescriptor(EffNodeDescriptor *descriptor) {
    EffNode *node;
    EffNodeInstance *instance;

    if (descriptor->version < 1.03f) {
        func_0035B6E0("old version!![%f]\n", descriptor->version);
        func_00157AC8(descriptor);
    }
    node = effCreateNode((u16)descriptor->type, (u16)descriptor->arg, (s32)descriptor->payload);
    if (descriptor->version <= 1.02f) {
        switch (node->type) {
        case 0:
        case 1:
            instance = (EffNodeInstance *)node->instance;
            EE_MMI_UNIT_MATRIX(instance->matrixB0);
            break;
        case 2:
            instance = (EffNodeInstance *)node->instance;
            EE_MMI_UNIT_MATRIX(instance->matrix20);
            break;
        }
        func_0035B6E0("effManager:set Identity matrix\n");
    }
    return node;
}

void func_001579C8(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

struct EffNode *effLoadResourceNode(const char *resource) {
    u32 resolvedId;
    struct SdfMemBlock *resourceHandle;
    struct EffNode *node;

    func_0035B6E0("d3p file read...[%s]\n", resource);
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    node = effCreateNodeFromDescriptor((EffNodeDescriptor *)resolvedId);
    sdfReleaseResourceAllocation(resourceHandle);
    return node;
}
