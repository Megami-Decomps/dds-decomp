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
INCLUDE_ASM(const s32, "event/evtStage", func_0023A9E0);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AA30);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AB58);

extern void func_0035C860(char *, char *, ...);
extern char D_00453698[];
extern char D_00421588[];

/* DDS2 twin of DDS1 func_00220110: start the event BF script by id. */
void evtCreateEventScriptProcess(s32 eventId) {
    func_0035C860(D_00453698, D_00421588, eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, D_00453698, 0);
}

INCLUDE_RODATA(const s32, "event/evtStage", D_00421588);

INCLUDE_ASM(const s32, "event/evtStage", func_0023ACE8);

void evtSetWorldSlotStatusFlag(void) {
    s64 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_00117810(slotData);
        return;
    }
}

void evtClearWorldSlotStatusFlag(void) {
    s64 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_00117820(slotData);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0023AE08);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AE70);

/* Attach the object to the node its owned handle points at. */
s32 evtStageRelinkOwnedNodeResource(void *object, void *arg1) {
    StageNodeRef *ref;
    void *node;

    if (object == NULL) {
        return 0;
    }
    ref = (StageNodeRef *)dds3GetObjectOwnedHandle(arg1);
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
