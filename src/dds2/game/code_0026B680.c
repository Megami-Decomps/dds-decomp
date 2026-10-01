#include "common.h"

/* Pass the object's +0x60 word to the second operation after setup. */
void evtCloseDisplayChannelAndEnsureMessageWindow(s32 objectAddress) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(*(u32 *)(objectAddress + 0x60));
}
