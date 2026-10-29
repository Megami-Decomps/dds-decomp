#include "sdk_ps2.h"
#include "sdf_scene_packet.h"
#include "sdf_gs_scene_state.h"
#include "common.h"
#include "file_config_option_api.h"
#include "sdf_vu0.h"
#include "kwln_background_fade_api.h"
#include "sdf_pad_motor_api.h"
#include "dds3Admin.h"
#include "kwln_draw_buffer.h"
#include "eff_random_mod.h"
#include "eff_random.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_tex_packet.h"
#include "kwln.h"
#include "kwln_task_update_count.h"
#include "kwln_process_registration.h"
#include "kwln_named_slot.h"
#include "kwln_task_create.h"
#include "kwln_task_runner.h"
#include "kwln_fade.h"
#include "kwln_fade_timing.h"
#include "sdf.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_linked_packet.h"
#include "sdf_draw.h"
#include "sdf_gs_blend.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "btl.h"
/* Declared before first use (were implicit int). */
extern void eglSetOt(u32 surfaceIndex);
extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);
extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

extern u8 D_00435C60;

extern u32 kwlnTextureViewerPageIndex;

extern u32 dds3RenderTex;

extern sdfGsMemSlice_t *D_00435CC4;

extern sdfTexHandle_t *dds3RenderTexture;

extern u16 D_00435CE8;

extern u32 dds3DrawFlag;

extern u16 fadenow;

extern u16 fademax;

extern s16 D_00435CEA;

extern u16 D_00435CEC;

extern u16 D_00435CEE;



extern void dds3PadVibrationSet(u32, u8, s32);

#include "sdf_packet_alloc.h"
#include "kwln_fade_color.h"
#include "evt_frame_state_packet.h"


extern void kwlnDrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

extern u32 kwlnTextureGetPageIndex(void);

extern s32 kwlnTextureCountIncompleteResources(void);


extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);


extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern s32 kwlnDebugGraphUpdate(dds3ProcessID_t *task);

extern void kwlnDebugGraphTaskDestroy(dds3ProcessID_t *task);

extern s32 D_00435BA4;

extern s32 distblurovercnt;

extern s32 ripblurovercnt;

extern SdfPoolNode D_00380708;

extern dds3ProcessID_t *kwlnDebugGraphTask;

extern f32 D_0037F5EC[];


extern sdfTexHandle_t *kwlnCurrentIncompleteResource;

extern s32 kwlnTextureViewerHandlePad(void);

extern s8 dds3PadBitTable[32];

extern s32 kwlnSelectIncompleteResourceAtPageIndex(s32);

extern void kwlnDrawTextureViewerFrame(SdfPoolNode *surface, sdfTexHandle_t *image);

extern SdfPoolNode kwlnPositionedTextSurface;

extern s8 D_0037F53B[];

extern void kwlnTextureAttachTask(SdfPoolNode *surface);

extern s16 kwlnBackgroundFadeFirstRamp;

extern s16 kwlnBackgroundFadeSecondRamp;

extern u8 kwlnBackgroundFadeColor[4];

extern s8 kwlnBackgroundFadeMode;

extern u8 kwlnLargeMotorTarget;

extern u8 dds3PadVibrationState[2];

extern KwlnTaskUpdateResult dds3AdminPollModeCompletion(dds3ProcessID_t *task);
extern void dds3AdminiNeoShutdown(dds3ProcessID_t *task);
extern char dds3AdminTaskName[];
extern u16 kwlnBackgroundFadeCounter;
extern u16 kwlnBackgroundFadeDuration;

extern EffRandState effSharedRandomState[];

extern void sdfRandomize(void *data, u32 tag);

void dds3InitAdmini(void) {
    AdminWork *work;
    s32 i;

    if (dds3SearchProcessName(dds3AdminTaskName) == NULL) {
        work = sdfChipAllocDirect(0x24);
        work->flags = 0;
        work->value = 0;
        work->activeMode = -1;
        work->requestedMode = -1;
        work->historyIndex = 0;
        for (i = 0; i < 8; i++) {
            work->signedHistory[i] = -1;
            work->unsignedHistory[i] = 0;
        }
        work->ownedRequestData = 0;
        work->requestSizeLowByte = 0;
        dds3StartProcess(dds3AdminTaskName, 2, 0, 1, dds3AdminPollModeCompletion, dds3AdminiNeoShutdown, (u32)work);
    }
}

extern KwlnNamedSlot D_0043D410[];
extern char D_00411198[];
extern s32 D_00435C20;
extern s32 D_00435C24;
extern s32 D_00435C28;
extern s32 D_00435C2C;

s32 fclEncycFreeEntry(const char *name);

s32 fclEncycFindFreeSlot(const char *name) {
    s32 slotIndex;
    s32 characterIndex;
    s32 previous;

    if (dds3SearchProcessName(D_00411198) == NULL) {
        return -1;
    }
    if (fclEncycFreeEntry(name) >= 0) {
        return -1;
    }
    for (slotIndex = 0; slotIndex < 32; slotIndex++) {
        if (D_0043D410[slotIndex].kind == 0) {
            break;
        }
    }
    if (slotIndex == 32) {
        return -1;
    }
    D_0043D410[slotIndex].kind = 0;
    characterIndex = 0;
    while ((D_0043D410[slotIndex].name[characterIndex] = name[characterIndex]) != '\0') {
        if (characterIndex == 31) {
            D_0043D410[slotIndex].name[characterIndex] = '\0';
            break;
        }
        characterIndex++;
    }
    D_0043D410[slotIndex].task = NULL;
    D_0043D410[slotIndex].update = NULL;
    D_0043D410[slotIndex].destroy = NULL;
    D_0043D410[slotIndex].setState = NULL;
    D_0043D410[slotIndex].enabled = 0;
    D_0043D410[slotIndex].userValue = NULL;
    D_0043D410[slotIndex].previous = -1;
    D_0043D410[slotIndex].next = -1;
    previous = D_00435C24;
    if (previous < 0) {
        D_00435C20 = slotIndex;
        D_00435C24 = slotIndex;
        D_00435C2C = slotIndex;
    } else {
        D_00435C24 = slotIndex;
        D_0043D410[previous].next = slotIndex;
        D_0043D410[slotIndex].previous = previous;
    }
    D_00435C28++;
    return slotIndex;
}

/* One shared initializer for the three configure modes and unregister. */
static inline void kwlnSetNamedSlotState(s32 index, s32 kind, KwlnTaskUpdateCallback update, TaskDestroy destroy,
                                         void (*setState)(s32), void *userValue) {
    D_0043D410[index].kind = kind;
    D_0043D410[index].task = NULL;
    D_0043D410[index].update = update;
    D_0043D410[index].destroy = destroy;
    D_0043D410[index].setState = setState;
    D_0043D410[index].enabled = 0;
    D_0043D410[index].userValue = userValue;
}

#define KWLN_PAD_INPUTS_PER_SET 16
#define KWLN_PAD_REPEAT_BIT 2
#define KWLN_ALT_CONFIRM_INPUT 0x11
#define KWLN_ALT_CANCEL_INPUT 0x13
#define KWLN_PAD_SMALL_MOTOR 0
#define KWLN_PAD_LARGE_MOTOR 1
#define KWLN_PAD_MOTOR_COUNT 2
#define KWLN_DEBUG_RUMBLE_PERIOD 0x4650
#define KWLN_DEBUG_SMALL_PULSE_FRAMES 0xF
#define KWLN_DEBUG_LARGE_PULSE_FRAMES 30
#define KWLN_DEBUG_RUMBLE_VARIATION 150
#define KWLN_DEBUG_RUMBLE_BASE 100
#define KWLN_SHARED_RANDOM_SEED 0x12345678
#define KWLN_CELL_COLUMN_SPAN 0xC0
#define KWLN_CELL_ROW_SPAN 0x60
#define KWLN_PREVIEW_MAX_TEXELS 0x100
#define KWLN_PREVIEW_WRAP_TEXELS 0x400
#define KWLN_DIAG_DEPTH 0x0FFFFF80

/* Find an exact NUL-terminated name along the linked slot indices, or return -1. */
s32 fclEncycFreeEntry(const char *name) {
    s32 slotIndex = D_00435C20;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (D_0043D410[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != D_0043D410[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (D_0043D410[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = D_0043D410[slotIndex].next;
    }
    return -1;
}

/* Unregister a named slot: stop its task, clear it and unlink it from the slot chain. */
s32 fclEncycFreeEntries(const char *name) {
    s32 slotIndex;

    if (dds3SearchProcessName(D_00411198) == NULL) {
        return 0;
    }
    slotIndex = fclEncycFreeEntry(name);
    if (slotIndex < 0) {
        return 0;
    }
    if (D_0043D410[slotIndex].kind > 0) {
        if (D_0043D410[slotIndex].kind < 3) {
            if (dds3ExistProcess(D_0043D410[slotIndex].task) == KWLN_PROCESS_REGISTERED) {
                dds3KillProcess(D_0043D410[D_00435C2C].task, 1);
            }
        }
    }
    D_0043D410[slotIndex].name[0] = 0;
    kwlnSetNamedSlotState(slotIndex, 0, NULL, NULL, NULL, 0);
    if (D_0043D410[slotIndex].previous < 0) {
        D_00435C20 = D_0043D410[slotIndex].next;
    } else {
        D_0043D410[D_0043D410[slotIndex].previous].next = D_0043D410[slotIndex].next;
    }
    if (D_0043D410[slotIndex].next < 0) {
        D_00435C24 = D_0043D410[slotIndex].previous;
    } else {
        D_0043D410[D_0043D410[slotIndex].next].previous = D_0043D410[slotIndex].previous;
    }
    D_0043D410[slotIndex].previous = -1;
    D_0043D410[slotIndex].next = -1;
    if (D_00435C2C == slotIndex) {
        D_00435C2C = D_00435C20;
    }
    D_00435C28--;
    return 1;
}


s32 kwlnConfigureNamedSlotMode3(const char *name, void (*setState)(s32)) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    kwlnSetNamedSlotState(index, 3, NULL, NULL, setState, 0);
    return 1;
}

s32 fclEncycAllocEntry(const char *name, KwlnTaskUpdateCallback update, TaskDestroy destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    kwlnSetNamedSlotState(index, 1, update, destroy, NULL, userValue);
    return 1;
}

s32 kwlnConfigureNamedSlotMode2(const char *name, KwlnTaskUpdateCallback update, TaskDestroy destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    kwlnSetNamedSlotState(index, 2, update, destroy, NULL, userValue);
    return 1;
}

void dds3InitDebugSystem(void) {
}

extern s8 dds3PadButtonState[64];

/* Fresh presses wrap; repeat-bit input stops at the implemented end bounds.
 * Without scrolling, forward motion is bounded by visibleRows, not itemCount. */
void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput) {
    s32 backwardState;
    s32 forwardState;

    if (cursorRow == NULL) {
        return;
    }
    if (itemCount < 2) {
        return;
    }
    backwardState = dds3PadButtonState[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
    if (backwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            } else if (itemCount < visibleRows) {
                *cursorRow = itemCount - 1;
            } else {
                *cursorRow = visibleRows - 1;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        } else if (visibleRows < itemCount) {
            *scrollOffset = itemCount - visibleRows;
            *cursorRow = visibleRows - 1;
        } else {
            *cursorRow = itemCount - 1;
        }
    } else if (dds3PadButtonState[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
        if (scrollOffset == NULL) {
            if (*cursorRow > 0) {
                (*cursorRow)--;
            }
        } else if (*cursorRow > 0) {
            (*cursorRow)--;
        } else if (*scrollOffset > 0) {
            (*scrollOffset)--;
        }
    }
    forwardState = dds3PadButtonState[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
    if (forwardState < 0) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            } else {
                *cursorRow = 0;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        } else {
            *scrollOffset = 0;
            *cursorRow = 0;
        }
    } else if (dds3PadButtonState[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
        if (scrollOffset == NULL) {
            if (*cursorRow < visibleRows - 1) {
                (*cursorRow)++;
            }
        } else if (*cursorRow < visibleRows - 1) {
            (*cursorRow)++;
        } else if (*scrollOffset + visibleRows < itemCount) {
            (*scrollOffset)++;
        }
    }
}

/* Step the primary then secondary cursor pair; return 1/-1 for confirm/cancel.
 * Any nonzero padSet tests set 1's confirm/cancel, while movement uses padSet. */
s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor) {
    SelectSub(primaryScroll, primaryCursor, primaryCount, primaryVisible, padSet, 7, 6);
    SelectSub(secondaryScroll, secondaryCursor, secondaryCount, secondaryVisible, padSet, 5, 4);
    if (padSet == 0) {
        if (dds3PadButtonState[1] < 0) {
            return 1;
        }
        if (dds3PadButtonState[3] < 0) {
            return -1;
        }
    } else {
        if (dds3PadButtonState[KWLN_ALT_CONFIRM_INPUT] < 0) {
            return 1;
        }
        if (dds3PadButtonState[KWLN_ALT_CANCEL_INPUT] < 0) {
            return -1;
        }
    }
    return 0;
}

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % KWLN_DEBUG_RUMBLE_PERIOD == 0) {
        dds3PadVibrationSet(KWLN_PAD_SMALL_MOTOR, 1, KWLN_DEBUG_SMALL_PULSE_FRAMES);
        dds3PadVibrationSet(KWLN_PAD_LARGE_MOTOR, (u8)(dds3GetRandIntA(0, KWLN_DEBUG_RUMBLE_VARIATION) + KWLN_DEBUG_RUMBLE_BASE), KWLN_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

extern u32 kwlnPadButtonHoldCounters[4][16];
extern u32 kwlnPadStickHoldCounters[2][2];
extern s8 dds3PadButtonPrev[2][2][16];
extern u8 D_00435C30[2][2][2];
extern s8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern u8 D_00438D98[2];
extern u32 D_00438DA0[2];

void dds3PadInitialize(void) {
    s32 port;
    s32 bank;
    s32 i;
    u32 (*inputTimers)[16] = kwlnPadButtonHoldCounters;

    for (port = 0; port < 2; port++) {
        for (bank = 0; bank < 2; bank++) {
            for (i = 0; i < 16; i++) {
                dds3PadButtonPrev[bank][port][i] = 0;
                dds3PadButtonState[bank * 32 + port * 16 + i] = 0;
                inputTimers[bank * 2 + port][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            kwlnPadStickHoldCounters[port][i] = 0;
            D_00435C30[port][i][0] = 0;
            D_00435C30[port][i][1] = 0;
            D_00435C38[port][i] = 0;
            D_00435C40[port][i][0] = 0;
            D_00435C40[port][i][1] = 0;
            D_00435C48[port][i] = 0;
        }
    }
    dds3PadVibrationState[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438D98[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438D98[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_LARGE_MOTOR] = 0;
}

extern u8 sdfPadAnalogSticks[8];
extern void dds3UpdateVibrationSlots(void);

/* vu0 routine: native stick length, normalization and forward dot product. */
INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C20);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C24);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C28);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C2C);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C2F);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C30);

void kwlnUpdatePadInputState(void) {
    s8 buttons[2][16];
    u8 sticks[2][2];
    f32 v[4];
    f32 forward[4];
    f32 length;
    f32 dot;
    f32 angle;
    s32 pad;
    s32 i;
    s32 j;
    s32 n;

    memset(forward, 0, sizeof(forward));
    forward[2] = -1.0f;
    for (pad = 0; pad < 2; pad++) {
        for (i = 0; i < 16; i++) {
            if (dds3PadBitTable[pad * 16 + i] != 0) {
                buttons[0][i] = 1;
                buttons[1][i] = 1;
            } else {
                buttons[0][i] = 0;
                buttons[1][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            sticks[i][0] = sdfPadAnalogSticks[pad * 4 + i * 2];
            sticks[i][1] = sdfPadAnalogSticks[pad * 4 + i * 2 + 1];
        }
        for (i = 0; i < 2; i++) {
            D_00435C30[pad][i][0] = sticks[i][0];
            D_00435C30[pad][i][1] = sticks[i][1];
        }
        v[0] = sticks[1][0] - 0x80;
        v[1] = 0;
        v[2] = sticks[1][1] - 0x80;
        v[3] = 0;
        SDF_VU0_LOADVECTOR(v);
        SDF_VU0_GETLENGTH(length);
        if (length >= 64.0f) {
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(forward);
            SDF_VU0_INNERPRODUCT(dot);
            if (dot < -1.0f) {
                dot = -1.0f;
            }
            if (dot > 1.0f) {
                dot = 1.0f;
            }
            angle = acosf(dot);
            if (v[0] < 0.0f) {
                angle = 6.283185307179586f - angle;
            }
            n = (s32)(angle * 57.29577951308232f);
            n = (n + 22) / 45;
            n %= 8;
            switch (n) {
            case 0:
                buttons[1][6] = 1;
                break;
            case 1:
                buttons[1][6] = 1;
                buttons[1][5] = 1;
                break;
            case 2:
                buttons[1][5] = 1;
                break;
            case 3:
                buttons[1][5] = 1;
                buttons[1][7] = 1;
                break;
            case 4:
                buttons[1][7] = 1;
                break;
            case 5:
                buttons[1][7] = 1;
                buttons[1][4] = 1;
                break;
            case 6:
                buttons[1][4] = 1;
                break;
            case 7:
                buttons[1][4] = 1;
                buttons[1][6] = 1;
                break;
            }
        }
        for (j = 0; j < 2; j++) {
            for (i = 0; i < 16; i++) {
                if (buttons[j][i] != 0) {
                    dds3PadButtonPrev[j][pad][i] = 1;
                    if (++kwlnPadButtonHoldCounters[j * 2 + pad][i] == 1) {
                        dds3PadButtonPrev[j][pad][i] = -0x7D;
                    }
                } else {
                    dds3PadButtonPrev[j][pad][i] = 0;
                    kwlnPadButtonHoldCounters[j * 2 + pad][i] = 0;
                }
                if (kwlnPadButtonHoldCounters[j * 2 + pad][i] >= 8 && !(kwlnPadButtonHoldCounters[j * 2 + pad][i] & 1)) {
                    dds3PadButtonPrev[j][pad][i] |= 2;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            f32 stickLength;

            v[0] = sticks[i][0] - 0x80;
            v[1] = 0;
            v[2] = sticks[i][1] - 0x80;
            v[3] = 0;
            SDF_VU0_LOADVECTOR(v);
            SDF_VU0_GETLENGTH(stickLength);
            if (stickLength >= 64.0f) {
                D_00435C38[pad][i] = 1;
                if (++kwlnPadStickHoldCounters[pad][i] == 1) {
                    D_00435C38[pad][i] = -0x7D;
                }
            } else {
                D_00435C38[pad][i] = 0;
                kwlnPadStickHoldCounters[pad][i] = 0;
            }
            if (kwlnPadStickHoldCounters[pad][i] >= 8 && !(kwlnPadStickHoldCounters[pad][i] & 1)) {
                D_00435C38[pad][i] |= 2;
            }
        }
    }
    dds3UpdateVibrationSlots();
}

void kwlnPadStepLargeMotorLevel(void) {
    u8 target = kwlnLargeMotorTarget;

    if (dds3PadVibrationState[1] < target) {
        if (target - dds3PadVibrationState[1] > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] + 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else if (target < dds3PadVibrationState[1]) {
        if (dds3PadVibrationState[1] - target > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] - 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else {
        dds3PadVibrationState[1] = target;
    }
}

/* Honor the saved rumble option, normalize small-motor strength and clamp
 * motor indices to the large motor. Preserve target update before output. */
void dds3PadVibrationSet(u32 motor, u8 level, s32 duration) {
    if (fileTestSavedSlotFlags(FILE_CONFIG_OPTION_RUMBLE) == 0) {
        return;
    }
    if (motor == KWLN_PAD_SMALL_MOTOR) {
        if (level != 0) {
            level = 1;
        }
    } else if (motor >= KWLN_PAD_MOTOR_COUNT) {
        motor = KWLN_PAD_LARGE_MOTOR;
    }
    if (motor == KWLN_PAD_SMALL_MOTOR) {
        D_00438D98[KWLN_PAD_SMALL_MOTOR] = level;
        dds3PadVibrationState[KWLN_PAD_SMALL_MOTOR] = level;
    } else {
        D_00438D98[motor] = level;
        kwlnPadStepLargeMotorLevel();
    }
    D_00438DA0[motor] = duration;
    if (motor == KWLN_PAD_SMALL_MOTOR) {
        sdfPadSetAct0(0, level);
    } else {
        sdfPadSetAct1(0, dds3PadVibrationState[motor]);
    }
}

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void kwlnPadResetMotorLevelsAndOutput(void) {
    dds3PadVibrationState[KWLN_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_LARGE_MOTOR] = 0;
    sdfPadSetAct(0, 0, 0);
}


void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = KWLN_PAD_LARGE_MOTOR;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = D_00438D98;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < KWLN_PAD_MOTOR_COUNT; motor++) {
        if (D_00438DA0[motor] != 0) {
            D_00438DA0[motor]--;
            if (D_00438DA0[motor] == 0) {
                levels[motor] = 0;
                if (motor == KWLN_PAD_SMALL_MOTOR) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + KWLN_PAD_LARGE_MOTOR;
                if (*largeLevel != targets[KWLN_PAD_LARGE_MOTOR]) {
                    kwlnPadStepLargeMotorLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}

s32 kwlnTaskNoopCallback(dds3ProcessID_t *task) {
    return 0;
}

/* Seed the shared effect random state with the fixed initialization seed. */
void dds3InitRandCB(void) {
    sdfRandomize(effSharedRandomState, KWLN_SHARED_RANDOM_SEED);
}

extern s16 D_00435BA8;
extern u16 D_00438A1E;
extern u16 D_00435C50;
extern u16 D_00435C52;
extern u16 D_00435C54;
extern u16 D_00435C56;
extern char D_00435C58[];

s32 kwlnDebugGraphUpdate(dds3ProcessID_t *task) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfPrimTransPacket_t *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: kwlnDebugGraphUpdate". */
    s32 pixels;

    sdfDrawTagInit(packetList);
    blendPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(blendPacket);
    sdfDrawTagLink(packetList, (u32)blendPacket);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x70D0, 0x85A8,
                    0xFFFF7F, 0x1260, 0xF0, 0x20000000, 0x40806040));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x85C0, 0xFFFF80, 0, D_00435C58, (u16)D_00435BA8));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x8620, 0xFFFF80, 0, D_00435C58, D_00438A1E));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x85C0, 0xFFFF80, 6, D_00435C58, D_00435C50));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x8620, 0xFFFF80, 6, D_00435C58, D_00435C52));
    pixels = (s32)((f32)(u16)D_00435BA8 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00438A1E / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x80000080, 0, 0x73A0, 0x8650,
                    originX + ((pixels + 0x2A) << 4),
                    0x8660, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C50 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x85D0,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8610, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C52 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x8630,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8670, 0xFF0000, NULL);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, packetList);
    if (D_00435C54 >= 0x47 || D_00435C50 < (u16)D_00435BA8) {
        D_00435C50 = D_00435BA8;
        D_00435C54 = 0;
    } else {
        D_00435C54++;
    }
    if (D_00435C56 >= 0x47 || D_00435C52 < D_00438A1E) {
        D_00435C52 = D_00438A1E;
        D_00435C56 = 0;
    } else {
        D_00435C56++;
    }
    return 0;
}

void kwlnDebugGraphTaskDestroy(dds3ProcessID_t *task) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void kwlnDebugGraphSetEnabled(s8 enabled) {
    if (enabled == 1) {
        kwlnDebugGraphTask = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, kwlnDebugGraphUpdate, kwlnDebugGraphTaskDestroy, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(kwlnDebugGraphTask, 0);
    }
}

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void kwlnDrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image) {
    s32 width = image->width;
    s32 height = image->height;
    s32 drawWidth;
    s32 drawHeight;

    sdf2DLinkTexPacket(packetList, image, SDF_GS_CONTEXT_1);
    drawWidth = width * 0x10;
    drawHeight = height * 8;
    if (width < height) {
        if (height > KWLN_PREVIEW_MAX_TEXELS) {
            drawHeight = 0x800;
            drawWidth = (width << 8) / height * 0x10;
        }
    } else if (width > KWLN_PREVIEW_MAX_TEXELS) {
        drawWidth = 0x1000;
        drawHeight = (height << 8) / width * 8;
    }
    if (width == KWLN_PREVIEW_WRAP_TEXELS) {
        width--;
    }
    if (height == KWLN_PREVIEW_WRAP_TEXELS) {
        height--;
    }
    sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7A60, 0, 0, drawWidth + 0x7180, drawHeight + 0x7A60,
                                width * 0x10, height * 0x10, KWLN_DIAG_DEPTH, 0);
}

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void kwlnTextureDrawPageCounter(sdfDrawTag_t *packetList) {
    char pageText[0x70];
    s32 pageIndex = kwlnTextureGetPageIndex();
    s32 resourceCount = kwlnTextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x79C0, KWLN_DIAG_DEPTH, 0, pageText));
}

extern const char *D_0037F560[];
extern char D_004112F0[];
extern char D_00411308[];

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

void kwlnDrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture) {
    char text[0x70];
    s32 formatIndex = 0;
    s32 formatMode = 0;

    sprintf(text, "W:%4d H:%4d", texture->width, texture->height);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x82A0, 0x0FFFFF80, 0, text));
    switch (texture->pixelFormat) {
    case 0: formatIndex = 0; formatMode = 6; break;
    case 1: formatIndex = 1; formatMode = 6; break;
    case 2: formatIndex = 2; formatMode = 6; break;
    case 10: formatIndex = 3; formatMode = 6; break;
    case 19: formatIndex = 4; formatMode = 4; break;
    case 20: formatIndex = 5; formatMode = 5; break;
    case 27: formatIndex = 6; formatMode = 4; break;
    case 36: formatIndex = 7; formatMode = 5; break;
    case 44: formatIndex = 8; formatMode = 5; break;
    default: break;
    }
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x8300, 0x0FFFFF80,
                                                                formatMode, D_0037F560[formatIndex]));
    sprintf(text, "ID[%2d,%2d]", texture->resourceKey, texture->battleTextureSlot);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
}

#define KWLN_DIAG_PACKET_LIST_BYTES 0x20
#define KWLN_VIEWER_FAST_PAGE_STEP 10
#define KWLN_MAP_ROW_UNITS 0x1000
#define KWLN_MAP_ROW_SHIFT 12
#define KWLN_MAP_ROW_MASK 0xFFF
#define KWLN_MAP_COLUMN_SHIFT 6
#define KWLN_MAP_COLUMN_UNITS 64
#define KWLN_MAP_COLUMN_COUNT 64
#define KWLN_MAP_ROW_SPAN 0x400
#define KWLN_MAP_BORDER_WIDTH 0x440
#define KWLN_MAP_CELL_X_STEP 16
#define KWLN_MAP_ROW_Y_STEP 8
#define KWLN_MAP_CELL_X_SHIFT 4
#define KWLN_MAP_GREEN 0x80008000
#define KWLN_MAP_YELLOW 0x80008080
#define KWLN_MAP_RED 0x80000080
#define KWLN_MAP_BLUE 0x80800000
#define KWLN_MAP_CYAN 0x80808000
#define KWLN_HELD_TEXTURE_CLAMP_MODE 5

/* Count texture entries without a reference clone source. */
s32 kwlnTextureCountIncompleteResources(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 incompleteCount = 0;
    while (texture != NULL) {
        while (texture->reference != NULL && texture->reference->cloneSource != NULL) {
            previousTexture = texture->prev;
            if (previousTexture == NULL) {
                return incompleteCount;
            }
            texture = previousTexture;
        }
        incompleteCount++;
        texture = texture->prev;
    }
    return incompleteCount;
}

u32 kwlnTextureGetPageIndex(void) {
    return kwlnTextureViewerPageIndex;
}

/* DDS2 first skips clone-source entries at the head, even for requested index zero.
 * This assumes a nonempty list; initial exhaustion returns 0 before selection writes. */
s32 kwlnSelectIncompleteResourceAtPageIndex(s32 requestedIndex) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 visitedIndex = 0;

    while (texture->reference != NULL && texture->reference->cloneSource != NULL) {
        previousTexture = texture->prev;
        if (previousTexture == NULL) {
            return 0;
        }
        texture = previousTexture;
    }
    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (texture != NULL && visitedIndex != requestedIndex) {
        kwlnCurrentIncompleteResource = texture;
        while (texture->reference != NULL && texture->reference->cloneSource != NULL) {
            previousTexture = texture->prev;
            if (previousTexture == NULL) {
                return visitedIndex;
            }
            texture = previousTexture;
        }
        previousTexture = texture->prev;
        if (previousTexture == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        texture = previousTexture;
    }
    kwlnCurrentIncompleteResource = texture;
    return visitedIndex;
}

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 kwlnTextureViewerHandlePad(void) {
    if (dds3PadBitTable[0x13] < 0) {
        return 0;
    }
    if (dds3PadBitTable[0x14] & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex -= 1;
    } else if (dds3PadBitTable[0x15] & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex += 1;
    } else if ((dds3PadBitTable[0x18] & KWLN_PAD_REPEAT_BIT) || (dds3PadBitTable[0x19] & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex -= KWLN_VIEWER_FAST_PAGE_STEP;
    } else if ((dds3PadBitTable[0x1A] & KWLN_PAD_REPEAT_BIT) || (dds3PadBitTable[0x1B] & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex += KWLN_VIEWER_FAST_PAGE_STEP;
    }
    kwlnTextureViewerPageIndex = kwlnSelectIncompleteResourceAtPageIndex(kwlnTextureViewerPageIndex);
    if (dds3PadBitTable[0x12] < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

extern s32 D_00435CB0;

void kwlnDrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

void kwlnDrawTextureViewerFrame(SdfPoolNode *surface, sdfTexHandle_t *image) {
    sdfDrawTag_t *packetList;
    s32 counter;
    s32 mode;

    if (image == NULL) {
        return;
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    kwlnTextureDrawPageCounter(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    kwlnDrawImageOutline(packetList, image);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
    kwlnDrawTextureResourceDetails(packetList, image);

    if (D_00435C60 != 0) {
        kwlnDrawTextureListDiagnostic(packetList, 0x8240, 0x7A60);
        counter = D_00435CB0;
        mode = (counter >> 4) & 1;
        if (image->primaryResource != NULL) {
            kwlnDrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->primaryResource, mode);
        }
        counter = D_00435CB0;
        /* Boolean highlight selection; see docs/idioms.md, "Trace-justified boolean extraction: texture viewer highlight". */
        mode = (counter & 0x10) ? 2 : 0;
        if (image->secondaryResource != NULL) {
            kwlnDrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->secondaryResource, mode);
        }
        D_00435CB0++;
    }
    surface->append(surface, packetList);
}

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 kwlnSwapActiveResource(SdfPoolNode *surface) {
    s32 inputStatus;

    if (sdfTexHandleLink == 0) {
        return 0;
    }
    inputStatus = kwlnTextureViewerHandlePad();
    kwlnDrawTextureViewerFrame(surface, kwlnCurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 kwlnLoadDefaultResource(void) {
    if (sdfTexHandleLink == 0) {
        return 0;
    }
    if (kwlnTextureViewerHandlePad() == 0) {
        return -1;
    }
    kwlnDrawTextureViewerFrame(&kwlnPositionedTextSurface, kwlnCurrentIncompleteResource);
    return 0;
}

/* Initialize viewer state and return its default renderer for the first eligible
 * node. Exhausting a nonempty list returns NULL after the initial state writes. */
s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    if (texture == NULL) {
        return NULL;
    }
    kwlnCurrentIncompleteResource = texture;
    D_00435C60 = 1;
    kwlnTextureViewerPageIndex = 0;
    while (texture->reference != NULL && texture->reference->cloneSource != NULL) {
        texture = kwlnCurrentIncompleteResource->prev;
        if (texture == NULL) {
            return NULL;
        }
        kwlnCurrentIncompleteResource = texture;
    }
    return kwlnLoadDefaultResource;
}

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void kwlnDrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch ((s32)block->allocationMode) {
        case 1:
            color = KWLN_MAP_GREEN;
            break;
        case 2:
            color = KWLN_MAP_YELLOW;
            break;
        case 3:
            color = KWLN_MAP_RED;
            break;
        default:
            color = 0;
            break;
        }
    } else {
        color = mode == 1 ? KWLN_MAP_BLUE : KWLN_MAP_CYAN;
    }
    remainingUnits = block->size;
    topY = y + (block->word >> KWLN_MAP_ROW_SHIFT) * KWLN_MAP_ROW_Y_STEP;
    columnIndex = (block->word & KWLN_MAP_ROW_MASK) >> KWLN_MAP_COLUMN_SHIFT;
    leftX = x + columnIndex * KWLN_MAP_CELL_X_STEP;
    if (columnIndex > 0) {
        rowUnits = (KWLN_MAP_COLUMN_COUNT - columnIndex) * KWLN_MAP_COLUMN_UNITS;
        if (remainingUnits >= rowUnits) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + (KWLN_MAP_COLUMN_COUNT - columnIndex) * KWLN_MAP_CELL_X_STEP, topY + KWLN_MAP_ROW_Y_STEP,
                                         KWLN_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> KWLN_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * KWLN_MAP_CELL_X_STEP, topY + KWLN_MAP_ROW_Y_STEP,
                                         KWLN_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += KWLN_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= KWLN_MAP_ROW_UNITS) {
        sdfPrimLinkLine(packetList, color, 0, x, topY, x + KWLN_MAP_ROW_SPAN, topY + KWLN_MAP_ROW_Y_STEP,
                                     KWLN_DIAG_DEPTH, NULL);
        remainingUnits -= KWLN_MAP_ROW_UNITS;
        topY += KWLN_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> KWLN_MAP_COLUMN_SHIFT) << KWLN_MAP_CELL_X_SHIFT), topY + KWLN_MAP_ROW_Y_STEP,
                                     KWLN_DIAG_DEPTH, NULL);
    }
}


/* Draw the allocation-map border, then walk blocks through their prev links. */
void kwlnDrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfGsMemSlice_t *block = sdfGsMemGetLinkHead();

    if (block != NULL) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, KWLN_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            kwlnDrawTextureAllocationMapBlock(packetList, x, y, block, 0);
            block = block->prev;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void kwlnTextureAttachTask(SdfPoolNode *surface) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(KWLN_DIAG_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    kwlnDrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    surface->append(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 kwlnEnsureDefaultResource(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(&kwlnPositionedTextSurface);
    return 0;
}

/* Two fixed frame banks: thirteen draw groups and their scene/overlay packets. */
#include "kwln_frame_draw_bank.h"
typedef char KwlnFrameDrawBank_size_check[sizeof(KwlnFrameDrawBank) == 0x1F40 ? 1 : -1];

extern KwlnFrameDrawBank D_00380870[2];
extern u32 D_00435CBC;
extern sdfGrFrameSet_t D_0040B290;
extern SdfLightSources D_0037F770;
extern f32 kwlnDefaultColorVector[4];
extern Matrix4 sdfViewMatrix;
extern Matrix4 D_00384790;
extern SdfLightSources kwlnOverlayLightSources;
extern f32 kwlnOverlayAmbientColor[4];
extern void sdf3DLinkInitPacket(sdfDrawTag_t *, DmaPacketHeader *);

/* Initialize the selected frame bank's program, scene and overlay packet chains. */
void kwlnInitializeFrameDrawPackets(s32 bufferIndex) {
    sdfDrawTagInit(&D_00380870[bufferIndex].initialList);
    sdf3DLinkInitPacket(&D_00380870[bufferIndex].initialList,
                                        &D_00380870[bufferIndex].programReference);
    sdfDrawTagInit(&D_00380870[bufferIndex].sceneList);
    sdfPreDrawTagInit(&D_00380870[bufferIndex].linkedList);
    sdfGrSetStdDrawEnv(&D_00380870[bufferIndex].scene, &D_0040B290);
    /* Set the RGBAQ color word while retaining its floating-point Q word. */
    *(u32 *)&D_00380870[bufferIndex].scene.centeredBounds.rgbaq.value = D_00435CBC;
    if (dds3DrawFlag & 0x10000000) {
        sdfGrSetClearPacket(&D_00380870[bufferIndex].scene.centeredBounds, 0, 0,
                                         D_0040B290.bufferFormat,
                                         D_0040B290.auxiliaryFormat);
        dds3DrawFlag &= 0xEFFFFFFF;
    }
    sdfGrLinkStdDrawEnv(&D_00380870[bufferIndex].sceneList,
                                 &D_00380870[bufferIndex].linkedList,
                                 &D_00380870[bufferIndex].scene.link);
    sdf3DMakeViewportPacket(&D_00380870[bufferIndex].sceneMatrix,
                             &dds3ScreenParam, &sdfViewMatrix);
    sdfDrawTagLink(&D_00380870[bufferIndex].sceneList, (u32)&D_00380870[bufferIndex].sceneMatrix);
    sdf3DMakeLightParallelPacket(&D_00380870[bufferIndex].scenePacketStorage, D_0037F770, kwlnDefaultColorVector);
    sdfDrawTagLink(&D_00380870[bufferIndex].sceneList, (u32)&D_00380870[bufferIndex].scenePacketStorage);

    sdfDrawTagInit(&D_00380870[bufferIndex].thirdList);
    sdf3DMakeViewportPacket(&D_00380870[bufferIndex].thirdMatrix, &D_0037F980, &D_00384790);
    sdfDrawTagLink(&D_00380870[bufferIndex].thirdList, (u32)&D_00380870[bufferIndex].thirdMatrix);
    sdf3DMakeLightParallelPacket(&D_00380870[bufferIndex].overlayLightPacket, kwlnOverlayLightSources, kwlnOverlayAmbientColor);
    sdfDrawTagLink(&D_00380870[bufferIndex].thirdList, (u32)&D_00380870[bufferIndex].overlayLightPacket);
}

extern void sdfGrSetScreen(s32 displayModeIndex);
extern void kwlnInitializeFrameDrawPackets(s32 bufferIndex);
extern void dds3InitZClearDrawTag(void);
extern u8 D_00435BC8;
extern void eglInit(void);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void dds3InitDrawEnv(void) {
    sdfGrSetScreen(1);
    sdfCameraBuildProjection(&dds3ScreenParam.camera);
    sdfCameraBuildProjection(&D_0037F980.camera);
    kwlnInitializeFrameDrawPackets(0);
    kwlnInitializeFrameDrawPackets(1);
    dds3InitZClearDrawTag();
    D_00435BC8 = 0;
    eglInit();
}


/* One 0x1F40-byte draw bank of the held-texture render target. */
#include "kwln_texture_bank.h"

extern sdfGrFrameSet_t dds3RenderTexId;
extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern u32 D_00435CD0;
extern SdfLightSources kwlnHeldTextureLightSources;
extern f32 kwlnHeldTextureAmbientColor[4];
extern Matrix4 kwlnHeldTextureViewMatrix;

/* Rebuild the held-texture render target's view and both frame banks. */
void dds3InitRenderDrawEnv(void) {
    s32 i;

    dds3RenderTexId.width = D_00435CCC;
    dds3RenderTexId.unk2 = D_00435CCE;
    dds3RenderTexId.height = D_00435CCE;
    dds3RenderTexId.bufferFormat = 0;
    dds3RenderTexId.auxiliaryFormat = 0x30;
    dds3RenderTexId.buffers[0] = D_00435CC4;
    dds3RenderTexId.buffers[1] = 0;
    dds3RenderTexId.buffers[2] = D_0040B290.buffers[2];
    sdfCameraBuildProjection(&D_0037F7B0.camera);
    for (i = 0; i != 2; i++) {
        sdfDrawTagInit(&D_00380870[i].heldTexture.list);
        sdfGrSetDrawEnv(&D_00380870[i].heldTexture.scene, &dds3RenderTexId, 0);
        *(u32 *)&D_00380870[i].heldTexture.scene.centeredBounds.rgbaq.value = D_00435CD0;
        sdfDrawTagLink(&D_00380870[i].heldTexture.list, (u32)&D_00380870[i].heldTexture.scene);
        sdf3DMakeViewportPacket(&D_00380870[i].heldTexture.matrix, &D_0037F7B0, &kwlnHeldTextureViewMatrix);
        sdfDrawTagLink(&D_00380870[i].heldTexture.list, (u32)&D_00380870[i].heldTexture.matrix);
        sdf3DMakeLightParallelPacket(&D_00380870[i].heldTexture.lighting, kwlnHeldTextureLightSources, kwlnHeldTextureAmbientColor);
        sdfDrawTagLink(&D_00380870[i].heldTexture.list, (u32)&D_00380870[i].heldTexture.lighting);
    }
}

extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern void dds3FreeRenderTexture(void);
extern void dds3InitRenderDrawEnv(void);
extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
extern sdfTexHandle_t *sdfTexCreateResourceWithReference(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary);
/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 dds3AllocRenderTexture(u16 width, u16 height, f32 aspect) {
    sdfTexHandle_t *texture;

    if (dds3RenderTexture != 0) {
        dds3FreeRenderTexture();
    }
    dds3RenderTex = 0;
    D_00435CCC = width;
    D_00435CCE = height;
    D_00435CC4 = sdfGsAllocFrameBuff(width, height, 0);
    if (D_00435CC4 == 0) {
        return 0;
    }
    texture = sdfTexCreateResourceWithReference(width, height, 0, 0, D_00435CC4, 0, 0, 0);
    if (texture == 0) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, KWLN_HELD_TEXTURE_CLAMP_MODE);
    sdfTexMakeGIFPacket(texture);
    D_0037F7B0.camera.aspect = aspect;
    D_0037F7B0.camera.width = width;
    D_0037F7B0.camera.height = height;
    dds3InitRenderDrawEnv();
    return 1;
}

/* Release the held texture if present and clear tracking; no separate buffer free. */
void dds3FreeRenderTexture(void) {
    if (dds3RenderTexture != 0) {
        sdfTexReleaseReference(dds3RenderTexture);
    }
    dds3RenderTexture = 0;
    D_00435CC4 = 0;
    dds3RenderTex = 0;
}

/* Set the flag and return 1 when a held handle exists; otherwise leave it and return 0. */
#include "kwln_texture_reference_flag_api.h"

s32 dds3RenderTextureON(void) {
    s32 result = 0;

    if (dds3RenderTexture == 0) {
        return result;
    }
    dds3RenderTex = 1;
    return 1;
}

void dds3RenderTextureOFF(void) {
    dds3RenderTex = 0;
}

u32 dds3CheckRenderTexture(void) {
    return dds3RenderTex;
}

sdfTexHandle_t *dds3GetRenderTexture(void) {
    return dds3RenderTexture;
}

extern f32 sdfViewShakeVector[];
extern f32 D_00435D10[];
extern f32 fabsf(f32 value);

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void kwlnAdvanceShakeOffsets(void) {
    f32 limits[2];
    f32 weights[2];
    s32 axis;

    memcpy(weights, D_00435D10, sizeof(weights));
    if (D_00435CE8 == 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        SDF_VU0_VECTOR_W_SET_ZERO();
        return;
    }
    if (D_00435CEA == 0) {
        D_00435CE8 = 0;
        return;
    }
    limits[0] = D_00435CEC * 0.01f;
    limits[1] = D_00435CEE * 0.01f;
    for (axis = 0; axis < 2; axis++) {
        if (limits[axis] != 0.0f) {
            f32 step = dds3GetRandFloatA(0) * limits[axis] * 0.5f + limits[axis] * 0.5f;

            if (limits[axis] * weights[axis] <= fabsf(sdfViewShakeVector[axis])) {
                if (sdfViewShakeVector[axis] <= 0.0f) {
                    sdfViewShakeVector[axis] += step;
                } else {
                    sdfViewShakeVector[axis] -= step;
                }
            } else {
                sdfViewShakeVector[axis] += 2.0f * (step * (dds3GetRandFloatA(0) - 0.5f));
            }
            if (sdfViewShakeVector[axis] < -limits[axis]) {
                sdfViewShakeVector[axis] = -limits[axis];
            }
            if (limits[axis] < sdfViewShakeVector[axis]) {
                sdfViewShakeVector[axis] = limits[axis];
            }
        }
    }
    if (D_00435CEA > 0) {
        s32 divisor = D_00435CEA + 5;

        D_00435CEC -= D_00435CEC / divisor;
        D_00435CEE -= D_00435CEE / divisor;
        D_00435CEA--;
    }
}

#define KWLN_FADE_TICKS_PER_FRAME 10
#define KWLN_FADE_MARK_NUMERATOR 4
#define KWLN_FADE_COUNT_DOWN_BIT 1
#define KWLN_FADE_COUNT_UP_BIT 2
#define KWLN_FADE_DIRECTION_BITS 3
#define KWLN_FADE_MAX_ALPHA 0x80
#define KWLN_FADE_ALPHA_SHIFT 7
#define KWLN_COLOR_ALPHA_CHANNEL 3
#define KWLN_BGFADE_COUNT_DOWN_BIT 0x04000000
#define KWLN_BGFADE_COUNT_UP_BIT 0x08000000
#define KWLN_BGFADE_DIRECTION_BITS 0x0C000000
#define KWLN_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF
#define KWLN_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF
#define KWLN_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF
#define KWLN_BGFADE_FIRST_RAMP_MAX 0x31
#define KWLN_BGFADE_SECOND_RAMP_MAX 0x4F
#define KWLN_BGFADE_FIRST_RAMP_SCALE 49.0f
#define KWLN_BGFADE_SECOND_RAMP_SCALE 79.0f

/* Store tenths in the 16-bit duration before deriving its forty-percent mark.
 * Zero frames only disables the configuration; mode zero stores the -1 sentinel. */
void dds3SetQuake(s32 fadeMode, s32 frames) {
    if (frames == 0) {
        D_00435CE8 = 0;
        return;
    }
    D_00435CE8 = 1;
    if (fadeMode == 0) {
        D_00435CEA = -1;
    } else {
        D_00435CEA = fadeMode;
    }
    D_00435CEE = frames * KWLN_FADE_TICKS_PER_FRAME;
    D_00435CEC = D_00435CEE * KWLN_FADE_MARK_NUMERATOR / KWLN_FADE_TICKS_PER_FRAME;
}

/* Disable the configured fade without clearing its stored timing parameters. */
void dds3StopQuake(void) {
    D_00435CE8 = 0;
}

/* Fill each frame state's z-clear packet for both draw buffers. */
void dds3InitZClearDrawTag(void) {
    s32 state;
    s32 buffer;
    u64 topLeft;
    u64 bottomRight;

    for (state = 0; state < 3; state++) {
        for (buffer = 0; buffer < 2; buffer++) {
            dds3ZClearDrawTag[state][buffer].dma.quadwordCount = 10;
            dds3ZClearDrawTag[state][buffer].dma.reservedByte = 0;
            dds3ZClearDrawTag[state][buffer].dma.control = 0x70;
            dds3ZClearDrawTag[state][buffer].dma.address = 0;
            dds3ZClearDrawTag[state][buffer].dma.firstVifCode = 0;
            dds3ZClearDrawTag[state][buffer].dma.secondVifCode = 0x5000000A;
            dds3ZClearDrawTag[state][buffer].testGifTag = 0x1000000000000001;
            dds3ZClearDrawTag[state][buffer].testGifRegisters = 0xE;
            dds3ZClearDrawTag[state][buffer].clearTest.registerId = 0x47;
            dds3ZClearDrawTag[state][buffer].clearTest.value = 0x32001;
            dds3ZClearDrawTag[state][buffer].clearGifTag = 0x1003400000000004;
            dds3ZClearDrawTag[state][buffer].clearGifRegisters = 0xE;
            topLeft = 0x79007000;
            dds3ZClearDrawTag[state][buffer].topLeftColor.registerId = 1;
            dds3ZClearDrawTag[state][buffer].topLeftColor.value = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].topLeftPosition.registerId = 5;
            dds3ZClearDrawTag[state][buffer].topLeftPosition.value = topLeft;
            bottomRight = 0x87009000;
            dds3ZClearDrawTag[state][buffer].bottomRightColor.registerId = 1;
            dds3ZClearDrawTag[state][buffer].bottomRightColor.value = 0x3F80000000000000;
            dds3ZClearDrawTag[state][buffer].bottomRightPosition.registerId = 5;
            dds3ZClearDrawTag[state][buffer].bottomRightPosition.value = bottomRight;
            dds3ZClearDrawTag[state][buffer].blendGifTag = 0x1000000000008002;
            dds3ZClearDrawTag[state][buffer].blendGifRegisters = 0xE;
            dds3ZClearDrawTag[state][buffer].blend.test.registerId = 0x47;
            dds3ZClearDrawTag[state][buffer].blend.test.value = 0x5101B;
            dds3ZClearDrawTag[state][buffer].blend.alpha.registerId = 0x42;
            dds3ZClearDrawTag[state][buffer].blend.alpha.value = 0x44;
        }
    }
}

/* Append the selected state's packet for the current draw buffer to a new list. */
sdfDrawTag_t *dds3LinkZClearDrawTag(s32 stateIndex) {
    sdfDrawTag_t *packetList = dds3FontReqAllocDrawTag();

    sdfDrawTagLink(packetList, (u32)&dds3ZClearDrawTag[stateIndex][dds3GetFrameID()]);
    return packetList;
}

/* Cancel the foreground direction and zero RGBA; timing words are unchanged. */
void dds3FadeClear(void) {
    dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
    fadergba.r = 0;
    fadergba.g = 0;
    fadergba.b = 0;
    fadergba.a = 0;
}

/* Store RGBA and cancel foreground direction flags without changing timing. */
void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha) {
    dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = alpha;
}

/* Return the live color address through outColor, rather than copying channels. */
void kwlnFadeGetColor(KwlnFadeColor **outColor) {
    *outColor = &fadergba;
}

/* Replace RGB only; alpha, direction flags and timing are preserved. */
void kwlnFadeSetRGB(s8 red, s8 green, s8 blue) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
}

/* Start the countdown with caller RGB. A zero duration also clears RGB via FadeClear.
 * The signed alpha byte -0x80 carries the raw maximum-alpha value 0x80. */
void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = -KWLN_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | KWLN_FADE_COUNT_DOWN_BIT) & ~KWLN_FADE_COUNT_UP_BIT;
}

/* Start the same countdown using existing RGB; zero duration preserves RGB. */
void dds3FadeIn2(s32 duration) {
    fadergba.a = -KWLN_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | KWLN_FADE_COUNT_DOWN_BIT) & ~KWLN_FADE_COUNT_UP_BIT;
}

/* Start the count-up with caller RGB; zero duration leaves maximum alpha. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration) {
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = 0;
    if (duration == 0) {
        fadenow = 0;
        fademax = 0;
        fadergba.a = -KWLN_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
}

/* Start the same count-up using existing RGB; zero duration preserves RGB. */
void dds3FadeOut2(s32 duration) {
    fadergba.a = 0;
    if (duration == 0) {
        fadenow = 0;
        fadergba.a = -KWLN_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
}

/* Return whether either foreground counter direction is enabled. */
s32 dds3FadeCheck(void) {
    return (dds3DrawFlag & KWLN_FADE_DIRECTION_BITS) != 0;
}

/* Step the u16 counter and sample alpha before testing exact completion.
 * Completion clears timing/flags but leaves the sampled endpoint alpha intact. */
void dds3FadeUpdate(void) {
    if (dds3FadeCheck() != 0) {
        if (dds3DrawFlag & KWLN_FADE_COUNT_DOWN_BIT) {
            fadenow -= 1;
        } else {
            fadenow += 1;
        }
        fadergba.a = (fadenow << KWLN_FADE_ALPHA_SHIFT) / fademax;
        if (((dds3DrawFlag & KWLN_FADE_COUNT_DOWN_BIT) && fadenow == 0) || ((dds3DrawFlag & KWLN_FADE_COUNT_UP_BIT) && fadenow == fademax)) {
            fadenow = 0;
            fademax = 0;
            dds3DrawFlag &= ~KWLN_FADE_DIRECTION_BITS;
        }
    }
}

/* Submit diagnostics when enabled and either counter is nonzero; only positive
 * counters produce text, so negative-only errors still submit an empty list. */
void dds3DrawDistRipOverError(void) {
    sdfDrawTag_t *packetList;

    if (D_00435BA4 != 0) {
        if (distblurovercnt != 0 || ripblurovercnt != 0) {
            packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(KWLN_DIAG_PACKET_LIST_BYTES);
            sdfDrawTagInit(packetList);
            if (distblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, D_004112F0, distblurovercnt));
            }
            if (ripblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7B40, 0xFEFFFF, 4, D_00411308, ripblurovercnt));
            }
            D_00380708.append(&D_00380708, packetList);
        }
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf.h"
#include "sdf_gs_packet.h"
#include "sdf_packet_list.h"
#include "kwln_fade_color.h"
#include "kwln_draw_buffer.h"

extern s16 kwlnBackgroundFadeFirstRamp; /* the background fade's band ramps */
extern s16 kwlnBackgroundFadeSecondRamp;
extern u8 kwlnBackgroundFadeOverlayPackets[]; /* the overlay packet, one per draw buffer */
extern SdfPoolNode sdfGrOtTable[];
extern s32 kwlnFadeIsBackgroundOverlayActive(void);

#define KWLN_FADE_OVERLAY_SURFACE 92
#define KWLN_FADE_MAX_ALPHA 0x80
#define KWLN_GS_PABE 0x49
#define KWLN_GS_FBA_1 0x4A
#define KWLN_OVERLAY_TEST 0x31003       /* alpha and depth tests always pass */
#define KWLN_OVERLAY_ALPHA 0x01         /* (Cd - Cs) * As / 128 + Cs */
#define KWLN_OVERLAY_DEPTH 0xFEFFFFULL
#define KWLN_OVERLAY_LEFT 0x7000
#define KWLN_OVERLAY_RIGHT 0x9000
#define KWLN_OVERLAY_TOP 0x7900
#define KWLN_OVERLAY_BOTTOM 0x8700

/* An end tag carrying VIF DIRECT of ten quadwords: four A+D register writes,
 * then a blended sprite of two vertices. */
typedef struct KwlnFadeOverlayPacket {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite alpha;
    SdfGsRegisterWrite pabe;
    SdfGsRegisterWrite fba;
    SdfGsRegisterWrite test;
    u64 spriteTag;
    u64 spriteRegisters;
    SdfGsRegisterWrite vertices[4];
} KwlnFadeOverlayPacket;

typedef char KwlnFadeOverlayPacket_size_must_be_0xB0[(sizeof(KwlnFadeOverlayPacket) == 0xB0) ? 1 : -1];

static void kwlnWriteRegister(SdfGsRegisterWrite *write, u64 value, u64 registerId) {
    write->value = value;
    write->registerId = registerId;
}

static u64 kwlnOverlayVertex(u32 x, u32 y) {
    return (KWLN_OVERLAY_DEPTH << 32) | (u32)(y << 16) | x;
}

/* While the fade colour has any alpha, cover the screen with it: the sprite
 * keeps (128 - alpha) / 128 of what is drawn. An active background overlay
 * narrows it to the rows between its two bands. Upstream: INCLUDE_ASM in
 * both games; same packet as the retail routine. */
void kwlnQueueBackgroundFadeOverlayPacket(void) {
    u32 buffer = dds3GetFrameID();
    s32 overlayAlpha = KWLN_FADE_MAX_ALPHA - fadergba.a;
    KwlnFadeOverlayPacket *packet;
    u32 top = KWLN_OVERLAY_TOP;
    u32 bottom = KWLN_OVERLAY_BOTTOM;
    u64 color;
    sdfDrawTag_t *packets;
    SdfPoolNode *surface;

    if (overlayAlpha >= KWLN_FADE_MAX_ALPHA) {
        return;
    }
    packet = (KwlnFadeOverlayPacket *)(kwlnBackgroundFadeOverlayPackets + buffer * sizeof(KwlnFadeOverlayPacket));
    packet->header.dmaTag = 0x7000000A;              /* end, ten quadwords */
    packet->header.vifCommands = 0x5000000AULL << 32; /* NOP; DIRECT ten quadwords */
    packet->header.gifTag = 0x1000000000000004ULL;    /* NLOOP 4, NREG 1 */
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    kwlnWriteRegister(&packet->alpha, KWLN_OVERLAY_ALPHA, SDF_GS_ALPHA_1);
    kwlnWriteRegister(&packet->pabe, 0, KWLN_GS_PABE);
    kwlnWriteRegister(&packet->fba, 0, KWLN_GS_FBA_1);
    kwlnWriteRegister(&packet->test, KWLN_OVERLAY_TEST, SDF_GS_TEST_1);
    packet->spriteTag = 0x1023400000008004ULL; /* NLOOP 4, EOP, PRE, PRIM blended sprite, NREG 1 */
    packet->spriteRegisters = SDF_GIF_REGISTER_AD;

    if (kwlnFadeIsBackgroundOverlayActive()) {
        top = 0x7A88 - kwlnBackgroundFadeFirstRamp * 8;
        bottom = 0x8488 + kwlnBackgroundFadeSecondRamp * 8;
    }
    /* Retail shifts the alpha in 32 bits, sign-extending a negative one (a
     * fade alpha above 0x80) over Q; the sprite is untextured, so Q goes
     * unread, but the packet keeps retail's bits. */
    color = 0x3F80000000000000ULL /* Q = 1.0 */ | (u64)(s64)(s32)((u32)overlayAlpha << 24) |
            ((u64)fadergba.b << 16) | ((u64)fadergba.g << 8) | fadergba.r;
    kwlnWriteRegister(&packet->vertices[0], color, SDF_GS_RGBAQ);
    kwlnWriteRegister(&packet->vertices[1], kwlnOverlayVertex(KWLN_OVERLAY_LEFT, top), SDF_GS_XYZ2);
    kwlnWriteRegister(&packet->vertices[2], color, SDF_GS_RGBAQ);
    kwlnWriteRegister(&packet->vertices[3], kwlnOverlayVertex(KWLN_OVERLAY_RIGHT, bottom), SDF_GS_XYZ2);

    packets = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(packets, (u32)packet);
    surface = &sdfGrOtTable[KWLN_FADE_OVERLAY_SURFACE];
    surface->append(surface, packets);
}
#else
INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnQueueBackgroundFadeOverlayPacket);
#endif

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void kwlnFadeResetBackground(void) {
    dds3DrawFlag &= KWLN_BGFADE_KEEP_OTHER_BITS;
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = 0;
    kwlnBackgroundFadeFirstRamp = KWLN_BGFADE_FIRST_RAMP_MAX;
    kwlnBackgroundFadeSecondRamp = KWLN_BGFADE_SECOND_RAMP_MAX;
}

/* Start the background countdown; zero duration performs the full background reset. */
void kwlnFadeBackgroundStartOut(s32 duration) {
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = KWLN_FADE_MAX_ALPHA;
    if (duration == 0) {
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = 0;
        kwlnBackgroundFadeCounter = 0;
        kwlnBackgroundFadeDuration = 0;
        kwlnBackgroundFadeFirstRamp = KWLN_BGFADE_FIRST_RAMP_MAX;
        kwlnBackgroundFadeSecondRamp = KWLN_BGFADE_SECOND_RAMP_MAX;
        kwlnFadeResetBackground();
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = duration;
        dds3DrawFlag = (dds3DrawFlag | KWLN_BGFADE_COUNT_DOWN_BIT) & KWLN_BGFADE_CLEAR_UP_MASK;
    }
    D_0037F5EC[2] = 2048.0f;
}

/* Start the background count-up; zero duration leaves max alpha and zero ramps. */
void kwlnFadeBackgroundStartIn(s32 duration) {
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = 0;
    if (duration == 0) {
        kwlnBackgroundFadeCounter = 0;
        kwlnBackgroundFadeDuration = 0;
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = KWLN_FADE_MAX_ALPHA;
        dds3DrawFlag &= KWLN_BGFADE_KEEP_OTHER_BITS;
        kwlnBackgroundFadeFirstRamp = 0;
        kwlnBackgroundFadeSecondRamp = 0;
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = 0;
        dds3DrawFlag = (dds3DrawFlag & KWLN_BGFADE_CLEAR_DOWN_MASK) | KWLN_BGFADE_COUNT_UP_BIT;
    }
    D_0037F5EC[2] = 2041.0f;
}

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
KwlnBackgroundOverlayActivity kwlnFadeIsBackgroundOverlayActive(void) {
    if (dds3DrawFlag & KWLN_BGFADE_DIRECTION_BITS) {
        return KWLN_BACKGROUND_OVERLAY_ACTIVE;
    }
    if (kwlnBackgroundFadeMode == 0) {
        if (KWLN_FADE_MAX_ALPHA - kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] >= KWLN_FADE_MAX_ALPHA) {
            return KWLN_BACKGROUND_OVERLAY_INACTIVE;
        }
    } else if (kwlnBackgroundFadeFirstRamp == KWLN_BGFADE_FIRST_RAMP_MAX) {
        return KWLN_BACKGROUND_OVERLAY_INACTIVE;
    }
    return KWLN_BACKGROUND_OVERLAY_ACTIVE;
}

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void kwlnFadeSetMode(s32 visibilityMode) {
    kwlnBackgroundFadeMode = visibilityMode;
    if (kwlnBackgroundFadeMode == 0) {
        kwlnBackgroundFadeFirstRamp = 0;
        kwlnBackgroundFadeSecondRamp = 0;
    } else {
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = KWLN_FADE_MAX_ALPHA;
    }
}

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void kwlnStepBackgroundFade(void) {
    f32 fadeRatio;

    if (dds3DrawFlag & KWLN_BGFADE_DIRECTION_BITS) {
        if (dds3DrawFlag & KWLN_BGFADE_COUNT_DOWN_BIT) {
            kwlnBackgroundFadeCounter--;
        } else {
            kwlnBackgroundFadeCounter++;
        }
        fadeRatio = (f32)kwlnBackgroundFadeCounter / (f32)kwlnBackgroundFadeDuration;
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = (kwlnBackgroundFadeCounter << KWLN_FADE_ALPHA_SHIFT) / kwlnBackgroundFadeDuration;
        kwlnBackgroundFadeFirstRamp = KWLN_BGFADE_FIRST_RAMP_SCALE - fadeRatio * KWLN_BGFADE_FIRST_RAMP_SCALE;
        kwlnBackgroundFadeSecondRamp = KWLN_BGFADE_SECOND_RAMP_SCALE - fadeRatio * KWLN_BGFADE_SECOND_RAMP_SCALE;
        if (((dds3DrawFlag & KWLN_BGFADE_COUNT_DOWN_BIT) && kwlnBackgroundFadeCounter == 0) ||
            ((dds3DrawFlag & KWLN_BGFADE_COUNT_UP_BIT) && kwlnBackgroundFadeCounter == kwlnBackgroundFadeDuration)) {
            kwlnBackgroundFadeCounter = 0;
            kwlnBackgroundFadeDuration = 0;
            dds3DrawFlag &= KWLN_BGFADE_KEEP_OTHER_BITS;
        }
    }
}

void kwlnDrawBackgroundFadeBands(void) {
    s16 firstRamp = 0;
    s16 secondRamp = 0;
    s32 fade;
    s32 alpha;

    if (kwlnBackgroundFadeMode == 0) {
        fade = KWLN_FADE_MAX_ALPHA - kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL];
    } else {
        firstRamp = kwlnBackgroundFadeFirstRamp;
        fade = 0;
        secondRamp = kwlnBackgroundFadeSecondRamp;
        if (firstRamp == KWLN_BGFADE_FIRST_RAMP_MAX) {
            return;
        }
    }
    if (fade >= KWLN_FADE_MAX_ALPHA) {
        return;
    }

    eglSetOt(0x4F);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    alpha = (KWLN_FADE_MAX_ALPHA - fade) << 24;
    evtSubmitDefaultDepthGradientRect(0, -2 - firstRamp, 0x200, 0x33, alpha, alpha, alpha, alpha);
    evtSubmitDefaultDepthGradientRect(0, secondRamp + 0x171, 0x200, 0x51, alpha, alpha, alpha, alpha);
}

void dds3SetScreenFovy(f32 fieldOfView) {
    D_0037F5EC[0] = fieldOfView;
}

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004112F0);

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_00411308);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C38);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C3F);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C40);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C41);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C42);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C48);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C49);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C50);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C52);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C54);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C56);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C58);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C60);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C68);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C70);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C78);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C80);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C88);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C90);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C98);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CA0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnCurrentIncompleteResource);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnTextureViewerPageIndex);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CB0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", egl_OT_Offset);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CBC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3RenderTex);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3RenderTexture);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CD0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3DrawFlag);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3ScreenBlurFlag);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3ScreenBlurAlpha);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3ScreenBlurScale);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEA);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", fadergba);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeColor);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFB);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeMode);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeFirstRamp);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeSecondRamp);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D02);

INCLUDE_SDATA(const s32, "game/code_00102DC8", distblurovercnt);

INCLUDE_SDATA(const s32, "game/code_00102DC8", ripblurovercnt);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D10);
