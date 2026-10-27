#include "common.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "ee_mmi.h"
#include "eff.h"
#include "eff_node_descriptor.h"
#include "eff_node.h"
#include "eff_create_node_from_parameter.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "eff_manager_task.h"
#include "kwln_task_runner.h"
#include "eff_legacy_node_instance.h"

extern SdfPoolNode sdfGrOtTable[];
#include "sdf_packet_alloc.h"
extern void effObjectListCountersReset(void);


extern void effBillDispatchAll(void);
extern void dds3ParticleManagerExit(void);
extern void dds3PolygonManagerExit(void);

void effManagerInitializeSubsystems(dds3ProcessID_t *task) {
    (void)task;
    dds3ParticleManagerExit();
    dds3PolygonManagerExit();
    effBillDispatchAll();
}

/* Submit the test and alpha state packets to both field draw buffers. */
KwlnTaskUpdateResult effSubmitFieldBlendStateTask(dds3ProcessID_t *task) {
    sdfDrawTag_t *list;
    sdfPrimTransPacket_t *state;
    SdfPoolNode *surface;

    (void)task;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = 0xE;
    state->registers.test.value = 0x51801;
    state->registers.test.registerId = 0x47;
    state->registers.alpha.value = 0x44;
    state->registers.alpha.registerId = 0x42;
    sdfDrawTagLink(list, (u32)state);
    surface = &sdfGrOtTable[51];
    surface->append(surface, list);

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    state = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    state->header.dmaTag = 3;
    state->header.vifCommands = 0x5000000310000000ULL;
    state->header.gifTag = 0x1000000000008002ULL;
    state->header.gifRegisters = 0xE;
    state->registers.test.value = 0x71801;
    state->registers.test.registerId = 0x47;
    state->registers.alpha.value = 0x42;
    state->registers.alpha.registerId = 0x42;
    sdfDrawTagLink(list, (u32)state);
    surface++;
    surface->append(surface, list);

    effObjectListCountersReset();
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

extern void billFlushPendingChildPackets(void);
extern void billFlushPendingRenderPairs(void);
extern void dds3UpdateCrossfade(void);
extern void effDispatchActive(void);

KwlnTaskUpdateResult dds3EffectManagerRun(dds3ProcessID_t *task) {
    (void)task;
    dds3PolygonLinkOt();
    dds3TrackPolyLinkOt();
    billFlushPendingChildPackets();
    billFlushPendingRenderPairs();
    dds3UpdateCrossfade();
    effDispatchActive();
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Parameter-node optional flag callback; the retail implementation is empty. */
void dds3EffectPCFSetLoop(void) {
}

/* Parameter-node optional query always returns one. Keep its u32 surface. */
u32 dds3EffectPCFGetLoop(void) {
    return 1;
}

/* Parameter-node restart callback; the retail implementation is empty. */
void dds3EffectPCFReset(void) {
}

/* Parameter-node condition callback always accepts the instance. */
u32 dds3EffectPCFGetState(void) {
    return 1;
}

EffNode *dds3EffectCreate(u16 type, u16 arg, s32 param) {
    EffNode *node = (EffNode *)sdfChipAllocDirect(0x10);

    node->type = type;
    node->unkC = 1.03f;
    node->arg = arg;
    node->instanceWord = EffMajorTable[type].create(arg, param);
    return node;
}

void dds3EffectDelete(EffNode *node) {
    EffMajorTable[node->type].destroy(node->instanceWord);
    sdfChipFreeDirect(node);
}

void dds3EffectDispatchUpdate(EffNode *node) {
    EffMajorTable[node->type].update(node->instanceWord);
}

void dds3EffectReset(EffNode *node) {
    EffMajorTable[node->type].restart(node->instanceWord);
}

void dds3EffectSetScale(EffNode *node, f32 factor) {
    EffMajorTable[node->type].applyScale(node->instanceWord, factor);
}

EffNodeConditionResult effInvokeNodeConditionOrAcceptDefault(EffNode *node) {
    if (EffMajorTable[node->type].queryCondition == NULL) {
        return 1;
    }
    return EffMajorTable[node->type].queryCondition(node->instanceWord);
}

void effCopyVectorToNodeInstance(EffNode *node, const void *vector) {
    EffMajorTable[node->type].setVector(node->instanceWord, vector);
}

void effApplyNodeTransformMatrix(EffNode *node, const void *matrix) {
    EffMajorTable[node->type].setMatrix(node->instanceWord, matrix);
}

void effSetNodeParameterValue(EffNode *node, u32 parameterWord) {
    EffMajorTable[node->type].setParameterWord(node->instanceWord, parameterWord);
}

void effDispatchOptionalNodeFlag(EffNode *node, u8 flag) {
    if (EffMajorTable[node->type].dispatchOptionalFlag != NULL) {
        EffMajorTable[node->type].dispatchOptionalFlag(node->instanceWord, flag);
    }
}

s32 effInvokeOptionalNodeInstanceCallback(EffNode *node) {
    if (EffMajorTable[node->type].queryOptional == NULL) {
        return 1;
    }
    return EffMajorTable[node->type].queryOptional(node->instanceWord);
}

#include "sdf_debug_api.h"

/* Build the effect node for a resource descriptor; descriptors older than 1.03 are converted first, and ones up to 1.02 get an identity matrix. */
EffNode *dds3EffectLoad(EffNodeDescriptor *descriptor) {
    EffNode *node;
    EffLegacyNodeInstanceView *instance;

    if (descriptor->version < 1.03f) {
        printf("old version!![%f]\n", descriptor->version);
        dds3EffectDataRevision(descriptor);
    }
    node = dds3EffectCreate((u16)descriptor->type, (u16)descriptor->arg, (s32)descriptor->payload);
    if (descriptor->version <= 1.02f) {
        switch (node->type) {
        case 0:
        case 1:
            instance = (EffLegacyNodeInstanceView *)node->instanceWord;
            EE_MMI_UNIT_MATRIX(&instance->matrixAtB0);
            break;
        case 2:
            instance = (EffLegacyNodeInstanceView *)node->instanceWord;
            EE_MMI_UNIT_MATRIX(&instance->matrixAt20);
            break;
        }
        printf("effManager:set Identity matrix\n");
    }
    return node;
}

EffNode *effCreateNodeFromParameter(u32 parameter) {
    return dds3EffectCreate(5, 0, parameter);
}

struct EffNode *dds3EffectFileLoad(const char *resource) {
    union {
        u32 addressWord;
        EffNodeDescriptor *descriptor;
    } loadedResource;
    struct sdfMemHandle_t *resourceHandle;
    struct EffNode *node;

    printf("d3p file read...[%s]\n", resource);
    resourceHandle = sdfDevLoadFileEx(resource, &loadedResource.addressWord, 0);
    node = dds3EffectLoad(loadedResource.descriptor);
    sdfMemFreeDirect(resourceHandle);
    return node;
}
