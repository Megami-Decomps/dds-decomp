#include "common.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_node_descriptor.h"
#include "eff_node.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "eff_manager_task.h"

extern SdfPoolNode kwlnDrawSurfaces[];
#include "sdf_packet_alloc.h"
extern void effObjectListCountersReset(void);


void effManagerInitializeSubsystems(KwlnTask *task) {
    (void)task;
    effInitializeEmitterSubsystem();
    parInitializeParticleSubsystem();
    effBillDispatchAll();
}

/* Submit the test and alpha state packets to both field draw buffers. */
s32 effSubmitFieldBlendStateTask(KwlnTask *task) {
    SdfListHead *list;
    SdfGsBlendPacket *state;
    SdfPoolNode *surface;

    (void)task;
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    state = (SdfGsBlendPacket *)sdfAllocPacketAligned(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = 0xE;
    state->registers.test.value = 0x51801;
    state->registers.test.registerId = 0x47;
    state->registers.alpha.value = 0x44;
    state->registers.alpha.registerId = 0x42;
    sdfAppendPacket(list, (u32)state);
    surface = &kwlnDrawSurfaces[51];
    surface->append(surface, list);

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    state = (SdfGsBlendPacket *)sdfAllocPacketAligned(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = 0xE;
    state->registers.test.value = 0x71801;
    state->registers.test.registerId = 0x47;
    state->registers.alpha.value = 0x42;
    state->registers.alpha.registerId = 0x42;
    sdfAppendPacket(list, (u32)state);
    surface++;
    surface->append(surface, list);

    effObjectListCountersReset();
    return 0;
}

s32 effManagerUpdateAndDispatch(KwlnTask *task) {
    (void)task;
    func_00163010();
    parDrawPendingCellSystems();
    billFlushPendingChildPackets();
    billFlushPendingRenderPairs();
    dds3UpdateCrossfade();
    effDispatchActive();
    return 0;
}

/* Parameter-node optional flag callback; the retail implementation is empty. */
void effParamNodeOptionalFlagNoOp(void) {
}

/* Parameter-node optional query always returns one. Keep its u32 surface. */
u32 effParamNodeOptionalQueryAlwaysTrue(void) {
    return 1;
}

/* Parameter-node restart callback; the retail implementation is empty. */
void effParamNodeRestartNoOp(void) {
}

/* Parameter-node condition callback always accepts the instance. */
u32 effParamNodeConditionAlwaysTrue(void) {
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

extern void sdfDebugPrintf(const char *fmt, ...);

typedef struct EffNodeInstance {
    u8 pad00[0x20];
    u8 matrix20[0x40]; /* 0x20 */
    u8 pad60[0x50];
    u8 matrixB0[0x40]; /* 0xB0 */
} EffNodeInstance;

/* Build the effect node for a resource descriptor; descriptors older than 1.03 are converted first, and ones up to 1.02 get an identity matrix. */
EffNode *effCreateNodeFromDescriptor(EffNodeDescriptor *descriptor) {
    EffNode *node;
    EffNodeInstance *instance;

    if (descriptor->version < 1.03f) {
        sdfDebugPrintf("old version!![%f]\n", descriptor->version);
        effConvertLegacyNodeDescriptor(descriptor);
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
        sdfDebugPrintf("effManager:set Identity matrix\n");
    }
    return node;
}

void effCreateNodeFromParameter(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

struct EffNode *effLoadResourceNode(const char *resource) {
    u32 resolvedId;
    struct SdfMemBlock *resourceHandle;
    struct EffNode *node;

    sdfDebugPrintf("d3p file read...[%s]\n", resource);
    resourceHandle = sdfReadNamedResource(resource, &resolvedId, 0);
    node = effCreateNodeFromDescriptor((EffNodeDescriptor *)resolvedId);
    sdfReleaseResourceAllocation(resourceHandle);
    return node;
}
