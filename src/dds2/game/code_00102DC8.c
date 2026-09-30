#include "common.h"

typedef struct KwlnResourceNode {
    s32 unk0;
    struct KwlnResourceNode *next;
    s32 *ready;
} KwlnResourceNode;

extern s8 D_00435C60;

extern u32 D_00435CAC;

extern u32 D_00435CC0;

extern u32 D_00435CC4;

extern s32 D_00435CC8;

extern u16 D_00435CE8;

extern u32 D_00435CD4;

typedef struct {
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} KwlnFadeColor;

extern KwlnFadeColor D_00435CF0;

extern u16 D_00438DC0;

extern u16 D_00438DC2;

extern s16 D_00435CEA;

extern s16 D_00435CEC;

extern u16 D_00435CEE;

extern u32 kwlnTaskGetTimer(void);

extern s32 effMiscRandMod(s32, s32);

extern void func_00103F58(s32, u8, s32);

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void func_00104F30(void *, s32, s32);

extern u32 kwlnTextureGetPageIndex(void);

extern s32 kwlnTextureCountIncompleteResources(void);

extern s32 func_0035C860();

extern s32 func_0033D810();

extern void sdfAppendPacket();

extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern void func_00104180(void);

extern void func_001044E8(void);

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);


extern s32 D_00435BA4;

extern s32 D_00435D04;

extern s32 D_00435D08;

typedef struct {
    u8 unknown[0x10];
    void (*submit)(void *, void *);
} KwlnGraphicsSink;

extern KwlnGraphicsSink D_00380708;

extern void *D_00438DA8;

extern f32 D_0037F5EC[];

extern s32 D_004389F8;

extern s32 D_00435CA8;

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

extern KwlnViewerPadState D_0040B7D8;

extern s32 func_00104908(s32);

extern void func_00104AA8(void *data, s32 handle);

extern u8 D_00380748[];

extern s8 D_0037F53B[];

extern void kwlnTextureAttachTask(u8 *scene);

extern s16 D_00435CFE;

extern s16 D_00435D00;

extern u8 D_00435CF8[4];

extern s8 D_00435CFC;

extern u8 D_00438D99;

extern u8 D_00438D90[2];

extern void *func_00101740(const char *);
extern void *func_00328D68(s32);
extern void func_00102BC8(void);
extern void func_00102D48(void);
extern char D_00435C18[];
extern u64 sdfCreateResetPacketList(void);
extern u32 func_00100400(void);
extern u8 D_0043DDA0[];
extern u16 D_00438DC4;
extern u16 D_00438DC6;

extern u8 D_0037F550[];

extern void func_003412D8(void *data, u32 tag);

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

    if (func_00101740(D_00435C18) == NULL) {
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
        kwlnTaskCreate(D_00435C18, 2, 0, 1, func_00102BC8, func_00102D48, work);
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00102E88);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103050);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103108);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001032F0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103388);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103430);

void func_001034E0(void) {
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001034E8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001036B0);

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
    u8 target = D_00438D99;

    if (D_00438D90[1] < target) {
        if (target - D_00438D90[1] > 0x20) {
            D_00438D90[1] = D_00438D90[1] + 0x20;
        } else {
            D_00438D90[1] = target;
        }
    } else if (target < D_00438D90[1]) {
        if (D_00438D90[1] - target > 0x20) {
            D_00438D90[1] = D_00438D90[1] - 0x20;
        } else {
            D_00438D90[1] = target;
        }
    } else {
        D_00438D90[1] = target;
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103F58);

extern u32 D_00438DA0[2];
extern void sdfDevConsSetEntryPair(s32, s32, s32);

void func_00104020(void) {
    D_00438D90[0] = 0;
    D_00438D90[1] = 0;
    D_00438DA0[0] = 0;
    D_00438DA0[1] = 0;
    sdfDevConsSetEntryPair(0, 0, 0);
}


INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104058);

u32 func_00104150(void) {
    return 0;
}

void kwlnInitMagicState(void) {
    func_003412D8(D_0037F550, 0x12345678);
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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104568);

void kwlnTextureDrawPageCounter(void *task) {
    char buffer[0x70];
    s32 current = kwlnTextureGetPageIndex();
    s32 count = kwlnTextureCountIncompleteResources();
    func_0035C860(buffer, "TEX VIEWER [%d/%d]", current, count - 1);
    sdfAppendPacket(task, func_0033D810(0x7180, 0x79C0, 0xFFFFF80, 0, buffer));
}

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104700);

s32 kwlnTextureCountIncompleteResources(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)D_004389F8;
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
    return D_00435CAC;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104908);

s32 kwlnTextureViewerHandlePad(void) {
    if (D_0040B7D8.unk13 < 0) {
        return 0;
    }
    if (D_0040B7D8.unk14 & 2) {
        D_00435CAC -= 1;
    } else if (D_0040B7D8.unk15 & 2) {
        D_00435CAC += 1;
    } else if ((D_0040B7D8.unk18 & 2) || (D_0040B7D8.unk19 & 2)) {
        D_00435CAC -= 10;
    } else if ((D_0040B7D8.unk1A & 2) || (D_0040B7D8.unk1B & 2)) {
        D_00435CAC += 10;
    }
    D_00435CAC = func_00104908(D_00435CAC);
    if (D_0040B7D8.unk12 < 0) {
        D_00435C60 ^= 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104AA8);

s32 kwlnSwapActiveResource(s32 resource) {
    s32 status;

    if (D_004389F8 == 0) {
        return 0;
    }
    status = kwlnTextureViewerHandlePad();
    func_00104AA8((void *)resource, D_00435CA8);
    return status;
}

s32 kwlnLoadDefaultResource(void) {
    if (D_004389F8 == 0) {
        return 0;
    }
    if (kwlnTextureViewerHandlePad() == 0) {
        return -1;
    }
    func_00104AA8(D_00380748, D_00435CA8);
    return 0;
}

s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)D_004389F8;
    if (node == NULL) {
        return NULL;
    }
    D_00435CA8 = (s32)node;
    D_00435C60 = 1;
    D_00435CAC = 0;
    while (node->ready != NULL && *node->ready != 0) {
        node = ((KwlnResourceNode *)D_00435CA8)->next;
        if (node == NULL) {
            return NULL;
        }
        D_00435CA8 = (s32)node;
    }
    return kwlnLoadDefaultResource;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104D40);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104F30);

void kwlnTextureAttachTask(u8 *scene) {
    void *task = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(task);
    func_00104F30(task, 0x7180, 0x79C0);
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

s32 kwlnEnsureDefaultResource(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(D_00380748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105070);

extern void sdfGraphSetDisplayMode(s32);
extern void sdfCameraBuildProjection(void *);
extern void func_00105070(s32);
extern void func_001057B0(void);
extern u8 D_0037F5E0[];
extern u8 D_0037F980[];
extern u8 D_00435BC8;
extern void evtEnsureDrawVectorState(void);

void func_00105240(void) {
    sdfGraphSetDisplayMode(1);
    sdfCameraBuildProjection(D_0037F5E0);
    sdfCameraBuildProjection(D_0037F980);
    func_00105070(0);
    func_00105070(1);
    func_001057B0();
    D_00435BC8 = 0;
    evtEnsureDrawVectorState();
}


INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105290);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001053F0);

/* Release the retained texture and clear its request/pending flags. */
void kwlnTextureReleaseHeldReference(void) {
    if (D_00435CC8 != 0) {
        sdfTexReleaseReference(D_00435CC8);
    }
    D_00435CC8 = 0;
    D_00435CC4 = 0;
    D_00435CC0 = 0;
}

s32 kwlnTextureSetReferenceFlagIfPresent(void) {
    s32 result = 0;

    if (D_00435CC8 == 0) {
        return result;
    }
    D_00435CC0 = 1;
    return 1;
}

void func_00105538(void) {
    D_00435CC0 = 0;
}

u32 func_00105540(void) {
    return D_00435CC0;
}

s32 func_00105548(void) {
    return D_00435CC8;
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

u64 func_00105910(s32 arg0) {
    u64 list = sdfCreateResetPacketList();

    sdfAppendPacket(list, D_0043DDA0 + arg0 * 0x160 + func_00100400() * 0xB0);
    return list;
}

/* The low two bits select the active fade direction; clearing cancels it. */
void kwlnFadeClear(void) {
    D_00435CD4 &= ~3;
    D_00435CF0.r = 0;
    D_00435CF0.g = 0;
    D_00435CF0.b = 0;
    D_00435CF0.a = 0;
}

void kwlnFadeSetColor(s8 red, s8 green, s8 blue, s8 alpha) {
    D_00435CD4 &= ~3;
    D_00435CF0.r = red;
    D_00435CF0.g = green;
    D_00435CF0.b = blue;
    D_00435CF0.a = alpha;
}

void kwlnFadeGetColor(KwlnFadeColor **color) {
    *color = &D_00435CF0;
}

void kwlnFadeSetRGB(s8 red, s8 green, s8 blue) {
    D_00435CF0.r = red;
    D_00435CF0.g = green;
    D_00435CF0.b = blue;
}

void kwlnFadeOutStart(s8 red, s8 green, s8 blue, s32 duration) {
    D_00435CF0.r = red;
    D_00435CF0.g = green;
    D_00435CF0.b = blue;
    D_00435CF0.a = -0x80;
    if (duration == 0) {
        D_00435CF0.a = 0;
        D_00438DC0 = 0;
        D_00438DC2 = 0;
        kwlnFadeClear();
        return;
    }
    D_00438DC2 = duration;
    D_00438DC0 = duration;
    D_00435CD4 = (D_00435CD4 | 1) & ~2;
}

/* Enter the first fade direction; a zero duration finishes immediately. */
void kwlnFadeStartIn(s32 frames) {
    D_00435CF0.a = -0x80;
    if (frames == 0) {
        D_00435CF0.a = 0;
        D_00438DC0 = 0;
        D_00435CD4 &= ~3;
        D_00438DC2 = 0;
        return;
    }
    D_00438DC2 = frames;
    D_00438DC0 = frames;
    D_00435CD4 = (D_00435CD4 | 1) & ~2;
}

/* Select the second fade direction using the specified RGB color. */
void kwlnFadeInStart(s8 red, s8 green, s8 blue, s32 frames) {
    D_00435CF0.r = red;
    D_00435CF0.g = green;
    D_00435CF0.b = blue;
    D_00435CF0.a = 0;
    if (frames == 0) {
        D_00438DC0 = 0;
        D_00438DC2 = 0;
        D_00435CF0.a = -0x80;
        D_00435CD4 &= ~3;
        return;
    }
    D_00438DC2 = frames;
    D_00438DC0 = 0;
    D_00435CD4 = (D_00435CD4 & ~1) | 2;
}

/* Enter the second fade direction; a zero duration finishes immediately. */
void kwlnFadeStartOut(s32 frames) {
    D_00435CF0.a = 0;
    if (frames == 0) {
        D_00438DC0 = 0;
        D_00435CF0.a = -0x80;
        D_00435CD4 &= ~3;
        D_00438DC2 = 0;
        return;
    }
    D_00438DC2 = frames;
    D_00438DC0 = 0;
    D_00435CD4 = (D_00435CD4 & ~1) | 2;
}

u8 kwlnFadeIsActive(void) {
    return (D_00435CD4 & 3) != 0;
}

void kwlnFadeUpdate(void) {
    if (kwlnFadeIsActive() != 0) {
        if (D_00435CD4 & 1) {
            D_00438DC0 -= 1;
        } else {
            D_00438DC0 += 1;
        }
        D_00435CF0.a = (D_00438DC0 << 7) / D_00438DC2;
        if (((D_00435CD4 & 1) && D_00438DC0 == 0) || ((D_00435CD4 & 2) && D_00438DC0 == D_00438DC2)) {
            D_00438DC0 = 0;
            D_00438DC2 = 0;
            D_00435CD4 &= ~3;
        }
    }
}

void kwlnDrawBlurErrorCounters(void) {
    void *task;

    if (D_00435BA4 != 0) {
        if (D_00435D04 != 0 || D_00435D08 != 0) {
            task = sdfAllocPacketAligned(0x20);
            sdfInitPacketList(task);
            if (D_00435D04 > 0) {
                sdfAppendPacket(task, func_0033D810(0x73C0, 0x7AE0, 0xFEFFFF, 0xE, "DISTBLUR_NUMERR:%d", D_00435D04));
            }
            if (D_00435D08 > 0) {
                sdfAppendPacket(task, func_0033D810(0x73C0, 0x7B40, 0xFEFFFF, 4, "RIPBLUR_NUMERR :%d", D_00435D08));
            }
            D_00380708.submit(&D_00380708, task);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105CF8);

/* Reset the background fade's color bytes and its two parameter words. */
void kwlnFadeResetBackground(void) {
    D_00435CD4 &= 0xF3FFFFFF;
    D_00435CF8[0] = 0;
    D_00435CF8[1] = 0;
    D_00435CF8[2] = 0;
    D_00435CF8[3] = 0;
    D_00435CFE = 0x31;
    D_00435D00 = 0x4F;
}

void kwlnFadeBackgroundStartOut(s32 duration) {
    D_00435CF8[0] = 0;
    D_00435CF8[1] = 0;
    D_00435CF8[2] = 0;
    D_00435CF8[3] = 0x80;
    if (duration == 0) {
        D_00435CF8[3] = 0;
        D_00438DC4 = 0;
        D_00438DC6 = 0;
        D_00435CFE = 0x31;
        D_00435D00 = 0x4F;
        kwlnFadeResetBackground();
    } else {
        D_00438DC6 = duration;
        D_00438DC4 = duration;
        D_00435CD4 = (D_00435CD4 | 0x04000000) & 0xF7FFFFFF;
    }
    D_0037F5EC[2] = 2048.0f;
}

void kwlnFadeBackgroundStartIn(s32 duration) {
    D_00435CF8[0] = 0;
    D_00435CF8[1] = 0;
    D_00435CF8[2] = 0;
    D_00435CF8[3] = 0;
    if (duration == 0) {
        D_00438DC4 = 0;
        D_00438DC6 = 0;
        D_00435CF8[3] = 0x80;
        D_00435CD4 &= 0xF3FFFFFF;
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        D_00438DC6 = duration;
        D_00438DC4 = 0;
        D_00435CD4 = (D_00435CD4 & 0xFBFFFFFF) | 0x08000000;
    }
    D_0037F5EC[2] = 2041.0f;
}

s32 func_00106108(void) {
    if (D_00435CD4 & 0x0C000000) {
        return 1;
    }
    if (D_00435CFC == 0) {
        if (0x80 - D_00435CF8[3] >= 0x80) {
            return 0;
        }
    } else if (D_00435CFE == 0x31) {
        return 0;
    }
    return 1;
}

void kwlnFadeSetMode(s32 mode) {
    D_00435CFC = mode;
    if (D_00435CFC == 0) {
        D_00435CFE = 0;
        D_00435D00 = 0;
    } else {
        D_00435CF8[3] = 0x80;
    }
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106188);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106288);

void func_001063A8(f32 arg0) {
    D_0037F5EC[0] = arg0;
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

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CA8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CAC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CB0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CB8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CBC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CC8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CCE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CD0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CD4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CD8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CDA);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CDC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE4);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CE8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEA);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CEE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF0);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF1);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF2);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF3);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CF8);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFB);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFC);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435CFE);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D00);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D04);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D08);

INCLUDE_SDATA(const s32, "game/code_00102DC8", D_00435D10);

