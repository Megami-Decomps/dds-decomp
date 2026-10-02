#include "common.h"
#include "ee_mmi.h"

void effManagerInitializeSubsystems(void) {
    func_001536A0();
    func_0015B250();
    effBillDispatchAll();
}

INCLUDE_ASM(const s32, "effect/effManager", func_0014F860);

u32 effManagerUpdateAndDispatch(void) {
    func_0015B420();
    func_0015D0C0();
    func_00150750();
    func_00151010();
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

/* Per-effect-type operations act on the instance returned by create. */
typedef struct EffTypeOps {
    s32 (*create)(s32, s32); /* 0x00 */
    void (*update)(s32);     /* 0x04 */
    void (*destroy)(s32);    /* 0x08 */
    void (*fn0C)(s32);       /* 0x0C */
    s32 (*fn10)();           /* 0x10: returns 1 when absent */
    void (*fn14)();          /* 0x14 */
    void (*fn18)(s32);       /* 0x18 */
    void (*fn1C)(s32);       /* 0x1C */
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

EffNode *effCreateNode(u16 type, u16 arg, s32 param) {
    EffNode *node = (EffNode *)func_002CFEB8(0x10);

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

void effCopyVectorToNodeInstance(EffNode *node) {
    effNodeTypeOperations[node->type].fn14(node->instance);
}

void effApplyNodeTransformMatrix(EffNode *node) {
    effNodeTypeOperations[node->type].fn18(node->instance);
}

void effSetNodeParameterValue(EffNode *node) {
    effNodeTypeOperations[node->type].fn1C(node->instance);
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
extern void func_003003F0(const char *fmt, ...);
extern void *sdfReadNamedResource(void *, u32 *, s32);
extern void *sdfReleaseResourceAllocation(void *);

typedef struct EffNodeDescriptor {
    u16 type;      /* 0x00 */
    u8 pad02[2];
    u16 arg;       /* 0x04 */
    u8 pad06[6];
    f32 version;   /* 0x0C */
    u8 payload[1]; /* 0x10 */
} EffNodeDescriptor;

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
    node = effCreateNode(descriptor->type, descriptor->arg, (s32)descriptor->payload);
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

void *effLoadResourceNode(void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *node;

    func_003003F0("d3p file read...[%s]\n", resource);
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    node = effCreateNodeFromDescriptor(resolvedId);
    sdfReleaseResourceAllocation(resourceHandle);
    return node;
}
