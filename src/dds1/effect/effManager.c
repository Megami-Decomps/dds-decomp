#include "common.h"

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
    s32 type;
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

void func_0014FB38(EffNode *node) {
    effNodeTypeOperations[node->type].fn0C(node->instance);
}

void func_0014FB70(EffNode *node) {
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

void func_0014FC28(EffNode *node) {
    effNodeTypeOperations[node->type].fn18(node->instance);
}

void func_0014FC60(EffNode *node) {
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
extern void func_003003F0(const char *, void *);
extern void *func_002EB028(void *, u32 *, s32);
extern void *func_002D0918(void *);
extern void *func_0014FD20(u32);
INCLUDE_ASM(const s32, "effect/effManager", func_0014FD20);

void func_0014FE28(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

void *effLoadResourceNode(void *resource) {
    u32 resolvedId;
    void *resourceHandle;
    void *node;

    func_003003F0("d3p file read...[%s]\n", resource);
    resourceHandle = func_002EB028(resource, &resolvedId, 0);
    node = func_0014FD20(resolvedId);
    func_002D0918(resourceHandle);
    return node;
}
