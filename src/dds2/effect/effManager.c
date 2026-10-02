#include "common.h"

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
    s32 type;
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
    func_00164CB0();
    func_00158340();
    func_00158C00();
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
    EffNode *node = (EffNode *)func_00328D68(0x10);

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

extern void func_0035B6E0(const char *fmt, ...);
extern void *sdfReadNamedResource(void *, u32 *, s32);
extern void *func_001578C0(u32);
extern void *func_003297C8(void *);
INCLUDE_ASM(const s32, "effect/effManager", func_001578C0);

void func_001579C8(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

void *effLoadResourceNode(void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *node;

    func_0035B6E0("d3p file read...[%s]\n", resource);
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    node = func_001578C0(resolvedId);
    func_003297C8(resourceHandle);
    return node;
}
