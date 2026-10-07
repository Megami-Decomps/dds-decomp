#ifndef EFF_DEPENDENCY_H
#define EFF_DEPENDENCY_H

#include "dds3obj.h"

struct BillConfig;

/* Complete 0x50-byte kind-7 payload, allocated and cleared by its constructor.
 * The dependency at 0x0C has a state-dependent owner: node, billboard or sound.
 * Bytes 0x30..0x3B remain opaque. */
typedef struct EffectDependencyState {
    ObjBase *objectHandle;
    u32 flags;
    s32 state;
    void *handle;
    u32 word10;
    u32 word14;
    void *word18;
    u8 pad1C[4];
    void *owner;
    u16 entryId;
    u16 ownerKind;
    void *vector;
    void *node;
    u8 pad30[0xC];
    struct BillConfig *config;
    f32 sourcePosition[4];
} EffectDependencyState;

typedef char EffectDependencyState_size_must_be_0x50[(sizeof(EffectDependencyState) == 0x50) ? 1 : -1];
typedef char EffectDependencyState_handle_at_0x0C[((u32)&((EffectDependencyState *)0)->handle == 0x0C) ? 1 : -1];
typedef char EffectDependencyState_owner_at_0x20[((u32)&((EffectDependencyState *)0)->owner == 0x20) ? 1 : -1];
typedef char EffectDependencyState_node_at_0x2C[((u32)&((EffectDependencyState *)0)->node == 0x2C) ? 1 : -1];

typedef char EffectDependencyState_config_at_0x3C[((u32)&((EffectDependencyState *)0)->config == 0x3C) ? 1 : -1];
typedef char EffectDependencyState_position_at_0x40[((u32)&((EffectDependencyState *)0)->sourcePosition == 0x40) ? 1 : -1];

void dds3ResetWorldResourceState(EffWorldNode *object);
void effUpdateConfiguredBillboard(EffWorldNode *object);

void effObjReleaseStateDependencies(EffectDependencyState *state);

ObjBase *effObjGetObjectHandle(EffWorldNode *object);

#endif
