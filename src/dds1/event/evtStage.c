#include "common.h"

s32 func_0010FDC0(void);
void func_0010FE48(s32 ctx);
s32 objGetSlot1Data(void);
void func_001175A8(s32 ctx);
void func_001175B8(s32 ctx);

void func_0021FE38(void)
{
    s32 ctx;

    ctx = func_0010FDC0();
    if (ctx != 0) {
        func_0010FE48(ctx);
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_0021FE70);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FEC0);

INCLUDE_ASM(const s32, "event/evtStage", func_0021FFE8);

INCLUDE_ASM(const s32, "event/evtStage", func_00220110);

INCLUDE_ASM(const s32, "event/evtStage", func_00220178);

void func_00220228(void)
{
    s32 ctx;

    ctx = objGetSlot1Data();
    if (ctx != 0) {
        func_001175A8(ctx);
    }
}

void func_00220260(void)
{
    s32 ctx;

    ctx = objGetSlot1Data();
    if (ctx != 0) {
        func_001175B8(ctx);
    }
}

INCLUDE_ASM(const s32, "event/evtStage", func_00220298);

INCLUDE_ASM(const s32, "event/evtStage", func_00220300);

INCLUDE_ASM(const s32, "event/evtStage", func_00220340);
