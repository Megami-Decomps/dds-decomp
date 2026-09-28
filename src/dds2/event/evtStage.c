#include "common.h"

extern s32 dds3GetSlot1Data(void);

extern s32 dds3GetWorldSecondaryObject(void);

void func_0023A9A8(void) {
    s64 temp_v0;

    temp_v0 = dds3GetWorldSecondaryObject();
    if (temp_v0 != 0) {
        dds3DestroyWorldNode(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0023A9E0);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AA30);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AB58);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AC80);

INCLUDE_ASM(const s32, "event/evtStage", func_0023ACE8);

void func_0023AD98(void) {
    s64 temp_v0;

    temp_v0 = dds3GetSlot1Data();
    if (temp_v0 != 0) {
        func_00117810(temp_v0);
        return;
    }
}

void func_0023ADD0(void) {
    s64 temp_v0;

    temp_v0 = dds3GetSlot1Data();
    if (temp_v0 != 0) {
        func_00117820(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0023AE08);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AE70);

INCLUDE_ASM(const s32, "event/evtStage", func_0023AEB0);
