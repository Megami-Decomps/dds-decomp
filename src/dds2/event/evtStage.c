#include "common.h"

/* Object record reached through an owned-handle lookup; the linked node sits
   at +0x14. */
typedef struct StageNodeRef {
    u8 pad00[0x14];
    void *node; /* 0x14 */
} StageNodeRef;

extern void *dds3GetObjectOwnedHandle(void *);
extern void dds3SetSlotKey(void *, void *);
extern void dds3ReplaceObjectResource(void *);

extern s32 dds3GetSlot1Data(void);

extern s32 dds3GetWorldSecondaryObject(void);

void evtDestroySecondaryWorldNode(void) {
    s64 secondary;

    secondary = dds3GetWorldSecondaryObject();
    if (secondary != 0) {
        dds3DestroyWorldNode(secondary);
        return;
    }
}

typedef struct StageNodeChild {
    u8 pad00[0x40];
    s32 node; /* 0x40: first world node still attached */
} StageNodeChild;

typedef struct StageNodeParent {
    u8 pad00[8];
    StageNodeChild *child; /* 0x8 */
} StageNodeParent;

typedef struct StageSecondaryObject {
    u8 pad00[0x18];
    StageNodeParent *parent; /* 0x18 */
} StageSecondaryObject;

extern void dds3RemoveWorldObjectNode(s32 node);

/* Detach every world node from the secondary object's chain. */
void evtDrainSecondaryWorldNodes(void) {
    StageSecondaryObject *object = (StageSecondaryObject *)dds3GetWorldSecondaryObject();
    StageNodeParent *parent;

    if (object != NULL) {
        parent = object->parent;
        while (parent->child->node != 0) {
            dds3RemoveWorldObjectNode(parent->child->node);
        }
    }
}

extern char D_00421578[];
extern char D_003C8BA0[];
extern char D_003C8C60[];
extern char D_003C8C80[];
extern void *dds3AppendWorldNode(void);
extern void dds3SetWorldSecondaryObject(void *);
extern void dds3SetWorldObject(void *);
extern void dds3SetWorldObjectValue(void *, u32);
extern void fldFormatAreaDirectory(char *, s32, s32);
extern void dds3AttachResourceHandleToWorldObject(void *, u32);
extern void mdlSpawnViewerWorldObject(void);
extern void func_001063A8(f32);
extern void dds3DrawSetIndexedWord(u32, s32);
extern void kwlnDrawCopyWords20(void *);
extern void kwlnDrawCopyRow128(void *);
extern void func_0035C860(char *, char *, ...);

s32 evtCreateWorldObjectForKey(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    void *worldObject;

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    func_0035C860(resourcePath, D_00421578, directory, area, room);
    dds3AttachResourceHandleToWorldObject(worldObject, (u32)resourcePath);
    mdlSpawnViewerWorldObject();
    if (area < 10) {
        func_001063A8(0.9424777031f);
    } else {
        func_001063A8(0.75398216f);
    }
    dds3DrawSetIndexedWord((u32)D_003C8BA0, 0);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x80), 2);
    kwlnDrawCopyWords20(D_003C8C60);
    kwlnDrawCopyRow128(D_003C8C80);
    return 1;
}

extern void dds3AttachConstructedResourceToWorldObject(void *, s32, s32, s32, s32, s32, s32);
extern s32 sdfCheckPendingWorkWithInterrupts(void);

s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    void *worldObject;
    char directory[0x40];

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    dds3AttachConstructedResourceToWorldObject(worldObject, area, room, arg2, arg3, arg4, arg5);
    mdlSpawnViewerWorldObject();
    func_001063A8(0.75398216f);
    dds3DrawSetIndexedWord((u32)D_003C8BA0, 0);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_003C8BA0 + 0x80), 2);
    kwlnDrawCopyWords20(D_003C8C60);
    kwlnDrawCopyRow128(D_003C8C80);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    return 1;
}

extern void func_0035C860(char *, char *, ...);
extern char evtScriptResourcePathBuffer[];
extern char D_00421588[];

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtCreateEventScriptProcess(s32 eventId) {
    func_0035C860(evtScriptResourcePathBuffer, D_00421588, eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, evtScriptResourcePathBuffer, 0);
}

INCLUDE_RODATA(const s32, "event/evtStage", D_00421578);

INCLUDE_RODATA(const s32, "event/evtStage", D_00421588);

INCLUDE_ASM(const s32, "event/evtStage", func_0023ACE8);

void evtSetWorldSlotStatusFlag(void) {
    s64 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        sdfFreezeFloatCounter(slotData);
        return;
    }
}

void evtClearWorldSlotStatusFlag(void) {
    s64 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        sdfUnfreezeFloatCounter(slotData);
        return;
    }
}

extern void evtScaleValueByMultiplier(s32 slotData, f32 multiplier);

/* The unit remains an ABI argument; scaling resolves the active slot itself. */
void evtScaleSlotByClampedMultiplier(void *unused, f32 multiplier) {
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        evtScaleValueByMultiplier(slotData, multiplier);
    }
}

void evtSetWorldSlotValue(s32 unused, void *data) {
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_001177D0(slotData, data);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtStageRelinkOwnedNodeResource(void *object, void *owner) {
    StageNodeRef *ref;
    void *node;

    if (object == NULL) {
        return 0;
    }
    ref = (StageNodeRef *)dds3GetObjectOwnedHandle(owner);
    if (ref == NULL) {
        return 0;
    }
    node = ref->node;
    if (node == NULL) {
        return 0;
    }
    dds3SetSlotKey(node, object);
    dds3ReplaceObjectResource(node);
    return 1;
}
