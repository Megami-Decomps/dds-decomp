#include "common.h"

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

extern f32 D_0037F5EC[];

extern s32 D_004389F8;

extern s32 D_00435CA8;

extern s32 func_001049B8(void);

extern void func_00104AA8(void *data, s32 handle);

extern u8 D_00380748[];

extern s8 D_0037F53B[];

extern void kwlnTextureAttachTask(void *arg0);

extern s16 D_00435CFE;

extern s16 D_00435D00;

extern s8 D_00435CF8[4];

extern u8 D_0037F550[];

extern void func_003412D8(void *data, u32 tag);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00102DC8);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103790);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001037F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnDebugPulseColors);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001038D0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103A00);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103EF8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00103F58);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104020);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnDebugGraphSetEnabled);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104568);

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnTextureDrawPageCounter);

INCLUDE_RODATA(const s32, "game/code_00102DC8", D_004111F8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104700);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104898);

u32 kwlnTextureGetPageIndex(void) {
    return D_00435CAC;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104908);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001049B8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104AA8);

s32 kwlnSwapActiveResource(s32 resource) {
    s32 status;

    if (D_004389F8 == 0) {
        return 0;
    }
    status = func_001049B8();
    func_00104AA8((void *)resource, D_00435CA8);
    return status;
}

s32 kwlnLoadDefaultResource(void) {
    if (D_004389F8 == 0) {
        return 0;
    }
    if (func_001049B8() == 0) {
        return -1;
    }
    func_00104AA8(D_00380748, D_00435CA8);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnTextureFindIncompleteResource);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104D40);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00104F30);

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnTextureAttachTask);

s32 kwlnEnsureDefaultResource(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    kwlnTextureAttachTask(D_00380748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105070);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105240);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105290);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001053F0);

void func_001054E0(void) {
    if (D_00435CC8 != 0) {
        func_0032BB68(D_00435CC8);
    }
    D_00435CC8 = 0;
    D_00435CC4 = 0;
    D_00435CC0 = 0;
}

s32 func_00105518(void) {
    s32 ret = 0;

    if (D_00435CC8 == 0) {
        return ret;
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

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnFadeSetupFrames);

void func_001057A8(void) {
    D_00435CE8 = 0;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_001057B0);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105910);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", kwlnFadeOutStart);

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

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105B78);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105C20);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105CF8);

void kwlnFadeResetBackground(void) {
    D_00435CD4 &= 0xF3FFFFFF;
    D_00435CF8[0] = 0;
    D_00435CF8[1] = 0;
    D_00435CF8[2] = 0;
    D_00435CF8[3] = 0;
    D_00435CFE = 0x31;
    D_00435D00 = 0x4F;
}

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00105FE8);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106080);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106108);

INCLUDE_ASM(const s32, "game/code_00102DC8", func_00106160);

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
