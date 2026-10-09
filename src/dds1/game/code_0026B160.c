#include "common.h"
#include "sdf_resource.h"
#include "kwln_task_lifecycle.h"
#include "mnu.h"
#include "mnu_sprite_resource.h"

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

extern void sndEnsureMidiBankResident(s32 trackId);
extern void mnuDrawMovieMenuBackgroundQuad(s32 alpha);
extern void func_0026AF78(void);
extern void func_0026C098(s32 mode);
extern u8 mnuHasSpriteHandle(void);
extern void kwlnFadeOutStart(s32 red, s32 green, s32 blue, s32 duration);
extern void kwlnFadeInStart(s32 red, s32 green, s32 blue, s32 duration);
extern void sndStartTrackExtended(s32 trackId);
extern void mnuStartMovieMenuSfx16(s32 volume);
extern void mnuStartMovieMenuSfx17(s32 volume);
extern void mnuStartMovieMenuSfx18(s32 volume);
extern s32 kwlnFadeIsActive(void);
extern s32 mnuIsAnyMenuInputPressed(void);
extern void mnuRequestIndexedMovieResource(s32 index);
extern s32 func_00270088(void);
extern s32 mnuCheckMovieDecoderStatus(void);
extern void sdfSoundSetChannelCount(u32 channelCount);
extern void func_0026DD10(void);
extern void mnuDrawMovieMenuSpriteLayers(s32 context);
extern void func_0026C4A8(void);
extern void mnuSwapStateWords(void);
extern s32 func_0026C4B8(void);
extern void func_0026D108(void);
extern void func_0026C7E0(void);
extern s32 mnuPollMovieMenuInputAndTimeout(void);
extern void func_0026D270(void);
extern void mnuSelectMenuListCursorByAdvance(s32 amount);
extern void mnuTitleResetSequenceTimers(void);
extern void func_002E96D8(u32 trackId);
extern void func_0026CAB0(void);
extern s32 func_0026D660(void);
extern void func_0026D808(void);
extern void mnuClearGlobalMenuStateFields(void);
extern u32 func_0026BED0(void);
extern void dds3AdminSubmitModeRequest(s32 requestedMode, void *requestData,
                                       u32 dataBytes, s32 markHistory);
extern void mnuRestartRuntimeAfterViewer(s32 unused);
extern void fldPrepareDeferredSceneTransition(void);
extern void func_0026C1F8(s32 alpha);
extern void kwlnFadeClear(void);

s32 func_0026B1F0(KwlnTask *task) {
    s32 alpha;
    s32 menuAction;
    s32 selectedItem;

    /* The byte at 0x3C records an input interruption of the movie. */
    switch (mnuMovieMenuState->state) {
    case 0:
        mnuMovieMenuState->cursor = 0;
        mnuMovieMenuState->state = 2;
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
            kwlnFadeOutStart(0, 0, 0, 0x14);
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
        if (mnuHasSpriteHandle() != 0) {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            mnuMovieMenuState->state = 0x1E;
            kwlnFadeOutStart(0, 0, 0, 0x14);
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
        if (kwlnFadeIsActive() == 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
            mnuStopMovieDrawTask();
        }
        break;
    case 6:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx16(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 7:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = 0x80 - mnuMovieMenuState->cursor * 8;
        if (alpha > 0) {
            mnuStartMovieMenuSfx16(alpha);
        }
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 8:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = mnuMovieMenuState->cursor * 6;
        if (alpha >= 0x81) {
            alpha = 0x80;
        }
        if (alpha > 0) {
            mnuStartMovieMenuSfx17(alpha);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 9:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx17(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 10:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = 0x80 - mnuMovieMenuState->cursor * 8;
        if (alpha > 0) {
            mnuStartMovieMenuSfx17(alpha);
        }
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 11:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        alpha = mnuMovieMenuState->cursor * 6;
        if (alpha >= 0x81) {
            alpha = 0x80;
        }
        if (alpha > 0) {
            mnuStartMovieMenuSfx18(alpha);
        }
        if (mnuMovieMenuState->cursor < 0x14) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 12:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuStartMovieMenuSfx18(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->cursor = 0x4B;
        }
        if (mnuMovieMenuState->cursor < 0x4B) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            kwlnFadeInStart(0, 0, 0, 0x16);
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
        }
        break;
    case 13:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuMovieMenuState->cursor < 0x16) {
            mnuStartMovieMenuSfx18(0x80);
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            kwlnFadeOutStart(0, 0, 0, 0x1E);
            mnuMovieMenuState->state = 0xE;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 14:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuRequestIndexedMovieResource(0x40);
        mnuMovieMenuState->state = 0xF;
        mnuMovieMenuState->pad38[4] = 0;
        break;
    case 15:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (func_00270088() != 0) {
            mnuMovieMenuState->state = 0x10;
        }
        break;
    case 16:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->state = 0x11;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0x16);
            sdfSoundSetChannelCount(0x2D);
            mnuMovieMenuState->pad38[4] = 1;
        }
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuMovieMenuState->state = 0x11;
            mnuMovieMenuState->cursor = 0x16;
        }
        break;
    case 17:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (mnuMovieMenuState->cursor < 0x2D) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuStopMovieDrawTask();
            kwlnFadeOutStart(0, 0, 0, 0x1E);
            mnuMovieMenuState->state = 0x12;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 18:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if ((s8)mnuMovieMenuState->pad38[4] != 0) {
            mnuMovieMenuState->state = 0x16;
        } else {
            func_0026DD10();
            mnuMovieMenuState->state = 0x13;
        }
        break;
    case 19:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        if (func_00270088() != 0) {
            mnuMovieMenuState->state = 0x14;
        }
        break;
    case 20:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuIsAnyMenuInputPressed() != 0) {
            mnuMovieMenuState->state = 0x16;
            mnuMovieMenuState->cursor = 0;
            mnuStopMovieDrawTask();
        }
        if (mnuCheckMovieDecoderStatus() != 0) {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            mnuDrawMovieMenuSpriteLayers(0x3E);
            mnuMovieMenuState->state = 0x16;
            mnuMovieMenuState->cursor = 0;
            mnuStopMovieDrawTask();
        }
        break;
    case 21:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuMovieMenuState->cursor < 0x12) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuStopMovieDrawTask();
            mnuMovieMenuState->state = 0x16;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 22:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        func_0026C098(0);
        mnuMovieMenuState->state = 0x17;
        break;
    case 23:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        mnuDrawMovieMenuSpriteLayers(0x3E);
        if (mnuHasSpriteHandle() != 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
            func_0026C4A8();
            mnuSwapStateWords();
        }
        break;
    case 24:
        func_0026B1C0();
        if (func_0026C4B8() != 0) {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = mnuMovieMenuState->state + 1;
            func_0026D108();
        }
        break;
    case 25:
        func_0026B1C0();
        func_0026C7E0();
        menuAction = mnuPollMovieMenuInputAndTimeout();
        if (menuAction != -1) {
            if (menuAction != 1) {
                func_0026D270();
            } else {
                mnuSelectMenuListCursorByAdvance(0);
                func_0026D270();
                mnuMovieMenuState->state = 0x1E;
                mnuMovieMenuState->cursor = 0;
                mnuTitleResetSequenceTimers();
            }
        } else {
            func_0026D270();
            mnuMovieMenuState->state = 0x1B;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0xF);
            func_002E96D8(0x310000);
        }
        break;
    case 27:
        func_0026B1C0();
        if (mnuMovieMenuState->cursor < 0x78) {
            func_0026C7E0();
            func_0026D270();
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            kwlnFadeOutStart(0, 0, 0, 0x14);
            mnuMovieMenuState->state = 5;
            mnuMovieMenuState->cursor = 0;
            mnuReleaseSpriteHandle();
        }
        break;
    case 29:
        func_0026B1C0();
        func_0026CAB0();
        if (kwlnFadeIsActive() == 0) {
            mnuMovieMenuState->state = 0x1E;
            mnuMovieMenuState->cursor = 0;
            mnuTitleResetSequenceTimers();
        }
        break;
    case 30:
        func_0026B1C0();
        func_0026CAB0();
        menuAction = func_0026D660();
        switch (menuAction) {
        case 1:
            func_0026D808();
            mnuMovieMenuState->state = 0x1F;
            mnuMovieMenuState->cursor = 0;
            kwlnFadeInStart(0, 0, 0, 0xF);
            break;
        case 2:
            func_0026D808();
            mnuMovieMenuState->state = 0x20;
            mnuMovieMenuState->cursor = 0;
            mnuClearGlobalMenuStateFields();
            mnuMovieMenuState->state = 0x19;
            mnuMovieMenuState->cursor = 0;
            break;
        default:
            func_0026D808();
            break;
        }
        break;
    case 31:
        func_0026B1C0();
        if (mnuMovieMenuState->cursor < 0xF) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
            func_0026CAB0();
        } else {
            mnuDrawMovieMenuBackgroundQuad(0x80);
            selectedItem = func_0026BED0();
            switch (selectedItem) {
            case 1:
                func_002E96D8(0x310000);
                kwlnFadeOutStart(0, 0, 0, 0x14);
                mnuMovieMenuState->state = 0x27;
                mnuMovieMenuState->cursor = 0;
                break;
            case 0:
                mnuMovieMenuState->mode = 1;
                dds3AdminSubmitModeRequest(3, 0, 0, 0);
                mnuMovieMenuState->state = 0x2B;
                break;
            case 2:
                func_002E96D8(0x310000);
                mnuMovieMenuState->mode = selectedItem;
                mnuMovieMenuState->cursor = 0;
                kwlnFadeOutStart(0, 0, 0, 0xA);
                dds3AdminSubmitModeRequest(0xD, 0, 0, 0);
                mnuMovieMenuState->state = 0x2B;
                break;
            default:
                break;
            }
        }
        break;
    case 32:
        func_0026B1C0();
        func_0026CAB0();
        mnuClearGlobalMenuStateFields();
        mnuMovieMenuState->state = 0x19;
        mnuMovieMenuState->cursor = 0;
        break;
    case 39:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        kwlnFadeClear();
        mnuRestartRuntimeAfterViewer(0);
        fldPrepareDeferredSceneTransition();
        mnuMovieMenuState->state = 0x2B;
        mnuMovieMenuState->cursor = 0;
        break;
    case 40:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 0x3C) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            kwlnFadeOutStart(0, 0, 0, 0x14);
            mnuMovieMenuState->state = 0x29;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 41:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 0x96) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            kwlnFadeInStart(0, 0, 0, 0x16);
            mnuMovieMenuState->state = 0x2A;
            mnuMovieMenuState->cursor = 0;
        }
        break;
    case 42:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        func_0026C1F8(0x80);
        if (mnuMovieMenuState->cursor < 0x2D) {
            mnuMovieMenuState->cursor = mnuMovieMenuState->cursor + 1;
        } else {
            mnuMovieMenuState->cursor = 0;
            mnuMovieMenuState->state = 0x27;
        }
        break;
    case 43:
        mnuDrawMovieMenuBackgroundQuad(0x80);
        break;
    }
    return 0;
}

u32 func_0026BCE8(void) {
    func_0026B050();
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
