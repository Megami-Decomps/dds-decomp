#ifndef DDS_SND_PAD_H
#define DDS_SND_PAD_H

#include "common.h"

/* Named primary/secondary buttons are ports 0/1 of input bank 1. */
typedef struct SndPad {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            s8 primaryTriangle;
            s8 primaryCancel;
            s8 alternatePrev; /* +0x24: battle panels use as an alternate previous input. */
            s8 alternateNext; /* +0x25: battle panels use as an alternate next input. */
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 secondaryCross;
            s8 secondaryTriangle;
            s8 secondaryCancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 secondaryUp;
            s8 secondaryDown;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
} SndPad;

typedef char SndPadSizeCheck[(sizeof(SndPad) == 0x40) ? 1 : -1];
typedef char SndPadButtonsOffsetCheck[((u32)&((SndPad *)0)->buttons == 0x20) ? 1 : -1];
typedef char SndPadConfirmOffsetCheck[((u32)&((SndPad *)0)->confirm == 0x21) ? 1 : -1];
typedef char SndPadPrimaryCancelOffsetCheck[((u32)&((SndPad *)0)->primaryCancel == 0x23) ? 1 : -1];
typedef char SndPadPrevOffsetCheck[((u32)&((SndPad *)0)->prev == 0x26) ? 1 : -1];
typedef char SndPadNextOffsetCheck[((u32)&((SndPad *)0)->next == 0x27) ? 1 : -1];
typedef char SndPadSecondaryCancelOffsetCheck[((u32)&((SndPad *)0)->secondaryCancel == 0x33) ? 1 : -1];
typedef char SndPadCoarseDownOffsetCheck[((u32)&((SndPad *)0)->coarseDown == 0x34) ? 1 : -1];
typedef char SndPadCoarseUpOffsetCheck[((u32)&((SndPad *)0)->coarseUp == 0x35) ? 1 : -1];
typedef char SndPadFineDownOffsetCheck[((u32)&((SndPad *)0)->fineDown == 0x38) ? 1 : -1];
typedef char SndPadFineUpOffsetCheck[((u32)&((SndPad *)0)->fineUp == 0x3A) ? 1 : -1];

#endif
