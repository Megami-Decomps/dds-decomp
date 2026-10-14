#include "common.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "pcp_vu0.h"
#include "fld.h"
#include "sdf.h"
#include "fld_light_set.h"

extern u32 D_0038BBD8[];

extern u32 D_0038BB50[];

extern u32 D_0038BB60[];

extern void *memset(void *s, s32 c, u32 n);

extern s32 D_00436170;

extern u32 D_00436128;
extern u32 D_00389988[];


extern void *D_004360F0;

extern void kwlnSetDrawColorTarget(s32, f32 *);

extern s32 kwlnSetLightColorTarget(s32, s32, void *);

extern void kwlnSetBackgroundColorTarget(s32, f32 *);

extern s32 kwlnSetLightDirectionTarget(s32, s32, void *);

extern void evtSetDrawVectorTarget(s32, f32, f32, f32, f32);

extern void fldSetSwayMode(u32);

extern void *fldSkyLightSetBuffer;

extern SdfFlagListWork *fldCameraColorEffect;

extern u32 fldCameraColorEnabled;




extern SdfFlagListParams fldCameraColorParameters[];

extern FldCameraSetting *fldCameraSettings;

extern FldCameraSetting fldAppliedCameraSettings[];



extern s32 D_00389780[];
extern s32 D_004360E8;
extern s32 D_00436168;
extern s32 D_0043616C;
extern void fldSetPlayerAndPeerLighting(s32 duration, f32 redA, f32 greenA,
                                       f32 blueA, f32 redB, f32 greenB, f32 blueB,
                                       f32 x, f32 y, f32 z);

/* Apply the selected scene-light set with the requested transition duration. */
void fldApplySceneLightSet(s32 index, s32 mode) {
    FldLightSet *light;
    f32 vec[4];
    f32 dir[4];
    s32 duration;
    s32 defaultDuration;
    s32 area;
    s32 value;

    if (D_00389780[0] < 200) {
        if (D_004360E8 == 0 && D_00389780[0] < 40) {
            mode = 0;
        }
        defaultDuration = 15;
        duration = 0;
        if (mode != 0) {
            duration = defaultDuration;
        }
        if (mode >= 2) {
            duration = mode;
        }
        if (D_00436168 != index) {
            if (index == 0) {
                index = D_00436128;
            }
            light = &((FldLightSet *)fldSkyLightSetBuffer)[index];
            D_0043616C = D_00389988[11];
            D_00389988[11] = index;
            D_00436168 = index;
            area = light->fadeArea;
            D_00389988[13] = area;
            D_00389988[14] = light->displayStateSlot14;
            value = light->fadeMode;
            D_00389988[15] = value;
            D_00389988[16] = light->swayMode;
            fldSetFadeTarget(area, value, duration);
            fldSetSwayMode(D_00389988[16]);
            vec[0] = light->drawColorFixedR * 0.00390625f;
            vec[1] = light->drawColorFixedG * 0.00390625f;
            vec[2] = light->drawColorFixedB * 0.00390625f;
            vec[3] = 0;
            kwlnSetDrawColorTarget(duration, vec);
            evtSetDrawVectorTarget(duration, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
            dir[0] = light->lightDirectionAX;
            dir[1] = light->lightDirectionAY;
            dir[2] = light->lightDirectionAZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 0, dir);
            vec[0] = light->lightColorAX;
            vec[1] = light->lightColorAY;
            vec[2] = light->lightColorAZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 0, vec);
            dir[0] = light->lightDirectionBX;
            dir[1] = light->lightDirectionBY;
            dir[2] = light->lightDirectionBZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 1, dir);
            vec[0] = light->lightColorBX;
            vec[1] = light->lightColorBY;
            vec[2] = light->lightColorBZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 1, vec);
            dir[0] = light->lightDirectionCX;
            dir[1] = light->lightDirectionCY;
            dir[2] = light->lightDirectionCZ;
            dir[3] = 0;
            kwlnSetLightDirectionTarget(duration, 2, dir);
            vec[0] = light->lightColorCX;
            vec[1] = light->lightColorCY;
            vec[2] = light->lightColorCZ;
            vec[3] = 0;
            kwlnSetLightColorTarget(duration, 2, vec);
            vec[0] = light->backgroundColorX;
            vec[1] = light->backgroundColorY;
            vec[2] = light->backgroundColorZ;
            vec[3] = 1.0f;
            kwlnSetBackgroundColorTarget(duration, vec);
            fldSetPlayerAndPeerLighting(duration, light->unitColorA[0], light->unitColorA[1], light->unitColorA[2],
                          light->unitColorB[0], light->unitColorB[1], light->unitColorB[2],
                          light->unitLightDirection[0], light->unitLightDirection[1],
                          light->unitLightDirection[2]);
        }
    }
}

extern u32 D_00436158;
extern s32 D_00436168;

void fldSetDisplayState(u32 state) {
    D_00436128 = state;
    D_00436158 = 0;
    D_00389988[12] = 0;
    D_00389988[11] = state;
    D_00436168 = 0;
}

void fldInitializeDisplayPointerTable(void) {
    u32 *displayTable = D_0038BBD8;

    memset(displayTable, 0, 0x14);
    displayTable[0] = (u32)D_0038BB50;
    displayTable[1] = (u32)D_0038BB60;
}

extern u32 fldPlayerObject;
struct EffWorldNode;

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

        vec[0] = light->drawColorFixedR * 0.00390625f;
        vec[1] = light->drawColorFixedG * 0.00390625f;
        vec[2] = light->drawColorFixedB * 0.00390625f;
        vec[3] = 0.0f;
        kwlnSetDrawColorTarget(0, vec);
        evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue,
                              light->depthNearValue, light->depthFarValue);

        dir[0] = light->lightDirectionAX;
        dir[1] = light->lightDirectionAY;
        dir[2] = light->lightDirectionAZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 0, dir);
        vec[0] = light->lightColorAX;
        vec[1] = light->lightColorAY;
        vec[2] = light->lightColorAZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 0, vec);
        dir[0] = light->lightDirectionBX;
        dir[1] = light->lightDirectionBY;
        dir[2] = light->lightDirectionBZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 1, dir);
        vec[0] = light->lightColorBX;
        vec[1] = light->lightColorBY;
        vec[2] = light->lightColorBZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 1, vec);
        dir[0] = light->lightDirectionCX;
        dir[1] = light->lightDirectionCY;
        dir[2] = light->lightDirectionCZ;
        dir[3] = 0.0f;
        kwlnSetLightDirectionTarget(0, 2, dir);
        vec[0] = light->lightColorCX;
        vec[1] = light->lightColorCY;
        vec[2] = light->lightColorCZ;
        vec[3] = 0.0f;
        kwlnSetLightColorTarget(0, 2, vec);
        vec[0] = light->backgroundColorX;
        vec[1] = light->backgroundColorY;
        vec[2] = light->backgroundColorZ;
        vec[3] = 1.0f;
        kwlnSetBackgroundColorTarget(0, vec);

        unit = evtUnitGetNestedValue((struct EffWorldNode *)fldPlayerObject);
        evtSetUnitStatusFlags(unit);
        red = light->unitColorA[0] * 128.0f;
        green = light->unitColorA[1] * 128.0f;
        blue = light->unitColorA[2] * 128.0f;
        colorA = red | (blue << 16) | (green << 8) | 0x80000000;
        red = light->unitColorB[0] * 128.0f;
        green = light->unitColorB[1] * 128.0f;
        blue = light->unitColorB[2] * 128.0f;
        colorB = red | (blue << 16) | (green << 8) | 0x80000000;
        evtInitializeUnitColorTransition(unit, 0, colorA, colorB);
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

extern u32 D_00435F1C;

void fldApplyLightSetToChaseCameraUnit(void) {
    FldLightSet *light = &((FldLightSet *)fldSkyLightSetBuffer)[D_00436128];
    f32 dir[4];
    EvtUnit *unit;
    u32 colorA;
    u32 colorB;
    s32 red, green, blue;
    s32 index;

    unit = evtUnitGetNestedValue((struct EffWorldNode *)D_00435F1C);
    index = 0;
    evtSetUnitStatusFlags(unit);
    red = light->unitColorA[0] * 128.0f;
    green = light->unitColorA[1] * 128.0f;
    blue = light->unitColorA[2] * 128.0f;
    colorA = red | (blue << 16) | (green << 8) | 0x80000000;
    red = light->unitColorB[0] * 128.0f;
    green = light->unitColorB[1] * 128.0f;
    blue = light->unitColorB[2] * 128.0f;
    colorB = red | (blue << 16) | (green << 8) | 0x80000000;
    evtInitializeUnitColorTransition(unit, 0, colorA, colorB);
    dir[0] = light->unitLightDirection[0];
    dir[1] = light->unitLightDirection[1];
    dir[2] = light->unitLightDirection[2];
    dir[3] = 0.0f;
    VU0_LOAD_VF(vf10, dir);
    evtSetUnitNormalizedDirection(unit, index);
    VU0_LOAD_VF(vf10, dir);
    evtSetUnitNormalizedDirection(unit, 0);
}

void fldApplyLightSetCurrent(void) {
    FldLightSet *light = &((FldLightSet *)D_004360F0)[D_00436128];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->fadeArea;
    D_00389988[13] = area;
    D_00389988[14] = light->displayStateSlot14;
    value = light->fadeMode;
    D_00389988[15] = value;
    D_00389988[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_00389988[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
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
    area = light->fadeArea;
    D_00389988[13] = area;
    D_00389988[14] = light->displayStateSlot14;
    value = light->fadeMode;
    D_00389988[15] = value;
    D_00389988[16] = light->swayMode;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_00389988[16]);
    vec[0] = light->drawColorFixedR * 0.00390625f;
    vec[1] = light->drawColorFixedG * 0.00390625f;
    vec[2] = light->drawColorFixedB * 0.00390625f;
    vec[3] = 0;
    kwlnSetDrawColorTarget(0, vec);
    evtSetDrawVectorTarget(0, light->fogNearValue, light->fogFarValue, light->depthNearValue, light->depthFarValue);
    dir[0] = light->lightDirectionAX;
    dir[1] = light->lightDirectionAY;
    dir[2] = light->lightDirectionAZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 0, dir);
    vec[0] = light->lightColorAX;
    vec[1] = light->lightColorAY;
    vec[2] = light->lightColorAZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 0, vec);
    dir[0] = light->lightDirectionBX;
    dir[1] = light->lightDirectionBY;
    dir[2] = light->lightDirectionBZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 1, dir);
    vec[0] = light->lightColorBX;
    vec[1] = light->lightColorBY;
    vec[2] = light->lightColorBZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 1, vec);
    dir[0] = light->lightDirectionCX;
    dir[1] = light->lightDirectionCY;
    dir[2] = light->lightDirectionCZ;
    dir[3] = 0;
    kwlnSetLightDirectionTarget(0, 2, dir);
    vec[0] = light->lightColorCX;
    vec[1] = light->lightColorCY;
    vec[2] = light->lightColorCZ;
    vec[3] = 0;
    kwlnSetLightColorTarget(0, 2, vec);
    vec[0] = light->backgroundColorX;
    vec[1] = light->backgroundColorY;
    vec[2] = light->backgroundColorZ;
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
            fldCameraColorParameters->color.finalColor = fldCameraColorParameters->color.initialColor = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
            fldCameraColorParameters->alpha.alpha = color->vectorY;
            switch (color->mode) {
            case 0:
                fldCameraColorParameters->alpha.surfaceIndex = 1;
                break;
            case 1:
                fldCameraColorParameters->alpha.surfaceIndex = 2;
                break;
            default:
                fldCameraColorParameters->alpha.surfaceIndex = 3;
                break;
            }
            fldCameraColorParameters->count = color->slotIndex;
            fldCameraColorParameters->speed = color->vectorZ;
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

