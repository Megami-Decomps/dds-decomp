#include "common.h"
#include "kwln_background_fade_api.h"
#include "sdf_resource.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "mnu.h"
#include "mnu_sprite_resource.h"


extern void mnuStopMovieDrawTask(void);
extern void *memset(void *, s32, u32);
extern void mnuRecreateMenuSelectionList(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern void mnuLoadTitleMenuAssets(void);
extern MnuSpriteResourceGroup *mnuCreateMovieSpriteResource(s32, u8, u8);
extern s32 mnuUpdateMovieMenuTask(KwlnTask *);
extern u32 D_003BA8EC;
extern char D_003AFD80[];

extern MovieMenuState *mnuMovieMenuState;

KwlnTask *mnuCreateTitleMenuTask(s32 mode) {
    struct SdfMemBlock *allocation;

    kwlnFadeBackgroundStartOut(0);
    mnuStopMovieDrawTask();
    allocation = sdfAllocGeneralBlock(0x40);
    mnuMovieMenuState = (void *)sdfResourceRetainAddress(allocation);
    memset(mnuMovieMenuState, 0, 0x40);
    mnuMovieMenuState->allocation = allocation;
    mnuRecreateMenuSelectionList();
    mnuMovieMenuState->state = 0;
    mnuMovieMenuState->cursor = 0;

    if (mode == 0) {
        mnuSelectMenuListCursorByAdvance(0);
    } else if (mode == 1) {
        mnuSelectMenuListCursorByAdvance(2);
    } else if (mode == 2) {
        mnuSelectMenuListCursorByAdvance(0);
    }

    mnuMovieMenuState->mode = mode;
    mnuLoadTitleMenuAssets();
    mnuMovieMenuState->resources = mnuCreateMovieSpriteResource(0x3C, 8, 0);
    D_003BA8EC = 0x80000000;
    return kwlnTaskCreate(D_003AFD80, 0x2B19, 1, 0, mnuUpdateMovieMenuTask, 0, 0);
}

INCLUDE_RODATA(const s32, "game/code_0026B050", D_003AFD80);

INCLUDE_SDATA(const s32, "game/code_0026B050", mnuMovieMenuState);

