#include "common.h"

extern u32 D_003BC5BC;


extern u8 D_003BC590[];

extern u8 D_003BC598[];


extern s32 evtDestroySecondaryWorldNode(void);

extern u8 D_003BC5A0[];

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern s8 D_003BC58C;

extern s32 D_003BC5A8;

extern s32 D_003BC5AC;


/* The title-menu callback passes zero; the restart does not use it. */
void mnuRestartRuntimeAfterViewer(s32 unused) {
    evtDestroySecondaryWorldNode();
    sdfDestroyRuntimeTask();
    sdfCreateRuntimeTask();
}

