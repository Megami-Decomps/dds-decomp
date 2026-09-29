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

extern u32 D_003BA904;

extern KwlnFadeColor D_003BA920;

extern u16 D_003BA918;
extern s16 D_003BA91A;
extern s16 D_003BA91C;
extern u16 D_003BA91E;

extern u32 D_003BA8F0;

extern u32 D_003BA8F4;

extern s32 D_003BA8F8;

extern u32 D_003BA8DC;
extern s8 D_003BA890;

extern s8 D_003BA7FC;

extern s8 D_003BA92B;

extern s8 D_003BA92C;

extern s16 D_003BA92E;

extern s16 D_003BA930;
extern s8 D_003BA928[4];

extern s8 D_0032453B[];

extern f32 D_003245EC[];

extern f32 D_003245E0[];

extern f32 D_00324980[];

extern u8 D_00325748[];

extern u8 D_003BD690[2];

extern s32 D_003BD6A0[2];

extern u8 D_003BD699;
extern u16 D_003BD6C0;

extern u16 D_003BD6C2;

extern s32 D_003BD308;
extern s32 D_003BA8D8;
extern u8 D_00324550[];
extern void func_002E8430(void *data, u32 tag);
extern s32 func_00104A98(void);
extern void func_00104B88(void *data, s32 handle);

extern void kwlnTextureAttachTask(u8 *scene);

extern void func_00105150(s32 arg0);

extern void func_00105890(void);

extern void evtUnk8A48Ensure(void);

extern void sdfGraphSetDisplayMode(s32 arg0);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void func_00105010(void *, s32, s32);
extern void sdfAppendPacket(void *, s32);
extern u32 kwlnTaskGetTimer(void);
extern s32 effMiscRandMod(s32, s32);
extern void func_00104068(s32, u8, s32);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);
extern void func_00104290(void);
extern void *D_003BD6A8;
extern s32 func_001049A8(void);
extern s32 func_003014F0();
extern s32 func_002E4960(s32, s32, s32, s32, const char *);

extern void func_002E1718(void *arg0);

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
extern void func_00102E58(void);
extern void *func_002CFEB8(s32);

void func_00102ED8(void) {
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
        kwlnTaskCreate(D_003BA848, 2, 0, 1, func_00102CD8, func_00102E58, work);
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

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001038A0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103908);

s32 kwlnDebugPulseColors(void) {
    if (kwlnTaskGetTimer() % 0x4650 == 0) {
        func_00104068(0, 1, 0xF);
        func_00104068(1, (u8)(effMiscRandMod(0, 150) + 100), 30);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001039E0);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00103B10);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104008);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104068);

void func_00104130(void) {
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
    func_002E8430(D_00324550, 0x12345678);
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

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104678);

void kwlnTextureDrawPageCounter(void *task) {
    char buffer[0x70];
    s32 current = kwlnTextureGetPageIndex();
    s32 count = func_001049A8();
    func_003014F0(buffer, "TEX VIEWER [%d/%d]", current, count - 1);
    sdfAppendPacket(task, func_002E4960(0x7180, 0x79C0, 0xFFFFF80, 0, buffer));
}

INCLUDE_RODATA(const s32, "game/code_00102ED8", D_0039E078);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104810);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001049A8);

u32 kwlnTextureGetPageIndex(void) {
    return D_003BA8DC;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104A18);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104A98);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00104B88);

s32 kwlnSwapActiveResource(s32 resource) {
    s32 status;

    if (D_003BD308 == 0) {
        return 0;
    }
    status = func_00104A98();
    func_00104B88((void *)resource, D_003BA8D8);
    return status;
}

s32 kwlnLoadDefaultResource(void) {
    if (D_003BD308 == 0) {
        return 0;
    }
    if (func_00104A98() == 0) {
        return -1;
    }
    func_00104B88(D_00325748, D_003BA8D8);
    return 0;
}

s32 (*kwlnTextureFindIncompleteResource(void))(void) {
    KwlnResourceNode *node = (KwlnResourceNode *)D_003BD308;
    if (node == NULL) {
        return NULL;
    }
    D_003BA8D8 = (s32)node;
    D_003BA890 = 1;
    D_003BA8DC = 0;
    while (node->ready != NULL && *node->ready != 0) {
        node = ((KwlnResourceNode *)D_003BA8D8)->next;
        if (node == NULL) {
            return NULL;
        }
        D_003BA8D8 = (s32)node;
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
    kwlnTextureAttachTask(D_00325748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105150);

void func_00105320(void) {
    sdfGraphSetDisplayMode(1);
    func_002E1718(&D_003245E0);
    func_002E1718(&D_00324980);
    func_00105150(0);
    func_00105150(1);
    func_00105890();
    D_003BA7FC = 0;
    evtUnk8A48Ensure();
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105370);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001054D0);

/* Release the retained texture and clear its request/pending flags. */
void func_001055C0(void) {
    if (D_003BA8F8 != 0) {
        sdfTexReleaseReference(D_003BA8F8);
    }
    D_003BA8F8 = 0;
    D_003BA8F4 = 0;
    D_003BA8F0 = 0;
}

s32 func_001055F8(void) {
    s32 result = 0;

    if (D_003BA8F8 == 0) {
        return result;
    }
    D_003BA8F0 = 1;
    return 1;
}

void func_00105618(void) {
    D_003BA8F0 = 0;
}

u32 func_00105620(void) {
    return D_003BA8F0;
}

s32 func_00105628(void) {
    return D_003BA8F8;
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

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001059F0);

/* The low two bits select the active fade direction; clearing cancels it. */
void kwlnFadeClear(void) {
    D_003BA904 &= ~3;
    D_003BA920.r = 0;
    D_003BA920.g = 0;
    D_003BA920.b = 0;
    D_003BA920.a = 0;
}

void kwlnFadeSetColor(s8 red, s8 green, s8 blue, s8 alpha) {
    D_003BA904 &= ~3;
    D_003BA920.r = red;
    D_003BA920.g = green;
    D_003BA920.b = blue;
    D_003BA920.a = alpha;
}

void kwlnFadeGetColor(KwlnFadeColor **color) {
    *color = &D_003BA920;
}

void kwlnFadeSetRGB(s8 red, s8 green, s8 blue) {
    D_003BA920.r = red;
    D_003BA920.g = green;
    D_003BA920.b = blue;
}

void kwlnFadeOutStart(s8 red, s8 green, s8 blue, s32 duration) {
    D_003BA920.r = red;
    D_003BA920.g = green;
    D_003BA920.b = blue;
    D_003BA920.a = -0x80;
    if (duration == 0) {
        D_003BA920.a = 0;
        D_003BD6C0 = 0;
        D_003BD6C2 = 0;
        kwlnFadeClear();
        return;
    }
    D_003BD6C2 = duration;
    D_003BD6C0 = duration;
    D_003BA904 = (D_003BA904 | 1) & ~2;
}

/* Enter the first fade direction; a zero duration finishes immediately. */
void kwlnFadeStartIn(s32 duration) {
    D_003BA920.a = -0x80;
    if (duration == 0) {
        D_003BA920.a = 0;
        D_003BA904 &= ~3;
        D_003BD6C0 = 0;
        D_003BD6C2 = 0;
    } else {
        D_003BD6C2 = duration;
        D_003BD6C0 = duration;
        D_003BA904 = (D_003BA904 | 1) & ~2;
    }
}

/* Select the second fade direction using the specified RGB color. */
void kwlnFadeInStart(s8 red, s8 green, s8 blue, s32 duration) {
    D_003BA920.r = red;
    D_003BA920.g = green;
    D_003BA920.b = blue;
    D_003BA920.a = 0;
    if (duration == 0) {
        D_003BD6C0 = 0;
        D_003BD6C2 = 0;
        D_003BA920.a = -0x80;
        D_003BA904 &= ~3;
        return;
    }
    D_003BD6C2 = duration;
    D_003BD6C0 = 0;
    D_003BA904 = (D_003BA904 & ~1) | 2;
}

/* Enter the second fade direction; a zero duration finishes immediately. */
void kwlnFadeStartOut(s32 duration) {
    D_003BA920.a = 0;
    if (duration == 0) {
        D_003BD6C0 = 0;
        D_003BA920.a = -0x80;
        D_003BA904 &= ~3;
        D_003BD6C2 = 0;
    } else {
        D_003BD6C2 = duration;
        D_003BD6C0 = 0;
        D_003BA904 = (D_003BA904 & ~1) | 2;
    }
}

u8 kwlnFadeIsActive(void) {
    return (D_003BA904 & 3) != 0;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105C58);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105D00);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00105DD8);

/* Reset the background fade's color bytes and its two parameter words. */
void kwlnFadeResetBackground(void) {
    D_003BA904 &= 0xF3FFFFFF;
    D_003BA928[0] = 0;
    D_003BA928[1] = 0;
    D_003BA928[2] = 0;
    D_003BA928[3] = 0;
    D_003BA92E = 0x31;
    D_003BA930 = 0x4F;
}

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001060C8);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106160);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_001061E8);
INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106240);
INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106268);

INCLUDE_ASM(const s32, "game/code_00102ED8", func_00106368);

void func_00106488(f32 arg0) {
    D_003245EC[0] = arg0;
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

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8D8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8DC);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8E0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8E8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8EC);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8F0);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8F4);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8F8);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8FC);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA8FE);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA900);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA904);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA908);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA90A);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA90C);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA910);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA914);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA918);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91A);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91C);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA91E);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA920);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA921);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA922);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA923);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA928);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92B);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92C);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA92E);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA930);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA934);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA938);

INCLUDE_SDATA(const s32, "game/code_00102ED8", D_003BA940);

