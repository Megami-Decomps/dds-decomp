#include "common.h"
#include "itf_debug_panel_work.h"
#include "itf.h"
#include "snd_pad.h"
#include "itf_mes_window.h"
#include "sdf_packet_list.h"
#include "dds3_task_create.h"
#include "scr.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"
#include "itf_panel_draw.h"
#include "sdk_ps2.h"
#include "sdf_trig.h"
#include "btl_state.h"
#include "mnu_result.h"
#include "itf_color_panel_row.h"

#include "sdf_debug_api.h"
extern s8 D_00324530[];
extern const char *D_00358308[];
extern u32 sndTestMessageResourceIndex;
extern sdfTexHandle_t *sndTestMessageTexture;
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);
SYMBOL_AT(sdfGrOt_t, D_00325708, sdfGrOtTable, 0xBC0);

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;
typedef char UiQuadColorSizeCheck[sizeof(UiQuadColor) == 0x10 ? 1 : -1];

extern sdfGrOt_t sdfGrOtTable[];
extern s32 pbDebugPanel3State;
extern UiQuadColor itfTestOverlayColor;
extern u64 *pbAllocPrimPacket(u32, u32, u32, u32, u32, u32, u32);

#include "sdf_packet_alloc.h"

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct ItfColorPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfColorPanelWork;

typedef char ItfColorPanelWorkSizeCheck[sizeof(ItfColorPanelWork) == 0x10 ? 1 : -1];

extern ItfColorPanelWork pbDebugPanel3X;
extern ItfColorPanelRow itfOverlayColorEditorRows[6];
extern SndPad dds3PadButtonState;
extern sdfGrOt_t sdfDebugGrOt;
extern s32 itfTestOverlayPhaseStep;
extern u8 D_003BB2B4;
extern char D_003BB2B8[]; /* "FILTER:" */
extern char D_003BB2C0[]; /* ">" */
extern char D_003BB2C8[]; /* " %s" */
extern char D_003BB2D0[]; /* "%3d" */
extern char D_003BB2D8[]; /* "%4d" */
extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
void pbDrawRotatedPrim(s32 surfaceIndex);

extern u8 D_00358038[];

extern char D_003A14F0[]; /* "TestMsgMngC" */

extern char D_003A1500[]; /* "TestMsgMngD" */

extern s32 D_003BA8EC;

extern void TestMsgReloadTexture(void);

extern s32 TestMsgMngC_Process(dds3ProcessID_t *task);

extern void itfDebugPrintMark(void);

extern s32 TestMsgMngD_Process(dds3ProcessID_t *task);

void itfDebugMesMngInitialize(void) {
    D_003BA8EC = 0x80FFFFFF;
    TestMsgReloadTexture();
    itfMesMngSetKutiCallback(((scrProcessWork_s *)dds3GetProcessData(scrStartScript2(0x3E8, (const scrHeader_t *)D_00358038, 0)))->MesHandle, itfDebugPrintMark);
    dds3StartProcess(D_003A14F0, 0x3EF, 0, 0, TestMsgMngC_Process, 0, 0);
    dds3StartProcess(D_003A1500, 0x2AFE, 0, 0, TestMsgMngD_Process, 0, 0);
}

void TestMsgReloadTexture(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexFreeDelayed(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfMiscLoadTexture(D_00358308[sndTestMessageResourceIndex]);
    }
}

s32 TestMsgMngC_Process(dds3ProcessID_t *task) {
    if (D_00324530[0] < 0) {
        TestMsgReloadTexture();
    }
    return 0;
}

SYMBOL_AT(sdfGrOt_t, D_003255A8, sdfGrOtTable, 0xA60);

extern u8 D_00358318[];

extern u8 D_00358328[];

extern u8 D_00358338[];

s32 TestMsgMngD_Process(dds3ProcessID_t *task) {
    sdfDrawTag_t *mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(mem);
        itfMiscDrawAlphaPrim(mem, 0, 0);
        itfMiscDrawSpritePrim((DrawVertex *)D_00358318, (DrawColorRec *)D_00358328, (DrawColorRec *)D_00358338, 0xFFF, sndTestMessageTexture, 0, mem);
        D_003255A8.regFunc(&D_003255A8, mem);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_003A14F0);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_003A1500);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_003A1510);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_003A1528);

INCLUDE_RODATA(const s32, "interface/itfTestMesManager", D_003A1540);

void itfDebugPrintMark(void) {
    printf("********* AAAA ********\n");
}

typedef struct ItfFovPanelWork {
    u8 pad00[8];
    f32 degrees;
    u8 pad0C[4];
} ItfFovPanelWork;

extern ItfFovPanelWork D_003D73C0;
extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
extern s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);

s32 pbDebugPrintPanel(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    f32 radiansToDegrees = 57.2957795f;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
                    (u32)dds3PrimFrameRect(0x8500, 0x79C0, 0xFEFFFF,
                                  0xA80, 0x120, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfDrawTagLink(list,
                    (u32)sdfDevPrintfEx(&packet, "FOVY: %6.2f",
                        dds3ScreenParam.camera.fov * radiansToDegrees));
    D_00325708.regFunc(&D_00325708, list);
    if (dds3PadButtonState.secondaryCancel < 0) {
        return -1;
    }
    D_003D73C0.degrees = dds3ScreenParam.camera.fov * radiansToDegrees;
    if (pbCalcFovDegrees(&D_003D73C0.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        dds3ScreenParam.camera.fov = D_003D73C0.degrees * 0.0174532925f;
    }
    return 0;
}

s32 pbCalcFovDegrees(f32 *angleDegrees, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *angleDegrees;
    s32 changed = 0;

    if (dds3PadButtonState.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (dds3PadButtonState.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (dds3PadButtonState.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (dds3PadButtonState.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (dds3PadButtonState.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (dds3PadButtonState.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (dds3PadButtonState.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (dds3PadButtonState.fineDown & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *angleDegrees = current;
        return 1;
    }
    return 0;
}

extern ItfDebugPanelWork D_003D73D0;
extern const char *D_00358348[6];
extern s32 D_003BA910[2];
extern u8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern sdfGrOt_t sdfDebugGrOt;
extern char D_003BB260[];
extern char D_003BB268[];
extern char D_003BB270[];
extern char D_003BB278[];
extern char D_003BB280[];
extern char D_003BB288[];

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 pbDebugPrintPanel2(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet,
        "BLUR:        %s", dds3ScreenBlurFlag ? D_003BB260 : D_003BB268));

    y = 0x7A20;
    if (D_003D73D0.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x82C0, D_003D73D0.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_003D73D0.blink++;
        if (D_003D73D0.blink < 32 || (D_003D73D0.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB270));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfDevSetPrintCB(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_003BB278, D_00358348[row]));
        highlight = 0;
        if (D_003D73D0.editing != 0 && row == D_003D73D0.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB280, dds3ScreenBlurAlpha));
            break;
        case 1:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB280, dds3ScreenBlurScale));
            break;
        case 2:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB288, D_003BA910[0], D_003BA910[1]));
            break;
        }
        y += 0x60;
    }
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect((D_003BA910[0] * 16) + 0x7FC0, (D_003BA910[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);

    if ((s8)dds3PadButtonState.secondaryTriangle < 0) {
        dds3ScreenBlurFlag ^= 1;
    }
    if (D_003D73D0.editing == 0) {
        previous = D_003D73D0.selection;
        if ((u8)dds3PadButtonState.secondaryUp & 2) {
            D_003D73D0.selection--;
            if (D_003D73D0.selection < 0) {
                D_003D73D0.selection = 2;
            }
        }
        if ((u8)dds3PadButtonState.secondaryDown & 2) {
            D_003D73D0.selection++;
            if (D_003D73D0.selection >= 3) {
                D_003D73D0.selection = 0;
            }
        }
        if (previous != D_003D73D0.selection) {
            D_003D73D0.blink = 0;
        }
        if (dds3PadButtonState.secondaryCross < 0) {
            D_003D73D0.editing ^= 1;
            D_003D73D0.blink = 0;
        }
        if ((u8)dds3PadButtonState.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (D_003D73D0.selection == 2) {
            if (dds3PadButtonState.secondaryUp != 0) {
                D_003BA910[1] -= 16;
                if (D_003BA910[1] < -224) {
                    D_003BA910[1] = -224;
                }
            }
            if (dds3PadButtonState.secondaryDown != 0) {
                D_003BA910[1] += 16;
                if (D_003BA910[1] > 224) {
                    D_003BA910[1] = 224;
                }
            }
            if (dds3PadButtonState.coarseDown != 0) {
                D_003BA910[0] -= 16;
                if (D_003BA910[0] < -256) {
                    D_003BA910[0] = -256;
                }
            }
            if (dds3PadButtonState.coarseUp != 0) {
                D_003BA910[0] += 16;
                if (D_003BA910[0] > 256) {
                    D_003BA910[0] = 256;
                }
            }
        } else if (D_003D73D0.selection == 0) {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                dds3ScreenBlurAlpha -= 10;
                if (dds3ScreenBlurAlpha < 0) {
                    dds3ScreenBlurAlpha = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                dds3ScreenBlurAlpha += 10;
                if (dds3ScreenBlurAlpha > 255) {
                    dds3ScreenBlurAlpha = 255;
                }
            }
        } else {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                dds3ScreenBlurScale -= 10;
                if (dds3ScreenBlurScale < -255) {
                    dds3ScreenBlurScale = -255;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                dds3ScreenBlurScale += 10;
                if (dds3ScreenBlurScale > 255) {
                    dds3ScreenBlurScale = 255;
                }
            }
        }
        if (dds3PadButtonState.secondaryCancel < 0) {
            D_003D73D0.editing ^= 1;
            D_003D73D0.blink = 0;
        }
    }
    return 0;
}

/* Draw the four color channels and handle selection and value editing. */
/* Retail 0x001A0680 keeps the row < 4 select inside the four-row loop. */
s32 pbDebugPrintPanel3(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfDevSetPrintCB(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB2B8));
    if (pbDebugPanel3X.editing == 0) {
        sdfDevSetPrintCB(&packet, 0x8980, pbDebugPanel3X.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        pbDebugPanel3X.blink++;
        if (pbDebugPanel3X.blink < 32 || (pbDebugPanel3X.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB2C0));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfDevSetPrintCB(&packet, 0x8980, y, 0xFF0000, itfOverlayColorEditorRows[row].highlight);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_003BB2C8, itfOverlayColorEditorRows[row].title));
        highlight = 0;
        if (pbDebugPanel3X.editing != 0 && row == pbDebugPanel3X.selection) {
            highlight = 6;
        }
        sdfDevSetPrintCB(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB2D0, itfTestOverlayColor.channels[row]));
        } else {
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB2D8, itfTestOverlayPhaseStep));
        }
        y += 0x60;
    }
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, list);
    pbDebugPanel3State += itfTestOverlayPhaseStep;
    pbDrawRotatedPrim(0x52);
    if ((s8)dds3PadButtonState.secondaryTriangle < 0) {
        D_003BB2B4 ^= 1;
    }
    if (pbDebugPanel3X.editing == 0) {
        previousSelection = pbDebugPanel3X.selection;
        if ((u8)dds3PadButtonState.secondaryUp & 2) {
            pbDebugPanel3X.selection--;
            if (pbDebugPanel3X.selection < 0) {
                pbDebugPanel3X.selection = 3;
            }
        }
        if ((u8)dds3PadButtonState.secondaryDown & 2) {
            pbDebugPanel3X.selection++;
            if (pbDebugPanel3X.selection >= 4) {
                pbDebugPanel3X.selection = 0;
            }
        }
        if (previousSelection != pbDebugPanel3X.selection) {
            pbDebugPanel3X.blink = 0;
        }
        if (dds3PadButtonState.secondaryCross < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
        if ((u8)dds3PadButtonState.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (pbDebugPanel3X.selection < 4) {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] -= 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] < 0) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                itfTestOverlayColor.channels[pbDebugPanel3X.selection] += 10;
                if (itfTestOverlayColor.channels[pbDebugPanel3X.selection] > 255) {
                    itfTestOverlayColor.channels[pbDebugPanel3X.selection] = 255;
                }
            }
        } else {
            previousRate = itfTestOverlayPhaseStep;
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                itfTestOverlayPhaseStep -= 0x100;
                if (itfTestOverlayPhaseStep < 0) {
                    itfTestOverlayPhaseStep = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                itfTestOverlayPhaseStep += 0x100;
                if (itfTestOverlayPhaseStep > 0x800) {
                    itfTestOverlayPhaseStep = 0x800;
                }
            }
            if (previousRate != itfTestOverlayPhaseStep) {
                pbDebugPanel3State = 0;
            }
        }
        if (dds3PadButtonState.secondaryCancel < 0) {
            pbDebugPanel3X.editing ^= 1;
            pbDebugPanel3X.blink = 0;
        }
    }
    return 0;
}

u64 *pbAllocPrimPacket(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

extern void *sdf2DMakeStdPacket(void *packet, s32 packetBytes);

u64 *pbMakeSpritePacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdf2DMakeStdPacket((void *)sdfGrAllocPacketBuff(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *pbMakePrimPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdf2DMakeStdPacket((void *)sdfGrAllocPacketBuff(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

void pbDrawRotatedPrim(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    f32 phase;

    phase = (f32)(pbDebugPanel3State % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSin(phase) + 1.0f) * 0.5f * itfTestOverlayColor.alpha);
    component = (u8 *)&itfTestOverlayColor;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= (u32)alpha << 24;
    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list, (u32)pbMakeSpritePacket(0x33001, 0));
    sdfDrawTagLink(list, (u32)pbMakePrimPacket(6, 0));
    sdfDrawTagLink(list, (u32)pbAllocPrimPacket(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &sdfGrOtTable[surfaceIndex];
    surface->regFunc(surface, list);
}

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", sndTestMessageResourceIndex);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", sndTestMessageTexture);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB248);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB250);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB258);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB260);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB268);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB270);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB278);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB280);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB288);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB290);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB298);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2A0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2A8);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2B0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2B4);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2B8);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2C0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2C8);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2D0);

INCLUDE_SDATA(const s32, "interface/itfTestMesManager", D_003BB2D8);

