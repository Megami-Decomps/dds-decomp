#include "common.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "mnu_movie.h"
#include "sdf.h"
#include "mnu_movie_resource.h"
#include "itf.h"
#include "kwln_task_lifecycle.h"

extern u8 D_0037B8BC[];

extern MovObj mnuMovieDrawContext;

extern SdfPoolNode D_003253C8;

extern char D_003B1140[]; /* "mnuStaffImageProc" */

extern char D_003B1168[]; /* "staffProc" */

extern MnuMovieResourceEntry D_0037B168[];

extern KwlnTask *mnuMovieDrawTask;
extern KwlnTask *kwlnTaskCreate();

extern u16 mnuMovieTaskState;

extern MnuStaffMovieWork *mnuMovieWork;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_003B1A78[]; /* "mnuMovieDraw" */

extern s32 D_003BC618;
extern void mnuLoadMovieRollSprite(void);
extern void func_0026E8D8(void);
extern void func_002ECA40(s32);
extern void sdfSetGridScaledDrawBounds(s32, s32, s32, s32, u32);
extern void func_002ECCF8(void *, SdfPoolNode *);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026EC90);

extern void func_0026E798(s32 x, s32 y, s32 z, s32 alpha, EffectSlotSet *set, s32 index, f32 scaleX, f32 scaleY, s32 option, s32 texture);

/* Draw an active roll entry with its sprite-specific slot index; alpha follows the staff movie's scroll fade. */
void func_0026F118(MnuMovieRollEntry *entry, EffectSlotSet *set, s32 texture) {
    s32 x;
    s32 y;
    s32 alpha;

    if (entry->active != 0) {
        x = entry->x;
        y = entry->y;
        alpha = (s32)mnuMovieWork->unk10 * 0.17f;
        alpha = alpha * 0.6f;
        switch (entry->sprite) {
        case 0:
            func_0026E798(x, y, 0x64, alpha, set, 0xD, 8.0f, 8.0f, 0x60, texture);
            return;
        case 1:
            func_0026E798(x, y, 0x64, alpha, set, 0xE, 8.0f, 8.0f, 0x60, texture);
            return;
        case 2:
            func_0026E798(x, y, 0x64, alpha, set, 0xF, 8.0f, 8.0f, 0x60, texture);
            break;
        }
    }
}

typedef struct StaffImage {
    s32 startFrame;
    s32 movieIndex;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    u32 imageIndex;
} StaffImage;

extern StaffImage D_0037AFC0[];

extern u32 D_0037AF70[];

extern s32 D_003BC614;

extern u8 D_0037B950[];

extern u8 D_0037B970[];

extern u8 D_0037B980[];

extern u8 D_0037C388[];

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F230);

void func_0026F500(void) {
    mnuLoadStaffFonts();
}

void func_0026F518(void) {
    mnuUnloadStaffFonts();
}

extern FrFontGlyph *frFontBuildColoredGlyphWithSharedFlags();
s32 func_0026F530(s32 alternate, u32 color, const char *source, f32 x, f32 y) {
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

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F5E8);

extern s32 mnuCheckMovieDecoderStatus(void);
extern void mnuStopMovieDrawTask(void);
extern void mnuRequestIndexedMovieResource(s32 index);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1140);

void func_0026F918(void) {
    s32 x = D_0037AFC0[D_003BC614].x;
    s32 y = D_0037AFC0[D_003BC614].y;
    s32 width = D_0037AFC0[D_003BC614].width;
    s32 height = D_0037AFC0[D_003BC614].height;
    s32 alpha = 128;
    s32 i;

    mnuMovieWork->unk10 = 0;
    switch (D_003BC618) {
    case 1:
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuMovieWork->fadeInTicks = 0;
            D_003BC618 = 2;
            mnuMovieWork->unk10 = 128;
        } else {
            if (mnuMovieDrawContext.soundNode.playbackFrameIndex < 32) {
                alpha = mnuMovieDrawContext.soundNode.playbackFrameIndex * 4;
                if (alpha > 128) {
                    alpha = 128;
                }
            }
            sdfSetGridScaledDrawBounds(x, y, width, height, ((u32)alpha << 24) | 0x808080);
            mnuMovieWork->unk10 = alpha;
        }
        break;
    case 2:
        mnuMovieWork->unk10 = 128;
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
            mnuMovieWork->unk10 = 0;
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
            mnuMovieWork->unk10 = alpha;
        }
        break;
    case 4:
        mnuMovieWork->unk10 = 0;
        if (mnuMovieWork->scrollTicks < D_0037AFC0[D_003BC614].startFrame ||
            mnuMovieWork->scrollTicks / 60 > (D_00379F70[255].offsetY + 224) / 60 - 4) {
            break;
        }
        for (i = 14; i >= D_003BC614; i--) {
            if (mnuMovieWork->scrollTicks >= D_0037AFC0[i].startFrame) {
                if (mnuCheckMovieDecoderStatus() == 0 && D_003BC614 != 0) {
                    return;
                }
                sdfSetGridScaledDrawBounds(D_0037AFC0[D_003BC614].x, D_0037AFC0[D_003BC614].y,
                    D_0037AFC0[D_003BC614].width, D_0037AFC0[D_003BC614].height, 0);
                if (D_003BC614 == 0) {
                    mnuRequestIndexedMovieResource(D_0037AFC0[0].movieIndex);
                    D_003BC614 = 0;
                    mnuMovieWork->imageIndex = D_0037AFC0[0].imageIndex;
                } else {
                    mnuRequestIndexedMovieResource(D_0037AFC0[i].movieIndex);
                    D_003BC614 = i;
                    mnuMovieWork->imageIndex = D_0037AFC0[i].imageIndex;
                }
                D_003BC618 = 1;
                return;
            }
        }
        /* Waiting and finished states both leave the image transparent. */
    case 5:
        mnuMovieWork->unk10 = 0;
        break;
    }
}

s32 mnuStaffImageProc(void) {
    mnuDrawIconAlphaSprite(-10, -10, 0, 0x80, mnuMovieWork->spriteSet, 0x10, 0, 0x27);
    mnuDrawIconAlphaSprite(D_0037AFC0[D_003BC614].x - 5, D_0037AFC0[D_003BC614].y - 5, 0, 0x80, mnuMovieWork->spriteSet, D_0037AF70[mnuMovieWork->imageIndex], 0, 0x53);
    func_0026F230(0x53);
    func_0026F918();
    return 0;
}

void mnuFinishStaffMovieAndFreeState(void) {
    s64 pendingWork;

    mnuMovieTaskState = 2;
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_0026F518();
    do {
        pendingWork = sdfCheckPendingWorkWithInterrupts();
    } while (pendingWork != 0);
    sdfQueueGeneralAllocationRelease(mnuMovieWork->allocation);
    mnuMovieWork = NULL;
}

void mnuReleaseMovieResourceAfterPendingWork(void) {
    effDestroyResourceSlotSet(mnuMovieWork->spriteSet);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_002ECA40(0);
}

void mnuInitializeMovieRollViewport(void) {
    s32 i;
    MnuStaffMovieWork *movie = mnuMovieWork;

    movie->unk10 = 0;
    movie->imageIndex = 0;
    D_003BC614 = 0;
    D_003BC618 = 4;
    mnuLoadMovieRollSprite();
    func_002ECA40(1);
    sdfSetGridScaledDrawBounds(0x68, 0x69, 0x180, 0xEE, 0x80808080);
    for (i = 0; i < 32; i++) {
        func_0026E8D8();
        D_003DC1E0[i].active = 0;
    }
}

extern u32 D_003BA8EC;

extern u16 mnuMovieTaskState;

extern char D_003B1168[];



extern void func_0026A5F0(s32);

extern s32 func_0026F5E8(void);

extern void mnuFinishStaffMovieAndFreeState(void);

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
    func_0026A5F0(0x13);
    mnuMovieTaskState = 1;
    kwlnTaskCreate(D_003B1168, 0x408, 0, 0, func_0026F5E8, mnuFinishStaffMovieAndFreeState, 0);
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

s32 mnuMovieDraw(void) {
    func_002ECCF8(&mnuMovieDrawContext, &D_003253C8);
    return 0;
}

void mnuStartMovieDrawTaskForResource(const char *fileName, SdfMovieDescriptor *descriptor) {
    if (mnuMovieDrawTask == 0) {
        sdfMovieInitializeStreamWork(&mnuMovieDrawContext, descriptor, fileName);
        mnuMovieDrawTask = kwlnTaskCreate(D_003B1A78, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

void mnuRequestIndexedMovieResource(s32 index) {
    MnuMovieResourceEntry *entry = &D_0037B168[index];

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

s32 mnuCheckMovieDecoderStatus(void) {
    sdfPacCheckDecoderStatus(&mnuMovieDrawContext);
}


s32 func_00270088(void) {
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

