#include "common.h"
#include "mnu_title_stream.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "sdf_trig.h"
#include "mnu.h"
#include "mnu_movie.h"
#include "sdf.h"
#include "mnu_movie_resource.h"
#include "itf.h"
#include "kwln_task_lifecycle.h"
#include "kwln_fade.h"

extern u8 D_0037B8BC[];

extern MovObj mnuMovieDrawContext;

extern SdfPoolNode D_003253C8;

extern char D_003B1140[]; /* "mnuStaffImageProc" */

extern char D_003B1168[]; /* "staffProc" */

extern MnuMovieResourceEntry mnuMovieResourceEntries[];

extern KwlnTask *mnuMovieDrawTask;

extern u16 mnuMovieTaskState;

extern MnuStaffMovieWork *mnuMovieWork;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern s32 D_003BC618;
extern void mnuLoadMovieRollSprite(void);
extern void mnuInitializeMovieRollEntry(void);
extern void sdfSetGridScaledDrawBounds(s32 column, s32 row, s32 width, s32 height, u32 drawColor);
extern void func_002ECCF8(void *, SdfPoolNode *);

/* Three signed image-relative control points per movie image bank. */
typedef struct StaffImageCorners {
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} StaffImageCorners;
typedef char StaffImageCornersSizeCheck[(sizeof(StaffImageCorners) == 12) ? 1 : -1];

extern StaffImageCorners mnuStaffRollImageFadeControlPoints[];
extern MnuStaffImageRecord mnuStaffRollImageRecords[];
extern s32 D_003BC614;
extern s32 mnuCheckMovieDecoderStatus(void);
extern s32 mnuGetMoviePlaybackMode(void);
extern void mnuDrawScaledMovieRollSprite(s32 x, s32 y, s32 z, s32 alpha,
                                         EffectSlotSet *set, f32 scaleX, f32 scaleY,
                                         s32 index, s32 option, s32 texture);

/* Attenuate active roll sprites around the current image's three control points.
 * The coordinate declarations preserve the native equal-priority allocation;
 * first-point X before Y gives the native integer-sum lifetimes. */
void mnuDrawAnimatedMovieRollEntry(MnuMovieRollEntry *entry, EffectSlotSet *set, s32 texture) {
    f32 ex;
    f32 ey;
    f32 nearRadius = 18350.078125f;
    f32 farRadius;
    f32 y0;
    f32 x0;
    f32 x1;
    f32 y1;
    f32 x2;
    f32 y2;
    f32 center;
    f32 dx;
    f32 dy;
    f32 dist;
    s32 alpha;

    x0 = mnuStaffRollImageRecords[D_003BC614].x + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].x0;
    y0 = mnuStaffRollImageRecords[D_003BC614].y + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].y0;
    x1 = mnuStaffRollImageRecords[D_003BC614].x + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].x1;
    y1 = mnuStaffRollImageRecords[D_003BC614].y + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].y1;
    farRadius = 20971.51953125f;
    x2 = mnuStaffRollImageRecords[D_003BC614].x + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].x2;
    y2 = mnuStaffRollImageRecords[D_003BC614].y + mnuStaffRollImageFadeControlPoints[mnuMovieWork->imageIndex].y2;
    if (entry->active != 0) {
        alpha = 21;
        ex = entry->x;
        ey = entry->y;
        center = 128.0f;
        sdfSinPoly((f32)entry->timer / (f32)entry->duration * 3.141592503f);
        if ((u32)(D_003BC618 - 1) < 3U && (mnuCheckMovieDecoderStatus() != 0 || mnuGetMoviePlaybackMode() != 0)) {
            dy = y1 - (ey + center);
            dx = x1 - (ex + center);
            dist = dx * dx;
            dist += dy * 1.5f * dy;
            if (dist < nearRadius) {
                alpha = alpha - (nearRadius - dist) / nearRadius * alpha * ((f32)(s32)mnuMovieWork->imageAlpha * 0.0078125f);
            }
            dy = y0 - (ey + center);
            dx = x0 - (ex + center);
            dist = dx * dx;
            dist += dy * 1.5f * dy;
            if (dist < nearRadius) {
                alpha = alpha - alpha * ((nearRadius - dist) / nearRadius) * ((f32)(s32)mnuMovieWork->imageAlpha * 0.0078125f);
            }
            dy = y2 - (ey + center);
            dx = x2 - (ex + center);
            dist = dx * dx;
            dist += dy * 1.5f * dy;
            if (dist < farRadius) {
                alpha = alpha - alpha * ((farRadius - dist) / farRadius) * ((f32)(s32)mnuMovieWork->imageAlpha * 0.0078125f);
            }
        }
        switch (entry->sprite) {
        case 0:
            mnuDrawScaledMovieRollSprite(ex, ey, 0x1F4, alpha, set, 8.0f, 8.0f, 0xD, 0x60, texture);
            return;
        case 1:
            mnuDrawScaledMovieRollSprite(ex, ey, 0x1F4, alpha, set, 8.0f, 8.0f, 0xE, 0x60, texture);
            return;
        case 2:
            mnuDrawScaledMovieRollSprite(ex, ey, 0x1F4, alpha, set, 8.0f, 8.0f, 0xF, 0x60, texture);
            break;
        }
    }
}


/* Draw an active roll entry with its sprite-specific slot index; alpha follows the staff movie's scroll fade. */
void mnuDrawMovieRollEntry(MnuMovieRollEntry *entry, EffectSlotSet *set, s32 texture) {
    s32 x;
    s32 y;
    s32 alpha;

    if (entry->active != 0) {
        x = entry->x;
        y = entry->y;
        alpha = (s32)mnuMovieWork->imageAlpha * 0.17f;
        alpha = alpha * 0.6f;
        switch (entry->sprite) {
        case 0:
            mnuDrawScaledMovieRollSprite(x, y, 0x64, alpha, set, 8.0f, 8.0f, 0xD, 0x60, texture);
            return;
        case 1:
            mnuDrawScaledMovieRollSprite(x, y, 0x64, alpha, set, 8.0f, 8.0f, 0xE, 0x60, texture);
            return;
        case 2:
            mnuDrawScaledMovieRollSprite(x, y, 0x64, alpha, set, 8.0f, 8.0f, 0xF, 0x60, texture);
            break;
        }
    }
}


extern u32 mnuStaffRollImageSpriteFrames[];


extern u8 mnuCampCommandLabels[];

extern u8 mnuCampItemCommandLabels[];

extern u8 mnuCampSetCommandLabels[];

extern u8 mnuCampCommandVisibilityFlagIds[];

extern u32 mnuStaffRollImageMode32SpriteFrames[];
extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);
extern void mnuUpdateMovieRollEntry(MnuMovieRollEntry *);
extern void mnuDrawAnimatedMovieRollEntry(MnuMovieRollEntry *, EffectSlotSet *, s32);

/* Draw the five-tile staff backdrop and both 32-entry roll passes.
 * Phases1/2/3 add the selected image overlay; keep every fresh movie-work read
 * after callbacks and the final GS TEST/ALPHA restore in native order. */
void mnuDrawStaffRollFrame(s32 surface) {
    s32 i;

    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x3000D, surface);
    uiDrawActiveSurfaceRegion(surface);
    mnuDrawIconAlphaSprite(-53, -10, 400, 128, mnuMovieWork->spriteSet, 12, 32, surface);
    mnuDrawIconAlphaSprite(85, -10, 400, 128, mnuMovieWork->spriteSet, 12, 32, surface);
    mnuDrawIconAlphaSprite(223, -10, 400, 128, mnuMovieWork->spriteSet, 12, 32, surface);
    mnuDrawIconAlphaSprite(361, -10, 400, 128, mnuMovieWork->spriteSet, 12, 32, surface);
    mnuDrawIconAlphaSprite(499, -10, 400, 128, mnuMovieWork->spriteSet, 12, 32, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
    for (i = 0; i < 32; i++) {
        mnuUpdateMovieRollEntry(&mnuMovieRollEntries[i]);
        sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
        sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
        mnuDrawAnimatedMovieRollEntry(&mnuMovieRollEntries[i], mnuMovieWork->spriteSet, surface);
    }
    if (D_003BC618 == 1 || D_003BC618 == 3 || D_003BC618 == 2) {
        uiDrawActiveSurfaceRegion(surface);
        sdfSubmitGsTestOneRegisterPacket(0x3001B, surface);
        mnuDrawIconAlphaSprite(mnuStaffRollImageRecords[D_003BC614].x - 5,
                              mnuStaffRollImageRecords[D_003BC614].y - 5, 1, 128,
                              mnuMovieWork->spriteSet,
                              mnuStaffRollImageMode32SpriteFrames[mnuMovieWork->imageIndex], 32, surface);
        sdfDispatchSurfaceWithPreparedTexturePacket(surface);
        sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
        sdfSubmitGsTestOneRegisterPacket(0x50000, surface);
        for (i = 0; i < 32; i++) {
            mnuDrawMovieRollEntry(&mnuMovieRollEntries[i], mnuMovieWork->spriteSet, surface);
        }
    }
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, surface);
    sdfSubmitGsTestOneRegisterPacket(0x5100D, surface);
    /* Retail has jal+epilogue; reset this function's completed roll-loop counter.
     * .01.sibling keeps the call; .13.life deletes this reset. See docs/idioms.md,
     * "Native post-call loop reset: DDS1 staff frame". */
    i = 0;
}

extern void mnuLoadStaffFonts(void);

void mnuPrepareStaffRollText(void) {
    mnuLoadStaffFonts();
}

extern void mnuUnloadStaffFonts(void);

void mnuReleaseStaffRollText(void) {
    mnuUnloadStaffFonts();
}

extern FrFontGlyph *frFontBuildColoredGlyphWithSharedFlags();
s32 mnuDrawStaffRollText(f32 x, f32 y, s32 alternate, u32 color, const char *source) {
    FrFontGlyph *glyph;

    if (alternate == 0) {
        glyph = frFontBuildColoredGlyphWithSharedFlags(
            (s32)(x * 16.0f), (s32)(y * 8.0f), 0, 1, 1, 10, color, source, 0);
    } else {
        glyph = frFontBuildColoredGlyphWithSharedFlags(
            (s32)(x * 16.0f), (s32)(y * 8.0f), 0, 0, 1, 8, color, source, 0);
    }
    frFontMeasureLines(glyph);
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    return frFontQueueGlyphForCurrentDrawBuffer(glyph);
}

extern char D_003BC628[];
extern void sprintf(char *, const char *, ...);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern void mnuAdvanceTitleStateUnderSemaphore(void);
extern void mnuInitializeMovieRollViewport(void);
extern void mnuReleaseMovieResourceAfterPendingWork(KwlnTask *task);
extern s32 mnuStaffImageProc(KwlnTask *task);
extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);

s32 mnuUpdateStaffRoll(KwlnTask *task) {
    char text[128];
    s32 request;
    s32 i;
    s32 y;
    MnuStaffTextRow *entry;

    (void)task;
    switch (mnuMovieWork->phase) {
    case 0:
        if (mnuPollTitleStreamStateLocked() == 2) {
            if (sdfCheckPendingWorkWithInterrupts() == 0) {
                mnuMovieWork->phase = 1;
                mnuMovieWork->scrollTicks = 0;
            }
        }
        break;
    case 1:
        mnuPrepareStaffRollText();
        mnuInitializeMovieRollViewport();
        kwlnTaskCreate(D_003B1140, 0x2B19, 0, 0, mnuStaffImageProc,
                       mnuReleaseMovieResourceAfterPendingWork, 0);
        mnuTitleStreamUpdateAndLogBgm();
        kwlnFadeOutStart(0, 0, 0, 0x1E);
        mnuMovieWork->phase = 3;
        mnuMovieWork->scrollTicks = 0;
        break;
    case 2:
    case 3:
        for (i = 0; i < 0x100; i++) {
            y = mnuStaffRollTextRows[i].offsetY - mnuMovieWork->scrollTicks + 0x1C0;
            if (i == 0xFF) {
                y = y < 0xE0 ? 0xE0 : y;
            }
            if ((u32)(y + 0x1E) < 0x1DF) {
                entry = &mnuStaffRollTextRows[i];
                if (entry->leftText != 0 && mnuStaffRollTextRows[i].rightText == 0) {
                    sprintf(text, D_003BC628, entry->leftText);
                    mnuDrawStaffRollText(20.0f, (f32)y, entry->leftFont, 0x80808080, text);
                } else if (mnuStaffRollTextRows[i].rightText != 0) {
                    sprintf(text, D_003BC628, mnuStaffRollTextRows[i].rightText);
                    mnuDrawStaffRollText(30.0f, (f32)y, mnuStaffRollTextRows[i].rightFont, 0x80808080, text);
                }
            }
        }
        mnuMovieWork->scrollTicks++;
        if (mnuMovieWork->scrollTicks == (mnuStaffRollTextRows[0xFF].offsetY + 0xE0) / 60 * 60 + 0x12C) {
            kwlnFadeInStart(0, 0, 0, 0x28);
        }
        if (mnuMovieWork->scrollTicks / 60 > (mnuStaffRollTextRows[0xFF].offsetY + 0xE0) / 60 + 5) {
            mnuAdvanceTitleStateUnderSemaphore();
        }
        if (mnuMovieWork->scrollTicks / 60 > (mnuStaffRollTextRows[0xFF].offsetY + 0xE0) / 60 + 6) {
            if (D_003BC618 != 4 && D_003BC618 != 5) {
                return 0;
            }
            request = 0x293;
            dds3AdminSubmitModeRequest(6, &request, 4, 0);
        }
        break;
    default:
        return 0;
    }
    return 0;
}

extern s32 mnuCheckMovieDecoderStatus(void);
extern void mnuStopMovieDrawTask(void);
extern void mnuRequestIndexedMovieResource(s32 index);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1140);

void mnuAdvanceStaffImageSequence(void) {
    s32 x = mnuStaffRollImageRecords[D_003BC614].x;
    s32 y = mnuStaffRollImageRecords[D_003BC614].y;
    s32 width = mnuStaffRollImageRecords[D_003BC614].width;
    s32 height = mnuStaffRollImageRecords[D_003BC614].height;
    s32 alpha = 128;
    s32 i;

    mnuMovieWork->imageAlpha = 0;
    switch (D_003BC618) {
    case 1:
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuMovieWork->fadeInTicks = 0;
            D_003BC618 = 2;
            mnuMovieWork->imageAlpha = 128;
        } else {
            if (mnuMovieDrawContext.soundNode.playbackFrameIndex < 32) {
                alpha = mnuMovieDrawContext.soundNode.playbackFrameIndex * 4;
                if (alpha > 128) {
                    alpha = 128;
                }
            }
            sdfSetGridScaledDrawBounds(x, y, width, height, ((u32)alpha << 24) | 0x808080);
            mnuMovieWork->imageAlpha = alpha;
        }
        break;
    case 2:
        mnuMovieWork->imageAlpha = 128;
        mnuMovieWork->fadeInTicks++;
        if (mnuMovieWork->fadeInTicks >= 0) {
            if (mnuMovieWork->fadeInTicks >= 128) {
                mnuMovieWork->fadeOutTicks = 0;
                D_003BC618 = 3;
            } else {
                sdfSetGridScaledDrawBounds(x, y, width, height, 0x80808080);
                return;
            }
        }
        break;
    case 3:
        mnuMovieWork->fadeOutTicks++;
        if (mnuMovieWork->fadeOutTicks >= 260) {
            mnuStopMovieDrawTask();
            i = ++D_003BC614;
            mnuMovieWork->imageAlpha = 0;
            if (i == 15) {
                D_003BC618 = 5;
            } else {
                D_003BC618 = 4;
            }
        } else {
            alpha = 128.0f - mnuMovieWork->fadeOutTicks * 0.5f;
            if (alpha < 0) {
                alpha = 0;
            }
            sdfSetGridScaledDrawBounds(x, y, width, height, ((u32)alpha << 24) | 0x808080);
            mnuMovieWork->imageAlpha = alpha;
        }
        break;
    case 4:
        mnuMovieWork->imageAlpha = 0;
        if (mnuMovieWork->scrollTicks < mnuStaffRollImageRecords[D_003BC614].startFrame ||
            mnuMovieWork->scrollTicks / 60 > (mnuStaffRollTextRows[255].offsetY + 224) / 60 - 4) {
            break;
        }
        for (i = 14; i >= D_003BC614; i--) {
            if (mnuMovieWork->scrollTicks >= mnuStaffRollImageRecords[i].startFrame) {
                if (mnuCheckMovieDecoderStatus() == 0 && D_003BC614 != 0) {
                    return;
                }
                sdfSetGridScaledDrawBounds(mnuStaffRollImageRecords[D_003BC614].x, mnuStaffRollImageRecords[D_003BC614].y,
                    mnuStaffRollImageRecords[D_003BC614].width, mnuStaffRollImageRecords[D_003BC614].height, 0);
                if (D_003BC614 == 0) {
                    mnuRequestIndexedMovieResource(mnuStaffRollImageRecords[0].movieIndex);
                    D_003BC614 = 0;
                    mnuMovieWork->imageIndex = mnuStaffRollImageRecords[0].imageIndex;
                } else {
                    mnuRequestIndexedMovieResource(mnuStaffRollImageRecords[i].movieIndex);
                    D_003BC614 = i;
                    mnuMovieWork->imageIndex = mnuStaffRollImageRecords[i].imageIndex;
                }
                D_003BC618 = 1;
                return;
            }
        }
        /* Waiting and finished states both leave the image transparent. */
    case 5:
        mnuMovieWork->imageAlpha = 0;
        break;
    }
}

s32 mnuStaffImageProc(KwlnTask *task) {
    mnuDrawIconAlphaSprite(-10, -10, 0, 0x80, mnuMovieWork->spriteSet, 0x10, 0, 0x27);
    mnuDrawIconAlphaSprite(mnuStaffRollImageRecords[D_003BC614].x - 5, mnuStaffRollImageRecords[D_003BC614].y - 5, 0, 0x80, mnuMovieWork->spriteSet, mnuStaffRollImageSpriteFrames[mnuMovieWork->imageIndex], 0, 0x53);
    mnuDrawStaffRollFrame(0x53);
    mnuAdvanceStaffImageSequence();
    return 0;
}

void mnuFinishStaffMovieAndFreeState(KwlnTask *task) {
    s32 pendingWork;

    mnuMovieTaskState = 2;
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    mnuReleaseStaffRollText();
    do {
        pendingWork = sdfCheckPendingWorkWithInterrupts();
    } while (pendingWork != 0);
    sdfQueueGeneralAllocationRelease(mnuMovieWork->allocation);
    mnuMovieWork = NULL;
}

void mnuReleaseMovieResourceAfterPendingWork(KwlnTask *task) {
    effDestroyResourceSlotSet(mnuMovieWork->spriteSet);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    sdfSetStreamFrameDrawMode(0);
}

void mnuInitializeMovieRollViewport(void) {
    s32 i;
    MnuStaffMovieWork *movie = mnuMovieWork;

    movie->imageAlpha = 0;
    movie->imageIndex = 0;
    D_003BC614 = 0;
    D_003BC618 = 4;
    mnuLoadMovieRollSprite();
    sdfSetStreamFrameDrawMode(1);
    sdfSetGridScaledDrawBounds(0x68, 0x69, 0x180, 0xEE, 0x80808080);
    for (i = 0; i < 32; i++) {
        mnuInitializeMovieRollEntry();
        mnuMovieRollEntries[i].active = 0;
    }
}

extern u32 D_003BA8EC;

extern u16 mnuMovieTaskState;

extern char D_003B1168[];




extern s32 mnuUpdateStaffRoll(KwlnTask *task);

extern void mnuFinishStaffMovieAndFreeState(KwlnTask *task);

void mnuMovieCreateTask(void) {
    struct SdfMemBlock *allocation;
    MnuStaffMovieWork *movie;

    D_003BA8EC = 0x80000000;
    allocation = sdfAllocGeneralBlock(0x20);
    movie = (MnuStaffMovieWork *)sdfResourceRetainAddress(allocation);
    mnuMovieWork = movie;
    movie->allocation = allocation;
    movie->phase = 0;
    movie->scrollTicks = 0;
    mnuQueueTitleSoundStreamEntry(0x13);
    mnuMovieTaskState = 1;
    kwlnTaskCreate(D_003B1168, 0x408, 0, 0, mnuUpdateStaffRoll, mnuFinishStaffMovieAndFreeState, 0);
}

u32 mnuStartStaffMovieRequest(void) {
    mnuMovieCreateTask();
    return 0xffffffff;
}

s32 mnuStopStaffTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B1140, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003B1168, 1);
    return 0;
}

s32 mnuMovieDraw(KwlnTask *task) {
    func_002ECCF8(&mnuMovieDrawContext, &D_003253C8);
    return 0;
}

void mnuStartMovieDrawTaskForResource(const char *fileName, SdfMovieDescriptor *descriptor) {
    if (mnuMovieDrawTask == 0) {
        sdfMovieInitializeStreamWork(&mnuMovieDrawContext, descriptor, fileName);
        mnuMovieDrawTask = kwlnTaskCreate(D_003B1A78, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

void mnuRequestIndexedMovieResource(s32 movieIndex) {
    MnuMovieResourceEntry *entry = &mnuMovieResourceEntries[movieIndex];

    mnuStartMovieDrawTaskForResource(entry->fileName, &entry->descriptor);
}

void mnuStopMovieDrawTask(void) {
    if (mnuMovieDrawTask == 0) {
        return;
    }
    sdfCancelAndReleaseMovieStreamWork(&mnuMovieDrawContext);
    kwlnTaskDestroyWithHierarchy(mnuMovieDrawTask, 0);
    mnuMovieDrawTask = 0;
}

extern s32 sdfPacCheckDecoderStatus(MovObj *job);

s32 mnuCheckMovieDecoderStatus(void) {
    sdfPacCheckDecoderStatus(&mnuMovieDrawContext);
}


s32 mnuGetMoviePlaybackMode(void) {
    return D_0037B8BC[0];
}

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1168);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1178);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1198);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B11F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1218);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1238);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1258);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1278);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1298);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B12F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1318);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1338);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1358);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1378);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1398);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B13F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1418);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1438);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1458);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1478);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1498);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B14F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1518);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1538);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1558);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1578);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1598);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B15F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1618);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1638);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1658);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1678);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1698);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B16F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1718);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1738);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1758);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1778);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1798);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17D8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B17F8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1818);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1838);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1850);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1868);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1888);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18A0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18B8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18D0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B18F0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1908);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1920);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1938);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1958);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1970);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1990);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19B0);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19C8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B19E8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A08);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A20);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A38);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A58);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1A78);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC614);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC618);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC620);

INCLUDE_SDATA(const s32, "game/code_0026EC90", D_003BC628);

INCLUDE_SDATA(const s32, "game/code_0026EC90", mnuMovieDrawTask);

