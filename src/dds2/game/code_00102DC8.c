#include "common.h"
#include "sdf.h"

typedef struct KwlnResourceNode {
    s32 unk0;
    struct KwlnResourceNode *next;
    s32 *ready;
} KwlnResourceNode;

extern s8 D_00435C60;

extern u32 kwlnTextureViewerPageIndex;

extern u32 kwlnTextureReferenceFlag;

extern u32 D_00435CC4;

extern s32 kwlnHeldTextureReference;

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

extern s16 D_00435CEC;

extern u16 D_00435CEE;

extern u32 kwlnTaskGetTimer(void);

extern s32 effMiscRandMod(s32, s32);

extern void kwlnPadStartMotor(u32, u8, s32);

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void kwlnDrawTextureListDiagnostic(void *, s32, s32);

extern u32 kwlnTextureGetPageIndex(void);

extern s32 kwlnTextureCountIncompleteResources(void);

extern s32 func_0035C860();

extern s32 sdfCreateFormattedSifCommand();

extern void sdfAppendPacket();

extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern void func_00104180(void);

extern void func_001044E8(void);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);


extern s32 D_00435BA4;

extern s32 kwlnDistanceBlurErrorCount;

extern s32 kwlnRippleBlurErrorCount;

typedef struct {
    u8 unknown[0x10];
    void (*submit)(void *, void *);
} KwlnGraphicsSink;

extern KwlnGraphicsSink D_00380708;

extern void *D_00438DA8;

extern f32 D_0037F5EC[];

extern s32 sdfResourceListHead;

extern s32 kwlnCurrentIncompleteResource;

extern s32 kwlnTextureViewerHandlePad(void);

typedef struct {
    u8 pad00[0x12];
    s8 unk12;
    s8 unk13;
    u8 unk14;   /* 0x14: -1 on the page cursor */
    u8 unk15;   /* 0x15: +1 on the page cursor */
    u8 pad16[2];
    u8 unk18;   /* 0x18: -10 on the page cursor */
    u8 unk19;
    u8 unk1A;   /* 0x1A: +10 on the page cursor */
    u8 unk1B;
} KwlnViewerPadState;

extern KwlnViewerPadState sdfPadButtonStates;

extern s32 func_00104908(s32);

extern void func_00104AA8(void *data, s32 handle);

extern u8 kwlnPositionedTextSurface[];

extern s8 D_0037F53B[];

extern void kwlnTextureAttachTask(u8 *scene);

extern s16 D_00435CFE;

extern s16 D_00435D00;

extern u8 kwlnBackgroundFadeColor[4];

extern s8 kwlnBackgroundFadeMode;

extern u8 kwlnLargeMotorTarget;

extern u8 kwlnPadMotorLevels[2];

extern void *func_00101740(const char *);
extern void *sdfAllocSizeClassBlock(s32);
extern void *func_00102BC8(void *task);
extern void dds3AdminReleaseTaskWork(void);
extern char dds3AdminTaskName[];
extern u64 sdfCreateResetPacketList(void);
extern u32 kwlnGetDrawBufferIndex(void);
extern u8 D_0043DDA0[];
extern u16 kwlnBackgroundFadeCounter;
extern u16 kwlnBackgroundFadeDuration;

extern u8 effSharedRandomState[];

extern void effMiscSeedRandom(void *data, u32 tag);

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

void kwlnDebugTaskCreate(void) {
    KwlnDebugWork *work;
    s32 i;

    if (func_00101740(dds3AdminTaskName) == NULL) {
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
        kwlnTaskCreate(dds3AdminTaskName, 2, 0, 1, func_00102BC8, dds3AdminReleaseTaskWork, work);
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00102E88);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103108);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001032F0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103388);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103430);

void func_001034E0(void) {
}

extern s8 D_0037F510[];

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
void kwlnDrawSpriteCell(void *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, func_0011F250(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Append the same cell rectangle with caller-selected depth. */
void kwlnDrawSpriteCellZ(void *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth) {
    s32 columnSpan = KWLN_CELL_COLUMN_SPAN, rowSpan = KWLN_CELL_ROW_SPAN;
    sdfAppendPacket(packetList, func_0011F250(column * 0x10 + 0x6FD0, row * 8 + 0x78E8, depth, columnCount * columnSpan + rowSpan, rowCount * rowSpan + 0x30, 0x60000000, 0x40806020));
}

/* Despite the legacy name, this periodically pulses both pad motors, not colors. */
s32 kwlnDebugPulseColors(void) {
    if (kwlnTaskGetTimer() % KWLN_DEBUG_RUMBLE_PERIOD == 0) {
        kwlnPadStartMotor(KWLN_PAD_SMALL_MOTOR, 1, KWLN_DEBUG_SMALL_PULSE_FRAMES);
        kwlnPadStartMotor(KWLN_PAD_LARGE_MOTOR, (u8)(effMiscRandMod(0, KWLN_DEBUG_RUMBLE_VARIATION) + KWLN_DEBUG_RUMBLE_BASE), KWLN_DEBUG_LARGE_PULSE_FRAMES);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001038D0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103A00);

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

extern u8 D_00438D98[2];
extern u32 D_00438DA0[2];
extern s32 fileTestSavedSlotFlags();
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


INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104058);

u32 func_00104150(void) {
    return 0;
}

/* Seed the shared effect random state with the fixed initialization seed. */
void kwlnInitMagicState(void) {
    effMiscSeedRandom(effSharedRandomState, KWLN_SHARED_RANDOM_SEED);
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104180);

void func_001044E8(void) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void kwlnDebugGraphSetEnabled(s8 mode) {
    if (mode == 1) {
        D_00438DA8 = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, func_00104180, func_001044E8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_00438DA8, 0);
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
    func_0035C860(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x7180, 0x79C0, KWLN_DIAG_DEPTH, 0, pageText));
}

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104700);

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

/* DDS2 first scans readiness at the head, even for requested index zero.
 * This assumes a nonempty list; initial exhaustion returns 0 before selection writes. */
s32 func_00104908(s32 requestedIndex) {
    KwlnResourceNode *resourceNode = (KwlnResourceNode *)sdfResourceListHead;
    KwlnResourceNode *nextNode;
    s32 visitedIndex = 0;

    while (resourceNode->ready != NULL && *resourceNode->ready != 0) {
        nextNode = resourceNode->next;
        if (nextNode == NULL) {
            return 0;
        }
        resourceNode = nextNode;
    }
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
    kwlnTextureViewerPageIndex = func_00104908(kwlnTextureViewerPageIndex);
    if (sdfPadButtonStates.unk12 < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104AA8);

/* With a nonempty list, invoke its renderer even when input reports exit.
 * surfaceAddress identifies the submission callback owner; return input status. */
s32 kwlnSwapActiveResource(s32 surfaceAddress) {
    s32 inputStatus;

    if (sdfResourceListHead == 0) {
        return 0;
    }
    inputStatus = kwlnTextureViewerHandlePad();
    func_00104AA8((void *)surfaceAddress, kwlnCurrentIncompleteResource);
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
    func_00104AA8(kwlnPositionedTextSurface, kwlnCurrentIncompleteResource);
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
    D_00435C60 = 1;
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
void func_00104D40(void *packetList, s32 x, s32 y, SdfTexHead *block, u8 mode) {
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
        sdfAppendPacket(packetList, func_0011F250(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, KWLN_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            func_00104D40(packetList, x, y, block, 0);
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
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105070);

extern void sdfGraphSetDisplayMode(s32);
extern void sdfCameraBuildProjection(void *);
extern void func_00105070(s32);
extern void func_001057B0(void);
extern u8 sdfSceneProjectionParameters[];
extern u8 D_0037F980[];
extern u8 D_00435BC8;
extern void evtEnsureDrawVectorState(void);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void evtResetDisplayProjectionAndVectorState(void) {
    sdfGraphSetDisplayMode(1);
    sdfCameraBuildProjection(sdfSceneProjectionParameters);
    sdfCameraBuildProjection(D_0037F980);
    func_00105070(0);
    func_00105070(1);
    func_001057B0();
    D_00435BC8 = 0;
    evtEnsureDrawVectorState();
}


INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105290);

extern u16 D_00435CCC;
extern u16 D_00435CCE;
extern void kwlnTextureReleaseHeldReference(void);
extern void func_00105290(void);
extern u32 sdfAllocImageBuffer(u32 width, u32 height, u32 mode);
extern s32 sdfTexCreateResourceWithReference(u32 width, u32 height, u32 a, u32 b, u32 buffer, u32 c, u32 d, u32 e);
extern void sdfTexSetClampMode(s32 texture, s32 mode);
extern void sdfTexCreateFirstPacket(s32 texture);
extern f32 D_0037F7B0[];

/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value) {
    s32 textureHandle;

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
    textureHandle = sdfTexCreateResourceWithReference(width, height, 0, 0, D_00435CC4, 0, 0, 0);
    if (textureHandle == 0) {
        return 0;
    }
    kwlnHeldTextureReference = textureHandle;
    sdfTexSetClampMode(textureHandle, KWLN_HELD_TEXTURE_CLAMP_MODE);
    sdfTexCreateFirstPacket(textureHandle);
    D_0037F7B0[1] = value;
    D_0037F7B0[6] = width;
    D_0037F7B0[7] = height;
    func_00105290();
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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105550);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001057B0);

/* Append the selected state's packet for the current draw buffer to a new list. */
u64 evtBuildFrameStatePacketList(s32 stateIndex) {
    u64 packetList = sdfCreateResetPacketList();

    sdfAppendPacket(packetList, D_0043DDA0 + stateIndex * KWLN_FRAME_STATE_BYTES + kwlnGetDrawBufferIndex() * KWLN_FRAME_BUFFER_BYTES);
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
void kwlnFadeSetColor(s8 red, s8 green, s8 blue, s8 alpha) {
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
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~KWLN_FADE_COUNT_DOWN_BIT) | KWLN_FADE_COUNT_UP_BIT;
}

/* Return whether either foreground counter direction is enabled. */
u8 kwlnFadeIsActive(void) {
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

    if (D_00435BA4 != 0) {
        if (kwlnDistanceBlurErrorCount != 0 || kwlnRippleBlurErrorCount != 0) {
            packetList = sdfAllocPacketAligned(KWLN_DIAG_PACKET_LIST_BYTES);
            sdfInitPacketList(packetList);
            if (kwlnDistanceBlurErrorCount > 0) {
                sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", kwlnDistanceBlurErrorCount));
            }
            if (kwlnRippleBlurErrorCount > 0) {
                sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", kwlnRippleBlurErrorCount));
            }
            D_00380708.submit(&D_00380708, packetList);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105CF8);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106288);

void func_001063A8(f32 value) {
    D_0037F5EC[0] = value;
}

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C20);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C24);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C28);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C2C);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C2F);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C30);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C31);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C3F);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C40);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C41);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C42);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C49);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C50);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435C54);

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

