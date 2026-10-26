#ifndef BTL_MARKED_COMMAND_STATE_H
#define BTL_MARKED_COMMAND_STATE_H

#include "common.h"

/* The API returns this word-sized value; the native mode state block stores bytes. */
typedef u32 BtlMarkedCommandState;

/* Active mode remaps the marked actor to alternate action IDs. */
enum {
    BTL_MARKED_COMMAND_STATE_INACTIVE = 0,
    BTL_MARKED_COMMAND_STATE_ACTIVE = 1,
};

#endif /* BTL_MARKED_COMMAND_STATE_H */
