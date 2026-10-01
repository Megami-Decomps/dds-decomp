#include "common.h"

extern void mdlFlagSet(s32);

/* One event slot: the flag word the slot owns followed by the rest of it. */
typedef struct EvtSlot {
    s32 flag; /* 0x00 */
    u8 pad04[0x44];
} EvtSlot;

extern EvtSlot D_003CE1AC[];

/* Set the model flag owned by the given slot. */
void mnuSetFlagForMenuEntry(s32 index) {
    s32 flag;

    flag = D_003CE1AC[index].flag;
    if (flag != 0) {
        mdlFlagSet(flag);
    }
}
