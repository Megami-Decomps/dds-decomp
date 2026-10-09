#include "common.h"
#include "sdf_task_work.h"
#include "mnu_scene_work.h"
#include "dsp_name.h"

extern void func_0024E260(s32, s32, s32, s32, s32, s32);
extern TaskWork *mnuSceneResourceContext;
extern u32 *mnuGetSelectedNodeValue(void);
extern s32 func_00255E08();
extern void func_0024E5A0(s32, s32, s32, s32, s32, s32, f32, f32);
extern void func_0025D2F8(s32, s32, s32, s32, MenuSceneWork *, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
extern f32 sdfSinPoly(f32 angle);
extern void func_00254758(s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_00253E58", func_00253E58);

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void mnuDrawPanelWithPackedColorPattern(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0) {
    u32 arr[4];
    s32 v;

    v = (s32)(((f32)(a3 << 4)) * 0.0078125f);
    v |= 0x0A050700;
    arr[0] = v;
    arr[1] = v;
    arr[2] = v;
    arr[3] = v;
    a1 += 0x32;
    a1 *= 8;
    uiDrawGradientColorRect(a0 * 16, a1, a2, 0x2000, 0xC70, arr, t0);
}

/* Fill-level dispatch: direct, inverted, or sin-pulsed, then the shared layers. */
s32 mnuDrawMantraCostTransition(void) {
    s32 nodePrev;
    s32 nodeNext;
    MenuSceneWork *scene;
    s32 level;
    s32 scaled;
    s32 raw;
    f32 frac;
    u32 *cost;

    frac = 0.0f;
    mnuGetSelectedNodeValue();
    nodePrev = sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    nodeNext = sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    scene = (MenuSceneWork *)(u32)nodeNext;
    func_00255E08(nodePrev, 0x80, 0x52);
    level = *(s32 *)(nodePrev + 0x1C);
    switch (level) {
    case 0:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        scaled = (s32)(frac * 128.0f);
        func_0024E5A0(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        func_0025D2F8(0, 0, 1, scaled, scene, 0x53);
        func_00254758(0, 0, 1, 0x80, 0x53);
        func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
        func_0024E260(0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, scaled, 0x53);
        func_0024E260(0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 2:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        frac = 1.0f - frac;
        scaled = (s32)(frac * 128.0f);
        func_0024E5A0(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        func_0025D2F8(0, 0, 1, scaled, scene, 0x53);
        func_00254758(0, 0, 0, 0x80, 0x53);
        func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
        func_0024E260(0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, scaled, 0x53);
        func_0024E260(0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 1:
        /* frac keeps its 0.0f initializer when raw == 0. */
        raw = scene->transitionBlendCounter;
        if (raw > 0) {
            frac = (f32)raw / 10.0f;
        } else if (raw < 0) {
            frac = (f32)(-raw) / 10.0f;
        }
        frac = 1.0f - sdfSinPoly(frac * 3.14159265f);
        func_0024E5A0(0, 0, 0, 0x80, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, 0x80, 0x53);
        func_0025D2F8(0, 0, 1, (s32)(frac * 128.0f), scene, 0x53);
        func_00254758(0, 0, 1, 0x80, 0x53);
        func_0024E260(0, 0, 0, 0x80, 0xF, 0x53);
        func_0024E260(0, 0, 0, 0x80, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, 0x80, 0x53);
        func_0024E260(0x20, 0, 0, 0x80, 0xB, 0x53);
        break;
    default:
        break;
    }
    mnuChooseDisplaySpriteKindFromEntryFlags(scene, 0x80, 0x53);
    func_0024E260(0, 0, 0, 0x80, 0xD, 0x53);
    return 0;
}

void mnuDrawDisplaySpriteAndPanelMarks(s32 p0, s32 a1, s32 a2) {
    func_00255E08();
    func_0024E260(0, 0, 0, a1, 0x5A, a2);
    itfDspDrawMarksA(a1, a2);
}

/* Three sprite layers drawn at the origin for one draw context. */
void func_002546D8(s32 scale, s32 context) {
    func_0024E260(0, 0, 0, scale, 0x4F, context);
    func_0024E260(0, 0, 0, scale, 0xC, context);
    func_0024E260(0, 0, 0, scale, 0xD, context);
}

void func_00254758(s32 x, s32 y, s32 z, s32 drawContext, s32 drawArgument) {
    func_0024E260(x, y, z, drawContext, 14, drawArgument);
}

void func_00254778(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    func_0024E260(a0, a1, a2, a3, 0x47, a4);
    func_0024E260(a0, a1, a2, a3, 0x48, a4);
    func_0024E260(a0, a1, a2, a3, 0x49, a4);
}

typedef struct {
    s8 spriteIndices[7];
} DspUnitSpriteTable;

typedef struct {
    u8 pad00[0x38];
    s32 shade; /* 0x38 */
} DspEffectShade;

typedef struct {
    u8 pad00[0x828];
    DspEffectShade *shade; /* 0x828 */
} DspEffectRoot;

typedef struct {
    u8 pad00[4];
    u16 unitId; /* 0x4 */
} DspTablePartyRecord;

typedef struct {
    DspTablePartyRecord *partyRecord; /* 0x0 */
    s32 profileId;                    /* 0x4: index into the 19-byte mantra-name rows */
} DspTableProgress;

typedef struct {
    u8 pad00[0x1C];
    struct { u8 pad00[0x70]; DspTableProgress *progress; } *selectedNode; /* 0x1C */
    u8 pad20[0x10];
    s32 *selectedValue; /* 0x30 */
} DspTableUnit;

typedef struct {
    u8 pad00[0xC];
    DspTableUnit *unit; /* 0xC */
} DspTableWindowContext;

typedef struct {
    u8 pad00[0x24];
    DspEffectRoot *effectRoot; /* 0x24 */
} DspTableScene;

extern DspUnitSpriteTable D_003BC440[];
extern DspMantraName *D_003BAA78;
extern void *memset(void *, s32, u32);
extern u32 strlen(const char *);
extern void effUpdateAttached(s32, s32, s32, DspEffectRoot *, s32);
extern s32 frFontQueueTextAndOptionallyMeasure(s32 x, s32 y, u32 first, u32 second, s8 type, void *name, s32 flag, s32 option);

/* Draw the selected mantra's display: sprite, fade-driven effect and its name (shifted left for long names). */
void func_00254810(s32 x, s32 y, s32 z, s32 alpha, DspTableWindowContext *windowContext, s32 drawContext) {
    DspUnitSpriteTable sprites = D_003BC440[0];
    DspTableUnit *unit = windowContext->unit;
    s32 *selectedValue = unit->selectedValue;
    DspTableProgress *progress = unit->selectedNode->progress;
    DspTableScene *scene = (DspTableScene *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u8 scratch[0x20];

    selectedValue[1] = alpha;
    func_0024E260(x, y, z, alpha, sprites.spriteIndices[progress->partyRecord->unitId] + 0x4A, drawContext);
    scene->effectRoot->shade->shade = (s32)((f32)(alpha << 8) * 0.0078125f);
    effUpdateAttached(0xD80, 0xC30, 1, scene->effectRoot, 0x53);
    memset(scratch, 0, sizeof(scratch));
    if (strlen((const char *)(D_003BAA78 + progress->profileId)) >= 5) {
        frFontQueueTextAndOptionallyMeasure(x + 0xEF, y + 0x15E, z, alpha | 0xA09DC300, 0, D_003BAA78 + progress->profileId, 0x80000000, drawContext);
    } else {
        frFontQueueTextAndOptionallyMeasure(x + 0xF8, y + 0x15E, z, alpha | 0xA09DC300, 0, D_003BAA78 + progress->profileId, 0x80000000, drawContext);
    }
}

INCLUDE_SDATA(const s32, "game/code_00253E58", D_003BC440);

