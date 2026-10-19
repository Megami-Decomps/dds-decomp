#include "common.h"
#include "dsp_channel.h"

extern s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *definition);

/* Pass the object's +0x60 word to the second operation after setup. */
void evtCloseDisplayChannelAndEnsureMessageWindow(void *context) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(*(u32 *)((u8 *)context + 0x60));
}
