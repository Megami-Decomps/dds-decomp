#include "common.h"
#include "evt_message_window_api.h"
#include "dsp_channel.h"
#include "menu_slot_flag_event_view.h"


/* Reopen the message window over the terminal scene's loaded message data. */
void evtCloseDisplayChannelAndEnsureMessageWindow(MenuSlotFlagEventView *context) {
    fclDestroyMessage();
    fclInitMessage((struct ItfMesSub *)context->messageResources.dataAddress);
}
