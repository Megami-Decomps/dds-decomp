#include "mnu.h"

extern void mdlFlagSet(s32);


/* Set the model flag owned by the given slot. */
void mnuSetFlagForMenuEntry(s32 index) {
    s32 flag;

    flag = D_003CE1A8[index].flag;
    if (flag != 0) {
        mdlFlagSet(flag);
    }
}
