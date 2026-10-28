#ifndef DDS_POLY_ACTIVITY_STATE_H
#define DDS_POLY_ACTIVITY_STATE_H

#include "common.h"

/* Native activity fields are 16-bit words; zero disables updates and any
 * nonzero value keeps the spawner active. Preserve that full state domain. */
typedef u16 PolyActivityState;

enum {
    POLY_ACTIVITY_INACTIVE = 0
};

#endif
