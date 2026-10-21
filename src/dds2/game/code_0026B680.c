#include "common.h"
#include "evt_message_window_api.h"
#include "dsp_channel.h"


/* Pass the object's +0x60 word to the second operation after setup. */
void evtCloseDisplayChannelAndEnsureMessageWindow(void *context) {
    dspCloseChannel();
    evtCreateMessageWindowIfMissing((struct ItfMesSub *)*(u32 *)((u8 *)context + 0x60));
}
