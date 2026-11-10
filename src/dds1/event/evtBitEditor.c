#include "sdk_ps2.h"
#include "sdf_request.h"
#include "kernel.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "mdl_part_entry.h"
#include "mdl_flag_api.h"
#include "sdf_sif_command.h"

extern s8 dds3PadBitTable[];
extern s32 sdfGrAllocPacketBuff(s32 size);
extern sdfGrOt_t D_00325708;

extern s32 D_003BBD80, D_003BBD84, D_003BBD88;

extern char D_003BBD90[], D_003BBD98[], D_003BBDA0[];

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32,
    s32 *, s32 *, s32 *, s32 *);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* Native 0021F664..74 copies this space/NUL template as one block;
     * see docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_003BBD90, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_003BBD80 + D_003BBD88) * 16 + D_003BBD84));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_003BBD80 + D_003BBD88) * 16 + D_003BBD84));
    }
    for (column = 0; column < 16; column++) {
        style = D_003BBD84 == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_003BBD98, column));
    }
    for (row = 0; row < 16; row++) {
        style = D_003BBD88 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_003BBDA0, (D_003BBD80 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_003BBD80 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = D_003BBD84 == column || D_003BBD88 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = D_003BBD84 == column || D_003BBD88 == row ? 14 : 8;
            }
            if (D_003BBD84 == column && D_003BBD88 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    D_00325708.regFunc(&D_00325708, packets);
    if ((dds3PadBitTable[24] & 2) && D_003BBD80 > 0) {
        D_003BBD80 -= 16;
        if (D_003BBD80 < 0) D_003BBD80 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_003BBD80 + 16 < 256) {
            if (D_003BBD80 + 32 > 256) D_003BBD80 = 240;
            else D_003BBD80 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_003BBD80, &D_003BBD84, &D_003BBD88);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84)) {
            evtBitOff((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84);
        } else {
            evtBitOn((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84);
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBD80);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBD84);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBD88);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBD90);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBD98);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_003BBDA0);

INCLUDE_SDATA(const s32, "event/evtBitEditor", evtPendingEventSelection);

