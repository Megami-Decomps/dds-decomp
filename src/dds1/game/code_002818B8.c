#include "common.h"
#include "mnu.h"

void mnuClearPairedSpriteRecords(s32 scene, s32 groupIndex) {
    s32 remaining;
    s32 entry;

    remaining = 1;
    entry = groupIndex * 0x134 + scene + 0x16c;
    do {
        remaining = remaining - 1;
        mnuClearPanelWorkState(entry);
        entry = entry + 0x20;
    } while (-1 < remaining);
}

extern u32 uiBlendColors(u32, u32, s32);
extern void uiDrawGradientColorRect(s32, s32, s32, s32, s32, s32 *, s32);

void mnuDrawWidthScaledPanelGradient(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    s32 colors[4];
    s32 width = 0x138;
    s32 color = uiBlendColors(0x14806E4D, 0x14806E00, work->blend);
    s32 height = 0x170;

    if (work->compact == 0) {
        x += 0x660;
        y += 0x110;
    } else {
        x += 0x1A0;
        y += 0x50;
        height = 0x100;
    }
    colors[3] = colors[1] = color & 0xFFFFFF00;
    colors[2] = colors[0] = color;
    uiDrawGradientColorRect(x, y, z, ((width * (0x200 - work->blend)) / 0x200 + 0x40) << 4,
                 height, colors, surface);
}

void mnuDrawHeightScaledPanelGradient(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    s32 colors[4];
    s32 color = uiBlendColors(0x80501E80, 0x80501E00, work->blend);
    s32 height = 0x4A;
    s32 extent;

    if (work->compact == 0) {
        x += 0x660;
        y += 0x280;
    } else {
        x += 0x1A0;
        y += 0x150;
        height = 0x30;
    }
    colors[3] = color;
    colors[2] = color;
    colors[1] = colors[0] = color & 0xFFFFFF00;
    extent = (height * (0x200 - work->blend) / 0x200) << 3;
    uiDrawGradientColorRect(x, y - extent, z, 0x1000, extent, colors, surface);
}

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281AE8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281BE0);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281D40);

INCLUDE_RODATA(const s32, "game/code_002818B8", D_003B23D8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00282360);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC750);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC758);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC760);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC768);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC770);

