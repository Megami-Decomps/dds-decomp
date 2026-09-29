#include "common.h"

typedef struct MnuTransRec {
    u8 unk0[4];   /* 0x0 */
    s32 startId;  /* 0x4 */
    s32 endId;    /* 0x8 */
    s16 mode;     /* 0xC */
} MnuTransRec;

typedef struct MnuTransHead {
    u8 unk0[8];         /* 0x0 */
    MnuTransRec *first; /* 0x8 */
} MnuTransHead;

typedef struct MnuTransWork {
    u8 unk0[0x550];       /* 0x0 */
    s32 state;            /* 0x550: |state| > 5 triggers a resource reset */
    u8 unk554[0x30];      /* 0x554 */
    MnuTransHead list584; /* 0x584 */
    MnuTransHead list590; /* 0x590 */
} MnuTransWork;

extern MnuTransRec *mnuAppendDisplayListNode(MnuTransHead *head);
extern u32 *func_0024FA18(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void func_00253520(MnuTransWork *work);

void mnuBeginTransition(MnuTransWork *work, s32 mode) {
    MnuTransRec *transition = mnuAppendDisplayListNode(&work->list584);

    if (transition != NULL) {
        if (work->state >= 6) {
            mnuStopResourceAnimation();
            func_00253520(work);
        } else if (work->state < -5) {
            mnuResetResourceAnimation();
            func_00253520(work);
        }
        transition->mode = mode;
        if (mode == 1) {
            work->state = 10;
            transition->startId = *func_0024FA18();
            mnuStopResourceAnimation();
            transition->endId = *func_0024FA18();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->state = -10;
            transition->startId = *func_0024FA18();
            mnuResetResourceAnimation();
            transition->endId = *func_0024FA18();
            mnuStopResourceAnimation();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250B60);


