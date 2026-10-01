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

extern MnuTransRec *mnuAppendNodeToDisplayList(MnuTransHead *head);
extern u32 *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void func_00253558(MnuTransWork *work);

void mnuBeginTransitionAlt(MnuTransWork *work, s32 mode) {
    MnuTransRec *transition = mnuAppendNodeToDisplayList(&work->list590);

    if (transition != NULL) {
        if (work->state >= 6) {
            mnuStopResourceAnimation();
            func_00253558(work);
        } else if (work->state < -5) {
            mnuResetResourceAnimation();
            func_00253558(work);
        }
        transition->mode = mode;
        if (mode == 1) {
            work->state = 10;
            transition->startId = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
            transition->endId = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->state = -10;
            transition->startId = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            transition->endId = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
        }
    }
}
