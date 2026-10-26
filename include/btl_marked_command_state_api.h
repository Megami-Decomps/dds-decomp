#ifndef BTL_MARKED_COMMAND_STATE_API_H
#define BTL_MARKED_COMMAND_STATE_API_H

#include "btl_marked_command_state.h"

/* Both queries return INACTIVE when mode 0x314 or its state block is absent. */
BtlMarkedCommandState btlGetMarkedCommandCurrentState(void);
BtlMarkedCommandState btlGetMarkedCommandRequestedState(void);

#endif /* BTL_MARKED_COMMAND_STATE_API_H */
