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

extern void func_00103F58(s32, u8, s32);

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
    u8 unk14;   /* 0x14: +1 on the page cursor */
    u8 unk15;   /* 0x15: -1 on the page cursor */
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
extern void *func_00328D68(s32);
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
        work = func_00328D68(0x24);
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

s32 kwlnFindNamedSlot(const char *name) {
    s32 slot = D_00435C20;
    s32 i;

    while (slot >= 0) {
        i = 0;
        while (D_0043D410[slot].name[i] != '\0') {
            if (name[i] != D_0043D410[slot].name[i]) {
                break;
            }
            i++;
        }
        if (D_0043D410[slot].name[i] == '\0' && name[i] == '\0') {
            return slot;
        }
        slot = D_0043D410[slot].next;
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

/* Move one list cursor (scroll offset + cursor row) with the up / down buttons of pad set `padSet`: a fresh press wraps around the list, a held repeat stops at the ends. The scroll pointer is NULL for a list without scrolling. */
void kwlnStepListCursor(s32 *scroll, s32 *cursor, s32 count, s32 visible, s32 padSet, s32 downButton, s32 upButton) {
    s32 up;
    s32 down;

    if (cursor == NULL) {
        return;
    }
    if (count < 2) {
        return;
    }
    up = D_0037F510[upButton + padSet * 16];
    if (up < 0) {
        if (scroll == NULL) {
            if (*cursor > 0) {
                (*cursor)--;
            } else if (count < visible) {
                *cursor = count - 1;
            } else {
                *cursor = visible - 1;
            }
        } else if (*cursor > 0) {
            (*cursor)--;
        } else if (*scroll > 0) {
            (*scroll)--;
        } else if (visible < count) {
            *scroll = count - visible;
            *cursor = visible - 1;
        } else {
            *cursor = count - 1;
        }
    } else if (D_0037F510[upButton + padSet * 16] & 2) {
        if (scroll == NULL) {
            if (*cursor > 0) {
                (*cursor)--;
            }
        } else if (*cursor > 0) {
            (*cursor)--;
        } else if (*scroll > 0) {
            (*scroll)--;
        }
    }
    down = D_0037F510[downButton + padSet * 16];
    if (down < 0) {
        if (scroll == NULL) {
            if (*cursor < visible - 1) {
                (*cursor)++;
            } else {
                *cursor = 0;
            }
        } else if (*cursor < visible - 1) {
            (*cursor)++;
        } else if (*scroll + visible < count) {
            (*scroll)++;
        } else {
            *scroll = 0;
            *cursor = 0;
        }
    } else if (D_0037F510[downButton + padSet * 16] & 2) {
        if (scroll == NULL) {
            if (*cursor < visible - 1) {
                (*cursor)++;
            }
        } else if (*cursor < visible - 1) {
            (*cursor)++;
        } else if (*scroll + visible < count) {
            (*scroll)++;
        }
    }
}

/* Step two list cursors (scroll + cursor each) from pad set `padSet`; returns 1 / -1 when the confirm / cancel button of that set is down, else 0. */
s32 kwlnStepTwoListCursors(s32 padSet, s32 count2, s32 count1, s32 visible2, s32 visible1, s32 *scroll2, s32 *scroll1, s32 *cursor2, s32 *cursor1) {
    kwlnStepListCursor(scroll1, cursor1, count1, visible1, padSet, 7, 6);
    kwlnStepListCursor(scroll2, cursor2, count2, visible2, padSet, 5, 4);
    if (padSet == 0) {
        if (D_0037F510[1] < 0) {
            return 1;
        }
        if (D_0037F510[3] < 0) {
            return -1;
        }
    } else {
        if (D_0037F510[0x11] < 0) {
            return 1;
        }
        if (D_0037F510[0x13] < 0) {
            return -1;
        }
    }
    return 0;
}

void kwlnDrawSpriteCell(void *list, s32 col, s32 row, s32 cols, s32 rows) {
    s32 cw = 0xC0, ch = 0x60;
    sdfAppendPacket(list, func_0011F250(col * 0x10 + 0x6FD0, row * 8 + 0x78E8, 0xFEFFFF, cols * cw + ch, rows * ch + 0x30, 0x60000000, 0x40806020));
}

void kwlnDrawSpriteCellZ(void *list, s32 col, s32 row, s32 cols, s32 rows, s32 z) {
    s32 cw = 0xC0, ch = 0x60;
    sdfAppendPacket(list, func_0011F250(col * 0x10 + 0x6FD0, row * 8 + 0x78E8, z, cols * cw + ch, rows * ch + 0x30, 0x60000000, 0x40806020));
}

s32 kwlnDebugPulseColors(void) {
    if (kwlnTaskGetTimer() % 0x4650 == 0) {
        func_00103F58(0, 1, 0xF);
        func_00103F58(1, (u8)(effMiscRandMod(0, 150) + 100), 30);
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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103F58);

extern u32 D_00438DA0[2];
extern void sdfDevConsSetEntryPair(s32, s32, s32);

void kwlnPadResetMotorLevelsAndOutput(void) {
    kwlnPadMotorLevels[0] = 0;
    kwlnPadMotorLevels[1] = 0;
    D_00438DA0[0] = 0;
    D_00438DA0[1] = 0;
    sdfDevConsSetEntryPair(0, 0, 0);
}


INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104058);

u32 func_00104150(void) {
    return 0;
}

void kwlnInitMagicState(void) {
    effMiscSeedRandom(effSharedRandomState, 0x12345678);
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104180);

void func_001044E8(void) {
}

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

/* Draw a texture-sized outline: scale the longer side down to 0x100 texels (0x400 is treated as 0x3FF) and submit one textured line packet. */
void kwlnDrawImageOutline(s32 list, KwlnImageSize *image) {
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
    func_0035C860(buffer, "TEX VIEWER [%d/%d]", current, count - 1);
    sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x7180, 0x79C0, 0xFFFFF80, 0, buffer));
}

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104700);

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
    return kwlnTextureViewerPageIndex;
}

s32 func_00104908(s32 index) {
    KwlnResourceNode *node = (KwlnResourceNode *)sdfResourceListHead;
    KwlnResourceNode *next;
    s32 count = 0;

    while (node->ready != NULL && *node->ready != 0) {
        next = node->next;
        if (next == NULL) {
            return 0;
        }
        node = next;
    }
    if (index < 0) {
        index = 0;
    }
    while (node != NULL && count != index) {
        kwlnCurrentIncompleteResource = (s32)node;
        while (node->ready != NULL && *node->ready != 0) {
            next = node->next;
            if (next == NULL) {
                return count;
            }
            node = next;
        }
        next = node->next;
        if (next == NULL) {
            return count;
        }
        count++;
        node = next;
    }
    kwlnCurrentIncompleteResource = (s32)node;
    return count;
}

s32 kwlnTextureViewerHandlePad(void) {
    if (sdfPadButtonStates.unk13 < 0) {
        return 0;
    }
    if (sdfPadButtonStates.unk14 & 2) {
        kwlnTextureViewerPageIndex -= 1;
    } else if (sdfPadButtonStates.unk15 & 2) {
        kwlnTextureViewerPageIndex += 1;
    } else if ((sdfPadButtonStates.unk18 & 2) || (sdfPadButtonStates.unk19 & 2)) {
        kwlnTextureViewerPageIndex -= 10;
    } else if ((sdfPadButtonStates.unk1A & 2) || (sdfPadButtonStates.unk1B & 2)) {
        kwlnTextureViewerPageIndex += 10;
    }
    kwlnTextureViewerPageIndex = func_00104908(kwlnTextureViewerPageIndex);
    if (sdfPadButtonStates.unk12 < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104AA8);

s32 kwlnSwapActiveResource(s32 resource) {
    s32 status;

    if (sdfResourceListHead == 0) {
        return 0;
    }
    status = kwlnTextureViewerHandlePad();
    func_00104AA8((void *)resource, kwlnCurrentIncompleteResource);
    return status;
}

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

s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)sdfResourceListHead;
    if (node == NULL) {
        return NULL;
    }
    kwlnCurrentIncompleteResource = (s32)node;
    D_00435C60 = 1;
    kwlnTextureViewerPageIndex = 0;
    while (node->ready != NULL && *node->ready != 0) {
        node = ((KwlnResourceNode *)kwlnCurrentIncompleteResource)->next;
        if (node == NULL) {
            return NULL;
        }
        kwlnCurrentIncompleteResource = (s32)node;
    }
    return kwlnLoadDefaultResource;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104D40);

extern SdfTex *sdfGetTextureListHead(void);
extern void func_00104D40(void *, s32, s32, SdfTex *, s32);

void kwlnDrawTextureListDiagnostic(void *list, s32 x, s32 y) {
    SdfTex *texture = sdfGetTextureListHead();

    if (texture != NULL) {
        sdfAppendPacket(list, func_0011F250(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, 0x440, 0x820,
                                          0x80000000, 0x80806020));
        do {
            func_00104D40(list, x, y, texture, 0);
            texture = texture->prev;
        } while (texture != NULL);
    }
}

void kwlnTextureAttachTask(u8 *scene) {
    void *task = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(task);
    kwlnDrawTextureListDiagnostic(task, 0x7180, 0x79C0);
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

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

/* Allocate a width x height image buffer and wrap it in the held texture reference; returns 1 when both exist. */
s32 kwlnCreateHeldTextureBuffer(u16 width, u16 height, f32 value) {
    s32 texture;

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
    sdfTexSetClampMode(texture, 5);
    sdfTexCreateFirstPacket(texture);
    D_0037F7B0[1] = value;
    D_0037F7B0[6] = width;
    D_0037F7B0[7] = height;
    func_00105290();
    return 1;
}

/* Release the retained texture and clear its request/pending flags. */
void kwlnTextureReleaseHeldReference(void) {
    if (kwlnHeldTextureReference != 0) {
        sdfTexReleaseReference(kwlnHeldTextureReference);
    }
    kwlnHeldTextureReference = 0;
    D_00435CC4 = 0;
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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105550);

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
    D_00435CEE = frames * 10;
    D_00435CEC = D_00435CEE * 4 / 10;
}

void func_001057A8(void) {
    D_00435CE8 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001057B0);

u64 evtBuildFrameStatePacketList(s32 stateIndex) {
    u64 list = sdfCreateResetPacketList();

    sdfAppendPacket(list, D_0043DDA0 + stateIndex * 0x160 + kwlnGetDrawBufferIndex() * 0xB0);
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
void kwlnFadeStartIn(s32 frames) {
    kwlnFadeColor.a = -0x80;
    if (frames == 0) {
        kwlnFadeColor.a = 0;
        kwlnFadeCounter = 0;
        kwlnDrawControlFlags &= ~3;
        kwlnFadeDuration = 0;
        return;
    }
    kwlnFadeDuration = frames;
    kwlnFadeCounter = frames;
    kwlnDrawControlFlags = (kwlnDrawControlFlags | 1) & ~2;
}

/* Select the second fade direction using the specified RGB color. */
void kwlnFadeInStart(s8 red, s8 green, s8 blue, s32 frames) {
    kwlnFadeColor.r = red;
    kwlnFadeColor.g = green;
    kwlnFadeColor.b = blue;
    kwlnFadeColor.a = 0;
    if (frames == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeDuration = 0;
        kwlnFadeColor.a = -0x80;
        kwlnDrawControlFlags &= ~3;
        return;
    }
    kwlnFadeDuration = frames;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~1) | 2;
}

/* Enter the second fade direction; a zero duration finishes immediately. */
void kwlnFadeStartOut(s32 frames) {
    kwlnFadeColor.a = 0;
    if (frames == 0) {
        kwlnFadeCounter = 0;
        kwlnFadeColor.a = -0x80;
        kwlnDrawControlFlags &= ~3;
        kwlnFadeDuration = 0;
        return;
    }
    kwlnFadeDuration = frames;
    kwlnFadeCounter = 0;
    kwlnDrawControlFlags = (kwlnDrawControlFlags & ~1) | 2;
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

    if (D_00435BA4 != 0) {
        if (kwlnDistanceBlurErrorCount != 0 || kwlnRippleBlurErrorCount != 0) {
            task = sdfAllocPacketAligned(0x20);
            sdfInitPacketList(task);
            if (kwlnDistanceBlurErrorCount > 0) {
                sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", kwlnDistanceBlurErrorCount));
            }
            if (kwlnRippleBlurErrorCount > 0) {
                sdfAppendPacket(task, sdfCreateFormattedSifCommand(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", kwlnRippleBlurErrorCount));
            }
            D_00380708.submit(&D_00380708, task);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105CF8);

/* Reset the background fade's color bytes and its two parameter words. */
void kwlnFadeResetBackground(void) {
    kwlnDrawControlFlags &= 0xF3FFFFFF;
    kwlnBackgroundFadeColor[0] = 0;
    kwlnBackgroundFadeColor[1] = 0;
    kwlnBackgroundFadeColor[2] = 0;
    kwlnBackgroundFadeColor[3] = 0;
    D_00435CFE = 0x31;
    D_00435D00 = 0x4F;
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
        D_00435CFE = 0x31;
        D_00435D00 = 0x4F;
        kwlnFadeResetBackground();
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = duration;
        kwlnDrawControlFlags = (kwlnDrawControlFlags | 0x04000000) & 0xF7FFFFFF;
    }
    D_0037F5EC[2] = 2048.0f;
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
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        kwlnBackgroundFadeDuration = duration;
        kwlnBackgroundFadeCounter = 0;
        kwlnDrawControlFlags = (kwlnDrawControlFlags & 0xFBFFFFFF) | 0x08000000;
    }
    D_0037F5EC[2] = 2041.0f;
}

s32 kwlnFadeIsBackgroundOverlayActive(void) {
    if (kwlnDrawControlFlags & 0x0C000000) {
        return 1;
    }
    if (kwlnBackgroundFadeMode == 0) {
        if (0x80 - kwlnBackgroundFadeColor[3] >= 0x80) {
            return 0;
        }
    } else if (D_00435CFE == 0x31) {
        return 0;
    }
    return 1;
}

void kwlnFadeSetMode(s32 mode) {
    kwlnBackgroundFadeMode = mode;
    if (kwlnBackgroundFadeMode == 0) {
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        kwlnBackgroundFadeColor[3] = 0x80;
    }
}

/* Step the background fade counter in the direction chosen by the control flags and derive the fade alpha and the two ramp values. */
void kwlnStepBackgroundFade(void) {
    f32 ratio;

    if (kwlnDrawControlFlags & 0x0C000000) {
        if (kwlnDrawControlFlags & 0x04000000) {
            kwlnBackgroundFadeCounter--;
        } else {
            kwlnBackgroundFadeCounter++;
        }
        ratio = (f32)kwlnBackgroundFadeCounter / (f32)kwlnBackgroundFadeDuration;
        kwlnBackgroundFadeColor[3] = (kwlnBackgroundFadeCounter << 7) / kwlnBackgroundFadeDuration;
        D_00435CFE = 49.0f - ratio * 49.0f;
        D_00435D00 = 79.0f - ratio * 79.0f;
        if (((kwlnDrawControlFlags & 0x04000000) && kwlnBackgroundFadeCounter == 0) ||
            ((kwlnDrawControlFlags & 0x08000000) && kwlnBackgroundFadeCounter == kwlnBackgroundFadeDuration)) {
            kwlnBackgroundFadeCounter = 0;
            kwlnBackgroundFadeDuration = 0;
            kwlnDrawControlFlags &= 0xF3FFFFFF;
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

