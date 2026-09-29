#include "common.h"

extern s32 func_0024A010(void);

extern s32 D_00435DD0;

s32 func_002467B8(s32 arg0);

void func_00249088(s32 arg0, void *arg1);

void func_0024AB38(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern void *func_00101958(void);

void func_0024D430(void);

void func_00101968(s32 arg0, s32 arg1);

s32 evtCreateFrameVariableTask(void);

void *func_0024D6B0(s32 arg0);

extern u32 D_00435CD4;

void func_00137818(void);

void func_00246D80(u64 arg0);

typedef struct EventViewerState {
    u8 pad0[0x14];
    s32 glyphAdvanceLimit;
    s32 glyphAdvancePosition;
    u8 pad1C[0x2220];
    struct {
        u16 id;
        u8 pad2[6];
    } history[8];
    s32 historyCount;
    u32 currentId;
    u8 pad2284[0x13C];
    s32 updateCount;
    u8 pad23C4;
    u8 windowActive;
    u8 pad23C6[0x2A];
    s32 glyphTickCount;
    u8 pad23F4[0x1C];
    u32 glyph;
} EventViewerState;

u16 func_0024ABA0(EventViewerState *viewer);

extern char D_004230D0[]; /* "EventViewer" */

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247518);

INCLUDE_ASM(const s32, "game/code_00247518", func_002475C8);

INCLUDE_ASM(const s32, "game/code_00247518", func_002476B8);

INCLUDE_ASM(const s32, "game/code_00247518", func_002477F0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247858);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247DE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247EE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248000);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248B80);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248D70);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249088);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249518);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249598);

INCLUDE_ASM(const s32, "game/code_00247518", func_002496B0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249A98);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249B40);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249C40);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249DC8);

void evtViewerCountFlaggedUpdates(EventViewerState *viewer) {
    s64 active;

    active = func_0024A010();
    if (active != 0) {
        viewer->updateCount = viewer->updateCount + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00249EE8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A010);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A020);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A158);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A380);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A400);

void func_0024A5F8(void) {
}

void func_0024A600(EventViewerState *viewer) {
    s32 nextTick;

    if ((viewer->glyphAdvancePosition < viewer->glyphAdvanceLimit - 3) && (0 < viewer->glyphTickCount))
    {
        func_0019D518(viewer->glyph);
        nextTick = viewer->glyphTickCount + 1;
        viewer->glyphTickCount = nextTick;
        if (0x1d < nextTick) {
            viewer->glyphTickCount = 0;
        }
    }
}

void func_0024A668(void) {
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A670);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A738);

void func_0024A9F0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    if ((*(u32 *)(temp_v0 + 4) & 1) != 0) {
        func_0024A738(0, *(u32 *)(temp_v0 + 0x18), arg0);
        return;
    }
    func_0024A738(1, *(u32 *)(temp_v0 + 0x18), arg0);
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AA38);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AAB8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AB38);

u16 func_0024ABA0(EventViewerState *viewer) {
    u16 id;
    s32 index;

    index = viewer->historyCount - 1;
    if (viewer->historyCount == 0) {
        viewer->currentId = 0;
        return 0;
    }
    viewer->historyCount = index;
    id = viewer->history[index].id;
    viewer->currentId = (u32)id;
    return id;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024ABD0);

void func_0024ACC0(s32 arg0) {
    s32 v0;
    s32 v1;

    v0 = *(s32 *)(arg0 + 8);
    if (v0 == 0) {
        return;
    }
    v1 = *(s32 *)(v0 + 0x104);
    if (v1 == -1) {
        return;
    }
    itfMesCleanupWindow(v1, 1);
    v0 = *(s32 *)(arg0 + 8);
    func_001A34D0(*(s32 *)(v0 + 0x104));
    v0 = *(s32 *)(arg0 + 8);
    itfPanelSetPairFirst(*(s32 *)(v0 + 0x104), 0);
    v0 = *(s32 *)(arg0 + 8);
    itfMesResetWindow(*(s32 *)(v0 + 0x104));
    ((EventViewerState *)arg0)->windowActive = 0;
    *(u8 *)(arg0 + 0x23c4) = 0;
}

void func_0024AD30(EventViewerState *viewer) {
    viewer->windowActive = 1;
}

void func_0024AD40(EventViewerState *viewer) {
    viewer->windowActive = 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AD48);

s32 func_0024B040(u32 arg0) {
    u32 idx;
    u32 lo;

    idx = (arg0 << 16) >> 28;
    lo = arg0 & 0xfff;
    if (idx == 0) {
        return 1;
    }
    return (*(s32 *)(D_00435DD0 + idx * 4 + 0x35c) ^ lo) == 0;
}

u32 func_0024B078(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227D0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B080);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B268);

u32 func_0024B678(u32 arg0, u32 arg1, u32 arg2) {
    func_0024AB38(5, 0x90, 0x48, arg2);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229D0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229E0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229F0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A00);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A10);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A20);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A30);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A40);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A50);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A60);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A70);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A80);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A90);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AA0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AB0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AC0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B6A8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C540);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C650);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C8C0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C928);

u32 func_0024C9D0(void) {
    return 0;
}

u32 func_0024C9D8(u32 arg0, u32 arg1, u32 arg2) {
    s32 p;

    p = func_002467B8(arg2);
    if (p != 0) {
        *(s32 *)(p + 0xc) = *(s32 *)(arg2 + 0x2310);
        func_00249088(*(s32 *)(arg2 + 0x18), (void *)arg2);
        func_0024ABA0((EventViewerState *)arg2);
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CA28);

u32 kwlnBattleCopyMatrix(u32 arg0, u32 arg1, u8 *scene) {
    u8 *record = (u8 *)func_002467B8((s32)scene);
    if (record != NULL) {
        f32 *dst = *(f32 **)(record + 0x2C);
        f32 *src = (f32 *)(scene + 0x2350);
        s32 index = 3;
        do {
            index--;
            dst[0] = src[-8];
            dst[4] = src[-4];
            dst[8] = src[0];
            src++;
            dst++;
        } while (index >= 0);
        func_00249088(*(s32 *)(scene + 0x18), scene);
        func_0024ABA0((EventViewerState *)scene);
        return 0;
    }
    return (u32)record;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CBA0);

u32 func_0024CC08(u32 arg0, u32 arg1, u32 arg2) {
    func_0024ABA0((EventViewerState *)arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CC28);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CD00);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CD58);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CE18);

u32 func_0024D0F8(u32 arg0, u32 arg1, u32 arg2) {
    if (func_002467B8(arg2) != 0) {
        *(s32 *)(arg2 + 0x22ac) = 0;
        *(s32 *)(arg2 + 0x22b4) = 0;
        func_0024AB38(0xa, 0x9c, 0x54, arg2);
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_00423050);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D148);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D408);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D430);

void *func_0024D6B0(s32 arg0) {
    void *temp_v0;

    temp_v0 = func_00101958();
    func_00249088(*(s32 *)((u8 *)temp_v0 + 0x18), temp_v0);
    func_00101968(arg0, evtCreateFrameVariableTask());
    D_00435CD4 |= 0x2000000;
    return (void *)func_0024D430;
}

void *func_0024D710(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00137818();
    func_00246D80(temp_v0);
    D_00435CD4 |= 0x2000000;
    return (void *)func_0024D6B0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D760);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D788);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D878);

u8 func_0024D908(s32 arg0) {
    return *(s32 *)(arg0 + 0x10c) == 0x263;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D918);

void func_0024DAA0(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_0024D918(temp_v0);
}

void func_0024DAC0(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_0024D918(temp_v0);
}

void func_0024DAE0(void) {
    func_0025EE40();
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DAF8);

void evtEventViewerDestroyTask(void) {
    kwlnTaskDestroyWithHierarchyByName(D_004230D0, 1);
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DBB8);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004230D0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B2);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B4);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372B8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372C0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372C8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372CC);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372D0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372D8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372E0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372E8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372F0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004372F8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437300);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437308);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437310);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437318);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437320);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437328);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437330);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437338);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437340);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437348);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437350);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437358);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437360);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437368);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437370);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437378);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437380);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437388);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437390);

INCLUDE_SDATA(const s32, "game/code_00247518", D_00437398);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373A0);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373A8);

INCLUDE_SDATA(const s32, "game/code_00247518", D_004373B0);
