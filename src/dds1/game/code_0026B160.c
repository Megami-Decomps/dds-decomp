#include "common.h"

extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern char D_003AFD80[]; /* "titleProc" */

s32 mnuApplyInnerEffectVectorsAndTickObject(void);

typedef struct MovieMenuState {
    u32 handle;     /* 0x00 */
    u8 pad04[0x2C];
    u32 resources;  /* 0x30 */
} MovieMenuState;

extern MovieMenuState *mnuMovieMenuState;
extern s32 D_003BA730;
extern void mnuStopMovieDrawTask(void);
extern void mnuReleaseMenuResourceSlots(void);
extern void mnuReleaseSpriteHandle(void);
extern void mnuDestroyMovieMenuSelectionList(void);
extern void mnuReleaseMovieResourceGroup();
extern void mnuMovieShutdownA(void);
extern void sdfReleaseResourceAllocation(u32);

void func_0026B160(void) {
    mnuStopMovieDrawTask();
    mnuReleaseMenuResourceSlots();
    mnuReleaseSpriteHandle();
    mnuDestroyMovieMenuSelectionList();
    mnuReleaseMovieResourceGroup(mnuMovieMenuState->resources);
    mnuMovieShutdownA();
    sdfReleaseResourceAllocation(mnuMovieMenuState->handle);
    mnuMovieMenuState = 0;
    D_003BA730 = 1;
}

extern void func_0026CB10(void *, void *);

s32 func_0026B1C0(void) {
    func_0026CB10(D_003DC1C0, D_003DC1D0);
    return mnuApplyInnerEffectVectorsAndTickObject();
}

INCLUDE_ASM(const s32, "game/code_0026B160", func_0026B1F0);

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
