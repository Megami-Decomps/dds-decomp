#include "mnu.h"

extern void sdfReleaseChipBlock(void *);
/* Retail retains a jal and epilogue; default TU -O2 changes the shape. */

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(s32);

extern f32 effMiscRandUnitFloat(s32);

extern void *memset(void *, s32, u32);

extern f32 sdfSinPoly(f32);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

typedef union MnuVariantSpritePlacement {
    struct {
        u8 pad00[4];
        s16 x;
        s16 y;
        s16 spriteGroups;
        u16 flags;
    } fields;
    s16 values[6];
} MnuVariantSpritePlacement;

typedef struct DspScene {
    s32 frame;
    u8 pad04[8];
    u16 sceneId;
    u8 pad0E[6];
    s32 state;
} DspScene;

typedef struct MantraPulseEntry {
    s32 unk00;
    DspScene *scene;
} MantraPulseEntry;

typedef struct MantraPulseGrid MantraPulseGrid;
typedef struct MnuProfileOwner MnuProfileOwner;

extern MnuVariantSpritePlacement D_0036B7F0[];
extern s32 mnuSceneResourceContext;
extern void *func_002CB3B8(s32, s32);
extern u32 mnuGetMantraDisplayFlags(DspScene *, MnuProfileOwner *);
extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);
extern void func_0024EC08(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void func_00258A70(s32, s32, s32, s32, u32, f32, f32, s32);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259498);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A0);

void func_00259890(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuProfileOwner *profileOwner, MantraPulseGrid *grid,
                   f32 scaleX, f32 scaleY, MantraPulseEntry *entry,
                   s32 context) {
    u8 mappedIds[6] = { 5, 15, 45, 52, 63, 68 };
    DspScene *scene;
    s32 drawX;
    s32 drawY;
    s32 i;
    f32 pulse;

    {
        u32 flags;
        s32 display;

        display = (s32)func_002CB3B8(mnuSceneResourceContext, 1);
        scene = entry->scene;
        pulse = 0.0f;
        scene->frame++;
        if ((f32)scene->frame > 60.0f) {
            scene->frame = 0;
        }
        pulse = sdfSinPoly(pulse);
        drawX = x + D_0036B7F0[scene->sceneId].values[2];
        drawY = y + D_0036B7F0[scene->sceneId].values[3];
        flags = mnuGetMantraDisplayFlags(scene, profileOwner);

        if (flags & 1) {
            uiDrawUniformColorRect(drawX << 4, drawY << 3, 0, 0x300, 0x180,
                                   (s32)((f32)((alpha * 5) << 4) * 0.0078125f) |
                                       0x60501000,
                                   context);
        }
        {
            s32 displayFlags = ((u8 *)display)[0x5AC];

            if (displayFlags & 1) {
                func_0024EC08(x, y, depth, alpha, scene->sceneId, 0x20,
                              scaleX, scaleY, context);
            } else {
                for (i = 0; i < 6; i++) {
                    if (scene->sceneId == mappedIds[i]) {
                        func_0024EC08(x, y, depth, alpha, i + 0x59, 0x20,
                                      scaleX, scaleY, context);
                        break;
                    }
                }
                if (i == 6) {
                    func_0024EC08(x, y, depth, alpha, scene->sceneId, 0x20,
                                  scaleX, scaleY, context);
                }
            }
        }
        func_00258A70(drawX, drawY, depth,
                      (s32)((f32)alpha * (pulse * 0.4f + 0.2f)),
                      flags, scaleX, scaleY, context);
    }
}

INCLUDE_ASM(const s32, "game/code_00259498", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025A680);

void func_0025AA20(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, -10, 0, (s32)(scale * t), 0, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 0, param);
}

void func_0025AB38(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(10, 0, 0, (s32)(scale * t), 1, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 1, param);
}

void func_0025AC50(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, 10, 0, (s32)(scale * t), 2, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 2, param);
}

void func_0025AD68(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(-10, 0, 0, (s32)(scale * t), 3, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 3, param);
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AE80);

typedef struct MnuSpritePlacement {
    s16 resourceIndex;
    s16 spriteIndex;
    s16 x;
    s16 y;
} MnuSpritePlacement;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;
} DatGameCounters;

extern MnuSpritePlacement D_0036B510[];
extern s32 D_0036C698[];
extern DatGameCounters *datGameState;
extern char D_003BC4C8[];
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern s32 itfDrawGlyphChainWithWidthQuery(s32, s32, s32, u32, u8, u32, s32, u32);

/* Interpolate currency changes over twenty draws, with the native ticking sound. */
void mnuDrawAnimatedCurrencyCounter(s32 x, s32 y, s32 depth, s32 alpha,
                  MenuSceneMetadata *scene, s32 context) {
    char currencyText[16];
    u32 color = alpha | 0xA09DC300;
    s32 resource;

    resource = D_0036C698[D_0036B510[19].resourceIndex];
    func_002BF4E0((x + D_0036B510[19].x) << 4,
                  (y + D_0036B510[19].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  resource,
                  D_0036B510[19].spriteIndex, context);
    resource = D_0036C698[D_0036B510[34].resourceIndex];
    func_002BF4E0((x + D_0036B510[34].x) << 4,
                  (y + D_0036B510[34].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  resource,
                  D_0036B510[34].spriteIndex, context);
    if (datGameState->currency != scene->displayedCurrency) {
        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        scene->currencyFrame++;
        func_003014F0(currencyText, D_003BC4C8,
                      scene->displayedCurrency +
                      (datGameState->currency - scene->displayedCurrency) *
                          scene->currencyFrame / 20);
        if (scene->currencyFrame == 20) {
            scene->displayedCurrency = datGameState->currency;
            scene->currencyFrame = 0;
        }
    } else {
        func_003014F0(currencyText, D_003BC4C8, datGameState->currency);
    }
    itfDrawGlyphChainWithWidthQuery(x + 0x191, y + 0x39, depth, color,
                                    0, (u32)currencyText, 0, context);
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B350);

void mnuReleaseOptionalDrawAllocation(void *unused, void *allocation) {
    if (allocation != 0) {
        sdfReleaseChipBlock(allocation);
    }
}


typedef struct {
    s32 allocation;    /* 0x00 */
    u8 pad04[0x2C];
    s32 lifetime;      /* 0x30 */
    s32 owner;         /* 0x34 */
    u8 pad38[0xC];
    u8 variant;       /* 0x44 */
    u8 sprite;        /* 0x45 */
    u8 pad46[2];
} MovieSpriteResource;

void *mnuCreateSpriteResource(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = sdfAllocGeneralBlock(0x48);
    MovieSpriteResource *resource = sdfMemoryGetBlockAddress(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->lifetime = (s32)(effMiscRandUnitFloat(0) * 30.0f + 10.0f);
    return resource;
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B888);

INCLUDE_RODATA(const s32, "game/code_00259498", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_00259498", mnuSceneResourceContext);

