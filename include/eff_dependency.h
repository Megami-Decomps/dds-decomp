#ifndef EFF_DEPENDENCY_H
#define EFF_DEPENDENCY_H

#include "dds3obj.h"

struct BillConfig;

/* Effect-object dependency states drive handle update and release policy. */
typedef enum EffectDependencyStateKind {
    EFF_DEPENDENCY_STATE_EMPTY = 0,
    EFF_DEPENDENCY_STATE_BOUND_EFFECT_NODE = 1,
    EFF_DEPENDENCY_STATE_BILLBOARD_ANIMATION = 2,
    EFF_DEPENDENCY_STATE_BILLBOARD_NODE = 3,
    EFF_DEPENDENCY_STATE_BORROWED_HANDLE = 4,
    EFF_DEPENDENCY_STATE_EVENT_CUT = 5,
    EFF_DEPENDENCY_STATE_PARAMETERS_READY = 6,
    EFF_DEPENDENCY_STATE_MAGATUHI_TWO_ROWS = 7,
    EFF_DEPENDENCY_STATE_MAGATUHI_FOUR_ROWS = 8
} EffectDependencyStateKind;

/* Complete 0x50-byte kind-7 payload, allocated and cleared by its constructor.
 * The dependency at 0x0C has a state-dependent owner: node, billboard or sound.
 * Bytes 0x30..0x3B remain opaque. */
typedef struct EffectDependencyState {
    dds3ObjectBaseControlWorkData_t *objectHandle;
    u32 flags;
    EffectDependencyStateKind state;
    void *handle;
    f32 ownerLocalOffset[3];
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
typedef char EffectDependencyState_ownerLocalOffset_at_0x10[
    ((u32)&((EffectDependencyState *)0)->ownerLocalOffset == 0x10) ? 1 : -1];
typedef char EffectDependencyState_pad1C_at_0x1C[((u32)&((EffectDependencyState *)0)->pad1C == 0x1C) ? 1 : -1];
typedef char EffectDependencyState_owner_at_0x20[((u32)&((EffectDependencyState *)0)->owner == 0x20) ? 1 : -1];
typedef char EffectDependencyState_node_at_0x2C[((u32)&((EffectDependencyState *)0)->node == 0x2C) ? 1 : -1];

typedef char EffectDependencyState_config_at_0x3C[((u32)&((EffectDependencyState *)0)->config == 0x3C) ? 1 : -1];
typedef char EffectDependencyState_position_at_0x40[((u32)&((EffectDependencyState *)0)->sourcePosition == 0x40) ? 1 : -1];

void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *object);
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *object);

void FreeEffectData(EffectDependencyState *state);

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *object);

#endif
