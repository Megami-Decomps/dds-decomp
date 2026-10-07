#include "common.h"
#include "dds3_path.h"
#include "dds3obj.h"
#include "evt_world.h"

struct EvtScaledValue;
extern void sdfSetFloatCounterDirection(u32 *destination, u32 value);
extern void sdfFreezeFloatCounter(struct EvtScaledValue *);
extern void sdfUnfreezeFloatCounter(struct EvtScaledValue *);


extern void dds3SetSlotKey(void *, void *);
extern void dds3ReplaceObjectResource(void *);

extern Dds3PathCurveWork *dds3GetSlot1Data(void *object);

extern s32 dds3GetWorldSecondaryObject(void);
extern void dds3DestroyWorldNode(EffWorldNode *worldNode);

void evtDestroySecondaryWorldNode(void) {
    EffWorldNode *secondary;

    secondary = (EffWorldNode *)dds3GetWorldSecondaryObject();
    if (secondary != 0) {
        dds3DestroyWorldNode(secondary);
        return;
    }
}


extern void dds3RemoveWorldObjectNode(struct EffWorldNode *node);

/* Detach every unit node from the secondary object's per-kind list. */
void evtDrainSecondaryWorldNodes(void) {
    EffWorldNode *object = (EffWorldNode *)dds3GetWorldSecondaryObject();
    EvtWorldTable *table;

    if (object != NULL) {
        table = ((EvtWorldTable *)object->data);
        while (table->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3RemoveWorldObjectNode(table->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

extern char D_00421578[];
extern char D_003C8BA0[];
extern char D_003C8C60[];
extern char D_003C8C80[];
extern EffWorldNode *dds3AppendWorldNode(void);
extern void dds3SetWorldSecondaryObject(void *);
extern void dds3SetWorldObject(void *);
extern void dds3SetWorldObjectValue(void *, u32);
extern void fldFormatAreaDirectory(char *, s32, s32);
extern void dds3AttachResourceHandleToWorldObject(void *, u32);
extern void mdlSpawnViewerWorldObject(void);
extern void sdfSetViewFieldOfView(f32);
extern void dds3DrawSetIndexedWord(u32, s32);
extern void kwlnDrawCopyWords20(void *);
extern void kwlnDrawCopyRow128(void *);
extern void func_0035C860(char *, char *, ...);

s32 evtCreateWorldObjectForKey(s32 area, s32 room)
{
    char directory[0x40];
    char resourcePath[0x50];
    EffWorldNode *worldObject;

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
        sdfSetViewFieldOfView(0.9424777031f);
    } else {
        sdfSetViewFieldOfView(0.75398216f);
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
    EffWorldNode *worldObject;
    char directory[0x40];

    evtDestroySecondaryWorldNode();
    worldObject = dds3AppendWorldNode();
    dds3SetWorldSecondaryObject(worldObject);
    dds3SetWorldObject(worldObject);
    dds3SetWorldObjectValue(worldObject, (area << 16) + room);
    fldFormatAreaDirectory(directory, area, room);
    dds3AttachConstructedResourceToWorldObject(worldObject, area, room, arg2, arg3, arg4, arg5);
    mdlSpawnViewerWorldObject();
    sdfSetViewFieldOfView(0.75398216f);
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
extern s32 scrCreateProcessTaskFromResource(s32, const char *, s32);

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtCreateEventScriptProcess(s32 eventId) {
    func_0035C860(evtScriptResourcePathBuffer, D_00421588, eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, evtScriptResourcePathBuffer, 0);
}

INCLUDE_RODATA(const s32, "event/evtStage", D_00421578);

INCLUDE_RODATA(const s32, "event/evtStage", D_00421588);

INCLUDE_ASM(const s32, "event/evtStage", func_0023ACE8);

void evtSetWorldSlotStatusFlag(void *object) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfFreezeFloatCounter((struct EvtScaledValue *)slotData);
        return;
    }
}

void evtClearWorldSlotStatusFlag(void *object) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfUnfreezeFloatCounter((struct EvtScaledValue *)slotData);
        return;
    }
}

extern void evtScaleValueByMultiplier(f32 multiplier, struct EvtScaledValue *slotData);

/* Scale the object's retained path time within the clamped multiplier range. */
void evtScaleSlotByClampedMultiplier(void *object, f32 multiplier) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        if (multiplier < 0.0f) {
            multiplier = 0.0f;
        }
        if (multiplier > 1.0f) {
            multiplier = 1.0f;
        }
        evtScaleValueByMultiplier(multiplier, (struct EvtScaledValue *)slotData);
    }
}

void evtSetWorldSlotValue(void *object, u32 value) {
    Dds3PathCurveWork *slotData;

    slotData = dds3GetSlot1Data(object);
    if (slotData != 0) {
        sdfSetFloatCounterDirection((u32 *)&slotData->direction, value);
    }
}

/* Attach object to the node referenced by owner's owned handle. */
s32 evtStageRelinkOwnedNodeResource(void *object, void *owner) {
    ObjBase *ref;
    void *node;

    if (object == NULL) {
        return 0;
    }
    ref = dds3GetObjectOwnedHandle(owner);
    if (ref == NULL) {
        return 0;
    }
    node = ref->slots[1];
    if (node == NULL) {
        return 0;
    }
    dds3SetSlotKey(node, object);
    dds3ReplaceObjectResource(node);
    return 1;
}
