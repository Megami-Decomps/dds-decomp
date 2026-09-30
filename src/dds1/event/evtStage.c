#include "common.h"

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

INCLUDE_ASM(const s32, "event/evtStage", func_00220340);
