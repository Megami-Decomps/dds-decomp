#ifndef PTY_READ_ROSTER_STAT_API_H
#define PTY_READ_ROSTER_STAT_API_H

#include "common.h"
#include "dat_stat.h"

/* Read one signed base-stat value from the first active roster record. */
s8 ptyReadSignedRosterStatByte(DatBaseStatIndex statIndex);

#endif /* PTY_READ_ROSTER_STAT_API_H */
