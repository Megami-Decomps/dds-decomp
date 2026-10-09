#include "common.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_node.h"
#include "eff_node_descriptor.h"


void effManagerInitializeSubsystems(void) {
    func_001536A0();
    func_0015B250();
    effBillDispatchAll();
}

INCLUDE_ASM(const s32, "effect/effManager", func_0014F860);

u32 effManagerUpdateAndDispatch(void) {
    func_0015B420();
    parDrawPendingCellSystems();
    billFlushPendingChildPackets();
    billFlushPendingRenderPairs();
    func_001602F8();
    effDispatchActive();
    return 0;
}

void func_0014FA10(void) {
}

u32 func_0014FA18(void) {
    return 1;
}

void func_0014FA20(void) {
}

u32 func_0014FA28(void) {
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
    effNodeTypeOperations[node->type].restart(node->instance);
}

void effApplyNodeScale(EffNode *node, f32 factor) {
    effNodeTypeOperations[node->type].applyScale(node->instance, factor);
}

s32 effInvokeNodeConditionOrAcceptDefault(EffNode *node) {
    if (effNodeTypeOperations[node->type].queryCondition == NULL) {
        return 1;
    }
    return effNodeTypeOperations[node->type].queryCondition(node->instance);
}

void effCopyVectorToNodeInstance(EffNode *node, const void *vector) {
    effNodeTypeOperations[node->type].setVector(node->instance, vector);
}

void effApplyNodeTransformMatrix(EffNode *node, const void *matrix) {
    effNodeTypeOperations[node->type].setMatrix(node->instance, matrix);
}

void effSetNodeParameterValue(EffNode *node, u32 value) {
    effNodeTypeOperations[node->type].setParameterWord(node->instance, value);
}

void effDispatchOptionalNodeFlag(EffNode *node, u8 flag) {
    if (effNodeTypeOperations[node->type].dispatchOptionalFlag != NULL) {
        effNodeTypeOperations[node->type].dispatchOptionalFlag(node->instance, flag);
    }
}

s32 effInvokeOptionalNodeInstanceCallback(EffNode *node) {
    if (effNodeTypeOperations[node->type].queryOptional == NULL) {
        return 1;
    }
    return effNodeTypeOperations[node->type].queryOptional(node->instance);
}
extern void func_003003F0(const char *fmt, ...);

typedef struct EffNodeInstance {
    u8 pad00[0x20];
    u8 matrix20[0x40]; /* 0x20 */
    u8 pad60[0x50];
    u8 matrixB0[0x40]; /* 0xB0 */
} EffNodeInstance;

extern void func_0014FF28(EffNodeDescriptor *descriptor);

/* Build the effect node for a resource descriptor; descriptors older than 1.03 are converted first, and ones up to 1.02 get an identity matrix. */
EffNode *effCreateNodeFromDescriptor(EffNodeDescriptor *descriptor) {
    EffNode *node;
    EffNodeInstance *instance;

    if (descriptor->version < 1.03f) {
        func_003003F0("old version!![%f]\n", descriptor->version);
        func_0014FF28(descriptor);
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
        func_003003F0("effManager:set Identity matrix\n");
    }
    return node;
}

void func_0014FE28(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

struct EffNode *effLoadResourceNode(const char *resource) {
    u32 resolvedId;
    struct SdfMemBlock *resourceHandle;
    struct EffNode *node;

    func_003003F0("d3p file read...[%s]\n", resource);
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    node = effCreateNodeFromDescriptor((EffNodeDescriptor *)resolvedId);
    sdfReleaseResourceAllocation(resourceHandle);
    return node;
}
