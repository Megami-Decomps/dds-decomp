#ifndef FLD_INDEX_PAIR_H
#define FLD_INDEX_PAIR_H

#include "common.h"

/* A row of the field's shared index tables, whose second halfword is the
 * model flag the row requires. DDS1 and DDS2 share the layout. */
typedef struct {
    s16 unk0;
    s16 requiredModelFlag; /* Shared table halfword tested by mdlFlagTest. */
} FldIndexPair;

#endif
