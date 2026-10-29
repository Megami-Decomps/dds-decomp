#ifndef DDS_FLD_LMAP_TASK_H
#define DDS_FLD_LMAP_TASK_H

#include "common.h"
#include "sdf.h"

typedef struct MapRequestNode {
    s32 x;                          /* 0x00: queued map-world X coordinate */
    s32 y;                          /* 0x04: queued map-world Y coordinate */
    s32 z;                          /* 0x08: queued map-world Z coordinate */
    s32 ageTicks; /* 0x0C: zero is inactive; active nodes count age in update ticks. */
    struct MapRequestNode *next;
    struct MapRequestNode *prev;
    u8 pad18[8];
} MapRequestNode;

struct MapRequestState;

/* Dispatch borrows the live ring and node for the callback's duration. */
typedef void (*MapRequestDrawCallback)(s32 x, s32 y, s32 z,
                                       const struct MapRequestState *ring,
                                       const MapRequestNode *node, f32 progress);

/* The allocation owns the node storage; const-qualified callback pointers are
 * borrowed for dispatch and must not outlive it. */
typedef struct MapRequestState {
    struct sdfMemHandle_t *allocation; /* 0x00: descriptor for the ring and nodes. */
    MapRequestNode *ringFirstNode;
    MapRequestNode *nextEnqueueNode;
    MapRequestNode *oldestActiveNode;
    s16 count;
    s16 lifetimeTicks; /* 0x0E: age threshold and progress denominator for queued nodes. */
    s16 interval;
    s16 elapsed;
    MapRequestDrawCallback callback;
} MapRequestState;

/* Both constructors put the 0x20-byte nodes after a 0x44-byte queue prefix. */
typedef struct MapRequestRing {
    MapRequestState header;
    u8 pad1C[0x28];
    MapRequestNode nodes[1];
} MapRequestRing;

typedef char MapRequestNode_size_must_be_0x20[(sizeof(MapRequestNode) == 0x20) ? 1 : -1];
typedef char MapRequestState_size_must_be_0x1C[(sizeof(MapRequestState) == 0x1C) ? 1 : -1];
typedef char MapRequestState_allocation_offset_must_be_0[
    ((u32)&((MapRequestState *)0)->allocation == 0) ? 1 : -1];
typedef char MapRequestRing_nodes_offset_check[
    ((u32)&((MapRequestRing *)0)->nodes == 0x44) ? 1 : -1];

MapRequestState *sdfCreateLinkedRequestRing(s16 count, s16 arg);
void fldAdvanceMapRequest(MapRequestState *state, s32 x, s32 y, s32 z);
void fldSetMapRequestInterval(MapRequestState *, u16);
void fldDrawMapRequestHalo(s32 x, s32 y, s32 z, const MapRequestState *ring,
                           const MapRequestNode *node, f32 progress);
void fldDrawMapRequestPulse(s32 x, s32 y, s32 z, const MapRequestState *ring,
                            const MapRequestNode *node, f32 progress);
void fldDispatchMapRequestCallbacks(const MapRequestState *ring);

struct FileRequest;
struct EffectSlotSet;

/* Resource loading advances through request, poll, consume, and slot install. */
typedef s16 LmapResourceLoadPhase;
enum {
    LMAP_RESOURCE_LOAD_PHASE_INITIALIZE = 0,
    LMAP_RESOURCE_LOAD_PHASE_INSTALL_SLOTS = 1,
    LMAP_RESOURCE_LOAD_PHASE_REQUEST_ENTRY = 2,
    LMAP_RESOURCE_LOAD_PHASE_WAIT_FOR_ENTRY = 3,
    LMAP_RESOURCE_LOAD_PHASE_CONSUME_ENTRY = 4,
};

typedef s32 LmapResourceLoadResult;
enum {
    LMAP_RESOURCE_LOAD_IN_PROGRESS = 0,
    LMAP_RESOURCE_LOAD_COMPLETE = 1,
};

/* The file loader retains this subrecord at task + 0x78 between updates. */
typedef struct LmapLoadState {
    struct FileRequest *file;
    struct FileRequest *archive;
    LmapResourceLoadPhase phase;
    s16 entry;
    u32 unknown0C;
} LmapLoadState;

typedef char LmapLoadState_size_must_be_0x10[(sizeof(LmapLoadState) == 0x10) ? 1 : -1];

/* The main task loads resources, initializes the scene, then runs selection. */
typedef s32 LmapTaskPhase;
enum {
    LMAP_TASK_PHASE_LOADING_RESOURCES = 0,
    LMAP_TASK_PHASE_INITIALIZING_SCENE = 1,
    LMAP_TASK_PHASE_INTERACTIVE = 2,
};

/* Counter selection runs after scene setup and stages the exit message/fade. */
typedef u32 LmapCounterSelectionPhase;
enum {
    LMAP_COUNTER_SELECTION_PHASE_ACTIVE = 1,
    LMAP_COUNTER_SELECTION_PHASE_CAMERA_TRANSITION = 2,
    LMAP_COUNTER_SELECTION_PHASE_EXIT_MESSAGE = 3,
    LMAP_COUNTER_SELECTION_PHASE_EXIT_MESSAGE_INPUT = 4,
    LMAP_COUNTER_SELECTION_PHASE_EXIT_FADE = 5,
    LMAP_COUNTER_SELECTION_PHASE_COMMIT_EXIT = 6,
};

/* User data allocated by fldStartLmapTask and passed to the LmapMain task. */
typedef struct LmapTaskState {
    u32 unknown00;                /* 0x00 */
    LmapTaskPhase phase;          /* 0x04: main task lifecycle */
    s32 variant;                  /* 0x08: selected local-map variant */
    struct EffectSlotSet *spriteSlots[25]; /* 0x0C: retained local-map sprite owners */
    sdfMemHandle_t *allocation;      /* 0x70: loaded resource allocation */
    u32 resourceAddress;          /* 0x74: loaded resource address */
    LmapLoadState load;           /* 0x78 */
} LmapTaskState;

LmapResourceLoadResult fldAdvanceLmapResourceLoad(LmapTaskState *task);

extern LmapTaskState *D_0043888C;

typedef char LmapTaskState_size_must_be_0x88[(sizeof(LmapTaskState) == 0x88) ? 1 : -1];
#define LMAP_TASK_OFFSET(member) ((u32)&(((LmapTaskState *)0)->member))
typedef char LmapTaskState_spriteSlots_must_be_0x0C[(LMAP_TASK_OFFSET(spriteSlots) == 0x0C) ? 1 : -1];
typedef char LmapTaskState_allocation_must_be_0x70[(LMAP_TASK_OFFSET(allocation) == 0x70) ? 1 : -1];
typedef char LmapTaskState_resourceAddress_must_be_0x74[(LMAP_TASK_OFFSET(resourceAddress) == 0x74) ? 1 : -1];
typedef char LmapTaskState_load_must_be_0x78[(LMAP_TASK_OFFSET(load) == 0x78) ? 1 : -1];
typedef char LmapTaskState_loadPhase_must_be_0x80[(LMAP_TASK_OFFSET(load.phase) == 0x80) ? 1 : -1];
typedef char LmapTaskState_loadEntry_must_be_0x82[(LMAP_TASK_OFFSET(load.entry) == 0x82) ? 1 : -1];
#undef LMAP_TASK_OFFSET

#endif
