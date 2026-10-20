#include "snd_command_packet.h"
#include "common.h"
#include "scr_integer_return_value_api.h"
#include "scr_string_parameter_api.h"
#include "dds3_world_selection.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "dds3_world_value.h"
#include "mnu_sound_resource_path_api.h"
#include "mnu_voice_resource_path_api.h"
#include "ui_color.h"
#include "mnu_result.h"
#include "brs_result_counter.h"
#include "dds3_world_counter.h"
#include "snd_track_playback_api.h"
/* Declared before first use (were implicit int). */
extern void FlushCache(s32);
extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern s32 sceSifDmaStat(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sdfSoundSendNamedCommand(const char *name, u8 channel);
extern void sdfSoundStopNamedPlayback(void);



void brsUpdateResultBackgroundFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                  BrsProgressRow *row, s32 context, s32 rowIndex) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[rowIndex].backgroundState == 0) {
        u32 opacity = uiBlendColors(0x80808080, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[rowIndex].backgroundOpacity = opacity;
        if (work->fadeAnimation[rowIndex].backgroundOpacity >= 0x80) {
            work->fadeAnimation[rowIndex].backgroundState = 1;
        }
    }
}
INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBB0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBC0);

INCLUDE_ASM(const s32, "game/code_002665E0", brsDrawProgressRowEntries);

INCLUDE_ASM(const s32, "game/code_002665E0", brsDrawProgressRowUnitInfo);

void brsUpdateResultPortraitFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                 BrsProgressRow *row, s32 context, s32 rowIndex) {
    s32 remaining = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[rowIndex].portraitReady == 0) {
        u32 opacity = uiBlendColors(0x80808060, 0x80808000, remaining) & 0xFF;

        work->fadeAnimation[rowIndex].portraitOpacity = opacity;
        work->fadeAnimation[rowIndex].portraitPosition[0] = 0;
        work->fadeAnimation[rowIndex].portraitPosition[1] = 0;
        if (work->fadeAnimation[rowIndex].portraitOpacity >= 0x60) {
            work->fadeAnimation[rowIndex].portraitReady = 1;
        }
    }
}
#include "sdf.h"
extern SdfPoolNode kwlnDrawSurfaces[];
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void evtSetDrawSurfaceIndex(u32 surfaceIndex);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void btlDrawButtonIconFixed64(SdfPoolNode *, s32, s32, s32, s32, s32, s32, s32);
/* The four sprite corner colours start from the shared grey table. */
typedef struct BrsCornerColors {
    u32 rgba[4];
} BrsCornerColors;
extern const BrsCornerColors D_003AFC40;

void brsDrawProgressRowOverlays(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 surfaceIndex, s32 rowIndex) {
    DatPartyRecord *unit = progressRow->unit;
    DatPartyRecord *tintUnit;
    BrsCornerColors color = D_003AFC40;
    s32 iconX;
    s32 iconY;
    u32 packed;

    if (unit->flags & DAT_PARTY_FLAG_FRONTLINE) {
        iconX = 8;
    } else {
        iconX = 0x18;
    }
    iconX += work->fadeAnimation[rowIndex].portraitPosition[0];
    iconY = work->fadeAnimation[rowIndex].portraitPosition[1] - 0x14;
    evtSetDrawSurfaceIndex(surfaceIndex);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);

    tintUnit = progressRow->unit;
    if (tintUnit->hp != 0 && !(tintUnit->status & 0x4000)) {
        packed = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x00808080;
        color.rgba[0] = packed;
        color.rgba[1] = packed;
        color.rgba[2] = packed;
        color.rgba[3] = packed;
    } else {
        color.rgba[0] = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x00282222;
        packed = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x002C242A;
        color.rgba[1] = packed;
        color.rgba[2] = packed;
        color.rgba[3] = packed;
    }

    btlDrawButtonIconFixed64(&kwlnDrawSurfaces[surfaceIndex], iconX + (x >> 4),
                             iconY + (y >> 3), color.rgba[0], color.rgba[1], color.rgba[2],
                             color.rgba[3], unit->unitId);

    if (unit->hp == 0 || (unit->status & 0x4000)) {
        if (unit->flags & DAT_PARTY_FLAG_FRONTLINE) {
            iconX = 1;
            iconY = 9;
        } else {
            iconX = 0x16;
            iconY = 9;
        }

        packed = (work->fadeAnimation[rowIndex].portraitOpacity + 0x20) | 0x80808000;
        color.rgba[0] = packed;
        color.rgba[1] = packed;
        color.rgba[2] = packed;
        color.rgba[3] = packed;

        if (work->teardownResource != NULL) {
            effDrawSpriteWithCornerColors(x + (iconX << 4), y + (iconY << 3), z,
                          color.rgba, 0, work->teardownResource, 0xF, surfaceIndex);
        }
    }
}


extern void mnuSetTitleSequenceVolumePan(u32 sequenceId);
extern u32 ptyComputeTotalExp(DatPartyRecord *unit, s32 levelDelta);
extern s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);

void brsUpdateLevelProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 context, s32 rowIndex) {
    s32 level = work->levelAnimation[rowIndex].level;
    s32 fade = 0x100 - work->fadeProgress;
    s32 nextLevel = level + 1;
    s32 currentExp;
    s32 finished;

    nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
    switch (work->levelAnimation[rowIndex].state) {
    case 0:
        work->levelAnimation[rowIndex].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->levelAnimation[rowIndex].alpha >= 0x80) {
            work->levelAnimation[rowIndex].state = 1;
        }
        break;
    case 1:
        work->levelAnimation[rowIndex].state++;
        if (work->levelAnimation[rowIndex].remaining > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (level < 99 && progressRow->unit->hp != 0 && (progressRow->unit->status & 0x4000) == 0) {
            if (work->levelAnimation[rowIndex].state != 3) {
                work->levelAnimation[rowIndex].applied = brsCalcProgressAnimationStep(work, progressRow, rowIndex, 0);
                currentExp = work->levelAnimation[rowIndex].currentProgress + work->levelAnimation[rowIndex].applied;
                work->levelAnimation[rowIndex].currentProgress = currentExp;
                if (currentExp >= (s32)ptyComputeTotalExp(progressRow->unit, nextLevel - progressRow->unit->level) && work->levelAnimation[rowIndex].state == 2) {
                    work->levelAnimation[rowIndex].level++;
                    work->levelAnimation[rowIndex].level = work->levelAnimation[rowIndex].level < 2 ? 1 : (work->levelAnimation[rowIndex].level > 99 ? 99 : work->levelAnimation[rowIndex].level);
                    if (work->skipProgressAnimation != 0) {
                        finished = 0;
                        while (work->levelAnimation[rowIndex].level < 99) {
                            nextLevel = work->levelAnimation[rowIndex].level + 1;
                            nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
                            if (currentExp >= (s32)ptyComputeTotalExp(progressRow->unit, nextLevel - progressRow->unit->level)) {
                                work->levelAnimation[rowIndex].level++;
                                work->levelAnimation[rowIndex].level = work->levelAnimation[rowIndex].level < 2 ? 1 : (work->levelAnimation[rowIndex].level > 99 ? 99 : work->levelAnimation[rowIndex].level);
                            } else {
                                finished = 1;
                            }
                            if (work->levelAnimation[rowIndex].level >= 99) {
                                finished = 1;
                            }
                            if (finished != 0) {
                                break;
                            }
                        }
                    }
                    work->levelAnimation[rowIndex].iconState = 1;
                    work->levelAnimation[rowIndex].completionState = 1;
                    work->levelAnimation[rowIndex].completionFlashActive = 1;
                    work->levelAnimation[rowIndex].iconOpacity = 0x80;
                    work->levelAnimation[rowIndex].iconFrame = 0;
                    work->levelAnimation[rowIndex].alpha = 0;
                    work->levelAnimation[rowIndex].state++;
                    mnuSetTitleSequenceVolumePan(0x12);
                }
            }
        } else {
            work->levelAnimation[rowIndex].remaining = 0;
        }
        if (work->levelAnimation[rowIndex].state == 3) {
            work->levelAnimation[rowIndex].iconFrame++;
            work->levelAnimation[rowIndex].iconFrame = work->levelAnimation[rowIndex].iconFrame <= 0 ? 0 : (work->levelAnimation[rowIndex].iconFrame > 15 ? 15 : work->levelAnimation[rowIndex].iconFrame);
            if (work->levelAnimation[rowIndex].iconFrame >= 15) {
                work->levelAnimation[rowIndex].state++;
            }
        } else if (work->levelAnimation[rowIndex].state == 4) {
            work->levelAnimation[rowIndex].iconOpacity -= 16;
            work->levelAnimation[rowIndex].iconOpacity = work->levelAnimation[rowIndex].iconOpacity <= 0 ? 0 : (work->levelAnimation[rowIndex].iconOpacity > 128 ? 128 : work->levelAnimation[rowIndex].iconOpacity);
            if (work->levelAnimation[rowIndex].iconOpacity <= 0) {
                work->levelAnimation[rowIndex].completionFlashActive = 0;
            }
            work->levelAnimation[rowIndex].alpha += 8;
            work->levelAnimation[rowIndex].alpha = work->levelAnimation[rowIndex].alpha <= 0 ? 0 : (work->levelAnimation[rowIndex].alpha > 128 ? 128 : work->levelAnimation[rowIndex].alpha);
            if (work->levelAnimation[rowIndex].alpha >= 128) {
                work->levelAnimation[rowIndex].state = 2;
                if (work->levelAnimation[rowIndex].remaining > 0 && work->resultPhase == 0) {
                    mnuSetTitleSequenceVolumePan(0x14);
                    work->resultPhase = 1;
                }
            }
        }
        break;
    }
}

#include "eff.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"
extern struct FrFontGlyph *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct FrFontGlyph *);
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);
extern void *memcpy(void *, const void *, u32);
extern s32 sprintf(char *, const char *, ...);
extern const char D_003BC578[];

typedef struct BrsLevelCornerColors {
    u32 values[4];
} BrsLevelCornerColors;
extern const BrsLevelCornerColors D_003AFC50;

void brsDrawLevelProgressDetails(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                BrsProgressRow *row, s32 context, s32 index) {
    char text[32];
    BrsLevelCornerColors colors;
    u32 color = work->levelAnimation[index].alpha | 0x80808000;
    u32 fade = 0x100 - work->fadeProgress;
    s16 candidate;
    s16 nextLevel;
    s16 level;
    s32 totalProgress;
    s32 levelDelta;
    s32 baseExp;
    s32 progress;
    s32 required;
    struct FrFontGlyph *glyph;
    s32 rawPercent;
    s32 displayPercent;
    EffectSlotSet *resource;
    BdWork *bar;
    EffectSlotDescription *description;
    s32 width;
    s32 slot = 20;

    colors = D_003AFC50;
    colors.values[0] = color;
    colors.values[1] = color;
    colors.values[2] = color;
    colors.values[3] = color;
    level = work->levelAnimation[index].level;
    totalProgress = work->levelAnimation[index].currentProgress;
    candidate = work->levelAnimation[index].level + 1;
    nextLevel = candidate < 2 ? 1 : (candidate > 99 ? 99 : candidate);
    if (level < 99) {
        levelDelta = level - row->unit->level;
        baseExp = (s32)ptyComputeTotalExp(row->unit, levelDelta);
        if (totalProgress < baseExp) {
            progress = totalProgress;
            required = (s32)ptyComputeTotalExp(row->unit, nextLevel - level + levelDelta);
        } else {
            progress = totalProgress - baseExp;
            required = (s32)ptyComputeTotalExp(row->unit, nextLevel - level + levelDelta) - baseExp;
        }
        if (work->levelAnimation[index].completionFlashActive != 0 &&
            work->levelAnimation[index].state == 3) {
            required = 0;
            progress = 0;
        }
        if (row->unit->hp != 0 && !(row->unit->status & 0x4000)) {
            color = uiBlendColors(0x80808080, 0x80808000, fade);
        } else {
            color = uiBlendColors(0x4F4E4E80, 0x4F4E4E00, fade);
        }
        sprintf(text, D_003BC578, required - progress);
        glyph = itfCreateFont2TextGlyph(x + 0xC10, y + 0x98, z, color, text, NULL);
        frFontDrawGlyphChain(glyph, 1, context);
        frDestroyFontRequestAfter(glyph);
        if (progress > 0) {
            if (required != 0) {
                displayPercent = progress * 1000 / required;
                if (displayPercent >= 1001) {
                    if (required < progress) {
                        displayPercent %= 1000;
                    }
                }
                rawPercent = displayPercent;
                if (work->levelAnimation[index].progressInitialized == 0) {
                    work->levelAnimation[index].previousProgress = displayPercent;
                    work->levelAnimation[index].progressInitialized = 1;
                }
                if (displayPercent < 41) {
                    displayPercent = 40;
                }
                if (work->levelAnimation[index].remaining == 0) {
                    if (work->levelAnimation[index].previousProgress < rawPercent) {
                        if (rawPercent - work->levelAnimation[index].previousProgress < 11) {
                            displayPercent += 10;
                        }
                    }
                }
                displayPercent = displayPercent < 1 ? 0 : (displayPercent > 1000 ? 1000 : displayPercent);
                resource = work->teardownResource;
                bar = &resource->workEntries[slot];
                description = &resource->descriptions[slot];
                bar->geometry.bounds[2] = (bar->sourceWidth << 4) * displayPercent / 1000;
                width = description->colors[2] - description->colors[0];
                width = width * displayPercent / 1000 - width;
                bar->parameters[2] = width;
                effDrawSpriteWithCornerColors(x + 0x9B0, y + 0x120, z, colors.values,
                                              0, resource, slot, context);
                work->teardownResource->workEntries[slot].parameters[2] = 0;
                work->teardownResource->workEntries[slot].geometry.bounds[2] = work->teardownResource->workEntries[slot].sourceWidth * 16;
            }
        }
        if (work->levelAnimation[index].completionFlashActive != 0) {
            color = work->levelAnimation[index].iconOpacity | 0x80808000;
            colors.values[0] = color;
            colors.values[1] = color;
            colors.values[2] = color;
            colors.values[3] = color;
            effDrawSpriteWithCornerColors(x + 0x9B0, y + 0x120, z, colors.values,
                                          0, work->teardownResource, 20, context);
        }
    }
}


void brsUpdateLevelProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 progressRowIndex) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[progressRowIndex].iconState) {
    case 0:
        work->levelAnimation[progressRowIndex].iconColor = 0;
        work->levelAnimation[progressRowIndex].iconPosition[0] = 0xA6;
        work->levelAnimation[progressRowIndex].iconPosition[1] = 0x22;
        work->levelAnimation[progressRowIndex].iconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[progressRowIndex].iconAngle = (work->levelAnimation[progressRowIndex].iconAngle + 15) % 360;
        work->levelAnimation[progressRowIndex].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[progressRowIndex].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[progressRowIndex].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[progressRowIndex].iconPosition[1] = 0x24 - bob;
        work->levelAnimation[progressRowIndex].iconColor = (work->levelAnimation[progressRowIndex].iconColor > 0) ? ((work->levelAnimation[progressRowIndex].iconColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].iconColor) : 0;
        y = work->levelAnimation[progressRowIndex].iconPosition[1];
        work->levelAnimation[progressRowIndex].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[progressRowIndex].iconColor >= 0x80) {
            work->levelAnimation[progressRowIndex].iconState += 2;
            work->levelAnimation[progressRowIndex].iconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[progressRowIndex].iconState++;
        work->levelAnimation[progressRowIndex].iconAngle = 0x78;
        break;
    default:
        work->levelAnimation[progressRowIndex].iconColor -= 10;
        work->levelAnimation[progressRowIndex].iconColor = (work->levelAnimation[progressRowIndex].iconColor > 0) ? ((work->levelAnimation[progressRowIndex].iconColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].iconColor) : 0;
        y = work->levelAnimation[progressRowIndex].iconPosition[1] - 1;
        work->levelAnimation[progressRowIndex].iconPosition[1] = y;
        work->levelAnimation[progressRowIndex].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[progressRowIndex].iconColor <= 0) {
            work->levelAnimation[progressRowIndex].iconState = 0;
        }
        break;
    }
    switch (work->levelAnimation[progressRowIndex].completionState) {
    case 0:
        work->levelAnimation[progressRowIndex].auxiliaryColor = 0;
        work->levelAnimation[progressRowIndex].auxiliaryPosition[0] = 0x95;
        work->levelAnimation[progressRowIndex].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[progressRowIndex].auxiliaryColor += 10;
        work->levelAnimation[progressRowIndex].auxiliaryColor = (work->levelAnimation[progressRowIndex].auxiliaryColor > 0) ? ((work->levelAnimation[progressRowIndex].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].auxiliaryColor) : 0;
        if (work->levelAnimation[progressRowIndex].auxiliaryColor >= 0x80) {
            work->levelAnimation[progressRowIndex].completionState++;
            work->levelAnimation[progressRowIndex].completionColor = 0;
        }
        break;
    case 2:
        work->levelAnimation[progressRowIndex].completionColor++;
        work->levelAnimation[progressRowIndex].completionColor = (work->levelAnimation[progressRowIndex].completionColor > 0) ? ((work->levelAnimation[progressRowIndex].completionColor > 8) ? 8 : work->levelAnimation[progressRowIndex].completionColor) : 0;
        if (work->levelAnimation[progressRowIndex].completionColor >= 8) {
            work->levelAnimation[progressRowIndex].completionState++;
        }
        break;
    default:
        work->levelAnimation[progressRowIndex].auxiliaryColor -= 0x10;
        work->levelAnimation[progressRowIndex].auxiliaryColor = (work->levelAnimation[progressRowIndex].auxiliaryColor > 0) ? ((work->levelAnimation[progressRowIndex].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].auxiliaryColor) : 0;
        if (work->levelAnimation[progressRowIndex].auxiliaryColor <= 0) {
            work->levelAnimation[progressRowIndex].completionState = 0;
        }
        break;
    }
}



extern const BrsLevelCornerColors D_003AFC60;
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

void brsDrawLevelProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                 BrsProgressRow *row, s32 context, s32 rowIndex) {
    BrsLevelCornerColors colors = D_003AFC60;
    u32 color;
    s32 spriteX;
    s32 spriteY;
    s32 drawY;

    /* 0x00267E60..0x00267E80 retains the palette copy before recoloring. */
    if (work->teardownResource != NULL) {
        if (work->levelAnimation[rowIndex].iconState != 0) {
            spriteX = work->levelAnimation[rowIndex].iconPosition[0];
            spriteY = work->levelAnimation[rowIndex].iconPosition[1];
            color = work->levelAnimation[rowIndex].iconColor | 0x80808000;
            colors.values[0] = color;
            colors.values[1] = color;
            colors.values[2] = color;
            colors.values[3] = color;
            effDrawSpriteWithCornerColors(x + (spriteX << 4), y + (spriteY << 3),
                                          z, colors.values, 0, work->teardownResource, 0xD, context);
        }
        if (work->levelAnimation[rowIndex].completionState != 0) {
            spriteX = work->levelAnimation[rowIndex].auxiliaryPosition[0];
            spriteY = work->levelAnimation[rowIndex].auxiliaryPosition[1];
            color = work->levelAnimation[rowIndex].auxiliaryColor | 0x80808000;
            drawY = y + (spriteY << 3);
            colors.values[0] = color;
            colors.values[1] = color;
            colors.values[2] = color;
            colors.values[3] = color;
            effDrawSpriteWithCornerColors(x + (spriteX << 4), drawY, z, colors.values,
                                          0, work->teardownResource, 0x15, context);
            /* Keep the right-edge coordinate in spriteX; see docs/idioms.md,
             * "Trace-justified level-progress right-edge coordinate". */
            spriteX = work->levelAnimation[rowIndex].auxiliaryPosition[0] + 0x4D;
            effDrawSpriteWithCornerColors(x + (spriteX << 4), drawY, z, colors.values,
                                          0, work->teardownResource, 0x16, context);
        }
    }
}

void brsUpdateProfileProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 rowIndex) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[rowIndex].iconState) {
    case 0:
        work->profileAnimation[rowIndex].iconColor = 0;
        work->profileAnimation[rowIndex].iconPosition[0] = 0x12F;
        work->profileAnimation[rowIndex].iconPosition[1] = 0x22;
        work->profileAnimation[rowIndex].iconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[rowIndex].iconAngle = (work->profileAnimation[rowIndex].iconAngle + 15) % 360;
        work->profileAnimation[rowIndex].iconColor =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[rowIndex].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[rowIndex].iconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[rowIndex].iconPosition[1] = 0x24 - bob;
        work->profileAnimation[rowIndex].iconColor = (work->profileAnimation[rowIndex].iconColor > 0) ? ((work->profileAnimation[rowIndex].iconColor > 0x80) ? 0x80 : work->profileAnimation[rowIndex].iconColor) : 0;
        y = work->profileAnimation[rowIndex].iconPosition[1];
        work->profileAnimation[rowIndex].iconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[rowIndex].iconColor >= 0x80) {
            work->profileAnimation[rowIndex].iconState += 2;
            work->profileAnimation[rowIndex].iconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[rowIndex].iconState++;
        work->profileAnimation[rowIndex].iconAngle = 0x78;
        break;
    default:
        work->profileAnimation[rowIndex].iconColor -= 10;
        work->profileAnimation[rowIndex].iconColor = (work->profileAnimation[rowIndex].iconColor > 0) ? ((work->profileAnimation[rowIndex].iconColor > 0x80) ? 0x80 : work->profileAnimation[rowIndex].iconColor) : 0;
        y = work->profileAnimation[rowIndex].iconPosition[1] - 1;
        work->profileAnimation[rowIndex].iconPosition[1] = y;
        work->profileAnimation[rowIndex].iconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[rowIndex].iconColor <= 0) {
            work->profileAnimation[rowIndex].iconState = 0;
        }
        break;
    }
    switch (work->profileAnimation[rowIndex].completionState) {
    case 0:
        work->profileAnimation[rowIndex].auxiliaryColor = 0;
        work->profileAnimation[rowIndex].auxiliaryPosition[0] = 0x122;
        work->profileAnimation[rowIndex].auxiliaryPosition[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[rowIndex].auxiliaryColor += 10;
        work->profileAnimation[rowIndex].auxiliaryColor = (work->profileAnimation[rowIndex].auxiliaryColor > 0)
            ? ((work->profileAnimation[rowIndex].auxiliaryColor > 0x80) ? 0x80 : work->profileAnimation[rowIndex].auxiliaryColor)
            : 0;
        if (work->profileAnimation[rowIndex].auxiliaryColor >= 0x80) {
            work->profileAnimation[rowIndex].completionState++;
            work->profileAnimation[rowIndex].completionColor = 0;
        }
        break;
    case 2:
        work->profileAnimation[rowIndex].completionColor += 10;
        work->profileAnimation[rowIndex].completionColor = (work->profileAnimation[rowIndex].completionColor > 0)
            ? ((work->profileAnimation[rowIndex].completionColor > 0xFF) ? 0xFF : work->profileAnimation[rowIndex].completionColor)
            : 0;
        if (work->profileAnimation[rowIndex].completionColor >= 0xFF) {
            work->profileAnimation[rowIndex].completionState++;
        }
        break;
    default:
        work->profileAnimation[rowIndex].completionColor -= 4;
        work->profileAnimation[rowIndex].completionColor = (work->profileAnimation[rowIndex].completionColor > 0x80)
            ? ((work->profileAnimation[rowIndex].completionColor > 0xFF) ? 0xFF : work->profileAnimation[rowIndex].completionColor)
            : 0x80;
        break;
    }
}


extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC60);

void brsDrawProfileProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
    BrsProgressRow *row, s32 context, s32 rowIndex) {
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    u32 color;
    s32 drawY;
    s32 spriteX;
    s32 spriteY;
    s32 xOffset;

    if (work->teardownResource != NULL) {
        if (work->profileAnimation[rowIndex].iconState != 0) {
            spriteX = work->profileAnimation[rowIndex].iconPosition[0];
            spriteY = work->profileAnimation[rowIndex].iconPosition[1];
            color = work->profileAnimation[rowIndex].iconColor | 0x80808000;
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            effDrawSpriteWithCornerColors(x + (spriteX << 4),
                y + (spriteY << 3), z, colors, 0,
                work->teardownResource, 0x1A, context);
        }
        if (work->profileAnimation[rowIndex].completionState != 0) {
            spriteX = work->profileAnimation[rowIndex].auxiliaryPosition[0];
            spriteY = work->profileAnimation[rowIndex].auxiliaryPosition[1];
            /* Stage X before Y; see docs/idioms.md,
             * "Trace-justified result-profile coordinates". */
            xOffset = spriteX << 4;
            drawY = y + (spriteY << 3);
            color = work->profileAnimation[rowIndex].auxiliaryColor | 0x80808000;
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            effDrawSpriteWithCornerColors(x + xOffset,
                drawY, z, colors, 0, work->teardownResource, 0x17, context);
            spriteX = work->profileAnimation[rowIndex].auxiliaryPosition[0] + 0x4D;
            effDrawSpriteWithCornerColors(x + (spriteX << 4),
                drawY, z, colors, 0, work->teardownResource, 0x18, context);
            if (work->profileAnimation[rowIndex].completionState >= 2) {
                color = work->profileAnimation[rowIndex].completionColor | 0x80808000;
                colors[0] = color;
                colors[1] = color;
                colors[2] = color;
                colors[3] = color;
                effDrawSpriteWithCornerColors(x + 0x13A0, y + 0xF8, z, colors, 0,
                    work->teardownResource, 0xB, context);
            }
        }
    }
}


extern u32 ptyComputeTotalExp(DatPartyRecord *unit, s32 levelDelta);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);

s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *work, BrsProgressRow *progressRow, s32 rowIndex, s8 progressKind) {
    s32 result = 0;
    s32 levelDelta = work->levelAnimation[rowIndex].level - progressRow->unit->level;
    s32 nextLevelExp = ptyComputeTotalExp(progressRow->unit, levelDelta + 1);
    s32 range = nextLevelExp - ptyComputeTotalExp(progressRow->unit, levelDelta);
    s32 step;
    s32 denominator;

    if (range == 0) {
        range = ptyComputeTotalExp(progressRow->unit, levelDelta + 1);
    }
    if (progressKind == 0) {
        if (work->levelAnimation[rowIndex].remaining > 0) {
            if (work->levelAnimation[rowIndex].skipRamp == 0) {
                work->levelAnimation[rowIndex].frames++;
                work->levelAnimation[rowIndex].frames = work->levelAnimation[rowIndex].frames <= 0 ? 0 : (work->levelAnimation[rowIndex].frames > 120 ? 120 : work->levelAnimation[rowIndex].frames);
                denominator = 150 - work->levelAnimation[rowIndex].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->skipProgressAnimation != 0) {
                step = work->levelAnimation[rowIndex].remaining;
            }
            work->levelAnimation[rowIndex].remaining -= step;
            if (work->levelAnimation[rowIndex].remaining < 0) {
                step += work->levelAnimation[rowIndex].remaining;
                work->levelAnimation[rowIndex].remaining = 0;
            }
            work->levelAnimation[rowIndex].remaining = work->levelAnimation[rowIndex].remaining <= 0 ? 0 : (work->levelAnimation[rowIndex].remaining > 0x1000000 ? 0x1000000 : work->levelAnimation[rowIndex].remaining);
            result = step;
        }
    } else {
        range = prfGetCapValue(ptyGetCurrentProfileId(progressRow->unit));
        if (work->profileAnimation[rowIndex].remaining > 0) {
            if (work->profileAnimation[rowIndex].skipRamp == 0) {
                work->profileAnimation[rowIndex].frames++;
                work->profileAnimation[rowIndex].frames = work->profileAnimation[rowIndex].frames <= 0 ? 0 : (work->profileAnimation[rowIndex].frames > 120 ? 120 : work->profileAnimation[rowIndex].frames);
                denominator = 150 - work->profileAnimation[rowIndex].frames;
                step = 1;
                if (range >= denominator) {
                    step = range / denominator;
                }
            } else {
                step = 10000;
            }
            if (work->skipProgressAnimation != 0) {
                step = work->profileAnimation[rowIndex].remaining;
            }
            work->profileAnimation[rowIndex].remaining -= step;
            if (work->profileAnimation[rowIndex].remaining < 0) {
                step += work->profileAnimation[rowIndex].remaining;
                work->profileAnimation[rowIndex].remaining = 0;
            }
            work->profileAnimation[rowIndex].remaining = work->profileAnimation[rowIndex].remaining <= 0 ? 0 : (work->profileAnimation[rowIndex].remaining > 0x1000000 ? 0x1000000 : work->profileAnimation[rowIndex].remaining);
            result = step;
        }
    }
    return result;
}

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "dds3_world_value.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "mnu_title_effect.h"
#include "file.h"
#include "file_request_api.h"
#include "sdf_thread.h"

#include "mnu_title_stream.h"

#define BRS_RESULT_COUNTER_PAIR_COUNT 5
#define BRS_RESULT_SETTLED_POLL_LIMIT 6
#define BRS_RESULT_SETTLED_POLL_CLAMP 7

#define SND_TRACK_DEFAULT_VOLUME 0x7f
#define SND_TRACK_DEFAULT_PAN 0x3f
#define SND_TRANSFER_WORD_BYTES 4
#define SND_TRANSFER_WORD_HALFWORDS 2
#define SND_TRANSFER_PAIR_WORD_BYTES 8
#define SND_MIX_SAMPLE_COUNT 0x800
#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_EFFECT_BYTES 8
#define MNU_TITLE_VOICE_PREFIX_BYTES 5
#define MNU_TITLE_VOICE_PATH_BYTES 16
#define MNU_TITLE_SOUND_PATH_BYTES 32

/* Each compressed-frame size has a corresponding 600-frame allocation. */
#define MNU_SOUND_FRAME_BYTES_LARGE 0x180
#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0
#define MNU_SOUND_FRAME_BYTES_SMALL 0x60
#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400
#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200
#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100


#define MNU_STREAM_LOAD_IDLE 0
#define MNU_STREAM_LOAD_PENDING 1
#define MNU_STREAM_LOAD_COPIED 2
#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4


extern s32 dds3CreateCameraObject(s32 counter, f32 *position, f32 *rotation);

struct EffWorldNode;

extern void effObjSetInnerFloat(s32 object, f32 value);



extern u8 mnuTitleCameraPosition[];

extern u8 mnuTitleCameraRotation[];

extern void *mnuTitleCameraObject;

extern char D_003AFD48[]; /* "---------- AT3 --------\n", followed by 8 zero bytes no C function emits */

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern u32 mnuTitleAudioSilenceBuffer[];


extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamSemaphore;

extern u32 mnuTitleStreamFileRequest;

extern u32 D_003BC5BC;

extern s32 D_003BC5C8;

extern u8 D_003D9178[];


extern u32 sdfSoundIsCommandBusy(void);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);


extern KwlnTask *mnuTitleSoundTask;

extern char D_003771D8[];

/* libc sprintf returns the signed vfprintf character count. */
extern s32 sprintf(char *dst, const char *fmt, ...);

#include "sdf_debug_api.h"

extern void mnuCreateTitleEffectTask(void);

extern u8 D_003BC5A0[];

extern s32 scrReadIntParameter(s32);
extern u32 sdfSoundIsCommandBusy(void);

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;





extern s32 sceSifInitIopHeap(void);

extern s32 sceSifAllocIopHeap(s32 size);

extern void Exit(s32 status);

extern s32 sndStreamTransferIopHeapAddress;

extern char D_003AFBA0[]; /* "result2_draw" */

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounterView levelCounters[8];
    BrsResultCounterView profileCounters[8];
} BrsResultWork;

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 prfGetCapValue(u16);
extern s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);
extern void mnuSetTitleSequenceVolumePan(u32 sequenceId);


void brsUpdateProfileProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *row, s32 context, s32 rowIndex) {
    s32 fade = 0x100 - work->fadeProgress;
    s32 profileId;
    s32 cap;
    s32 currentProgress;
    s32 increment;

    ptyGetCurrentProfileRecord(row->unit);
    profileId = ptyGetCurrentProfileId(row->unit);
    if (profileId != 0) {
        cap = prfGetCapValue((u16)profileId);
    } else {
        cap = 0;
        work->profileAnimation[rowIndex].remaining = 0;
    }
    currentProgress = work->profileAnimation[rowIndex].currentProgress;
    switch (work->profileAnimation[rowIndex].state) {
    case 0:
        work->profileAnimation[rowIndex].alpha =
            uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->profileAnimation[rowIndex].alpha >= 0x80) {
            work->profileAnimation[rowIndex].state = 1;
        }
        break;
    case 1:
        work->profileAnimation[rowIndex].state++;
        if (work->profileAnimation[rowIndex].remaining > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (work->profileAnimation[rowIndex].state == 2) {
            if (cap != 0) {
                if (currentProgress < cap) {
                    increment = brsCalcProgressAnimationStep(work, row, rowIndex, 1);
                    work->profileAnimation[rowIndex].applied = increment;
                    work->profileAnimation[rowIndex].currentProgress += increment;
                    if (work->profileAnimation[rowIndex].currentProgress >= cap) {
                        work->profileAnimation[rowIndex].iconState = 1;
                        work->profileAnimation[rowIndex].completionState = 1;
                        work->profileAnimation[rowIndex].completionFlashActive = 1;
                        work->profileAnimation[rowIndex].iconOpacity = 0x80;
                        work->profileAnimation[rowIndex].iconFrame = 0;
                        work->profileAnimation[rowIndex].state++;
                        mnuSetTitleSequenceVolumePan(0x12);
                    }
                } else {
                    work->profileAnimation[rowIndex].remaining = 0;
                }
            }
        }
        break;
    }
}



/* Draw the profile name and its remaining-progress gauge.
 * Prior algorithm/research lineage: W16_Snd, PurB, LiteC, PurC, PurD, MatchC,
 * LaneH, NewRule, SdfModelVis, Lite, BtlSlot (dds-collab#17:6237048856).
 * Reconstructed from native loads/stores and truthful provider declarations.
 * Keep the draw-fresh resource reads and guard-edge denominator capture.
 */
#include "eff.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"
#include "fr_font_measure.h"
#include "prf_profile_cap.h"

extern s32 prfResolveMantraNameOrFallback(u16, u8 **);
extern s32 mnuIsTitleEntryAvailable(DatPartyRecord *);

typedef struct BrsProfilePalette { u32 values[4]; } BrsProfilePalette;
extern const BrsProfilePalette D_003AFC80;

void brsDrawProfileProgressDetails(s32 x, s32 y, s32 depth, BrsSkillPackageWork *work,
                                  BrsProgressRow *row, s32 context, s32 index) {
    BrsProfilePalette palette;
    u8 *text;
    s32 currentProfile;
    s32 cap;
    s32 progressAmount;
    s32 phase;
    u16 profileId;
    s32 progress, originalProgress, difference, range;
    s32 textWidth;
    s32 textOffset;
    s32 textY;
    s32 frameX, frameY;
    u32 color = work->profileAnimation[index].alpha | 0x80808000;
    u32 barColor;
    struct FrFontGlyph *glyph;
    EffectSlotSet *resources;
    EffectSlotSet *restoreResources;
    BdWork *bar;
    EffectSlotDescription *description;
    s32 fillSlot = 19;

    palette = D_003AFC80;
    palette.values[0] = color;
    palette.values[1] = color;
    palette.values[2] = color;
    palette.values[3] = color;
    ptyGetCurrentProfileRecord(row->unit);
    currentProfile = ptyGetCurrentProfileId(row->unit);
    if (currentProfile != 0) {
        profileId = currentProfile;
        cap = prfGetCapValue(profileId);
        if (row->unit->hp != 0 && !(row->unit->status & 0x4000)) {
            color = work->profileAnimation[index].alpha | 0x80808000;
        } else {
            color = (work->profileAnimation[index].alpha =
                work->profileAnimation[index].alpha <= 0 ? 0 :
                (work->profileAnimation[index].alpha > 128 ? 128 : work->profileAnimation[index].alpha)) | 0x4F4E4E00;
        }
        if (mnuIsTitleEntryAvailable(row->unit) || (row->unit->status & 0x40)) {
            barColor = 0x48644800 | (work->profileAnimation[index].alpha =
                work->profileAnimation[index].alpha <= 0 ? 0 :
                (work->profileAnimation[index].alpha > 128 ? 128 : work->profileAnimation[index].alpha));
            color = (work->profileAnimation[index].alpha =
                work->profileAnimation[index].alpha <= 0 ? 0 :
                (work->profileAnimation[index].alpha > 128 ? 128 : work->profileAnimation[index].alpha)) | 0x4F4E4E00;
            palette.values[0] = barColor;
            palette.values[1] = barColor;
            palette.values[2] = barColor;
            palette.values[3] = barColor;
        }
        prfResolveMantraNameOrFallback(profileId, &text);
        textY = y + 0x60;
        glyph = itfCreateConvertedTextGlyph(x + 0x14C0, textY, depth, color, text, NULL);
        textWidth = frFontMeasureLines(glyph);
        frDestroyFontRequestAfter(glyph);
        /* The native name origin uses signed half-width truncation. */
        textOffset = -(textWidth / 2);
        textOffset += 0x15D;
        textOffset <<= 4;
        glyph = itfCreateConvertedTextGlyph(x + textOffset,
                                            textY, depth, color, text, NULL);
        frFontDrawGlyphChain(glyph, 1, context);
        frDestroyFontRequestAfter(glyph);
    } else {
        cap = 0;
    }
    progressAmount = work->profileAnimation[index].currentProgress;
    phase = work->profileAnimation[index].state;
    /* Native signed rejection edges at 00269030/0026903C share the fallback.
     * The matched DDS2 renderer uses the same ordered signed guards.
     * Both checks precede the current-progress drawing path. */
    if (phase >= 0) {
        if (phase < 3) {
            if (cap != 0) {
                if (progressAmount < cap) {
                    /* The denominator is captured on the positive-progress guard edge:
                     * DDS2 002A0C00/04 and DDS1 00269054/58. CSE forwards the quotient
                     * divisor but retains this snapshot for its divide-zero check. */
                    s32 normalizationBase = cap;
                    if (progressAmount > 0) {
                        progress = progressAmount * 1000 / normalizationBase;
                        originalProgress = progress;
                        if (work->profileAnimation[index].progressInitialized == 0) {
                            work->profileAnimation[index].previousProgress = progress;
                            work->profileAnimation[index].progressInitialized = 1;
                        }
                        if (progress <= 40) {
                            progress = 40;
                        }
                        if (work->profileAnimation[index].remaining == 0 &&
                            originalProgress >= work->profileAnimation[index].previousProgress &&
                            originalProgress - work->profileAnimation[index].previousProgress < 41) {
                            difference = originalProgress - work->profileAnimation[index].previousProgress;
                            if (difference < 10) {
                                progress += 9;
                            } else {
                                progress += difference;
                            }
                        }
                        progress = progress <= 0 ? 0 : (progress > 1000 ? 1000 : progress);
                        resources = work->teardownResource;
                        bar = &resources->workEntries[fillSlot];
                        description = &resources->descriptions[fillSlot];
                        bar->geometry.bounds[2] = (bar->sourceWidth << 4) * progress / 1000;
                        range = description->colors[2] - description->colors[0];
                        range = range * progress / 1000 - range;
                        bar->parameters[2] = range;
                        effDrawSpriteWithCornerColors(x + 0x1290, y + 0x120, depth,
                                                     palette.values, 0, resources, fillSlot, context);
                        /* Draw may replace the resource; restore the current fill entry. */
                        restoreResources = work->teardownResource;
                        restoreResources->workEntries[fillSlot].parameters[2] = 0;
                        restoreResources->workEntries[fillSlot].geometry.bounds[2] =
                            restoreResources->workEntries[fillSlot].sourceWidth << 4;
                    }
                } else {
                    if (row->unit->hp == 0 || mnuIsTitleEntryAvailable(row->unit) || (row->unit->status & 0x4040)) {
                        barColor = work->profileAnimation[index].alpha | 0x60606000;
                        palette.values[0] = barColor;
                        palette.values[1] = barColor;
                        palette.values[2] = barColor;
                        palette.values[3] = barColor;
                    }
                    effDrawSpriteWithCornerColors(x + 0x13A0, y + 0xF8, depth, palette.values,
                                                 0, work->teardownResource, 10, context);
                }
            }
            return;
        }
    }
    frameX = x + 0x13A0;
    frameY = y + 0xF8;
    effDrawSpriteWithCornerColors(frameX, frameY, depth, palette.values,
                                 0, work->teardownResource, 11, context);
    effDrawSpriteWithCornerColors(frameX, frameY, depth, palette.values,
                                 0, work->teardownResource, 12, context);
}


/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = dds3SearchProcessName(D_003AFBA0);
    BrsResultWork *resultWork;
    s32 counterIndex;

    if (resultTask == NULL) {
        return 0;
    }
    resultWork = (BrsResultWork *)dds3GetProcessData(resultTask);
    for (counterIndex = 0; counterIndex < BRS_RESULT_COUNTER_PAIR_COUNT; counterIndex++) {
        if (resultWork->levelCounters[counterIndex].phase >= 2) {
            if (resultWork->levelCounters[counterIndex].remaining != 0 ||
                resultWork->levelCounters[counterIndex].progressIconState[0] != 0 ||
                resultWork->levelCounters[counterIndex].completionIconState[0] != 0) {
                return 0;
            }
        } else if (resultWork->levelCounters[counterIndex].remaining != 0) {
            return 0;
        }
        if (resultWork->profileCounters[counterIndex].phase >= 2) {
            if (resultWork->profileCounters[counterIndex].phase == 2) {
                if (resultWork->profileCounters[counterIndex].remaining != 0) {
                    return 0;
                }
            }
        }
    }
    resultWork->settledFrames++;
    resultWork->settledFrames = resultWork->settledFrames <= 0 ? 0 :
        resultWork->settledFrames >= BRS_RESULT_SETTLED_POLL_CLAMP ? BRS_RESULT_SETTLED_POLL_LIMIT : resultWork->settledFrames;
    if (resultWork->settledFrames < BRS_RESULT_SETTLED_POLL_LIMIT) {
        return 0;
    }
    return 1;
}

void mnuSetTitleSequenceVolumePan(u32 sequenceId) {
    sndSetSequenceVolumePan(sequenceId, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC590;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
}

extern s8 D_003BC58C;

extern char D_00377318[];

extern char D_00377338[];

extern s32 D_00370D10[];

extern s32 D_003BC5A8;

extern s16 D_003BD8B0;

extern s32 D_003BC5AC;

extern void sndInitializeIopSoundSystem(char *, s32, char *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void mnuInitializeTitleAudioStreams(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_003BC58C != 0) {
        sndQueueNoPayloadCommand40();
        return;
    }
    D_003BC58C = 1;
    sndInitializeIopSoundSystem(D_00377318, 4, D_00377338, 4);
    sndEnsureMidiBankResident(D_00370D10[0]);
    D_003BC5A8 = 4;
    D_003BD8B0 = -1;
    D_003BC5AC = 0;
    mnuInitializeTitleAudioStreams();
    mnuCreateTitleEffectTask();
}

u32 func_002694F8(void) {
    return 0x599;
}

s32 mnuIncrementTitleEffectFrameCounter(KwlnTask *task) {
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)dds3GetProcessData(task);
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(KwlnTask *task) {
    sdfChipFreeDirect((void *)dds3GetProcessData(task));
    mnuTitleSoundTask = 0;
}

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfChipAllocDirect(MNU_TITLE_EFFECT_BYTES);
    KwlnTask *effectTask = dds3StartProcess(D_003BC5A0, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = effectTask;
    dds3SetProcessData(effectTask, (u32)effectState);
    effectState->soundNameIndex = 0;
    effectState->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */

INCLUDE_RODATA(const s32, "game/code_002665E0", D_003AFC80);

void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask);

    if (sdfSoundIsCommandBusy() != 0) {
        sdfDebugPrintf("now playeng start...\n");
        sdfSoundStopNamedPlayback();
    }
    sprintf(voicePath, "%s%04d.ADB", D_003771D8 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
    sdfDebugPrintf("--------------- VOICE -> %s\n", voicePath);
    sdfSoundSendNamedCommand(voicePath, 0x64);
    effectState->frameCounter = 0;
}

void mnuStopTitleVoicePlayback(void) {
    sdfSoundStopNamedPlayback();
}

s32 mnuQueryTitleSoundBusy(void) {
    return sdfSoundIsCommandBusy();
}

void mnuNoOpTitleAudio(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->frameCounter;
}

u32 sndOpStartTrackFromScript(void) {
    s32 sequence;

    sequence = scrReadIntParameter(0);
    sndStartTrackDefault(sequence);
    return 1;
}

u32 sndOpSetTrackDefaultVolumePan(void) {
    s32 sequence;

    sequence = scrReadIntParameter(0);
    sndSetSequenceVolumePan(sequence, SND_TRACK_DEFAULT_VOLUME, SND_TRACK_DEFAULT_PAN);
    return 1;
}

u32 sndOpSendTitleAudioCommand(void) {
    sndQueueNoPayloadCommand40();
    return 1;
}

s32 mnuInitializeTitleEffects(void) {
    s32 commandName;
    commandName = scrReadStringParameter(0);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(commandName, 0x7f);
    return 1;
}

u32 sndOpStopNamedPlayback(void) {
    sdfSoundStopNamedPlayback();
    return 1;
}


u32 sndOpPushCommandBusyState(void) {
    u32 soundBusy;

    soundBusy = sdfSoundIsCommandBusy();
    scrSetIntegerReturnValue(soundBusy);
    return 1;
}

u32 sndOpAlwaysSucceeds(void) {
    return 1;
}

s32 movCheckStartupSoundState(void) {
    if (mnuPollTitleStreamStateLocked() == 0) {
        mnuQueueTitleSoundStreamEntry(scrReadIntParameter(0));
        return 0;
    }
    return mnuPollTitleStreamStateLocked() != 1;
}

u32 sndOpUpdateTitleBgmState(void) {
    mnuTitleStreamUpdateAndLogBgm();
    return 1;
}

u32 mnuResetTitlePlaybackCommands(void) {
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    return 1;
}

u32 mnuAdvanceTitleStreamFromScript(void) {
    mnuAdvanceTitleStateUnderSemaphore();
    return 1;
}

u8 mnuIsTitleStreamIdle(void) {
    s32 loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount) {
    struct {
        u32 source;
        u32 destination;
        u32 size;
        u32 attributes;
    } transfer;
    s32 dmaRequest;
    transfer.source = (u32)source;
    transfer.destination = iopAddress;
    transfer.size = wordCount * SND_TRANSFER_WORD_BYTES;
    transfer.attributes = 0;
    FlushCache(0);
    dmaRequest = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(dmaRequest) >= 0) {
    }
    return dmaRequest;
}

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount) {
    s32 bufferBytes = wordCount * SND_TRANSFER_WORD_BYTES;
    s32 halfwordIndex;

    D_003BC5B8 = sdfMemoryGetBlockAddress(sdfMemAllocDirect(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_003BC5B8)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    sndStreamTransferIopHeapAddress = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (sndStreamTransferIopHeapAddress <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = sndStreamTransferIopHeapAddress;
    D_003BC5B0[1] = sndStreamTransferIopHeapAddress + bufferBytes;
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[1], wordCount);
}

extern s32 sndDispatchIopAudioCommand(s32, ...);

/* Send the per-core parameter commands for both sound cores, then the IOP
 * stream buffer with the transfer size. */
void mnuConfigureTitleAudioStream(s32 transferSize) {
    volatile s32 core;

    for (core = 0; core < 2; core++) {
        sndDispatchIopAudioCommand(1, 0x8010, core | 0x980, 0x3FFF);
        sndDispatchIopAudioCommand(1, 0x8010, core | 0xA80, 0x3FFF);
        sndDispatchIopAudioCommand(1, 0x8010, core | 0x800, sndDispatchIopAudioCommand(1, 0x8020, core | 0x800) & ~0x30);
    }
    sndDispatchIopAudioCommand(1, 0x80E0, 0, 0x10, sndStreamTransferIopHeapAddress, transferSize << 3);
    sndDispatchIopAudioCommand(1, 0x8010, 0xF80, 0x7FFF);
    sndDispatchIopAudioCommand(1, 0x8010, 0x1080, 0x7FFF);
}

extern int sceSdRemoteInit(void);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

extern u64 D_003BD8B8;
extern s32 sndDispatchIopAudioCommand(s32, ...);

s32 mnuCopySoundStreamOnStatusChange(const void *source, u32 words) {
    u64 status = sndDispatchIopAudioCommand(1, 0x8100, 0);

    if ((D_003BD8B8 & 0x1000000) == (status & 0x1000000)) {
        return 0;
    }
    sndCopyWordsToIopSynchronously(source, D_003BC5B0[D_003BC5BC & 1], words);
    D_003BD8B8 = status;
    D_003BC5BC ^= 1;
    return 1;
}

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC570);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC578);

INCLUDE_SDATA(const s32, "game/code_002665E0", mnuTitleSoundTask);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC58C);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC590);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC598);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5A0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5A8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5AC);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5B0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5B8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5BC);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C0);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C4);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5C8);

INCLUDE_SDATA(const s32, "game/code_002665E0", D_003BC5CC);

