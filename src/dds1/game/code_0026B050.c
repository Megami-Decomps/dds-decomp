#include "common.h"
#include "kwln.h"

typedef struct MovieMenuState {
    s32 allocation;   /* 0x00 */
    u8 pad04[0x0C];
    s32 state;         /* 0x10 */
    s32 cursor;        /* 0x14 */
    s32 mode;          /* 0x18 */
    u8 pad1C[0x14];
    void *resources;   /* 0x30 */
    u8 pad34[0x0C];
} MovieMenuState;

extern void kwlnFadeBackgroundStartOut(s32);
extern void mnuStopMovieDrawTask(void);
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void *memset(void *, s32, u32);
extern void mnuRecreateMenuSelectionList(void);
extern void mnuSelectMenuListCursorByAdvance(s32);
extern void func_0026AEB0(void);
extern void *mnuCreateMovieSpriteResource(s32, u8, u8);
extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);
extern s32 func_0026B1F0(KwlnTask *);
extern u32 D_003BA8EC;
extern char D_003AFD80[];

extern MovieMenuState *mnuMovieMenuState;

KwlnTask *func_0026B050(s32 mode) {
    s32 allocation;

    kwlnFadeBackgroundStartOut(0);
    mnuStopMovieDrawTask();
    allocation = sdfAllocGeneralBlock(0x40);
    mnuMovieMenuState = sdfResourceRetainAddress(allocation);
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
    func_0026AEB0();
    mnuMovieMenuState->resources = mnuCreateMovieSpriteResource(0x3C, 8, 0);
    D_003BA8EC = 0x80000000;
    return kwlnTaskCreate(D_003AFD80, 0x2B19, 1, 0, func_0026B1F0, 0, 0);
}

INCLUDE_RODATA(const s32, "game/code_0026B050", D_003AFD80);

INCLUDE_SDATA(const s32, "game/code_0026B050", mnuMovieMenuState);
