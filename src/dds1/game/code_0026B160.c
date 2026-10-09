#include "common.h"
#include "sdf_resource.h"
#include "kwln_task_lifecycle.h"
#include "mnu.h"
#include "mnu_sprite_resource.h"

extern void sndEnsureMidiBankResident(s32);
extern void mnuDrawMovieMenuBackgroundQuad(s32);
extern void func_0026AF78(void);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern s32 kwlnFadeIsActive(void);
extern void kwlnFadeClear(void);
extern void func_0026C098(s32);
extern u8 mnuHasSpriteHandle(void);
extern void sndStartTrackExtended(s32);
extern void mnuStartMovieMenuSfx16(s32);
extern void mnuStartMovieMenuSfx17(s32);
extern void mnuStartMovieMenuSfx18(s32);
extern s32 mnuIsAnyMenuInputPressed(void);
extern void mnuRequestIndexedMovieResource(s32);
extern s32 func_00270088(void);
extern void sdfSoundSetChannelCount(u32);
extern s32 mnuCheckMovieDecoderStatus(void);
extern void func_0026DD10(void);
extern void mnuDrawMovieMenuSpriteLayers(s32);
extern void func_0026C4A8(void);
extern void mnuSwapStateWords(void);
extern s32 func_0026C4B8(void);
extern void func_0026D108(void);
extern void func_0026C7E0(void);
extern s32 mnuPollMovieMenuInputAndTimeout(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern void func_0026D270(void);
extern void mnuTitleResetSequenceTimers(void);
extern void func_002E96D8(u32);
extern void func_0026CAB0(void);
extern s32 func_0026D660(void);
extern void func_0026D808(void);
extern void mnuClearGlobalMenuStateFields(void);
extern u32 func_0026BED0(void);
extern void dds3AdminSubmitModeRequest(s32, void *, u32, s32);
extern void mnuRestartRuntimeAfterViewer();
extern void fldPrepareDeferredSceneTransition(void);
extern void func_0026C1F8(s32);

extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern char D_003AFD80[]; /* "titleProc" */

s32 mnuApplyInnerEffectVectorsAndTickObject(void);

extern MovieMenuState *mnuMovieMenuState;
extern s32 D_003BA730;
extern void mnuStopMovieDrawTask(void);
extern void mnuReleaseMenuResourceSlots(void);
extern void mnuReleaseSpriteHandle(void);
extern void mnuDestroyMovieMenuSelectionList(void);
extern KwlnTask *func_0026B050(s32);
extern void mnuReleaseMovieResourceGroup(MnuSpriteResourceGroup *);
extern void mnuMovieShutdownA(void);

void func_0026B160(void) {
    mnuStopMovieDrawTask();
    mnuReleaseMenuResourceSlots();
    mnuReleaseSpriteHandle();
    mnuDestroyMovieMenuSelectionList();
    mnuReleaseMovieResourceGroup(mnuMovieMenuState->resources);
    mnuMovieShutdownA();
    sdfReleaseResourceAllocation(mnuMovieMenuState->allocation);
    mnuMovieMenuState = 0;
    D_003BA730 = 1;
}

extern void func_0026CB10(void *, void *);

s32 func_0026B1C0(void) {
    func_0026CB10(D_003DC1C0, D_003DC1D0);
    return mnuApplyInnerEffectVectorsAndTickObject();
}

s32 func_0026B1F0(KwlnTask *task) {
    s32 alpha;
    s32 selection;
    switch (mnuMovieMenuState->state) {
    case 0:
        mnuMovieMenuState->state = 2;
        mnuMovieMenuState->cursor = 0;
        sndEnsureMidiBankResident(0x310000);
        break;
    case 2:
        mnuMovieMenuState->cursor = 0;
        D_003BA730 = 0;
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026AF78();
        if (mnuMovieMenuState->mode != 0) {
            mnuMovieMenuState->state = 3;
        } else {
            kwlnFadeOutStart(0, 0, 0, 20);
            mnuMovieMenuState->state = 5;
        }
        mnuMovieMenuState->mode = 0;
        break;
    case 3:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C098(0);
        mnuMovieMenuState->state = 4;
        mnuMovieMenuState->cursor = 0;
        break;
    case 4:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuHasSpriteHandle()) {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            mnuMovieMenuState->state = 30;
            kwlnFadeOutStart(0, 0, 0, 20);
            if (mnuMovieMenuState->mode >= 0) {
                if (mnuMovieMenuState->mode < 2) {
                    sndStartTrackExtended(0x310000);
                }
            }
        }
        break;
    case 5:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx16(0x80);
        if (!kwlnFadeIsActive()) {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
            mnuStopMovieDrawTask();
        }
        break;
    case 6:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx16(0x80);
        if (mnuIsAnyMenuInputPressed()) {
            mnuMovieMenuState->cursor = 75;
        }
        if (mnuMovieMenuState->cursor < 75) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 7:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = 128 - mnuMovieMenuState->cursor * 8;
        if (alpha > 0) {
            mnuStartMovieMenuSfx16(alpha);
        }
        if (mnuMovieMenuState->cursor < 22) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 8:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = mnuMovieMenuState->cursor * 6;
        if (alpha > 128) {
            alpha = 128;
        }
        if (alpha > 0) {
            mnuStartMovieMenuSfx17(alpha);
        }
        if (mnuMovieMenuState->cursor < 20) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 9:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx17(0x80);
        if (mnuIsAnyMenuInputPressed()) {
            mnuMovieMenuState->cursor = 75;
        }
        if (mnuMovieMenuState->cursor < 75) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 10:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = 128 - mnuMovieMenuState->cursor * 8;
        if (alpha > 0) {
            mnuStartMovieMenuSfx17(alpha);
        }
        if (mnuMovieMenuState->cursor < 22) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 11:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = mnuMovieMenuState->cursor * 6;
        if (alpha > 128) {
            alpha = 128;
        }
        if (alpha > 0) {
            mnuStartMovieMenuSfx18(alpha);
        }
        if (mnuMovieMenuState->cursor < 20) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 12:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx18(0x80);
        if (mnuIsAnyMenuInputPressed()) {
            mnuMovieMenuState->cursor = 75;
        }
        if (mnuMovieMenuState->cursor < 75) {
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeInStart(0, 0, 0, 22);
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 13:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuMovieMenuState->cursor < 22) {
            mnuStartMovieMenuSfx18(0x80);
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeOutStart(0, 0, 0, 30);
            mnuMovieMenuState->state = 14;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 14:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuRequestIndexedMovieResource(0x40);
        mnuMovieMenuState->state = 15;
        mnuMovieMenuState->movieSkipped = 0;
        break;
    case 15:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (func_00270088()) {
            mnuMovieMenuState->state = 16;
        }
        break;
    case 16:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuIsAnyMenuInputPressed()) {
            mnuMovieMenuState->state = 17;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 22);
            sdfSoundSetChannelCount(45);
            mnuMovieMenuState->movieSkipped = 1;
        }
        if (mnuCheckMovieDecoderStatus()) {
            mnuMovieMenuState->state = 17;
            mnuMovieMenuState->cursor = 22;
        }
        break;
    case 17:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuMovieMenuState->cursor < 45) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuStopMovieDrawTask();
            kwlnFadeOutStart(0, 0, 0, 30);
            mnuMovieMenuState->state = 18;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 18:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuMovieMenuState->movieSkipped) {
            mnuMovieMenuState->state = 22;
        } else {
            func_0026DD10();
            mnuMovieMenuState->state = 19;
        }
        break;
    case 19:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (func_00270088()) {
            mnuMovieMenuState->state = 20;
        }
        break;
    case 20:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuIsAnyMenuInputPressed()) {
            mnuMovieMenuState->state = 22;
            mnuMovieMenuState->cursor = 0;
            mnuStopMovieDrawTask();
        }
        if (mnuCheckMovieDecoderStatus()) {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            mnuDrawMovieMenuSpriteLayers(0x3E);
            mnuMovieMenuState->state = 22;
            mnuMovieMenuState->cursor = 0;
            mnuStopMovieDrawTask();
        }
        break;
    case 21:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuMovieMenuState->cursor < 18) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuStopMovieDrawTask();
            mnuMovieMenuState->state = 22;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 22:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        func_0026C098(0);
        mnuMovieMenuState->state = 23;
        break;
    case 23:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuHasSpriteHandle()) {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
            func_0026C4A8();
            mnuSwapStateWords();
        }
        break;
    case 24:
        func_0026B1C0();
        if (func_0026C4B8()) {
            mnuMovieMenuState->state++;
            mnuMovieMenuState->cursor = 0;
            func_0026D108();
        }
        break;
    case 25:
        func_0026B1C0();
        func_0026C7E0();
        switch (mnuPollMovieMenuInputAndTimeout()) {
        default:
            func_0026D270();
            break;
        case 1:
            mnuSelectMenuListCursorByAdvance(0);
            func_0026D270();
            mnuMovieMenuState->state = 30;
            mnuMovieMenuState->cursor = 0;
            mnuTitleResetSequenceTimers();
            break;
        case -1:
            func_0026D270();
            mnuMovieMenuState->state = 27;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 15);
            func_002E96D8(0x310000);
            break;
        }
        break;
    case 27:
        func_0026B1C0();
        if (mnuMovieMenuState->cursor < 120) {
            func_0026C7E0();
            func_0026D270();
            mnuMovieMenuState->cursor++;
        } else {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            kwlnFadeOutStart(0, 0, 0, 20);
            mnuMovieMenuState->state = 5;
            mnuMovieMenuState->cursor = 0;
            mnuReleaseSpriteHandle();
        }
        break;
    case 29:
        func_0026B1C0();
        func_0026CAB0();
        if (!kwlnFadeIsActive()) {
            mnuMovieMenuState->state = 30;
            mnuMovieMenuState->cursor = 0;
            mnuTitleResetSequenceTimers();
        }
        break;
    case 30:
        func_0026B1C0();
        func_0026CAB0();
        switch (func_0026D660()) {
        case 1:
            func_0026D808();
            mnuMovieMenuState->state = 31;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 15);
            break;
        case 2:
            func_0026D808();
            mnuMovieMenuState->state = 32;
            mnuMovieMenuState->cursor = 0;
            mnuClearGlobalMenuStateFields();
            mnuMovieMenuState->state = 25;
            mnuMovieMenuState->cursor = 0;
            break;
        default:
            func_0026D808();
            break;
        }
        break;
    case 31:
        func_0026B1C0();
        if (mnuMovieMenuState->cursor < 15) {
            mnuMovieMenuState->cursor++;
            func_0026CAB0();
        } else {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            selection = func_0026BED0();
            switch (selection) {
            case 1:
                func_002E96D8(0x310000);
                kwlnFadeOutStart(0, 0, 0, 20);
                mnuMovieMenuState->state = 39;
                mnuMovieMenuState->cursor = 0;
                break;
            case 0:
                mnuMovieMenuState->mode = 1;
                dds3AdminSubmitModeRequest(3, NULL, 0, 0);
                mnuMovieMenuState->state = 43;
                break;
            case 2:
                func_002E96D8(0x310000);
                mnuMovieMenuState->mode = selection;
                mnuMovieMenuState->cursor = 0;
                kwlnFadeOutStart(0, 0, 0, 10);
                dds3AdminSubmitModeRequest(13, NULL, 0, 0);
                mnuMovieMenuState->state = 43;
                break;
            }
        }
        break;
    case 32:
        func_0026B1C0();
        func_0026CAB0();
        mnuClearGlobalMenuStateFields();
        mnuMovieMenuState->state = 25;
        mnuMovieMenuState->cursor = 0;
        break;
    case 39:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        kwlnFadeClear();
        mnuRestartRuntimeAfterViewer(0);
        fldPrepareDeferredSceneTransition();
        mnuMovieMenuState->state = 43;
        mnuMovieMenuState->cursor = 0;
        break;
    case 40:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 60) {
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeOutStart(0, 0, 0, 20);
            mnuMovieMenuState->state = 41;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 41:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 150) {
            mnuMovieMenuState->cursor++;
        } else {
            kwlnFadeInStart(0, 0, 0, 22);
            mnuMovieMenuState->state = 42;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 42:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 45) {
            mnuMovieMenuState->cursor++;
        } else {
            mnuMovieMenuState->state = 39;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 43:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        break;
    }
    return 0;
}


u32 func_0026BCE8(s32 mode) {
    func_0026B050(mode);
    return 0xffffffff;
}

s32 mnuDestroyTitleMenuTask(void) {
    func_0026B160();
    kwlnTaskDestroyWithHierarchyByName(D_003AFD80, 1);
    return 0;
}

u32 func_0026BD38(void) {
    func_0026B050(0);
    return 0;
}
