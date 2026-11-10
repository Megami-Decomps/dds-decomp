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
SYMBOL_AT(sdfGrOt_t, D_00380708, sdfGrOtTable, 0xBC0);

extern s32 D_004371C0, D_004371C4, D_004371C8;

extern char D_004371D0[], D_004371D8[], D_004371E0[];

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32,
    s32 *, s32 *, s32 *, s32 *);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* Native 0023A1D4..E4 copies this space/NUL template as one block;
     * see docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_004371D0, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    }
    for (column = 0; column < 16; column++) {
        style = D_004371C4 == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_004371D8, column));
    }
    for (row = 0; row < 16; row++) {
        style = D_004371C8 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_004371E0, (D_004371C0 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_004371C0 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = D_004371C4 == column || D_004371C8 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = D_004371C4 == column || D_004371C8 == row ? 14 : 8;
            }
            if (D_004371C4 == column && D_004371C8 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    D_00380708.regFunc(&D_00380708, packets);
    if ((dds3PadBitTable[24] & 2) && D_004371C0 > 0) {
        D_004371C0 -= 16;
        if (D_004371C0 < 0) D_004371C0 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_004371C0 + 16 < 256) {
            if (D_004371C0 + 32 > 256) D_004371C0 = 240;
            else D_004371C0 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_004371C0, &D_004371C4, &D_004371C8);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
            evtBitOff((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        } else {
            evtBitOn((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371C0);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371C4);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371C8);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371D0);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371D8);

INCLUDE_SDATA(const s32, "event/evtBitEditor", D_004371E0);

INCLUDE_SDATA(const s32, "event/evtBitEditor", evtPendingEventSelection);

