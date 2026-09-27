#include "common.h"

extern s32 func_00112868(void);

extern s32 func_0010FDC0(void);

void func_0021FE38(void) {
    s64 temp_v0;

    temp_v0 = func_0010FDC0();
    if (temp_v0 != 0) {
        func_0010FE48(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0021FE70);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FEC0);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FFE8);

INCLUDE_ASM(const s32, "event/evtStage", func_00220110);

INCLUDE_ASM(const s32, "event/evtStage", func_00220178);

void func_00220228(void) {
    s64 temp_v0;

    temp_v0 = func_00112868();
    if (temp_v0 != 0) {
        func_001175A8(temp_v0);
        return;
    }
}

void func_00220260(void) {
    s64 temp_v0;

    temp_v0 = func_00112868();
    if (temp_v0 != 0) {
        func_001175B8(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_00220298);

INCLUDE_ASM(const s32, "event/evtStage", func_00220300);

INCLUDE_ASM(const s32, "event/evtStage", func_00220340);
