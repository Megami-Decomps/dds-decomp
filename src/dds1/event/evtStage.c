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

s32 dds3GetWorldSecondaryObject(void);
void dds3DestroyWorldNode(s32 ctx);
s32 dds3GetSlot1Data(void);
void func_001175A8(s32 ctx);
void func_001175B8(s32 ctx);

void evtDestroySecondaryWorldNode(void)
{
    s32 node;

    node = dds3GetWorldSecondaryObject();
    if (node != 0) {
        dds3DestroyWorldNode(node);
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0021FE70);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FEC0);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FFE8);

extern void func_003014F0(char *, char *, s32, s32, s32);
extern void scrCreateProcessTaskFromResource(s32, void *, s32);
extern char D_003D7B98[];

void evtCreateEventScriptProcess(s32 eventId) {
    func_003014F0(D_003D7B98, "/event/e%03d/e%03d/scr/e%03d.bf", eventId - eventId % 10, eventId, eventId);
    scrCreateProcessTaskFromResource(0x3EB, D_003D7B98, 0);
}

INCLUDE_ASM(const s32, "event/evtStage", func_00220178);

void evtSetWorldSlotStatusFlag(void)
{
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_001175A8(slotData);
    }
}

void evtClearWorldSlotStatusFlag(void)
{
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_001175B8(slotData);
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_00220298);

INCLUDE_ASM(const s32, "event/evtStage", func_00220300);

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
