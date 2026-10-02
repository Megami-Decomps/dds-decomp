#include "common.h"
#include "evt_unit.h"
#include "pcp_vu0.h"

extern u32 D_0038BBD8[];

extern u32 D_0038BB50[];

extern u32 D_0038BB60[];

extern void *memset(void *s, s32 c, u32 n);

extern s32 D_00436170;

extern u32 D_00436128;

extern u32 D_00389988[];

typedef struct {
    u8 type;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0xC];
    s32 vectorX;     /* 0x1C: x arg of evtSetDrawVectorTarget */
    s32 vectorZ;     /* 0x20: z arg of evtSetDrawVectorTarget */
    s32 vectorY;     /* 0x24: y arg of evtSetDrawVectorTarget */
    s32 vectorW;     /* 0x28: w arg of evtSetDrawVectorTarget */
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
} FldLightSet; /* 0xE0 bytes */

extern void *D_004360F0;

extern s32 kwlnSetDrawColorTarget(s32, void *);

extern s32 func_00107EF8(s32, s32, void *);

extern s32 kwlnSetBackgroundColorTarget(s32, void *);

extern s32 func_00107FF8(s32, s32, void *);

extern s32 evtSetDrawVectorTarget(s32, f32, f32, f32, f32);

extern void fldSetSwayMode(u32);

extern void *fldSkyLightSetBuffer;

extern s32 fldCameraColorEffect;

extern u32 fldCameraColorEnabled;

typedef struct FldColorParams {
    s32 enabled;
    s32 slotIndex;   /* 0x04: stored to the effect work at +0x38 */
    s32 mode;
    s32 red;
    s32 green;
    s32 blue;
    s32 vectorY;     /* 0x18: copied to the effect work at +0x24 */
    s32 vectorZ;     /* 0x1C: copied to the effect work at +0x3C */
} FldColorParams;

typedef struct FldCameraSetting {
    s32 unk0;
    FldColorParams color;
    u8 pad24[0x30];
} FldCameraSetting; /* 0x54 bytes */

typedef struct FldFadeColor {
    u8 pad0[4];
    s32 colorA;
    s32 colorB;
    u8 padC[0x18];
    s32 unk24;
    s32 unk28;
    u8 pad2C[0xC];
    s32 unk38;
    f32 unk3C;
} FldFadeColor;

extern FldFadeColor fldCameraColorParameters[];

extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting fldAppliedCameraSettings[];

extern u32 effCreateSelectionFlagListFromWork(const void *);

extern void effReleaseSelectionFlagList(s32);

INCLUDE_ASM(const s32, "game/code_001360B8", func_001360B8);

INCLUDE_ASM(const s32, "game/code_001360B8", fldSetDisplayState);

void fldInitializeDisplayPointerTable(void) {
    u32 *displayTable = D_0038BBD8;

    memset(displayTable, 0, 0x14);
    displayTable[0] = (u32)D_0038BB50;
    displayTable[1] = (u32)D_0038BB60;
}

extern s32 D_00389780[];
extern s32 D_00436168;
extern u32 fldPlayerObject;
struct EvtUnitNode;
extern s32 evtUnitGetNestedValue(struct EvtUnitNode *unit);
extern void evtSetUnitStatusFlags(EvtUnit *unit);
extern void func_0023C870(EvtUnit *unit, s32 index, u32 colorA, u32 colorB);
extern void evtSetUnitNormalizedDirection(EvtUnit *unit, s32 index);

/* Sky lighting also supplies the player's packed colors and VU direction. */
void fldApplySkyLightSetToPlayerVU(void) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    EvtUnit *unit;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;

    if (D_00389780[0] != 1 && D_00389780[0] < 200) {
        if (D_00436168 != 0) {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[D_00436168];
        } else {
            light = &((FldLightSet *)fldSkyLightSetBuffer)[D_00436128];
        }
        fldSetFadeTarget(D_00389988[13], D_00389988[15], 0);
        fldSetSwayMode(D_00389988[16]);

        vec[0] = light->unk2C * 0.00390625f;
        vec[1] = light->unk30 * 0.00390625f;
        vec[2] = light->unk34 * 0.00390625f;
        vec[3] = 0.0f;
        kwlnSetDrawColorTarget(0, vec);
        evtSetDrawVectorTarget(0, light->vectorX, light->vectorY,
                              light->vectorZ, light->vectorW);

        dir[0] = light->unk44;
        dir[1] = light->unk48;
        dir[2] = light->unk4C;
        dir[3] = 0.0f;
        func_00107FF8(0, 0, dir);
        vec[0] = light->unk38;
        vec[1] = light->unk3C;
        vec[2] = light->unk40;
        vec[3] = 0.0f;
        func_00107EF8(0, 0, vec);
        dir[0] = light->unk5C;
        dir[1] = light->unk60;
        dir[2] = light->unk64;
        dir[3] = 0.0f;
        func_00107FF8(0, 1, dir);
        vec[0] = light->unk50;
        vec[1] = light->unk54;
        vec[2] = light->unk58;
        vec[3] = 0.0f;
        func_00107EF8(0, 1, vec);
        dir[0] = light->unk74;
        dir[1] = light->unk78;
        dir[2] = light->unk7C;
        dir[3] = 0.0f;
        func_00107FF8(0, 2, dir);
        vec[0] = light->unk68;
        vec[1] = light->unk6C;
        vec[2] = light->unk70;
        vec[3] = 0.0f;
        func_00107EF8(0, 2, vec);
        vec[0] = light->unk80;
        vec[1] = light->unk84;
        vec[2] = light->unk88;
        vec[3] = 1.0f;
        kwlnSetBackgroundColorTarget(0, vec);

        unit = (EvtUnit *)evtUnitGetNestedValue((struct EvtUnitNode *)fldPlayerObject);
        evtSetUnitStatusFlags(unit);
        red = light->unitColorA[0] * 128.0f;
        green = light->unitColorA[1] * 128.0f;
        blue = light->unitColorA[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = light->unitColorB[0] * 128.0f;
        green = light->unitColorB[1] * 128.0f;
        blue = light->unitColorB[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        func_0023C870(unit, 0, colorA, colorB);
        dir[0] = light->unitLightDirection[0];
        dir[1] = light->unitLightDirection[1];
        dir[2] = light->unitLightDirection[2];
        dir[3] = 0.0f;
        VU0_LOAD_VF(vf10, dir);
        evtSetUnitNormalizedDirection(unit, 0);
        /* The setter replaces vf10, so the second update reloads it. */
        VU0_LOAD_VF(vf10, dir);
        evtSetUnitNormalizedDirection(unit, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136718);

void fldApplyLightSetCurrent(void) {
    FldLightSet *light = &((FldLightSet *)D_004360F0)[D_00436128];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->type;
    D_00389988[13] = area;
    D_00389988[14] = light->unk4;
    value = light->unk8;
    D_00389988[15] = value;
    D_00389988[16] = light->unkC;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_00389988[16]);
    vec[0] = light->unk2C * 0.00390625f;
    vec[1] = light->unk30 * 0.00390625f;
    vec[2] = light->unk34 * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->vectorX, light->vectorY, light->vectorZ, light->vectorW);
    dir[0] = light->unk44;
    dir[1] = light->unk48;
    dir[2] = light->unk4C;
    dir[3] = 0;
    func_00107FF8(0, 0, dir);
    vec[0] = light->unk38;
    vec[1] = light->unk3C;
    vec[2] = light->unk40;
    vec[3] = 0;
    func_00107EF8(0, 0, vec);
    dir[0] = light->unk5C;
    dir[1] = light->unk60;
    dir[2] = light->unk64;
    dir[3] = 0;
    func_00107FF8(0, 1, dir);
    vec[0] = light->unk50;
    vec[1] = light->unk54;
    vec[2] = light->unk58;
    vec[3] = 0;
    func_00107EF8(0, 1, vec);
    dir[0] = light->unk74;
    dir[1] = light->unk78;
    dir[2] = light->unk7C;
    dir[3] = 0;
    func_00107FF8(0, 2, dir);
    vec[0] = light->unk68;
    vec[1] = light->unk6C;
    vec[2] = light->unk70;
    vec[3] = 0;
    func_00107EF8(0, 2, vec);
    vec[0] = light->unk80;
    vec[1] = light->unk84;
    vec[2] = light->unk88;
    vec[3] = 1.0f;
    kwlnSetBackgroundColorTarget(0, vec);
}

void fldApplyLightSetIndex(s32 index) {
    FldLightSet *light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    D_00436128 = index;
    area = light->type;
    D_00389988[13] = area;
    D_00389988[14] = light->unk4;
    value = light->unk8;
    D_00389988[15] = value;
    D_00389988[16] = light->unkC;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_00389988[16]);
    vec[0] = light->unk2C * 0.00390625f;
    vec[1] = light->unk30 * 0.00390625f;
    vec[2] = light->unk34 * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->vectorX, light->vectorY, light->vectorZ, light->vectorW);
    dir[0] = light->unk44;
    dir[1] = light->unk48;
    dir[2] = light->unk4C;
    dir[3] = 0;
    func_00107FF8(0, 0, dir);
    vec[0] = light->unk38;
    vec[1] = light->unk3C;
    vec[2] = light->unk40;
    vec[3] = 0;
    func_00107EF8(0, 0, vec);
    dir[0] = light->unk5C;
    dir[1] = light->unk60;
    dir[2] = light->unk64;
    dir[3] = 0;
    func_00107FF8(0, 1, dir);
    vec[0] = light->unk50;
    vec[1] = light->unk54;
    vec[2] = light->unk58;
    vec[3] = 0;
    func_00107EF8(0, 1, vec);
    dir[0] = light->unk74;
    dir[1] = light->unk78;
    dir[2] = light->unk7C;
    dir[3] = 0;
    func_00107FF8(0, 2, dir);
    vec[0] = light->unk68;
    vec[1] = light->unk6C;
    vec[2] = light->unk70;
    vec[3] = 0;
    func_00107EF8(0, 2, vec);
    vec[0] = light->unk80;
    vec[1] = light->unk84;
    vec[2] = light->unk88;
    vec[3] = 1.0f;
    kwlnSetBackgroundColorTarget(0, vec);
}

void fldActivateCameraColorSetting(s32 enable) {
    FldCameraSetting *setting;
    FldColorParams *color;

    if (fldCameraColorEnabled == 0 && enable != 0) {
        if (fldCameraColorEffect != 0) {
            effReleaseSelectionFlagList(fldCameraColorEffect);
        }
        setting = fldCameraSettings;
        fldCameraColorEffect = 0;
        color = &setting->color;
        if (color->enabled != 0) {
            fldCameraColorParameters->colorB = fldCameraColorParameters->colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
            fldCameraColorParameters->unk24 = color->vectorY;
            switch (color->mode) {
            case 0:
                fldCameraColorParameters->unk28 = 1;
                break;
            case 1:
                fldCameraColorParameters->unk28 = 2;
                break;
            default:
                fldCameraColorParameters->unk28 = 3;
                break;
            }
            fldCameraColorParameters->unk38 = color->slotIndex;
            fldCameraColorParameters->unk3C = color->vectorZ;
            fldCameraColorEffect = effCreateSelectionFlagListFromWork(fldCameraColorParameters);
            setting = fldCameraSettings;
        }
    } else {
        setting = fldCameraSettings;
    }
    *fldAppliedCameraSettings = *setting;
    fldCameraColorEnabled = enable;
}

s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha) {
    s32 scaled;

    if (fade < 0) {
        fade = 0;
    }
    scaled = alpha * D_00436170 / 100;
    if (fade < 0xE0) {
        return color | (scaled << 24);
    }
    scaled = (1.0f - (f32)(fade - 0xE0) * 0.00390625f) * scaled;
    if (scaled < 0) {
        scaled = 0;
    }
    if (scaled > 0x80) {
        scaled = 0x80;
    }
    return color | (scaled << 24);
}

INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436168);

INCLUDE_SDATA(const s32, "game/code_001360B8", D_0043616C);

INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436170);

