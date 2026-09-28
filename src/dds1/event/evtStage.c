#include "common.h"

s32 dds3GetWorldSecondaryObject(void);
void dds3DestroyWorldNode(s32 ctx);
s32 dds3GetSlot1Data(void);
void func_001175A8(s32 ctx);
void func_001175B8(s32 ctx);

void func_0021FE38(void)
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

INCLUDE_ASM(const s32, "event/evtStage", func_00220110);

INCLUDE_ASM(const s32, "event/evtStage", func_00220178);

void func_00220228(void)
{
    s32 slotData;

    slotData = dds3GetSlot1Data();
    if (slotData != 0) {
        func_001175A8(slotData);
    }
}

void func_00220260(void)
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
