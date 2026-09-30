#include "common.h"

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
void func_0023AC80(s32 eventId) {
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

INCLUDE_ASM(const s32, "event/evtStage", func_0023AEB0);
