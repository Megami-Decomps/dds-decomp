#include "common.h"
#include "dat_state.h"

/* Query the fallback panel record with both filters unrestricted. The native
 * tail call forwards the selector's V0; retain the existing legacy declarations. */
void mnuGetDefaultMantraPanelRecord(u32 object) {
    mnuFindPendingMantraPanelRecord(object, 0xffffffffffffffff, 0xffffffffffffffff);
}
