#include "common.h"
#include "sdf.h"
#include "pcp_vu0.h"

typedef struct {
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} KwlnFadeColor;

typedef struct KwlnResourceNode {
    s32 unk0;
    struct KwlnResourceNode *next;
    s32 *ready;
} KwlnResourceNode;

extern u32 kwlnDrawControlFlags;

extern KwlnFadeColor kwlnFadeColor;

extern u16 D_003BA918;
extern s16 D_003BA91A;
extern u16 D_003BA91C;
extern u16 D_003BA91E;

extern u32 kwlnTextureReferenceFlag;

extern u32 D_003BA8F4;

extern s32 kwlnHeldTextureReference;

extern u32 kwlnTextureViewerPageIndex;
extern s8 D_003BA890;

extern s8 D_003BA7FC;

extern s8 kwlnBackgroundFadeMode;

extern s16 D_003BA92E;

extern s16 D_003BA930;
extern u8 kwlnBackgroundFadeColor[4];

extern s8 D_0032453B[];

extern f32 D_003245EC[];

extern f32 sdfSceneProjectionParameters[];

extern f32 D_00324980[];

extern u8 kwlnPositionedTextSurface[];

extern u8 kwlnPadMotorLevels[2];

extern s32 D_003BD6A0[2];

extern u8 kwlnLargeMotorTarget;
extern u16 kwlnFadeCounter;

extern u16 kwlnFadeDuration;
extern u16 kwlnBackgroundFadeCounter;
extern u16 kwlnBackgroundFadeDuration;

extern s32 sdfResourceListHead;
extern s32 kwlnCurrentIncompleteResource;
extern u8 effSharedRandomState[];
extern void effMiscSeedRandom(void *data, u32 tag);
extern s32 kwlnTextureViewerHandlePad(void);
extern void func_00104B88(void *data, s32 handle);

extern void kwlnTextureAttachTask(u8 *scene);

extern void func_00105150(s32 arg0);

extern void func_00105890(void);

extern void evtEnsureDrawVectorState(void);

extern void sdfGraphSetDisplayMode(s32 arg0);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void kwlnDrawTextureListDiagnostic(void *, s32, s32);
extern void sdfAppendPacket();
extern s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);
extern u32 kwlnTaskGetTimer(void);
extern s32 effMiscRandMod(s32, s32);
extern void kwlnPadStartMotor(u32, u8, s32);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);
extern void func_00104290(void);
extern void *D_003BD6A8;
extern s32 kwlnTextureCountIncompleteResources(void);
extern s32 func_003014F0();
extern s32 sdfCreateFormattedSifCommand();
extern s32 sdfCreateResetPacketList(void);
extern u32 kwlnGetDrawBufferIndex(void);
extern u8 D_003C2620[];
extern s32 D_003BA724;
extern s32 kwlnDistanceBlurErrorCount;
extern s32 kwlnRippleBlurErrorCount;
typedef struct KwlnPadState {
    u8 unk0[0x12];
    s8 unk12;
    s8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16[2];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
} KwlnPadState;
extern KwlnPadState sdfPadButtonStates;
extern s32 func_00104A18(s32);
typedef struct KwlnGraphicsSink {
    u8 unknown[0x10];
    void (*submit)(void *, void *);
} KwlnGraphicsSink;
extern KwlnGraphicsSink D_00325708;

extern void sdfCameraBuildProjection(void *arg0);

extern void sdfDevConsSetEntryPair(s32 arg0, s32 arg1, s32 arg2);

typedef struct KwlnDebugWork {
    s32 unk0;
    s32 unk4;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    u8 padB;
    s8 slotA[8];
    s8 slotB[8];
    s32 unk1C;
    s8 unk20;
} KwlnDebugWork;

extern char dds3AdminTaskName[];
extern void *kwlnTaskGetTaskByName(const char *);
extern void *dds3AdminPollModeCompletion(void *task);
extern void dds3AdminReleaseTaskWork(void);
extern void *sdfAllocSizeClassBlock(s32);

void kwlnDebugTaskCreate(void) {
    KwlnDebugWork *work;
    s32 i;

    if (kwlnTaskGetTaskByName(dds3AdminTaskName) == NULL) {
        work = sdfAllocSizeClassBlock(0x24);
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk8 = -1;
        work->unk9 = -1;
        work->unkA = 0;
        for (i = 0; i < 8; i++) {
            work->slotA[i] = -1;
            work->slotB[i] = 0;
        }
        work->unk1C = 0;
        work->unk20 = 0;
        kwlnTaskCreate(dds3AdminTaskName, 2, 0, 1, dds3AdminPollModeCompletion, dds3AdminReleaseTaskWork, work);
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00102F98);

typedef struct KwlnNamedSlot {
    s32 kind;
    char name[32];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 previous;
    s32 next;
} KwlnNamedSlot;

extern KwlnNamedSlot D_003C1C90[];
extern s32 D_003BA850;

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
    s32 slotIndex = D_003BA850;
    s32 characterIndex;

    while (slotIndex >= 0) {
        characterIndex = 0;
        while (D_003C1C90[slotIndex].name[characterIndex] != '\0') {
            if (name[characterIndex] != D_003C1C90[slotIndex].name[characterIndex]) {
                break;
            }
            characterIndex++;
        }
        if (D_003C1C90[slotIndex].name[characterIndex] == '\0' && name[characterIndex] == '\0') {
            return slotIndex;
        }
        slotIndex = D_003C1C90[slotIndex].next;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103218);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103400);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103498);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103540);

void func_001035F0(void) {
}

extern s8 D_00324510[];

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
    backwardState = D_00324510[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
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
    } else if (D_00324510[backwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
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
    forwardState = D_00324510[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET];
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
    } else if (D_00324510[forwardInput + padSet * KWLN_PAD_INPUTS_PER_SET] & KWLN_PAD_REPEAT_BIT) {
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
        if (D_00324510[1] < 0) {
            return 1;
        }
        if (D_00324510[3] < 0) {
            return -1;
        }
    } else {
        if (D_00324510[KWLN_ALT_CONFIRM_INPUT] < 0) {
            return 1;
        }
        if (D_00324510[KWLN_ALT_CANCEL_INPUT] < 0) {
            return -1;
        }
    }
    return 0;
}

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void kwlnDrawSpriteCell(u32 packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, func_0011D3E8(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void kwlnDrawSpriteCellZ(u32 packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, func_0011D3E8(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth,
                                           columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30,
                                           0x60000000, 0x40806020));
}

/* Despite the legacy name, this periodically pulses both pad motors, not colors. */
s32 kwlnDebugPulseColors(void) {
    if (kwlnTaskGetTimer() % KWLN_DEBUG_RUMBLE_PERIOD == 0) {
        kwlnPadStartMotor(KWLN_PAD_SMALL_MOTOR, 1, KWLN_DEBUG_SMALL_PULSE_FRAMES);
        kwlnPadStartMotor(KWLN_PAD_LARGE_MOTOR, (u8)(effMiscRandMod(0, KWLN_DEBUG_RUMBLE_VARIATION) + KWLN_DEBUG_RUMBLE_BASE), KWLN_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001039E0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103B10);

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

extern u8 D_003BD698[2];
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
        D_003BD698[KWLN_PAD_SMALL_MOTOR] = level;
        kwlnPadMotorLevels[KWLN_PAD_SMALL_MOTOR] = level;
    } else {
        D_003BD698[motor] = level;
        kwlnPadStepLargeMotorLevel();
    }
    D_003BD6A0[motor] = duration;
    if (motor == KWLN_PAD_SMALL_MOTOR) {
        sdfPadSetSmallMotor(0, level);
    } else {
        sdfPadSetLargeMotor(0, kwlnPadMotorLevels[motor]);
    }
}

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void kwlnPadResetMotorLevelsAndOutput(void) {
    kwlnPadMotorLevels[KWLN_PAD_SMALL_MOTOR] = 0;
    D_003BD6A0[KWLN_PAD_SMALL_MOTOR] = 0;
    kwlnPadMotorLevels[KWLN_PAD_LARGE_MOTOR] = 0;
    D_003BD6A0[KWLN_PAD_LARGE_MOTOR] = 0;
    sdfDevConsSetEntryPair(0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104168);

u32 func_00104260(void) {
    return 0;
}

/* Seed the shared effect random state with the fixed initialization seed. */
void kwlnInitMagicState(void) {
    effMiscSeedRandom(effSharedRandomState, KWLN_SHARED_RANDOM_SEED);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104290);

void func_001045F8(void) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void kwlnDebugGraphSetEnabled(s8 mode) {
    if (mode == 1) {
        D_003BD6A8 = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, func_00104290, func_001045F8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_003BD6A8, 0);
    }
}

typedef struct KwlnImageSize {
    u8 pad00[0xC];
    s16 width;  /* 0x0C */
    s16 height; /* 0x0E */
} KwlnImageSize;

extern s32 sdfConsCreateDrawPacket(s32, void *, s32);
extern void sdfAppendTexturedLinePacket(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1,
                                        s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32));

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void kwlnDrawImageOutline(s32 packetList, KwlnImageSize *image) {
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
void kwlnTextureDrawPageCounter(void *packetList) {
    char pageText[0x70];
    s32 pageIndex = kwlnTextureGetPageIndex();
    s32 resourceCount = kwlnTextureCountIncompleteResources();
    func_003014F0(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x7180, 0x79C0, KWLN_DIAG_DEPTH, 0, pageText));
}

INCLUDE_RODATA(const s32, "game/code_00102ED8", D_0039E078);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104810);

#define KWLN_DIAG_PACKET_LIST_BYTES 0x20
#define KWLN_VIEWER_FAST_PAGE_STEP 10
#define KWLN_MAP_ROW_UNITS 0x1000
#define KWLN_MAP_ROW_SHIFT 12
#define KWLN_MAP_ROW_MASK 0xFFF
#define KWLN_MAP_COLUMN_SHIFT 5
#define KWLN_MAP_COLUMN_UNITS 32
#define KWLN_MAP_COLUMN_COUNT 128
#define KWLN_MAP_ROW_SPAN 0x800
#define KWLN_MAP_BORDER_WIDTH 0x840
#define KWLN_MAP_CELL_X_STEP 16
#define KWLN_MAP_ROW_Y_STEP 8
#define KWLN_MAP_CELL_X_SHIFT 4
#define KWLN_MAP_GREEN 0x80008000
#define KWLN_MAP_YELLOW 0x80008080
#define KWLN_MAP_RED 0x80000080
#define KWLN_MAP_BLUE 0x80800000
#define KWLN_MAP_CYAN 0x80808000
#define KWLN_HELD_TEXTURE_CLAMP_MODE 5

/* Count nodes with a null readiness pointer or a zero readiness word. */
s32 kwlnTextureCountIncompleteResources(void) {
    KwlnResourceNode *resourceNode = (KwlnResourceNode *)sdfResourceListHead;
    KwlnResourceNode *nextNode;
    s32 incompleteCount = 0;
    while (resourceNode != NULL) {
        while (resourceNode->ready != NULL && *resourceNode->ready != 0) {
            nextNode = resourceNode->next;
            if (nextNode == NULL) {
                return incompleteCount;
            }
            resourceNode = nextNode;
        }
        incompleteCount++;
        resourceNode = resourceNode->next;
    }
    return incompleteCount;
}

u32 kwlnTextureGetPageIndex(void) {
    return kwlnTextureViewerPageIndex;
}

/* Clamp a negative request and return the reached index. DDS1 index zero
 * selects the head without scanning readiness; tail exits retain prior writes. */
s32 func_00104A18(s32 requestedIndex) {
    KwlnResourceNode *resourceNode = (KwlnResourceNode *)sdfResourceListHead;
    KwlnResourceNode *nextNode;
    s32 visitedIndex = 0;

    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (resourceNode != NULL && visitedIndex != requestedIndex) {
        kwlnCurrentIncompleteResource = (s32)resourceNode;
        while (resourceNode->ready != NULL && *resourceNode->ready != 0) {
            nextNode = resourceNode->next;
            if (nextNode == NULL) {
                return visitedIndex;
            }
            resourceNode = nextNode;
        }
        nextNode = resourceNode->next;
        if (nextNode == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        resourceNode = nextNode;
    }
    kwlnCurrentIncompleteResource = (s32)resourceNode;
    return visitedIndex;
}

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 kwlnTextureViewerHandlePad(void) {
    if (sdfPadButtonStates.unk13 < 0) {
        return 0;
    }
    if (sdfPadButtonStates.unk14 & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex -= 1;
    } else if (sdfPadButtonStates.unk15 & KWLN_PAD_REPEAT_BIT) {
        kwlnTextureViewerPageIndex += 1;
    } else if ((sdfPadButtonStates.unk18 & KWLN_PAD_REPEAT_BIT) || (sdfPadButtonStates.unk19 & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex -= KWLN_VIEWER_FAST_PAGE_STEP;
    } else if ((sdfPadButtonStates.unk1A & KWLN_PAD_REPEAT_BIT) || (sdfPadButtonStates.unk1B & KWLN_PAD_REPEAT_BIT)) {
        kwlnTextureViewerPageIndex += KWLN_VIEWER_FAST_PAGE_STEP;
    }
    kwlnTextureViewerPageIndex = func_00104A18(kwlnTextureViewerPageIndex);
    if (sdfPadButtonStates.unk12 < 0) {
        D_003BA890 ^= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104B88);

/* With a nonempty list, invoke its renderer even when input reports exit.
 * surfaceAddress identifies the submission callback owner; return input status. */
s32 kwlnSwapActiveResource(s32 surfaceAddress) {
    s32 inputStatus;

    if (sdfResourceListHead == 0) {
        return 0;
    }
    inputStatus = kwlnTextureViewerHandlePad();
    func_00104B88((void *)surfaceAddress, kwlnCurrentIncompleteResource);
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
    func_00104B88(kwlnPositionedTextSurface, kwlnCurrentIncompleteResource);
    return 0;
}

/* Initialize viewer state and return its default renderer for the first eligible
 * node. Exhausting a nonempty list returns NULL after the initial state writes. */
s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    KwlnResourceNode *resourceNode = (KwlnResourceNode *)sdfResourceListHead;
    if (resourceNode == NULL) {
        return NULL;
    }
    kwlnCurrentIncompleteResource = (s32)resourceNode;
    D_003BA890 = 1;
    kwlnTextureViewerPageIndex = 0;
    while (resourceNode->ready != NULL && *resourceNode->ready != 0) {
        resourceNode = ((KwlnResourceNode *)kwlnCurrentIncompleteResource)->next;
        if (resourceNode == NULL) {
            return NULL;
        }
        kwlnCurrentIncompleteResource = (s32)resourceNode;
    }
    return kwlnLoadDefaultResource;
}

typedef struct SdfTexHead {
    struct SdfTexHead *next;
    struct SdfTexHead *prev;
    s32 allocationMode;
    u32 address;
    s32 size;
    s16 width;
    s16 height;
    s32 format;
} SdfTexHead;

extern void sdfAppendFillRectanglePacket(SdfListHead *, s32, s32, s32, s32, s32, s32, s32, s32 (*)(s32));

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void func_00104E20(void *packetList, s32 x, s32 y, SdfTexHead *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch (block->allocationMode) {
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
    topY = y + (block->address >> KWLN_MAP_ROW_SHIFT) * KWLN_MAP_ROW_Y_STEP;
    columnIndex = (block->address & KWLN_MAP_ROW_MASK) >> KWLN_MAP_COLUMN_SHIFT;
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

extern SdfTexHead *sdfGetTextureListHead(void);

/* Draw the allocation-map border, then walk blocks through their prev links. */
void kwlnDrawTextureListDiagnostic(void *packetList, s32 x, s32 y) {
    SdfTexHead *block = sdfGetTextureListHead();

    if (block != NULL) {
        sdfAppendPacket(packetList, func_0011D3E8(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, KWLN_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            func_00104E20(packetList, x, y, block, 0);
            block = block->prev;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void kwlnTextureAttachTask(u8 *surface) {
    void *packetList = sdfAllocPacketAligned(KWLN_DIAG_PACKET_LIST_BYTES);
    sdfInitPacketList(packetList);
    kwlnDrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    (*(void (**)(void *, void *))(surface + 0x10))(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 kwlnEnsureDefaultResource(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105150);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void evtResetDisplayProjectionAndVectorState(void) {
    sdfGraphSetDisplayMode(1);
    sdfCameraBuildProjection(&sdfSceneProjectionParameters);
    sdfCameraBuildProjection(&D_00324980);
    func_00105150(0);
    func_00105150(1);
    func_00105890();
    D_003BA7FC = 0;
    evtEnsureDrawVectorState();
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105370);

extern u16 D_003BA8FC;
extern u16 D_003BA8FE;
extern void kwlnTextureReleaseHeldReference(void);
extern void func_00105370(void);
extern u32 sdfAllocImageBuffer(u32 width, u32 height, u32 mode);
extern s32 sdfTexCreateResourceWithReference(u32 width, u32 height, u32 a, u32 b, u32 buffer, u32 c, u32 d, u32 e);
extern void sdfTexSetClampMode(s32 texture, s32 mode);
extern void sdfTexCreateFirstPacket(s32 texture);
extern f32 D_003247B0[];

/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value) {
    s32 textureHandle;

    if (kwlnHeldTextureReference != 0) {
        kwlnTextureReleaseHeldReference();
    }
    kwlnTextureReferenceFlag = 0;
    D_003BA8FC = width;
    D_003BA8FE = height;
    D_003BA8F4 = sdfAllocImageBuffer(width, height, 0);
    if (D_003BA8F4 == 0) {
        return 0;
    }
    textureHandle = sdfTexCreateResourceWithReference(width, height, 0, 0, D_003BA8F4, 0, 0, 0);
    if (textureHandle == 0) {
        return 0;
    }
    kwlnHeldTextureReference = textureHandle;
    sdfTexSetClampMode(textureHandle, KWLN_HELD_TEXTURE_CLAMP_MODE);
    sdfTexCreateFirstPacket(textureHandle);
    D_003247B0[1] = value;
    D_003247B0[6] = width;
    D_003247B0[7] = height;
    func_00105370();
    return 1;
}

/* Release the held texture if present and clear tracking; no separate buffer free. */
void kwlnTextureReleaseHeldReference(void) {
    if (kwlnHeldTextureReference != 0) {
        sdfTexReleaseReference(kwlnHeldTextureReference);
    }
    kwlnHeldTextureReference = 0;
    D_003BA8F4 = 0;
    kwlnTextureReferenceFlag = 0;
}

/* Set the flag and return 1 when a held handle exists; otherwise leave it and return 0. */
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

s32 kwlnTextureGetHeldReference(void) {
    return kwlnHeldTextureReference;
}

extern f32 D_00324590[];
extern f32 D_003BA940[];
extern f32 effMiscRandUnitFloat(s32 mode);
extern f32 fabsf(f32 value);

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void kwlnAdvanceShakeOffsets(void) {
    f32 limits[2];
    f32 weights[2];
    s32 axis;

    memcpy(weights, D_003BA940, sizeof(weights));
    if (D_003BA918 == 0) {
        VU0_MOVE_VF(vf10, vf0);
        VU0_CLEAR_W(vf10);
        return;
    }
    if (D_003BA91A == 0) {
        D_003BA918 = 0;
        return;
    }
    limits[0] = D_003BA91C * 0.01f;
    limits[1] = D_003BA91E * 0.01f;
    for (axis = 0; axis < 2; axis++) {
        if (limits[axis] != 0.0f) {
            f32 step = effMiscRandUnitFloat(0) * limits[axis] * 0.5f + limits[axis] * 0.5f;

            if (limits[axis] * weights[axis] <= fabsf(D_00324590[axis])) {
                if (D_00324590[axis] <= 0.0f) {
                    D_00324590[axis] += step;
                } else {
                    D_00324590[axis] -= step;
                }
            } else {
                D_00324590[axis] += 2.0f * (step * (effMiscRandUnitFloat(0) - 0.5f));
            }
            if (D_00324590[axis] < -limits[axis]) {
                D_00324590[axis] = -limits[axis];
            }
            if (limits[axis] < D_00324590[axis]) {
                D_00324590[axis] = limits[axis];
            }
        }
    }
    if (D_003BA91A > 0) {
        s32 divisor = D_003BA91A + 5;

        D_003BA91C -= D_003BA91C / divisor;
        D_003BA91E -= D_003BA91E / divisor;
        D_003BA91A--;
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
        D_003BA918 = 0;
        return;
    }
    D_003BA918 = 1;
    if (mode == 0) {
        D_003BA91A = -1;
    } else {
        D_003BA91A = mode;
    }
    D_003BA91E = frames * KWLN_FADE_TICKS_PER_FRAME;
    D_003BA91C = D_003BA91E * KWLN_FADE_MARK_NUMERATOR / KWLN_FADE_TICKS_PER_FRAME;
}

/* Disable the configured fade without clearing its stored timing parameters. */
void kwlnCancelConfiguredFadeFrames(void) {
    D_003BA918 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105890);

/* Append the selected state's packet for the current draw buffer to a new list. */
s32 evtBuildFrameStatePacketList(s32 stateIndex) {
    s32 packetList = sdfCreateResetPacketList();

    sdfAppendPacket(packetList, D_003C2620 + stateIndex * KWLN_FRAME_STATE_BYTES + kwlnGetDrawBufferIndex() * KWLN_FRAME_BUFFER_BYTES);
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
void kwlnFadeOutStart(s8 red, s8 green, s8 blue, s32 duration) {
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
        kwlnDrawControlFlags &= ~KWLN_FADE_DIRECTION_BITS;
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
    } else {
        kwlnFadeDuration = duration;
        kwlnFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | KWLN_FADE_COUNT_DOWN_BIT) & ~KWLN_FADE_COUNT_UP_BIT;
    }
}

/* Start the count-up with caller RGB; zero duration leaves maximum alpha. */
void kwlnFadeInStart(s8 red, s8 green, s8 blue, s32 duration) {
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
    } else {
        kwlnFadeDuration = duration;
        kwlnFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
    }
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
    void *packetList;

    if (D_003BA724 != 0) {
        if (kwlnDistanceBlurErrorCount != 0 || kwlnRippleBlurErrorCount != 0) {
            packetList = sdfAllocPacketAligned(KWLN_DIAG_PACKET_LIST_BYTES);
            sdfInitPacketList(packetList);
            if (kwlnDistanceBlurErrorCount > 0) {
                sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", kwlnDistanceBlurErrorCount));
            }
            if (kwlnRippleBlurErrorCount > 0) {
                sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", kwlnRippleBlurErrorCount));
            }
            D_00325708.submit(&D_00325708, packetList);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105DD8);

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void kwlnFadeResetBackground(void) {
    kwlnDrawControlFlags &= KWLN_BGFADE_KEEP_OTHER_BITS;
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] = 0;
    D_003BA92E = KWLN_BGFADE_FIRST_RAMP_MAX;
    D_003BA930 = KWLN_BGFADE_SECOND_RAMP_MAX;
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
        D_003BA92E = KWLN_BGFADE_FIRST_RAMP_MAX;
        D_003BA930 = KWLN_BGFADE_SECOND_RAMP_MAX;
        kwlnFadeResetBackground();
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | KWLN_BGFADE_COUNT_DOWN_BIT) & KWLN_BGFADE_CLEAR_UP_MASK;
    }
    D_003245EC[2] = 2048.0f;
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
        D_003BA92E = 0;
        D_003BA930 = 0;
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & KWLN_BGFADE_CLEAR_DOWN_MASK) | KWLN_BGFADE_COUNT_UP_BIT;
    }
    D_003245EC[2] = 2041.0f;
}

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA850);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA854);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA858);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA85C);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA85F);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA860);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA861);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA86F);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA870);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA871);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA872);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA879);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA880);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA884);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA888);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA890);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA898);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8A0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8A8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8B0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8B8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8C0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8C8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8D0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnCurrentIncompleteResource);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnTextureViewerPageIndex);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8E0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDrawSurfaceIndex);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8EC);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnTextureReferenceFlag);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8F4);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnHeldTextureReference);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8FC);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8FE);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA900);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDrawControlFlags);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDrawOverlayEnabled);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDrawOverlayAlpha);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDrawOverlayScale);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA910);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA914);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA918);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91A);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91C);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91E);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnFadeColor);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA921);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA922);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA923);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnBackgroundFadeColor);

s32 kwlnFadeIsBackgroundOverlayActive(void) {
    if (kwlnDrawControlFlags & KWLN_BGFADE_DIRECTION_BITS) {
        return 1;
    }
    if (kwlnBackgroundFadeMode == 0) {
        if (KWLN_FADE_MAX_ALPHA - kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL] >= KWLN_FADE_MAX_ALPHA) {
            return 0;
        }
    } else if (D_003BA92E == KWLN_BGFADE_FIRST_RAMP_MAX) {
        return 0;
    }
    return 1;
}

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void kwlnFadeSetMode(s32 visibilityMode) {
    kwlnBackgroundFadeMode = visibilityMode;
    if (kwlnBackgroundFadeMode == 0) {
        D_003BA92E = 0;
        D_003BA930 = 0;
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
        D_003BA92E = KWLN_BGFADE_FIRST_RAMP_SCALE - fadeRatio * KWLN_BGFADE_FIRST_RAMP_SCALE;
        D_003BA930 = KWLN_BGFADE_SECOND_RAMP_SCALE - fadeRatio * KWLN_BGFADE_SECOND_RAMP_SCALE;
        if (((kwlnDrawControlFlags & KWLN_BGFADE_COUNT_DOWN_BIT) && kwlnBackgroundFadeCounter == 0) ||
            ((kwlnDrawControlFlags & KWLN_BGFADE_COUNT_UP_BIT) && kwlnBackgroundFadeCounter == kwlnBackgroundFadeDuration)) {
            kwlnBackgroundFadeCounter = 0;
            kwlnBackgroundFadeDuration = 0;
            kwlnDrawControlFlags &= KWLN_BGFADE_KEEP_OTHER_BITS;
        }
    }
}

void func_00106368(void) {
    s16 firstRamp = 0;
    s16 secondRamp = 0;
    s32 fade;
    s32 alpha;

    if (kwlnBackgroundFadeMode == 0) {
        fade = KWLN_FADE_MAX_ALPHA - kwlnBackgroundFadeColor[KWLN_COLOR_ALPHA_CHANNEL];
    } else {
        firstRamp = D_003BA92E;
        fade = 0;
        secondRamp = D_003BA930;
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

void func_00106488(f32 value) {
    D_003245EC[0] = value;
}

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnBackgroundFadeMode);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92E);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA930);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDistanceBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnRippleBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA940);

