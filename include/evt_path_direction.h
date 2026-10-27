#ifndef EVT_PATH_DIRECTION_H
#define EVT_PATH_DIRECTION_H

#include "common.h"

/* The counter advances toward duration in FORWARD mode and toward zero in REVERSE mode. */
typedef u32 EvtPathDirection;
enum {
    EVT_PATH_DIRECTION_FORWARD = 0,
    EVT_PATH_DIRECTION_REVERSE = 1
};

#endif /* EVT_PATH_DIRECTION_H */
