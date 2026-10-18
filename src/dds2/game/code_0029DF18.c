#include "snd_command_packet.h"
#include "common.h"
#include "scr_integer_return_value_api.h"
#include "scr_string_parameter_api.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "mnu_sound_resource_path_api.h"
#include "mnu_voice_resource_path_api.h"
#include "ui_color.h"
#include "mnu_result.h"
#include "brs_result_counter.h"
#include "sdf.h"
#include "snd_track_playback_api.h"
/* Declared before first use (were implicit int). */
extern void FlushCache(s32);
extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern void mnuResetTitleStreamAfterFileIdle(void);
extern s32 sceSifDmaStat(s32);
extern s32 sceSifSetDma(void *, s32);
#include "sdf_debug_api.h"
extern s32 sdfSoundSendNamedCommand(const char *name, u8 channel);
extern void sdfSoundStopNamedPlayback(void);
/* Original-style implicit int, kept: matched code depends on it: effDrawTextureSlotWithBlendedCornerColors. */

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;



struct EffectSlotSet;
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void *memcpy(void *, const void *, u32);
extern u32 D_004285E0[4];



/* Fade a result row's background in; the row is ready once it is fully opaque. */
void brsUpdateResultBackgroundFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 rowIndex) {
    s32 fade = 0x100 - work->fadeProgress;

    if (work->fadeAnimation[rowIndex].backgroundState == 0) {
        work->fadeAnimation[rowIndex].backgroundOpacity = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->fadeAnimation[rowIndex].backgroundOpacity >= 0x80) {
            work->fadeAnimation[rowIndex].backgroundState = 1;
        }
    }
}

void brsDrawProgressRowOverlays(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 surfaceIndex, s32 rowIndex);

void brsDrawProgressRowEntries(s32 x, s32 y, s32 z, BrsSkillPackageWork *work, BrsProgressRow *row,
                   BrsActiveProgressList *list, s32 context, s32 progressRowIndex) {
    u32 color[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    const s32 enemyPos[3] = { 0x19, 0x10, 2 };
    s32 allyPos[3] = { 0x38, 5, 3 };
    s32 i;
    DatPartyRecord *unit;
    u32 packed;
    const s32 *enemy;
    s32 *ally;

    packed = work->fadeAnimation[progressRowIndex].backgroundOpacity | 0x80808000;
    unit = row->unit;
    color[0] = packed;
    color[1] = packed;
    color[2] = packed;
    color[3] = packed;
    if (work->teardownResource == NULL) {
        return;
    }
    ally = allyPos;
    enemy = enemyPos;
    for (i = 0; i < 1; i++, ally += 3, enemy += 3) {
        s32 px;
        s32 py;
        s32 slot;

        if (unit->flags & 2) {
            effDrawTextureSlotWithBlendedCornerColors(x - 0x1D0, y + 0x60, z,
                          (work->fadeAnimation[progressRowIndex].backgroundOpacity << 8) >> 7, 0,
                          work->teardownResource, 0x1F, context);
            brsDrawProgressRowOverlays(x, y, z, work, &list->rows[progressRowIndex], context, progressRowIndex);
            px = enemy[0];
            py = enemy[1];
            slot = enemy[2];
        } else {
            brsDrawProgressRowOverlays(x, y, z, work, &list->rows[progressRowIndex], context, progressRowIndex);
            px = ally[0];
            py = ally[1];
            slot = ally[2];
        }
        effDrawSpriteWithCornerColors(x + (px << 4), y + (py << 3), z, color, 0,
                      work->teardownResource, slot, context);
    }
}

/* Shared gray palette retained for the still-ASM unit-info renderer. */
INCLUDE_ASM(const s32, "game/code_0029DF18", brsDrawProgressRowUnitInfo);

/* Fade a result row's portrait in; the row is ready once the opacity reaches 0x60. */
void brsUpdateResultPortraitFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 context, s32 rowIndex) {
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

extern SdfPoolNode kwlnDrawSurfaces[];
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void btlDrawIconAtSize(SdfPoolNode *, s32, s32, s32, s32, s32, s32, s32, s32, u16);

/* Draw the row's unit icon, then its unavailable overlay when required.
 * Direct indexed animation members preserve separate position/opacity epochs.
 * Resolve the coordinates before dimensions: sched2's equal-priority frontline
 * Y setup must precede the width spill; the fallback spill has higher priority. */
INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428590);

void brsDrawProgressRowOverlays(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 surfaceIndex, s32 rowIndex) {
    DatPartyRecord *unit = progressRow->unit;
    DatPartyRecord *tintUnit;
    u32 color[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 iconWidth;
    s32 iconHeight;
    s32 iconX;
    s32 iconY;
    u32 packed;

    if (unit->flags & DAT_PARTY_FLAG_FRONTLINE) {
        iconX = 0x11;
        iconY = 4;
        iconWidth = 0x37;
        iconHeight = 0x37;
    } else {
        iconX = 0x2A;
        iconY = 5;
        iconWidth = 0x2F;
        iconHeight = 0x2F;
    }

    iconX += work->fadeAnimation[rowIndex].portraitPosition[0];
    iconY += work->fadeAnimation[rowIndex].portraitPosition[1];
    evtSetDrawSurfaceIndex(surfaceIndex);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);

    tintUnit = progressRow->unit;
    if (tintUnit->hp != 0 && !(tintUnit->status & 0x4000)) {
        packed = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x00808080;
        color[0] = packed;
        color[1] = packed;
        color[2] = packed;
        color[3] = packed;
    } else {
        color[0] = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x00282222;
        packed = (work->fadeAnimation[rowIndex].portraitOpacity << 24) | 0x002C242A;
        color[1] = packed;
        color[2] = packed;
        color[3] = packed;
    }

    btlDrawIconAtSize(&kwlnDrawSurfaces[surfaceIndex], iconX + (x >> 4),
                      iconY + (y >> 3), iconWidth, iconHeight, color[0],
                      color[1], color[2], color[3], unit->unitId);

    if (unit->hp == 0 || (unit->status & 0x4000)) {
        if (unit->flags & DAT_PARTY_FLAG_FRONTLINE) {
            iconX = 0xB;
            iconY = 0x1E;
        } else {
            iconX = 0x24;
            iconY = 0x16;
        }

        packed = (work->fadeAnimation[rowIndex].portraitOpacity + 0x20) | 0x80808000;
        color[0] = packed;
        color[1] = packed;
        color[2] = packed;
        color[3] = packed;

        if (work->teardownResource != NULL) {
            effDrawSpriteWithCornerColors(x + (iconX << 4), y + (iconY << 3), z,
                          color, 0, work->teardownResource, 0x1C, surfaceIndex);
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
    switch (work->levelAnimation[rowIndex].drawPhase) {
    case 0:
        work->levelAnimation[rowIndex].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->levelAnimation[rowIndex].alpha >= 0x80) {
            work->levelAnimation[rowIndex].drawPhase = 1;
        }
        break;
    case 1:
        work->levelAnimation[rowIndex].drawPhase++;
        if (work->levelAnimation[rowIndex].applied > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (level < 99 && progressRow->unit->hp != 0 && (progressRow->unit->status & 0x4000) == 0) {
            if (work->levelAnimation[rowIndex].drawPhase == 3) {
                goto flash;
            }
            work->levelAnimation[rowIndex].appliedStep = brsCalcProgressAnimationStep(work, progressRow, rowIndex, 0);
            currentExp = work->levelAnimation[rowIndex].remaining + work->levelAnimation[rowIndex].appliedStep;
            work->levelAnimation[rowIndex].remaining = currentExp;
            if (currentExp >= (s32)ptyComputeTotalExp(progressRow->unit, nextLevel - progressRow->unit->level) && work->levelAnimation[rowIndex].drawPhase == 2) {
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
                work->levelAnimation[rowIndex].progressIconEnabled = 1;
                work->levelAnimation[rowIndex].iconState = 1;
                work->levelAnimation[rowIndex].completionFlashActive = 1;
                work->levelAnimation[rowIndex].flashOpacity = 0x80;
                work->levelAnimation[rowIndex].flashFrame = 0;
                work->levelAnimation[rowIndex].alpha = 0;
                work->levelAnimation[rowIndex].drawPhase++;
                mnuSetTitleSequenceVolumePan(0x12);
            }
        } else {
            work->levelAnimation[rowIndex].applied = 0;
        }
        if (work->levelAnimation[rowIndex].drawPhase == 3) {
flash:
            work->levelAnimation[rowIndex].flashFrame++;
            work->levelAnimation[rowIndex].flashFrame = work->levelAnimation[rowIndex].flashFrame <= 0 ? 0 : (work->levelAnimation[rowIndex].flashFrame > 15 ? 15 : work->levelAnimation[rowIndex].flashFrame);
            if (work->levelAnimation[rowIndex].flashFrame >= 15) {
                work->levelAnimation[rowIndex].drawPhase++;
            }
        } else if (work->levelAnimation[rowIndex].drawPhase == 4) {
            work->levelAnimation[rowIndex].flashOpacity -= 16;
            work->levelAnimation[rowIndex].flashOpacity = work->levelAnimation[rowIndex].flashOpacity <= 0 ? 0 : (work->levelAnimation[rowIndex].flashOpacity > 128 ? 128 : work->levelAnimation[rowIndex].flashOpacity);
            if (work->levelAnimation[rowIndex].flashOpacity <= 0) {
                work->levelAnimation[rowIndex].completionFlashActive = 0;
            }
            work->levelAnimation[rowIndex].alpha += 8;
            work->levelAnimation[rowIndex].alpha = work->levelAnimation[rowIndex].alpha <= 0 ? 0 : (work->levelAnimation[rowIndex].alpha > 128 ? 128 : work->levelAnimation[rowIndex].alpha);
            if (work->levelAnimation[rowIndex].alpha >= 128) {
                work->levelAnimation[rowIndex].drawPhase = 2;
                if (work->levelAnimation[rowIndex].applied > 0 && work->resultPhase == 0) {
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
typedef struct BrsProgressBarCoordinates {
    s16 fillX;
    s16 fillY;
    u8 fillSlot;
    u8 pad05;
    s16 tailX;
    s16 tailY;
    u8 tailSlot;
    u8 pad0B;
} BrsProgressBarCoordinates;

extern s32 sprintf(char *dst, const char *fmt, ...);
extern const char D_004379F0[];
extern struct FrFontGlyph *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct FrFontGlyph *);

/* Draw the EXP remaining and temporarily crop the progress sprites for this
 * row. Source lineage: Harbor/Luna's 30-word predecessor and the matched
 * profile renderer's draw-fresh resource/tail-origin ordering.
 * Capture remaining progress after palette setup; stock EE-GCC sched2
 * otherwise keeps its load ahead of the three leading palette stores.
 * Refresh mutable resource fields after drawing before restoring them. */
void brsDrawLevelProgressDetails(s32 x, s32 y, s32 depth, BrsSkillPackageWork *work,
                               BrsProgressRow *row, s32 context, s32 index) {
    char text[32];
    typedef struct { u32 values[4]; } BrsProgressBarPalette;
    extern const BrsProgressBarPalette D_004285B0;
    BrsProgressBarPalette palette = D_004285B0;
    extern const BrsProgressBarCoordinates D_004285C0;
    BrsProgressBarCoordinates coordinates = D_004285C0;
    s32 fade = 0x100 - work->fadeProgress;
    s32 fillX = coordinates.fillX;
    s32 fillY = coordinates.fillY;
    s32 fillSlot = coordinates.fillSlot;
    s32 tailX = coordinates.tailX;
    s32 tailY = coordinates.tailY;
    s32 tailSlot = coordinates.tailSlot;
    s32 level = work->levelAnimation[index].level;
    s16 nextLevel = work->levelAnimation[index].level + 1;
    s32 remaining;
    s32 levelDelta;
    s32 currentTotal;
    s32 startProgress;
    s32 endProgress;
    s32 progress;
    s32 originalProgress;
    s32 range;
    s32 tailOrigin;
    u32 color = work->levelAnimation[index].alpha | 0x80808000;
    struct FrFontGlyph *glyph;
    EffectSlotSet *resources;

    palette.values[0] = color;
    palette.values[1] = color;
    palette.values[2] = color;
    palette.values[3] = color;
    remaining = work->levelAnimation[index].remaining;
    nextLevel = nextLevel < 2 ? 1 : (nextLevel > 99 ? 99 : nextLevel);
    if (level >= 99) {
        return;
    }

    levelDelta = level - row->unit->level;
    currentTotal = (s32)ptyComputeTotalExp(row->unit, levelDelta);
    if (remaining < currentTotal) {
        startProgress = remaining;
        endProgress = (s32)ptyComputeTotalExp(row->unit, nextLevel - level + levelDelta);
    } else {
        startProgress = remaining - currentTotal;
        endProgress = (s32)ptyComputeTotalExp(row->unit, nextLevel - level + levelDelta) - currentTotal;
    }
    if (work->levelAnimation[index].completionFlashActive != 0 &&
        work->levelAnimation[index].drawPhase == 3) {
        endProgress = 0;
        startProgress = 0;
    }
    if (row->unit->hp != 0 && !(row->unit->status & 0x4000)) {
        color = uiBlendColors(0x80808080, 0x80808000, fade);
    } else {
        color = uiBlendColors(0x4F4E4E80, 0x4F4E4E00, fade);
    }
    sprintf(text, D_004379F0, endProgress - startProgress);
    glyph = itfCreateFont2TextGlyph(x + 0xC00, y + 0xE0, depth, color, text, 0);
    frFontDrawGlyphChain(glyph, 1, context);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);

    if (startProgress > 0 && endProgress != 0) {
        progress = startProgress * 1000 / endProgress;
        if (progress >= 1001 && endProgress < startProgress) {
            progress %= 1000;
        }
        originalProgress = progress;
        if (work->levelAnimation[index].progressInitialized == 0) {
            work->levelAnimation[index].previousProgress = progress;
            work->levelAnimation[index].progressInitialized = 1;
        }
        if (progress <= 40) {
            progress = 40;
        }
        if (work->levelAnimation[index].applied == 0 &&
            work->levelAnimation[index].previousProgress < originalProgress &&
            originalProgress - work->levelAnimation[index].previousProgress < 11) {
            progress += 10;
        }
        progress = progress <= 0 ? 0 : (progress > 1000 ? 1000 : progress);
        resources = work->teardownResource;
        resources->workEntries[fillSlot].geometry.bounds[2] =
            (resources->workEntries[fillSlot].sourceWidth << 4) * progress / 1000;
        range = resources->descriptions[fillSlot].colors[2] -
            resources->descriptions[fillSlot].colors[0];
        range = range * progress / 1000 - range;
        resources->workEntries[fillSlot].parameters[2] = range;
        effDrawSpriteWithCornerColors(x + (fillX << 4), y + (fillY << 3),
                                     depth, palette.values, 0, resources, fillSlot, context);
        tailOrigin = x + (tailX << 4);
        resources = work->teardownResource;
        effDrawSpriteWithCornerColors(tailOrigin +
                                     resources->workEntries[fillSlot].geometry.bounds[2],
                                     y + (tailY << 3), depth, palette.values, 0,
                                     resources, tailSlot, context);
        {
            EffectSlotSet *restorationResource = work->teardownResource;

            restorationResource->workEntries[fillSlot].parameters[2] = 0;
            restorationResource->workEntries[fillSlot].geometry.bounds[2] =
                restorationResource->workEntries[fillSlot].sourceWidth << 4;
        }
    }
    if (work->levelAnimation[index].completionFlashActive != 0) {
        color = work->levelAnimation[index].flashOpacity | 0x80808000;
        palette.values[0] = color;
        palette.values[1] = color;
        palette.values[2] = color;
        palette.values[3] = color;
        effDrawSpriteWithCornerColors(x + (fillX << 4), y + (fillY << 3),
                                     depth, palette.values, 0, work->teardownResource,
                                     fillSlot, context);
    }
}




void brsUpdateLevelProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 progressRowIndex) {
    s32 y;
    s32 bob;

    switch (work->levelAnimation[progressRowIndex].progressIconEnabled) {
    case 0:
        work->levelAnimation[progressRowIndex].progressIconOpacity = 0;
        work->levelAnimation[progressRowIndex].progressIconPosition[0] = 0xA6;
        work->levelAnimation[progressRowIndex].progressIconPosition[1] = 0x22;
        work->levelAnimation[progressRowIndex].progressIconAngle = 0xB4;
        break;
    case 1:
        work->levelAnimation[progressRowIndex].progressIconAngle = (work->levelAnimation[progressRowIndex].progressIconAngle + 15) % 360;
        work->levelAnimation[progressRowIndex].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[progressRowIndex].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->levelAnimation[progressRowIndex].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->levelAnimation[progressRowIndex].progressIconPosition[1] = 0x24 - bob;
        work->levelAnimation[progressRowIndex].progressIconOpacity = (work->levelAnimation[progressRowIndex].progressIconOpacity > 0) ? ((work->levelAnimation[progressRowIndex].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].progressIconOpacity) : 0;
        y = work->levelAnimation[progressRowIndex].progressIconPosition[1];
        work->levelAnimation[progressRowIndex].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->levelAnimation[progressRowIndex].progressIconOpacity >= 0x80) {
            work->levelAnimation[progressRowIndex].progressIconEnabled += 2;
            work->levelAnimation[progressRowIndex].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->levelAnimation[progressRowIndex].progressIconEnabled++;
        work->levelAnimation[progressRowIndex].progressIconAngle = 0x78;
        break;
    default:
        work->levelAnimation[progressRowIndex].progressIconOpacity -= 10;
        work->levelAnimation[progressRowIndex].progressIconOpacity = (work->levelAnimation[progressRowIndex].progressIconOpacity > 0) ? ((work->levelAnimation[progressRowIndex].progressIconOpacity > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].progressIconOpacity) : 0;
        y = work->levelAnimation[progressRowIndex].progressIconPosition[1] - 1;
        work->levelAnimation[progressRowIndex].progressIconPosition[1] = y;
        work->levelAnimation[progressRowIndex].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->levelAnimation[progressRowIndex].progressIconOpacity <= 0) {
            work->levelAnimation[progressRowIndex].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->levelAnimation[progressRowIndex].iconState) {
    case 0:
        work->levelAnimation[progressRowIndex].auxiliaryColor = 0;
        work->levelAnimation[progressRowIndex].unk58[0] = 0x95;
        work->levelAnimation[progressRowIndex].unk58[1] = 0x15;
        break;
    case 1:
        work->levelAnimation[progressRowIndex].auxiliaryColor += 10;
        work->levelAnimation[progressRowIndex].auxiliaryColor = (work->levelAnimation[progressRowIndex].auxiliaryColor > 0) ? ((work->levelAnimation[progressRowIndex].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].auxiliaryColor) : 0;
        if (work->levelAnimation[progressRowIndex].auxiliaryColor >= 0x80) {
            work->levelAnimation[progressRowIndex].iconState++;
            work->levelAnimation[progressRowIndex].iconOpacity = 0;
        }
        break;
    case 2:
        work->levelAnimation[progressRowIndex].iconOpacity++;
        work->levelAnimation[progressRowIndex].iconOpacity = (work->levelAnimation[progressRowIndex].iconOpacity > 0) ? ((work->levelAnimation[progressRowIndex].iconOpacity > 8) ? 8 : work->levelAnimation[progressRowIndex].iconOpacity) : 0;
        if (work->levelAnimation[progressRowIndex].iconOpacity >= 8) {
            work->levelAnimation[progressRowIndex].iconState++;
        }
        break;
    default:
        work->levelAnimation[progressRowIndex].auxiliaryColor -= 0x10;
        work->levelAnimation[progressRowIndex].auxiliaryColor = (work->levelAnimation[progressRowIndex].auxiliaryColor > 0) ? ((work->levelAnimation[progressRowIndex].auxiliaryColor > 0x80) ? 0x80 : work->levelAnimation[progressRowIndex].auxiliaryColor) : 0;
        if (work->levelAnimation[progressRowIndex].auxiliaryColor <= 0) {
            work->levelAnimation[progressRowIndex].iconState = 0;
        }
        break;
    }
}

extern u32 D_004285D0[4];

void brsDrawLevelProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *row, s32 depth, s32 rowIndex) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285D0, sizeof(color));
    if (work->teardownResource != NULL) {
        if (work->levelAnimation[rowIndex].progressIconEnabled != 0) {
            packed = work->levelAnimation[rowIndex].progressIconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            effDrawSpriteWithCornerColors(x + (work->levelAnimation[rowIndex].progressIconPosition[0] << 4),
                         y + (work->levelAnimation[rowIndex].progressIconPosition[1] << 3) + 0x40,
                         z, color, 0, work->teardownResource, 0x12, depth);
        }
        if (work->levelAnimation[rowIndex].iconState != 0) {
            packed = work->levelAnimation[rowIndex].auxiliaryColor | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
        }
    }
}

void brsUpdateProfileProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 rowIndex) {
    s32 y;
    s32 bob;

    switch (work->profileAnimation[rowIndex].progressIconEnabled) {
    case 0:
        work->profileAnimation[rowIndex].progressIconOpacity = 0;
        work->profileAnimation[rowIndex].progressIconPosition[0] = 0x12F;
        work->profileAnimation[rowIndex].progressIconPosition[1] = 0x22;
        work->profileAnimation[rowIndex].progressIconAngle = 0xB4;
        break;
    case 1:
        work->profileAnimation[rowIndex].progressIconAngle = (work->profileAnimation[rowIndex].progressIconAngle + 15) % 360;
        work->profileAnimation[rowIndex].progressIconOpacity =
            (u32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[rowIndex].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 144.0f);
        bob = (s32)(0.0f + (sdfSinPoly((f32)((work->profileAnimation[rowIndex].progressIconAngle + 90) % 360) / 180.0f * 3.1415899f) + 1.0f) * 0.5f * 22.0f);
        work->profileAnimation[rowIndex].progressIconPosition[1] = 0x24 - bob;
        work->profileAnimation[rowIndex].progressIconOpacity = (work->profileAnimation[rowIndex].progressIconOpacity > 0) ? ((work->profileAnimation[rowIndex].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[rowIndex].progressIconOpacity) : 0;
        y = work->profileAnimation[rowIndex].progressIconPosition[1];
        work->profileAnimation[rowIndex].progressIconPosition[1] = (y < 0xF) ? 0xE : ((y > 0x24) ? 0x24 : y);
        if (work->profileAnimation[rowIndex].progressIconOpacity >= 0x80) {
            work->profileAnimation[rowIndex].progressIconEnabled += 2;
            work->profileAnimation[rowIndex].progressIconAngle = 0x78;
        }
        break;
    case 2:
        work->profileAnimation[rowIndex].progressIconEnabled++;
        work->profileAnimation[rowIndex].progressIconAngle = 0x78;
        break;
    default:
        work->profileAnimation[rowIndex].progressIconOpacity -= 10;
        work->profileAnimation[rowIndex].progressIconOpacity = (work->profileAnimation[rowIndex].progressIconOpacity > 0) ? ((work->profileAnimation[rowIndex].progressIconOpacity > 0x80) ? 0x80 : work->profileAnimation[rowIndex].progressIconOpacity) : 0;
        y = work->profileAnimation[rowIndex].progressIconPosition[1] - 1;
        work->profileAnimation[rowIndex].progressIconPosition[1] = y;
        work->profileAnimation[rowIndex].progressIconPosition[1] = (y < 5) ? 4 : ((y > 0xE) ? 0xE : y);
        if (work->profileAnimation[rowIndex].progressIconOpacity <= 0) {
            work->profileAnimation[rowIndex].progressIconEnabled = 0;
        }
        break;
    }
    switch (work->profileAnimation[rowIndex].iconState) {
    case 0:
        work->profileAnimation[rowIndex].auxiliaryColor = 0;
        work->profileAnimation[rowIndex].unk58[0] = 0x122;
        work->profileAnimation[rowIndex].unk58[1] = 0x15;
        break;
    case 1:
        work->profileAnimation[rowIndex].auxiliaryColor = 0x80;
        work->profileAnimation[rowIndex].iconState++;
        work->profileAnimation[rowIndex].iconOpacity = 0;
        break;
    case 2:
        work->profileAnimation[rowIndex].iconOpacity = 0xFF;
        work->profileAnimation[rowIndex].iconState++;
        break;
    default:
        work->profileAnimation[rowIndex].iconOpacity -= 8;
        work->profileAnimation[rowIndex].iconOpacity = (work->profileAnimation[rowIndex].iconOpacity > 0)
            ? ((work->profileAnimation[rowIndex].iconOpacity > 0xFF) ? 0xFF : work->profileAnimation[rowIndex].iconOpacity)
            : 0;
        break;
    }
}

void brsDrawProfileProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 depth, s32 rowIndex) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285E0, sizeof(color));

    if (work->teardownResource != NULL) {
        if (work->profileAnimation[rowIndex].iconState >= 2) {
            packed = work->profileAnimation[rowIndex].iconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            effDrawSpriteWithCornerColors(x + 0x1180, y + 0x130, z, color, 0,
                         work->teardownResource, 0x16, depth);
            effDrawSpriteWithCornerColors(x + 0x17E0, y + 0x130, z, color, 0,
                         work->teardownResource, 0x17, depth);
        }
    }
}

extern u32 ptyComputeTotalExp(DatPartyRecord *unit, s32 levelDelta);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);

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
        if (work->levelAnimation[rowIndex].applied > 0) {
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
                step = work->levelAnimation[rowIndex].applied;
            }
            work->levelAnimation[rowIndex].applied -= step;
            if (work->levelAnimation[rowIndex].applied < 0) {
                step += work->levelAnimation[rowIndex].applied;
                work->levelAnimation[rowIndex].applied = 0;
            }
            work->levelAnimation[rowIndex].applied = work->levelAnimation[rowIndex].applied <= 0 ? 0 : (work->levelAnimation[rowIndex].applied > 0x1000000 ? 0x1000000 : work->levelAnimation[rowIndex].applied);
            result = step;
        }
    } else {
        range = ptyGetProfileRecordCap(ptyGetCurrentProfileId(progressRow->unit));
        if (work->profileAnimation[rowIndex].applied > 0) {
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
                step = work->profileAnimation[rowIndex].applied;
            }
            work->profileAnimation[rowIndex].applied -= step;
            if (work->profileAnimation[rowIndex].applied < 0) {
                step += work->profileAnimation[rowIndex].applied;
                work->profileAnimation[rowIndex].applied = 0;
            }
            work->profileAnimation[rowIndex].applied = work->profileAnimation[rowIndex].applied <= 0 ? 0 : (work->profileAnimation[rowIndex].applied > 0x1000000 ? 0x1000000 : work->profileAnimation[rowIndex].applied);
            result = step;
        }
    }
    return result;
}

#include "common.h"
#include "mnu_result.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "mnu_title_effect.h"
#include "mnu.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
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

extern u32 mnuTitleStreamSemaphore;

extern s32 scrReadIntParameter(s32);

extern u32 sdfSoundIsCommandBusy(void);


extern u32 D_00437A2C;

extern KwlnTask *mnuTitleSoundTask;

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamFileRequest;

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern char D_00428680[]; /* "titleProc" */

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_00437A00[];

extern char *strcat(char *, char *);

extern u8 D_00437A08[];

extern s32 WaitSema(u32);

extern s32 SignalSema(u32);

extern s32 sceSifInitIopHeap(void);
extern s32 sceSifAllocIopHeap(s32 size);
extern void Exit(s32 status);
extern s32 sndStreamTransferIopHeapAddress;

/* libc sprintf returns the signed vfprintf character count. */
extern s32 sprintf(char *dst, const char *fmt, ...);
extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern char D_00428550[]; /* "result2_draw" */

typedef struct BrsResultWork {
    u8 pad00[0xAEAC];
    s32 settledFrames;
    u8 padAEB0[8];
    s8 soundStarted;
    u8 padAEB9[0x193];
    BrsResultCounterView levelCounters[8];
    BrsResultCounterView profileCounters[8];
    u8 padB6CC[0x14];
    s32 fadeProgress;
    u8 padB6E4[0x20];
} BrsResultWork;

extern DatProfileRecord *ptyGetCurrentProfileRecord(DatPartyRecord *);
extern s32 ptyGetCurrentProfileId(DatPartyRecord *);
extern u32 ptyGetProfileRecordCap(u16);
extern s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *, BrsProgressRow *, s32, s8);
void mnuSetTitleSequenceVolumePan(u32 sequenceId);

void brsUpdateProfileProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *entry, s32 context, s32 rowIndex) {
    s32 fade = 0x100 - work->fadeProgress;
    s32 profileId;
    s32 cap;
    s32 progress;

    ptyGetCurrentProfileRecord(entry->unit);
    profileId = ptyGetCurrentProfileId(entry->unit);
    if (profileId != 0) {
        cap = ptyGetProfileRecordCap(profileId);
    } else {
        cap = 0;
        work->profileAnimation[rowIndex].applied = 0;
    }
    progress = work->profileAnimation[rowIndex].remaining;
    switch (work->profileAnimation[rowIndex].drawPhase) {
    case 0:
        work->profileAnimation[rowIndex].alpha = uiBlendColors(0x80808080, 0x80808000, fade) & 0xFF;
        if (work->profileAnimation[rowIndex].alpha >= 0x80) {
            work->profileAnimation[rowIndex].drawPhase = 1;
        }
        break;
    case 1:
        work->profileAnimation[rowIndex].drawPhase++;
        if (work->profileAnimation[rowIndex].applied > 0 && work->resultPhase == 0) {
            mnuSetTitleSequenceVolumePan(0x14);
            work->resultPhase = 1;
        }
        break;
    case 2:
    case 3:
    case 4:
        if (work->profileAnimation[rowIndex].drawPhase == 2) {
            if (cap != 0) {
                if (progress < cap) {
                    work->profileAnimation[rowIndex].appliedStep = brsCalcProgressAnimationStep(work, entry, rowIndex, 1);
                    work->profileAnimation[rowIndex].remaining += work->profileAnimation[rowIndex].appliedStep;
                    if (work->profileAnimation[rowIndex].remaining >= cap) {
                        work->profileAnimation[rowIndex].progressIconEnabled = 1;
                        work->profileAnimation[rowIndex].iconState = 1;
                        work->profileAnimation[rowIndex].completionFlashActive = 1;
                        work->profileAnimation[rowIndex].flashOpacity = 0x80;
                        work->profileAnimation[rowIndex].flashFrame = 0;
                        work->profileAnimation[rowIndex].alpha = 0;
                        work->profileAnimation[rowIndex].drawPhase++;
                        mnuSetTitleSequenceVolumePan(0x12);
                    }
                } else {
                    work->profileAnimation[rowIndex].applied = 0;
                }
            }
        } else {
            if (work->profileAnimation[rowIndex].alpha < 0x80) {
                work->profileAnimation[rowIndex].alpha += 16;
            }
            if (work->profileAnimation[rowIndex].alpha > 0x80) {
                work->profileAnimation[rowIndex].alpha = 0x80;
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

extern u32 scrGetIndexedRecordAddress(u32, s32 *);
extern s32 mnuIsTitleEntryAvailable(DatPartyRecord *);

typedef struct BrsProfilePalette { u32 values[4]; } BrsProfilePalette;
typedef struct BrsProfileBarCoordinates {
    s16 fillX, fillY;
    u8 fillSlot, pad05;
    s16 tailX, tailY;
    u8 tailSlot, pad0B;
} BrsProfileBarCoordinates;
extern const BrsProfilePalette D_004285F0;
extern const BrsProfileBarCoordinates D_00428600;

void brsDrawProfileProgressDetails(s32 x, s32 y, s32 depth, BrsSkillPackageWork *work,
                                  BrsProgressRow *row, s32 context, s32 index) {
    BrsProfilePalette palette = D_004285F0;
    BrsProfileBarCoordinates coordinates = D_00428600;
    s32 textAddress;
    s32 currentProfile;
    s32 cap;
    s32 remaining;
    s32 phase;
    s32 fillX, fillY, fillSlot, tailX, tailY, tailSlot;
    u16 profileId;
    s32 progress, originalProgress, difference, range;
    s32 textOffset;
    s32 tailPosition;
    u32 color = work->profileAnimation[index].alpha | 0x80808000;
    u32 barColor;
    struct FrFontGlyph *glyph;
    EffectSlotSet *resources;
    EffectSlotSet *restoreResources;

    palette.values[0] = color;
    palette.values[1] = color;
    palette.values[2] = color;
    palette.values[3] = color;
    ptyGetCurrentProfileRecord(row->unit);
    currentProfile = ptyGetCurrentProfileId(row->unit);
    if (currentProfile != 0) {
        profileId = currentProfile;
        cap = ptyGetProfileRecordCap(profileId);
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
        /* Name/fallback acquisition precedes its placement choice. */
        textOffset = scrGetIndexedRecordAddress(profileId, &textAddress) ? 0 : 18;
        glyph = itfCreateConvertedTextGlyph(x + ((textOffset + 0x149) << 4), y + 0xB8, depth,
                                            color, (const u8 *)textAddress, NULL);
        frFontDrawGlyphChain(glyph, 1, context);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
    } else {
        cap = 0;
    }
    remaining = work->profileAnimation[index].remaining;
    phase = work->profileAnimation[index].drawPhase;
    /* Native signed rejection edges at 002A0BC4/002A0BCC share the fallback.
     * EE GCC 2.96 pass 00 retains their order; a switch reverses it, while
     * an enclosing one-pass loop also aligns the progress join. */
    if (phase < 0) goto fallback;
    if (phase >= 3) goto fallback;
    fillX = coordinates.fillX;
    fillY = coordinates.fillY;
    fillSlot = coordinates.fillSlot;
    tailX = coordinates.tailX;
    tailY = coordinates.tailY;
    tailSlot = coordinates.tailSlot;
    if (cap != 0) {
        if (remaining < cap) {
            /* The denominator is captured on the positive-progress guard edge:
             * DDS2 002A0C00/04 and DDS1 00269054/58. CSE forwards the quotient
             * divisor but retains this snapshot for its divide-zero check. */
            s32 normalizationBase = cap;
            if (remaining > 0) {
                progress = remaining * 1000 / normalizationBase;
                originalProgress = progress;
                if (work->profileAnimation[index].progressInitialized == 0) {
                    work->profileAnimation[index].previousProgress = progress;
                    work->profileAnimation[index].progressInitialized = 1;
                }
                if (progress <= 40) {
                    progress = 40;
                }
                if (work->profileAnimation[index].applied == 0 &&
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
                resources->workEntries[fillSlot].geometry.bounds[2] =
                    (resources->workEntries[fillSlot].sourceWidth << 4) * progress / 1000;
                range = resources->descriptions[fillSlot].colors[2] -
                    resources->descriptions[fillSlot].colors[0];
                range = range * progress / 1000 - range;
                resources->workEntries[fillSlot].parameters[2] = range;
                effDrawSpriteWithCornerColors(x + (fillX << 4), y + (fillY << 3),
                                             depth, palette.values, 0, resources, fillSlot, context);
                /* Capture the tail origin, then reacquire the draw-owned entry. */
                tailPosition = x + (tailX << 4);
                resources = work->teardownResource;
                effDrawSpriteWithCornerColors(tailPosition +
                                             resources->workEntries[fillSlot].geometry.bounds[2], y + (tailY << 3), depth,
                                             palette.values, 0, resources, tailSlot, context);
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
            effDrawSpriteWithCornerColors(x + 0x13A0, y + 0x168, depth, palette.values,
                                         0, work->teardownResource, 26, context);
        }
    }
    return;
fallback:
    effDrawSpriteWithCornerColors(x + 0x1290, y + 0x160, depth, palette.values,
                                 0, work->teardownResource, 19, context);
}


/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
s32 brsPollResultCounterCompletion(void) {
    KwlnTask *resultTask = kwlnTaskGetTaskByName(D_00428550);
    BrsResultWork *resultWork;
    s32 counterIndex;

    if (resultTask == NULL) {
        return 0;
    }
    resultWork = (BrsResultWork *)kwlnTaskGetUserValue(resultTask);
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

void func_002A1020(void) {
}

void func_002A1028(void) {
}

void func_002A1030(void) {
}

void func_002A1038(void) {
}

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *mnuBuildSoundResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A00;
    return strcat(pathBuffer, filename);
}

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_00437A08;
    return strcat(pathBuffer, filename);
}

extern s8 D_004379FC;

extern char D_003E0B30[];

extern s32 D_003E0B50[];

extern s32 D_003D9D60[];

extern s32 D_00437A18;

extern s16 D_00438FCC;

extern s32 D_00437A1C;

extern void sndInitializeIopSoundSystem(char *, s32, s32 *, s32);

extern void sndEnsureMidiBankResident(s32);

extern void mnuInitializeTitleAudioStreams(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void) {
    if (D_004379FC != 0) {
        sndQueueNoPayloadCommand40();
        return;
    }
    D_004379FC = 1;
    sndInitializeIopSoundSystem(D_003E0B30, 4, D_003E0B50, 4);
    sndEnsureMidiBankResident(D_003D9D60[0]);
    D_00437A18 = 4;
    D_00438FCC = -1;
    D_00437A1C = 0;
    mnuInitializeTitleAudioStreams();
    mnuCreateTitleEffectTask();
}

u32 func_002A1118(void) {
    return 0x608;
}

s32 mnuIncrementTitleEffectFrameCounter(KwlnTask *task) {
    TitleEffectState *effectState;

    effectState = (TitleEffectState *)kwlnTaskGetUserValue(task);
    effectState->frameCounter = effectState->frameCounter + 1;
    return 0;
}

void mnuDestroyTitleEffectTask(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    mnuTitleSoundTask = 0;
}

extern u8 D_00437A10[];

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void) {
    TitleEffectState *effectState = (TitleEffectState *)sdfAllocSizeClassBlock(MNU_TITLE_EFFECT_BYTES);
    KwlnTask *effectTask = kwlnTaskCreate(D_00437A10, 0x5214, 1, 1,
                              mnuIncrementTitleEffectFrameCounter, mnuDestroyTitleEffectTask, 0);
    mnuTitleSoundTask = effectTask;
    kwlnTaskSetUserValue(effectTask, (u32)effectState);
    effectState->soundNameIndex = 0;
    effectState->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfSoundStopNamedPlayback();
    }
    sdfSoundSendNamedCommand(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

extern char D_00428610[];

extern char D_00428628[];

extern char D_003E09F0[];

/* The native char*-typed voice argument is passed to numeric %04d formatting;
 * retain that signature rather than treating it as a filename string. */
void mnuPlayTitleVoiceFile(char *voiceArgument) {
    char voicePath[MNU_TITLE_VOICE_PATH_BYTES];
    TitleEffectState *effectState = (TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask);
    if (sdfSoundIsCommandBusy() != 0) {
        sdfDebugPrintf(D_00428610);
        sdfSoundStopNamedPlayback();
    }
    sprintf(voicePath, D_00428628, D_003E09F0 + MNU_TITLE_VOICE_PREFIX_BYTES * effectState->soundNameIndex, voiceArgument);
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
    return ((TitleEffectState *)kwlnTaskGetUserValue(mnuTitleSoundTask))->frameCounter;
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

u32 sndOpResetTitleStreamAfterFileIdle(void) {
    mnuResetTitleStreamAfterFileIdle();
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
    s64 loadState;

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

    D_00437A28 = (u32)sdfMemoryGetBlockAddress(sdfAllocGeneralBlock(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_00437A28)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    sndStreamTransferIopHeapAddress = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (sndStreamTransferIopHeapAddress <= 0) {
        Exit(0);
    }
    D_00437A20[0] = sndStreamTransferIopHeapAddress;
    D_00437A20[1] = sndStreamTransferIopHeapAddress + bufferBytes;
    sndCopyWordsToIopSynchronously((const void *)D_00437A28, D_00437A20[0], wordCount);
    sndCopyWordsToIopSynchronously((const void *)D_00437A28, D_00437A20[1], wordCount);
}

extern s32 sndDispatchIopAudioCommand(s32, s32, ...);

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

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_00437A2C = 0;
}

extern s32 sndDispatchIopAudioCommand(s32, s32, ...);
extern u64 D_00438FD0;

s32 mnuCopySoundStreamOnStatusChange(const void *source, u32 words) {
    u64 status = sndDispatchIopAudioCommand(1, 0x8100, 0);

    if ((D_00438FD0 & 0x1000000) == (status & 0x1000000)) {
        return 0;
    }
    sndCopyWordsToIopSynchronously(source, D_00437A20[D_00437A2C & 1], words);
    D_00438FD0 = status;
    D_00437A2C ^= 1;
    return 1;
}

