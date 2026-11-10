#include "sdk_ps2.h"
#include "sdf_scene_packet.h"
#include "sdf_gs_scene_state.h"
#include "common.h"
#include "file_config_option_api.h"
#include "sdf_vu0.h"
#include "dds3_background_fade_api.h"
#include "sdf_pad_motor_api.h"
#include "dds3Admin.h"
#include "kernel.h"
#include "dds3_draw_buffer.h"
#include "eff_random_mod.h"
#include "eff_random.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_tex_packet.h"
#include "kernel.h"
#include "dds3_task_update_count.h"
#include "dds3_process_registration.h"
#include "dds3_named_slot.h"
#include "dds3_task_create.h"
#include "dds3_task_runner.h"
#include "dds3_fade.h"
#include "dds3_fade_timing.h"
#include "sdf.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_linked_packet.h"
#include "sdf_draw.h"
#include "sdf_gs_blend.h"
#include "sdf_sif_command.h"
#include "dds3_task_lifecycle.h"
#include "btl.h"
/* Declared before first use (were implicit int). */
extern void eglSetOt(u32 surfaceIndex);
extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);
extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);
extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

extern u8 D_00435C60;

extern u32 dds3TextureViewerPageIndex;

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
#include "dds3_fade_color.h"
#include "evt_frame_state_packet.h"


extern void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

extern u32 dds3TextureGetPageIndex(void);

extern s32 dds3TextureCountIncompleteResources(void);


extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);


extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

extern void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task);

extern s32 D_00435BA4;

extern s32 distblurovercnt;

extern s32 ripblurovercnt;

extern sdfGrOt_t D_00380708;

extern dds3ProcessID_t *dds3DebugGraphTask;

extern f32 D_0037F5EC[];


extern sdfTexHandle_t *dds3CurrentIncompleteResource;

extern s32 dds3TextureViewerHandlePad(void);

extern s8 dds3PadBitTable[32];

extern s32 dds3SelectIncompleteResourceAtPageIndex(s32);

extern void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);

extern sdfGrOt_t sdfDebugGrOt;

extern s8 D_0037F53B[];

extern void dds3TextureAttachTask(sdfGrOt_t *surface);

extern s16 dds3BackgroundFadeFirstRamp;

extern s16 dds3BackgroundFadeSecondRamp;

extern u8 dds3BackgroundFadeColor[4];

extern s8 dds3BackgroundFadeMode;

extern u8 dds3PadVibrationMode;

extern u8 dds3PadVibrationState[2];

extern dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task);
extern void dds3AdminiNeoShutdown(dds3ProcessID_t *task);
extern char dds3AdminTaskName[];
extern u16 dds3BackgroundFadeCounter;
extern u16 dds3BackgroundFadeDuration;

extern EffRandState effSharedRandomState[];

extern void sdfRandomize(void *data, u32 tag);

void dds3InitAdmini(void) {
    dds3AdminiData_t *work;
    s32 i;

    if (dds3SearchProcessName(dds3AdminTaskName) == NULL) {
        work = sdfChipAllocDirect(0x24);
        work->Stat = 0;
        work->RunTimer = 0;
        work->NowSequence = -1;
        work->NextSequence = -1;
        work->HistoryPointer = 0;
        for (i = 0; i < 8; i++) {
            work->SequenceHistory[i] = -1;
            work->FlagHistory[i] = 0;
        }
        work->CallData = 0;
        work->CallDataSize = 0;
        dds3StartProcess(dds3AdminTaskName, 2, 0, 1, dds3AdminiNeoProcess, dds3AdminiNeoShutdown, (u32)work);
    }
}

extern fclNamedSlot_t D_0043D410[];
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
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_0043D410[index].kind = kind;
    D_0043D410[index].task = NULL;
    D_0043D410[index].update = update;
    D_0043D410[index].destroy = destroy;
    D_0043D410[index].setState = setState;
    D_0043D410[index].enabled = 0;
    D_0043D410[index].userValue = userValue;
}

#define DDS3_PAD_INPUTS_PER_SET 16
#define DDS3_PAD_REPEAT_BIT 2
#define DDS3_ALT_CONFIRM_INPUT 0x11
#define DDS3_ALT_CANCEL_INPUT 0x13
#define DDS3_PAD_SMALL_MOTOR 0
#define DDS3_PAD_LARGE_MOTOR 1
#define DDS3_PAD_MOTOR_COUNT 2
#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650
#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF
#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30
#define DDS3_DEBUG_RUMBLE_VARIATION 150
#define DDS3_DEBUG_RUMBLE_BASE 100
#define DDS3_SHARED_RANDOM_SEED 0x12345678
#define DDS3_CELL_COLUMN_SPAN 0xC0
#define DDS3_CELL_ROW_SPAN 0x60
#define DDS3_PREVIEW_MAX_TEXELS 0x100
#define DDS3_PREVIEW_WRAP_TEXELS 0x400
#define DDS3_DIAG_DEPTH 0x0FFFFF80

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
            if (dds3ExistProcess(D_0043D410[slotIndex].task) == DDS3_PROCESS_REGISTERED) {
                dds3KillProcess(D_0043D410[D_00435C2C].task, 1);
            }
        }
    }
    D_0043D410[slotIndex].name[0] = 0;
    dds3SetNamedSlotState(slotIndex, 0, NULL, NULL, NULL, 0);
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


s32 fclEncycConfigureMode3(const char *name, void (*setState)(s32)) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 3, NULL, NULL, setState, 0);
    return 1;
}

s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 1, update, destroy, NULL, userValue);
    return 1;
}

s32 fclEncycConfigureMode2(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue) {
    s32 index = fclEncycFindFreeSlot(name);
    if (index < 0) {
        return 0;
    }
    dds3SetNamedSlotState(index, 2, update, destroy, NULL, userValue);
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
    backwardState = dds3PadButtonState[backwardInput + padSet * DDS3_PAD_INPUTS_PER_SET];
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
    } else if (dds3PadButtonState[backwardInput + padSet * DDS3_PAD_INPUTS_PER_SET] & DDS3_PAD_REPEAT_BIT) {
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
    forwardState = dds3PadButtonState[forwardInput + padSet * DDS3_PAD_INPUTS_PER_SET];
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
    } else if (dds3PadButtonState[forwardInput + padSet * DDS3_PAD_INPUTS_PER_SET] & DDS3_PAD_REPEAT_BIT) {
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
        if (dds3PadButtonState[DDS3_ALT_CONFIRM_INPUT] < 0) {
            return 1;
        }
        if (dds3PadButtonState[DDS3_ALT_CANCEL_INPUT] < 0) {
            return -1;
        }
    }
    return 0;
}

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = DDS3_CELL_COLUMN_SPAN, rowSpan = DDS3_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = DDS3_CELL_COLUMN_SPAN, rowSpan = DDS3_CELL_ROW_SPAN;
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % DDS3_DEBUG_RUMBLE_PERIOD == 0) {
        dds3PadVibrationSet(DDS3_PAD_SMALL_MOTOR, 1, DDS3_DEBUG_SMALL_PULSE_FRAMES);
        dds3PadVibrationSet(DDS3_PAD_LARGE_MOTOR, (u8)(dds3GetRandIntA(0, DDS3_DEBUG_RUMBLE_VARIATION) + DDS3_DEBUG_RUMBLE_BASE), DDS3_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

extern u32 dds3PadButtonHoldCounters[4][16];
extern u32 dds3PadStickHoldCounters[2][2];
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
    u32 (*inputTimers)[16] = dds3PadButtonHoldCounters;

    for (port = 0; port < 2; port++) {
        for (bank = 0; bank < 2; bank++) {
            for (i = 0; i < 16; i++) {
                dds3PadButtonPrev[bank][port][i] = 0;
                dds3PadButtonState[bank * 32 + port * 16 + i] = 0;
                inputTimers[bank * 2 + port][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            dds3PadStickHoldCounters[port][i] = 0;
            D_00435C30[port][i][0] = 0;
            D_00435C30[port][i][1] = 0;
            D_00435C38[port][i] = 0;
            D_00435C40[port][i][0] = 0;
            D_00435C40[port][i][1] = 0;
            D_00435C48[port][i] = 0;
        }
    }
    dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438D98[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438D98[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_LARGE_MOTOR] = 0;
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

void dds3RunProcessList(void) {
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
                    if (++dds3PadButtonHoldCounters[j * 2 + pad][i] == 1) {
                        dds3PadButtonPrev[j][pad][i] = -0x7D;
                    }
                } else {
                    dds3PadButtonPrev[j][pad][i] = 0;
                    dds3PadButtonHoldCounters[j * 2 + pad][i] = 0;
                }
                if (dds3PadButtonHoldCounters[j * 2 + pad][i] >= 8 && !(dds3PadButtonHoldCounters[j * 2 + pad][i] & 1)) {
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
                if (++dds3PadStickHoldCounters[pad][i] == 1) {
                    D_00435C38[pad][i] = -0x7D;
                }
            } else {
                D_00435C38[pad][i] = 0;
                dds3PadStickHoldCounters[pad][i] = 0;
            }
            if (dds3PadStickHoldCounters[pad][i] >= 8 && !(dds3PadStickHoldCounters[pad][i] & 1)) {
                D_00435C38[pad][i] |= 2;
            }
        }
    }
    dds3UpdateVibrationSlots();
}

void dds3GetVibrationLevel(void) {
    u8 target = dds3PadVibrationMode;

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
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        if (level != 0) {
            level = 1;
        }
    } else if (motor >= DDS3_PAD_MOTOR_COUNT) {
        motor = DDS3_PAD_LARGE_MOTOR;
    }
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        D_00438D98[DDS3_PAD_SMALL_MOTOR] = level;
        dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = level;
    } else {
        D_00438D98[motor] = level;
        dds3GetVibrationLevel();
    }
    D_00438DA0[motor] = duration;
    if (motor == DDS3_PAD_SMALL_MOTOR) {
        sdfPadSetAct0(0, level);
    } else {
        sdfPadSetAct1(0, dds3PadVibrationState[motor]);
    }
}

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void dds3PadVibrationReset(void) {
    dds3PadVibrationState[DDS3_PAD_SMALL_MOTOR] = 0;
    dds3PadVibrationState[DDS3_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[DDS3_PAD_LARGE_MOTOR] = 0;
    sdfPadSetAct(0, 0, 0);
}


void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = DDS3_PAD_LARGE_MOTOR;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = D_00438D98;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < DDS3_PAD_MOTOR_COUNT; motor++) {
        if (D_00438DA0[motor] != 0) {
            D_00438DA0[motor]--;
            if (D_00438DA0[motor] == 0) {
                levels[motor] = 0;
                if (motor == DDS3_PAD_SMALL_MOTOR) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + DDS3_PAD_LARGE_MOTOR;
                if (*largeLevel != targets[DDS3_PAD_LARGE_MOTOR]) {
                    dds3GetVibrationLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}

s32 dds3TaskNoopCallback(dds3ProcessID_t *task) {
    return 0;
}

/* Seed the shared effect random state with the fixed initialization seed. */
void dds3InitRandCB(void) {
    sdfRandomize(effSharedRandomState, DDS3_SHARED_RANDOM_SEED);
}

extern s16 D_00435BA8;
extern u16 sdfDrawFrameCounter;
extern u16 D_00435C50;
extern u16 D_00435C52;
extern u16 D_00435C54;
extern u16 D_00435C56;
extern char D_00435C58[];

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfPrimTransPacket_t *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: dds3DebugGraphUpdate". */
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
                    0x8620, 0xFFFF80, 0, D_00435C58, sdfDrawFrameCounter));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x85C0, 0xFFFF80, 6, D_00435C58, D_00435C50));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x8620, 0xFFFF80, 6, D_00435C58, D_00435C52));
    pixels = (s32)((f32)(u16)D_00435BA8 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)sdfDrawFrameCounter / 224.0f * 200.0f);
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
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
    if (D_00435C54 >= 0x47 || D_00435C50 < (u16)D_00435BA8) {
        D_00435C50 = D_00435BA8;
        D_00435C54 = 0;
    } else {
        D_00435C54++;
    }
    if (D_00435C56 >= 0x47 || D_00435C52 < sdfDrawFrameCounter) {
        D_00435C52 = sdfDrawFrameCounter;
        D_00435C56 = 0;
    } else {
        D_00435C56++;
    }
    return 0;
}

void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled) {
    if (enabled == 1) {
        dds3DebugGraphTask = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, dds3DebugGraphUpdate, dds3DebugGraphTaskDestroy, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(dds3DebugGraphTask, 0);
    }
}

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void dds3DrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image) {
    s32 width = image->width;
    s32 height = image->height;
    s32 drawWidth;
    s32 drawHeight;

    sdf2DLinkTexPacket(packetList, image, SDF_GS_CONTEXT_1);
    drawWidth = width * 0x10;
    drawHeight = height * 8;
    if (width < height) {
        if (height > DDS3_PREVIEW_MAX_TEXELS) {
            drawHeight = 0x800;
            drawWidth = (width << 8) / height * 0x10;
        }
    } else if (width > DDS3_PREVIEW_MAX_TEXELS) {
        drawWidth = 0x1000;
        drawHeight = (height << 8) / width * 8;
    }
    if (width == DDS3_PREVIEW_WRAP_TEXELS) {
        width--;
    }
    if (height == DDS3_PREVIEW_WRAP_TEXELS) {
        height--;
    }
    sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7A60, 0, 0, drawWidth + 0x7180, drawHeight + 0x7A60,
                                width * 0x10, height * 0x10, DDS3_DIAG_DEPTH, 0);
}

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList) {
    char pageText[0x70];
    s32 pageIndex = dds3TextureGetPageIndex();
    s32 resourceCount = dds3TextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x79C0, DDS3_DIAG_DEPTH, 0, pageText));
}

extern const char *D_0037F560[];
extern char D_004112F0[];
extern char D_00411308[];

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture) {
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
    sprintf(text, "ID[%2d,%2d]", texture->texId, texture->clutId);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
}

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20
#define DDS3_VIEWER_FAST_PAGE_STEP 10
#define DDS3_MAP_ROW_UNITS 0x1000
#define DDS3_MAP_ROW_SHIFT 12
#define DDS3_MAP_ROW_MASK 0xFFF
#define DDS3_MAP_COLUMN_SHIFT 6
#define DDS3_MAP_COLUMN_UNITS 64
#define DDS3_MAP_COLUMN_COUNT 64
#define DDS3_MAP_ROW_SPAN 0x400
#define DDS3_MAP_BORDER_WIDTH 0x440
#define DDS3_MAP_CELL_X_STEP 16
#define DDS3_MAP_ROW_Y_STEP 8
#define DDS3_MAP_CELL_X_SHIFT 4
#define DDS3_MAP_GREEN 0x80008000
#define DDS3_MAP_YELLOW 0x80008080
#define DDS3_MAP_RED 0x80000080
#define DDS3_MAP_BLUE 0x80800000
#define DDS3_MAP_CYAN 0x80808000
#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

/* Count texture entries without a reference clone source. */
s32 dds3TextureCountIncompleteResources(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 incompleteCount = 0;
    while (texture != NULL) {
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            previousTexture = texture->next;
            if (previousTexture == NULL) {
                return incompleteCount;
            }
            texture = previousTexture;
        }
        incompleteCount++;
        texture = texture->next;
    }
    return incompleteCount;
}

u32 dds3TextureGetPageIndex(void) {
    return dds3TextureViewerPageIndex;
}

/* DDS2 first skips clone-source entries at the head, even for requested index zero.
 * This assumes a nonempty list; initial exhaustion returns 0 before selection writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *previousTexture;
    s32 visitedIndex = 0;

    while (texture->ref != NULL && texture->ref->origin != NULL) {
        previousTexture = texture->next;
        if (previousTexture == NULL) {
            return 0;
        }
        texture = previousTexture;
    }
    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (texture != NULL && visitedIndex != requestedIndex) {
        dds3CurrentIncompleteResource = texture;
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            previousTexture = texture->next;
            if (previousTexture == NULL) {
                return visitedIndex;
            }
            texture = previousTexture;
        }
        previousTexture = texture->next;
        if (previousTexture == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        texture = previousTexture;
    }
    dds3CurrentIncompleteResource = texture;
    return visitedIndex;
}

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 dds3TextureViewerHandlePad(void) {
    if (dds3PadBitTable[0x13] < 0) {
        return 0;
    }
    if (dds3PadBitTable[0x14] & DDS3_PAD_REPEAT_BIT) {
        dds3TextureViewerPageIndex -= 1;
    } else if (dds3PadBitTable[0x15] & DDS3_PAD_REPEAT_BIT) {
        dds3TextureViewerPageIndex += 1;
    } else if ((dds3PadBitTable[0x18] & DDS3_PAD_REPEAT_BIT) || (dds3PadBitTable[0x19] & DDS3_PAD_REPEAT_BIT)) {
        dds3TextureViewerPageIndex -= DDS3_VIEWER_FAST_PAGE_STEP;
    } else if ((dds3PadBitTable[0x1A] & DDS3_PAD_REPEAT_BIT) || (dds3PadBitTable[0x1B] & DDS3_PAD_REPEAT_BIT)) {
        dds3TextureViewerPageIndex += DDS3_VIEWER_FAST_PAGE_STEP;
    }
    dds3TextureViewerPageIndex = dds3SelectIncompleteResourceAtPageIndex(dds3TextureViewerPageIndex);
    if (dds3PadBitTable[0x12] < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

extern s32 D_00435CB0;

void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image) {
    sdfDrawTag_t *packetList;
    s32 counter;
    s32 mode;

    if (image == NULL) {
        return;
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    dds3TextureDrawPageCounter(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    dds3DrawImageOutline(packetList, image);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
    dds3DrawTextureResourceDetails(packetList, image);

    if (D_00435C60 != 0) {
        dds3DrawTextureListDiagnostic(packetList, 0x8240, 0x7A60);
        counter = D_00435CB0;
        mode = (counter >> 4) & 1;
        if (image->pixelGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->pixelGsHandle, mode);
        }
        counter = D_00435CB0;
        /* Boolean highlight selection; see docs/idioms.md, "Trace-justified boolean extraction: texture viewer highlight". */
        mode = (counter & 0x10) ? 2 : 0;
        if (image->clutGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->clutGsHandle, mode);
        }
        D_00435CB0++;
    }
    surface->regFunc(surface, packetList);
}

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface) {
    s32 inputStatus;

    if (sdfTexHandleLink == 0) {
        return 0;
    }
    inputStatus = dds3TextureViewerHandlePad();
    dds3DrawTextureViewerFrame(surface, dds3CurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void) {
    if (sdfTexHandleLink == 0) {
        return 0;
    }
    if (dds3TextureViewerHandlePad() == 0) {
        return -1;
    }
    dds3DrawTextureViewerFrame(&sdfDebugGrOt, dds3CurrentIncompleteResource);
    return 0;
}

/* Initialize viewer state and return its default renderer for the first eligible
 * node. Exhausting a nonempty list returns NULL after the initial state writes. */
s32 (*dds3TextureFindIncompleteResource(void))(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    if (texture == NULL) {
        return NULL;
    }
    dds3CurrentIncompleteResource = texture;
    D_00435C60 = 1;
    dds3TextureViewerPageIndex = 0;
    while (texture->ref != NULL && texture->ref->origin != NULL) {
        texture = dds3CurrentIncompleteResource->next;
        if (texture == NULL) {
            return NULL;
        }
        dds3CurrentIncompleteResource = texture;
    }
    return dds3LoadDefaultResource;
}

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch ((s32)block->type) {
        case 1:
            color = DDS3_MAP_GREEN;
            break;
        case 2:
            color = DDS3_MAP_YELLOW;
            break;
        case 3:
            color = DDS3_MAP_RED;
            break;
        default:
            color = 0;
            break;
        }
    } else {
        color = mode == 1 ? DDS3_MAP_BLUE : DDS3_MAP_CYAN;
    }
    remainingUnits = block->size;
    topY = y + (block->addr >> DDS3_MAP_ROW_SHIFT) * DDS3_MAP_ROW_Y_STEP;
    columnIndex = (block->addr & DDS3_MAP_ROW_MASK) >> DDS3_MAP_COLUMN_SHIFT;
    leftX = x + columnIndex * DDS3_MAP_CELL_X_STEP;
    if (columnIndex > 0) {
        rowUnits = (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_COLUMN_UNITS;
        if (remainingUnits >= rowUnits) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> DDS3_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= DDS3_MAP_ROW_UNITS) {
        sdfPrimLinkLine(packetList, color, 0, x, topY, x + DDS3_MAP_ROW_SPAN, topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
        remainingUnits -= DDS3_MAP_ROW_UNITS;
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> DDS3_MAP_COLUMN_SHIFT) << DDS3_MAP_CELL_X_SHIFT), topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
    }
}


/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfGsMemSlice_t *block = sdfGsMemGetLinkHead();

    if (block != NULL) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, DDS3_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            dds3DrawTextureAllocationMapBlock(packetList, x, y, block, 0);
            block = block->next;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    dds3DrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    surface->regFunc(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    dds3TextureAttachTask(&sdfDebugGrOt);
    return 0;
}

/* Two fixed frame banks: thirteen draw groups and their scene/overlay packets. */
#include "dds3_frame_draw_bank.h"
typedef char dds3FrameDrawBank_size_check[sizeof(dds3FrameDrawBank_t) == 0x1F40 ? 1 : -1];

extern dds3FrameDrawBank_t dds3DrawEnv[2];
extern u32 D_00435CBC;
extern sdfGrFrameSet_t sdfGrFrameSet;
extern sdf3DLight_t *D_0037F770[3];
extern f32 kwlnDefaultColorVector[4];
extern Matrix4 sdfViewMatrix;
extern Matrix4 D_00384790;
extern sdf3DLight_t *kwlnOverlayLightSources[3];
extern f32 kwlnOverlayAmbientColor[4];
extern void sdf3DLinkInitPacket(sdfDrawTag_t *, DmaPacketHeader *);

/* Initialize the selected frame bank's program, scene and overlay packet chains. */
void dds3InitializeFrameDrawPackets(s32 bufferIndex) {
    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].initialList);
    sdf3DLinkInitPacket(&dds3DrawEnv[bufferIndex].initialList,
                                        &dds3DrawEnv[bufferIndex].programReference);
    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].sceneList);
    sdfPreDrawTagInit(&dds3DrawEnv[bufferIndex].linkedList);
    sdfGrSetStdDrawEnv(&dds3DrawEnv[bufferIndex].scene, &sdfGrFrameSet);
    /* Set the RGBAQ color word while retaining its floating-point Q word. */
    *(u32 *)&dds3DrawEnv[bufferIndex].scene.centeredBounds.rgbaq.value = D_00435CBC;
    if (dds3DrawFlag & 0x10000000) {
        sdfGrSetClearPacket(&dds3DrawEnv[bufferIndex].scene.centeredBounds, 0, 0,
                                         sdfGrFrameSet.bufferFormat,
                                         sdfGrFrameSet.auxiliaryFormat);
        dds3DrawFlag &= 0xEFFFFFFF;
    }
    sdfGrLinkStdDrawEnv(&dds3DrawEnv[bufferIndex].sceneList,
                                 &dds3DrawEnv[bufferIndex].linkedList,
                                 &dds3DrawEnv[bufferIndex].scene.link);
    sdf3DMakeViewportPacket(&dds3DrawEnv[bufferIndex].sceneMatrix,
                             &dds3ScreenParam, &sdfViewMatrix);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].sceneList, (u32)&dds3DrawEnv[bufferIndex].sceneMatrix);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[bufferIndex].scenePacketStorage, D_0037F770, kwlnDefaultColorVector);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].sceneList, (u32)&dds3DrawEnv[bufferIndex].scenePacketStorage);

    sdfDrawTagInit(&dds3DrawEnv[bufferIndex].thirdList);
    sdf3DMakeViewportPacket(&dds3DrawEnv[bufferIndex].thirdMatrix, &D_0037F980, &D_00384790);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].thirdList, (u32)&dds3DrawEnv[bufferIndex].thirdMatrix);
    sdf3DMakeLightParallelPacket(&dds3DrawEnv[bufferIndex].overlayLightPacket, kwlnOverlayLightSources, kwlnOverlayAmbientColor);
    sdfDrawTagLink(&dds3DrawEnv[bufferIndex].thirdList, (u32)&dds3DrawEnv[bufferIndex].overlayLightPacket);
}

extern void sdfGrSetScreen(s32 displayModeIndex);
extern void dds3InitializeFrameDrawPackets(s32 bufferIndex);
extern void dds3InitZClearDrawTag(void);
extern u8 D_00435BC8;
extern void eglInit(void);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void dds3InitDrawEnv(void) {
    sdfGrSetScreen(1);
    sdf3DMakeScreenParam(&dds3ScreenParam.camera);
    sdf3DMakeScreenParam(&D_0037F980.camera);
    dds3InitializeFrameDrawPackets(0);
    dds3InitializeFrameDrawPackets(1);
    dds3InitZClearDrawTag();
    D_00435BC8 = 0;
    eglInit();
}


/* One 0x1F40-byte draw bank of the held-texture render target. */
#include "dds3_texture_bank.h"

extern sdfGrFrameSet_t dds3RenderTexId;
extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern u32 dds3RenderDrawEnv;
extern sdf3DLight_t *dds3RenderLightParallel[3];
extern f32 dds3RenderAmbientColor[4];
extern Matrix4 dds3RenderViewportMatrix;

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
    dds3RenderTexId.buffers[2] = sdfGrFrameSet.buffers[2];
    sdf3DMakeScreenParam(&D_0037F7B0.camera);
    for (i = 0; i != 2; i++) {
        sdfDrawTagInit(&dds3DrawEnv[i].heldTexture.list);
        sdfGrSetDrawEnv(&dds3DrawEnv[i].heldTexture.scene, &dds3RenderTexId, 0);
        *(u32 *)&dds3DrawEnv[i].heldTexture.scene.centeredBounds.rgbaq.value = dds3RenderDrawEnv;
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.scene);
        sdf3DMakeViewportPacket(&dds3DrawEnv[i].heldTexture.matrix, &D_0037F7B0, &dds3RenderViewportMatrix);
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.matrix);
        sdf3DMakeLightParallelPacket(&dds3DrawEnv[i].heldTexture.lighting, dds3RenderLightParallel, dds3RenderAmbientColor);
        sdfDrawTagLink(&dds3DrawEnv[i].heldTexture.list, (u32)&dds3DrawEnv[i].heldTexture.lighting);
    }
}

extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern void dds3FreeRenderTexture(void);
extern void dds3InitRenderDrawEnv(void);
extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);
extern sdfTexHandle_t *sdfTexNew(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary);
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
    texture = sdfTexNew(width, height, 0, 0, D_00435CC4, 0, 0, 0);
    if (texture == 0) {
        return 0;
    }
    dds3RenderTexture = texture;
    sdfTexSetWrapMode(texture, DDS3_HELD_TEXTURE_CLAMP_MODE);
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
#include "dds3_texture_reference_flag_api.h"

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
void dds3RunQuake(void) {
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

#define DDS3_FADE_TICKS_PER_FRAME 10
#define DDS3_FADE_MARK_NUMERATOR 4
#define DDS3_FADE_COUNT_DOWN_BIT 1
#define DDS3_FADE_COUNT_UP_BIT 2
#define DDS3_FADE_DIRECTION_BITS 3
#define DDS3_FADE_MAX_ALPHA 0x80
#define DDS3_FADE_ALPHA_SHIFT 7
#define DDS3_COLOR_ALPHA_CHANNEL 3
#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000
#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000
#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000
#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF
#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF
#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF
#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31
#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F
#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f
#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

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
    D_00435CEE = frames * DDS3_FADE_TICKS_PER_FRAME;
    D_00435CEC = D_00435CEE * DDS3_FADE_MARK_NUMERATOR / DDS3_FADE_TICKS_PER_FRAME;
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
    dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
    fadergba.r = 0;
    fadergba.g = 0;
    fadergba.b = 0;
    fadergba.a = 0;
}

/* Store RGBA and cancel foreground direction flags without changing timing. */
void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha) {
    dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
    fadergba.r = red;
    fadergba.g = green;
    fadergba.b = blue;
    fadergba.a = alpha;
}

/* Return the live color address through outColor, rather than copying channels. */
void dds3FadeColorGet(dds3FadeColor_t **outColor) {
    *outColor = &fadergba;
}

/* Replace RGB only; alpha, direction flags and timing are preserved. */
void dds3FadeColorSet(s8 red, s8 green, s8 blue) {
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
    fadergba.a = -DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        fademax = 0;
        dds3FadeClear();
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | DDS3_FADE_COUNT_DOWN_BIT) & ~DDS3_FADE_COUNT_UP_BIT;
}

/* Start the same countdown using existing RGB; zero duration preserves RGB. */
void dds3FadeIn2(s32 duration) {
    fadergba.a = -DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        fadergba.a = 0;
        fadenow = 0;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = duration;
    dds3DrawFlag = (dds3DrawFlag | DDS3_FADE_COUNT_DOWN_BIT) & ~DDS3_FADE_COUNT_UP_BIT;
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
        fadergba.a = -DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~DDS3_FADE_COUNT_DOWN_BIT) | DDS3_FADE_COUNT_UP_BIT;
}

/* Start the same count-up using existing RGB; zero duration preserves RGB. */
void dds3FadeOut2(s32 duration) {
    fadergba.a = 0;
    if (duration == 0) {
        fadenow = 0;
        fadergba.a = -DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        fademax = 0;
        return;
    }
    fademax = duration;
    fadenow = 0;
    dds3DrawFlag = (dds3DrawFlag & ~DDS3_FADE_COUNT_DOWN_BIT) | DDS3_FADE_COUNT_UP_BIT;
}

/* Return whether either foreground counter direction is enabled. */
s32 dds3FadeCheck(void) {
    return (dds3DrawFlag & DDS3_FADE_DIRECTION_BITS) != 0;
}

/* Step the u16 counter and sample alpha before testing exact completion.
 * Completion clears timing/flags but leaves the sampled endpoint alpha intact. */
void dds3FadeUpdate(void) {
    if (dds3FadeCheck() != 0) {
        if (dds3DrawFlag & DDS3_FADE_COUNT_DOWN_BIT) {
            fadenow -= 1;
        } else {
            fadenow += 1;
        }
        fadergba.a = (fadenow << DDS3_FADE_ALPHA_SHIFT) / fademax;
        if (((dds3DrawFlag & DDS3_FADE_COUNT_DOWN_BIT) && fadenow == 0) || ((dds3DrawFlag & DDS3_FADE_COUNT_UP_BIT) && fadenow == fademax)) {
            fadenow = 0;
            fademax = 0;
            dds3DrawFlag &= ~DDS3_FADE_DIRECTION_BITS;
        }
    }
}

/* Submit diagnostics when enabled and either counter is nonzero; only positive
 * counters produce text, so negative-only errors still submit an empty list. */
void dds3DrawDistRipOverError(void) {
    sdfDrawTag_t *packetList;

    if (D_00435BA4 != 0) {
        if (distblurovercnt != 0 || ripblurovercnt != 0) {
            packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
            sdfDrawTagInit(packetList);
            if (distblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, D_004112F0, distblurovercnt));
            }
            if (ripblurovercnt > 0) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x73C0, 0x7B40, 0xFEFFFF, 4, D_00411308, ripblurovercnt));
            }
            D_00380708.regFunc(&D_00380708, packetList);
        }
    }
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf.h"
#include "sdf_gs_packet.h"
#include "sdf_packet_list.h"
#include "dds3_fade_color.h"
#include "dds3_draw_buffer.h"

extern s16 dds3BackgroundFadeFirstRamp; /* the background fade's band ramps */
extern s16 dds3BackgroundFadeSecondRamp;
extern u8 dds3FadePacket[]; /* the overlay packet, one per draw buffer */
extern sdfGrOt_t sdfGrOtTable[];
extern s32 dds3FadeIsBackgroundOverlayActive(void);

#define DDS3_FADE_OVERLAY_SURFACE 92
#define DDS3_FADE_MAX_ALPHA 0x80
#define DDS3_GS_PABE 0x49
#define DDS3_GS_FBA_1 0x4A
#define DDS3_OVERLAY_TEST 0x31003       /* alpha and depth tests always pass */
#define DDS3_OVERLAY_ALPHA 0x01         /* (Cd - Cs) * As / 128 + Cs */
#define DDS3_OVERLAY_DEPTH 0xFEFFFFULL
#define DDS3_OVERLAY_LEFT 0x7000
#define DDS3_OVERLAY_RIGHT 0x9000
#define DDS3_OVERLAY_TOP 0x7900
#define DDS3_OVERLAY_BOTTOM 0x8700

/* An end tag carrying VIF DIRECT of ten quadwords: four A+D register writes,
 * then a blended sprite of two vertices. */
typedef struct dds3FadePacket_t {
    SdfGsPacketHeader header;
    SdfGsRegisterWrite alpha;
    SdfGsRegisterWrite pabe;
    SdfGsRegisterWrite fba;
    SdfGsRegisterWrite test;
    u64 spriteTag;
    u64 spriteRegisters;
    SdfGsRegisterWrite vertices[4];
} dds3FadePacket_t;

typedef char dds3FadePacket_size_must_be_0xB0[(sizeof(dds3FadePacket_t) == 0xB0) ? 1 : -1];

static void dds3WriteRegister(SdfGsRegisterWrite *write, u64 value, u64 registerId) {
    write->value = value;
    write->registerId = registerId;
}

static u64 dds3OverlayVertex(u32 x, u32 y) {
    return (DDS3_OVERLAY_DEPTH << 32) | (u32)(y << 16) | x;
}

/* While the fade colour has any alpha, cover the screen with it: the sprite
 * keeps (128 - alpha) / 128 of what is drawn. An active background overlay
 * narrows it to the rows between its two bands. Upstream: INCLUDE_ASM in
 * both games; same packet as the retail routine. */
void dds3DrawFade(void) {
    u32 buffer = dds3GetFrameID();
    s32 overlayAlpha = DDS3_FADE_MAX_ALPHA - fadergba.a;
    dds3FadePacket_t *packet;
    u32 top = DDS3_OVERLAY_TOP;
    u32 bottom = DDS3_OVERLAY_BOTTOM;
    u64 color;
    sdfDrawTag_t *packets;
    sdfGrOt_t *surface;

    if (overlayAlpha >= DDS3_FADE_MAX_ALPHA) {
        return;
    }
    packet = (dds3FadePacket_t *)(dds3FadePacket + buffer * sizeof(dds3FadePacket_t));
    packet->header.dmaTag = 0x7000000A;              /* end, ten quadwords */
    packet->header.vifCommands = 0x5000000AULL << 32; /* NOP; DIRECT ten quadwords */
    packet->header.gifTag = 0x1000000000000004ULL;    /* NLOOP 4, NREG 1 */
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    dds3WriteRegister(&packet->alpha, DDS3_OVERLAY_ALPHA, SDF_GS_ALPHA_1);
    dds3WriteRegister(&packet->pabe, 0, DDS3_GS_PABE);
    dds3WriteRegister(&packet->fba, 0, DDS3_GS_FBA_1);
    dds3WriteRegister(&packet->test, DDS3_OVERLAY_TEST, SDF_GS_TEST_1);
    packet->spriteTag = 0x1023400000008004ULL; /* NLOOP 4, EOP, PRE, PRIM blended sprite, NREG 1 */
    packet->spriteRegisters = SDF_GIF_REGISTER_AD;

    if (dds3FadeIsBackgroundOverlayActive()) {
        top = 0x7A88 - dds3BackgroundFadeFirstRamp * 8;
        bottom = 0x8488 + dds3BackgroundFadeSecondRamp * 8;
    }
    /* Retail shifts the alpha in 32 bits, sign-extending a negative one (a
     * fade alpha above 0x80) over Q; the sprite is untextured, so Q goes
     * unread, but the packet keeps retail's bits. */
    color = 0x3F80000000000000ULL /* Q = 1.0 */ | (u64)(s64)(s32)((u32)overlayAlpha << 24) |
            ((u64)fadergba.b << 16) | ((u64)fadergba.g << 8) | fadergba.r;
    dds3WriteRegister(&packet->vertices[0], color, SDF_GS_RGBAQ);
    dds3WriteRegister(&packet->vertices[1], dds3OverlayVertex(DDS3_OVERLAY_LEFT, top), SDF_GS_XYZ2);
    dds3WriteRegister(&packet->vertices[2], color, SDF_GS_RGBAQ);
    dds3WriteRegister(&packet->vertices[3], dds3OverlayVertex(DDS3_OVERLAY_RIGHT, bottom), SDF_GS_XYZ2);

    packets = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(packets, (u32)packet);
    surface = &sdfGrOtTable[DDS3_FADE_OVERLAY_SURFACE];
    surface->regFunc(surface, packets);
}
#else
INCLUDE_ASM(const s32, "game/code_00102DC8", dds3DrawFade);
#endif

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void dds3FadeResetBackground(void) {
    dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
    dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_MAX;
    dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_MAX;
}

/* Start the background countdown; zero duration performs the full background reset. */
void dds3FadeBackgroundStartOut(s32 duration) {
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
    if (duration == 0) {
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
        dds3BackgroundFadeCounter = 0;
        dds3BackgroundFadeDuration = 0;
        dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_MAX;
        dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_MAX;
        dds3FadeResetBackground();
    } else {
        dds3BackgroundFadeDuration = duration;
        dds3BackgroundFadeCounter = duration;
        dds3DrawFlag = (dds3DrawFlag | DDS3_BGFADE_COUNT_DOWN_BIT) & DDS3_BGFADE_CLEAR_UP_MASK;
    }
    D_0037F5EC[2] = 2048.0f;
}

/* Start the background count-up; zero duration leaves max alpha and zero ramps. */
void dds3FadeBackgroundStartIn(s32 duration) {
    dds3BackgroundFadeColor[0] = 0;
    dds3BackgroundFadeColor[1] = 0;
    dds3BackgroundFadeColor[2] = 0;
    dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = 0;
    if (duration == 0) {
        dds3BackgroundFadeCounter = 0;
        dds3BackgroundFadeDuration = 0;
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
        dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
        dds3BackgroundFadeFirstRamp = 0;
        dds3BackgroundFadeSecondRamp = 0;
    } else {
        dds3BackgroundFadeDuration = duration;
        dds3BackgroundFadeCounter = 0;
        dds3DrawFlag = (dds3DrawFlag & DDS3_BGFADE_CLEAR_DOWN_MASK) | DDS3_BGFADE_COUNT_UP_BIT;
    }
    D_0037F5EC[2] = 2041.0f;
}

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
dds3BackgroundOverlayActivity_t dds3FadeIsBackgroundOverlayActive(void) {
    if (dds3DrawFlag & DDS3_BGFADE_DIRECTION_BITS) {
        return DDS3_BACKGROUND_OVERLAY_ACTIVE;
    }
    if (dds3BackgroundFadeMode == 0) {
        if (DDS3_FADE_MAX_ALPHA - dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] >= DDS3_FADE_MAX_ALPHA) {
            return DDS3_BACKGROUND_OVERLAY_INACTIVE;
        }
    } else if (dds3BackgroundFadeFirstRamp == DDS3_BGFADE_FIRST_RAMP_MAX) {
        return DDS3_BACKGROUND_OVERLAY_INACTIVE;
    }
    return DDS3_BACKGROUND_OVERLAY_ACTIVE;
}

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void dds3FadeSetMode(s32 visibilityMode) {
    dds3BackgroundFadeMode = visibilityMode;
    if (dds3BackgroundFadeMode == 0) {
        dds3BackgroundFadeFirstRamp = 0;
        dds3BackgroundFadeSecondRamp = 0;
    } else {
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = DDS3_FADE_MAX_ALPHA;
    }
}

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void dds3StepBackgroundFade(void) {
    f32 fadeRatio;

    if (dds3DrawFlag & DDS3_BGFADE_DIRECTION_BITS) {
        if (dds3DrawFlag & DDS3_BGFADE_COUNT_DOWN_BIT) {
            dds3BackgroundFadeCounter--;
        } else {
            dds3BackgroundFadeCounter++;
        }
        fadeRatio = (f32)dds3BackgroundFadeCounter / (f32)dds3BackgroundFadeDuration;
        dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL] = (dds3BackgroundFadeCounter << DDS3_FADE_ALPHA_SHIFT) / dds3BackgroundFadeDuration;
        dds3BackgroundFadeFirstRamp = DDS3_BGFADE_FIRST_RAMP_SCALE - fadeRatio * DDS3_BGFADE_FIRST_RAMP_SCALE;
        dds3BackgroundFadeSecondRamp = DDS3_BGFADE_SECOND_RAMP_SCALE - fadeRatio * DDS3_BGFADE_SECOND_RAMP_SCALE;
        if (((dds3DrawFlag & DDS3_BGFADE_COUNT_DOWN_BIT) && dds3BackgroundFadeCounter == 0) ||
            ((dds3DrawFlag & DDS3_BGFADE_COUNT_UP_BIT) && dds3BackgroundFadeCounter == dds3BackgroundFadeDuration)) {
            dds3BackgroundFadeCounter = 0;
            dds3BackgroundFadeDuration = 0;
            dds3DrawFlag &= DDS3_BGFADE_KEEP_OTHER_BITS;
        }
    }
}

void dds3DrawBackgroundFadeBands(void) {
    s16 firstRamp = 0;
    s16 secondRamp = 0;
    s32 fade;
    s32 alpha;

    if (dds3BackgroundFadeMode == 0) {
        fade = DDS3_FADE_MAX_ALPHA - dds3BackgroundFadeColor[DDS3_COLOR_ALPHA_CHANNEL];
    } else {
        firstRamp = dds3BackgroundFadeFirstRamp;
        fade = 0;
        secondRamp = dds3BackgroundFadeSecondRamp;
        if (firstRamp == DDS3_BGFADE_FIRST_RAMP_MAX) {
            return;
        }
    }
    if (fade >= DDS3_FADE_MAX_ALPHA) {
        return;
    }

    eglSetOt(0x4F);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    alpha = (DDS3_FADE_MAX_ALPHA - fade) << 24;
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

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3CurrentIncompleteResource);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3TextureViewerPageIndex);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CB0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", egl_OT_Offset);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CBC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3RenderTex);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3RenderTexture);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3RenderDrawEnv);

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

SDATA_PAD(4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3BackgroundFadeColor);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFB);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3BackgroundFadeMode);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3BackgroundFadeFirstRamp);

INCLUDE_SDATA(const s32, "game/code_00102DC8", dds3BackgroundFadeSecondRamp);

SDATA_PAD(2);

INCLUDE_SDATA(const s32, "game/code_00102DC8", distblurovercnt);

INCLUDE_SDATA(const s32, "game/code_00102DC8", ripblurovercnt);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D10);

