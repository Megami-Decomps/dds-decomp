#include "common.h"

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
extern s16 D_003BA91C;
extern u16 D_003BA91E;

extern u32 kwlnTextureReferenceFlag;

extern u32 D_003BA8F4;

extern s32 kwlnHeldTextureReference;

extern u32 D_003BA8DC;
extern s8 D_003BA890;

extern s8 D_003BA7FC;

extern s8 kwlnBackgroundFadeMode;

extern s16 D_003BA92E;

extern s16 D_003BA930;
extern u8 kwlnBackgroundFadeColor[4];

extern s8 D_0032453B[];

extern f32 D_003245EC[];

extern f32 D_003245E0[];

extern f32 D_00324980[];

extern u8 kwlnPositionedTextSurface[];

extern u8 D_003BD690[2];

extern s32 D_003BD6A0[2];

extern u8 kwlnLargeMotorTarget;
extern u16 kwlnFadeCounter;

extern u16 kwlnFadeDuration;
extern u16 kwlnBackgroundFadeCounter;
extern u16 kwlnBackgroundFadeDuration;

extern s32 sdfResourceListHead;
extern s32 kwlnCurrentIncompleteResource;
extern u8 effSharedRandomState[];
extern void func_002E8430(void *data, u32 tag);
extern s32 kwlnTextureViewerHandlePad(void);
extern void func_00104B88(void *data, s32 handle);

extern void kwlnTextureAttachTask(u8 *scene);

extern void func_00105150(s32 arg0);

extern void func_00105890(void);

extern void evtEnsureDrawVectorState(void);

extern void sdfGraphSetDisplayMode(s32 arg0);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void func_00105010(void *, s32, s32);
extern void sdfAppendPacket();
extern s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);
extern u32 kwlnTaskGetTimer(void);
extern s32 effMiscRandMod(s32, s32);
extern void func_00104068(s32, u8, s32);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);
extern void func_00104290(void);
extern void *D_003BD6A8;
extern s32 kwlnTextureCountIncompleteResources(void);
extern s32 func_003014F0();
extern s32 sdfCreateFormattedSifCommand();
extern u64 sdfCreateResetPacketList(void);
extern u32 func_00100518(void);
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

extern char D_003BA848[];
extern void *kwlnTaskGetTaskByName(const char *);
extern void func_00102CD8(void);
extern void dds3AdminReleaseTaskWork(void);
extern void *func_002CFEB8(s32);

void kwlnDebugTaskCreate(void) {
    KwlnDebugWork *work;
    s32 i;

    if (kwlnTaskGetTaskByName(D_003BA848) == NULL) {
        work = func_002CFEB8(0x24);
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
        kwlnTaskCreate(D_003BA848, 2, 0, 1, func_00102CD8, dds3AdminReleaseTaskWork, work);
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00102F98);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103160);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103218);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103400);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103498);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103540);

void func_001035F0(void) {
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001035F8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001037C0);

void kwlnDrawSpriteCell(u32 list, s32 col, s32 row, s32 cols, s32 rows) {
    s32 cw = 0xC0, ch = 0x60;
    sdfAppendPacket(list, func_0011D3E8(col * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF,
                                           cols * cw + ch, rows * ch + 0x30,
                                           0x60000000, 0x40806020));
}

void kwlnDrawSpriteCellZ(u32 list, s32 col, s32 row, s32 cols, s32 rows, s32 z) {
    s32 cw = 0xC0, ch = 0x60;
    sdfAppendPacket(list, func_0011D3E8(col * 0x10 + 0x6FD0, row * 8 + 0x78E8, z,
                                           cols * cw + ch, rows * ch + 0x30,
                                           0x60000000, 0x40806020));
}

s32 kwlnDebugPulseColors(void) {
    if (kwlnTaskGetTimer() % 0x4650 == 0) {
        func_00104068(0, 1, 0xF);
        func_00104068(1, (u8)(effMiscRandMod(0, 150) + 100), 30);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001039E0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103B10);

void kwlnPadStepLargeMotorLevel(void) {
    u8 target = kwlnLargeMotorTarget;

    if (D_003BD690[1] < target) {
        if (target - D_003BD690[1] > 0x20) {
            D_003BD690[1] = D_003BD690[1] + 0x20;
        } else {
            D_003BD690[1] = target;
        }
    } else if (target < D_003BD690[1]) {
        if (D_003BD690[1] - target > 0x20) {
            D_003BD690[1] = D_003BD690[1] - 0x20;
        } else {
            D_003BD690[1] = target;
        }
    } else {
        D_003BD690[1] = target;
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104068);

void kwlnPadResetMotorLevelsAndOutput(void) {
    D_003BD690[0] = 0;
    D_003BD6A0[0] = 0;
    D_003BD690[1] = 0;
    D_003BD6A0[1] = 0;
    sdfDevConsSetEntryPair(0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104168);

u32 func_00104260(void) {
    return 0;
}

void kwlnInitMagicState(void) {
    func_002E8430(effSharedRandomState, 0x12345678);
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104290);

void func_001045F8(void) {
}

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

/* Draw a texture-sized outline: scale the longer side down to 0x100 texels (0x400 is treated as 0x3FF) and submit one textured line packet. */
void func_00104678(s32 list, KwlnImageSize *image) {
    s32 width = image->width;
    s32 height = image->height;
    s32 drawWidth;
    s32 drawHeight;

    sdfConsCreateDrawPacket(list, image, 0);
    drawWidth = width * 0x10;
    drawHeight = height * 8;
    if (width < height) {
        if (height > 0x100) {
            drawHeight = 0x800;
            drawWidth = (width << 8) / height * 0x10;
        }
    } else if (width > 0x100) {
        drawWidth = 0x1000;
        drawHeight = (height << 8) / width * 8;
    }
    if (width == 0x400) {
        width--;
    }
    if (height == 0x400) {
        height--;
    }
    sdfAppendTexturedLinePacket(list, 0x80808080, 0, 0x7180, 0x7A60, 0, 0, drawWidth + 0x7180, drawHeight + 0x7A60,
                                width * 0x10, height * 0x10, 0x0FFFFF80, 0);
}

void kwlnTextureDrawPageCounter(void *task) {
    char buffer[0x70];
    s32 current = kwlnTextureGetPageIndex();
    s32 count = kwlnTextureCountIncompleteResources();
    func_003014F0(buffer, "TEX VIEWER [%d/%d]", current, count - 1);
    sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x7180, 0x79C0, 0xFFFFF80, 0, buffer));
}

INCLUDE_RODATA(const s32, "game/code_00102ED8", D_0039E078);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104810);

s32 kwlnTextureCountIncompleteResources(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)sdfResourceListHead;
    KwlnResourceNode *next;
    s32 count = 0;
    while (node != NULL) {
        while (node->ready != NULL && *node->ready != 0) {
            next = node->next;
            if (next == NULL) {
                return count;
            }
            node = next;
        }
        count++;
        node = node->next;
    }
    return count;
}

u32 kwlnTextureGetPageIndex(void) {
    return D_003BA8DC;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104A18);

s32 kwlnTextureViewerHandlePad(void) {
    if (sdfPadButtonStates.unk13 < 0) {
        return 0;
    }
    if (sdfPadButtonStates.unk14 & 2) {
        D_003BA8DC -= 1;
    } else if (sdfPadButtonStates.unk15 & 2) {
        D_003BA8DC += 1;
    } else if ((sdfPadButtonStates.unk18 & 2) || (sdfPadButtonStates.unk19 & 2)) {
        D_003BA8DC -= 10;
    } else if ((sdfPadButtonStates.unk1A & 2) || (sdfPadButtonStates.unk1B & 2)) {
        D_003BA8DC += 10;
    }
    D_003BA8DC = func_00104A18(D_003BA8DC);
    if (sdfPadButtonStates.unk12 < 0) {
        D_003BA890 ^= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104B88);

s32 kwlnSwapActiveResource(s32 resource) {
    s32 status;

    if (sdfResourceListHead == 0) {
        return 0;
    }
    status = kwlnTextureViewerHandlePad();
    func_00104B88((void *)resource, kwlnCurrentIncompleteResource);
    return status;
}

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

s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)sdfResourceListHead;
    if (node == NULL) {
        return NULL;
    }
    kwlnCurrentIncompleteResource = (s32)node;
    D_003BA890 = 1;
    D_003BA8DC = 0;
    while (node->ready != NULL && *node->ready != 0) {
        node = ((KwlnResourceNode *)kwlnCurrentIncompleteResource)->next;
        if (node == NULL) {
            return NULL;
        }
        kwlnCurrentIncompleteResource = (s32)node;
    }
    return kwlnLoadDefaultResource;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104E20);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105010);

void kwlnTextureAttachTask(u8 *scene) {
    void *task = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(task);
    func_00105010(task, 0x7180, 0x79C0);
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

s32 kwlnEnsureDefaultResource(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(kwlnPositionedTextSurface);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105150);

void evtResetDisplayProjectionAndVectorState(void) {
    sdfGraphSetDisplayMode(1);
    sdfCameraBuildProjection(&D_003245E0);
    sdfCameraBuildProjection(&D_00324980);
    func_00105150(0);
    func_00105150(1);
    func_00105890();
    D_003BA7FC = 0;
    evtEnsureDrawVectorState();
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105370);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001054D0);

/* Release the retained texture and clear its request/pending flags. */
void kwlnTextureReleaseHeldReference(void) {
    if (kwlnHeldTextureReference != 0) {
        sdfTexReleaseReference(kwlnHeldTextureReference);
    }
    kwlnHeldTextureReference = 0;
    D_003BA8F4 = 0;
    kwlnTextureReferenceFlag = 0;
}

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

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105630);

/* The frame count is stored in tenths before deriving the forty-percent mark. */
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
    D_003BA91E = frames * 10;
    D_003BA91C = D_003BA91E * 4 / 10;
}

void func_00105888(void) {
    D_003BA918 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105890);

u64 evtBuildFrameStatePacketList(s32 stateIndex) {
    u64 list = sdfCreateResetPacketList();

    sdfAppendPacket(list, D_003C2620 + stateIndex * 0x160 + func_00100518() * 0xB0);
    return list;
}

/* The low two bits select the active fade direction; clearing cancels it. */
void kwlnFadeClear(void) {
    kwlnDrawControlFlags &= ~3;
    kwlnFadeColor.r = 0;
    kwlnFadeColor.g = 0;
    kwlnFadeColor.b = 0;
    kwlnFadeColor.a = 0;
}

void kwlnFadeSetColor(s8 red, s8 green, s8 blue, s8 alpha) {
    kwlnDrawControlFlags &= ~3;
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = alpha;
}

void kwlnFadeGetColor(KwlnFadeColor **color) {
    *color = &kwlnFadeColor;
}

void kwlnFadeSetRGB(s8 red, s8 green, s8 blue) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
}

void kwlnFadeOutStart(s8 red, s8 green, s8 blue, s32 duration) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = -0x80;
    if (duration == 0) {
        kwlnFadeColor.a = 0;
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
        kwlnFadeClear();
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = duration;
    kwlnDrawControlFlags = (kwlnDrawControlFlags | 1) & ~2;
}

/* Enter the first fade direction; a zero duration finishes immediately. */
void kwlnFadeStartIn(s32 duration) {
    kwlnFadeColor.a = -0x80;
    if (duration == 0) {
        kwlnFadeColor.a = 0;
        kwlnDrawControlFlags &= ~3;
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
    } else {
        kwlnFadeDuration = duration;
        kwlnFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | 1) & ~2;
    }
}

/* Select the second fade direction using the specified RGB color. */
void kwlnFadeInStart(s8 red, s8 green, s8 blue, s32 duration) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = 0;
    if (duration == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
        kwlnFadeColor.a = -0x80;
        kwlnDrawControlFlags &= ~3;
        return;
    }
    kwlnFadeDuration = duration;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~1) | 2;
}

/* Enter the second fade direction; a zero duration finishes immediately. */
void kwlnFadeStartOut(s32 duration) {
    kwlnFadeColor.a = 0;
    if (duration == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeColor.a = -0x80;
        kwlnDrawControlFlags &= ~3;
        kwlnFadeDuration = 0;
    } else {
        kwlnFadeDuration = duration;
        kwlnFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & ~1) | 2;
    }
}

u8 kwlnFadeIsActive(void) {
    return (kwlnDrawControlFlags & 3) != 0;
}

void kwlnFadeUpdate(void) {
    if (kwlnFadeIsActive() != 0) {
        if (kwlnDrawControlFlags & 1) {
            kwlnFadeCounter -= 1;
        } else {
            kwlnFadeCounter += 1;
        }
        kwlnFadeColor.a = (kwlnFadeCounter << 7) / kwlnFadeDuration;
        if (((kwlnDrawControlFlags & 1) && kwlnFadeCounter == 0) || ((kwlnDrawControlFlags & 2) && kwlnFadeCounter == kwlnFadeDuration)) {
            kwlnFadeCounter = 0;
            kwlnFadeDuration = 0;
            kwlnDrawControlFlags &= ~3;
        }
    }
}

void kwlnDrawBlurErrorCounters(void) {
    void *task;

    if (D_003BA724 != 0) {
        if (kwlnDistanceBlurErrorCount != 0 || kwlnRippleBlurErrorCount != 0) {
            task = sdfAllocPacketAligned(0x20);
            sdfInitPacketList(task);
            if (kwlnDistanceBlurErrorCount > 0) {
                sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", kwlnDistanceBlurErrorCount));
            }
            if (kwlnRippleBlurErrorCount > 0) {
                sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", kwlnRippleBlurErrorCount));
            }
            D_00325708.submit(&D_00325708, task);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105DD8);

/* Reset the background fade's color bytes and its two parameter words. */
void kwlnFadeResetBackground(void) {
    kwlnDrawControlFlags &= 0xF3FFFFFF;
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[3] = 0;
    D_003BA92E = 0x31;
    D_003BA930 = 0x4F;
}

void kwlnFadeBackgroundStartOut(s32 duration) {
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[3] = 0x80;
    if (duration == 0) {
        kwlnBackgroundFadeColor[3] = 0;
        kwlnBackgroundFadeCounter = 0;
        kwlnBackgroundFadeDuration = 0;
        D_003BA92E = 0x31;
        D_003BA930 = 0x4F;
        kwlnFadeResetBackground();
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | 0x04000000) & 0xF7FFFFFF;
    }
    D_003245EC[2] = 2048.0f;
}

void kwlnFadeBackgroundStartIn(s32 duration) {
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[3] = 0;
    if (duration == 0) {
        kwlnBackgroundFadeCounter = 0;
        kwlnBackgroundFadeDuration = 0;
        kwlnBackgroundFadeColor[3] = 0x80;
        kwlnDrawControlFlags &= 0xF3FFFFFF;
        D_003BA92E = 0;
        D_003BA930 = 0;
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & 0xFBFFFFFF) | 0x08000000;
    }
    D_003245EC[2] = 2041.0f;
}

s32 kwlnFadeIsBackgroundOverlayActive(void) {
    if (kwlnDrawControlFlags & 0x0C000000) {
        return 1;
    }
    if (kwlnBackgroundFadeMode == 0) {
        if (0x80 - kwlnBackgroundFadeColor[3] >= 0x80) {
            return 0;
        }
    } else if (D_003BA92E == 0x31) {
        return 0;
    }
    return 1;
}

void kwlnFadeSetMode(s32 mode) {
    kwlnBackgroundFadeMode = mode;
    if (kwlnBackgroundFadeMode == 0) {
        D_003BA92E = 0;
        D_003BA930 = 0;
    } else {
        kwlnBackgroundFadeColor[3] = 0x80;
    }
}

/* Step the background fade counter in the direction chosen by the control flags and derive the fade alpha and the two ramp values. */
void func_00106268(void) {
    f32 ratio;

    if (kwlnDrawControlFlags & 0x0C000000) {
        if (kwlnDrawControlFlags & 0x04000000) {
            kwlnBackgroundFadeCounter--;
        } else {
            kwlnBackgroundFadeCounter++;
        }
        ratio = (f32)kwlnBackgroundFadeCounter / (f32)kwlnBackgroundFadeDuration;
        kwlnBackgroundFadeColor[3] = (kwlnBackgroundFadeCounter << 7) / kwlnBackgroundFadeDuration;
        D_003BA92E = 49.0f - ratio * 49.0f;
        D_003BA930 = 79.0f - ratio * 79.0f;
        if (((kwlnDrawControlFlags & 0x04000000) && kwlnBackgroundFadeCounter == 0) ||
            ((kwlnDrawControlFlags & 0x08000000) && kwlnBackgroundFadeCounter == kwlnBackgroundFadeDuration)) {
            kwlnBackgroundFadeCounter = 0;
            kwlnBackgroundFadeDuration = 0;
            kwlnDrawControlFlags &= 0xF3FFFFFF;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106368);

void func_00106488(f32 value) {
    D_003245EC[0] = value;
}

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

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8DC);

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

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92B);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnBackgroundFadeMode);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92E);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA930);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnDistanceBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102ED8", kwlnRippleBlurErrorCount);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA940);

