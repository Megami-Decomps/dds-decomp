#include "common.h"
#include "mnu.h"

extern u8 D_0037B8BC[];

extern u8 mnuMovieDrawContext[];

extern u8 D_003253C8[];

extern char D_003B1140[]; /* "mnuStaffImageProc" */

extern char D_003B1168[]; /* "staffProc" */

extern u8 D_0037B168[];

extern u32 mnuMovieDrawTask;

extern u16 mnuMovieTaskState;

extern MnuStaffMovieWork *mnuMovieWork;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern char D_003B1A78[]; /* "mnuMovieDraw" */

typedef struct MnuMovieRollEntry {
    u8 pad00[0xC];
    u8 unk0C;
    u8 pad0D[0xF];
} MnuMovieRollEntry;

extern MnuMovieRollEntry D_003DC1E0[];
extern s32 D_003BC618;
extern void mnuLoadMovieRollSprite(void);
extern void func_0026E8D8(void);
extern void func_002ECA40(s32);
extern void sdfSetGridScaledDrawBounds(s32, s32, s32, s32, u32);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026EC90);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F118);

typedef struct StaffImage {
    u8 pad00[8];
    s32 x;
    s32 y;
    u8 pad10[0xC];
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

struct FrFontCtx;
struct FrFontGlyph;
extern struct FrFontCtx *frFontBuildColoredGlyphWithSharedFlags();
extern u32 frFontMeasureLines(struct FrFontGlyph *);
extern void frFontDrawGlyphWithSharedFlags(struct FrFontGlyph *, s8);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);

s32 func_0026F530(s32 alternate, u32 color, const char *source, f32 x, f32 y) {
    struct FrFontGlyph *glyph;

    if (alternate == 0) {
        glyph = (struct FrFontGlyph *)frFontBuildColoredGlyphWithSharedFlags(
            (s32)(x * 16.0f), (s32)(y * 8.0f), 0, 1, 1, 10, color, source, 0);
    } else {
        glyph = (struct FrFontGlyph *)frFontBuildColoredGlyphWithSharedFlags(
            (s32)(x * 16.0f), (s32)(y * 8.0f), 0, 0, 1, 8, color, source, 0);
    }
    frFontMeasureLines(glyph);
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    return frFontQueueGlyphInSelectedSlot(glyph);
}

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F5E8);

INCLUDE_RODATA(const s32, "game/code_0026EC90", D_003B1140);

INCLUDE_ASM(const s32, "game/code_0026EC90", func_0026F918);

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
    sdfQueueNonzeroResourceId(mnuMovieWork->allocation);
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
        D_003DC1E0[i].unk0C = 0;
    }
}

extern u32 D_003BA8EC;

extern u16 mnuMovieTaskState;

extern char D_003B1168[];

extern s32 sdfAllocGeneralBlock(s32);

extern s32 sdfResourceRetainAddress(s32);

extern void func_0026A5F0(s32);

extern s32 func_0026F5E8(void);

extern void mnuFinishStaffMovieAndFreeState(void);

void mnuMovieCreateTask(void) {
    s32 handle;
    MnuStaffMovieWork *movie;

    D_003BA8EC = 0x80000000;
    handle = sdfAllocGeneralBlock(0x20);
    movie = (MnuStaffMovieWork *)sdfResourceRetainAddress(handle);
    mnuMovieWork = movie;
    movie->allocation = handle;
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
    func_002ECCF8(mnuMovieDrawContext, D_003253C8);
    return 0;
}

void mnuStartMovieDrawTaskForResource(u32 resource, void *data) {
    if (mnuMovieDrawTask == 0) {
        func_002ED8D0(mnuMovieDrawContext, data, resource);
        mnuMovieDrawTask = kwlnTaskCreate(D_003B1A78, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

void mnuRequestIndexedMovieResource(s32 index) {
    u8 *entry = D_0037B168 + index * 24;

    mnuStartMovieDrawTaskForResource(*(s32 *)entry, (s32)(entry + 4));
}

void mnuStopMovieDrawTask(void) {
    if (mnuMovieDrawTask == 0) {
        return;
    }
    sdfCancelAndReleasePacWork(mnuMovieDrawContext);
    kwlnTaskDestroyWithHierarchy(mnuMovieDrawTask, 0);
    mnuMovieDrawTask = 0;
}

s32 mnuCheckMovieDecoderStatus(void) {
    sdfPacCheckDecoderStatus(mnuMovieDrawContext);
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

