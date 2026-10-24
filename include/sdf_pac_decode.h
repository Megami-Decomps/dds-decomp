#ifndef SDF_PAC_DECODE_H
#define SDF_PAC_DECODE_H

#include "sdf_pac_state.h"

/* Values returned by sdfPacDecode and stored in PacState.phase. */
enum {
    SDF_PAC_PHASE_COMPLETE = -1,
    SDF_PAC_PHASE_CALLBACK_BOUNDARY = 0,
    SDF_PAC_PHASE_SKIP_BYTES = 1,
    SDF_PAC_PHASE_INPUT_HANDLER = 2
};

#endif /* SDF_PAC_DECODE_H */
