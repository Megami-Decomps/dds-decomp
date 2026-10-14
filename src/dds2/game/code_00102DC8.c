#include "sdf_scene_packet.h"
#include "sdf_gs_scene_state.h"
#include "common.h"
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
#include "kwln_named_slot.h"
#include "kwln_task_create.h"
#include "kwln_fade.h"
#include "sdf.h"
#include "sdf_texture_registry.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_linked_packet.h"
#include "sdf_draw.h"
#include "sdf_gs_blend.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"

extern u8 D_00435C60;

extern u32 kwlnTextureViewerPageIndex;

extern u32 kwlnTextureReferenceFlag;

extern SdfTexResource *D_00435CC4;

extern SdfTex *kwlnHeldTextureReference;

extern u16 D_00435CE8;

extern u32 kwlnDrawControlFlags;

typedef struct {
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} KwlnFadeColor;

extern KwlnFadeColor kwlnFadeColor;

extern u16 kwlnFadeCounter;

extern u16 kwlnFadeDuration;

extern s16 D_00435CEA;

extern u16 D_00435CEC;

extern u16 D_00435CEE;



extern void kwlnPadStartMotor(u32, u8, s32);

#include "sdf_packet_alloc.h"


extern void kwlnDrawTextureListDiagnostic(void *, s32, s32);

extern u32 kwlnTextureGetPageIndex(void);

extern s32 kwlnTextureCountIncompleteResources(void);

extern s32 sprintf();

extern void *sdfCreateFormattedSifCommand(s32, s32, s32, s32, const char *, ...);


extern void *sdfCreateBorderedRectanglePacket(s32, s32, s32, s32, s32, s32, s32);

extern s32 kwlnDebugGraphUpdate(KwlnTask *task);

extern void kwlnDebugGraphTaskDestroy(KwlnTask *task);

typedef s32 (*KwlnTaskUpdateCallback)();
typedef void (*KwlnTaskDestroyCallback)();



extern s32 D_00435BA4;

extern s32 kwlnDistanceBlurErrorCount;

extern s32 kwlnRippleBlurErrorCount;

extern SdfPoolNode D_00380708;

extern KwlnTask *kwlnDebugGraphTask;

extern f32 D_0037F5EC[];


extern s32 kwlnCurrentIncompleteResource;

extern s32 kwlnTextureViewerHandlePad(void);

extern s8 sdfPadButtonStates[32];

extern s32 kwlnSelectIncompleteResourceAtPageIndex(s32);

extern void kwlnDrawTextureViewerFrame(void *data, s32 handle);

extern SdfPoolNode kwlnPositionedTextSurface;

extern s8 D_0037F53B[];

extern void kwlnTextureAttachTask(SdfPoolNode *surface);

extern s16 D_00435CFE;

extern s16 D_00435D00;

extern u8 kwlnBackgroundFadeColor[4];

extern s8 kwlnBackgroundFadeMode;

extern u8 kwlnLargeMotorTarget;

extern u8 kwlnPadMotorLevels[2];

extern s32 dds3AdminPollModeCompletion(KwlnTask *task);
extern void dds3AdminReleaseTaskWork(KwlnTask *task);
extern char dds3AdminTaskName[];
extern u8 D_0043DDA0[];
extern u16 kwlnBackgroundFadeCounter;
extern u16 kwlnBackgroundFadeDuration;

extern u8 effSharedRandomState[];

extern void effMiscSeedRandom(void *data, u32 tag);

void kwlnDebugTaskCreate(void) {
    AdminWork *work;
    s32 i;

    if (kwlnTaskGetTaskByName(dds3AdminTaskName) == NULL) {
        work = sdfAllocSizeClassBlock(0x24);
        work->flags = 0;
        work->value = 0;
        work->activeMode = -1;
        work->requestedMode = -1;
        work->historyIndex = 0;
        for (i = 0; i < 8; i++) {
            work->signedHistory[i] = -1;
            work->unsignedHistory[i] = 0;
        }
        work->requestData = 0;
        work->requestSizeLowByte = 0;
        kwlnTaskCreate(dds3AdminTaskName, 2, 0, 1, dds3AdminPollModeCompletion, dds3AdminReleaseTaskWork, (u32)work);
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnRegisterNamedSlot);

extern KwlnNamedSlot D_0043D410[];
extern s32 D_00435C20;

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
s32 kwlnFindNamedSlot(const char *name) {
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

extern char D_00411198[];
extern s32 D_00435C24;
extern s32 D_00435C28;
extern s32 D_00435C2C;

/* Unregister a named slot: stop its task, clear it and unlink it from the slot chain. */
s32 kwlnUnregisterNamedSlot(const char *name) {
    s32 slotIndex;

    if (kwlnTaskGetTaskByName(D_00411198) == NULL) {
        return 0;
    }
    slotIndex = kwlnFindNamedSlot(name);
    if (slotIndex < 0) {
        return 0;
    }
    if (D_0043D410[slotIndex].kind > 0) {
        if (D_0043D410[slotIndex].kind < 3) {
            if (kwlnTaskIsRegistered((KwlnTask *)D_0043D410[slotIndex].task) == 1) {
                kwlnTaskDestroyWithHierarchy((KwlnTask *)D_0043D410[D_00435C2C].task, 1);
            }
        }
    }
    D_0043D410[slotIndex].kind = 0;
    D_0043D410[slotIndex].name[0] = 0;
    D_0043D410[slotIndex].task = 0;
    D_0043D410[slotIndex].opaque28 = 0;
    D_0043D410[slotIndex].opaque2C = 0;
    D_0043D410[slotIndex].opaque30 = 0;
    D_0043D410[slotIndex].opaque34 = 0;
    D_0043D410[slotIndex].opaque38 = 0;
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


INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnConfigureNamedSlotMode3);

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnConfigureNamedSlotMode1);

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnConfigureNamedSlotMode2);

void kwlnRuntimeStartupHook(void) {
}

extern s8 D_0037F510[64];

/* Fresh presses wrap; repeat-bit input stops at the implemented end bounds.
 * Without scrolling, forward motion is bounded by visibleRows, not itemCount. */
void kwlnStepListCursor(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput) {
    s32 backwardState;
    s32 forwardState;

    if (cursorRow == NULL) {
        return;
    }
    if (itemCount < 2) {
        return;
    }
    backwardState = D_0037F510[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
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
    } else if (D_0037F510[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
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
    forwardState = D_0037F510[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
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
    } else if (D_0037F510[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
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
s32 kwlnStepTwoListCursors(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor) {
    kwlnStepListCursor(primaryScroll, primaryCursor, primaryCount, primaryVisible, padSet, 7, 6);
    kwlnStepListCursor(secondaryScroll, secondaryCursor, secondaryCount, secondaryVisible, padSet, 5, 4);
    if (padSet == 0) {
        if (D_0037F510[1] < 0) {
            return 1;
        }
        if (D_0037F510[3] < 0) {
            return -1;
        }
    } else {
        if (D_0037F510[KWLN_ALT_CONFIRM_INPUT] < 0) {
            return 1;
        }
        if (D_0037F510[KWLN_ALT_CANCEL_INPUT] < 0) {
            return -1;
        }
    }
    return 0;
}

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void kwlnDrawSpriteCell(SdfListHead *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void kwlnDrawSpriteCellZ(SdfListHead *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 kwlnDebugPulsePadMotors(KwlnTask *task) {
    if (kwlnTaskGetTimer(task) % KWLN_DEBUG_RUMBLE_PERIOD == 0) {
        kwlnPadStartMotor(KWLN_PAD_SMALL_MOTOR, 1, KWLN_DEBUG_SMALL_PULSE_FRAMES);
        kwlnPadStartMotor(KWLN_PAD_LARGE_MOTOR, (u8)(effMiscRandMod(0, KWLN_DEBUG_RUMBLE_VARIATION) + KWLN_DEBUG_RUMBLE_BASE), KWLN_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

extern u32 D_0037F3C0[4][16];
extern u32 D_0037F4C0[2][2];
extern s8 D_0037F4D0[2][2][16];
extern u8 D_00435C30[2][2][2];
extern s8 D_00435C38[2][2];
extern u8 D_00435C40[2][2][2];
extern u8 D_00435C48[2][2];
extern u8 D_00438D98[2];
extern u32 D_00438DA0[2];

void kwlnResetPadInputAndMotorState(void) {
    s32 port;
    s32 bank;
    s32 i;
    u32 (*inputTimers)[16] = D_0037F3C0;

    for (port = 0; port < 2; port++) {
        for (bank = 0; bank < 2; bank++) {
            for (i = 0; i < 16; i++) {
                D_0037F4D0[bank][port][i] = 0;
                D_0037F510[bank * 32 + port * 16 + i] = 0;
                inputTimers[bank * 2 + port][i] = 0;
            }
        }
        for (i = 0; i < 2; i++) {
            D_0037F4C0[port][i] = 0;
            D_00435C30[port][i][0] = 0;
            D_00435C30[port][i][1] = 0;
            D_00435C38[port][i] = 0;
            D_00435C40[port][i][0] = 0;
            D_00435C40[port][i][1] = 0;
            D_00435C48[port][i] = 0;
        }
    }
    kwlnPadMotorLevels[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438D98[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_SMALL_MOTOR] = 0;
    kwlnPadMotorLevels[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438D98[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_LARGE_MOTOR] = 0;
}

extern u8 sdfPadAnalogSticks[8];
extern f32 func_003532B8(f32);
extern void kwlnUpdatePadMotorDurations(void);

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
            if (sdfPadButtonStates[pad * 16 + i] != 0) {
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
        VU0_LOAD_VF(vf10, v);
        VU0_LENGTH_VF10(length);
        if (length >= 64.0f) {
            VU0_NORMALIZE_VF10();
            VU0_LOAD_VF(vf11, forward);
            VU0_DOT_XYZ(dot, vf10, vf11);
            if (dot < -1.0f) {
                dot = -1.0f;
            }
            if (dot > 1.0f) {
                dot = 1.0f;
            }
            angle = func_003532B8(dot);
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
                    D_0037F4D0[j][pad][i] = 1;
                    if (++D_0037F3C0[j * 2 + pad][i] == 1) {
                        D_0037F4D0[j][pad][i] = -0x7D;
                    }
                } else {
                    D_0037F4D0[j][pad][i] = 0;
                    D_0037F3C0[j * 2 + pad][i] = 0;
                }
                if (D_0037F3C0[j * 2 + pad][i] >= 8 && !(D_0037F3C0[j * 2 + pad][i] & 1)) {
                    D_0037F4D0[j][pad][i] |= 2;
                }
            }
        }
        for (i = 0; i < 2; i++) {
            f32 stickLength;

            v[0] = sticks[i][0] - 0x80;
            v[1] = 0;
            v[2] = sticks[i][1] - 0x80;
            v[3] = 0;
            VU0_LOAD_VF(vf10, v);
            VU0_LENGTH_VF10(stickLength);
            if (stickLength >= 64.0f) {
                D_00435C38[pad][i] = 1;
                if (++D_0037F4C0[pad][i] == 1) {
                    D_00435C38[pad][i] = -0x7D;
                }
            } else {
                D_00435C38[pad][i] = 0;
                D_0037F4C0[pad][i] = 0;
            }
            if (D_0037F4C0[pad][i] >= 8 && !(D_0037F4C0[pad][i] & 1)) {
                D_00435C38[pad][i] |= 2;
            }
        }
    }
    kwlnUpdatePadMotorDurations();
}

void kwlnPadStepLargeMotorLevel(void) {
    u8 target = kwlnLargeMotorTarget;

    if (kwlnPadMotorLevels[1] < target) {
        if (target - kwlnPadMotorLevels[1] > 0x20) {
            kwlnPadMotorLevels[1] = kwlnPadMotorLevels[1] + 0x20;
        } else {
            kwlnPadMotorLevels[1] = target;
        }
    } else if (target < kwlnPadMotorLevels[1]) {
        if (kwlnPadMotorLevels[1] - target > 0x20) {
            kwlnPadMotorLevels[1] = kwlnPadMotorLevels[1] - 0x20;
        } else {
            kwlnPadMotorLevels[1] = target;
        }
    } else {
        kwlnPadMotorLevels[1] = target;
    }
}

extern s32 fileTestSavedSlotFlags(u32 kind);
extern void sdfPadSetSmallMotor(s32 padIndex, u16 strength);
extern void sdfPadSetLargeMotor(s32 padIndex, u8 strength);

/* Honor the saved rumble option, normalize small-motor strength and clamp
 * motor indices to the large motor. Preserve target update before output. */
void kwlnPadStartMotor(u32 motor, u8 level, s32 duration) {
    if (fileTestSavedSlotFlags(0) == 0) {
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
        kwlnPadMotorLevels[KWLN_PAD_SMALL_MOTOR] = level;
    } else {
        D_00438D98[motor] = level;
        kwlnPadStepLargeMotorLevel();
    }
    D_00438DA0[motor] = duration;
    if (motor == KWLN_PAD_SMALL_MOTOR) {
        sdfPadSetSmallMotor(0, level);
    } else {
        sdfPadSetLargeMotor(0, kwlnPadMotorLevels[motor]);
    }
}

extern void sdfDevConsSetEntryPair(s32, s32, s32);

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void kwlnPadResetMotorLevelsAndOutput(void) {
    kwlnPadMotorLevels[KWLN_PAD_SMALL_MOTOR] = 0;
    kwlnPadMotorLevels[KWLN_PAD_LARGE_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_SMALL_MOTOR] = 0;
    D_00438DA0[KWLN_PAD_LARGE_MOTOR] = 0;
    sdfDevConsSetEntryPair(0, 0, 0);
}


void kwlnUpdatePadMotorDurations(void) {
    s32 largeMotor = KWLN_PAD_LARGE_MOTOR;
    u8 *levels = kwlnPadMotorLevels;
    u8 *targets = D_00438D98;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < KWLN_PAD_MOTOR_COUNT; motor++) {
        if (D_00438DA0[motor] != 0) {
            D_00438DA0[motor]--;
            if (D_00438DA0[motor] == 0) {
                levels[motor] = 0;
                if (motor == KWLN_PAD_SMALL_MOTOR) {
                    sdfPadSetSmallMotor(0, 0);
                } else {
                    sdfPadSetLargeMotor(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + KWLN_PAD_LARGE_MOTOR;
                if (*largeLevel != targets[KWLN_PAD_LARGE_MOTOR]) {
                    kwlnPadStepLargeMotorLevel();
                    sdfPadSetLargeMotor(0, *largeLevel);
                }
            }
        }
    }
}

s32 kwlnTaskNoopCallback(KwlnTask *task) {
    return 0;
}

/* Seed the shared effect random state with the fixed initialization seed. */
void kwlnInitMagicState(void) {
    effMiscSeedRandom(effSharedRandomState, KWLN_SHARED_RANDOM_SEED);
}

extern s16 D_00435BA8;
extern u16 D_00438A1E;
extern u16 D_00435C50;
extern u16 D_00435C52;
extern u16 D_00435C54;
extern u16 D_00435C56;
extern char D_00435C58[];

s32 kwlnDebugGraphUpdate(KwlnTask *task) {
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    SdfGsBlendPacket *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: kwlnDebugGraphUpdate". */
    s32 pixels;

    sdfInitPacketList(packetList);
    blendPacket = (SdfGsBlendPacket *)sdfAllocPacketAligned(0x40);
    sdfBuildPrimaryAlphaBlendDmaPacket(blendPacket);
    sdfAppendPacket(packetList, (u32)blendPacket);
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(0x70D0, 0x85A8,
                    0xFFFF7F, 0x1260, 0xF0, 0x20000000, 0x40806040));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(originX,
                    0x85C0, 0xFFFF80, 0, D_00435C58, (u16)D_00435BA8));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(originX,
                    0x8620, 0xFFFF80, 0, D_00435C58, D_00438A1E));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x8080,
                    0x85C0, 0xFFFF80, 6, D_00435C58, D_00435C50));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x8080,
                    0x8620, 0xFFFF80, 6, D_00435C58, D_00435C52));
    pixels = (s32)((f32)(u16)D_00435BA8 / 224.0f * 200.0f);
    sdfCreatePacketA(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00438A1E / 224.0f * 200.0f);
    sdfCreatePacketA(packetList, 0x80000080, 0, 0x73A0, 0x8650,
                    originX + ((pixels + 0x2A) << 4),
                    0x8660, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C50 / 224.0f * 200.0f);
    sdfCreatePacketA(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x85D0,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8610, 0xFF0000, NULL);
    pixels = (s32)((f32)D_00435C52 / 224.0f * 200.0f);
    sdfCreatePacketA(packetList, 0x8000FFFF, 0,
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

void kwlnDebugGraphTaskDestroy(KwlnTask *task) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void kwlnDebugGraphSetEnabled(s8 mode) {
    if (mode == 1) {
        kwlnDebugGraphTask = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, kwlnDebugGraphUpdate, kwlnDebugGraphTaskDestroy, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(kwlnDebugGraphTask, 0);
    }
}

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void kwlnDrawImageOutline(SdfListHead *packetList, SdfTex *image) {
    s32 width = image->width;
    s32 height = image->height;
    s32 drawWidth;
    s32 drawHeight;

    sdfConsCreateDrawPacket(packetList, image, 0);
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
    sdfAppendTexturedLinePacket(packetList, 0x80808080, 0, 0x7180, 0x7A60, 0, 0, drawWidth + 0x7180, drawHeight + 0x7A60,
                                width * 0x10, height * 0x10, KWLN_DIAG_DEPTH, 0);
}

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void kwlnTextureDrawPageCounter(SdfListHead *packetList) {
    char pageText[0x70];
    s32 pageIndex = kwlnTextureGetPageIndex();
    s32 resourceCount = kwlnTextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7180, 0x79C0, KWLN_DIAG_DEPTH, 0, pageText));
}

extern const char *D_0037F560[];
extern char D_004112F0[];
extern char D_00411308[];

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

void kwlnDrawTextureResourceDetails(SdfListHead *packetList, SdfTex *texture) {
    char text[0x70];
    s32 formatIndex = 0;
    s32 formatMode = 0;

    sprintf(text, "W:%4d H:%4d", texture->width, texture->height);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7180, 0x82A0, 0x0FFFFF80, 0, text));
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
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x7180, 0x8300, 0x0FFFFF80,
                                                                formatMode, D_0037F560[formatIndex]));
    sprintf(text, "ID[%2d,%2d]", texture->resourceKey, texture->battleTextureSlot);
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
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
    SdfTex *texture = sdfResourceListHead;
    SdfTex *previousTexture;
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
    SdfTex *texture = sdfResourceListHead;
    SdfTex *previousTexture;
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
        kwlnCurrentIncompleteResource = (s32)texture;
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
    kwlnCurrentIncompleteResource = (s32)texture;
    return visitedIndex;
}

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 kwlnTextureViewerHandlePad(void) {
    if (sdfPadButtonStates[0x13] < 0) {
        return 0;
    }
    if (sdfPadButtonStates[0x14] & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex -= 1;
    } else if (sdfPadButtonStates[0x15] & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex += 1;
    } else if ((sdfPadButtonStates[0x18] & KWLN_PAD_REPEAT_BIT) || (sdfPadButtonStates[0x19] & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex -= KWLN_VIEWER_FAST_PAGE_STEP;
    } else if ((sdfPadButtonStates[0x1A] & KWLN_PAD_REPEAT_BIT) || (sdfPadButtonStates[0x1B] & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex += KWLN_VIEWER_FAST_PAGE_STEP;
    }
    kwlnTextureViewerPageIndex = kwlnSelectIncompleteResourceAtPageIndex(kwlnTextureViewerPageIndex);
    if (sdfPadButtonStates[0x12] < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

extern s32 D_00435CB0;

void kwlnDrawTextureViewerFrame(void *data, s32 handle) {
    SdfPoolNode *surface;
    SdfTex *image;
    SdfListHead *packetList;
    s32 counter;
    s32 mode;

    surface = (SdfPoolNode *)data;
    image = (SdfTex *)handle;
    if (handle == 0) {
        return;
    }
    packetList = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packetList);
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    kwlnTextureDrawPageCounter(packetList);
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    kwlnDrawImageOutline(packetList, image);
    sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
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
 * surfaceAddress identifies the submission callback owner; return input status. */
s32 kwlnSwapActiveResource(s32 surfaceAddress) {
    s32 inputStatus;

    if (sdfResourceListHead == 0) {
        return 0;
    }
    inputStatus = kwlnTextureViewerHandlePad();
    kwlnDrawTextureViewerFrame((void *)surfaceAddress, kwlnCurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 kwlnLoadDefaultResource(void) {
    if (sdfResourceListHead == 0) {
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
    SdfTex *texture = sdfResourceListHead;
    if (texture == NULL) {
        return NULL;
    }
    kwlnCurrentIncompleteResource = (s32)texture;
    D_00435C60 = 1;
    kwlnTextureViewerPageIndex = 0;
    while (texture->reference != NULL && texture->reference->cloneSource != NULL) {
        texture = ((SdfTex *)(u32)kwlnCurrentIncompleteResource)->prev;
        if (texture == NULL) {
            return NULL;
        }
        kwlnCurrentIncompleteResource = (s32)texture;
    }
    return kwlnLoadDefaultResource;
}

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void kwlnDrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, SdfTexResource *block, u8 mode) {
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
            sdfAppendFillRectanglePacket(packetList, color, 0, leftX, topY,
                                         leftX + (KWLN_MAP_COLUMN_COUNT - columnIndex) * KWLN_MAP_CELL_X_STEP, topY + KWLN_MAP_ROW_Y_STEP,
                                         KWLN_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> KWLN_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfAppendFillRectanglePacket(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * KWLN_MAP_CELL_X_STEP, topY + KWLN_MAP_ROW_Y_STEP,
                                         KWLN_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += KWLN_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= KWLN_MAP_ROW_UNITS) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY, x + KWLN_MAP_ROW_SPAN, topY + KWLN_MAP_ROW_Y_STEP,
                                     KWLN_DIAG_DEPTH, NULL);
        remainingUnits -= KWLN_MAP_ROW_UNITS;
        topY += KWLN_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfAppendFillRectanglePacket(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> KWLN_MAP_COLUMN_SHIFT) << KWLN_MAP_CELL_X_SHIFT), topY + KWLN_MAP_ROW_Y_STEP,
                                     KWLN_DIAG_DEPTH, NULL);
    }
}

extern SdfTexResource *sdfGetTextureListHead(void);

/* Draw the allocation-map border, then walk blocks through their prev links. */
void kwlnDrawTextureListDiagnostic(void *packetList, s32 x, s32 y) {
    SdfTexResource *block = sdfGetTextureListHead();

    if (block != NULL) {
        sdfAppendPacket(packetList, (u32)sdfCreateBorderedRectanglePacket(x - 0x20, y - 0x10,
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
    SdfListHead *packetList = (SdfListHead *)sdfAllocPacketAligned(KWLN_DIAG_PACKET_LIST_BYTES);
    sdfInitPacketList(packetList);
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
typedef struct KwlnFrameDrawBank {
    SdfListHead initialList;
    DmaPacketHeader programReference;
    u8 packetGroups[13][0x1B0];
    SdfListHead sceneList;
    SdfLinkedPacketList linkedList;
    SdfSceneNode scene;
    ConsMatrixPacket sceneMatrix;
    u8 scenePacketStorage[0x420]; /* Light and following per-frame packets. */
    SdfListHead thirdList;
    ConsMatrixPacket thirdMatrix;
    SdfLightingPacketStorage overlayLightPacket;
} KwlnFrameDrawBank;
typedef char KwlnFrameDrawBank_size_check[sizeof(KwlnFrameDrawBank) == 0x1F40 ? 1 : -1];
typedef char MatrixPacket_size_check[sizeof(ConsMatrixPacket) == 0xD0 ? 1 : -1];

extern KwlnFrameDrawBank D_00380870[2];
extern u32 D_00435CBC;
extern SdfGraphObj D_0040B290;
extern SdfLightSources D_0037F770;
extern f32 kwlnDefaultColorVector[4];
extern u8 sdfViewMatrix[];
extern u8 D_00384790[];
extern SdfLightSources D_0037FB10;
extern f32 D_0037FB20[4];
extern void sdfConsAppendProgramReferencePacket(SdfListHead *, DmaPacketHeader *);
extern void sdfBuildLightingPacket(void *, SdfLightSources, f32 *);

/* Initialize the selected frame bank's program, scene and overlay packet chains. */
void kwlnInitializeFrameDrawPackets(s32 bufferIndex) {
    sdfInitPacketList(&D_00380870[bufferIndex].initialList);
    sdfConsAppendProgramReferencePacket(&D_00380870[bufferIndex].initialList,
                                        &D_00380870[bufferIndex].programReference);
    sdfInitPacketList(&D_00380870[bufferIndex].sceneList);
    sdfClearLinkedPacketList(&D_00380870[bufferIndex].linkedList);
    sdfInitSceneNode(&D_00380870[bufferIndex].scene, &D_0040B290);
    /* Set the RGBAQ color word while retaining its floating-point Q word. */
    *(u32 *)&D_00380870[bufferIndex].scene.centeredBounds.rgbaq.value = D_00435CBC;
    if (kwlnDrawControlFlags & 0x10000000) {
        sdfBuildCenteredViewBoundsPacket(&D_00380870[bufferIndex].scene.centeredBounds, 0, 0,
                                         D_0040B290.bufferFormat,
                                         D_0040B290.auxiliaryFormat);
        kwlnDrawControlFlags &= 0xEFFFFFFF;
    }
    sdfAppendLinkedPacketPayload(&D_00380870[bufferIndex].sceneList,
                                 &D_00380870[bufferIndex].linkedList,
                                 &D_00380870[bufferIndex].scene.link);
    sdfConsBuildMatrixPacket(&D_00380870[bufferIndex].sceneMatrix,
                             &sdfSceneProjectionParameters, sdfViewMatrix);
    sdfAppendPacket(&D_00380870[bufferIndex].sceneList, (u32)&D_00380870[bufferIndex].sceneMatrix);
    sdfBuildLightingPacket(&D_00380870[bufferIndex].scenePacketStorage, D_0037F770, kwlnDefaultColorVector);
    sdfAppendPacket(&D_00380870[bufferIndex].sceneList, (u32)&D_00380870[bufferIndex].scenePacketStorage);

    sdfInitPacketList(&D_00380870[bufferIndex].thirdList);
    sdfConsBuildMatrixPacket(&D_00380870[bufferIndex].thirdMatrix, &D_0037F980, D_00384790);
    sdfAppendPacket(&D_00380870[bufferIndex].thirdList, (u32)&D_00380870[bufferIndex].thirdMatrix);
    sdfBuildLightingPacket(&D_00380870[bufferIndex].overlayLightPacket, D_0037FB10, D_0037FB20);
    sdfAppendPacket(&D_00380870[bufferIndex].thirdList, (u32)&D_00380870[bufferIndex].overlayLightPacket);
}

extern void sdfGraphSetDisplayMode(s32);
extern void kwlnInitializeFrameDrawPackets(s32);
extern void evtInitializeFrameStatePacketTemplates(void);
extern u8 D_00435BC8;
extern void evtEnsureDrawVectorState(void);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void evtResetDisplayProjectionAndVectorState(void) {
    sdfGraphSetDisplayMode(1);
    sdfCameraBuildProjection(&sdfSceneProjectionParameters.camera);
    sdfCameraBuildProjection(&D_0037F980.camera);
    kwlnInitializeFrameDrawPackets(0);
    kwlnInitializeFrameDrawPackets(1);
    evtInitializeFrameStatePacketTemplates();
    D_00435BC8 = 0;
    evtEnsureDrawVectorState();
}


/* One 0x1F40-byte draw bank of the held-texture render target. */
typedef struct KwlnTextureBank {
    SdfListHead list;
    SdfSceneDrawPacket scene;
    ConsMatrixPacket matrix;
    SdfLightingPacketStorage lighting;
    u8 pad[0x1F40 - 0x260 - sizeof(SdfLightingPacketStorage)];
} KwlnTextureBank;

extern KwlnTextureBank D_003822A0[2];
extern SdfGraphObj D_00384730;
extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern u32 D_00435CD0;
extern u32 D_0040B2A0[];
extern SdfLightSources D_0037F940;
extern f32 D_0037F950[4];
extern u8 D_00384750[0x40];

/* Rebuild the held-texture render target's view and both frame banks. */
void kwlnInitHeldTextureScenePackets(void) {
    s32 i;

    D_00384730.width = D_00435CCC;
    D_00384730.unk2 = D_00435CCE;
    D_00384730.height = D_00435CCE;
    D_00384730.bufferFormat = 0;
    D_00384730.auxiliaryFormat = 0x30;
    D_00384730.buffers[0] = D_00435CC4;
    D_00384730.buffers[1] = 0;
    D_00384730.buffers[2] = (SdfTexResource *)D_0040B2A0[0];
    sdfCameraBuildProjection(&D_0037F7B0.camera);
    for (i = 0; i != 2; i++) {
        sdfInitPacketList(&D_003822A0[i].list);
        sdfBuildTextureScenePacket(&D_003822A0[i].scene, &D_00384730, 0);
        *(u32 *)&D_003822A0[i].scene.centeredBounds.rgbaq.value = D_00435CD0;
        sdfAppendPacket(&D_003822A0[i].list, (u32)&D_003822A0[i].scene);
        sdfConsBuildMatrixPacket(&D_003822A0[i].matrix, &D_0037F7B0, D_00384750);
        sdfAppendPacket(&D_003822A0[i].list, (u32)&D_003822A0[i].matrix);
        sdfBuildLightingPacket(&D_003822A0[i].lighting, D_0037F940, D_0037F950);
        sdfAppendPacket(&D_003822A0[i].list, (u32)&D_003822A0[i].lighting);
    }
}

extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern void kwlnTextureReleaseHeldReference(void);
extern void kwlnInitHeldTextureScenePackets(void);
extern SdfTexResource *sdfAllocImageBuffer(s32 width, s32 height, s32 format);
extern SdfTex *sdfTexCreateResourceWithReference(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, SdfTexResource *primary, s32 paletteFormat, s32 arg6, SdfTexResource *secondary);
extern void sdfTexSetClampMode(SdfTex *texture, u8 value);
/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value) {
    SdfTex *texture;

    if (kwlnHeldTextureReference != 0) {
        kwlnTextureReleaseHeldReference();
    }
    kwlnTextureReferenceFlag = 0;
    D_00435CCC = width;
    D_00435CCE = height;
    D_00435CC4 = sdfAllocImageBuffer(width, height, 0);
    if (D_00435CC4 == 0) {
        return 0;
    }
    texture = sdfTexCreateResourceWithReference(width, height, 0, 0, D_00435CC4, 0, 0, 0);
    if (texture == 0) {
        return 0;
    }
    kwlnHeldTextureReference = texture;
    sdfTexSetClampMode(texture, KWLN_HELD_TEXTURE_CLAMP_MODE);
    sdfTexCreateFirstPacket(texture);
    D_0037F7B0.camera.aspect = value;
    D_0037F7B0.camera.width = width;
    D_0037F7B0.camera.height = height;
    kwlnInitHeldTextureScenePackets();
    return 1;
}

/* Release the held texture if present and clear tracking; no separate buffer free. */
void kwlnTextureReleaseHeldReference(void) {
    if (kwlnHeldTextureReference != 0) {
        sdfTexReleaseReference(kwlnHeldTextureReference);
    }
    kwlnHeldTextureReference = 0;
    D_00435CC4 = 0;
    kwlnTextureReferenceFlag = 0;
}

/* Set the flag and return 1 when a held handle exists; otherwise leave it and return 0. */
#include "kwln_texture_reference_flag_api.h"

s32 kwlnTextureSetReferenceFlagIfPresent(void) {
    s32 result = 0;

    if (kwlnHeldTextureReference == 0) {
        return result;
    }
    kwlnTextureReferenceFlag = 1;
    return 1;
}

void kwlnTextureClearReferenceFlag(void) {
    kwlnTextureReferenceFlag = 0;
}

u32 kwlnTextureGetReferenceFlag(void) {
    return kwlnTextureReferenceFlag;
}

SdfTex *kwlnTextureGetHeldReference(void) {
    return kwlnHeldTextureReference;
}

extern f32 D_0037F590[];
extern f32 D_00435D10[];
extern f32 fabsf(f32 value);

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void kwlnAdvanceShakeOffsets(void) {
    f32 limits[2];
    f32 weights[2];
    s32 axis;

    memcpy(weights, D_00435D10, sizeof(weights));
    if (D_00435CE8 == 0) {
        VU0_MOVE_VF(vf10, vf0);
        VU0_CLEAR_W(vf10);
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
            f32 step = effMiscRandUnitFloat(0) * limits[axis] * 0.5f + limits[axis] * 0.5f;

            if (limits[axis] * weights[axis] <= fabsf(D_0037F590[axis])) {
                if (D_0037F590[axis] <= 0.0f) {
                    D_0037F590[axis] += step;
                } else {
                    D_0037F590[axis] -= step;
                }
            } else {
                D_0037F590[axis] += 2.0f * (step * (effMiscRandUnitFloat(0) - 0.5f));
            }
            if (D_0037F590[axis] < -limits[axis]) {
                D_0037F590[axis] = -limits[axis];
            }
            if (limits[axis] < D_0037F590[axis]) {
                D_0037F590[axis] = limits[axis];
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
#define KWLN_FRAME_STATE_BYTES 0x160
#define KWLN_FRAME_BUFFER_BYTES 0xB0
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
void kwlnFadeSetupFrames(s32 mode, s32 frames) {
    if (frames == 0) {
        D_00435CE8 = 0;
        return;
    }
    D_00435CE8 = 1;
    if (mode == 0) {
        D_00435CEA = -1;
    } else {
        D_00435CEA = mode;
    }
    D_00435CEE = frames * KWLN_FADE_TICKS_PER_FRAME;
    D_00435CEC = D_00435CEE * KWLN_FADE_MARK_NUMERATOR / KWLN_FADE_TICKS_PER_FRAME;
}

/* Disable the configured fade without clearing its stored timing parameters. */
void kwlnCancelConfiguredFadeFrames(void) {
    D_00435CE8 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", evtInitializeFrameStatePacketTemplates);

/* Append the selected state's packet for the current draw buffer to a new list. */
SdfListHead *evtBuildFrameStatePacketList(s32 stateIndex) {
    SdfListHead *packetList = sdfCreateResetPacketList();

    sdfAppendPacket(packetList, (u32)(D_0043DDA0 + stateIndex * KWLN_FRAME_STATE_BYTES + kwlnGetDrawBufferIndex() * KWLN_FRAME_BUFFER_BYTES));
    return packetList;
}

/* Cancel the foreground direction and zero RGBA; timing words are unchanged. */
void kwlnFadeClear(void) {
    kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
    kwlnFadeColor.r = 0;
    kwlnFadeColor.g = 0;
    kwlnFadeColor.b = 0;
    kwlnFadeColor.a = 0;
}

/* Store RGBA and cancel foreground direction flags without changing timing. */
void kwlnFadeSetColor(s32 red, s32 green, s32 blue, s32 alpha) {
    kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = alpha;
}

/* Return the live color address through outColor, rather than copying channels. */
void kwlnFadeGetColor(KwlnFadeColor **outColor) {
    *outColor = &kwlnFadeColor;
}

/* Replace RGB only; alpha, direction flags and timing are preserved. */
void kwlnFadeSetRGB(s8 red, s8 green, s8 blue) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
}

/* Start the countdown with caller RGB. A zero duration also clears RGB via FadeClear.
 * The signed alpha byte -0x80 carries the raw maximum-alpha value 0x80. */
void kwlnFadeOutStart(s32 red, s32 green, s32 blue, s32 duration) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = -KWLN_FADE_MAX_ALPHA;
    if (duration == 0) {
        kwlnFadeColor.a = 0;
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
        kwlnFadeClear();
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = duration;
    kwlnDrawControlFlags = (kwlnDrawControlFlags | KWLN_FADE_COUNT_DOWN_BIT) & ~KWLN_FADE_COUNT_UP_BIT;
}

/* Start the same countdown using existing RGB; zero duration preserves RGB. */
void kwlnFadeStartIn(s32 duration) {
    kwlnFadeColor.a = -KWLN_FADE_MAX_ALPHA;
    if (duration == 0) {
        kwlnFadeColor.a = 0;
        kwlnFadeCounter = 0;
        kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
        kwlnFadeDuration = 0;
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = duration;
    kwlnDrawControlFlags = (kwlnDrawControlFlags | KWLN_FADE_COUNT_DOWN_BIT) & ~KWLN_FADE_COUNT_UP_BIT;
}

/* Start the count-up with caller RGB; zero duration leaves maximum alpha. */
void kwlnFadeInStart(s32 red, s32 green, s32 blue, s32 duration) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = 0;
    if (duration == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
        kwlnFadeColor.a = -KWLN_FADE_MAX_ALPHA;
        kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
}

/* Start the same count-up using existing RGB; zero duration preserves RGB. */
void kwlnFadeStartOut(s32 duration) {
    kwlnFadeColor.a = 0;
    if (duration == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeColor.a = -KWLN_FADE_MAX_ALPHA;
        kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
        kwlnFadeDuration = 0;
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
}

/* Return whether either foreground counter direction is enabled. */
s32 kwlnFadeIsActive(void) {
    return (kwlnDrawControlFlags & KWLN_FADE_DIRECTION_BITS) != 0;
}

/* Step the u16 counter and sample alpha before testing exact completion.
 * Completion clears timing/flags but leaves the sampled endpoint alpha intact. */
void kwlnFadeUpdate(void) {
    if (kwlnFadeIsActive() != 0) {
        if (kwlnDrawControlFlags & KWLN_FADE_COUNT_DOWN_BIT) {
            kwlnFadeCounter -= 1;
        } else {
            kwlnFadeCounter += 1;
        }
        kwlnFadeColor.a = (kwlnFadeCounter << KWLN_FADE_ALPHA_SHIFT) / kwlnFadeDuration;
        if (((kwlnDrawControlFlags & KWLN_FADE_COUNT_DOWN_BIT) && kwlnFadeCounter == 0) || ((kwlnDrawControlFlags & KWLN_FADE_COUNT_UP_BIT) && kwlnFadeCounter == kwlnFadeDuration)) {
            kwlnFadeCounter = 0;
            kwlnFadeDuration = 0;
            kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
        }
    }
}

/* Submit diagnostics when enabled and either counter is nonzero; only positive
 * counters produce text, so negative-only errors still submit an empty list. */
void kwlnDrawBlurErrorCounters(void) {
    SdfListHead *packetList;

    if (D_00435BA4 != 0) {
        if (kwlnDistanceBlurErrorCount != 0 || kwlnRippleBlurErrorCount != 0) {
            packetList = (SdfListHead *)sdfAllocPacketAligned(KWLN_DIAG_PACKET_LIST_BYTES);
            sdfInitPacketList(packetList);
            if (kwlnDistanceBlurErrorCount > 0) {
                sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, D_004112F0, kwlnDistanceBlurErrorCount));
            }
            if (kwlnRippleBlurErrorCount > 0) {
                sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(0x73C0, 0x7B40, 0xFEFFFF, 4, D_00411308, kwlnRippleBlurErrorCount));
            }
            D_00380708.append(&D_00380708, packetList);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnQueueBackgroundFadeOverlayPacket);

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void kwlnFadeResetBackground(void) {
    kwlnDrawControlFlags &= KWLN_BGFADE_KEEP_OTHER_BITS;
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = 0;
    D_00435CFE = KWLN_BGFADE_FIRST_RAMP_MAX;
    D_00435D00 = KWLN_BGFADE_SECOND_RAMP_MAX;
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
        D_00435CFE = KWLN_BGFADE_FIRST_RAMP_MAX;
        D_00435D00 = KWLN_BGFADE_SECOND_RAMP_MAX;
        kwlnFadeResetBackground();
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | KWLN_BGFADE_COUNT_DOWN_BIT) & KWLN_BGFADE_CLEAR_UP_MASK;
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
        kwlnDrawControlFlags &= KWLN_BGFADE_KEEP_OTHER_BITS;
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & KWLN_BGFADE_CLEAR_DOWN_MASK) | KWLN_BGFADE_COUNT_UP_BIT;
    }
    D_0037F5EC[2] = 2041.0f;
}

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
s32 kwlnFadeIsBackgroundOverlayActive(void) {
    if (kwlnDrawControlFlags & KWLN_BGFADE_DIRECTION_BITS) {
        return 1;
    }
    if (kwlnBackgroundFadeMode == 0) {
        if (KWLN_FADE_MAX_ALPHA - kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] >= KWLN_FADE_MAX_ALPHA) {
            return 0;
        }
    } else if (D_00435CFE == KWLN_BGFADE_FIRST_RAMP_MAX) {
        return 0;
    }
    return 1;
}

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void kwlnFadeSetMode(s32 visibilityMode) {
    kwlnBackgroundFadeMode = visibilityMode;
    if (kwlnBackgroundFadeMode == 0) {
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = KWLN_FADE_MAX_ALPHA;
    }
}

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void kwlnStepBackgroundFade(void) {
    f32 fadeRatio;

    if (kwlnDrawControlFlags & KWLN_BGFADE_DIRECTION_BITS) {
        if (kwlnDrawControlFlags & KWLN_BGFADE_COUNT_DOWN_BIT) {
            kwlnBackgroundFadeCounter--;
        } else {
            kwlnBackgroundFadeCounter++;
        }
        fadeRatio = (f32)kwlnBackgroundFadeCounter / (f32)kwlnBackgroundFadeDuration;
        kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = (kwlnBackgroundFadeCounter << KWLN_FADE_ALPHA_SHIFT) / kwlnBackgroundFadeDuration;
        D_00435CFE = KWLN_BGFADE_FIRST_RAMP_SCALE - fadeRatio * KWLN_BGFADE_FIRST_RAMP_SCALE;
        D_00435D00 = KWLN_BGFADE_SECOND_RAMP_SCALE - fadeRatio * KWLN_BGFADE_SECOND_RAMP_SCALE;
        if (((kwlnDrawControlFlags & KWLN_BGFADE_COUNT_DOWN_BIT) && kwlnBackgroundFadeCounter == 0) ||
            ((kwlnDrawControlFlags & KWLN_BGFADE_COUNT_UP_BIT) && kwlnBackgroundFadeCounter == kwlnBackgroundFadeDuration)) {
            kwlnBackgroundFadeCounter = 0;
            kwlnBackgroundFadeDuration = 0;
            kwlnDrawControlFlags &= KWLN_BGFADE_KEEP_OTHER_BITS;
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
        firstRamp = D_00435CFE;
        fade = 0;
        secondRamp = D_00435D00;
        if (firstRamp == KWLN_BGFADE_FIRST_RAMP_MAX) {
            return;
        }
    }
    if (fade >= KWLN_FADE_MAX_ALPHA) {
        return;
    }

    evtSetDrawSurfaceIndex(0x4F);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    alpha = (KWLN_FADE_MAX_ALPHA - fade) << 24;
    evtSubmitDefaultDepthGradientRect(0, -2 - firstRamp, 0x200, 0x33, alpha, alpha, alpha, alpha);
    evtSubmitDefaultDepthGradientRect(0, secondRamp + 0x171, 0x200, 0x51, alpha, alpha, alpha, alpha);
}

void sdfSetViewFieldOfView(f32 value) {
    D_0037F5EC[0] = value;
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

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDrawSurfaceIndex);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CBC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnTextureReferenceFlag);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnHeldTextureReference);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CD0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDrawControlFlags);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDrawOverlayEnabled);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDrawOverlayAlpha);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDrawOverlayScale);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEA);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnFadeColor);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF1);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF2);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF3);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeColor);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFB);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnBackgroundFadeMode);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D00);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnDistanceBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102DC8", kwlnRippleBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D10);

