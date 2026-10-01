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
void sdfFreezeFloatCounter(s32 ctx);
void sdfUnfreezeFloatCounter(s32 ctx);
extern char D_003AC038[];
extern void *func_00101218(s32, s32);
extern void func_003003F0(char *, void *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void *dds3AppendWorldNode(s32, s32);
extern void dds3SetWorldSecondaryObject(void *);
extern void dds3SetWorldObject(void *);
extern void dds3SetWorldObjectValue(void *, s32, s32);
extern void fldFormatAreaDirectory(char *, s32, s32);
extern void dds3AttachResourceHandleToWorldObject(void *, char *);
extern void mdlSpawnViewerWorldObject(void *);
extern void func_00106488(void *, s32, f32);
extern void dds3DrawSetIndexedWord(void *, s32);
extern void kwlnDrawCopyWords20(void *);
extern void kwlnDrawCopyRow128(void *);
extern char D_003AC008[];
extern char D_00367DF0[];
extern char D_00367EB0[];
extern char D_00367ED0[];
extern void func_003014F0(char *, char *, s32, s32, s32);

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

INCLUDE_ASM(const s32, "event/evtStage", func_00220298);

void func_00220300(s32 arg0, void *arg1) {
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_00117568(slotData, arg1);
    }
}

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
