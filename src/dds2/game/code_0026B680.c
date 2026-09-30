#include "common.h"

/* Pass the object's +0x60 word to the second operation after setup. */
void func_0026B680(s32 objectAddress) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(*(u32 *)(objectAddress + 0x60));
}
