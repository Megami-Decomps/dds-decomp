#include "common.h"
#include "dds3obj.h"
#include "evt_world.h"


extern void dds3SetSlotKey(void *, void *);
extern void dds3ReplaceObjectResource(void *);

s32 dds3GetWorldSecondaryObject(void);
void dds3DestroyWorldNode(s32 ctx);
s32 dds3GetSlot1Data(void);
void sdfFreezeFloatCounter(s32 ctx);
void sdfUnfreezeFloatCounter(s32 ctx);
extern char D_003AC038[];
extern void *func_00101218(s32, s32);
extern void func_003003F0(char *, void *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void *dds3AppendWorldNode(void);
extern void dds3SetWorldSecondaryObject(void *);
extern void dds3SetWorldObject(void *);
extern void dds3SetWorldObjectValue(void *, u32);
extern void fldFormatAreaDirectory(char *, s32, s32);
extern void dds3AttachResourceHandleToWorldObject(void *, u32);
extern void mdlSpawnViewerWorldObject(void);
extern void func_00106488(f32);
extern void dds3DrawSetIndexedWord(u32, s32);
extern void kwlnDrawCopyWords20(void *);
extern void kwlnDrawCopyRow128(void *);
extern char D_003AC008[];
extern char D_00367DF0[];
extern char D_00367EB0[];
extern char D_00367ED0[];
extern void func_003014F0(char *, char *, ...);

void evtDestroySecondaryWorldNode(void)
{
    s32 node;

    node = dds3GetWorldSecondaryObject();
    if (node != 0) {
        dds3DestroyWorldNode(node);
    }
}


struct WorldListNode;
extern void dds3RemoveWorldObjectNode(struct WorldListNode *node);

/* Detach every unit node from the secondary object's per-kind list. */
void evtDrainSecondaryWorldNodes(void) {
    EvtWorldObject *object = (EvtWorldObject *)dds3GetWorldSecondaryObject();
    EvtWorldTable *table;

    if (object != NULL) {
        table = object->table;
        while (table->slots[EVT_WORLD_SLOT_UNIT].head != NULL) {
            dds3RemoveWorldObjectNode(table->slots[EVT_WORLD_SLOT_UNIT].head);
        }
    }
}

/* Build the field world object and load its stage resource. */
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
    func_003014F0(resourcePath, D_003AC008, directory, area, room);
    dds3AttachResourceHandleToWorldObject(worldObject, (u32)resourcePath);
    mdlSpawnViewerWorldObject();
    if (area < 10) {
        func_00106488(0.9424777031f);
    } else {
        func_00106488(0.75398216f);
    }
    dds3DrawSetIndexedWord((u32)D_00367DF0, 0);
    dds3DrawSetIndexedWord((u32)(D_00367DF0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_00367DF0 + 0x80), 2);
    kwlnDrawCopyWords20(D_00367EB0);
    kwlnDrawCopyRow128(D_00367ED0);
    return 1;
}

/* Build a field world object from the supplied resource parameters. */
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
    func_00106488(0.75398216f);
    dds3DrawSetIndexedWord((u32)D_00367DF0, 0);
    dds3DrawSetIndexedWord((u32)(D_00367DF0 + 0x40), 1);
    dds3DrawSetIndexedWord((u32)(D_00367DF0 + 0x80), 2);
    kwlnDrawCopyWords20(D_00367EB0);
    kwlnDrawCopyRow128(D_00367ED0);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    return 1;
}

extern void scrCreateProcessTaskFromResource(s32, void *, s32);
extern char evtScriptResourcePathBuffer[];

INCLUDE_RODATA(const s32, "event/evtStage", D_003AC008);

void evtCreateEventScriptProcess(s32 eventId) {
    func_003014F0(evtScriptResourcePathBuffer, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, evtScriptResourcePathBuffer, 0);
}

INCLUDE_ASM(const s32, "event/evtStage", func_00220178);

void evtSetWorldSlotStatusFlag(void)
{
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        sdfFreezeFloatCounter(slotData);
    }
}

void evtClearWorldSlotStatusFlag(void)
{
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        sdfUnfreezeFloatCounter(slotData);
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
        func_00117568(slotData, data);
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
