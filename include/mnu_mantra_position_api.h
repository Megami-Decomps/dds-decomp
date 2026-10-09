#ifndef MNU_MANTRA_POSITION_API_H
#define MNU_MANTRA_POSITION_API_H

#include "common.h"

struct MantraNodePos;

/* Return a panel-position record by its signed 16-bit table index. */
struct MantraNodePos *mnuGetMantraPanelPositionRecord(s16 index);

#endif
